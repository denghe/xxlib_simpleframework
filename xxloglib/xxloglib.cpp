#include "xxloglib.h"
#include "xx_logger.h"

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




// 根据文件名 创建日志 db 文件. 成功返回 Logger 的指针 ctx. 失败返回 空
XXLOGLIB_API void* xxlog2New(char const* fn)
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
XXLOGLIB_API void xxlog2WriteAll(void* ctx, int level, long long time, char const* machine, char const* service, char const* instanceId, char const* title, long long opcode, char const* desc)
{
	((xx::Logger*)ctx)->WriteAll((xx::LogLevel)level, time, machine, service, instanceId, title, opcode, desc);
}

// 记录日志( 相比 WriteAll 缺失的参数 会去读默认值 ). 返回 nullptr 表示成功( 失败返回错误信息 )
XXLOGLIB_API void xxlog2Write(void* ctx, int level, char const* title, long long opcode, char const* desc)
{
	((xx::Logger*)ctx)->Write((xx::LogLevel)level, title, opcode, desc);
}

// 设置一些不容易变化的项目的默认值, 以便使用 Write 时能反复写入这些数据 而不需要再传
XXLOGLIB_API void xxlog2SetDefaultValue(void* ctx, char const* machine, char const* service, char const* instanceId)
{
	((xx::Logger*)ctx)->SetDefaultValue(machine, service, instanceId);
}

// 析构 Logger
XXLOGLIB_API long long xxlog2GetCounter(void* ctx)
{
	return ((xx::Logger*)ctx)->counter;
}


// 析构 Logger
XXLOGLIB_API void xxlog2Delete(void* ctx)
{
	delete (xx::Logger*)ctx;
}









// 专门写一份供 C# 用, C# 自己管理线程和内存队列
struct Logger
{
	xx::MemPool mp;
	xx::SQLite_v db;
	xx::LogFuncs funcs;
	xx::String_v machine;
	xx::String_v service;
	xx::String_v instanceId;

	Logger(char const* const& fn)
		: db(mp, fn)
		, funcs(*db)
		, machine(mp)
		, service(mp)
		, instanceId(mp)
	{
		if (!db->TableExists("log")) funcs.CreateTable_log();
	}
};

// 根据文件名 创建日志 db 文件. 成功返回 Logger 的指针 ctx. 失败返回 空
XXLOGLIB_API void* xxlogNew(char const* fn)
{
	try
	{
		return new Logger(fn);
	}
	catch (...)
	{
		return nullptr;
	}
}

// 直接传所有日志参数, 记录日志. 返回 nullptr 表示成功( 失败返回错误信息 )
XXLOGLIB_API char const* xxlogWriteAll(void* ctx, int level, long long time, char const* machine, char const* service, char const* instanceId, char const* title, long long opcode, char const* desc)
{
	auto& self = *(Logger*)ctx;
	auto str = "";
	try
	{
		self.funcs.InsertLog((xx::LogLevel)level, time, (machine ? machine : str), (service ? service : str), (instanceId ? instanceId : str), (title ? title : str), opcode, (desc ? desc : str));
	}
	catch (...)
	{
		return self.db->lastErrorMessage;
	}
	return nullptr;
}

// 记录日志( 相比 WriteAll 缺失的参数 会去读默认值 ). 返回 nullptr 表示成功( 失败返回错误信息 )
XXLOGLIB_API char const* xxlogWrite(void* ctx, int level, char const* title, long long opcode, char const* desc)
{
	auto& self = *(Logger*)ctx;
	return xxlogWriteAll(ctx, level, xx::GetNowDateTimeTicks(), self.machine->buf, self.service->buf, self.instanceId->buf, title, opcode, desc);
}

// 设置一些不容易变化的项目的默认值, 以便使用 Write 时能反复写入这些数据 而不需要再传
XXLOGLIB_API void xxlogSetDefaultValue(void* ctx, char const* machine, char const* service, char const* instanceId)
{
	auto& self = *(Logger*)ctx;
	self.machine->Assign(machine); self.machine->C_str();
	self.service->Assign(service); self.service->C_str();
	self.instanceId->Assign(instanceId); self.instanceId->C_str();
}

// 开始批写( 启动事务, 合并写入批次, 能提升性能, 但增加了数据损坏的可能性 ). 返回 nullptr 表示成功( 失败返回错误信息 )
XXLOGLIB_API char const* xxlogBeginTrans(void* ctx)
{
	auto& self = *(Logger*)ctx;
	try
	{
		self.db->BeginTransaction();
	}
	catch (...)
	{
		return self.db->lastErrorMessage;
	}
	return nullptr;
}

// 结束批写( 提交事务 ). 返回 nullptr 表示成功( 失败返回错误信息 )
XXLOGLIB_API char const* xxlogEndTrans(void* ctx)
{
	auto& self = *(Logger*)ctx;
	try
	{
		self.db->EndTransaction();
	}
	catch (...)
	{
		return self.db->lastErrorMessage;
	}
	return nullptr;
}

// 析构 Logger
XXLOGLIB_API void xxlogDelete(void* ctx)
{
	delete (Logger*)ctx;
}
