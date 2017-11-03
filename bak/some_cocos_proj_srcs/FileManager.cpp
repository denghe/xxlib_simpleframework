#include "Precompile.h"
#include "cJSON.h"


MAKE_INSTANCE_CPP( FileManager );



FileManager::FileManager()
    : _running( true )
    , _disposed( true )
    , _state( FileManagerState::None )
    , _load_imageFilesCounter( 0 )
    , _load_resFilesCounter( 0 )
    , _lua_eh_loadIndex( 0 )
    , _lua_eh_download( 0 )
    , _lua_eh_load( 0 )
    , _asyncLoading( false )
{
    for( auto &d : _downloaders )
    {
        d.setFileIndexRowMutex( &_fileIndexRowMutex );
    }
    thread( &FileManager::process, this ).detach();
}

FileManager::~FileManager()
{
    _running = false;
    while( !_disposed ) sleepMS( 50 );
    vector<FileIndexRow*> tmp;
    for( auto& o : _loadLog )
    {
        tmp.push_back( o.first );
    }
    for( auto& o : tmp )
    {
        unloadCore( o );
    }
}

void FileManager::registerLoadIndexEventHandler( EventHandlerType eh )
{
    _eh_loadIndex = eh;
}

void FileManager::registerLoadIndexEventHandler( LUA_FUNCTION eh )
{
    _lua_eh_loadIndex = eh;
}

void FileManager::registerLoadEventHandler( EventHandlerType eh )
{
    _eh_load = eh;
}

void FileManager::registerLoadEventHandler( LUA_FUNCTION eh )
{
    _lua_eh_load = eh;
}

void FileManager::registerDownloadEventHandler( EventHandlerType eh )
{
    _eh_download = eh;
}

void FileManager::registerDownloadEventHandler( LUA_FUNCTION eh )
{
    _lua_eh_download = eh;
}


bool FileManager::loadIndex( string const & fileName, string const & baseURL )
{
    if( !_eh_loadIndex && !_lua_eh_loadIndex )
    {
        CCLOG( "%s", "FileManager::loadIndex        need registerLoadIndexEventHandler !" );
        return false;
    }
    {
        lock_guard<mutex> g( _stateMutex );
        if( _state != FileManagerState::None )
        {
            CCLOG( "%s", "FileManager::loadIndex        _state != None !" );
            return false;
        }
        _state = FileManagerState::IndexLoading;
    }
    _loadIndex_fileName = fileName;
    _baseURL = baseURL;

    return true;
}

void FileManager::download( vector<string> const & files )
{
    for( auto &fn : files )
    {
        if( auto row = getFileIndexRow( fn ) )                       // 对应到文件索引
        {
            lock_guard<mutex> g( _fileIndexRowMutex );
            if( row->state == FileStates::NeedDownload
                || row->state == FileStates::CRCNeedDownload
                || row->state == FileStates::NeedChecksum
                || row->state == FileStates::NeedExtract )
            {
                _download_files.push( row );
            }
        }
        else
        {
            CCLOG( "%s%s", "FileManager::download       getFileIndexRow( fn )     can't find fileName: ", fn.c_str() );
        }
    }

    // 后台下载相关回调将于 loadIndex 成功后注册
}

bool FileManager::load( vector<string> const & files )
{
    {
        lock_guard<mutex> g( _stateMutex );
        if( !_eh_load && !_lua_eh_load )
        {
            CCLOG( "%s", "FileManager::load     need registerLoadEventHandler !" );
            return false;
        }
        if( _state == FileManagerState::None )
        {
            CCLOG( "%s", "FileManager::load     _state == None !" );
            return false;
        }
        if( _state == FileManagerState::IndexLoading )
        {
            CCLOG( "%s", "FileManager::load     _state == IndexLoading !" );
            return false;
        }
        if( _state == FileManagerState::Loading )       // 已经在执行了
        {
            CCLOG( "%s", "FileManager::load     _state == Loading !" );
            return false;
        }
    }

    _load_files.clear();

    for( auto &fn : files )
    {
        if( auto row = getFileIndexRow( fn ) )
        {
            _load_files.push_back( row );

            lock_guard<mutex> g2( _fileIndexRowMutex );
            if( row->state == FileStates::NeedDownload
                || row->state == FileStates::CRCNeedDownload
                || row->state == FileStates::NeedChecksum
                || row->state == FileStates::NeedExtract )
            {
                _load_needDownloadFiles.push( row );
            }
        }
        else
        {
            log( "%s%s", "FileManager::load       getFileIndexRow( fn )     can't find fileName: ", fn.c_str() );
            printf( "%s%s", "FileManager::load       getFileIndexRow( fn )     can't find fileName: ", fn.c_str() );
            
        }
    }

    setState( FileManagerState::Loading );
    return true;
}

