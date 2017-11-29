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

extern "C" {

XXNBSOCKETLIB_API void Init_Sock() noexcept
{
	XxNBSocket::SockInit();
}



XXNBSOCKETLIB_API void* XxMemPool_New() noexcept
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

XXNBSOCKETLIB_API void* XxNBSocket_New(void* mp) noexcept
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

XXNBSOCKETLIB_API void XxNBSocket_Delete(void* nbs) noexcept
{
	if (!nbs) return;
	auto self = (XxNBSocket*)nbs;
	self->mempool->Release(self);
}

XXNBSOCKETLIB_API void XxMemPool_Delete(void* mp) noexcept
{
	if (!mp) return;
	delete (XxMemPool*)mp;
}

XXNBSOCKETLIB_API void XxNBSocket_SetAddress(void* nbs, char* ip, uint16_t port) noexcept
{
	auto self = (XxNBSocket*)nbs;
	return self->SetAddress(ip, port);
}

XXNBSOCKETLIB_API int XxNBSocket_Connect(void* nbs, int sec, int usec) noexcept
{
	auto self = (XxNBSocket*)nbs;
	return self->Connect(sec, usec);
}

XXNBSOCKETLIB_API void XxNBSocket_Disconnect(void* nbs, int delayTicks) noexcept
{
	auto self = (XxNBSocket*)nbs;
	self->Disconnect(delayTicks);
}

XXNBSOCKETLIB_API int XxNBSocket_Update(void* nbs, int sec, int usec) noexcept
{
	auto self = (XxNBSocket*)nbs;
	return self->Update(sec, usec);
}

XXNBSOCKETLIB_API int XxNBSocket_GetState(void* nbs) noexcept
{
	auto self = (XxNBSocket*)nbs;
	return (int)self->state;
}

XXNBSOCKETLIB_API int XxNBSocket_GetTicks(void* nbs) noexcept
{
	auto self = (XxNBSocket*)nbs;
	return (int)self->ticks;
}

XXNBSOCKETLIB_API int XxNBSocket_Send(void* nbs, char* buf, int dataLen) noexcept
{
	auto self = (XxNBSocket*)nbs;
	return self->Send(buf, dataLen);
}

XXNBSOCKETLIB_API char* XxNBSocket_PeekRecv(void* nbs, int* dataLen) noexcept
{
	auto self = (XxNBSocket*)nbs;
	if (self->recvBufs.empty()) return nullptr;
	auto& pkg = self->recvBufs.front();
	*dataLen = pkg.dataLen;
	return pkg.buf;
}

XXNBSOCKETLIB_API void XxNBSocket_PopRecv(void* nbs) noexcept
{
	auto self = (XxNBSocket*)nbs;
	self->recvBufs.pop_front();
}

}

