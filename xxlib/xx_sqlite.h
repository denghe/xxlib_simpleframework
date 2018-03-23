#pragma once
#include "sqlite3.h"

namespace xx
{
	class SQLite;
	class SQLiteQuery;
	class SQLiteReader;
	// SQLiteString 在最下面

	using SQLite_p = Ptr<SQLite>;
	using SQLite_r = Ref<SQLite>;

	using SQLiteQuery_p = Ptr<SQLiteQuery>;
	using SQLiteQuery_r = Ref<SQLiteQuery>;

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
	// 排它锁持有方式，仅当关闭数据库连接，或者将锁模式改回为NORMAL时，再次访问数据库文件（读或写）才会放掉
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
	class SQLiteBase : public Object
	{
	protected:
		SQLiteBase(MemPool* mp, char const* const& fn, bool readOnly);
		~SQLiteBase();
		sqlite3* dbctx = nullptr;
	};

	class SQLite : public SQLiteBase
	{
	public:
		// 临时拿来拼 sql 串
		String sqlBuilder;

		// 保存最后一次的错误信息
		int lastErrorCode = 0;
		const char* lastErrorMessage = nullptr;

	private:
		friend SQLiteQuery;

		// 为throw错误提供便利
		void ThrowError(int const& errCode, char const* const& errMsg = nullptr);

		SQLiteQuery_p query_BeginTransaction;
		SQLiteQuery_p query_Commit;
		SQLiteQuery_p query_Rollback;
		SQLiteQuery_p query_EndTransaction;
		SQLiteQuery_p query_TableExists;
		SQLiteQuery_p query_GetTableCount;
		SQLiteQuery_p query_Attach;
		SQLiteQuery_p query_Detach;
	public:

		// fn 可以是 :memory: 以创建内存数据库
		SQLite(MemPool* mp, char const* const& fn, bool readOnly = false);

		// 获取受影响行数
		int GetAffectedRows();

		// 下列函数均靠 try 检测是否执行出错

		// 根据 SQL 语句, 创建一个查询对象( 有错误发生将返回空 )
		SQLiteQuery_p CreateQuery(char const* const& sql, int const& sqlLen = 0);

		// 各种 set pragma( 通常推荐设置 SQLiteJournalModes::WAL 似乎能提升一些 insert 的性能 )

		// 事务数据记录模式( 设成 WAL 能提升一些性能 )
		void SetPragmaJournalMode(SQLiteJournalModes jm);

		// 启用外键约束( 默认为未启用 )
		void SetPragmaForeignKeys(bool enable);

		// 数据写盘模式
		void SetPragmaSynchronousType(SQLiteSynchronousTypes st);

		// 临时表处理模式
		void SetPragmaTempStoreType(SQLiteTempStoreTypes tst);

		// 排它锁持有方式
		void SetPragmaLockingMode(SQLiteLockingModes lm);

		// 内存数据库页数
		void SetPragmaCacheSize(int cacheSize);


		// 附加另外一个库
		void Attach(char const* const& alias, char const* const& fn);	// ATTACH DATABASE 'fn' AS 'alias'

		// 反附加另外一个库
		void Detach(char const* const& alias);							// DETACH DATABASE 'alias'

		// 启动事务
		void BeginTransaction();										// BEGIN TRANSACTION

		// 提交事务
		void Commit();													// COMMIT TRANSACTION

		// 回滚
		void Rollback();												// ROLLBACK TRANSACTION

		// 结束事务( 同 Commit )
		void EndTransaction();											// END TRANSACTION

		// 返回 1 表示只包含 'sqlite_sequence' 这样一个预创建表. 
		// android 下也有可能返回 2, 有张 android 字样的预创建表存在
		int GetTableCount();											// SELECT count(*) FROM sqlite_master

		// 判断表是否存在
		bool TableExists(char const* const& tn);						// SELECT count(*) FROM sqlite_master WHERE type = 'table' AND name = ?

		// 会清空表数据, 并且重置自增计数. 如果存在约束, 有可能清空失败.
		void TruncateTable(char const* const& tn);						// DELETE FROM ?; DELETE FROM sqlite_sequence WHERE name = ?;

		// 直接执行一个 SQL 语句( 相当于 create query + execute + destroy 全套 )
		void Execute(char const* const& sql, int(*selectRowCB)(void* userData, int numCols, char** colValues, char** colNames) = nullptr, void* const& userData = nullptr);

	};


	class SQLiteQuery : public Object
	{
	protected:
		SQLite& owner;
		sqlite3_stmt* stmt = nullptr;
	public:
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
		void SetParameter(int parmIdx, String const& str, bool makeCopy = false);
		void SetParameter(int parmIdx, BBuffer const& buf, bool makeCopy = false);
		void SetParameter(int parmIdx, String_p const& str, bool makeCopy = false);
		void SetParameter(int parmIdx, BBuffer_p const& buf, bool makeCopy = false);
		template<typename EnumType>
		void SetParameter(int parmIdx, EnumType const& v);