void FileManager::unloadCore( FileIndexRow* fir )
{
    // loadLog 反向操作

    auto it = _loadLog.find( fir );
    if( it == _loadLog.end() )
    {
        CCLOG( "unload error, loadLog can't find fir = %s", fir->name.c_str() );
        return;
    }
    it->second--;
    if( it->second )
    {
        return;
    }
    _loadLog.erase( it );

    auto tc = Director::getInstance()->getTextureCache();
    switch( fir->type )
    {
    case FileTypes::Jpg:            // 几种单图类型，只要贴图有引用就不移除
    case FileTypes::Png:
    case FileTypes::Pvr:
    case FileTypes::Ccz:
        tc->tryRemoveTextureForKey( fir->name );
        break;
    case FileTypes::FramePlist:     // 先扫 Texture 对应的 SpriteFrames 是否还存在引用，有就不删。没有就一起删
        if( SpriteFrameCache::getInstance()->tryRemoveSpriteFramesFromFile( fir->name ) )
        {
            tc->tryRemoveTextureForKey( fir->name );
        }
        break;
    case FileTypes::Wav:
    case FileTypes::Mp3:
    case FileTypes::Caf:
    case FileTypes::Ogg:
        SimpleAudioEngine::getInstance()->unloadEffect( fir->name.c_str() );
        break;
    case FileTypes::BgWav:
    case FileTypes::BgMp3:
    case FileTypes::BgCaf:
    case FileTypes::BgOgg:
        SimpleAudioEngine::getInstance()->stopBackgroundMusic( true );
        break;
    default:
        break;
    }

}


void FileManager::unloadCore( vector<FileIndexRow*> & firs )
{
    for( auto &fir : firs )
    {
        unloadCore( fir );
    }
}

bool FileManager::unload( vector<string> const & files )
{
    if( _state == FileManagerState::None )
    {
        CCLOG( "%s", "FileManager::download     _state == None !" );
        return false;
    }
    if( _state == FileManagerState::IndexLoading )
    {
        CCLOG( "%s", "FileManager::download     _state == IndexLoading !" );
        return false;
    }
    if( _state == FileManagerState::Loading )
    {
        CCLOG( "%s", "FileManager::download     _state == Loading !" );
        return false;
    }

    for( auto &fn : files )
    {
        if( auto fir = getFileIndexRow( fn ) )
        {
            unloadCore( fir );
        }
        else
        {
            CCLOG( "%s%s", "FileManager::unload       getFileIndexRow( fn )     can't find fileName: ", fn.c_str() );
        }
    }
    return true;
}





void FileManager::process()
{
    function<void()> f;

    while( _running )       // 死循环直到通知退出
    {
        {
            lock_guard<mutex> g( _stateMutex );

            // 路由要执行的函数体
            if( _state == FileManagerState::None )      // 未初始化时先空转
            {
                goto Lab_Sleep;
            }
            else if( _state == FileManagerState::IndexLoading )
            {
                f = [ this ] { loadIndex(); };
            }
            else if( _state == FileManagerState::Downloading )
            {
                f = nullptr;                            // 后台下载经由 load 函数事后恢复
                //f = [ this ] { download(); };
            }
            else
            {
                f = [ this ] { load(); };
            }
        }

        if( f )
        {
            f();
            continue;
        }

Lab_Sleep:
        sleepMS( 50 );
        continue;
    }
    _disposed = true;       // 成功退出循环时标记为已退出
}

// db ver 结构
struct Ver
{
    int ver;
    int len;
    string md5;

