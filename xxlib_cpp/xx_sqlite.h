#pragma once
#include <xx_mempool.h>
#include <sqlite3.h>

// todo: _v _p support

namespace xx
{
	struct SQLite;
	struct SQLiteQuery;
	struct SQLiteReader;

	// 保持与 SQLite 的宏一致
	enum class SQLiteDataTypes : uint8_t
	{
		Integer = 1,
		Float = 2,
		Text = 3,
		Blob = 4,
		Null = 5
	};

	// 由慢及快
	// 数据写盘模式
	enum class SQLiteSynchronousTypes : uint8_t
	{
		// 等完全写入磁盘
		Full,
		// 阶段性等写磁盘
		Normal,
		// 完全不等
		Off,
		MAX
	};

	static const char* SQLiteSynchronousTypes_ss[] =
	{
		"FULL", "NORMAL", "OFF"
	};

	// 由慢及快
	// 事务数据记录模式
	enum class SQLiteJournalModes : uint8_t
	{
		// 终止事务时删掉 journal 文件
		Delete,
		// 不删, 字节清 0 ( 可能比删快 )
		Truncate,
		// 在文件头打标记( 可能比字节清 0 快 )
		Persist,
		// 内存模式( 可能丢数据 )
		Memory,
		// write-ahead 模式( 似乎比上面都快, 不会丢数据 )
		WAL,
		// 无事务支持( 最快 )
		Off,
		MAX
	};

	static const char* SQLiteJournalModes_ss[] =
	{
		"DELETE", "TRUNCATE", "PERSIST", "MEMORY", "WAL", "OFF"
	};

	// 由慢及快
	// 临时表处理模式
	enum class SQLiteTempStoreTypes : uint8_t
	{
		// 默认( 视 C TEMP_STORE 宏而定 )
		Default,
		// 在文件中建临时表
		File,
		// 在内存中建临时表
		Memory,
		MAX
	};

	static const char* SQLiteTempStoreTypes_ss[] =
	{
		"DEFAULT", "FILE", "MEMORY"
	};

	// 由慢及快
	// 释放排它锁，仅当关闭数据库连接，或者将锁模式改回为NORMAL时，再次访问数据库文件（读或写）才会放掉
	enum class SQLiteLockingModes : uint8_t
	{
		// 数据库连接在每一个读或写事务终点的时候放掉文件锁
		Normal,
		// 连接永远不会释放文件锁. 第一次执行读操作时，会获取并持有共享锁，第一次写，会获取并持有排它锁
		Exclusive,
		MAX
	};

	static const char* SQLiteLockingModes_ss[] =
	{
		"NORMAL", "EXCLUSIVE"
	};

	struct SQLite : MPObject
	{
		xx::List_v<SQLiteQuery*> queries;
		xx::String_v sqlBuilder;
		sqlite3* dbctx = nullptr;
		int lastErrorCode = 0;
		const char* lastErrorMessage = nullptr;
		SQLiteQuery* query_Exists = nullptr;

		SQLite(char const* const& fn, bool readOnly = false);
		~SQLite();

		int SetPragma(SQLiteSynchronousTypes st);
		int SetPragma(SQLiteJournalModes jm);
		int SetPragma(SQLiteTempStoreTypes tst);
		int SetPragma(SQLiteLockingModes lm);
		int SetPragma(int cacheSize);

		template<typename ... Parameters>
		int SetPragmas(Parameters const& ... ps);
		template<typename Parameter, typename ... Parameters>
		int SetPragmasCore(Parameter const& p, Parameters const& ... ps);
		int SetPragmasCore();


		int BeginTransaction();
		int Commit();
		int Rollback();
		int EndTransaction();

		int TableExists(char const* const& tn);

		SQLiteQuery* CreateQuery(char const* const& sql, int const& sqlLen = 0);
		int Execute(char const* const& sql);
	};

	struct SQLiteQuery : MPObject
	{
		SQLite* owner;
		uint32_t owner_queries_index = -1;
		sqlite3_stmt* stmt = nullptr;
		int numParams = 0;
		typedef std::function<void(SQLiteReader& sr)> ReadFunc;

