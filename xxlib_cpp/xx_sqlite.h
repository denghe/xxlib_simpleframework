#pragma once
#include <xx_mempool.h>
#include <sqlite3.h>

// todo: _v _p support

namespace xx
{
	struct SQLite;
	struct SQLiteQuery;
	struct SQLiteReader;

	using SQLite_p = Ptr<SQLite>;
	using SQLite_v = Dock<SQLite>;
	template<>
	struct MemmoveSupport<SQLite_v>
	{
		static const bool value = true;
	};

	using SQLiteQuery_p = Ptr<SQLiteQuery>;
	using SQLiteQuery_v = Dock<SQLiteQuery>;
	template<>
	struct MemmoveSupport<SQLiteQuery_v>
	{
		static const bool value = true;
	};

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


	// 搞出个基类只是为了控制对象回收的顺序
	struct SQLiteBase : MPObject
	{
		SQLiteBase(char const* const& fn, bool readOnly);
		~SQLiteBase();
		sqlite3* dbctx = nullptr;
	};

	struct SQLite : SQLiteBase
	{
		// 临时拿来拼 sql 串
		xx::String_v sqlBuilder;

		// 保存最后一次的错误信息
		int lastErrorCode = 0;
		const char* lastErrorMessage = nullptr;

		// 为设置错误提供便利
		void ThrowError(int const& errCode, char const* const& errMsg = nullptr);

		// 获取受影响行数
		int GetAffectedRows();

		SQLiteQuery_p query_BeginTransaction = nullptr;
		SQLiteQuery_p query_Commit = nullptr;
		SQLiteQuery_p query_Rollback = nullptr;
		SQLiteQuery_p query_EndTransaction = nullptr;
		SQLiteQuery_p query_TableExists = nullptr;
		SQLiteQuery_p query_GetTableCount = nullptr;

		SQLite(char const* const& fn, bool readOnly = false);

		// 下列函数均靠 try 检测是否执行出错

		// 根据 SQL 语句, 创建一个查询对象( 有错误发生将返回空 )
		SQLiteQuery_p CreateQuery(char const* const& sql, int const& sqlLen = 0);

		// 各种 set pragma
		void SetPragma(SQLiteSynchronousTypes st);
		void SetPragma(SQLiteJournalModes jm);
		void SetPragma(SQLiteTempStoreTypes tst);
		void SetPragma(SQLiteLockingModes lm);
		void SetPragma(int cacheSize);

		template<typename ... Parameters>
		void SetPragmas(Parameters const& ... ps);
		template<typename Parameter, typename ... Parameters>
		void SetPragmasCore(Parameter const& p, Parameters const& ... ps);
		void SetPragmasCore();

		// 启动事务
		void BeginTransaction();					// BEGIN TRANSACTION

		// 提交事务
		void Commit();								// COMMIT TRANSACTION

		// 回滚
		void Rollback();							// ROLLBACK TRANSACTION

		// 结束事务( 同 Commit )
		void EndTransaction();						// END TRANSACTION

		// 返回 1 表示只包含 'sqlite_sequence' 这样一个预创建表. android 下也有可能返回 2, 有张 android 字样的预创建表存在
		int GetTableCount();						// SELECT count(*) FROM sqlite_master

		bool TableExists(char const* const& tn);	// SELECT count(*) FROM sqlite_master WHERE type = 'table' AND name = ?

		// 会清空表数据, 并且重置自增计数. 如果存在约束, 有可能清空失败.
		void TruncateTable(char const* const& tn);	// DELETE FROM ?; DELETE FROM sqlite_sequence WHERE name = ?;

		// 直接执行一个 SQL 语句( 相当于 create query + execute + destroy 全套 )
		void Execute(char const* const& sql, int(*selectRowCB)(void* userData, int numCols, char** colValues, char** colNames) = nullptr, void* const& userData = nullptr);

	};


	struct SQLiteQuery : MPObject
	{
		SQLite& owner;
		sqlite3_stmt* stmt = nullptr;
		typedef std::function<void(SQLiteReader& sr)> ReadFunc;

		SQLiteQuery(SQLite& owner, char const* const& sql, int const& sqlLen);
		~SQLiteQuery();

		// 下面这些函数都是靠 try 来检测错误