    // 从文件读入属性, 读失败将返回 false, 成功返回 true
    inline bool readFromJson( string const & fn )
    {
        // 读取 ver 的内容
        auto s = Utils::fileGetText( fn );

        // 转为 json 体
        auto cj = cJSON_Parse( s.c_str() );

        // 失败退出
        if( !cj ) return false;

        // 确保回收 cj
        Utils::ScopeGuard autoDeleteCJ( [ cj ] { cJSON_Delete( cj ); } );

        // 开始一系列的读取
        if( auto cj_item = cJSON_GetObjectItem( cj, "ver" ) )
            ver = cj_item->valueint;
        else
            return false;

        if( auto cj_item = cJSON_GetObjectItem( cj, "md5" ) )
            md5 = cj_item->valuestring;
        else
            return false;

        if( auto cj_item = cJSON_GetObjectItem( cj, "len" ) )
            len = cj_item->valueint;
        else
            return false;

        // 成功返回
        return true;
    }
};

void FileManager::loadIndex()
{
    // 大流程:
    // 如果 运行时 db  未找到, 则 于可写目录 创建空表
    // 加载 运行时 db
    // 判断 包内 db .ver 文件的内容, 读出 包内 db 的版本号
    // 如果 包内 db 版本号 比运行时 db 的大, 则 将包内 db 复制到 可写目录, load 并 令 运行时 db.upgrade( 包内 db )
    // 下载 web db ver 文件到可写目录, 读出 web db 的版本号
    // 如果 web db 版本号 比运行时 db 的大, 则 load 并 令 运行时db.upgrade( web db )
    // 运行时 db 填充完整文件名

    // 补充: .ver 文件, 是一个 json, 含有 ver ( db 的版本号 ), md5( 下载后的校验用 ), len( 下载长度参考 ) 三个属性

    // 准备工作
    //

    // 引用到文件系统
    auto fu = FileUtils::getInstance();

    // 运行时db 完整文件名
    auto fn_rt = Global::WritablePath + _loadIndex_fileName;
    // web db 完整文件名
    auto fn_web = fn_rt + ".web";
    // web db ver 完整文件名
    auto fn_web_ver = fn_rt + ".ver.web";
    // 复制出来的 pkg db 完整文件名
    auto fn_pkg = fn_rt + ".pkg";
    // pkg db 完整文件名
    auto fn_ori = Global::ResourcePath + _loadIndex_fileName;
    // pkg db ver 完整文件名
    auto fn_ori_ver = fn_ori + ".ver";
    // web db 下载路径
    auto url_web = _baseURL + _loadIndex_fileName;
    // web db ver 下载路径
    auto url_web_ver = url_web + ".ver";

    // 报错并重置状态的函数
    auto err = [ = ]( string const & s )
    {
        setState( FileManagerState::None );
        _events_loadIndex.push( FileEvent( FileEventTypes::LoadIndex_LoadError, s ) );
    };


    // 设置 runtime db 文件路径
    _fileIndex.setFileName( fn_rt );

    // 注: 创建空表方式因为在 android 下无法开启事务 导致插入缓慢故弃用
    // 网上有个说法是 String sqliteDir = "/data/data/" + getApplicationContext().getPackageName() + "/databases";
    // 当前是在 .......files/data/ 下面创建的文件, 不知道改成 .......databases/  会不会修复

    // 检查文件是否存在
    // 不存在则从包内复制一份
    if( !fu->isFileExist( fn_rt ) && !Utils::fileCopy( fn_ori, fn_rt ) )
    {
        err( "Utils::fileCopy( fn_ori, fn_rt ) error" );
        return;
    }

    // 载入运行时 db
    if( !_fileIndex.load() )
    {
        err( "_fileIndex.load() error" );
        return;
    }

    // 读 包内 db .ver 文件的内容
    Ver ver_pkg = { 0, 0 };
    if( !ver_pkg.readFromJson( fn_ori_ver ) )
    {
        err( "ver_pkg.readFromJson( fn_ori_ver ) error" );
        return;
    }

    // 包内 db 比运行时 db 新( 升级过程序 )
    if( ver_pkg.ver > _fileIndex.getVersion() )
    {
        // 将包内db 复制到可写区( 因为 android 的db在 zip 内无法改变, 当前还未集成 VFS 的 sqlite 打开方式 )
        if( !Utils::fileCopy( fn_ori, fn_pkg ) )
        {
            err( "Utils::fileCopy( fn_ori, fn_pkg ) error" );
            return;
        }

        // 载入 已复制的 pkg db
        FileIndex fi_pkg( fn_pkg );
        if( !fi_pkg.load() )
        {
            err( "fi_pkg.load() error" );
            return;
        }

        // 升级运行时 db
        if( !_fileIndex.upgrade( fi_pkg, true, ver_pkg.ver ) )
        {
            err( "_fileIndex.upgrade( fi_pkg, true, ver_pkg.ver ) error" );
            return;
        }
    }


    // 开始下载 web db ver
    // 声明公用下载任务上下文
    FileDownloadTask dt;
    FileIndexRow fir;
    dt.fileIndexRow = &fir;
    Ver ver_web = { 0, 0 };

    // 引用一个下载器
    auto &d = _downloaders[ 0 ];

    // 用于存放当前下载事件类型( 以这种方式来通知 下面的 while 等待块 作出相应的行为 )
    auto et = (FileEventTypes)0;

    // 如果开放了下载
    if( _baseURL.size() )
    {
        // 开始
        _events_loadIndex.push( FileEvent( FileEventTypes::LoadIndex_Started
            , _baseURL, _loadIndex_fileName ) );

        // 准备下载 web db

        // 初始化下载器
        d.registerDownloadEventHandler( [ &]( FileEvent && e )      // 注册下载事件回调
        {
            et = e.eventType;                                       // 设当前事件类型
            _events_loadIndex.push( move( e ) );
        } );

        // 删旧 ver 文件
        Utils::fileDelete( fn_web_ver );

        // 伪造 FileindexRow for 下载 ver
        fir.name = fn_web_ver;
        fir.fullName = fn_web_ver;              // 指定写盘文件名
        fir.size = 0;                           // 长度未知
        fir.crc32 = 0;                          // 不校验
        fir.md5 = "";                           // 不校验
        fir.state = FileStates::NeedDownload;   // 需要下载
        dt.fullURL = url_web_ver;               // 指定下载文件名
        dt.currentDownloadSize = 0;             // 已下载 size 初始化为 0
        dt.originalFileSize = 0;                // 文件已删, 当前尺寸为 0

        // 开始下载
        _events_loadIndex.push( FileEvent( FileEventTypes::LoadIndex_DownloadBegin
            , fir.name, dt.fullURL ) );
        d.download( dt );

        // 等下载结束
        // 非错误事件都需要继续等待 后期可能扩展 cancel. 可加条件退出
        while( !( et >= FileEventTypes::Download_Finished
            && et < FileEventTypes::Download_Error_End ) )
        {
            if( !_running ) return;
            sleepMS( 50 );
        }

        // 如果下载出错, 退出
        if( et != FileEventTypes::Download_Finished )
        {
            setState( FileManagerState::None );
            return;
        }

        // 下载成功
        _events_loadIndex.push( FileEvent( FileEventTypes::LoadIndex_DownloadFinished
            , fir.name, dt.fullURL ) );

        // 开始取 ver 的内容
        if( !ver_web.readFromJson( fn_web_ver ) )
        {
            err( "ver_web.readFromJson( fn_web_ver ) err" );
            return;
        }

        if( ver_web.ver > _fileIndex.getVersion() )  // web db 版本比本地新, 需要下载
        {
            // 删旧文件
            Utils::fileDelete( fn_web );

            // 构造下载任务
            fir.name = fn_web;
            fir.fullName = fn_web;
            fir.size = ver_web.len;
            fir.crc32 = 0;
            fir.md5 = ver_web.md5;
            fir.state = FileStates::NeedDownload;
            dt.fullURL = url_web;
            dt.currentDownloadSize = 0;
            dt.originalFileSize = 0;

            // 清之前的事件
            et = (FileEventTypes)0;

            // 开始下载
            _events_loadIndex.push( FileEvent( FileEventTypes::LoadIndex_DownloadBegin
                , fir.name, dt.fullURL ) );
            d.download( dt );

            // 等下载结束
            // 非错误事件都需要继续等待 后期可能扩展 cancel. 可加条件退出
            while( !( et >= FileEventTypes::Download_Finished
                && et < FileEventTypes::Download_Error_End ) )
            {
                if( !_running ) return;
                sleepMS( 50 );
            }

            // 如果下载出错, 退出
            if( et != FileEventTypes::Download_Finished )
            {
                setState( FileManagerState::None );
                return;
            }

            // 下载成功
            _events_loadIndex.push( FileEvent( FileEventTypes::LoadIndex_DownloadFinished
                , fir.name, dt.fullURL ) );

            // 加载 web db
            FileIndex fi_web( fn_web );
            if( !fi_web.load() )
            {
                err( "fi_web.load() err" );
                return;
            }

            // 升级
            if( !_fileIndex.upgrade( fi_web, false, ver_web.ver ) )
            {
                err( "_fileIndex.upgrade( fi_web, false, ver_web.ver )" );
                return;
            }
        }
    }

    // 填充完整文件名
    _fileIndex.fillFullName();

    // 完成: 设状态为 Downloading
    setState( FileManagerState::Downloading );
    _events_loadIndex.push( FileEvent( FileEventTypes::LoadIndex_Finished ) );


    // 注册后台下载器的提供与事件回调
    download();
}



