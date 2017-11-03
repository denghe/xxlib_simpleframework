#include "Precompile.h"
/* curl */
#include <curl/curl.h>
#include <curl/easy.h>


FileDownloadTask::FileDownloadTask()
: fileIndexRow( nullptr )
, originalFileSize( 0 )
, currentDownloadSize( 0 )
{
}

FileDownloadTask::FileDownloadTask( FileDownloadTask const & other )
: fullURL( other.fullURL )
, fileIndexRow( other.fileIndexRow )
, originalFileSize( other.originalFileSize )
, currentDownloadSize( other.currentDownloadSize )
{
}

FileDownloadTask::FileDownloadTask( FileDownloadTask && other )
: fullURL( move( other.fullURL ) )
, fileIndexRow( other.fileIndexRow )
, originalFileSize( other.originalFileSize )
, currentDownloadSize( other.currentDownloadSize )
{
}

FileDownloadTask & FileDownloadTask::operator=( FileDownloadTask const & other )
{
    fullURL = other.fullURL;
    fileIndexRow = other.fileIndexRow;
    originalFileSize = other.originalFileSize;
    currentDownloadSize = other.currentDownloadSize;
    return *this;
}

FileDownloadTask & FileDownloadTask::operator=( FileDownloadTask && other )
{
    fullURL = move( other.fullURL );
    fileIndexRow = other.fileIndexRow;
    originalFileSize = other.originalFileSize;
    currentDownloadSize = other.currentDownloadSize;
    return *this;
}




void FileDownloader::init()
{
    _running = true;
    _disposed = false;
    _downloading = false;
    //_curl = nullptr;
    _file = nullptr;
    _needStop = false;
    _fileIndexRowMutex = &_defaultFileIndexRowMutex;

    _curl = curl_easy_init();
    curl_easy_setopt( (CURL*)_curl, CURLOPT_NOSIGNAL,        1 );         // 禁 alarm 超时( 解决多线程问题, 但是可能卡域名解析 )
    curl_easy_setopt( (CURL*)_curl, CURLOPT_CONNECTTIMEOUT , 8L );
    //curl_easy_setopt( (CURL*)_curl, CURLOPT_LOW_SPEED_LIMIT, 128L );    // 在部分网络不良好的环境下
    //curl_easy_setopt( (CURL*)_curl, CURLOPT_LOW_SPEED_TIME,  10L );     // 导致大量连接断开错误，实际上还是有数据在传输
    curl_easy_setopt( (CURL*)_curl, CURLOPT_ACCEPT_ENCODING, "");
    
    thread( &FileDownloader::process, this ).detach();
}

FileDownloader::FileDownloader()
{
    init();
}

void FileDownloader::registerDownloadEventHandler( EventHandler f )
{
    lock_guard<mutex> g( _funcMutex );
    _eventHandler = f;
}

void FileDownloader::registerDownloadTaskSupplier( TaskSupplier f )
{
    lock_guard<mutex> g( _funcMutex );
    _taskSupplier = f;
}

void FileDownloader::registerSupplierAndHandler( TaskSupplier ts, EventHandler eh )
{
    lock_guard<mutex> g( _funcMutex );
    _taskSupplier = ts;
    _eventHandler = eh;
}

void FileDownloader::setFileIndexRowMutex( mutex* m )
{
    if( m ) _fileIndexRowMutex = m;
    else _fileIndexRowMutex = &_defaultFileIndexRowMutex;
}


FileDownloader::~FileDownloader()
{
    _running = false;
    while( !_disposed ) sleepMS( 1 );
    curl_easy_cleanup( (CURL*)_curl );
    _curl = nullptr;
}

bool FileDownloader::download( FileDownloadTask const & task )
{
    if( _downloading || !_running ) return false;
    _needStop = false;                          // 清一把保险
    _task = task;
    _downloading = true;                        // 通知 process 线程开始下载
    return true;
}

void FileDownloader::stop()
{
    if( _downloading ) _needStop = true;
}

