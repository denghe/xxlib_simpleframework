#include "xx_sqlite.h"

struct Rand
{
	unsigned int z1 = 12345, z2 = 12345, z3 = 12345, z4 = 12345;
	inline unsigned int Next()
	{
		unsigned int b;
		b = ((z1 << 6) ^ z1) >> 13;
		z1 = ((z1 & 4294967294U) << 18) ^ b;
		b = ((z2 << 2) ^ z2) >> 27;
		z2 = ((z2 & 4294967288U) << 2) ^ b;
		b = ((z3 << 13) ^ z3) >> 21;
		z3 = ((z3 & 4294967280U) << 7) ^ b;
		b = ((z4 << 3) ^ z4) >> 12;
		z4 = ((z4 & 4294967168U) << 13) ^ b;
		return (z1 ^ z2 ^ z3 ^ z4);
	}
	const char const* visibleChars =
		"1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz?!"
		"1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz?!"
		"1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz?!"
		"1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz?!";
	inline void FillVisibleChars(char* buf, int len)
	{
		auto mod = len % 4;
		auto intCount = len / 4;
		auto intBuf = (int*)buf;
		unsigned int data;
		for (int i = 0; i < intCount; ++i)
		{
			data = Next();
			((char*)&intBuf[i])[0] = visibleChars[((unsigned char*)&data)[0]];
			((char*)&intBuf[i])[1] = visibleChars[((unsigned char*)&data)[1]];
			((char*)&intBuf[i])[2] = visibleChars[((unsigned char*)&data)[2]];
			((char*)&intBuf[i])[3] = visibleChars[((unsigned char*)&data)[3]];
		}
		data = Next();
		for (int i = 0; i < mod; ++i)
		{
			buf[intCount * 4 + i] = visibleChars[((unsigned char*)&data)[i]];
		}
	}
};

int main()
{
	xx::MemPool mp;

	Rand rnd;
	xx::String_v str(mp);
	str->Resize(8);
	str->C_str();

	xx::SQLite_v lite(mp, "C:/DB/test1.db");

	// 配合事务, insert 能达到 90万次一秒. 死系统大概率坏数据, 须配合备份方案.
	//lite->SetPragmas(
	//	xx::SQLiteSynchronousTypes::Off, xx::SQLiteJournalModes::Off,
	//	xx::SQLiteTempStoreTypes::Memory/*, xx::SQLiteLockingModes::Exclusive*/);

	// 折中的方案, 随机 insert 一万多次一秒, 死系统小概率坏数据
	lite->SetPragmas(xx::SQLiteSynchronousTypes::Normal, xx::SQLiteJournalModes::WAL);

	// 默认配置, insert 百把次一秒, 通常不会坏数据.
	//lite->SetPragmas(xx::SQLiteSynchronousTypes::Full, xx::SQLiteJournalModes::Delete);


	lite->Execute("delete from UserOnline");

	//lite->BeginTransaction();

	auto q = lite->CreateQuery("insert into UserOnline (UserID, UserName, SessionId, Action, CreateIP, CreateTime, UpdateTime) values (?, ?, ?, ?, ?, ?, ?)");

	xx::Stopwatch sw;
	int64_t count = 10000;
	for (int64_t i = 0; i < count; i++)
	{
		rnd.FillVisibleChars(str->buf, 8);
		q->SetParameters((int)rnd.Next(), str->buf, (int)rnd.Next(), str->buf, str->buf, str->buf, str->buf);
		q->Execute();
	}

	//lite->Commit();

	auto elapsedMS = sw() + 1;
	lite->Cout("insert ", count, " rows success! elapsed MS = ", elapsedMS, ", QPS = ", (count * 1000 / elapsedMS), "\n");

	q->Release();
	q = lite->CreateQuery("select ID, UserID, UserName, SessionId, Action, CreateIP, CreateTime, UpdateTime from UserOnline limit 0, 10");

	q->Execute([&](xx::SQLiteReader& reader)
	{
		lite->Cout("ID = ", reader.ReadInt64(0), ", UserID = ", reader.ReadInt32(1), ", UserName = ", reader.ReadString(2)
			, ", SessionId = ", reader.ReadInt32(3), ", Action = ", reader.ReadString(4)
			, ", CreateIP = ", reader.ReadString(5), ", CreateTime = ", reader.ReadString(6)
			, ", UpdateTime = ", reader.ReadString(7), "\n");
	});

	std::cin.get();
	return 0;
}








/*

下面是完整安全的写法

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



*/