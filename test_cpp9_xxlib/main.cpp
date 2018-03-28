#include "LOGDB_sqlite.h"

int main()
{
	xx::MemPool mp;
	xx::SQLite sqlite(&mp, "C:/Logs/xx.log.db");
	LOGDB::SQLiteFuncs sf(sqlite);
	if (!sqlite.TableExists("log"))
	{
		sf.CreateTable_log();

		sf.InsertLog(LOGDB::Level::Info, xx::GetDateTimeNowTicks()
			, "m1", "s1", "i1", "t1", 1, "d1");

		sf.InsertLog(LOGDB::Level::Info, xx::GetDateTimeNowTicks()
			, "m1", "s1", "i1", "t2", 2, xx::String(&mp, L"中文内容"));
	}
	std::cout << sf.SelectLogs() << std::endl;
	return 0;
}