void FileManager::download()
{
    // 声明公用下载任务上下文
    auto d = &_downloaders[ 0 ];         // 指针指向 [0] 下载器 方便复制到 lambda

    auto ts = [ this, d ]
    {
        // 不断的从 _download_files 获取任务
        FileIndexRow* fir;
        if( _download_files.pop( fir ) )
        {
            lock_guard<mutex> lg( _fileIndexRowMutex );
            if( fir->state == FileStates::Finished ) return;
            d->download( FileDownloadTask( _baseURL + fir->name, fir ) );
        }
    };

    auto cb = [ this, d ]( FileEvent && e )
    {
        // 如果 e 为报错, 将任务重新放回 _download_files
        if( e.eventType > FileEventTypes::Download_Error_Begin
            && e.eventType < FileEventTypes::Download_Error_End )
        {
            _download_files.push( (FileIndexRow*)e.userData );
        }
        if( e.eventType == FileEventTypes::Download_Finished )
        {
            auto fir = (FileIndexRow*)e.userData;               // 下完之后存db
            lock_guard<mutex> lg( _fileIndexRowMutex );
            fir->state = FileStates::Finished;
            _fileIndex.dbUpdateState( fir );
        }
        // 将 e 移进事件队列
        _events_download.push( move( e ) );
    };

    d->registerSupplierAndHandler( ts, cb );
}