		void SetParameter(int parmIdx, int const& v);
		void SetParameter(int parmIdx, sqlite_int64 const& v);
		void SetParameter(int parmIdx, double const& v);
		void SetParameter(int parmIdx, char const* const& str, int strLen = 0, bool makeCopy = false);
		void SetParameter(int parmIdx, char const* const& buf, size_t const& len, bool makeCopy = false);
		void SetParameter(int parmIdx, String* const& str, bool makeCopy = false);
		void SetParameter(int parmIdx, BBuffer* const& buf, bool makeCopy = false);
		void SetParameter(int parmIdx, String_v const& str, bool makeCopy = false);
		void SetParameter(int parmIdx, BBuffer_v const& buf, bool makeCopy = false);
		void SetParameter(int parmIdx, String_p const& str, bool makeCopy = false);
		void SetParameter(int parmIdx, BBuffer_p const& buf, bool makeCopy = false);

		template<typename ... Parameters>
		void SetParameters(Parameters const& ... ps);
		template<typename Parameter, typename ... Parameters>
		void SetParametersCore(int& parmIdx, Parameter const& p, Parameters const& ... ps);
		void SetParametersCore(int& parmIdx);

		void Execute(ReadFunc && rf = nullptr);
	};



	struct SQLiteReader
	{
		sqlite3_stmt* stmt;
		int numCols = 0;

		SQLiteReader(sqlite3_stmt* stmt);
		SQLiteDataTypes GetColumnDataType(int colIdx);
		char const* GetColumnName(int colIdx);
		bool IsDBNull(int colIdx);
		int ReadInt32(int colIdx);
		sqlite_int64 ReadInt64(int colIdx);
		double ReadDouble(int colIdx);
		char const* ReadString(int colIdx);
		std::pair<char const*, int> ReadText(int colIdx);
		std::pair<char const*, int> ReadBlob(int colIdx);
	};





	/******************************************************************************************************/
	// impls
	/******************************************************************************************************/


	inline SQLiteBase::SQLiteBase(char const* const& fn, bool readOnly)
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
	inline SQLiteBase::~SQLiteBase()
	{
		sqlite3_close(dbctx);
		dbctx = nullptr;
	}

	/***************************************************************/
	// SQLite

	inline SQLite::SQLite(char const* const& fn, bool readOnly)
		: SQLiteBase(fn, readOnly)
		, sqlBuilder(mempool())
	{
	}

	inline void SQLite::ThrowError(int const& errCode, char const* const& errMsg)
	{
		lastErrorCode = errCode;
		lastErrorMessage = errMsg ? errMsg : sqlite3_errmsg(dbctx);
		throw errCode;
	}

	inline int SQLite::GetAffectedRows()
	{
		return sqlite3_changes(dbctx);
	}

	inline SQLiteQuery_p SQLite::CreateQuery(char const* const& sql, int const& sqlLen)
	{
		auto r = mempool().Create<SQLiteQuery>(*this, sql, sqlLen ? sqlLen : (int)strlen(sql));
		if (!r) throw lastErrorCode;
		return r;
	}

	inline void SQLite::SetPragma(SQLiteSynchronousTypes st)
	{
		if ((int)st < 0 || (int)st >(int)SQLiteSynchronousTypes::MAX) ThrowError(-1, "bad SQLiteSynchronousTypes");
		sqlBuilder->Clear();
		sqlBuilder->Append("PRAGMA synchronous = ", SQLiteSynchronousTypes_ss[(int)st]);
		Execute(sqlBuilder->C_str());
	}

	inline void SQLite::SetPragma(SQLiteJournalModes jm)
	{
		if ((int)jm < 0 || (int)jm >(int)SQLiteJournalModes::MAX) ThrowError(-1, "bad SQLiteJournalModes");
		sqlBuilder->Clear();
		sqlBuilder->Append("PRAGMA journal_mode = ", SQLiteJournalModes_ss[(int)jm]);
		Execute(sqlBuilder->C_str());
	}

	inline void SQLite::SetPragma(SQLiteTempStoreTypes tst)
	{
		if ((int)tst < 0 || (int)tst >(int)SQLiteTempStoreTypes::MAX) ThrowError(-1, "bad SQLiteTempStoreTypes");
		sqlBuilder->Clear();
		sqlBuilder->Append("PRAGMA temp_store = ", SQLiteTempStoreTypes_ss[(int)tst]);
		Execute(sqlBuilder->C_str());
	}

	inline void SQLite::SetPragma(SQLiteLockingModes lm)
	{
		if ((int)lm < 0 || (int)lm >(int)SQLiteLockingModes::MAX) ThrowError(-1, "bad SQLiteLockingModes");
		sqlBuilder->Clear();
		sqlBuilder->Append("PRAGMA locking_mode = ", SQLiteLockingModes_ss[(int)lm]);
		Execute(sqlBuilder->C_str());
	}

