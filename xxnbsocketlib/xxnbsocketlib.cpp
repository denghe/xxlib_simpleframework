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






XXNBSOCKETLIB_API int XxNBSocket_SendXxBBuffer(void* nbs, void* bb) noexcept
{
	auto self = (XxNBSocket*)nbs;
	return self->Send((XxBBuffer*)bb);
}

XXNBSOCKETLIB_API void* XxNBSocket_PeekRecvXxBBuffer(void* nbs) noexcept
{
	auto self = (XxNBSocket*)nbs;
	if (self->recvBufs.empty()) return nullptr;
	return &self->recvBufs.front();					// todo: 从池创建新的BB 将 front 的数据填进去, pop front 后返回. 改名为 PopRecvXxBuffer. 老接口可废弃
}

XXNBSOCKETLIB_API void* XxBBuffer_New(void* mp, char* buf, int dataLen) noexcept
{
	if (!mp) return nullptr;
	try
	{
		return ((XxMemPool*)mp)->Create<XxBBuffer>(buf, dataLen);
	}
	catch (...)
	{
		return nullptr;
	}
}

XXNBSOCKETLIB_API void XxBBuffer_Delete(void* bb) noexcept
{
	if (!bb) return;
	auto self = (XxBBuffer*)bb;
	self->mempool->Release(self);
}

XXNBSOCKETLIB_API void XxBBuffer_Reserve(void* bb, int capacity) noexcept
{
	auto self = (XxBBuffer*)bb;
	self->Reserve(capacity);
}

XXNBSOCKETLIB_API void XxBBuffer_Clear(void* bb) noexcept
{
	auto self = (XxBBuffer*)bb;
	self->Clear();
}

XXNBSOCKETLIB_API int XxBBuffer_GetDataLen(void* bb) noexcept
{
	auto self = (XxBBuffer*)bb;
	return self->dataLen;
}

XXNBSOCKETLIB_API int XxBBuffer_GetOffset(void* bb) noexcept
{
	auto self = (XxBBuffer*)bb;
	return self->offset;
}





XXNBSOCKETLIB_API void XxBBuffer_WriteBoolean(void* bb, bool v) noexcept
{
	auto self = (XxBBuffer*)bb;
	self->Write(v);
}
XXNBSOCKETLIB_API void XxBBuffer_WriteInt8(void* bb, char v) noexcept
{
	auto self = (XxBBuffer*)bb;
	self->Write(v);
}
XXNBSOCKETLIB_API void XxBBuffer_WriteInt16(void* bb, short v) noexcept
{
	auto self = (XxBBuffer*)bb;
	self->Write(v);
}
XXNBSOCKETLIB_API void XxBBuffer_WriteInt32(void* bb, int v) noexcept
{
	auto self = (XxBBuffer*)bb;
	self->Write(v);
}
XXNBSOCKETLIB_API void XxBBuffer_WriteInt64(void* bb, long long v) noexcept
{
	auto self = (XxBBuffer*)bb;
	self->Write(v);
}
XXNBSOCKETLIB_API void XxBBuffer_WriteUInt8(void* bb, unsigned char v) noexcept
{
	auto self = (XxBBuffer*)bb;
	self->Write(v);
}
XXNBSOCKETLIB_API void XxBBuffer_WriteUInt16(void* bb, unsigned short v) noexcept
{
	auto self = (XxBBuffer*)bb;
	self->Write(v);
}
XXNBSOCKETLIB_API void XxBBuffer_WriteUInt32(void* bb, unsigned int v) noexcept
{
	auto self = (XxBBuffer*)bb;
	self->Write(v);
}
XXNBSOCKETLIB_API void XxBBuffer_WriteUInt64(void* bb, unsigned long long v) noexcept
{
	auto self = (XxBBuffer*)bb;
	self->Write(v);
}
XXNBSOCKETLIB_API void XxBBuffer_WriteFloat(void* bb, float v) noexcept
{
	auto self = (XxBBuffer*)bb;
	self->Write(v);
}
XXNBSOCKETLIB_API void XxBBuffer_WriteDouble(void* bb, double v) noexcept
{
	auto self = (XxBBuffer*)bb;
	self->Write(v);
}