void FileManager::load()
{
    // 大体流程:
    // 向 先前的后台下载器 下达 stop 指令, 令其 尽快停止当前下载行为
    // 将所有下载器的 任务提供者切换为从 load 列表下载, 事件压到 load 事件队列
    // 当某一个下载器 发生错误时, 将下载任务 放回 load 列表, 且清除该下载器的 提供者, 不再继续下载其他任务
    // 等待所有下载器, 通通变为 未下载 态,
    // 之后检查 load 列表 是否已经为空, 要 load 的这批文件的 FileState 是否都是 Finished
    // 条件满足 则进入 异步加载( 这阶段如果出错就是致命的了 )
    // 条件不满足 则报错 网络不给力之类
    // 异步加载, 根据文件类型来路由相应方式.
    // 小文件缓存. 最后退出


    // 下面这步计算提前, 以便 Load_Started 时向 lua 输出 各步骤的文件数

    // 清完成计数器
    _load_imageFilesCounter = 0;
    vector<FileIndexRow*> firs_async;       // 临时容器 for 异步加载

    // 扫出需要让 TextureCache  addImageAsync 的文件
    for( auto fir : _load_files )
    {
        if( fir->type > FileTypes::TextureTypes_Begin && fir->type < FileTypes::TextureTypes_End )
        {
            firs_async.push_back( fir );
        }
    }


    // 清完成计数器
    _load_resFilesCounter = 0;
    vector<FileIndexRow*> firs_sync;        // 临时容器 for 同步加载


    // 扫出需要同步加载的文件
    for( auto fir : _load_files )
    {
        if( fir->type != FileTypes::FramePlist
            && !( fir->type > FileTypes::SoundTypes_Begin && fir->type < FileTypes::SoundTypes_End )
            ) continue;
        firs_sync.push_back( fir );
    }

    // 开始事件, 输出 总文件数, 要下载的文件个数, 要异步加载的贴图个数, 要同步加载的资源个数
    _events_load.push( FileEvent( FileEventTypes::Load_Started
        , (int)_load_files.size(), (int)_load_needDownloadFiles.size(), (int)firs_async.size(), (int)firs_sync.size() ) );

    // 先判断是否需要下载
    if( !_load_needDownloadFiles.empty() )
    {
        // 算总字节数
        int totalSize = 0;
        _load_needDownloadFiles.foreach( [ &totalSize ]( FileIndexRow*& o ) { totalSize += o->size; } );
        _events_load.push( FileEvent( FileEventTypes::Load_DownloadInit, totalSize ) );

        int numFiles = _load_needDownloadFiles.size();
        atomic<int> finishCounter( 0 );
        atomic<int> errorCounter( 0 );
        for( auto &downloader : _downloaders )
        {
            // 取指针来用, 方便复制到 lambda
            auto d = &downloader;
            auto ts = [ this, d ]
            {
                // 不断的从 _load_files 获取任务
                FileIndexRow* fir;
                if( _load_needDownloadFiles.pop( fir ) )
                {
                    d->download( FileDownloadTask( _baseURL + fir->name, fir ) );
                }
            };
            auto cb = [ this, d, &errorCounter, &finishCounter ]( FileEvent && e )
            {
                // 如果 e 为报错, 当前下载器终止 获取任务, 归还当前下载任务 并退出( 错误计数 +1 )
                if( e.eventType > FileEventTypes::Download_Error_Begin
                    && e.eventType < FileEventTypes::Download_Error_End )
                {
                    d->registerSupplierAndHandler( nullptr, nullptr );
                    _load_needDownloadFiles.push( (FileIndexRow*)e.userData );
                    ++errorCounter;
                }
                else if( e.eventType == FileEventTypes::Download_Finished )
                {
                    ++finishCounter;        // 累加计数器, 存 db
                    auto fir = (FileIndexRow*)e.userData;
                    lock_guard<mutex> lg( _fileIndexRowMutex );
                    fir->state = FileStates::Finished;
                    _fileIndex.dbUpdateState( fir );
                }
                // 将 e 移进事件队列
                _events_load.push( move( e ) );
            };
            d->registerSupplierAndHandler( ts, cb );
        }

        _events_load.push( FileEvent( FileEventTypes::Load_DownloadBegin ) );

        // 等待下载完成或退出
        while( true )
        {
            if( errorCounter >= _countof( _downloaders ) )
            {
                for( auto &downloader : _downloaders )
                {
                    // 解除 downloader 的工作
                    downloader.registerSupplierAndHandler( nullptr, nullptr );
                }

                setState( FileManagerState::Downloading );
                _events_load.push( FileEvent( FileEventTypes::Load_DownloadFailed ) );

                // 因失败而退出
                _load_needDownloadFiles.clear();
                return;
            }

            if( finishCounter == numFiles )
            {
                for( auto &downloader : _downloaders )
                {
                    // 解除 downloader 的工作
                    downloader.registerSupplierAndHandler( nullptr, nullptr );
                }
                // 恢复后台下载
                download();

                // 跳出 while, 继续下面的异步加载
                break;
            }
            sleepMS( 50 );
        }

        setState( FileManagerState::Downloading );
        _events_load.push( FileEvent( FileEventTypes::Load_DownloadFinished ) );
    }

    // 开始异步加载

    // 需求: 对于 load 的所有文件( 调用者须提供 .plist , .atlas 及其他们引用到的所有文件的完整列表 ), 
    //      非贴图类, 需要 异步读出并放到 cache; 贴图类, 通过异步加载, load

    // 前置方案:
    // 1. 会有一个 LRUCache 于 FileUtils 之 getData/getText 中嵌入;
    // 2. 各种 c2dx 对象, 可能需要经过改造 令其支持 除文件名以外的 Data 加载方式

    // 载入方案:
    // 1. push 所有 贴图文件到 主线程加载队列, 当前线程 wait
    // 2. 于主线程 帧回调 check 主线程加载队列, foreach 调用 TextureCache 之 addImageAsync
    // 3. 回调函数累加计数器, 供当前线程的 wait 扫
    // 4. 当计数器与之前 push 的文件数相同时, 停止 wait
    // 5. foreach 所有 非贴图文件
    // 6. 通过 FileUtils 取到的 Data, push 到主线程, 当前线程 wait
    // 7. 于主线程 帧回调 check Data, 用同步方式加载, 置完成标志
    // 8. 当前线程的 wait 扫到 完成标志, goto 5 ( 要不要设计超时机制 再说 )
    // 9. foreach 完成, 压完成事件, 退出


    // 记录 load 日志
    for( auto& fir : _load_files )
    {
        auto it = _loadLog.insert( make_pair( fir, 1 ) );
        if( !it.second )
        {
            it.first->second++;

            // 已加载, 从 firs_async firs_sync 中移除, 发完成事件
            {
                auto i = find( firs_async.begin(), firs_async.end(), fir );
                if( i != firs_async.end() )
                {
                    firs_async.erase( i );
                    _events_load.push( FileEvent( FileEventTypes::Load_AsyncLoaded, fir->name ) );
                }
            }

            {
                auto i = find( firs_sync.begin(), firs_sync.end(), fir );
                if( i != firs_sync.end() )
                {
                    firs_sync.erase( i );
                    _events_load.push( FileEvent( FileEventTypes::Load_SyncLoaded, fir->name ) );
                }

            }

        }
    }


    // 将扫出的 贴图 文件放入待处理队列
    for( auto fir : firs_async )
    {
        _load_imageFiles.push( fir );
        _events_load.push( FileEvent( FileEventTypes::Load_AsyncLoading, fir->name ) );
    }

    // 等 image file 异步加载完成: check 已完成的个数
    while( (int)firs_async.size() > _load_imageFilesCounter )
    {
        if( !_running )
        {
            _load_imageFiles.clear();
            return;
        }
        sleepMS( 50 );
    }

    // 将扫出的需要同步加载的文件放入 cache
    for( auto fir : firs_sync )
    {
        // 跳过 sound 不处理( 不受 c2dx 文件系统管束, 直接经由系统级 api 来通过文件路径直读 )
        if( fir->type > FileTypes::SoundTypes_Begin && fir->type < FileTypes::SoundTypes_End ) continue;

        //_events_load.push( FileEvent( FileEventTypes::Load_AsyncCaching, fir->name ) );
        (void)Utils::fileGetData( fir->name );
//        std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );
        //_events_load.push( FileEvent( FileEventTypes::Load_AsyncCached, fir->name ) );
    }

    // 将扫出的文件放入待处理队列
    for( auto fir : firs_sync )
    {
        _events_load.push( FileEvent( FileEventTypes::Load_SyncLoading, fir->name ) );
        _load_resFiles.push( fir );
    }

    // 等主线程同步加载完成
    while( (int)firs_sync.size() > _load_resFilesCounter )
    {
        //if( !_running )
        //{
        //    _load_resFiles.clear();
        //    return;
        //}
        sleepMS( 50 );
    }

    // 成功退出
    setState( FileManagerState::Downloading );
    _events_load.push( FileEvent( FileEventTypes::Load_Finished ) );
}

