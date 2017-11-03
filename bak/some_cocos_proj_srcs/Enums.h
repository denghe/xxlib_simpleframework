#ifndef __ENUMS_H__
#define __ENUMS_H__

// 当前项目所有 enum 都放在此处 方便互引, 统一管理, 生成 lua 表

// 各种相关网络配置参数
enum class NetConfigs : int
{
    // send 时用到的 buffer 长
    BufferSize = 1024,

    // 要 send 的数据的累计长( 待发的 )
    MaxWriteBufferSize = 327680,

    // 每次调用 send 的限长
    MaxSendSizePerFrame = 8192,

    // 这两个是 select 的等待超时时长 sec : usec
    WorkerSelectTimeOutSecond = 1,
    WorkerSelectTimeOutUSecond = 0,

    // 用作累存待发数据的 list buffer 容器每个块的最小尺寸
    ListBufferMinBlockSize = 1024,

    // 数据包容器的定长
    PacketBufferSize = 32768,
};


// socket 连接状态
enum class SocketStates
{
    // 未知( 初始, 默认态)
    Unknown,

    // 活动中
    Alive,

    // 已关闭
    Closed,

    // 已死
    Dead
};


// 网络事件
enum class NetEvents
{
    // 未知( 默认值 )
    Unknown,

    // 已连上
    Connected,

    // 连失败
    ConnectFailed,

    // 已关闭
    Closed,

    // 收到数据( 不映射到 LUA )
    Received,

    // 收到数据包
    ReceivedPacket,
};


// 网络物理类型
enum class NetTypes
{
    // 无网
    None,

    // 无线网
    Wifi,

    // 电话网
    Wwan
};


// http 请求结果
enum class HttpRequestResults
{
    // 成功
    Success,

    // 失败
    Fail
};


// 文件相关事件类型( 含各种下载加载操作 )
enum class FileEventTypes                                       // 下方是 参数说明
{
    /************************************************************************/
    /* LoadIndex 相关                                                       */
    /************************************************************************/

    // 开始执行
    LoadIndex_Started,                                          // db纯文件名, baseURL

    // 开始下载 Web db
    LoadIndex_DownloadBegin,                                    // web db 文件全名, 完整下载URL

    /////////////////////////////////
    // 这期间会产生 Download_ 系列事件
    /////////////////////////////////

    // Web db 下载完成
    LoadIndex_DownloadFinished,

    // 加载 Web db
    LoadIndex_WebLoaded,

    // 已载入原始包中的 db
    LoadIndex_PackageLoaded,

    // 已检查 Runtime db 是否存在
    LoadIndex_RuntimeChecked,

    // 已从 Package 复制到 可写区
    LoadIndex_RuntimeCopyed,                                    // 源文件全名, runtime db 文件全名

    // 已载入 Runtime db
    LoadIndex_RuntimeLoaded,                                    // runtime db 文件全名

    // 已使用 Original db 来升级 Runtime db ( 可能升级了原始包 )
    LoadIndex_RuntimeUpdatedByPackage,                          // 要升级的 runtime db 文件全名, 升级用 packet db 文件全名

    // 使用 Web db 来升级 Runtime db
    LoadIndex_RuntimeUpdatedByWeb,                              // 要升级的 runtime db 文件全名, 升级用 web db 文件全名

    // LoadIndex 完成
    LoadIndex_Finished,

    //////////////////////////////////////////////////////////////////////////
    // 标志位事件
    LoadIndex_Error_Begin,

    // sqlite 打开失败
    LoadIndex_OpenDBError,                                      // 文件全名

    // 从原始包复制 db 到可写区 错
    LoadIndex_CopyFileError,                                    // 源文件全名, 目标文件全名

    // 载入失败
    LoadIndex_LoadError,                                        // 文件全名

    // update db 错
    LoadIndex_UpdateError,                                      // 升级用db文件全名

    // 标志位事件
    LoadIndex_Error_End,
    //////////////////////////////////////////////////////////////////////////


    /************************************************************************/
    /* Download 相关                                                        */
    /************************************************************************/

    // 开始下载
    Download_Started,                                           // 短文件名( 可通过它来取到 FileIndexRow 获取详细信息 )

    // 已获取长度信息
    Download_GotLength,                                         // 短文件名, 完整长度, 已存在的文件长度

    // 文件追加写入( 这个事件通常会发生很多次, 与文件长度成正比 )
    Download_Appending,                                         // 短文件名, 本次追加长度

    // 已下完( 未校验 )
    Download_Downloaded,                                        // 短文件名

    // crc 文件追加写入( crc 文件不知道长度 但通常很短 )
    Download_CRCAppending,                                      // 短文件名, 本次追加长度