// 线程函数
void FileDownloader::process()
{
    while( _running )                           // 死循环直到通知退出
    {
        TaskSupplier tsf;               // 安全获取 下载任务提供函数
        {
            lock_guard<mutex> g( _funcMutex );
            tsf = _taskSupplier;
        }
        if( tsf )                               // 如果有 reg 下载任务供应函数, 调之
        {
            tsf();
        }
        if( _downloading )                      // 如果有下载任务( 通过 download 函数设的 ), 开始下之
        {
            download();
            if( _needStop )                     // 手动停止的这个标记将维持
            {
                EventHandler cbf;            // 安全获取 事件处理函数
                {
                    lock_guard<mutex> g( _funcMutex );
                    cbf = _eventHandler;
                }
                if( cbf )
                {
                    cbf( FileEvent( FileEventTypes::Download_Stoped ).attachUserData( &_task ) );
                }

                _needStop = false;
            }
            _downloading = false;
        }
        else sleepMS( 200 );                     // 无下载任务的情况下省点 cpu. 开始下载不需要那么及时
    }
    _disposed = true;                           // 成功退出循环时标记为已退出
}

// 这是 curl 写文件 回调. userdata 为 FileDownloader
size_t write_function_header( char *buf, size_t size, size_t nmemb, void *userdata )
{
    return nmemb;
}

// 这是 curl 写文件 回调. userdata 为 FileDownloader
size_t write_function_bin( char *buf, size_t size, size_t nmemb, void *userdata )
{
    auto d = (FileDownloader*)userdata;
    auto written = fwrite( buf, size, nmemb, d->_file );
    fflush( d->_file );                         // 写文件 立即写入
    d->_task.currentDownloadSize += size * nmemb;
    FileDownloader::EventHandler cbf;        // 安全获取 事件处理函数
    {
        lock_guard<mutex> g( d->_funcMutex );
        cbf = d->_eventHandler;
    }
    if( cbf )
    {
        cbf( FileEvent( FileEventTypes::Download_Appending, d->_task.fileIndexRow->name, size * nmemb ).attachUserData( &d->_task ) );
    }
    if( !d->_running || d->_needStop ) return 0;// 如果当前需要停止运行, 返回 0 终止下载
    return written;
}

// 这是 curl 写 crc 文件 回调. userdata 为 FileDownloader
size_t write_function_crc( char *buf, size_t size, size_t nmemb, void *userdata )
{
    auto d = (FileDownloader*)userdata;
    d->_crc.append( buf, size * nmemb );
    FileDownloader::EventHandler cbf;        // 安全获取 事件处理函数
    {
        lock_guard<mutex> g( d->_funcMutex );
        cbf = d->_eventHandler;
    }
    if( cbf )
    {
        cbf( FileEvent( FileEventTypes::Download_CRCAppending, size * nmemb ).attachUserData( &d->_task ) );
    }
    if( !d->_running || d->_needStop ) return 0;// 如果当前需要停止运行, 返回 0 终止下载
    return nmemb;
}