void FileManager::update( float dt )
{
    static int frame_idx = 0;
    frame_idx++;

    auto tc = Director::getInstance()->getTextureCache();

    FileIndexRow* fir = nullptr;

    // 异步加载贴图
    if( !_asyncLoading && _load_imageFiles.pop( fir ) )
    {
        _asyncLoading = true;
        tc->addImageAsync( fir->name, [ this, fir ]( Texture2D* tex )
        {
            // 加持防删
            tex->retain();

            // 顺便看看要不要关抗锯齿
            if( fir->noaa )
            {
                tex->setAliasTexParameters();
            }

            _events_load.push( FileEvent( FileEventTypes::Load_AsyncLoaded, fir->name ) );
            ++_load_imageFilesCounter;
            _asyncLoading = false;
        } );

        goto LabPushEvents;
    }

    //struct timeval now;
    //struct timeval now2;
    //auto t1 = [ &]
    //{
    //    //gettimeofday( &now, nullptr );
    //};
    //auto t2 = [ &, this ]
    //{
    //    //gettimeofday( &now2, nullptr );
    //    //CCLOG( "************************ preload %d, %dus, dt: %0.3f, fi: %u, fn: %s"
    //    //       , (int)_load_resFilesCounter
    //    //       , ( (int)now2.tv_sec - (int)now.tv_sec ) * 1000000 + (int)now2.tv_usec - (int)now.tv_usec
    //    //       , dt
    //    //       , frame_idx
    //    //       , fir->name.c_str() );
    //};
    //auto t3 = [ &, this ]( const char* m )
    //{
    //    //gettimeofday( &now2, nullptr );
    //    //CCLOG( "------------------------ %dus, %s"
    //    //       , ( (int)now2.tv_sec - (int)now.tv_sec ) * 1000000 + (int)now2.tv_usec - (int)now.tv_usec
    //    //       , m );
    //};

    // 同步加载
    if( _load_resFiles.pop( fir ) ) // 从 while 改为了 if
    {
        switch( fir->type )
        {
        case FileTypes::FramePlist:
            //t1();
            SpriteFrameCache::getInstance()->addSpriteFramesWithFile( fir->name, true );
            //t2();
            break;
        case FileTypes::BgWav:
        case FileTypes::BgMp3:
        case FileTypes::BgCaf:
        case FileTypes::BgOgg:
            //t1();
            // 不预读音乐音效（因为 android 下似乎只能预读 3 ~ 5 个 )
            // SimpleAudioEngine::getInstance()->preloadBackgroundMusic( fir->name.c_str() );
            //t2();
            break;
        case FileTypes::Wav:
        case FileTypes::Mp3:
        case FileTypes::Caf:
        case FileTypes::Ogg:
        {
            //t1();
            // SimpleAudioEngine::getInstance()->preloadEffect( fir->name.c_str() );
            //t2();
            break;
        }
            //case FileTypes::ParticlePlist:
            //    tc->addImage( Utils::getTextureFileNameFromPlistFile( fir->name ) )->retain();
            //    break;
        default:break;
        }
        // maybe have another file type
        _events_load.push( FileEvent( FileEventTypes::Load_SyncLoaded, fir->name ) );
        ++_load_resFilesCounter;
    }

LabPushEvents:

    // 抛 event 到 handlers
    FileEvent e, e2;

    auto push = [ &]
    {
        LuaHelper::push( (int)e.eventType );
        for( auto p : e.parameters )
        {
            if( p.isNumeric )
            {
                LuaHelper::push( p.value_numeric );
            }
            else
            {
                LuaHelper::push( p.value_string );
            }
        }
        return e.parameters.size() + 1;
    };

    for( int i = 0; i < 2; ++i )
    {
        if( _events_loadIndex.pop( e ) )
        {
            if( _eh_loadIndex ) _eh_loadIndex( e );
            if( _lua_eh_loadIndex )
            {
                LuaHelper::executeFunction( _lua_eh_loadIndex, push() );
            }
        }
        else break;
    }

    for( int i = 0; i < 2; ++i )
    {
        if( _events_download.pop( e ) )
        {
            if( _eh_download ) _eh_download( e );
            if( _lua_eh_download )
            {
                LuaHelper::executeFunction( _lua_eh_download, push() );
            }
        }
        else break;
    }
   
    for( int i = 0; i < 2; ++i )
    {
        if( _events_load.pop( e ) )
        {
            // 合并不断下载追加的事件
            if( e.eventType == FileEventTypes::Download_Appending )
            {
                // 试取下一个事件, 如果有, 且为 下载追加, 且为 相同文件, 则合并, 继续取, 重复这个过程直到不能合并为止
                while( _events_load.pop( e2, [ &]( FileEvent const & item )
                {
                    return item.eventType == FileEventTypes::Download_Appending
                        && e.parameters[ 0 ].value_string == item.parameters[ 0 ].value_string;
                } ) )
                {
                    e.parameters[ 1 ].value_numeric += e2.parameters[ 1 ].value_numeric;
                }
            }

            if( _eh_load ) _eh_load( e );
            if( _lua_eh_load )
            {
                LuaHelper::executeFunction( _lua_eh_load, push() );
            }
        }
        else break;
    }
}

