#pragma once

#include "sqlite3.h"
#include <string>
#include <vector>
#include <functional>

namespace xx
{
	namespace sqlite
	{
		static char const * const NullValue = nullptr;

		struct Query;
		struct Table;

		struct Connection
		{
			// zero vars
			Connection();

			// call assign
			Connection(std::string const & fn, bool writeable = true);

			// call close
			virtual ~Connection();

			// close & set db file full path
			void assign(std::string const & fn, bool writeable = true);

			// open db, success return true
			bool Open();

			// clear stmts, close db
			void Close();

			// new stmt		n: 参数个数, 不传将扫 ? 号个数
			Query* CreateQuery(std::string const & sql, int n = 0);

			// delete stmt
			void ReleaseQuery(Query * q);

			// delete all stmts
			virtual void ReleaseQueries();

			// execute BEGIN TRANSACTION
			void BeginTransaction();

			// execute COMMIT TRANSACTION
			void Commit();

			// execute ROLLBACK TRANSACTION
			void Rollback();

			// execute END TRANSACTION
			void EndTransaction();

			// execute SELECT count(*) FROM sqlite_master WHERE type = 'table' AND name = ?
			bool Exists(std::string const & tn);

			// simple exec
			bool Execute(std::string const & sql);


			friend Query;
		protected:
			sqlite3*            db;
			std::string         fileName;
			bool                writeable;
			std::vector<Query*> queries;
			Query*              qExists;

			Connection(Connection const& other) = delete;
			Connection & operator=(Connection const& other) = delete;
		};

		class DataReader
		{
		public:
			DataReader(sqlite3_stmt* stmt);

			// fill mode, if value is null, return false
			bool ReadAt(int columnIndex, char const*& v);
			bool ReadAt(int columnIndex, std::string& v);
			bool ReadAt(int columnIndex, int& v);
			bool ReadAt(int columnIndex, long long& v);
			bool ReadAt(int columnIndex, double& v);

			// return mode
			char const* ReadCStringAt(int columnIndex);
			std::string ReadStringAt(int columnIndex);
			int ReadInt32At(int columnIndex);
			long long ReadInt64At(int columnIndex);
			double ReadDoubleAt(int columnIndex);

			// index++
			bool Read(char const*& v);
			bool Read(std::string& v);
			bool Read(int& v);
			bool Read(long long& v);
			bool Read(double& v);

			char const* ReadCString();
			std::string ReadString();
			int ReadInt32();
			long long ReadInt64();
			double ReadDouble();
			void Reset();

		private:
			sqlite3_stmt* stmt;
			int currentIndex;
		};

		struct Query
		{
			typedef std::function<void(DataReader& dr)> RowReaderType;

			/*
			execute sql, read output data. only support single sql segment
			sample:

			auto q = conn.newQuery( "REPLACE INTO test1 ( `name`, `desc` ) VALUES ( ?, ? )" );
			q->Add( p1 );
			q->Add( p2 );
			q->execute();

			Foo o;
			q->assign( "SELECT * from test1" );
			q->execute( []( Sqlite::DataReader& dr )
			{
			dr >> o.xxx >> o.eee >> o.fff ...
			} );
			*/
			bool Execute(RowReaderType rr = nullptr);

			// reset sql query
			bool Assign(std::string const & sql, int parameterCount = 0);

			// push parameter values
			//

			bool Add(int v);
			bool Add(long long v);
			bool Add(double v);
			bool Add(std::string const& v);
			bool Add(char const* v);                 // nullptr = NULL
			bool Add(unsigned char const* buf, size_t len);
			template<typename T>
			bool Add(T const& v)
			{
				static_assert(std::is_enum<T>::value, "must be enum type");
				if (sizeof(T) <= 4) return Add((int)v);
				else return Add((long long)v);
			}

			friend Connection;
		private:
			Query();
			Query(Connection* conn, std::string const& sql, int parameterCount = 0);
			~Query();

			Connection*         conn;
			sqlite3_stmt*       stmt;
			int                 vc;    // value count
			int                 vi;    // value index

			Query(Query const & other) = delete;
			Query & operator=(Query const & other) = delete;
		};


	}

}