void FileDownloader::download()
{
    // 指向文件索引行
    auto &fir = _task.fileIndexRow;

    // 设文件状态( lock )
    auto setState = [ &]( FileStates s )
    {
        lock_guard<mutex> g( *_fileIndexRowMutex );
        fir->state = s;
    };

    // 安全 call 事件处理函数
    auto eventCallback = [ this, fir ]( FileEvent && e )
    {
        FileDownloader::EventHandler cbf;
        {
            lock_guard<mutex> g( _funcMutex );
            cbf = _eventHandler;
        }
        if( cbf )
        {
            cbf( move( e.attachUserData(fir) ) );
        }
    };

    // return 时自动关闭文件
    Utils::ScopeGuard file_close_rg( [ this ]
    {
        if( this->_file )
        {
            fclose( _file );
            this->_file = nullptr;
        }
    } );

    // 得到写盘文件 和 下载url 绝对路径
    auto url = _task.fullURL;
    auto url_crc = url + ".crc";

    auto curl = (CURL*)_curl;

    // 设 写回调的 userdata 参数 为 this
    curl_easy_setopt( curl, CURLOPT_FILE, this );

    // 下载路径, 续传, 写回调 什么的, 在下载点附近设


    // 根据当前 下载任务 的 state 来路由接下来要做的事
    {
        lock_guard<mutex> g( *_fileIndexRowMutex );
        switch( fir->state )
        {
        case FileStates::Finished:                               // 有可能因解压而造成已完成
            eventCallback( FileEvent( FileEventTypes::Download_Finished, fir->name ) );
            return;

        case FileStates::NeedDownload:
            fir->state = FileStates::Downloading;                // 置下载态
            goto Lab_Download;

        case FileStates::CRCNeedDownload:
            fir->state = FileStates::CRCDownloading;             // 置下载 crc 态
            goto Lab_CRCDownload;

        case FileStates::NeedChecksum:
            fir->state = FileStates::Checksuming;                // 置校验态
            goto Lab_Checksum;

        case FileStates::NeedExtract:
            fir->state = FileStates::Extracting;                 // 置解压态
            goto Lab_Extract;
        default:
            // todo: CCLOG
            break;
        }
    }

Lab_Download:

    // 建目录
    if( !Utils::ensureDirectory( Utils::filePathCutFileName( fir->fullName ) ) )
    {
        eventCallback( FileEvent( FileEventTypes::Download_OpenFileError, fir->name ) );
        setState( FileStates::NeedDownload );
        return;
    }

    // 二进制追加或新建( 不保留 EOF 符 ) 
    _file = fopen( fir->fullName.c_str(), "a+b" );
    if( !_file )
    {
        eventCallback( FileEvent( FileEventTypes::Download_OpenFileError, fir->name ) );
        setState( FileStates::NeedDownload );
        return;
    }

    // 取文件当前尺寸
    _task.originalFileSize = Utils::fileMoveSeekEnd( _file );

    // 设置各种下载参数
    curl_easy_setopt( curl, CURLOPT_URL, url.c_str() );                        // 设下载路径

    // 抛 开始下载 事件
    eventCallback( FileEvent( FileEventTypes::Download_Started, fir->name ) );
    

    // 如果文件总长未知, 试取一把
    if( !fir->size )
    {
        curl_easy_setopt( curl, CURLOPT_HEADER, 1L );                          // 下载文件头
        curl_easy_setopt( curl, CURLOPT_NOBODY, 1L );                          // 不下载文件内容

        Utils::ScopeGuard curl_readLength_rg( [ &]                             // 出这个代码段自动还原 curl 设置
        {
            curl_easy_setopt( curl, CURLOPT_HEADER, 0L );                      // 还原设置 for 内容下载
            curl_easy_setopt( curl, CURLOPT_NOBODY, 0L );                      // 还原设置 for 内容下载
        } );

        // 设 写回调
        curl_easy_setopt( curl, CURLOPT_WRITEFUNCTION, write_function_header );

        auto code = curl_easy_perform( curl );                                 // 下载文件头
        if( code )
        {
            eventCallback( FileEvent( FileEventTypes::Download_ResponseError, fir->name ) );
            setState( FileStates::NeedDownload );
            return;
        }
        long retcode = 0;
        code = curl_easy_getinfo( curl, CURLINFO_RESPONSE_CODE, &retcode );
        if( ( code == CURLE_OK ) && retcode == 200 )
        {
            double length = 0;
            code = curl_easy_getinfo( curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &length );
            fir->size = length;
            if( code != CURLE_OK || fir->size == -1 )
            {
                fir->size = 0;  // 未取到长度
                eventCallback( FileEvent( FileEventTypes::Download_GotLength, fir->name, 0, _task.originalFileSize ) );
            }
            else
            {
                eventCallback( FileEvent( FileEventTypes::Download_GotLength, fir->name, length, _task.originalFileSize ) );
            }
        }
        else
        {
            eventCallback( FileEvent( FileEventTypes::Download_GetLengthError, fir->name ) );
            setState( FileStates::NeedDownload );
            return;
        }
    }
    else
    {
        eventCallback( FileEvent( FileEventTypes::Download_GotLength, fir->name, fir->size, _task.originalFileSize ) );
    }

    // 如果有最大长度, 与当前长度比较, 看看能不能不下载, 跳到下一步
    if( fir->size )
    {
        if( _task.originalFileSize == fir->size )                // 长度符合预期, 直接 checksum 或 下载 crc
        {
            eventCallback( FileEvent( FileEventTypes::Download_Downloaded, fir->name ) );

            if( fir->crc32 || fir->md5.size() )                                 // 潜规则: 如果 crc32, md5 为空 则下载含有 crc 信息的文件, 附加扩展名为 .crc
            {
                setState( FileStates::NeedChecksum );
                //goto Lab_Checksum;
            }
            //else
            //{
            //    setState( FileStates::CRCDownloading );
            //    goto Lab_CRCDownload;
            //}
            goto Lab_Checksum;      // 先废弃掉下载 .crc 
        }
        if( _task.originalFileSize > fir->size )                 // 如果已存在的文件比要下载的长, 认为是错误的文件, 删掉尸体, 不产生事件
        {
            Utils::fileDelete( fir->fullName );
            _task.originalFileSize = 0;
        }
    }

    // 续传
    if( _task.originalFileSize )
    {
        curl_easy_setopt( curl, CURLOPT_RESUME_FROM, (long)_task.originalFileSize );   // 设续传字节数
    }
    else
    {
        curl_easy_setopt( curl, CURLOPT_RESUME_FROM, (long)0 );
    }

    // 设 写回调
    curl_easy_setopt( curl, CURLOPT_WRITEFUNCTION, write_function_bin );

    {
    // 下载
    auto res = curl_easy_perform( curl );
    if( res )
    {
        if (res == CURLE_WRITE_ERROR)
        {
            eventCallback( FileEvent( FileEventTypes::Download_WriteFileError, fir->name, res ) );
            setState( FileStates::NeedDownload );
        }
        else if (res == CURLE_RANGE_ERROR)
        {
            Utils::fileCut(this->_file, 0);
            _task.originalFileSize = 0;
            _task.currentDownloadSize = 0;
            eventCallback( FileEvent( FileEventTypes::Download_ResponseError, fir->name, res ) );
            setState( FileStates::NeedDownload );
        }
        else
        {
            eventCallback( FileEvent( FileEventTypes::Download_ResponseError, fir->name, res ) );
            setState( FileStates::NeedDownload );
        }

        return;
    }

    // 取返回码
    long resp_code = 0;
    res = curl_easy_getinfo( curl, CURLINFO_RESPONSE_CODE, &resp_code );

    // 判断返回码. 如果出错, 本次 append 可能是无效数据, 切掉
    if( res != 0 || ( resp_code != 200 && resp_code != 206 ) )
    {
        if( _task.originalFileSize )
        {
            Utils::fileCut( _file, _task.originalFileSize );                    // 切掉本次下载的数据部分 保留文件原始内容
        }
        else
        {
            file_close_rg.run();                                                // 关文件
            Utils::fileDelete( fir->fullName );                                 // 删掉
        }
        eventCallback( FileEvent( FileEventTypes::Download_ResponseError, fir->name, res, resp_code ) );
        setState( FileStates::NeedDownload );
        return;                                                                 // 这里不会再关文件了
    }
    }

    //// 判断是否需要下载 crc 文件
    //if( fir->crc32 || fir->md5.size() )
    //{
    eventCallback( FileEvent( FileEventTypes::Download_Downloaded, fir->name ) );
    goto Lab_Checksum;      // 先跳过 Lab_CRCDownload
    //}




Lab_CRCDownload:

    // 清 _crc 准备下载
    _crc = "";

    // 设 crc 下载 url
    curl_easy_setopt( curl, CURLOPT_URL, url_crc.c_str() );

    // 不续传
    curl_easy_setopt( curl, CURLOPT_RESUME_FROM, 0L );

    // 设 写 crc 回调
    curl_easy_setopt( curl, CURLOPT_WRITEFUNCTION, write_function_crc );

    {
    auto res = curl_easy_perform( curl );                                      // 下载
    if( res )
    {
        auto et = ( res == CURLE_WRITE_ERROR ? FileEventTypes::Download_WriteFileError : FileEventTypes::Download_ResponseError );
        eventCallback( FileEvent( et, fir->name ) );
        setState( FileStates::CRCNeedDownload );
        return;
    }

    // 取返回码
    auto resp_code = 0;
    res = curl_easy_getinfo( curl, CURLINFO_RESPONSE_CODE, &resp_code );

    // 判断返回码. 如果出错, 本次 append 可能是无效数据, 切掉
    if( res != 0 || ( resp_code != 200 && resp_code != 206 ) )
    {
        eventCallback( FileEvent( FileEventTypes::Download_ResponseError, fir->name, res, resp_code ) );
        setState( FileStates::CRCNeedDownload );
        return;

        // test code
        //_crc = "-676980427,ED329A85E35AEB31AA9F971E9D394D6D";
    }
    }

    // 将下载的 crc 内容存到 fir 中
    // 文件格式为 crc,md5 即中间用 逗号分割. 不符或转换失败将报错
    {
        auto ss = Utils::split( _crc, "," );
        if( ss.size() != 2 || ss[ 0 ].size() == 0 || ss[ 1 ].size() == 0 )
        {
            eventCallback( FileEvent( FileEventTypes::Download_CRCFormatError, fir->name, _crc ).attachUserData( &_task ) );
            setState( FileStates::CRCNeedDownload );
            return;
        }
        fir->crc32 = atoi( ss[ 0 ].c_str() );
        if( fir->crc32 == 0 )
        {
            eventCallback( FileEvent( FileEventTypes::Download_CRCFormatError, fir->name, ss[ 0 ] ).attachUserData( &_task ) );
            setState( FileStates::CRCNeedDownload );
            return;
        }
        fir->md5 = ss[ 1 ];
    }

    setState( FileStates::NeedChecksum );



Lab_Checksum:

    if( !fir->crc32 && !fir->md5.size() ) goto Lab_Finished;                // 如果未提供 crc md5 就不校验

    {
    if( _file )                             // 已打开( 需要移动游标 )
    {
        fseek( _file, 0, SEEK_SET );
    }
    else                                    // 打开文件( 如果需要的话 )
    {
        _file = fopen( fir->fullName.c_str(), "rb" );
        if( !_file )
        {
            eventCallback( FileEvent( FileEventTypes::Download_OpenFileError, fir->name ) );
            setState( FileStates::NeedChecksum );
            return;
        }
    }

    // 开始校验
    int  len;
    char buf[ 1024 ];
    uLong crc = crc32( 0L, Z_NULL, 0 );     // 初始化上下文?
    WW_MD5_CTX md5;
    WW_MD5_Init( &md5 );                    // 初始化上下文?

    auto md5_data = (byte*)buf;
    char *md5_string = buf + 16;

    while( !feof( _file ) && ( len = fread( buf, 1, sizeof( buf ), _file ) ) )
    {
        crc = crc32( crc, (byte*)buf, len );
        WW_MD5_Update( &md5, buf, len );
    }

    memset( md5_data, 0, sizeof( buf ) );
    WW_MD5_Final( (byte*)md5_data, &md5 );

    static char digt2hex[] = {
        '0', '1', '2', '3', '4', '5', '6', '7',
        '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
    };

#define D2H(idx)                                                \
    md5_string[idx * 2    ] = digt2hex[md5_data[idx] >> 4  ];   \
    md5_string[idx * 2 + 1] = digt2hex[md5_data[idx] & 0x0F]

    D2H( 0x0 );   D2H( 0x1 );   D2H( 0x2 );   D2H( 0x3 );
    D2H( 0x4 );   D2H( 0x5 );   D2H( 0x6 );   D2H( 0x7 );
    D2H( 0x8 );   D2H( 0x9 );   D2H( 0xa );   D2H( 0xb );
    D2H( 0xc );   D2H( 0xd );   D2H( 0xe );   D2H( 0xf );

    md5_string[ 32 ] = 0;
#undef D2H

    if( fir->md5.size() && stricmp( md5_string, fir->md5.c_str() ) )
    {
        file_close_rg.runAndCancel();
        Utils::fileDelete( fir->fullName );
        eventCallback( FileEvent( FileEventTypes::Download_MD5Error, fir->name, string( md5_string ), fir->md5 ) );
        setState( FileStates::NeedDownload );
        return;
    }

    if( fir->crc32 && ( int )(uint)crc != fir->crc32 )
    {
        file_close_rg.runAndCancel();
        Utils::fileDelete( fir->fullName );
        eventCallback( FileEvent( FileEventTypes::Download_CRC32Error, fir->name, (int)crc, fir->crc32 ) );
        setState( FileStates::NeedDownload );
        return;
    }

    if( false )      // todo: check if need extract
    {
        setState( FileStates::NeedExtract );
        goto Lab_Extract;
    }
    goto Lab_Finished;
    }




Lab_Extract:
    // todo: 解压( 前面的下载文件名, url 也要根据 所在 zip 文件来生成 )




Lab_Finished:

    file_close_rg.runAndCancel();
    setState( FileStates::Finished );
    eventCallback( FileEvent( FileEventTypes::Download_Finished, fir->name ) );       // 下载完成, 产生事件并返回
}





