#ifndef __FILEDOWNLOADER_H__
#define __FILEDOWNLOADER_H__

// 不暴露到 LUA

// 文件下载任务描述( 该描述可直接提供 或经由 FileManager 来综合管理 )
struct FileDownloadTask
{
    template<typename STRING_T>
    FileDownloadTask( STRING_T && fullURL, FileIndexRow* fileIndexRow, int originalFileSize = 0 )
        : fullURL( forward<STRING_T>( fullURL ) )
        , fileIndexRow( fileIndexRow )
        , originalFileSize( originalFileSize )
        , currentDownloadSize( 0 )
    { }
    FileDownloadTask();
    FileDownloadTask( FileDownloadTask const & other );
    FileDownloadTask( FileDownloadTask && other );
    FileDownloadTask & operator=( FileDownloadTask const & other );
    FileDownloadTask & operator=( FileDownloadTask && other );

    
    string          fullURL;                        // 下载 url 根( 后面有 / )
    FileIndexRow*   fileIndexRow;                   // 指向相应 FileIndexRow( 实际下载 url 为 根 url + row.name, size 将被用于判断/存储下载总长 )       
    int             originalFileSize;               // 开始下载时的文件尺寸( 可能是没有下载完的 )
    int             currentDownloadSize;            // 本次已下载字节数( 加上 originalFileSize 才是当前文件长度 )
};


// 文件下载类( 内置一个 thread. 为简化问题, 该下载器每次只能下载单个文件, 但是可以为下载器设一个下载任务提供函数 )
class FileDownloader
{
public:
    typedef function<void( FileEvent && )> EventHandler;
    typedef function<void()> TaskSupplier;

    FileDownloader();
    ~FileDownloader();

    // 注册 事件处理函数
    void registerDownloadEventHandler( EventHandler f );

    // 注册 下载任务供应函数. process 会先调. 通过 download 函数下达任务
    void registerDownloadTaskSupplier( TaskSupplier f );

    // 同时注册上面 2 种函数
    void registerSupplierAndHandler( TaskSupplier ts, EventHandler eh );

    // 设置 download 期间用于操作 fileIndexRow state 的 mutex
    void setFileIndexRowMutex( mutex* m );

    // 设置下载任务并通知线程开始下载. 如果 已经在下载, 会 return false
    bool download( FileDownloadTask const & task );

    // 通知线程停止下载. 成功后产生 Stoped 事件
    void stop();

//private:
    mutex *                 _fileIndexRowMutex;             // 于修改 fileIndexRow.state 时使用的 mutex
    mutex                   _defaultFileIndexRowMutex;      // _fileIndexRowMutex 的默认值( 为了方便写代码 )
    atomic<bool>            _downloading;                   // 是否正在下载( download 函数执行期间 该值将为 true )
    bool                    _needStop;                      // 用于存放是否需要停止的信号
    FileDownloadTask        _task;                          // 当前文件下载任务

    EventHandler            _eventHandler;                  // 事件处理函数
    TaskSupplier            _taskSupplier;                  // 下载任务供应函数 
    mutex                   _funcMutex;                     // 针对上面 2 个函数. set 和 call 回调时 lock 一把防崩

    bool                    _running;                       // 用于通知线程退出
    bool                    _disposed;                      // true 表示线程成功退出

    void *                  _curl;                          // CURL上下文. 用 void 是为避免在 h 中引用 curl
    FILE *                  _file;                          // 文件上下文( download 的时候才会填充 )
    string                  _crc;                           // 下载 crc 的时候往这里写, 不进行文件操作

private:
    // for constructor
    void init();                                            

    // 下载线程调用函数
    void process();                                         

    // 供 process 调用的下载函数
    void download();                                        

    FileDownloader( FileDownloader const & other ) {};                      // delete
    FileDownloader & operator=( FileDownloader const & ) { return *this; }; // delete
};


#endif

