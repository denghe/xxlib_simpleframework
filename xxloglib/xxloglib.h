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
	XXLOGLIB_API void* xxlogNew(char const* fn);

	// 直接传所有日志参数, 记录日志. 返回 nullptr 表示成功( 失败返回错误信息 )
	XXLOGLIB_API char const* xxlogWriteAll(void* ctx, int level, long long time
		, char const* machine, char const* service, char const* instanceId
		, char const* title, long long opcode, char const* desc);

	// 记录日志( 相比 WriteAll 缺失的参数 会去读默认值 ). 返回 nullptr 表示成功( 失败返回错误信息 )
	XXLOGLIB_API char const* xxlogWrite(void* ctx, int level, char const* title, long long opcode, char const* desc);

	// 设置一些不容易变化的项目的默认值, 以便使用 Write 时能反复写入这些数据 而不需要再传
	XXLOGLIB_API void xxlogSetDefaultValue(void* ctx, char const* machine, char const* service, char const* instanceId);

	// 开始批写( 启动事务, 合并写入批次, 能提升性能, 但增加了数据损坏的可能性 ). 返回 nullptr 表示成功( 失败返回错误信息 )
	XXLOGLIB_API char const* xxlogBeginTrans(void* ctx);

	// 结束批写( 提交事务 ). 返回 nullptr 表示成功( 失败返回错误信息 )
	XXLOGLIB_API char const* xxlogEndTrans(void* ctx);

	// todo: 设置记录条数或超时自动删

	// 析构 Logger
	XXLOGLIB_API void xxlogDelete(void*);

#ifdef __cplusplus
}
#endif