//curl_easy_setopt( _curl, CURLOPT_NOPROGRESS, false );                       // 开启进度
//curl_easy_setopt( _curl, CURLOPT_PROGRESSDATA, &task );                     // 设 进度回调 的 userdata 参数 为 task
//curl_easy_setopt( _curl, CURLOPT_PROGRESSFUNCTION, progress_function );     // 进度回调

//int progress_function_ex( void *userdata, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow )
//{
//}

//int progress_function( void *userdata, double dltotal, double dlnow, double ultotal, double ulnow )
//{
//    //return progress_function_ex( userdata, (curl_off_t)dltotal, (curl_off_t)dlnow, (curl_off_t)ultotal, (curl_off_t)ulnow );

//    auto task = (FileDownloadTask *)userdata;
//    double progress = 0;
//    if( dltotal > 0 )
//    {
//        progress = ( task->currSize + dlnow ) / ( task->currSize + dltotal ) * 100;
//        //task->currSize += dlnow;
//    }
//    task->downloader->_events.push( FileDownloadEvent( FileDownloadEventTypes::Downloading, task ) );
//    return 0;
//}
//
//void FileDownloader::add( string const & downloadURL, string saveToFileName, bool append, int maxFileSize )
//{
//    if( _tasks.exists( [ &]( FileDownloadTask const & t ) { return t.url == downloadURL; } ) ) return;
//    if( !append ) Utils::fileDelete( saveToFileName );
//    FileDownloadTask t = { this, downloadURL, saveToFileName, 0, maxFileSize, 0 };    // originalFileSize 留到 download 函数中 open file 再取
//    _tasks.push( t );
//}
//
//void FileDownloader::remove( FileDownloadTask* task )
//{
//    _events.erase( [ &]( FileDownloadEvent& e ) { return e.task == task; } );
//}
//
//void FileDownloader::remove( string const & downloadURL )
//{
//    _events.erase( [ &]( FileDownloadEvent& e ) { return e.task->url == downloadURL; } );
//}
//