		SQLiteQuery(SQLite* owner, char const* const& sql, int const& sqlLen);
		~SQLiteQuery();

		int SetParameter(int parmIdx, int const& v);
		int SetParameter(int parmIdx, sqlite_int64 const& v);
		int SetParameter(int parmIdx, double const& v);
		int SetParameter(int parmIdx, char const* const& str, int strLen = 0, bool makeCopy = false);
		int SetParameter(int parmIdx, char const* const& buf, size_t const& len, bool makeCopy = false);
		int SetParameter(int parmIdx, String* const& str, bool makeCopy = false);
		int SetParameter(int parmIdx, BBuffer* const& buf, bool makeCopy = false);
		int SetParameter(int parmIdx, String_v const& str, bool makeCopy = false);
		int SetParameter(int parmIdx, BBuffer_v const& buf, bool makeCopy = false);
		int SetParameter(int parmIdx, String_p const& str, bool makeCopy = false);
		int SetParameter(int parmIdx, BBuffer_p const& buf, bool makeCopy = false);

		template<typename ... Parameters>
		int SetParameters(Parameters const& ... ps);
		template<typename Parameter, typename ... Parameters>
		int SetParametersCore(int& parmIdx, Parameter const& p, Parameters const& ... ps);
		int SetParametersCore(int& parmIdx);

		bool Execute(ReadFunc && rf = nullptr);
	};

	struct SQLiteReader
	{
		sqlite3_stmt* stmt;
		SQLiteReader(sqlite3_stmt* stmt);
		int GetColumnCount();
		SQLiteDataTypes GetColumnDataType(int colIdx);
		char const* GetColumnName(int colIdx);
		bool IsNull(int colIdx);
		int ReadInt32(int colIdx);
		sqlite_int64 ReadInt64(int colIdx);
		double ReadDouble(int colIdx);
		char const* ReadString(int colIdx);
		std::pair<char const*, int> ReadText(int colIdx);
		std::pair<char const*, int> ReadBlob(int colIdx);
	};


	using SQLite_p = Ptr<SQLite>;
	using SQLite_v = MemHeaderBox<SQLite>;
	template<>
	struct MemmoveSupport<SQLite_v>
	{
		static const bool value = true;
	};


	using SQLiteQuery_p = Ptr<SQLiteQuery>;
	using SQLiteQuery_v = MemHeaderBox<SQLiteQuery>;
	template<>
	struct MemmoveSupport<SQLiteQuery_v>
	{
		static const bool value = true;
	};


	/******************************************************************************************************/
	// impls
	/******************************************************************************************************/

	/***************************************************************/
	// SQLite

	inline SQLite::SQLite(char const* const& fn, bool readOnly)
		: queries(mempool())
		, sqlBuilder(mempool())
	{
		int r = 0;
		if (readOnly)
		{
			r = sqlite3_open_v2(fn, &dbctx, SQLITE_OPEN_READONLY, nullptr);
		}
		else
		{
			r = sqlite3_open_v2(fn, &dbctx, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);
		}
		if (r) throw r;
	}