	template<typename ... Parameters>
	void SQLite::SetPragmas(Parameters const& ... ps)
	{
		SetPragmasCore(ps...);
	}

	template<typename Parameter, typename ... Parameters>
	void SQLite::SetPragmasCore(Parameter const& p, Parameters const& ... ps)
	{
		SetPragma(p);
		SetPragmasCore(ps...);
	}
	inline void SQLite::SetPragmasCore() {}

	inline void SQLite::SetPragma(int cacheSize)
	{
		if (cacheSize < 1) ThrowError(-1, "bad cacheSize( default is 2000 )");
		sqlBuilder->Clear();
		sqlBuilder->Append("PRAGMA default_cache_size = ", cacheSize);
		Execute(sqlBuilder->C_str());

		sqlBuilder->Clear();
		sqlBuilder->Append("PRAGMA cache_size = ", cacheSize);
		Execute(sqlBuilder->C_str());
	}


	inline void SQLite::Execute(char const* const& sql, int(*selectRowCB)(void* userData, int numCols, char** colValues, char** colNames), void* const& userData)
	{
		lastErrorCode = sqlite3_exec(dbctx, sql, selectRowCB, userData, (char**)&lastErrorMessage);
		if (lastErrorCode != SQLITE_OK) throw lastErrorCode;
	}


	inline void SQLite::BeginTransaction()
	{
		auto& q = query_BeginTransaction;
		if (!q) q = CreateQuery("BEGIN TRANSACTION");
		q->Execute();
	}

	inline void SQLite::Commit()
	{
		auto& q = query_Commit;
		if (!q) q = CreateQuery("COMMIT TRANSACTION");
		q->Execute();
	}

	inline void SQLite::Rollback()
	{
		auto& q = query_Rollback;
		if (!q) q = CreateQuery("ROLLBACK TRANSACTION");
		q->Execute();
	}

	inline void SQLite::EndTransaction()
	{
		auto& q = query_EndTransaction;
		if (!q) q = CreateQuery("END TRANSACTION");
		q->Execute();
	}

	inline bool SQLite::TableExists(char const* const& tn)
	{
		auto& q = query_TableExists;
		if (!q) q = CreateQuery("SELECT count(*) FROM sqlite_master WHERE type = 'table' AND name = ?");
		q->SetParameters(tn);
		bool exists;
		q->Execute([&](SQLiteReader& dr)
		{
			exists = dr.ReadInt32(0) > 0;
		});
		return exists ? 1 : 0;
	}

	inline int SQLite::GetTableCount()
	{
		auto& q = query_GetTableCount;
		if (!q) q = CreateQuery("SELECT count(*) FROM sqlite_master WHERE type = 'table'");
		int count = 0;
		q->Execute([&](SQLiteReader& dr)
		{
			count = dr.ReadInt32(0);
		});
		return count;
	}

	inline void SQLite::TruncateTable(char const* const& tn)
	{
		sqlBuilder->Clear();
		sqlBuilder->Append("BEGIN; DELETE FROM [", tn, "]; DELETE FROM [sqlite_sequence] WHERE [name] = '", tn, "'; COMMIT;");
		Execute(sqlBuilder->C_str());
	}




	/***************************************************************/
	// SQLiteQuery

	inline SQLiteQuery::SQLiteQuery(SQLite& owner, char const* const& sql, int const& sqlLen)
		: owner(owner)
	{
		auto r = sqlite3_prepare_v2(owner.dbctx, sql, sqlLen, &stmt, nullptr);
		if (r != SQLITE_OK) owner.ThrowError(r);
	}

	inline SQLiteQuery::~SQLiteQuery()
	{
		sqlite3_finalize(stmt);
	}

	inline void SQLiteQuery::SetParameter(int parmIdx, int const& v)
	{
		auto r = sqlite3_bind_int(stmt, parmIdx, v);
		if (r != SQLITE_OK) owner.ThrowError(r);
	}

	inline void SQLiteQuery::SetParameter(int parmIdx, sqlite_int64 const& v)
	{
		auto r = sqlite3_bind_int64(stmt, parmIdx, v);
		if (r != SQLITE_OK) owner.ThrowError(r);
	}