//void FileDownloader::setTask( string const & downloadURL, string saveToFileName, bool append, int maxFileSize )
//{
//if( _tasks.exists( [ &]( FileDownloadTask const & t ) { return t.url == downloadURL; } ) ) return;
//if( !append ) Utils::fileDelete( saveToFileName );
//FileDownloadTask t = { this, downloadURL, saveToFileName, 0, maxFileSize, 0 };    // originalFileSize 留到 download 函数中 open file 再取
//_tasks.push( t );
//}

//void FileDownloader::update()
//{
//    if( _events.empty() ) return;   // 如果 _cb 一直为空 似乎队列会变的很大?
//    if( !_cb )
//    {
//        CCLOG( "FileDownloader::update() the _cb is nullptr, please regCB first, before add download task." );
//    }
//    FileDownloadEvent e;
//    while( _events.pop( e ) )
//    {
//        _cb( e );
//    }
//}



//
//
////typedef size_t( *ft )( void *ptr, size_t, size_t, void * );
//size_t process( void *ptr, size_t size, size_t nmemb, void *userdata )
//{
//    static int x = 0;
//    if( x++ == 2 )
//    {
//        log( "return CURL_WRITEFUNC_PAUSE" );
//        return CURL_WRITEFUNC_PAUSE;    // 这个受超时影响
//    }
//
//    log( "size = %d, nmemb = %d", (int)size, (int)nmemb );
//    std::string s;
//    BufferHelper::dump( s, (char*)ptr, size * nmemb );
//    //printf( "data = %s", s.c_str() );
//    return nmemb;
//}
//
//bool test( )
//{
//    // 根据网上一些文字的说法, 可能会卡域名解析. 故可以先在单个线程中 控制超时并探索 url, 得到 ip 后, 于多线程下使用
//
//    /*
//    在多线程应用中，需要在主线程中调用这个函数。这个函数设置libcurl所需的环境。通常情况，如果不显式的调用它，第一次调用 curl_easy_init()时，curl_easy_init 会调用 curl_global_init，在单线程环境下，这不是问题。但是多线程下就不行了，因为curl_global_init不是线程安全的。在多个线 程中调用curl_easy_int，然后如果两个线程同时发现curl_global_init还没有被调用，同时调用 curl_global_init，悲剧就发生了。这种情况发生的概率很小，但可能性是存在的。
//    */
//    curl_global_init( CURL_GLOBAL_ALL );                        // global init
//
//    auto c = curl_easy_init( );                                  // session init
//    curl_easy_setopt( c, CURLOPT_URL, "http://127.0.0.1/1.data" );
//    curl_easy_setopt( c, CURLOPT_VERBOSE, 1L );                 /* Switch on full protocol/debug output while testing */
//    curl_easy_setopt( c, CURLOPT_NOPROGRESS, 1L );              /* disable progress meter, set to 0L to enable and disable debug output */
//
//    /*
//    libcurl 有个很好的特性，它甚至可以控制域名解析的超时。但是在默认情况下，它是使用alarm + siglongjmp 实现的。用alarm在多线程下做超时，本身就几乎不可能。如果只是使用alarm，并不会导致程序崩溃，但是，再加上siglongjmp，就要命了 （程序崩溃的很可怕，core中几乎看不出有用信息），因为其需要一个sigjmp_buf型的全局变量，多线程修改它。（通常情况下，可以每个线程一个 sigjmp_buf 型的变量，这种情况下，多线程中使用 siglongjmp 是没有问题的，但是libcurl只有一个全局变量，所有的线程都会用）。
//    具体是类似 curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30L) 的超时设置，导致alarm的使用（估计发生在域名解析阶段），如前所述，这在多线程中是不行的。解决方式是禁用掉alarm这种超时， curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L)。
//    */
//    curl_easy_setopt( c, CURLOPT_NOSIGNAL, 1L );
//    /*
//    CURLOPT_HEADER：设为1，则在返回的内容里包含http header；
//    CURLOPT_FOLLOWLOCATION：设为0，则不会自动301，302跳转；
//    CURLOPT_NOBODY: 如果你不想在输出中包含body部分，设置这个选项为一个非零值。
//    CURLOPT_FAILONERROR: 如果你想让PHP在发生错误(HTTP代码返回大于等于300)时，不显示，设置这个选项为一人非零值。默认行为是返回一个正常页，忽略代码。
//    CURLOPT_TIMEOUT: 设置一个长整形数，作为最大延续多少秒。
//    CURLOPT_LOW_SPEED_LIMIT: 设置一个长整形数，控制传送多少字节。
//    CURLOPT_LOW_SPEED_TIME: 设置一个长整形数，控制多少秒传送 CURLOPT_LOW_SPEED_LIMIT 规定的字节数。
//    */
//
//    curl_easy_setopt( c, CURLOPT_LOW_SPEED_LIMIT, 1000L );
//    curl_easy_setopt( c, CURLOPT_LOW_SPEED_TIME, 5L );
//
//    // todo: 还要设超时
//    curl_easy_setopt( c, CURLOPT_FILE, (void *)123 );
//    curl_easy_setopt( c, CURLOPT_WRITEFUNCTION, process );               /* send all data to this function  */
//    curl_easy_perform( c ); /* get it! */
//    log( "sssssssssssssss" );
//    curl_easy_cleanup( c ); /* cleanup curl stuff */
//    curl_global_cleanup( );
//    return false;
//}
// 扫文件表, 看看是否 有什么文件 已经被其他 同优先级 队列纳入下载( 理论上讲不可能出现这种情况 )
// plan A:
//      如果有文件已经被低优先级队列纳入下载( 且状态为未下载 ), 则从目标队列中将该文件移到当前优先级队列
//      如果当前队列并未下载完该文件( 比如取消下载了 ), 则将移过来的文件 还回低优先级队列( 遍历 _requests 重新下单 )
// plan B:
//      允许各批次文件有重复, 在下载前, 先 lock check FileIndex( 也就是说读写都加锁 ), 如果该文件正在下载或校验解压之类, 则跳过
// plan C:
//      如果发现 要下载的文件 已位于某优先级的下载队列, 则将这些文件往 "前" 移, 优先下载
//      下载队列中没有的文件, 则插到相应优先级队列最前面


