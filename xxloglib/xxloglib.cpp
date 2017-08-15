#include "xxloglib.h"
#include "db/LOGDB_sqlite.h"

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

struct Logger
{
	xx::MemPool mp;
	xx::SQLite_v db;
	LOGDB::SQLiteFuncs funcs;
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
		db->SetPragmaSynchronousType(xx::SQLiteSynchronousTypes::Off);
		db->SetPragmaJournalMode(xx::SQLiteJournalModes::Off);
		db->SetPragmaLockingMode(xx::SQLiteLockingModes::Exclusive);
		if (!db->TableExists("log")) funcs.CreateTable_log();
	}
};

// 根据文件名 创建日志 db 文件. 成功返回 Logger 的指针 ctx. 失败返回 空
XXLOGLIB_API void* XxLogNew(char const* fn)
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
XXLOGLIB_API char const* XxLogWriteAll(void* ctx, int level, long long time, char const* machine, char const* service, char const* instanceId, char const* title, long long opcode, char const* desc)
{
	auto& self = *(Logger*)ctx;
	auto str = "";
	try
	{
		self.funcs.InsertLog((LOGDB::Level const&)level, time, (machine ? machine : str), (service ? service : str), (instanceId ? instanceId : str), (title ? title : str), opcode, (desc ? desc : str));
	}
	catch (...)
	{
		return self.db->lastErrorMessage;
	}
	return nullptr;
}

// 设置一些不容易变化的项目的默认值, 以便使用 Write 时能反复写入这些数据 而不需要再传
XXLOGLIB_API void XxLogSetDefaultValue(void* ctx, char const* machine, char const* service, char const* instanceId)
{
	auto& self = *(Logger*)ctx;
	self.machine->Assign(machine); self.machine->C_str();
	self.service->Assign(service); self.service->C_str();
	self.instanceId->Assign(instanceId); self.instanceId->C_str();

}

// 记录日志( 相比 WriteAll 缺失的参数 会去读默认值 ). 返回 nullptr 表示成功( 失败返回错误信息 )
XXLOGLIB_API char const* XxLogWrite(void* ctx, int level, char const* title, long long opcode, char const* desc)
{
	auto& self = *(Logger*)ctx;
	return XxLogWriteAll(ctx, level, xx::GetNowDateTimeTicks(), self.machine->buf, self.service->buf, self.instanceId->buf, title, opcode, desc);
}

// 析构 Logger
XXLOGLIB_API void XxLogDelete(void* ctx)
{
	delete (Logger*)ctx;
}
