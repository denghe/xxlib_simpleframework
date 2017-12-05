#include "xxloglib.h"
#include "xx_logger.h"

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


// 根据文件名 创建日志 db 文件. 成功返回 Logger 的指针 ctx. 失败返回 空
XXLOGLIB_API void* xxlogNew(char const* fn)
{
	try
	{
		return new xx::Logger(fn);
	}
	catch (...)
	{
		return nullptr;
	}
}

// todo: 尝试 char* 转为 uint16_t* 再转为 utf8, 不用再在 C# 那边做, C# 那边直接 string

// 直接传所有日志参数, 记录日志. 返回 nullptr 表示成功( 失败返回错误信息 )
XXLOGLIB_API void xxlogWriteAll(void* ctx, int level, long long time, char const* machine, char const* service, char const* instanceId, char const* title, long long opcode, char const* desc)
{
	((xx::Logger*)ctx)->WriteAll((xx::LogLevel)level, time, machine, service, instanceId, title, opcode, desc);
}

// 记录日志( 相比 WriteAll 缺失的参数 会去读默认值 ). 返回 nullptr 表示成功( 失败返回错误信息 )
XXLOGLIB_API void xxlogWrite(void* ctx, int level, char const* title, long long opcode, char const* desc)
{
	((xx::Logger*)ctx)->Write((xx::LogLevel)level, title, opcode, desc);
}

// 设置一些不容易变化的项目的默认值, 以便使用 Write 时能反复写入这些数据 而不需要再传
XXLOGLIB_API void xxlogSetDefaultValue(void* ctx, char const* machine, char const* service, char const* instanceId)
{
	((xx::Logger*)ctx)->SetDefaultValue(machine, service, instanceId);
}

// 析构 Logger
XXLOGLIB_API long long xxlogGetCounter(void* ctx)
{
	return ((xx::Logger*)ctx)->counter;
}


// 析构 Logger
XXLOGLIB_API void xxlogDelete(void* ctx)
{
	delete (xx::Logger*)ctx;
}