FileIndexRow* FileManager::getFileIndexRow( string const & fn )
{
    // 先 check fn 是否为 绝对路径
    // 绝对路径: 剥离出完整相对路径 再找， 找到则返回
    // 相对路径: 用 search path 来拼接出多个 key 反复找

    if( FileUtils::getInstance()->isAbsolutePath( fn ) )
    {
        if( fn.find_first_of( Global::WritablePath, 0 ) == 0 )
        {
            auto key = fn.substr( Global::WritablePath.size() );
            return _fileIndex.getRow( key );
        }
        else if( fn.find_first_of( Global::ResourcePath, 0 ) == 0 )
        {
            auto key = fn.substr( Global::ResourcePath.size() );
            return _fileIndex.getRow( key );
        }
        else
        {
            CCLOG( "FileManager::getFileIndexRow can't find file: %s", fn.c_str() );
            return nullptr;
        }
    }

    FileIndexRow* rtv = nullptr;
    for( auto sp : Global::SearchPaths )
    {
        auto key = sp + fn;
        rtv = _fileIndex.getRow( key );
        if( rtv ) return rtv;
    }
    return rtv;
}

FileIndex* FileManager::getIndex()
{
    return &_fileIndex;
}

void FileManager::setState( FileManagerState v )
{
    lock_guard<mutex> g( _stateMutex );
    _state = v;
}


