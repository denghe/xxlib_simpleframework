#include "Precompile.h"
#if __WIN
#include <WinSock2.h>
#endif


int NetUtils::getSockErrNo()
{
#if __WIN
    int e = WSAGetLastError();
    return e == WSAEWOULDBLOCK ? EWOULDBLOCK : e;
#else
    return errno;
#endif
}

int NetUtils::getSockOpt( Socket_t s, int level, int optname, void * optval, SockLen_t * optlen )
{
#if __WIN
    return getsockopt( s, level, optname, (char*)optval, optlen );
#else
    return getsockopt( s, level, optname, optval, optlen );
#endif
}

int NetUtils::closeSocket( Socket_t s )
{
#if __WIN
    return closesocket( s );
#else
    return ::close( s );
#endif
}

int NetUtils::handleWriteReady( Socket_t s, int timeoutMS )
{
    timeval tv;
    if( timeoutMS < 0 ) timeoutMS = -1;
    else
    {
        tv.tv_sec = timeoutMS / 1000;
        tv.tv_usec = ( timeoutMS % 1000 ) * 1000;
    }

    int width = s + 1;
    fd_set wset, eset;

    int rtv = 0;
    while( true )
    {
        FD_ZERO( &wset );
        FD_ZERO( &eset );
        FD_SET( s, &wset );
        FD_SET( s, &eset );

        rtv = ::select( width, 0, &wset, &eset, timeoutMS == -1 ? 0 : &tv );    // ×èÈû »ò ³¬Ê±

        if( rtv == -1 && errno == EINTR ) continue; // interupt by signal

        break; // timeout or ready
    }
    return rtv;
}

int NetUtils::setNonBlockMode( Socket_t s )
{
#ifdef _WIN32
    u_long v = -1;
    return ::ioctlsocket( s, FIONBIO, &v );
#else
    int flag = ::fcntl( s, F_GETFL, 0 );
    if( ( flag & O_NONBLOCK ) == O_NONBLOCK ) return 0; // already nonblock
    return ::fcntl( s, F_SETFL, flag | O_NONBLOCK );
#endif
}

int NetUtils::setBlockMode( Socket_t s )
{
#ifdef _WIN32
    u_long v = 0;
    return ::ioctlsocket( s, FIONBIO, &v );
#else
    int flag = ::fcntl( s, F_GETFL, 0 );
    if( ( flag & O_NONBLOCK ) == O_NONBLOCK )
    { // nonblock
        flag &= ~O_NONBLOCK;
        return fcntl( s, F_SETFL, flag );
    }
    return 0;
#endif
}