		template<typename ... Parameters>
		void SetParameters(Parameters const& ... ps);
		template<typename Parameter, typename ... Parameters>
		void SetParametersCore(int& parmIdx, Parameter const& p, Parameters const& ... ps);
		void SetParametersCore(int& parmIdx);

		void Execute(ReadFunc && rf = nullptr);
	};



	class SQLiteReader
	{
	protected:
		sqlite3_stmt* stmt;
	public:
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


	inline SQLiteBase::SQLiteBase(MemPool* mp, char const* const& fn, bool readOnly)
		: Object(mp)
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

	inline SQLite::SQLite(MemPool* mp, char const* const& fn, bool readOnly)
		: SQLiteBase(mp, fn, readOnly)
		, sqlBuilder(mp)
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
		auto r = mempool->CreatePtr<SQLiteQuery>(*this, sql, sqlLen ? sqlLen : (int)strlen(sql));
		if (!r) throw lastErrorCode;
		return r;
	}

	inline void SQLite::SetPragmaSynchronousType(SQLiteSynchronousTypes st)
	{
		if ((int)st < 0 || (int)st >(int)SQLiteSynchronousTypes::MAX) ThrowError(-1, "bad SQLiteSynchronousTypes");
		sqlBuilder.Clear();
		sqlBuilder.Append("PRAGMA synchronous = ", SQLiteSynchronousTypes_ss[(int)st]);
		Execute(sqlBuilder.c_str());
	}

	inline void SQLite::SetPragmaJournalMode(SQLiteJournalModes jm)
	{
		if ((int)jm < 0 || (int)jm >(int)SQLiteJournalModes::MAX) ThrowError(-1, "bad SQLiteJournalModes");
		sqlBuilder.Clear();
		sqlBuilder.Append("PRAGMA journal_mode = ", SQLiteJournalModes_ss[(int)jm]);
		Execute(sqlBuilder.c_str());
	}

	inline void SQLite::SetPragmaTempStoreType(SQLiteTempStoreTypes tst)
	{
		if ((int)tst < 0 || (int)tst >(int)SQLiteTempStoreTypes::MAX) ThrowError(-1, "bad SQLiteTempStoreTypes");
		sqlBuilder.Clear();
		sqlBuilder.Append("PRAGMA temp_store = ", SQLiteTempStoreTypes_ss[(int)tst]);
		Execute(sqlBuilder.c_str());
	}

	inline void SQLite::SetPragmaLockingMode(SQLiteLockingModes lm)
	{
		if ((int)lm < 0 || (int)lm >(int)SQLiteLockingModes::MAX) ThrowError(-1, "bad SQLiteLockingModes");
		sqlBuilder.Clear();
		sqlBuilder.Append("PRAGMA locking_mode = ", SQLiteLockingModes_ss[(int)lm]);
		Execute(sqlBuilder.c_str());
	}

	inline void SQLite::SetPragmaCacheSize(int cacheSize)
	{
		if (cacheSize < 1) ThrowError(-1, "bad cacheSize( default is 2000 )");
		sqlBuilder.Clear();
		sqlBuilder.Append("PRAGMA cache_size = ", cacheSize);
		Execute(sqlBuilder.c_str());
	}

	inline void SQLite::SetPragmaForeignKeys(bool enable)
	{
		sqlBuilder.Clear();
		sqlBuilder.Append("PRAGMA foreign_keys = ", (enable ? "true" : "false"));
		Execute(sqlBuilder.c_str());
	}

	inline void SQLite::Execute(char const* const& sql, int(*selectRowCB)(void* userData, int numCols, char** colValues, char** colNames), void* const& userData)
	{
		lastErrorCode = sqlite3_exec(dbctx, sql, selectRowCB, userData, (char**)&lastErrorMessage);
		if (lastErrorCode != SQLITE_OK) throw lastErrorCode;
	}


	inline void SQLite::Attach(char const* const& alias, char const* const& fn)
	{
		auto& q = query_Attach;
		if (!q) q = CreateQuery("ATTACH DATABASE ? AS ?");
		q->SetParameters(fn, alias);
		q->Execute();
	}

	inline void SQLite::Detach(char const* const& alias)
	{
		auto& q = query_Detach;
		if (!q) q = CreateQuery("DETACH DATABASE ?");
		q->SetParameters(alias);
		q->Execute();
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
		// todo: 对 tn 转义
		sqlBuilder.Clear();
		sqlBuilder.Append("BEGIN; DELETE FROM [", tn, "]; DELETE FROM [sqlite_sequence] WHERE [name] = '", tn, "'; COMMIT;");
		Execute(sqlBuilder.c_str());
	}




	/***************************************************************/
	// SQLiteQuery

