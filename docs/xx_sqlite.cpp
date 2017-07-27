#include "xx_sqlite.h"

namespace xx
{
	namespace sqlite
	{

		Connection::Connection()
			: db(nullptr)
			, writeable(false)
			, qExists(nullptr)
		{
		}

		Connection::Connection(std::string const& fn, bool writeable)
			: db(nullptr)
			, fileName(fn)
			, writeable(writeable)
			, qExists(nullptr)
		{
		}

		Connection::~Connection()
		{
			Close();
		}

		void Connection::assign(std::string const& fn, bool writeable)
		{
			Close();
			fileName = fn;
			writeable = writeable;
		}

		bool Connection::Open()
		{
			if (db) return false;
			int r = 0;
			if (writeable)
			{
				r = sqlite3_open_v2(fileName.c_str(), &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);
			}
			else
			{
				r = sqlite3_open_v2(fileName.c_str(), &db, SQLITE_OPEN_READONLY, nullptr);
			}
			if (r != SQLITE_OK)
			{
				Close();
				return false;
			}
			return true;
		}

		void Connection::Close()
		{
			if (!db) return;
			ReleaseQueries();
			sqlite3_close(db);
			db = nullptr;
		}


		Query * Connection::CreateQuery(std::string const& sql, int parameterCount /*= 0 */)
		{
			Query * rtv = nullptr;
			if (!db) return rtv;
			rtv = new Query(this, sql, parameterCount);
			queries.push_back(rtv);
			return rtv;
		}

		void Connection::ReleaseQuery(Query * q)
		{
			if (!q) return;
			delete q;
			auto it = find(queries.begin(), queries.end(), q);
			if (it != queries.end())
			{
				queries.erase(it);
			}
		}

		void Connection::ReleaseQueries()
		{
			for (auto& q : queries) delete q;
			queries.clear();
			qExists = nullptr;
		}





		void Connection::BeginTransaction()
		{
			Execute("BEGIN TRANSACTION");
		}

		void Connection::Commit()
		{
			Execute("COMMIT TRANSACTION");
		}

		void Connection::Rollback()
		{
			Execute("ROLLBACK TRANSACTION");
		}

		void Connection::EndTransaction()
		{
			Execute("END TRANSACTION");
		}

		bool Connection::Exists(std::string const& tn)
		{
			bool rtv = false;
			if (!qExists)
			{
				qExists = CreateQuery("SELECT count(*) FROM sqlite_master WHERE type = 'table' AND name = ?");
			}
			(*qExists).Add(tn);
			qExists->Execute([&](DataReader& dr)
			{
				rtv = dr.ReadInt32() > 0;
			});
			return rtv;
		}

		bool Connection::Execute(std::string const& sql)
		{
			if (!db) return false;
			return sqlite3_exec(db, sql.c_str(), nullptr, nullptr, nullptr) == SQLITE_OK;
		}




















		Query::Query()
			: conn(nullptr)
			, stmt(nullptr)
			, vc(0)
			, vi(0)
		{
		}

		Query::Query(Connection* conn, std::string const& sql, int parameterCount)
			: conn(conn)
			, stmt(nullptr)
			, vi(0)
		{
			Assign(sql, parameterCount);
		}

		Query::~Query()
		{
			if (stmt) sqlite3_finalize(stmt);
		}

		bool Query::Assign(std::string const& sql, int n)
		{
			if (stmt)
			{
				sqlite3_finalize(stmt);
				stmt = nullptr;
				vi = 0;
			}

			if (sqlite3_prepare_v2(conn->db, sql.c_str(), (int)sql.size(), &stmt, nullptr) != SQLITE_OK)
			{
				if (stmt)
				{
					sqlite3_finalize(stmt);
					stmt = nullptr;
				}
				return false;
			}

			if (n)
			{
				vc = n;
			}
			else
			{
				// 数 sql 串里有多少个 ?, 当前先这样实现.
				vc = 0;
				for (auto& c : sql)
				{
					if (c == '?') ++vc;
				}
			}

			return true;
		}

		bool Query::Execute(RowReaderType rr)
		{
			if (!stmt)
			{
				printf("Query::execute failed. _stmt = nullptr");
				return false;
			}
			if (vc != vi)
			{
				printf("Query::execute failed. parameter count is not valid. _vc = %d, _vi = %d", vc, vi);
				return false;
			}
			vi = 0;
			int ok = sqlite3_step(stmt);
			if (!(ok == SQLITE_OK || ok == SQLITE_DONE || ok == SQLITE_ROW))
			{
				printf("Query::execute sqlite3_step failed. code = %d, errmsg = %s", ok, sqlite3_errmsg(conn->db));
				return false;
			}
			if (rr)
			{
				DataReader dr(stmt);
				while (ok == SQLITE_ROW)
				{
					dr.Reset();
					rr(dr);
					ok = sqlite3_step(stmt);
				}
			}
			ok = sqlite3_reset(stmt);
			if (ok == SQLITE_OK || ok == SQLITE_DONE) return true;
			printf("Query::execute sqlite3_reset failed. code = %d", ok);
			return false;
		}

