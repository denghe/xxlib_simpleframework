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

	XXNBSOCKETLIB_API void xxnbs_init() noexcept
	{
		XxNBSocket::SockInit();
	}

	XXNBSOCKETLIB_API XxNBSocket* xxnbs_new() noexcept
	{
		// 在头部留出 userdata 的空间
		auto p = malloc(sizeof(XxNBSocket) + sizeof(void*));

		// 不考虑内存不足
		return new ((void**)p + 1) XxNBSocket();
	}

	XXNBSOCKETLIB_API void xxnbs_delete(XxNBSocket* nbs) noexcept
	{
		if (!nbs) return;
		nbs->~XxNBSocket();
		free((void**)nbs - 1);
	}

	XXNBSOCKETLIB_API void xxnbs_set_userdata(XxNBSocket* nbs, void* userdata) noexcept
	{
		*((void**)nbs - 1) = userdata;
	}

	XXNBSOCKETLIB_API void* xxnbs_get_userdata(XxNBSocket* nbs) noexcept
	{
		return *((void**)nbs - 1);
	}

	XXNBSOCKETLIB_API void xxnbs_set_address(XxNBSocket* nbs, char const* ip, int port) noexcept
	{
		return nbs->SetAddress(ip, (uint16_t)port);
	}
	XXNBSOCKETLIB_API int xxnbs_set_address6(XxNBSocket* nbs, char const* ip, int port) noexcept
	{
		return nbs->SetAddress6(ip, (uint16_t)port);
	}

	XXNBSOCKETLIB_API int xxnbs_connect(XxNBSocket* nbs, int sec, int usec) noexcept
	{
		return nbs->Connect(sec, usec);
	}

	XXNBSOCKETLIB_API void xxnbs_disconnect(XxNBSocket* nbs, int delayTicks) noexcept
	{
		nbs->Disconnect(delayTicks);
	}

	XXNBSOCKETLIB_API int xxnbs_update(XxNBSocket* nbs, int sec, int usec) noexcept
	{
		return nbs->Update(sec, usec);
	}

	XXNBSOCKETLIB_API int xxnbs_get_state(XxNBSocket* nbs) noexcept
	{
		return (int)nbs->state;
	}

	XXNBSOCKETLIB_API int xxnbs_get_ticks(XxNBSocket* nbs) noexcept
	{
		return (int)nbs->ticks;
	}

	XXNBSOCKETLIB_API int xxnbs_send(XxNBSocket* nbs, char const* buf, int offset, int dataLen) noexcept
	{
		return nbs->Send(buf + offset, dataLen);
	}

	XXNBSOCKETLIB_API char const* xxnbs_peek(XxNBSocket* nbs, int* dataLen) noexcept
	{
		if (nbs->recvBufs.empty()) return nullptr;
		auto& pkg = nbs->recvBufs.front();
		*dataLen = pkg.dataLen;
		return pkg.buf;
	}

	XXNBSOCKETLIB_API void xxnbs_pop(XxNBSocket* nbs) noexcept
	{
		nbs->recvBufs.pop_front();
	}

}