	inline SQLiteQuery::SQLiteQuery(SQLite& owner, char const* const& sql, int const& sqlLen)
		: Object(owner.mempool)
		, owner(owner)
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
		r = sqlite3_bind_text(stmt, parmIdx, str->c_str(), (int)str->dataLen, makeCopy ? SQLITE_TRANSIENT : SQLITE_STATIC);
		if (r != SQLITE_OK) owner.ThrowError(r);
	}

	inline void SQLiteQuery::SetParameter(int parmIdx, BBuffer* const& buf, bool makeCopy)
	{
		int r = SQLITE_OK;
		if (!buf) r = sqlite3_bind_null(stmt, parmIdx);
		if (r != SQLITE_OK) owner.ThrowError(r);
		r = sqlite3_bind_blob(stmt, parmIdx, buf->buf, (int)buf->dataLen, makeCopy ? SQLITE_TRANSIENT : SQLITE_STATIC);
		if (r != SQLITE_OK) owner.ThrowError(r);

	}

	inline void SQLiteQuery::SetParameter(int parmIdx, String const& str, bool makeCopy)
	{
		auto r = sqlite3_bind_text(stmt, parmIdx, ((String*)&str)->c_str(), (int)str.dataLen, makeCopy ? SQLITE_TRANSIENT : SQLITE_STATIC);
		if (r != SQLITE_OK) owner.ThrowError(r);
	}

	inline void SQLiteQuery::SetParameter(int parmIdx, BBuffer const& buf, bool makeCopy)
	{
		auto r = sqlite3_bind_blob(stmt, parmIdx, buf.buf, (int)buf.dataLen, makeCopy ? SQLITE_TRANSIENT : SQLITE_STATIC);
		if (r != SQLITE_OK) owner.ThrowError(r);
	}

	inline void SQLiteQuery::SetParameter(int parmIdx, String_p const& str, bool makeCopy)
	{
		int r = SQLITE_OK;
		if (!str) r = sqlite3_bind_null(stmt, parmIdx);
		if (r != SQLITE_OK) owner.ThrowError(r);
		r = sqlite3_bind_text(stmt, parmIdx, (*(String_p*)&str)->c_str(), (int)str->dataLen, makeCopy ? SQLITE_TRANSIENT : SQLITE_STATIC);
		if (r != SQLITE_OK) owner.ThrowError(r);
	}

	inline void SQLiteQuery::SetParameter(int parmIdx, BBuffer_p const& buf, bool makeCopy)
	{
		int r = SQLITE_OK;
		if (!buf) r = sqlite3_bind_null(stmt, parmIdx);
		if (r != SQLITE_OK) owner.ThrowError(r);
		r = sqlite3_bind_blob(stmt, parmIdx, buf->buf, (int)buf->dataLen, makeCopy ? SQLITE_TRANSIENT : SQLITE_STATIC);
		if (r != SQLITE_OK) owner.ThrowError(r);
	}

	template<typename EnumType>
	void SQLiteQuery::SetParameter(int parmIdx, EnumType const& v)
	{
		static_assert(std::is_enum<EnumType>::value, "parameter only support sqlite base types and enum types.");
		if (sizeof(EnumType) <= 4) SetParameter(parmIdx, (int)(typename std::underlying_type<EnumType>::type)v);
		else SetParameter(parmIdx, (sqlite_int64)(typename std::underlying_type<EnumType>::type)v);
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
		if (r == SQLITE_DONE || (r == SQLITE_ROW && !rf)) goto LabEnd;
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
	class SQLiteString : public String
	{
	public:
		SQLiteString(MemPool* mp) : String(mp) {}

		// 用于插入数字
		template<typename T>
		void SQLAppend(T const& v)
		{
			static_assert(std::is_arithmetic<T>::value, "");
			Append(v);
		}

		// 插入字串
		inline void SQLAppend(char const* const& v, size_t len = 0)
		{
			if (!v)
			{
				Append("null");
				return;
			}
			// 转义 '	// todo: 感觉有必要直接找到 sqlite 的转义函数抄一段. 这个先用着
			if (!len) len = strlen(v);
			Reserve(dataLen + len * 2 + 2);
			buf[dataLen++] = '\'';
			for (size_t i = 0; i < len; ++i)
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
			if(!v) Append("null");
			else SQLAppend(v->buf, v->dataLen);
		}
		inline void SQLAppend(String& v)
		{
			return SQLAppend(v.buf, v.dataLen);
		}
		inline void SQLAppend(String_p& v)
		{
			SQLAppend(v.pointer);
		}

		static constexpr char const* const hexStr = "0123456789abcdef";
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
		inline void SQLAppend(BBuffer& v)
		{
			SQLAppend(&v);
		}
		inline void SQLAppend(BBuffer_p& v)
		{
			SQLAppend(v.pointer);
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
		void SQLAppend(List<T> const& os)
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
	using SQLiteString_r = Ref<SQLiteString>;

}
