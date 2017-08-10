#include "db\DB_sqlite.h"

int main()
{
	xx::MemPool mp;
	xx::SQLite_v sql(mp, "data.db");
	sql->SetPragmaForeignKeys(true);

	DB::SQLiteInitFuncs ifs(*sql);
	//DB::SQLiteLoginFuncs lfs(*sql);
	DB::SQLiteManageFuncs mfs(*sql);

	try
	{
		// 建表
		if (!sql->TableExists("game_account")) ifs.CreateTable_game_account();
		if (!sql->TableExists("manage_account")) ifs.CreateTable_manage_account();
		if (!sql->TableExists("manage_permission")) ifs.CreateTable_manage_permission();
		if (!sql->TableExists("manage_role")) ifs.CreateTable_manage_role();
		if (!sql->TableExists("manage_bind_role_permission")) ifs.CreateTable_manage_bind_role_permission();
		if (!sql->TableExists("manage_bind_account_role")) ifs.CreateTable_manage_bind_account_role();

		// 清表数据( 关和开启外键约束这样就能更随意高速的批量删数据 )
		sql->SetPragmaForeignKeys(false);
		sql->TruncateTable("manage_bind_account_role");
		sql->TruncateTable("manage_bind_role_permission");
		sql->TruncateTable("manage_role");
		sql->TruncateTable("manage_permission");
		sql->TruncateTable("manage_account");
		sql->TruncateTable("game_account");
		sql->SetPragmaForeignKeys(true);

		// 准备点原始素材
		auto u1 = mp.Str("a");
		auto u2 = mp.Str("bb");
		auto u3 = mp.Str("ccc");
		auto p1 = mp.Str("1");
		auto p2 = mp.Str("22");
		auto p3 = mp.Str("333");

		// 插入测试数据
		// acc
		mfs.InsertAccount(u1, p1);
		mfs.InsertAccount(u2, p2);
		mfs.InsertAccount(u3, p3);

		// role
		mfs.InsertRole(1, mp.Str("admin"), mp.Str("admin's desc"));
		mfs.InsertRole(2, mp.Str("user"), mp.Str("user's desc"));
		mfs.InsertRole(3, mp.Str("guest"), mp.Str("guest's desc"));

		// perm
		mfs.InsertPermission(1, mp.Str("xxx1"), mp.Str("g1"), mp.Str("xxx1's desc"));
		mfs.InsertPermission(2, mp.Str("xxx2"), mp.Str("g1"), mp.Str("xxx2's desc"));
		mfs.InsertPermission(3, mp.Str("xxx3"), mp.Str("g1"), mp.Str("xxx3's desc"));
		mfs.InsertPermission(4, mp.Str("xxx1"), mp.Str("g2"), mp.Str("xxx1's desc"));
		mfs.InsertPermission(5, mp.Str("xxx2"), mp.Str("g2"), mp.Str("xxx2's desc"));
		mfs.InsertPermission(6, mp.Str("xxx3"), mp.Str("g2"), mp.Str("xxx3's desc"));

		// 测试绑身份
		mfs.InsertBindAccountRole(1, 1);
		mfs.InsertBindAccountRole(1, 2);
		mfs.InsertBindAccountRole(1, 3);

		mfs.InsertBindAccountRole(2, 2);
		mfs.InsertBindAccountRole(2, 3);

		mfs.InsertBindAccountRole(3, 3);

		// 测试绑身份的权限
		mfs.InsertBindRolePermission(1, 1);
		mfs.InsertBindRolePermission(1, 2);
		mfs.InsertBindRolePermission(1, 3);
		mfs.InsertBindRolePermission(1, 4);
		mfs.InsertBindRolePermission(1, 5);
		mfs.InsertBindRolePermission(1, 6);

		mfs.InsertBindRolePermission(2, 1);
		mfs.InsertBindRolePermission(2, 2);
		mfs.InsertBindRolePermission(2, 3);

		mfs.InsertBindRolePermission(3, 4);
		mfs.InsertBindRolePermission(3, 5);
		mfs.InsertBindRolePermission(3, 6);


		// 测试读取用户权限列表
		{
			mp.Cout("SelectPermissionIdsByAccountId(1)'s rtv = ", mfs.SelectPermissionIdsByAccountId(1), '\n');
			mp.Cout("SelectPermissionIdsByAccountId(2)'s rtv = ", mfs.SelectPermissionIdsByAccountId(2), '\n');
			mp.Cout("SelectPermissionIdsByAccountId(3)'s rtv = ", mfs.SelectPermissionIdsByAccountId(3), '\n');
		}

		// 测试删角色
		{
			// 先删依赖和引用
			mfs.DeleteBindRolePermissionByRoleId(1);
			mp.Cout("DeleteBindRolePermissionByRoleId affected rows = ", sql->GetAffectedRows(), '\n');
			mfs.DeleteBindAccountRoleByRoleId(1);
			mp.Cout("DeleteBindAccountRoleByRoleId affected rows = ", sql->GetAffectedRows(), '\n');
			mfs.DeleteRole(1);
			mp.Cout("DeleteRole affected rows = ", sql->GetAffectedRows(), '\n');
		}

		// 测试按用户名查询账号
		{
			auto rtv = mfs.SelectAccountByUsername(u2);
			mp.Cout("SelectAccountByUsername's rtv = ", rtv, '\n');
		}

		// 测试改密码
		{
			mfs.UpdateAccount_ChangePassword(1, p2);
			mp.Cout("UpdateAccount_ChangePassword affected rows = ", sql->GetAffectedRows(), '\n');
			mfs.UpdateAccount_ChangePassword(2, p3);
			mp.Cout("UpdateAccount_ChangePassword affected rows = ", sql->GetAffectedRows(), '\n');
			mfs.UpdateAccount_ChangePassword(3, p1);
			mp.Cout("UpdateAccount_ChangePassword affected rows = ", sql->GetAffectedRows(), '\n');
		}

		// 测试改用户名
		{
			try
			{
				// 这句应该会提示用户名重复
				mfs.UpdateAccount_ChangeUsername(1, u2);
			}
			catch (int e)
			{
				mp.Cout("e = ", e, ", msg = ", sql->lastErrorMessage, "\n");
			}
			mfs.UpdateAccount_ChangeUsername(1, mp.Str("ererere"));
			mp.Cout("UpdateAccount_ChangeUsername affected rows = ", sql->GetAffectedRows(), '\n');
		}

		// 测试删账号
		{
			// 先删依赖数据

			mfs.DeleteBindAccountRoleByAccountId(2);
			mp.Cout("DeleteBindAccountRoleByAccountId affected rows = ", sql->GetAffectedRows(), '\n');

			mfs.DeleteAccount(2);
			mp.Cout("DeleteAccount affected rows = ", sql->GetAffectedRows(), '\n');
		}


		// 显示各表的数据
		{
			mp.Cout("SelectAccounts's rtv = ", mfs.SelectAccounts(), '\n');
			mp.Cout("SelectRoles's rtv = ", mfs.SelectRoles(), '\n');
			mp.Cout("SelectPermissions's rtv = ", mfs.SelectPermissions(), '\n');
			mp.Cout("SelectBindRolePermissions's rtv = ", mfs.SelectBindRolePermissions(), '\n');
			mp.Cout("SelectBindAccountRoles's rtv = ", mfs.SelectBindAccountRoles(), '\n');
		}
	}
	catch (int errCode)
	{
		mp.Cout("errCode = ", errCode, ", lastErrorMessage = ", sql->lastErrorMessage, "\n");
	}

LabEnd:
	std::cin.get();
	return 0;
}