XXNBSOCKETLIB_API void XxBBuffer_WritePodBoolean(void* bb, bool v) noexcept
{
	auto self = (XxBBuffer*)bb;
	self->WritePod(v);
}
XXNBSOCKETLIB_API void XxBBuffer_WritePodInt8(void* bb, char v) noexcept
{
	auto self = (XxBBuffer*)bb;
	self->WritePod(v);
}
XXNBSOCKETLIB_API void XxBBuffer_WritePodInt16(void* bb, short v) noexcept
{
	auto self = (XxBBuffer*)bb;
	self->WritePod(v);
}
XXNBSOCKETLIB_API void XxBBuffer_WritePodInt32(void* bb, int v) noexcept
{
	auto self = (XxBBuffer*)bb;
	self->WritePod(v);
}
XXNBSOCKETLIB_API void XxBBuffer_WritePodInt64(void* bb, long long v) noexcept
{
	auto self = (XxBBuffer*)bb;
	self->WritePod(v);
}
XXNBSOCKETLIB_API void XxBBuffer_WritePodUInt8(void* bb, unsigned char v) noexcept
{
	auto self = (XxBBuffer*)bb;
	self->WritePod(v);
}
XXNBSOCKETLIB_API void XxBBuffer_WritePodUInt16(void* bb, unsigned short v) noexcept
{
	auto self = (XxBBuffer*)bb;
	self->WritePod(v);
}
XXNBSOCKETLIB_API void XxBBuffer_WritePodUInt32(void* bb, unsigned int v) noexcept
{
	auto self = (XxBBuffer*)bb;
	self->WritePod(v);
}
XXNBSOCKETLIB_API void XxBBuffer_WritePodUInt64(void* bb, unsigned long long v) noexcept
{
	auto self = (XxBBuffer*)bb;
	self->WritePod(v);
}
XXNBSOCKETLIB_API void XxBBuffer_WritePodFloat(void* bb, float v) noexcept
{
	auto self = (XxBBuffer*)bb;
	self->WritePod(v);
}
XXNBSOCKETLIB_API void XxBBuffer_WritePodDouble(void* bb, double v) noexcept
{
	auto self = (XxBBuffer*)bb;
	self->WritePod(v);
}

XXNBSOCKETLIB_API void XxBBuffer_WriteLenData(void* bb, char* s, int len) noexcept
{
	auto self = (XxBBuffer*)bb;
	self->WriteLenData(s, len);
}