    // 已下完 crc 文件( 未校验 )
    Download_CRCDownloaded,                                     // 短文件名, crc文件内容

    // 正在校验 crc32, md5
    Download_Checksuming,                                       // 短文件名, crc32, md5

    // 已校验
    Download_Checksumed,                                        // 短文件名, crc32, md5

    // 正在解压
    Download_Extracting,                                        // 短文件名

    // 已解压
    Download_Extracted,                                         // 短文件名

    // 下载结束( 含校验, 解压 )
    Download_Finished,                                          // 短文件名

    //////////////////////////////////////////////////////////////////////////
    // 标志位事件
    Download_Error_Begin,

    // 下面是下载相关错误事件

    // 下载被终止( crc, zip 被终止也是这个 )
    Download_Stoped,

    // 超时
    Download_Timeout,

    // fopen 文件错误
    Download_OpenFileError,

    // 取长度时出错
    Download_GetLengthError,

    // 初始化 curl 失败
    Download_InitCurlError,

    // 写文件错( 如果写文件函数回调 return 0 终止下载也会得到这个错误 )
    Download_WriteFileError,

    // 下载回应错误
    Download_ResponseError,


    // 下面是校验相关错误事件

    // CRC 文件内容格式错
    Download_CRCFormatError,

    // md5 校验错
    Download_MD5Error,

    // crc32 校验错
    Download_CRC32Error,

    // 下面是解压相关错误事件

    // zip 文件 fopen 失败
    Download_OpenZipError,

    // 读 zip 信息区错
    Download_ReadZipInfoError,

    // 读 解压信息 错
    Download_ReadFileInfoError,

    // 创建目录错
    Download_CreateDirError,

    // 打开 zip 中的文件失败
    Download_OpenZipFileError,

    // fopen 目标文件失败
    Download_OpenDestFileError,

    // 读 zip 中的文件失败
    Download_ReadZipFileError,

    // 读 zip 中的下一个文件 失败
    Download_ReadNextFileError,

    // 标志位事件
    Download_Error_End,
    //////////////////////////////////////////////////////////////////////////

    /************************************************************************/
    /* Load 相关                                                            */
    /************************************************************************/

    // 开始执行
    Load_Started,                                               // 总文件数, 要下载的文件个数, 要异步加载的贴图个数, 要同步加载的资源个数

    // 准备下载列表
    Load_DownloadInit,                                          // 要下载的文件总字节数

    // 开始下载 这批文件 
    Load_DownloadBegin,                                         // 文件全名, URL

    /////////////////////////////////
    // 这期间会产生 Download_ 系列事件, 即便是出错, 也不会立即退出( 除非所有 downloader 均出错 )
    /////////////////////////////////

    // 下载完成 
    Load_DownloadFinished,                                      // 文件全名, URL

    // 正在异步加载贴图
    Load_AsyncLoading,                                          // 文件名

    // 异步加载完成
    Load_AsyncLoaded,                                           // 文件名

    // 正在 cache
    Load_AsyncCaching,                                          // 文件名

    // 已 cache
    Load_AsyncCached,                                           // 文件名

    // 正在同步加载plist
    Load_SyncLoading,                                           // 文件名

    // 同步加载完成
    Load_SyncLoaded,                                            // 文件名

    // 整个 Load 过程完成
    Load_Finished,                                              //


    //////////////////////////////////////////////////////////////////////////
    // 标志位事件
    Load_Error_Begin,

    // 综合性载入错误  todo: 进一步细化
    Load_Error,                                                 // 错误信息

    // 下载失败( 所有下载器经过尝试均已失败 )
    Load_DownloadFailed,

    // 标志位事件
    Load_Error_End,
    //////////////////////////////////////////////////////////////////////////

};


// 文件下载状态
enum class FileStates : int
{
    // 已完成. 当步骤进行到此时, 需要调 fileIndexRow 的 updateState 存盘
    Finished,

    // 需要下
    NeedDownload,

    // 正在下
    Downloading,

    // 需要下 crc
    CRCNeedDownload,

    // 正在下 crc
    CRCDownloading,

    // 需要校验
    NeedChecksum,

    // 正在校验
    Checksuming,

    // 需要解压
    NeedExtract,

    // 正在解压
    Extracting
};


// 文件类型
enum class FileTypes : int
{
    Unknown,
    // 下面是各种图片/贴图
    TextureTypes_Begin = 100,                     // 占位符_开始( 用于判断是否为贴图 )   
    Jpg,
    Png,
    Pvr,
    Ccz,                                    // pvr.ccz
    Dds,
    Webp,
    TextureTypes_End,                       // 占位符_结束( 用于判断是否为贴图 )
    // 杂七杂八
    SoundTypes_Begin = 200,
    BgWav,                                  // 背景音乐
    BgMp3,                                  // 背景音乐
    BgCaf,                                  // 背景音乐
    BgOgg,                                  // 背景音乐
    Wav,                                    // 前景音效
    Mp3,                                    // 前景音效
    Caf,                                    // 前景音效
    Ogg,                                    // 前景音效
    SoundTypes_End,
    
