#include "xx_sqlite.h"

int main()
{
	xx::MemPool mp;
	xx::SQLite_v lite(mp, "C:/DB/test1.db");

	if (auto r = lite->Execute("delete from inserttest"))
	{
		lite->Cout("set parms failed. errCode = ", lite->lastErrorCode, ", errMsg = ", lite->lastErrorMessage);
		return 0;
	}

	auto q = lite->CreateQuery("insert into inserttest (n) values (?)");
	if (!q)
	{
		lite->Cout("CreateQuery failed. errCode = ", lite->lastErrorCode, ", errMsg = ", lite->lastErrorMessage);
		return 0;
	}

	xx::Stopwatch sw;
	int64_t count = 10000;
	for (int64_t i = 0; i < count; i++)
	{
		if (auto r = q->SetParameters(i))
		{
			lite->Cout("set parms failed. errCode = ", lite->lastErrorCode, ", errMsg = ", lite->lastErrorMessage);
			return 0;
		}

		if (!q->Execute())
		{
			lite->Cout("exec failed. errCode = ", lite->lastErrorCode, ", errMsg = ", lite->lastErrorMessage);
			return 0;
		}
	}
	auto elapsedMS = sw();
	lite->Cout("insert ", count, " rows success! elapsed MS = ", elapsedMS, ", QPS = ", (count * 1000 / elapsedMS), "\n");

	q->Release();
	q = lite->CreateQuery("select n from inserttest limit 0, 10");
	if (!q)
	{
		lite->Cout("CreateQuery failed. errCode = ", lite->lastErrorCode, ", errMsg = ", lite->lastErrorMessage);
		return 0;
	}

	int rowNum = 0;
	if (!q->Execute([&](xx::SQLiteReader& reader)
	{
		lite->Cout("rowNum = ", rowNum++, ", n = ", reader.ReadInt64(0), "\n");
	}))
	{
		lite->Cout("CreateQuery failed. errCode = ", lite->lastErrorCode, ", errMsg = ", lite->lastErrorMessage);
		return 0;
	}

	return 0;
}