	int SQLite::SetPragma(SQLiteSynchronousTypes st)
	{
		if ((int)st < 0 || (int)st >(int)SQLiteSynchronousTypes::MAX)
		{
			lastErrorCode = -1;
			lastErrorMessage = "bad SQLiteSynchronousTypes";
			return lastErrorCode;
		}
		sqlBuilder->Clear();
		sqlBuilder->Append("PRAGMA synchronous = ", SQLiteSynchronousTypes_ss[(int)st]);
		return Execute(sqlBuilder->C_str());
	}
	int SQLite::SetPragma(SQLiteJournalModes jm)
	{
		if ((int)jm < 0 || (int)jm >(int)SQLiteJournalModes::MAX)
		{
			lastErrorCode = -1;
			lastErrorMessage = "bad SQLiteJournalModes";
			return lastErrorCode;
		}
		sqlBuilder->Clear();
		sqlBuilder->Append("PRAGMA journal_mode = ", SQLiteJournalModes_ss[(int)jm]);
		return Execute(sqlBuilder->C_str());
	}
	int SQLite::SetPragma(SQLiteTempStoreTypes tst)
	{
		if ((int)tst < 0 || (int)tst >(int)SQLiteTempStoreTypes::MAX)
		{
			lastErrorCode = -1;
			lastErrorMessage = "bad SQLiteTempStoreTypes";
			return lastErrorCode;
		}
		sqlBuilder->Clear();
		sqlBuilder->Append("PRAGMA temp_store = ", SQLiteTempStoreTypes_ss[(int)tst]);
		return Execute(sqlBuilder->C_str());
	}
	int SQLite::SetPragma(SQLiteLockingModes lm)
	{
		if ((int)lm < 0 || (int)lm >(int)SQLiteLockingModes::MAX)
		{
			lastErrorCode = -1;
			lastErrorMessage = "bad SQLiteLockingModes";
			return lastErrorCode;
		}
		sqlBuilder->Clear();
		sqlBuilder->Append("PRAGMA locking_mode = ", SQLiteLockingModes_ss[(int)lm]);
		return Execute(sqlBuilder->C_str());
	}

	template<typename ... Parameters>
	int SQLite::SetPragmas(Parameters const& ... ps)
	{
		return SetPragmasCore(ps...);
	}

	template<typename Parameter, typename ... Parameters>
	int SQLite::SetPragmasCore(Parameter const& p, Parameters const& ... ps)
	{
		if (auto r = SetPragma(p)) return r;
		return SetPragmasCore(ps...);
	}
	int SQLite::SetPragmasCore() { return 0; }


	int SQLite::SetPragma(int cacheSize)
	{
		if (cacheSize < 1)
		{
			lastErrorCode = -1;
			lastErrorMessage = "bad cacheSize( default is 2000 )";
			return lastErrorCode;
		}
		sqlBuilder->Clear();
		sqlBuilder->Append("PRAGMA default_cache_size = ", cacheSize);
		if (auto r = Execute(sqlBuilder->C_str())) return r;

		sqlBuilder->Clear();
		sqlBuilder->Append("PRAGMA cache_size = ", cacheSize);
		if (auto r = Execute(sqlBuilder->C_str())) return r;
		return 0;
	}


	inline SQLite::~SQLite()
	{
		for (int i = (int)queries->dataLen - 1; i >= 0; --i)
		{
			mempool().Release(queries->At(i));
		}
		sqlite3_close(dbctx);
		dbctx = nullptr;
	}

	inline SQLiteQuery* SQLite::CreateQuery(char const* const& sql, int const& sqlLen)
	{
		return mempool().Create<SQLiteQuery>(this, sql, sqlLen ? sqlLen : (int)strlen(sql));
	}

	inline int SQLite::Execute(char const* const& sql)
	{
		return lastErrorCode = sqlite3_exec(dbctx, sql, nullptr, nullptr, (char**)&lastErrorMessage);
	}


	int SQLite::BeginTransaction()
	{
		return Execute("BEGIN TRANSACTION");
	}

	int SQLite::Commit()
	{
		return Execute("COMMIT TRANSACTION");
	}

	int SQLite::Rollback()
	{
		return Execute("ROLLBACK TRANSACTION");
	}

	int SQLite::EndTransaction()
	{
		return Execute("END TRANSACTION");
	}

	// 返回值 -n: 执行出错  0: 未找到   1: 找到
	int SQLite::TableExists(char const* const& tn)
	{
		if (!query_Exists)
		{
			query_Exists = CreateQuery("SELECT count(*) FROM sqlite_master WHERE type = 'table' AND name = ?");
		}
		if (!query_Exists) return -1;
		if (query_Exists->SetParameters(tn)) return -2;
		bool exists;
		bool success = query_Exists->Execute([&](SQLiteReader& dr)
		{
			exists = dr.ReadInt32(0) > 0;
		});
		if (!success) return -3;
		return exists ? 1 : 0;
	}



