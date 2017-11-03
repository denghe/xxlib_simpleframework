#ifndef __SQLITE___H__
#define __SQLITE___H__

namespace Sqlite
{
    static char const * const NullValue = nullptr;

    struct Query;
    struct Table;

    struct Connection
    {
        // zero vars
        Connection();

        // call assign
        Connection( string const & fn, bool writeable = true );

        // call close
        virtual ~Connection();

        // close & set db file full path
        void assign( string const & fn, bool writeable = true );

        // open db, success return true
        bool open();

        // clear stmts, close db
        void close();

        // create stmt
        Query * newQuery( string const & sql, int n = 0 );  // 可传入参数个数

        // delete stmt
        void deleteQuery( Query * q );

        // clear stmts
        virtual void clearQueries();

        // execute BEGIN TRANSACTION
        void beginTransaction();

        // execute COMMIT TRANSACTION
        void commit();

        // execute ROLLBACK TRANSACTION
        void rollback();

        // execute END TRANSACTION
        void endTransaction();

        // execute SELECT count(*) FROM sqlite_master WHERE type = 'table' AND name = ?
        bool exists( string const & tn );

        // simple exec
        bool execute( string const & sql );

        // execute CREATE TABLE ( ......... )
        bool createTable( Table const & t );

        // execute DROP TABLE IF EXISTS ?
        bool dropTable( string const & tn );

        friend Query;
    protected:
        sqlite3 *           _db;
        string              _fn;
        bool                _writeable;
        vector<Query*>      _qs;
        Query*              _existsQuery;

        // = delete
        Connection( Connection const & other );
        Connection & operator=( Connection const & other );
    };

    class DataReader
    {
    public:
        DataReader( sqlite3_stmt * stmt );

        // fill mode, if value is null, return false
        bool readAt( int columnIndex, char const * & v );
        bool readAt( int columnIndex, string & v );
        bool readAt( int columnIndex, int & v );
        bool readAt( int columnIndex, int64 & v );
        bool readAt( int columnIndex, double & v );

        // return mode
        char const * readCStringAt( int columnIndex );
        string readStringAt( int columnIndex );
        int readInt32At( int columnIndex );
        int64 readInt64At( int columnIndex );
        double readDoubleAt( int columnIndex );

        // _index++
        bool read( char const * & v );
        bool read( string & v );
        bool read( int & v );
        bool read( int64 & v );
        bool read( double & v );

        char const * readCString();
        string readString();
        int readInt32();
        int64 readInt64();
        double readDouble();
        void reset();

    private:
        sqlite3_stmt * _stmt;
        int _index;
    };

    struct Query
    {
        typedef function<void( DataReader& dr )> RowReaderType;

        /*
        execute sql, read output data. only support single sql segment
        sample:

        auto q = conn.newQuery( "REPLACE INTO test1 ( `name`, `desc` ) VALUES ( ?, ? )" );
        (*q) << ... << ...;
        q->execute();

        Foo o;
        q->assign( "SELECT * from test1" );
        q->execute( []( Sqlite::DataReader& dr )
        {
            dr >> o.xxx >> o.eee >> o.fff ...
        } );
        */
        bool execute( RowReaderType rr = nullptr );

        // reset sql query
        bool assign( string const & sql, int parameterCount = 0 );

        // push parameter values
        //

        bool add( int v );
        bool add( int64 v );
        bool add( double v );
        bool add( string const & v );
        bool add( char const * v );                 // nullptr = NULL
        bool add( byte const * buf, size_t len );
        template<typename T>
        void add( T const & v )
        {
            static_assert( std::is_enum<T>::value, "must be enum type" );
            if( sizeof( T ) <= 4 ) add( (int)v );
            else add( (int64)v );
        }
        template<typename T>
        Query & operator << ( T const & v ) { add( v ); return *this; }


        friend Connection;
    private:
        Query();
        Query( Connection * conn, string const & sql, int parameterCount = 0 );
        ~Query();

        Connection *        _conn;
        sqlite3_stmt *      _stmt;
        string              _sql;
        int                 _vc;    // value count
        int                 _vi;    // value index

        // = delete
        Query( Query const & other );
        Query & operator=( Query const & other );
    };








    // 下面是 结构描述类, 主用于建表

    struct Column
    {
        // 数据类型来自 Navicat 软件
        enum class DataTypes
        {
            // 整数
            INTEGER,
            // 文本
            TEXT,
            // 浮点
            REAL,
            // 原样照存( byte[] 之类 )
            BLOB,
        };
        // 数据库中的字段名
        string name;
        DataTypes dataType;
        bool primaryKey;
        bool autoIncrease;
        bool nullable;
    };

    struct Table
    {
        Table( string const & name );
        void addColumn( string const & name, Column::DataTypes dataType, bool primaryKey = false, bool autoIncrease = false, bool nullable = false );
        string name;
        void clear();

        friend Connection;
    private:
        vector<Column> _columns;
    };

}

#endif








//// sqlite3 stmt 参数( 当前只支持 int, int64, double, string )
//struct Parameter
//{
//    enum class DataType
//    {
//        Unknown,
//        Int32,
//        Int64,
//        Double,
//        String
//    };

//    Parameter();
//    Parameter( int v );
//    Parameter( int64 v );
//    Parameter( double v );
//    Parameter( string const & v );

//    void assign( int v );
//    void assign( int64 v );
//    void assign( double v );
//    void assign( string const & v );

//    Parameter( Parameter const & other );
//    Parameter( Parameter && other );
//    Parameter & operator=( Parameter const & other );
//    Parameter & operator=( Parameter && other );

//    friend Query;
//private:
//    DataType            _vt;
//    union
//    {
//        int             _v32;
//        int64           _v64;
//        double          _vd;
//    };
//    string              _vs;
//};

//struct Parameters : public vector<Parameter>
//{
//    void addParameter( int v );
//    void addParameter( int64 v );
//    void addParameter( double v );
//    void addParameter( string const & v );
//    void addParameter( char const * s );
//    //template<size_t len>
//    //void addParameter( char const ( &v )[ len ] )
//    //{
//    //    this->push_back( Parameter( string( v, len - 1 ) ) );
//    //}

//    template<typename T>
//    void addParameter( T const & v )
//    {
//        static_assert( std::is_enum<T>::value, "must be enum type" );
//        if( sizeof( T ) <= 4 ) addParameter( (int)v );
//        else addParameter( (int64)v );
//    }

//    template<typename T>
//    Parameters & operator << ( T const & v ) { addParameter( v ); return *this; }
//};
