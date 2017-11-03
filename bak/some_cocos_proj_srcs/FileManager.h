#ifndef __FILEMANAGER_H__
#define __FILEMANAGER_H__


// 文件管理器运行态
enum class FileManagerState
{
    None,                   // 初始态, 只能 loadIndex
    IndexLoading,           // 正在 loadIndex, 如果失败 将变为 None
    Downloading,            // 后台下载中( 这是一种常态. 只要后台下载队列不空, 就会一直下载 )
    Loading                 // 载入中, 将暂停后台下载并开始当前批次的下载和异步加载, 之后恢复后台下载
};


// 文件管理器( 下载, 异步加载, 卸载. 该类以单例方式工作 )
/*

管理器以 下单 方式运作, 下面几个函数 只是用于设置 当前所要做的事情, 具体的由 process() 去做. 其机制与 文件下载器 有点类似
    通过 loadIndex 传入 index db 相关信息 以 创建/加载/下载/升级 文件索引db
    通过 download 传入文件名列表 以追加后台下载项, 下载器会不断尝试后台下载 + 校验. 校验不过会删了重下. 
    通过 load 函数 传入文件名列表 以发起 当前批次下载 + 异步加载请求. 
    通过 unload 函数 传入文件名列表 以发起资源卸载请求. ( 该函数立即执行并生效, 上面 3 个是异步, 要等待事件通知才知道是否成功之类 )
另: 
    根据 文件管理器运行态, 首先必须 loadIndex, 成功之后才能 load. download 函数随时可调用, 但会在 loadIndex 结束之后才开始下载
    理论上讲存在 load 与 unload 的并发冲突, 这需要使用者根据事件 错开调用时机

下载方式:
    通过 curl, 设超时 5 秒( 经验数据 ), 设 1k 5秒低速( 经验数据 ), 以多线方式下载, 校验, 解压, 长期执行.
    始终从 _download_files pop 出当前要下载的文件, 并开始下载. 综合控制下载器行为, 根据它们的状态来分配下载任务.
    load 函数执行期间, 将停止后台下载解压校验任务, 向 _download_files 压入一批新文件, 再恢复 后台下载解压校验 的运行.
    通过事件监控, 可得知这批文件是否已经下载完成. 如果未来要添加 cancel load 操作, 停止事件监控, 恢复后台下载就好.
*/
class FileManager
{
public:
    typedef function<void( FileEvent const & )> EventHandlerType;

    FileManager();
    ~FileManager();

    void registerLoadIndexEventHandler( LUA_FUNCTION eh );
    void registerDownloadEventHandler( LUA_FUNCTION eh );
    void registerLoadEventHandler( LUA_FUNCTION eh );

    // todo: maybe need unreg ? maybe not ?

    void registerLoadIndexEventHandler( EventHandlerType eh );          // 注册 索引相关 的事件回调. 传入 nullptr 为反注册
    void registerDownloadEventHandler( EventHandlerType eh );           // 注册 后台下载 的事件回调. 传入 nullptr 为反注册
    void registerLoadEventHandler( EventHandlerType eh );               // 注册 立即下载 + 异步加载 的事件回调. 传入 nullptr 为反注册

    // 试载入文件索引: ( 游戏 launcher 部分要做的事情 )
    // fileName: index db 文件名( 相对路径. 运行时版本位于 可写区, 打包版本位于资源 )
    // baseURL: 所有 url 根，结尾为 /. 下载的 db 文件为 baseURL + fileName + ".web" , 下载的 db 的 crc 文件为 baseURL + fileName + ".web.crc"
    // 如果 baseURL 为 空"", 则跳过下载这步
    // 注: 路径均为相对路径, 前面不加 /
    bool loadIndex( string const & fileName, string const & baseURL );

    // 立即插入需要 后台下载 的文件列表. 不去重不排序( 随时可插入 )
    // 为优化下载流量, 实时响应, 设下载器[0] 为后台专用. load 时切换 下载任务提供程序和事件处理
    void download( vector<string> const & files );

    // 注册 立即下载 + 异步加载 文件列表. 如果先前的 load 行为还没有 结束, 将 return false
    bool load( vector<string> const & files );

    // 试卸载资源. 异步加载, 卸载 仅针对 可异步加载的资源类文件. 当前 c2dx 版本为 TextureCache 支持 AsyncLoad 的贴图
    bool unload( vector<string> const & files );


    // 注册到 frame update 回调, 以产生事件( 不生成 LUA )
    void update(float dt);

    // 得到 index 以方便 lua 中通过短文件名取信息
    FileIndex* getIndex();

    MAKE_INSTANCE_H( FileManager );
    FileIndexRow* getFileIndexRow( string const & fn );                 // 从 _fileIndex 查找 fn( 分为相对路径和绝对路径, 相对又分为 实际路径和 search 路径 )
private:
    void unloadCore( FileIndexRow* fir );
    void unloadCore( vector<FileIndexRow*> & firs );

    void process();                                                     // 调度管理线程( 切换执行下面 3 个函数之一 )
    void loadIndex();
    void download();
    void load();
    mutex                                   _stateMutex;                // 切换 state 用到的 mutex
    FileManagerState                        _state;                     // FileManager 执行态
    void setState( FileManagerState v );

