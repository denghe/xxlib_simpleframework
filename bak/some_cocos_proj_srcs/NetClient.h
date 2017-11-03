#ifndef _NETCLIENT_H_
#define _NETCLIENT_H_

typedef pair<NetEvents, NetPacket*> NetClientEventType;

// 实现指令接收后打包，确保其完整性后回调事件
class NetClient : public Ref
{
public:
    // 将该份 client 注册到 NetEngine 的 _clients
    NetClient();

    // 将该份 client 反注册到 NetEngine 的 _clients
    ~NetClient();

    static NetClient* create();

    // 连接是否畅通
    bool isAlive() const;

    // 连接是否关闭
    bool isClosed() const;

    // 连接是否死亡
    bool isDead() const;

    // 取当前连接状态
    SocketStates getState() const;

    // 发数组
    bool send( const char * buf, int len ); 

    // 发包
    bool send( NetPacket * p );

    // 继续发没发完的( 这通常位于每 frame 初期 call )
    void sendLeft();

    // 设置连接相关数据
    void setParms( string ipv4, uint16 port, int timeoutMS = 8000 );

    // 试创建连接( 使用 setParms 设置的参数 )，失败将返回 false
    bool connect();

    // 设置连接信息 并 试创建连接，失败将返回 false
    bool connect( string ipv4, uint16 port, int timeoutMS = 8000 );

    // 关闭连接
    void close();

    // 注册 lua 闭包事件回调
    void registerNetEventHandler( LUA_FUNCTION fn );

    // 反注册 lua 闭包回调
    void unregisterNetEventHandler();

    // 清事件队列
    void clear();

    friend NetEngine;
    friend Hosts;
protected:
    // 强发定量数据
    int sendCore( const char * buf, int len );

    // 收数据. 填到 _readBuf, 并调 onReceived
    int receive();

    // 主用于每 frame 抛事件到 lua
    void update();

    // lock 修改 state
    void setState( SocketStates s );

    std::mutex      _mutex;
    Socket_t        _socket;
    SocketStates    _state;
    ListBuffer      _sendBuf;
    NetPacket       _readBuf;

    // 各种事件处理函数
    void onConnected();
    void onConnectFailed();
    void onClosed();
    void onReceived( NetPacket & p );  // p 即为 readBuf, 需要根据未处理的数据长来设置 woffset

    string                              _ipv4;
    uint16                              _port;
    int                                 _timeoutMS;
    LUA_FUNCTION                        _luaFN;
    ThreadSafeQueue<NetClientEventType> _evnts;

    bool                                _threadRunning;
};


#endif