    MiscTypes_Begin = 300,
    FramePlist,                             // sprite frame 声明档
    SpineAtlas,                             // spine 贴图声明档
    SpineJson,                              // spine 动画定义档
    Lua,                                    // 明文 lua script
    ParticlePlist,                          // particle quad 声明档
    Db,                                     // sqlite db
    TTF,                                    // true type font
    MiscTypes_End,
    // todo: others, 待补
};

// ExtSprite 的 shader 类型
enum class ShaderTypes
{
    None,
    Gray,
    Blur,
};


// ExtButton 的 states
enum class ButtonStates
{
    Normal,
    HighLighted,
    Disabled,
    Selected
};

// ExtButton 的 touch 事件
enum class ButtonTouchEvents
{
    Down,
    DragInside,
    DragOutside,
    DragEnter,
    DragExit,
    UpInside,
    UpOutside,
    Cancel,
};

// ExtScrollView 的 移动方向
enum class ScrollViewDirection
{
    // 横向
    Horizontal,

    // 纵向
    Vertical,

    // 双向
    Both
};

// ExtScrollView 的 action 事件( todo: 当前只实现了 Stoped 事件 )
// 理论上讲每个事件都不必跟参数( 可自行直接取值 )
enum class ScrollViewEvents
{
    Begin,

    // 手指正在划动( 每 frame )
    Draging,

    // 手指抬起后惯性移动( 每 frame )
    Moving,

    // 触及边缘, 手指放开的 弹回 正发生( 单次 )
    Bounce,

    // 完全停下来( 单次 )
    Stoped,
};

// 富文本每行基线对齐方式
enum class RichTextLineVAligns
{
    Top,
    Middle,
    Bottom
};






// 购买失败 的类型
enum class BuyFailedTypes
{
    BuyErrorUnknown,            // 这几个是买失败后的 PaymentTrans.error.code.  等同于 ios SK 打头的几个 enum
    BuyErrorClientInvalid,
    BuyErrorPaymentCancelled,
    BuyErrorPaymentInvalid,
    BuyErrorPaymentNotAllowed,
    BuyErrorStoreProductNotAvailable,
};

// 校验出错 的类型
enum class VerifyErrorTypes
{
    OpenUrlFailed,              // 打不开校验网址
    ResponseError,              // HTTP 响应错误
    ResponseFormatError,        // HTTP 响应数据格式错误
    VerifyTimeout,              // 去apple校验超时，客户端可能需要给出 重试按钮

    Xxxxxxxxx,                  // todo: more errors
    ApplicationIdError,
    WrongProductId,
};

enum class PurchaseEventTypes
{
    PullSuccess,                // 拉产品表成功
    Pulling,                    // 正在拉产品表( 服务器还未返回 )

    BuyInvalidId,               // 找不到或禁止的 product ID( 首先在 _products 里就没有找到 )

    Buying,                     // 正在买。对应 ios 事件 enum 为 SKPaymentTransactionStatePurchasing
    BuySuccess,                 // 买成功。对应 ios 事件 enum 为 SKPaymentTransactionStatePurchased
    BuyRestored,                // 周期订阅成功。对应 ios 事件 enum 为 SKPaymentTransactionStateRestored
    BuyFailed,                  // 买失败。对应 ios 事件 enum 为 SKPaymentTransactionStateFailed. 事件参数为 BuyFailedTypes

    // 购买成功后
    VerifySuccess,              // 校验成功（未记录或已记录），游戏服未发放：通知游戏服发放, 客户端等游戏服消息（含transId），之后关闭订单
    Verifyed,                   // 校验已通过, 游戏服已发放：直接关闭订单，客户端做相应提示
    VerifyedAnother,            // 校验已通过, 被充值角色并非当前角色, 游戏服已发放：直接关闭订单，客户端不提示
    VerifyTimeout,              // apple 校验超时， 客户端应该提示重试
    VerifyInvalidReceipt,       // 校验 status != 0, 客户端应提示非法操作之类
    VerifyError,                // 各种校验出错，参数里面含有日志

    // for exchange
    ExchangeSuccess,            // 兑换成功( 但不一定就充值成功，玩家须等待 )
    ExchangeUsed,               // 卡号已被用过
    ExchangeInvalid,            // 无效卡号
    ExchangeElapsed,            // 过期卡号
    ExchangeError,              // 各种出错，参数里面含有日志
};



#endif