    // 下面是相关步骤的参数 store 区以及子函数
    // for loadIndex
    string                                  _loadIndex_fileName;        // index db 的文件名( 下载路径为 _baseURL + 它 )
    FileIndex                               _fileIndex;                 // 总 文件索引
    mutex                                   _fileIndexRowMutex;         // 凡是关系到 FileIndoexRow 类的操作 都要 lock( 比如访问/改属性值 )

    // for load
    vector<FileIndexRow*>                   _load_files;                // load 时传入的原始文件列表
    ThreadSafeQueue<FileIndexRow*>          _load_needDownloadFiles;    // 需要 立即下载 的文件索引指针列表
    ThreadSafeQueue<FileIndexRow*>          _load_imageFiles;           // 主线程 update 中扫, 如果有文件, 则用 TextureCache addImageAsync 加载
    atomic<int>                             _load_imageFilesCounter;    // TextureCache addImageAsync 回调将累加该 counter
    ThreadSafeQueue<FileIndexRow*>          _load_resFiles;             // 主线程 update 中扫, 如果有文件, 则根据文件类型用相应的加载方式加载
    atomic<int>                             _load_resFilesCounter;      // 加载过程中将累加该 counter

    // for download
    ThreadSafeQueue<FileIndexRow*>          _download_files;            // 后台要下载 的文件列表
    FileDownloader                          _downloaders[ 2 ];          // 多线不分块下载器组( 其中 _downloaders[ 0 ] 被用于 下载 index db 以及后台 )
	
    string                                  _baseURL;                   // 下载各种文件的根 url

    EventHandlerType                         _eh_loadIndex;
    EventHandlerType                         _eh_download;
    EventHandlerType                         _eh_load;

    LUA_FUNCTION                            _lua_eh_loadIndex;
    LUA_FUNCTION                            _lua_eh_download;
    LUA_FUNCTION                            _lua_eh_load;

    ThreadSafeQueue<FileEvent>              _events_loadIndex;          // 相关事件容器
    ThreadSafeQueue<FileEvent>              _events_download;           // 相关事件容器
    ThreadSafeQueue<FileEvent>              _events_load;               // 相关事件容器

    bool                                    _running;                   // 线程退出标记位
    bool                                    _disposed;                  // true 表示线程成功退出

    unordered_map<FileIndexRow*, int>       _loadLog;                   // 用以记录文件 load 次数, unload 时反向操作, FileManager 析构时 unload all

    bool                                    _asyncLoading;              // 正在异步加载的标记（异步加载一次一个，回调后再搞别的）
};


#endif







/*

设计使用方式:


FileManager
host:
单例 init, free

lua:


fm:load(fs, q, cb)

长相:

fm = ww.FileManager:getInstance()   -- 可能是一个全局的变量

-- launcher:

fm:regLoadIndexCB( function( eventName )
         if eventName == "Started" then
    else if eventName == "Downloading" then
    else if eventName == "Verifyed" then
    else if eventName == "Copyed" then
    else if eventName == "Opened" then
    else if eventName == "Loaded" then
    else
    end
end )
fm:loadDB( "index.db", "index.db", "index.db.www", "http://asdfasdfasdf/v234/index.db" )







-- 游戏运行时:



local files = {
"asdf.png",
"qwer.plist",
"zxcv.json"
-- ...
}
local priority = 1         -- Priority 优先级 优先级值为 1~n , 越大越优先

local callback = function( batch, eventName, ... )

if eventName == "需要下载" then                 -- 为了方便编码, 刚开始 可能会 发生一次
    batch._dialog = xx.ShowDialog( 各种参数 )  -- 这个事件中创建对话框
elseif eventName == "全部完成" then             -- 只发生一次
    batch._dialog:hide();                      -- _ui 为附加到 batch 上的 ui 对象
-- navgateToxxxxx
目标UI._batch = batch                      -- 保存一份引用, 以便在 目标UI 退出后, 调 目标UI._batch:unload(); 目标UI._batch = nil

elseif eventName == "下载中" then
    batch._dialog:setXxxxx( ... )              -- 设各种值,态
elseif eventName == "校验中" then
    batch._dialog:setXxxxx( ... )              -- 设各种值,态
elseif eventName == "解压中" then
    batch._dialog:setXxxxx( ... )              -- 设各种值,态
elseif eventName == "单个下载完成" then
    batch._dialog:setXxxxx( ... )              -- 设各种值,态
elseif eventName == "单个校验完成" then
    batch._dialog:setXxxxx( ... )              -- 设各种值,态
elseif eventName == "单个解压完成" then
    batch._dialog:setXxxxx( ... )              -- 设各种值,态
elseif eventName == "断网啥啥" then
    batch:cancel()                             -- 取消本次操作
elseif eventName == "各种出错失败" then
    batch._dialog:setXxxxx( ... )              -- 设各种值,态
elseif eventName == "已取消" then
    xxxxxxx                                    -- 执行了 batch:cancel() 或程序内部取消了本批下载( 已下载的文件并不会删 )
else
    -- ...编码异常? 已卸载事件 用于调试?
end
    -- ...
end

local batch = fm:load( files, priority, callback )

batch._dialog = xx.ShowDialog()        -- 令 loading ui 与 batch 纠缠
batch._dialog._batch = batch


*/