XXNBSOCKETLIB_API int XxBBuffer_ReadBoolean(void* bb, bool* v) noexcept
{
	auto self = (XxBBuffer*)bb;
	return self->NoExceptRead(v);
}
XXNBSOCKETLIB_API int XxBBuffer_ReadInt8(void* bb, char* v) noexcept
{
	auto self = (XxBBuffer*)bb;
	return self->NoExceptRead(v);
}
XXNBSOCKETLIB_API int XxBBuffer_ReadInt16(void* bb, short* v) noexcept
{
	auto self = (XxBBuffer*)bb;
	return self->NoExceptRead(v);
}
XXNBSOCKETLIB_API int XxBBuffer_ReadInt32(void* bb, int* v) noexcept
{
	auto self = (XxBBuffer*)bb;
	return self->NoExceptRead(v);
}
XXNBSOCKETLIB_API int XxBBuffer_ReadInt64(void* bb, long long* v) noexcept
{
	auto self = (XxBBuffer*)bb;
	return self->NoExceptRead(v);
}
XXNBSOCKETLIB_API int XxBBuffer_ReadUInt8(void* bb, unsigned char* v) noexcept
{
	auto self = (XxBBuffer*)bb;
	return self->NoExceptRead(v);
}
XXNBSOCKETLIB_API int XxBBuffer_ReadUInt16(void* bb, unsigned short* v) noexcept
{
	auto self = (XxBBuffer*)bb;
	return self->NoExceptRead(v);
}
XXNBSOCKETLIB_API int XxBBuffer_ReadUInt32(void* bb, unsigned int* v) noexcept
{
	auto self = (XxBBuffer*)bb;
	return self->NoExceptRead(v);
}
XXNBSOCKETLIB_API int XxBBuffer_ReadUInt64(void* bb, unsigned long long* v) noexcept
{
	auto self = (XxBBuffer*)bb;
	return self->NoExceptRead(v);
}
XXNBSOCKETLIB_API int XxBBuffer_ReadFloat(void* bb, float* v) noexcept
{
	auto self = (XxBBuffer*)bb;
	return self->NoExceptRead(v);
}
XXNBSOCKETLIB_API int XxBBuffer_ReadDouble(void* bb, double* v) noexcept
{
	auto self = (XxBBuffer*)bb;
	return self->NoExceptRead(v);
}

XXNBSOCKETLIB_API int XxBBuffer_ReadPodBoolean(void* bb, bool* v) noexcept
{
	auto self = (XxBBuffer*)bb;
	return self->NoExceptReadPod(v);
}
XXNBSOCKETLIB_API int XxBBuffer_ReadPodInt8(void* bb, char* v) noexcept
{
	auto self = (XxBBuffer*)bb;
	return self->NoExceptReadPod(v);
}
XXNBSOCKETLIB_API int XxBBuffer_ReadPodInt16(void* bb, short* v) noexcept
{
	auto self = (XxBBuffer*)bb;
	return self->NoExceptReadPod(v);
}
XXNBSOCKETLIB_API int XxBBuffer_ReadPodInt32(void* bb, int* v) noexcept
{
	auto self = (XxBBuffer*)bb;
	return self->NoExceptReadPod(v);
}
XXNBSOCKETLIB_API int XxBBuffer_ReadPodInt64(void* bb, long long* v) noexcept
{
	auto self = (XxBBuffer*)bb;
	return self->NoExceptReadPod(v);
}
XXNBSOCKETLIB_API int XxBBuffer_ReadPodUInt8(void* bb, unsigned char* v) noexcept
{
	auto self = (XxBBuffer*)bb;
	return self->NoExceptReadPod(v);
}
XXNBSOCKETLIB_API int XxBBuffer_ReadPodUInt16(void* bb, unsigned short* v) noexcept
{
	auto self = (XxBBuffer*)bb;
	return self->NoExceptReadPod(v);
}
XXNBSOCKETLIB_API int XxBBuffer_ReadPodUInt32(void* bb, unsigned int* v) noexcept
{
	auto self = (XxBBuffer*)bb;
	return self->NoExceptReadPod(v);
}
XXNBSOCKETLIB_API int XxBBuffer_ReadPodUInt64(void* bb, unsigned long long* v) noexcept
{
	auto self = (XxBBuffer*)bb;
	return self->NoExceptReadPod(v);
}
XXNBSOCKETLIB_API int XxBBuffer_ReadPodFloat(void* bb, float* v) noexcept
{
	auto self = (XxBBuffer*)bb;
	return self->NoExceptReadPod(v);
}
XXNBSOCKETLIB_API int XxBBuffer_ReadPodDouble(void* bb, double* v) noexcept
{
	auto self = (XxBBuffer*)bb;
	return self->NoExceptReadPod(v);
}

XXNBSOCKETLIB_API int XxBBuffer_ReadLenData(void* bb, char** s, int* len) noexcept
{
	auto self = (XxBBuffer*)bb;
	return self->NoExceptReadLenData((char const**)s, (uint32_t*)len);
}