	/***************************************************************/
	// SQLiteQuery

	inline SQLiteQuery::SQLiteQuery(SQLite* owner, char const* const& sql, int const& sqlLen)
		: owner(owner)
	{
		auto r = sqlite3_prepare_v2(owner->dbctx, sql, sqlLen, &stmt, nullptr);
		if (r != SQLITE_OK)
		{
			owner->lastErrorCode = r;
			owner->lastErrorMessage = sqlite3_errmsg(owner->dbctx);
			throw r;
		}

		auto s = sql;	// 已知问题: s + 1 会导致第 1 个字符检测不到. 但 sql 中第 1 个字符不可能是 ?
		while (s = strchr(s + 1, '?')) ++numParams;	// numParams = std::count(sql, sql + sqlLen, '?');

		owner_queries_index = owner->queries->dataLen;
		owner->queries->Add(this);
	}

	inline SQLiteQuery::~SQLiteQuery()
	{
		sqlite3_finalize(stmt);
		XX_LIST_SWAP_REMOVE(owner->queries, this, owner_queries_index);
	}

	inline int SQLiteQuery::SetParameter(int parmIdx, int const& v)
	{
		return sqlite3_bind_int(stmt, parmIdx, v);
	}
	inline int SQLiteQuery::SetParameter(int parmIdx, sqlite_int64 const& v)
	{
		return sqlite3_bind_int64(stmt, parmIdx, v);
	}
	inline int SQLiteQuery::SetParameter(int parmIdx, double const& v)
	{
		return sqlite3_bind_double(stmt, parmIdx, v);
	}
	inline int SQLiteQuery::SetParameter(int parmIdx, char const* const& str, int strLen, bool makeCopy)
	{
		if (!str) sqlite3_bind_null(stmt, parmIdx);
		return sqlite3_bind_text(stmt, parmIdx, str, strLen ? strLen : (int)strlen(str), makeCopy ? SQLITE_TRANSIENT : SQLITE_STATIC);
	}
	inline int SQLiteQuery::SetParameter(int parmIdx, char const* const& buf, size_t const& len, bool makeCopy)
	{
		if (!buf) sqlite3_bind_null(stmt, parmIdx);
		return sqlite3_bind_blob(stmt, parmIdx, buf, (int)len, makeCopy ? SQLITE_TRANSIENT : SQLITE_STATIC);
	}
	inline int SQLiteQuery::SetParameter(int parmIdx, String* const& str, bool makeCopy)
	{
		if (!str) sqlite3_bind_null(stmt, parmIdx);
		return sqlite3_bind_text(stmt, parmIdx, str->C_str(), str->dataLen, makeCopy ? SQLITE_TRANSIENT : SQLITE_STATIC);
	}
	inline int SQLiteQuery::SetParameter(int parmIdx, BBuffer* const& buf, bool makeCopy)
	{
		if (!buf) sqlite3_bind_null(stmt, parmIdx);
		return sqlite3_bind_blob(stmt, parmIdx, buf->buf, buf->dataLen, makeCopy ? SQLITE_TRANSIENT : SQLITE_STATIC);
	}
	inline int SQLiteQuery::SetParameter(int parmIdx, String_v const& str, bool makeCopy)
	{
		return sqlite3_bind_text(stmt, parmIdx, (*(String_v*)&str)->C_str(), str->dataLen, makeCopy ? SQLITE_TRANSIENT : SQLITE_STATIC);
	}
	inline int SQLiteQuery::SetParameter(int parmIdx, BBuffer_v const& buf, bool makeCopy)
	{
		return sqlite3_bind_blob(stmt, parmIdx, buf->buf, buf->dataLen, makeCopy ? SQLITE_TRANSIENT : SQLITE_STATIC);
	}
	inline int SQLiteQuery::SetParameter(int parmIdx, String_p const& str, bool makeCopy)
	{
		if (!str) sqlite3_bind_null(stmt, parmIdx);
		return sqlite3_bind_text(stmt, parmIdx, (*(String_v*)&str)->C_str(), str->dataLen, makeCopy ? SQLITE_TRANSIENT : SQLITE_STATIC);
	}
	inline int SQLiteQuery::SetParameter(int parmIdx, BBuffer_p const& buf, bool makeCopy)
	{
		if (!buf) sqlite3_bind_null(stmt, parmIdx);
		return sqlite3_bind_blob(stmt, parmIdx, buf->buf, buf->dataLen, makeCopy ? SQLITE_TRANSIENT : SQLITE_STATIC);
	}


