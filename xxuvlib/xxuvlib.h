#pragma once
#ifdef _WIN32
	#include <SDKDDKVer.h>
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
	#ifdef xxloglib_EXPORTS
	#define XXLOGLIB_API __declspec(dllexport)
	#else
	#define XXLOGLIB_API __declspec(dllimport)
	#endif
#else
	#define XXLOGLIB_API
#endif

#ifdef __cplusplus
extern "C" {
#endif

	// 根据文件名 创建日志 db 文件. 成功返回 Logger 的指针 ctx. 失败返回 空
	XXLOGLIB_API void* xxlogNew(char const* fn);

	// 直接传所有日志参数, 记录日志. 返回 nullptr 表示成功( 失败返回错误信息 )
	XXLOGLIB_API void xxlogWriteAll(void* ctx, int level, long long time, char const* machine, char const* service, char const* instanceId, char const* title, long long opcode, char const* desc);

	// 记录日志( 相比 WriteAll 缺失的参数 会去读默认值 ). 返回 nullptr 表示成功( 失败返回错误信息 )
	XXLOGLIB_API void xxlogWrite(void* ctx, int level, char const* title, long long opcode, char const* desc);

	// 设置一些不容易变化的项目的默认值, 以便使用 Write 时能反复写入这些数据 而不需要再传
	XXLOGLIB_API void xxlogSetDefaultValue(void* ctx, char const* machine, char const* service, char const* instanceId);

	// 获取已写入记录数( 已进入事务 但也有可能因为最后的事务未成功提交或写盘而出现不一致 )
	XXLOGLIB_API long long xxlogGetCounter(void*);

	// 析构 Logger
	XXLOGLIB_API void xxlogDelete(void*);

#ifdef __cplusplus
}
#endif
