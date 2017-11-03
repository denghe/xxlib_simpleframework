#ifndef __NETUTILS_H__
#define __NETUTILS_H__


// for cross platform
#if __WIN
typedef size_t      Socket_t;       // 原文是 SOCKET  因不方便在 .h 中暴露 windows 系列声明 故 写成 size_t
typedef int         SockLen_t;
#else
typedef int         Socket_t;
typedef socklen_t   SockLen_t;
#endif


namespace NetUtils
{

    int getSockErrNo();

    int getSockOpt( Socket_t s, int level, int optname, void * optval, SockLen_t * optlen );

    int closeSocket( Socket_t s );

    int handleWriteReady( Socket_t s, int timeoutMS );

    int setNonBlockMode( Socket_t s );

    int setBlockMode( Socket_t s );

}


#endif