	template<typename ... Parameters>
	int SQLiteQuery::SetParameters(Parameters const& ... ps)
	{
		int parmIdx = 1;
		if (auto r = SetParametersCore(parmIdx, ps...))
		{
			owner->lastErrorCode = r;
			owner->lastErrorMessage = sqlite3_errmsg(owner->dbctx);
			return r;
		}
		return 0;
	}

	template<typename Parameter, typename ... Parameters>
	int SQLiteQuery::SetParametersCore(int& parmIdx, Parameter const& p, Parameters const& ... ps)
	{
		if (int r = SetParameter(parmIdx, p)) return r;
		return SetParametersCore(++parmIdx, ps...);
	}

	int SQLiteQuery::SetParametersCore(int& parmIdx) { return 0; }

	inline bool SQLiteQuery::Execute(ReadFunc && rf)
	{
		int r = sqlite3_step(stmt);
		if (!(r == SQLITE_OK || r == SQLITE_DONE || r == SQLITE_ROW))
		{
			owner->lastErrorCode = r;
			owner->lastErrorMessage = sqlite3_errmsg(owner->dbctx);
			return false;
		}
		if (rf)
		{
			SQLiteReader dr(stmt);
			while (r == SQLITE_ROW)
			{
				rf(dr);
				r = sqlite3_step(stmt);
			}
		}
		r = sqlite3_reset(stmt);
		if (r == SQLITE_OK || r == SQLITE_DONE) return true;
		owner->lastErrorCode = r;
		owner->lastErrorMessage = sqlite3_errmsg(owner->dbctx);
		return false;
	}



	/***************************************************************/
	// SQLiteReader

	inline SQLiteReader::SQLiteReader(sqlite3_stmt* stmt) : stmt(stmt) {}

	inline int SQLiteReader::GetColumnCount()
	{
		return sqlite3_column_count(stmt);
	}

	inline SQLiteDataTypes SQLiteReader::GetColumnDataType(int colIdx)
	{
		assert(colIdx >= 0 && colIdx < GetColumnCount());
		return (SQLiteDataTypes)sqlite3_column_type(stmt, colIdx);
	}

	inline char const* SQLiteReader::GetColumnName(int colIdx)
	{
		assert(colIdx >= 0 && colIdx < GetColumnCount());
		return sqlite3_column_name(stmt, colIdx);
	}

	inline bool SQLiteReader::IsNull(int colIdx)
	{
		assert(colIdx >= 0 && colIdx < GetColumnCount());
		return GetColumnDataType(colIdx) == SQLiteDataTypes::Null;
	}

	inline char const* SQLiteReader::ReadString(int colIdx)
	{
		assert(colIdx >= 0 && colIdx < GetColumnCount());
		if (IsNull(colIdx)) return nullptr;
		return (char const*)sqlite3_column_text(stmt, colIdx);
	}

	inline int SQLiteReader::ReadInt32(int colIdx)
	{
		assert(colIdx >= 0 && colIdx < GetColumnCount() && !IsNull(colIdx));
		return sqlite3_column_int(stmt, colIdx);
	}

	inline sqlite_int64 SQLiteReader::ReadInt64(int colIdx)
	{
		assert(colIdx >= 0 && colIdx < GetColumnCount() && !IsNull(colIdx));
		return sqlite3_column_int64(stmt, colIdx);
	}

	inline double SQLiteReader::ReadDouble(int colIdx)
	{
		assert(colIdx >= 0 && colIdx < GetColumnCount() && !IsNull(colIdx));
		return sqlite3_column_double(stmt, colIdx);
	}