	inline void SQLiteQuery::SetParameter(int parmIdx, double const& v)
	{
		auto r = sqlite3_bind_double(stmt, parmIdx, v);
		if (r != SQLITE_OK) owner.ThrowError(r);
	}

	inline void SQLiteQuery::SetParameter(int parmIdx, char const* const& str, int strLen, bool makeCopy)
	{
		int r = SQLITE_OK;
		if (!str) r = sqlite3_bind_null(stmt, parmIdx);
		if (r != SQLITE_OK) owner.ThrowError(r);
		r = sqlite3_bind_text(stmt, parmIdx, str, strLen ? strLen : (int)strlen(str), makeCopy ? SQLITE_TRANSIENT : SQLITE_STATIC);
		if (r != SQLITE_OK) owner.ThrowError(r);
	}

	inline void SQLiteQuery::SetParameter(int parmIdx, char const* const& buf, size_t const& len, bool makeCopy)
	{
		int r = SQLITE_OK;
		if (!buf) r = sqlite3_bind_null(stmt, parmIdx);
		if (r != SQLITE_OK) owner.ThrowError(r);
		r = sqlite3_bind_blob(stmt, parmIdx, buf, (int)len, makeCopy ? SQLITE_TRANSIENT : SQLITE_STATIC);
		if (r != SQLITE_OK) owner.ThrowError(r);
	}

	inline void SQLiteQuery::SetParameter(int parmIdx, String* const& str, bool makeCopy)
	{
		int r = SQLITE_OK;
		if (!str) r = sqlite3_bind_null(stmt, parmIdx);
		if (r != SQLITE_OK) owner.ThrowError(r);
		r = sqlite3_bind_text(stmt, parmIdx, str->C_str(), str->dataLen, makeCopy ? SQLITE_TRANSIENT : SQLITE_STATIC);
		if (r != SQLITE_OK) owner.ThrowError(r);
	}

	inline void SQLiteQuery::SetParameter(int parmIdx, BBuffer* const& buf, bool makeCopy)
	{
		int r = SQLITE_OK;
		if (!buf) r = sqlite3_bind_null(stmt, parmIdx);
		if (r != SQLITE_OK) owner.ThrowError(r);
		r = sqlite3_bind_blob(stmt, parmIdx, buf->buf, buf->dataLen, makeCopy ? SQLITE_TRANSIENT : SQLITE_STATIC);
		if (r != SQLITE_OK) owner.ThrowError(r);

	}

	inline void SQLiteQuery::SetParameter(int parmIdx, String_v const& str, bool makeCopy)
	{
		auto r = sqlite3_bind_text(stmt, parmIdx, (*(String_v*)&str)->C_str(), str->dataLen, makeCopy ? SQLITE_TRANSIENT : SQLITE_STATIC);
		if (r != SQLITE_OK) owner.ThrowError(r);
	}

	inline void SQLiteQuery::SetParameter(int parmIdx, BBuffer_v const& buf, bool makeCopy)
	{
		auto r = sqlite3_bind_blob(stmt, parmIdx, buf->buf, buf->dataLen, makeCopy ? SQLITE_TRANSIENT : SQLITE_STATIC);
		if (r != SQLITE_OK) owner.ThrowError(r);
	}

	inline void SQLiteQuery::SetParameter(int parmIdx, String_p const& str, bool makeCopy)
	{
		int r = SQLITE_OK;
		if (!str) r = sqlite3_bind_null(stmt, parmIdx);
		if (r != SQLITE_OK) owner.ThrowError(r);
		r = sqlite3_bind_text(stmt, parmIdx, (*(String_p*)&str)->C_str(), str->dataLen, makeCopy ? SQLITE_TRANSIENT : SQLITE_STATIC);
		if (r != SQLITE_OK) owner.ThrowError(r);
	}

	inline void SQLiteQuery::SetParameter(int parmIdx, BBuffer_p const& buf, bool makeCopy)
	{
		int r = SQLITE_OK;
		if (!buf) r = sqlite3_bind_null(stmt, parmIdx);
		if (r != SQLITE_OK) owner.ThrowError(r);
		r = sqlite3_bind_blob(stmt, parmIdx, buf->buf, buf->dataLen, makeCopy ? SQLITE_TRANSIENT : SQLITE_STATIC);
		if (r != SQLITE_OK) owner.ThrowError(r);
	}


	template<typename ... Parameters>
	void SQLiteQuery::SetParameters(Parameters const& ... ps)
	{
		int parmIdx = 1;
		SetParametersCore(parmIdx, ps...);
	}

