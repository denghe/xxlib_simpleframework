#include "xxnbsocket.h"
#include "xxnbsocketlib.h"

#ifdef _WIN32

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

#endif

XXNBSOCKETLIB_API void SockInit()
{
	XxNBSocket::SockInit();
}



XXNBSOCKETLIB_API void* NewXxMemPool()
{
	try
	{
		return new XxMemPool();
	}
	catch (...)
	{
		return nullptr;
	}
}

XXNBSOCKETLIB_API void* NewXxNBSocket(void* mp)
{
	if (!mp) return nullptr;
	try
	{
		return ((XxMemPool*)mp)->Create<XxNBSocket>();
	}
	catch (...)
	{
		return nullptr;
	}
}

XXNBSOCKETLIB_API void DeleteXxNBSocket(void* nbs)
{
	if (!nbs) return;
	XxMemPool::Get(nbs).Release((XxNBSocket*)nbs);
}

XXNBSOCKETLIB_API void DeleteXxMemPool(void* mp)
{
	delete (XxMemPool*)mp;
}

XXNBSOCKETLIB_API void SetAddress(void* nbs, char* ip, uint16_t port)
{
	auto self = (XxNBSocket*)nbs;
	return self->SetAddress(ip, port);
}

XXNBSOCKETLIB_API int Connect(void* nbs, int sec, int usec)
{
	auto self = (XxNBSocket*)nbs;
	return self->Connect(sec, usec);
}

XXNBSOCKETLIB_API void Disconnect(void* nbs, int delayTicks)
{
	auto self = (XxNBSocket*)nbs;
	self->Disconnect(delayTicks);
}

XXNBSOCKETLIB_API int Update(void* nbs, int sec, int usec)
{
	auto self = (XxNBSocket*)nbs;
	return self->Update(sec, usec);
}

XXNBSOCKETLIB_API int GetState(void* nbs)
{
	auto self = (XxNBSocket*)nbs;
	return (int)self->state;
}

XXNBSOCKETLIB_API int GetTicks(void* nbs)
{
	auto self = (XxNBSocket*)nbs;
	return (int)self->ticks;
}

XXNBSOCKETLIB_API int Send(void* nbs, char* buf, int dataLen)
{
	auto self = (XxNBSocket*)nbs;
	return self->Send(buf, dataLen);
}

XXNBSOCKETLIB_API char* PeekRecv(void* nbs, int* dataLen)
{
	auto self = (XxNBSocket*)nbs;
	if (self->recvBufs.empty()) return nullptr;
	auto& pkg = self->recvBufs.front();
	*dataLen = pkg.dataLen;
	return pkg.buf;
}

XXNBSOCKETLIB_API void PopRecv(void* nbs)
{
	auto self = (XxNBSocket*)nbs;
	self->recvBufs.pop_front();
}
