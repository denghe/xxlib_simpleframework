#ifndef _COMMONINCLUDES_H_
#define _COMMONINCLUDES_H_

/* Hardware, Compiler defines */
#include "Platform.h"

/* Common headers */
#if __WIN
// 注释掉是因为 生成 lua bind 的时候 生成器会扫这几个 .h , 然后就是各种找不到文件. 具体引用已挪到具体 cpp 中
//#define WIN32_LEAN_AND_MEAN
//#include <WinSock2.h>
//#include <windows.h>
//#include <tchar.h>
//#include <malloc.h>
//#include <direct.h>
//#include <intrin.h>
//#include <io.h>
#elif __IOS
#include <stdio.h>
#include <netdb.h>
#include <unistd.h>
#include <malloc/malloc.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#elif __ANDROID
#include <stdio.h>
#include <netdb.h>
#include <asm/page.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <netinet/tcp.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <malloc.h>
#else
#include <stdio.h>
#include <sys/time.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#endif

#include <string>
#include <vector>
#include <list>
#include <deque>
#include <queue>
#include <map>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <bitset>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <random>
#include <memory>
#include <new>

#include <type_traits>
#include <array>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <atomic>
#include <functional>
#include <chrono>
#include <locale>

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <fcntl.h>

#endif