	inline std::pair<char const*, int> SQLiteReader::ReadText(int colIdx)
	{
		auto ptr = (char const*)sqlite3_column_text(stmt, colIdx);
		auto len = sqlite3_column_bytes(stmt, colIdx);
		return std::make_pair(ptr, len);
	}

	inline std::pair<char const*, int> SQLiteReader::ReadBlob(int colIdx)
	{
		auto ptr = (char const*)sqlite3_column_blob(stmt, colIdx);
		auto len = sqlite3_column_bytes(stmt, colIdx);
		return std::make_pair(ptr, len);
	}






	/***************************************************************************************/

	// 针对 List<T>, 生成 ( xx, xx, xx )
	struct SQLiteString : String
	{
		SQLiteString() : String() {}

		// 用于插入数字
		template<typename T>
		void SQLAppend(T const& v)
		{
			static_assert(std::is_arithmetic<T>::value, "");
			Append(v);
		}

		// 插入字串
		void SQLAppend(char const* const& v, uint32_t len = 0)
		{
			if (!v)
			{
				Append("null");
				return;
			}
			// 转义 '	// todo: 感觉有必要直接找到 sqlite 的转义函数抄一段. 这个先用着
			if (!len) len = (uint32_t)strlen(v);
			Reserve(dataLen + len * 2 + 2);
			buf[dataLen++] = '\'';
			for (uint32_t i = 0; i < len; ++i)
			{
				if (v[i] != '\'')
				{
					buf[dataLen++] = v[i];
				}
				else
				{
					buf[dataLen] = '\'';
					buf[dataLen + 1] = '\'';
					dataLen += 2;
				}
			}
			buf[dataLen++] = '\'';
		}

		// 同上
		void SQLAppend(String* const& v)
		{
			SQLAppend((v ? v->C_str() : nullptr), (v ? v->dataLen : 0));
		}
		void SQLAppend(String_v& v)
		{
			return SQLAppend(&*v);
		}
		void SQLAppend(String_p& v)
		{
			return SQLAppend(v.pointer);
		}

		static constexpr char* const hexStr = "0123456789abcdef";
		// 插入 BLOB
		void SQLAppend(BBuffer* const& v)
		{
			if (!v)
			{
				Append("null");
				return;
			}
			// 将 v 转成 x'11aa22bb....' (  sqlite3.c  71051 行 )
			auto len = v->dataLen;
			Reserve(dataLen + len * 2 + 3);
			this->buf[dataLen] = 'x';
			this->buf[dataLen + 1] = '\'';
			this->buf[dataLen + len * 2 + 2] = '\'';
			dataLen += 2;
			for (uint32_t i = 0; i < len; i++)
			{
				v->buf[dataLen + i * 2 + 0] = hexStr[(uint8_t)buf[i] >> 4];
				v->buf[dataLen + i * 2 + 1] = hexStr[buf[i] & 0x0F];
			}
			dataLen += len * 2 + 1;
		}
		void SQLAppend(BBuffer_v& v)
		{
			return SQLAppend(&*v);
		}
		void SQLAppend(BBuffer_p& v)
		{
			return SQLAppend(v.pointer);
		}


		// List 的重载
		template<typename T>
		void SQLAppend(List<T>* const& os)
		{
			assert(os && os->dataLen);
			Append("( ");
			for (uint32_t i = 0; i < os->dataLen; ++i)
			{
				SQLAppend(os->At(i));
				Append(", ");
			};
			dataLen -= 2;
			Append(" )");
		}

		template<typename T>
		void SQLAppend(List_v<T> const& os)
		{
			return SQLAppend(&*os);
		}

		template<typename T>
		void SQLAppend(List_p<T> const& os)
		{
			return SQLAppend(os.pointer);
		}
	};


	using SQLiteString_p = Ptr<SQLiteString>;
	using SQLiteString_v = MemHeaderBox<SQLiteString>;
	template<>
	struct MemmoveSupport<SQLiteString_v>
	{
		static const bool value = true;
	};

}