	template<typename Parameter, typename ... Parameters>
	void SQLiteQuery::SetParametersCore(int& parmIdx, Parameter const& p, Parameters const& ... ps)
	{
		SetParameter(parmIdx, p);
		SetParametersCore(++parmIdx, ps...);
	}

	inline void SQLiteQuery::SetParametersCore(int& parmIdx) {}

	inline void SQLiteQuery::Execute(ReadFunc && rf)
	{
		SQLiteReader dr(stmt);

		int r = sqlite3_step(stmt);
		if (r == SQLITE_DONE || r == SQLITE_ROW && !rf) goto LabEnd;
		else if (r != SQLITE_ROW) goto LabErr;

		dr.numCols = sqlite3_column_count(stmt);
		do
		{
			rf(dr);
			r = sqlite3_step(stmt);
		} while (r == SQLITE_ROW);
		assert(r == SQLITE_DONE);

	LabEnd:
		r = sqlite3_reset(stmt);
		if (r == SQLITE_OK) return;

	LabErr:
		auto ec = r;
		auto em = sqlite3_errmsg(owner.dbctx);
		sqlite3_reset(stmt);
		owner.ThrowError(ec, em);
	}



	/***************************************************************/
	// SQLiteReader

	inline SQLiteReader::SQLiteReader(sqlite3_stmt* stmt) : stmt(stmt) {}

	inline SQLiteDataTypes SQLiteReader::GetColumnDataType(int colIdx)
	{
		assert(colIdx >= 0 && colIdx < numCols);
		return (SQLiteDataTypes)sqlite3_column_type(stmt, colIdx);
	}

	inline char const* SQLiteReader::GetColumnName(int colIdx)
	{
		assert(colIdx >= 0 && colIdx < numCols);
		return sqlite3_column_name(stmt, colIdx);
	}

	inline bool SQLiteReader::IsDBNull(int colIdx)
	{
		assert(colIdx >= 0 && colIdx < numCols);
		return GetColumnDataType(colIdx) == SQLiteDataTypes::Null;
	}

	inline char const* SQLiteReader::ReadString(int colIdx)
	{
		assert(colIdx >= 0 && colIdx < numCols);
		if (IsDBNull(colIdx)) return nullptr;
		return (char const*)sqlite3_column_text(stmt, colIdx);
	}

	inline int SQLiteReader::ReadInt32(int colIdx)
	{
		assert(colIdx >= 0 && colIdx < numCols && !IsDBNull(colIdx));
		return sqlite3_column_int(stmt, colIdx);
	}

	inline sqlite_int64 SQLiteReader::ReadInt64(int colIdx)
	{
		assert(colIdx >= 0 && colIdx < numCols && !IsDBNull(colIdx));
		return sqlite3_column_int64(stmt, colIdx);
	}

	inline double SQLiteReader::ReadDouble(int colIdx)
	{
		assert(colIdx >= 0 && colIdx < numCols && !IsDBNull(colIdx));
		return sqlite3_column_double(stmt, colIdx);
	}

	inline std::pair<char const*, int> SQLiteReader::ReadText(int colIdx)
	{
		assert(colIdx >= 0 && colIdx < numCols);
		auto ptr = (char const*)sqlite3_column_text(stmt, colIdx);
		auto len = sqlite3_column_bytes(stmt, colIdx);
		return std::make_pair(ptr, len);
	}

	inline std::pair<char const*, int> SQLiteReader::ReadBlob(int colIdx)
	{
		assert(colIdx >= 0 && colIdx < numCols);
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
		inline void SQLAppend(char const* const& v, uint32_t len = 0)
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
		inline void SQLAppend(String* const& v)
		{
			SQLAppend((v ? v->C_str() : nullptr), (v ? v->dataLen : 0));
		}
		inline void SQLAppend(String_v& v)
		{
			return SQLAppend(&*v);
		}
		inline void SQLAppend(String_p& v)
		{
			return SQLAppend(v.pointer);
		}

		static constexpr char* const hexStr = "0123456789abcdef";
		// 插入 BLOB
		inline void SQLAppend(BBuffer* const& v)
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
		inline void SQLAppend(BBuffer_v& v)
		{
			return SQLAppend(&*v);
		}
		inline void SQLAppend(BBuffer_p& v)
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
	using SQLiteString_v = Dock<SQLiteString>;
	template<>
	struct MemmoveSupport<SQLiteString_v>
	{
		static const bool value = true;
	};

}