// 下面是一小段插入性能测试


//#include "xx_sqlite.h"
//
//struct Rand
//{
//	unsigned int z1 = 12345, z2 = 12345, z3 = 12345, z4 = 12345;
//	inline unsigned int Next()
//	{
//		unsigned int b;
//		b = ((z1 << 6) ^ z1) >> 13;
//		z1 = ((z1 & 4294967294U) << 18) ^ b;
//		b = ((z2 << 2) ^ z2) >> 27;
//		z2 = ((z2 & 4294967288U) << 2) ^ b;
//		b = ((z3 << 13) ^ z3) >> 21;
//		z3 = ((z3 & 4294967280U) << 7) ^ b;
//		b = ((z4 << 3) ^ z4) >> 12;
//		z4 = ((z4 & 4294967168U) << 13) ^ b;
//		return (z1 ^ z2 ^ z3 ^ z4);
//	}
//	const char const* visibleChars =
//		"1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz?!"
//		"1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz?!"
//		"1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz?!"
//		"1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz?!";
//	inline void FillVisibleChars(char* buf, int len)
//	{
//		auto mod = len % 4;
//		auto intCount = len / 4;
//		auto intBuf = (int*)buf;
//		unsigned int data;
//		for (int i = 0; i < intCount; ++i)
//		{
//			data = Next();
//			((char*)&intBuf[i])[0] = visibleChars[((unsigned char*)&data)[0]];
//			((char*)&intBuf[i])[1] = visibleChars[((unsigned char*)&data)[1]];
//			((char*)&intBuf[i])[2] = visibleChars[((unsigned char*)&data)[2]];
//			((char*)&intBuf[i])[3] = visibleChars[((unsigned char*)&data)[3]];
//		}
//		data = Next();
//		for (int i = 0; i < mod; ++i)
//		{
//			buf[intCount * 4 + i] = visibleChars[((unsigned char*)&data)[i]];
//		}
//	}
//};
//
//int main()
//{
//	xx::MemPool mp;
//
//	Rand rnd;
//	xx::String_v str(mp);
//	str->Resize(8);
//	str->C_str();
//
//	xx::SQLite_v lite(mp, "C:/DB/test1.db");
//
//	// 配合事务, insert 能达到 90万次一秒. 死系统大概率坏数据, 须配合备份方案.
//	//lite->SetPragmas(
//	//	xx::SQLiteSynchronousTypes::Off, xx::SQLiteJournalModes::Off,
//	//	xx::SQLiteTempStoreTypes::Memory/*, xx::SQLiteLockingModes::Exclusive*/);
//
//	// 折中的方案, 随机 insert 一万多次一秒, 死系统小概率坏数据
//	lite->SetPragmaSynchronousType(xx::SQLiteSynchronousTypes::Normal);
//	lite->SetPragmaJournalMode(xx::SQLiteJournalModes::WAL);
//
//	// 默认配置, insert 百把次一秒, 通常不会坏数据.
//	//lite->SetPragmas(xx::SQLiteSynchronousTypes::Full, xx::SQLiteJournalModes::Delete);
//
//
//	lite->Execute("delete from UserOnline");
//
//	//lite->BeginTransaction();
//
//	auto q = lite->CreateQuery("insert into UserOnline (UserID, UserName, SessionId, Action, CreateIP, CreateTime, UpdateTime) values (?, ?, ?, ?, ?, ?, ?)");
//
//	xx::Stopwatch sw;
//	int64_t count = 10000;
//	for (int64_t i = 0; i < count; i++)
//	{
//		rnd.FillVisibleChars(str->buf, 8);
//		q->SetParameters((int)rnd.Next(), str->buf, (int)rnd.Next(), str->buf, str->buf, str->buf, str->buf);
//		q->Execute();
//	}
//
//	//lite->Commit();
//
//	auto elapsedMS = sw() + 1;
//	lite->Cout("insert ", count, " rows success! elapsed MS = ", elapsedMS, ", QPS = ", (count * 1000 / elapsedMS), "\n");
//
//	q->Release();
//	q = lite->CreateQuery("select ID, UserID, UserName, SessionId, Action, CreateIP, CreateTime, UpdateTime from UserOnline limit 0, 10");
//
//	q->Execute([&](xx::SQLiteReader& reader)
//	{
//		lite->Cout("ID = ", reader.ReadInt64(0), ", UserID = ", reader.ReadInt32(1), ", UserName = ", reader.ReadString(2)
//			, ", SessionId = ", reader.ReadInt32(3), ", Action = ", reader.ReadString(4)
//			, ", CreateIP = ", reader.ReadString(5), ", CreateTime = ", reader.ReadString(6)
//			, ", UpdateTime = ", reader.ReadString(7), "\n");
//	});
//
//	std::cin.get();
//	return 0;
//}
//







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