#include "db/LOGDB_sqlite.h"

namespace xx
{
	struct Logger
	{
		MemPool mp;
		SQLite_v db;
		LOGDB::SQLiteFuncs funcs;
		Logger(char const* const& fn) : db(mp, fn), funcs(*db)
		{
			db->SetPragmaSynchronousType(xx::SQLiteSynchronousTypes::Off);
			db->SetPragmaJournalMode(xx::SQLiteJournalModes::Off);
			db->SetPragmaLockingMode(xx::SQLiteLockingModes::Exclusive);
			if (!db->TableExists("log")) funcs.CreateTable_log();
		}
	};
}

xx::Logger logger("test_cpp4_log.db");
const char const* machineName = "pc1";
void Log(LOGDB::Level const& level, char const* const& service,
	char const* const& instanceId, char const* const& title, int64_t const& opcode, char const* const& desc)
{
	try
	{
		logger.funcs.InsertLog(level, xx::GetNowDateTimeTicks(), machineName, service, instanceId, title, opcode, desc);
	}
	catch (...)
	{
		std::cout << "exec wrong !!!" << std::endl;
	}
}

int main()
{
	xx::Stopwatch sw;
	size_t i = 0;
	for (; i < 1000000; i++)
	{
		Log(LOGDB::Level::Error, "test_cpp4", "0", "testlog", 0, "this is a test log");
	}
	std::cout << "inserted " << i << " logs. elapsed ms = " << sw() << std::endl;
}
