#pragma once
#include <SDKDDKVer.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#ifdef xxloglib_EXPORTS
#define XXLOGLIB_API __declspec(dllexport)
#else
#define XXLOGLIB_API __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C" {
#endif

	// 根据文件名 创建日志 db 文件. 成功返回 Logger 的指针 ctx. 失败返回 空
	XXLOGLIB_API void* XxLogNew(char const* fn);

	// 直接传所有日志参数, 记录日志. 返回 nullptr 表示成功( 失败返回错误信息 )
	XXLOGLIB_API char const* XxLogWriteAll(void* ctx, int level, long long time, char const* machine, char const* service, char const* instanceId, char const* title, long long opcode, char const* desc);

	// 设置一些不容易变化的项目的默认值, 以便使用 Write 时能反复写入这些数据 而不需要再传
	XXLOGLIB_API void XxLogSetDefaultValue(void* ctx, char const* machine, char const* service, char const* instanceId);

	// 记录日志( 相比 WriteAll 缺失的参数 会去读默认值 ). 返回 nullptr 表示成功( 失败返回错误信息 )
	XXLOGLIB_API char const* XxLogWrite(void* ctx, int level, char const* title, long long opcode, char const* desc);

	// 析构 Logger
	XXLOGLIB_API void XxLogDelete(void*);

#ifdef __cplusplus
}
#endif
