#pragma once
#ifdef _WIN32
#include <SDKDDKVer.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#ifdef xxnbsocketlib_EXPORTS
#define XXNBSOCKETLIB_API __declspec(dllexport)
#else
#define XXNBSOCKETLIB_API __declspec(dllimport)
#endif
#else
#define XXNBSOCKETLIB_API
#endif

#ifdef __cplusplus
extern "C" {
#endif

	// 初始化网络系统( WSAStartup / signal ). 只需要一开始执行一次.
	XXNBSOCKETLIB_API void Init_Sock();

	// 建 XxMemPool
	XXNBSOCKETLIB_API void* XxMemPool_New();

	// 建 XxNBSocket( 传入 XxMemPool )
	XXNBSOCKETLIB_API void* XxNBSocket_New(void* mp);

	// 杀 XxNBSocket
	XXNBSOCKETLIB_API void XxNBSocket_Delete(void* nbs);

	// 杀 XxMemPool( 在 XxNBSocket 之后再杀 )
	XXNBSOCKETLIB_API void XxMemPool_Delete(void* mp);

	// 设 ip & port
	XXNBSOCKETLIB_API void XxNBSocket_SetAddress(void* nbs, char* ip, uint16_t port);

	// 开始连接. 可传入阻塞时长
	// 返回负数 表示出错. 0 表示没发生错误 但也没连上. 1 表示连接成功
	XXNBSOCKETLIB_API int XxNBSocket_Connect(void* nbs, int sec, int usec);

	// 断开连接. 可传入延迟多少 ticks 断开. 0 立即断开.
	XXNBSOCKETLIB_API void XxNBSocket_Disconnect(void* nbs, int delayTicks);

	// 帧驱动. 可传入阻塞时长( 仅对 Connecting 状态有效 )
	// 返回负数表示出错. 0 表示没发生错误
	XXNBSOCKETLIB_API int XxNBSocket_Update(void* nbs, int sec, int usec);

	// 取当前状态. 
	// Disconnected,		// 0: 初始状态 / 出错 / 成功断开后
	// Connecting,			// 1: 执行 Connect 之后
	// Connected,			// 2: 握手并进入可收发状态之后
	// Disconnecting,		// 3: 执行 Disconnect( 延迟断开时长 ) 之后
	XXNBSOCKETLIB_API int XxNBSocket_GetState(void* nbs);

	// 取当前状态已持续的 ticks( Disconnecting 除外 )
	XXNBSOCKETLIB_API int XxNBSocket_GetTicks(void* nbs);

	// 发一段数据. 复制模式. 发送方不需要保持该数据不变.
	// 返回负数表示出错. 0 表示成功放入待发送队列或无数据可发. > 0 表示已立刻发送成功的长度. 
	// 如果有剩下部分, 会放入待发送队列, 在下次 Update 时继续发
	XXNBSOCKETLIB_API int XxNBSocket_Send(void* nbs, char* buf, int dataLen);

	// 如果接收缓冲区有包, 将返回 buf 指针填充长度. 否则返回 null( 表示无数据可取 )
	// 流程: while( buf = PeekRecv( &dataLen ) ) {  ... PopRecv(); }
	XXNBSOCKETLIB_API char* XxNBSocket_PeekRecv(void* nbs, int* dataLen);

	// 对于已处理的 PeekRecv 的数据, 用这个函数来弹出删掉, 以便继续 Peek 下一条.
	XXNBSOCKETLIB_API void XxNBSocket_PopRecv(void* nbs);



	// XxNBSocket_Send 的 XxBBuffer 版
	XXNBSOCKETLIB_API int XxNBSocket_SendXxBBuffer(void* nbs, void* bb);

	// XxNBSocket_PeekRecv 的 XxBBuffer 版
	XXNBSOCKETLIB_API void* XxNBSocket_PeekRecvXxBBuffer(void* nbs);

	/********************************************************************************************/
	// XxBBuffer 相关
	/********************************************************************************************/

	// 建 XxBBuffer( 传入 XxMemPool )
	XXNBSOCKETLIB_API void* XxBBuffer_New(void* mp, char* buf, int dataLen);

	// 删 XxBBuffer
	XXNBSOCKETLIB_API void XxBBuffer_Delete(void* bb);

	// 扩容
	XXNBSOCKETLIB_API void XxBBuffer_Reserve(void* bb, int capacity);

	// 清长度和读偏移
	XXNBSOCKETLIB_API void XxBBuffer_Clear(void* bb);

	// 返回数据长度
	XXNBSOCKETLIB_API int XxBBuffer_GetDataLen(void* bb);

	// 返回读偏移量
	XXNBSOCKETLIB_API int XxBBuffer_GetOffset(void* bb);


	// 各种变长写入指定数据类型, 成功返回 0. 失败返回错误码.
	XXNBSOCKETLIB_API void XxBBuffer_WriteBoolean(void* bb, bool v);
	XXNBSOCKETLIB_API void XxBBuffer_WriteInt8(void* bb, char v);
	XXNBSOCKETLIB_API void XxBBuffer_WriteInt16(void* bb, short v);
	XXNBSOCKETLIB_API void XxBBuffer_WriteInt32(void* bb, int v);
	XXNBSOCKETLIB_API void XxBBuffer_WriteInt64(void* bb, long long v);
	XXNBSOCKETLIB_API void XxBBuffer_WriteUInt8(void* bb, unsigned char v);
	XXNBSOCKETLIB_API void XxBBuffer_WriteUInt16(void* bb, unsigned short v);
	XXNBSOCKETLIB_API void XxBBuffer_WriteUInt32(void* bb, unsigned int v);
	XXNBSOCKETLIB_API void XxBBuffer_WriteUInt64(void* bb, unsigned long long v);
	XXNBSOCKETLIB_API void XxBBuffer_WriteFloat(void* bb, float v);
	XXNBSOCKETLIB_API void XxBBuffer_WriteDouble(void* bb, double v);

	// 各种定长写入指定数据类型, 成功返回 0. 失败返回错误码.
	XXNBSOCKETLIB_API void XxBBuffer_WritePodBoolean(void* bb, bool v);
	XXNBSOCKETLIB_API void XxBBuffer_WritePodInt8(void* bb, char v);
	XXNBSOCKETLIB_API void XxBBuffer_WritePodInt16(void* bb, short v);
	XXNBSOCKETLIB_API void XxBBuffer_WritePodInt32(void* bb, int v);
	XXNBSOCKETLIB_API void XxBBuffer_WritePodInt64(void* bb, long long v);
	XXNBSOCKETLIB_API void XxBBuffer_WritePodUInt8(void* bb, unsigned char v);
	XXNBSOCKETLIB_API void XxBBuffer_WritePodUInt16(void* bb, unsigned short v);
	XXNBSOCKETLIB_API void XxBBuffer_WritePodUInt32(void* bb, unsigned int v);
	XXNBSOCKETLIB_API void XxBBuffer_WritePodUInt64(void* bb, unsigned long long v);
	XXNBSOCKETLIB_API void XxBBuffer_WritePodFloat(void* bb, float v);
	XXNBSOCKETLIB_API void XxBBuffer_WritePodDouble(void* bb, double v);

	// 写入 变长长度 + 内容
	XXNBSOCKETLIB_API void XxBBuffer_WriteLenData(void* bb, char* s, int len);

	// 各种变长读指定数据类型, 成功返回 0. 失败返回错误码.
	XXNBSOCKETLIB_API int XxBBuffer_ReadBoolean(void* bb, bool* v);
	XXNBSOCKETLIB_API int XxBBuffer_ReadInt8(void* bb, char* v);
	XXNBSOCKETLIB_API int XxBBuffer_ReadInt16(void* bb, short* v);
	XXNBSOCKETLIB_API int XxBBuffer_ReadInt32(void* bb, int* v);
	XXNBSOCKETLIB_API int XxBBuffer_ReadInt64(void* bb, long long* v);
	XXNBSOCKETLIB_API int XxBBuffer_ReadUInt8(void* bb, unsigned char* v);
	XXNBSOCKETLIB_API int XxBBuffer_ReadUInt16(void* bb, unsigned short* v);
	XXNBSOCKETLIB_API int XxBBuffer_ReadUInt32(void* bb, unsigned int* v);
	XXNBSOCKETLIB_API int XxBBuffer_ReadUInt64(void* bb, unsigned long long* v);
	XXNBSOCKETLIB_API int XxBBuffer_ReadFloat(void* bb, float* v);
	XXNBSOCKETLIB_API int XxBBuffer_ReadDouble(void* bb, double* v);

	// 各种定长读指定数据类型, 成功返回 0. 失败返回错误码.
	XXNBSOCKETLIB_API int XxBBuffer_ReadPodBoolean(void* bb, bool* v);
	XXNBSOCKETLIB_API int XxBBuffer_ReadPodInt8(void* bb, char* v);
	XXNBSOCKETLIB_API int XxBBuffer_ReadPodInt16(void* bb, short* v);
	XXNBSOCKETLIB_API int XxBBuffer_ReadPodInt32(void* bb, int* v);
	XXNBSOCKETLIB_API int XxBBuffer_ReadPodInt64(void* bb, long long* v);
	XXNBSOCKETLIB_API int XxBBuffer_ReadPodUInt8(void* bb, unsigned char* v);
	XXNBSOCKETLIB_API int XxBBuffer_ReadPodUInt16(void* bb, unsigned short* v);
	XXNBSOCKETLIB_API int XxBBuffer_ReadPodUInt32(void* bb, unsigned int* v);
	XXNBSOCKETLIB_API int XxBBuffer_ReadPodUInt64(void* bb, unsigned long long* v);
	XXNBSOCKETLIB_API int XxBBuffer_ReadPodFloat(void* bb, float* v);
	XXNBSOCKETLIB_API int XxBBuffer_ReadPodDouble(void* bb, double* v);

	// 读出 变长长度 + 内容, 成功返回 0. 失败返回错误码.
	XXNBSOCKETLIB_API int XxBBuffer_ReadLenData(void* bb, char** s, int* len);

	// todo: 读出数据到另一个 XxBBuffer, 只读引用模式支持??

#ifdef __cplusplus
}
#endif
