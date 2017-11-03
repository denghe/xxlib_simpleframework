#ifndef _NETCORE_H_
#define _NETCORE_H_

class Hosts;
class NetClient;
struct NetContext
{
    NetContext( uint32 ipv4, uint16 port, int timeoutMS, NetClient * client );

    uint32      ip;
    uint16      port;
    int         timeoutMS;
    NetClient*  client;
};


class NetEngine
{
public:
    MAKE_INSTANCE_H( NetEngine );

    // 初始化平台 socket
    NetEngine();

    // 反初始化平台 socket
    ~NetEngine();

    // for 网络情况变更事件 平台代码中 调用 设置当前物理网络类型
    static void setNetType( NetTypes e );

    // 得到当前物理网络类型
    NetTypes getNetType();

    // 试 close 所有 NetClient 并同步等待其关完
    void shutdown();

    // 为 client 创建工作线程
    bool makeWorker( NetClient * token, const char * name, uint16 port, int timeoutMS = -1 );

    // 试 close 所有 NetClient
    void closeAll();

    // 更新 所有 Socket？
    void update();

    friend NetClient;
private:
    // 通过连接参数上下文试建立 tcp 连接
    static bool connect( NetContext * ctx );

    // client 工作线程函数
    void * workerProcess( NetContext * ctx );

    ThreadSafeQueue<NetClient*>     _clients;
    std::atomic<int>                _numWorkers;
    static NetTypes                 _netType;
};



#endif
