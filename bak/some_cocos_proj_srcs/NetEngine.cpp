#include "Precompile.h"
#if __WIN
#include <WinSock2.h>
#endif


NetContext::NetContext( uint32 ipv4, uint16 port, int timeoutMS, NetClient * nc )
: ip( ipv4 )
, port( port )
, timeoutMS( timeoutMS )
, client( nc )
{
}

MAKE_INSTANCE_CPP( NetEngine );

void NetEngine::shutdown()
{
    closeAll();
    while( _numWorkers ) sleepMS( 100 );     // wait all workers dead
}

bool NetEngine::makeWorker( NetClient * token, const char * name, uint16 port, int timeoutMS )
{
    unsigned naddr;
    hostent * entry = ::gethostbyname( name );        // parse name
    if( entry ) naddr = *(unsigned *)entry->h_addr;
    else return false;

    auto ctx = new NetContext( naddr, port, timeoutMS, token );
    ++_numWorkers;

    std::thread( &NetEngine::workerProcess, this, ctx ).detach();

    return true;
}

bool NetEngine::connect( NetContext * ctx )
{
    Socket_t s = socket( AF_INET, SOCK_STREAM, 0 );      // create socket

    sockaddr_in addr;                                   // create addr
    addr.sin_family = AF_INET;
    addr.sin_port = htons( ctx->port );
    addr.sin_addr.s_addr = ctx->ip;
    memset( addr.sin_zero, 0, sizeof( addr.sin_zero ) );

    int r;
    do
    {
        NetUtils::setNonBlockMode( s );                        // set mode
        do
        {
            r = ::connect( s, (sockaddr *)&addr, sizeof( addr ) );
        } while( r == -1 && NetUtils::getSockErrNo() == EINTR );    // connect

        if( r == -1 )
        {
            int e = NetUtils::getSockErrNo();
            if( e != EINPROGRESS &&
                e != EWOULDBLOCK )
                break;

            r = NetUtils::handleWriteReady( s, ctx->timeoutMS );
            if( r <= 0 ) r = -1;
            else
            {
                SockLen_t len = 4;
                if( NetUtils::getSockOpt( s, SOL_SOCKET, SO_ERROR, &r, &len ) == -1
                    || r != 0 )
                    r = -1;
            }
        }
    } while( 0 );

    if( r == -1 )
    {
        NetUtils::closeSocket( s );
        return false;
    }

    ctx->client->_socket = s;
    return true;
}

void * NetEngine::workerProcess( NetContext * ctx )
{
    bool connectRtv = NetEngine::connect( ctx );
    auto nc = ctx->client;
    delete ctx;

    if( connectRtv )
    {
        //_clients.push( nc );
        nc->onConnected();
    }
    else
    {
        nc->onConnectFailed();
        --_numWorkers;
        return 0;
    }

    nc->_state = SocketStates::Alive;

    auto maxfd = nc->_socket;
    fd_set rset;
    timeval tv;
    int r;
    nc->_threadRunning = true;
    while( !nc->isClosed() )
    {    // begin net loop
        FD_ZERO( &rset );
        FD_SET( nc->_socket, &rset );

        tv.tv_sec = (int)NetConfigs::WorkerSelectTimeOutSecond;
        tv.tv_usec = (int)NetConfigs::WorkerSelectTimeOutUSecond;

        r = select( maxfd + 1, &rset, 0, 0, &tv );
        if( r == -1 ) break;
        if( r > 0 && FD_ISSET( nc->_socket, &rset ) ) nc->receive();
    }
    //_clients.erase( [ nc ]( NetClient*& item ) { return nc == item; } );
    //nc->onClosed();
    //nc->_readBuf.woffset( 0 );
    //nc->setState( SocketStates::Dead );
    nc->_threadRunning = false;
    --_numWorkers;

    return 0;
}

NetEngine::NetEngine()
: _numWorkers( 0 )
{
#if defined(__WIN32)
    WSADATA d;
    WSAStartup( MAKEWORD( 2, 2 ), &d );
#else
    // 必须忽略！！！
    // 当远程连接断开时，
    // 继续write数据，会引发该信号量
    signal( SIGPIPE, SIG_IGN );
#endif
}

NetEngine::~NetEngine()
{
    shutdown();
#if defined (__WIN32)
    WSACleanup();
#endif
}

void NetEngine::setNetType( NetTypes e ) // for 网络情况变更事件 平台代码中 调用
{
    _netType = e;
    if( e == NetTypes::None )
    {
        if( _instance )                 // 这里有可能因 android 机器慢而未初始化
        {
            _instance->closeAll();
        }
    }
}

NetTypes NetEngine::getNetType()
{
    return _netType;
}

void NetEngine::closeAll()
{
    _clients.foreach( []( NetClient*& c ) { c->close(); } );    // 当网络状态变为　不可用　时调之
}

void NetEngine::update()
{
    _clients.foreach( []( NetClient*& c ) { c->update(); } );
}

NetTypes NetEngine::_netType;