		bool Query::Add(int v)
		{
			return sqlite3_bind_int(stmt, ++vi, v) == SQLITE_OK;
		}

		bool Query::Add(long long v)
		{
			return sqlite3_bind_int64(stmt, ++vi, v) == SQLITE_OK;
		}

		bool Query::Add(double v)
		{
			return sqlite3_bind_double(stmt, ++vi, v) == SQLITE_OK;
		}

		bool Query::Add(std::string const& v)
		{
			return sqlite3_bind_text(stmt, ++vi, v.c_str(), (int)v.size(), SQLITE_STATIC) == SQLITE_OK;
		}

		bool Query::Add(char const* s)
		{
			if (s) return sqlite3_bind_text(stmt, ++vi, s, (int)strlen(s), SQLITE_STATIC) == SQLITE_OK;
			return sqlite3_bind_null(stmt, ++vi) == SQLITE_OK;
		}

		bool Query::Add(unsigned char const* buf, size_t len)
		{
			return sqlite3_bind_blob(stmt, ++vi, buf, (int)len, SQLITE_STATIC) == SQLITE_OK;
		}









		DataReader::DataReader(sqlite3_stmt* stmt)
			: stmt(stmt)
			, currentIndex(0)
		{
		}

		bool DataReader::ReadAt(int columnIndex, char const*& v)
		{
			if (sqlite3_column_type(stmt, columnIndex) == SQLITE_NULL) return false;
			v = (char const*)sqlite3_column_text(stmt, columnIndex);
			return true;
		}

		bool DataReader::ReadAt(int columnIndex, std::string& v)
		{
			if (sqlite3_column_type(stmt, columnIndex) == SQLITE_NULL) return false;
			v.assign((char const*)sqlite3_column_text(stmt, columnIndex));
			return true;
		}

		bool DataReader::ReadAt(int columnIndex, int& v)
		{
			if (sqlite3_column_type(stmt, columnIndex) == SQLITE_NULL) return false;
			v = sqlite3_column_int(stmt, columnIndex);
			return true;
		}

		bool DataReader::ReadAt(int columnIndex, long long& v)
		{
			if (sqlite3_column_type(stmt, columnIndex) == SQLITE_NULL) return false;
			v = sqlite3_column_int64(stmt, columnIndex);
			return true;
		}

		bool DataReader::ReadAt(int columnIndex, double& v)
		{
			if (sqlite3_column_type(stmt, columnIndex) == SQLITE_NULL) return false;
			v = sqlite3_column_double(stmt, columnIndex);
			return true;
		}

		char const* DataReader::ReadCString()
		{
			char const* v = nullptr;
			ReadAt(currentIndex++, v);
			return v;
		}

		std::string DataReader::ReadString()
		{
			std::string v;
			ReadAt(currentIndex++, v);
			return v;
		}

		int DataReader::ReadInt32()
		{
			int v = 0;
			ReadAt(currentIndex++, v);
			return v;
		}

		long long DataReader::ReadInt64()
		{
			long long v = 0;
			ReadAt(currentIndex++, v);
			return v;
		}

		double DataReader::ReadDouble()
		{
			double v = 0;
			ReadAt(currentIndex++, v);
			return v;
		}


		char const* DataReader::ReadCStringAt(int columnIndex)
		{
			char const* v = nullptr;
			ReadAt(columnIndex, v);
			return v;
		}

		std::string DataReader::ReadStringAt(int columnIndex)
		{
			std::string v;
			ReadAt(columnIndex, v);
			return v;
		}

		int DataReader::ReadInt32At(int columnIndex)
		{
			int v = 0;
			ReadAt(columnIndex, v);
			return v;
		}

		long long DataReader::ReadInt64At(int columnIndex)
		{
			long long v = 0;
			ReadAt(columnIndex, v);
			return v;
		}

		double DataReader::ReadDoubleAt(int columnIndex)
		{
			double v = 0;
			ReadAt(columnIndex, v);
			return v;
		}



		bool DataReader::Read(char const*& v)
		{
			return ReadAt(currentIndex++, v);
		}

		bool DataReader::Read(std::string & v)
		{
			return ReadAt(currentIndex++, v);
		}

		bool DataReader::Read(int& v)
		{
			return ReadAt(currentIndex++, v);
		}

		bool DataReader::Read(long long& v)
		{
			return ReadAt(currentIndex++, v);
		}

		bool DataReader::Read(double& v)
		{
			return ReadAt(currentIndex++, v);
		}

		void DataReader::Reset()
		{
			currentIndex = 0;
		}


	}
}
