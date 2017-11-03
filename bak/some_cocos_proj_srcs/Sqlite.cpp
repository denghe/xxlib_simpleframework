#include "Precompile.h"

namespace Sqlite
{

    Connection::Connection()
        : _db( nullptr )
        , _writeable( false )
        , _existsQuery( nullptr )
    {
    }

    Connection::Connection( string const & fn, bool writeable )
        : _db( nullptr )
        , _fn( fn )
        , _writeable( writeable )
        , _existsQuery( nullptr )
    {
    }

    Connection::~Connection()
    {
        close();
    }

    void Connection::assign( string const & fn, bool writeable )
    {
        close();
        _fn = fn;
        _writeable = writeable;
    }

    bool Connection::open()
    {
        if( _db ) return false;
        int r = 0;
        if( _writeable )
        {
            r = sqlite3_open_v2( _fn.c_str(), &_db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr );
        }
        else
        {
            r = sqlite3_open_v2( _fn.c_str(), &_db, SQLITE_OPEN_READONLY, nullptr );
        }
        if( r != SQLITE_OK )
        {
            close();
            return false;
        }
        return true;
    }

    void Connection::close()
    {
        if( !_db ) return;
        clearQueries();
        sqlite3_close( _db );
        _db = nullptr;
    }


    Query * Connection::newQuery( string const & sql, int parameterCount /*= 0 */ )
    {
        Query * rtv = nullptr;
        if( !_db ) return rtv;
        rtv = new Query( this, sql, parameterCount );
        _qs.push_back( rtv );
        return rtv;
    }

    void Connection::deleteQuery( Query * q )
    {
        if( !q ) return;
        delete q;
        auto it = find( _qs.begin(), _qs.end(), q );
        if( it != _qs.end() )
        {
            _qs.erase( it );
        }
    }

    void Connection::clearQueries()
    {
        for( auto& q : _qs ) delete q;
        _qs.clear();
        _existsQuery = nullptr;
    }





    void Connection::beginTransaction()
    {
        execute( "BEGIN TRANSACTION" );
    }

    void Connection::commit()
    {
        execute( "COMMIT TRANSACTION" );
    }

    void Connection::rollback()
    {
        execute( "ROLLBACK TRANSACTION" );
    }

    void Connection::endTransaction()
    {
        execute( "END TRANSACTION" );
    }

    bool Connection::exists( string const & tn )
    {
        bool rtv = false;
        if( !_existsQuery )
        {
            _existsQuery = newQuery( "SELECT count(*) FROM sqlite_master WHERE type = 'table' AND name = ?" );
        }
        ( *_existsQuery ) << tn;
        _existsQuery->execute( [ &]( Sqlite::DataReader& dr )
        {
            rtv = dr.readInt32() > 0;
        } );
        return rtv;
    }

    bool Connection::execute( string const & sql )
    {
        if( !_db ) return false;
        return sqlite3_exec( _db, sql.c_str(), nullptr, nullptr, nullptr ) == SQLITE_OK;
    }


    bool Connection::createTable( Table const & t )
    {
        static char const * const dataTypeNames[] =
        {
            "INTEGER",
            "TEXT",
            "REAL",
            "BLOB",
        };
        char buf[ 16384 ], *p = buf;
        int aipk = -1;
        for( size_t i = 0; i < t._columns.size(); ++i )
        {
            auto& c = t._columns[ i ];
            if( c.dataType == Column::DataTypes::INTEGER && c.primaryKey )
            {
                aipk = i;
                break;
            }
        }
        if( aipk >= 0 )
        {
            /*
            CREATE TABLE "test" (
            "c1"  INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
            "c2"  TEXT NOT NULL
            );
            */
            p += Utils::fill( p, "CREATE TABLE \"", t.name, "\" (" );
            for( size_t i = 0; i < t._columns.size(); i++ )
            {
                auto& c = t._columns[ i ];
                if( i == aipk )
                    p += Utils::fill( p, "\r\n\"", c.name, "\" INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL", ( i < t._columns.size() - 1 ? ", " : "" ) );
                else
                    p += Utils::fill( p, "\r\n\"", c.name, "\" ", dataTypeNames[ (int)c.dataType ], ( c.nullable ? "" : " NOT NULL" ), ( i < t._columns.size() - 1 ? ", " : "" ) );
            }
            p += Utils::fill( p, "\r\n);" );
        }
        else
        {
            /*
            CREATE TABLE "xxx" (
            "name" TEXT(200) NOT NULL,
            "type" INTEGER NOT NULL,
            PRIMARY KEY (
            "name" ASC, .... 当前未生成 ASC
            )
            );
            */
            p += Utils::fill( p, "CREATE TABLE \"", t.name, "\" (" );
            vector<Column const *> pks;
            for( auto& c : t._columns )
            {
                p += Utils::fill( p, "\r\n\"", c.name + "\" ", dataTypeNames[ (int)c.dataType ], ( c.nullable ? "" : " NOT NULL" ), ", " );
                if( c.primaryKey ) pks.push_back( &c );
            }

            if( pks.size() )
            {
                p += Utils::fill( p, "\r\nPRIMARY KEY( " );
                for( size_t i = 0; i < pks.size(); i++ )
                {
                    auto& c = *pks[ i ];
                    p += Utils::fill( p, "\r\n\"", c.name, "\"", ( i < pks.size() - 1 ? ", " : "" ) );
                }
                p += Utils::fill( p, "\r\n)" );
            }
            p += Utils::fill( p, "\r\n)" );
        }
        *p = '\0';

        return sqlite3_exec( _db, buf, nullptr, nullptr, nullptr ) == SQLITE_OK;
    }

    bool Connection::dropTable( string const & tn )
    {
        char buf[ 512 ], *p = buf;
        p += Utils::fill( p, "DROP TABLE IF EXISTS `", tn, "`" );       // maybe need escape here
        *p = '\0';
        return sqlite3_exec( _db, buf, nullptr, nullptr, nullptr ) == SQLITE_OK;
    }



















    Query::Query()
        : _conn( nullptr )
        , _stmt( nullptr )
        , _vc( 0 )
        , _vi( 0 )
    {
    }

    Query::Query( Connection * conn, string const & sql, int parameterCount )
        : _conn( conn )
        , _stmt( nullptr )
        , _vi( 0 )
    {
        assign( sql, parameterCount );
    }

    Query::~Query()
    {
        if( _stmt ) sqlite3_finalize( _stmt );
    }

    bool Query::assign( string const & sql, int n )
    {
        _sql = sql;

        if( _stmt )
        {
            sqlite3_finalize( _stmt );
            _stmt = nullptr;
            _vi = 0;
        }

        if( sqlite3_prepare_v2( _conn->_db, sql.c_str(), sql.size(), &_stmt, nullptr ) != SQLITE_OK )
        {
            if( _stmt )
            {
                sqlite3_finalize( _stmt );
                _stmt = nullptr;
            }
            return false;
        }

        if( n )
        {
            _vc = n;
        }
        else
        {
            // 数 sql 串里有多少个 ?, 当前先这样实现.
            _vc = 0;
            for( auto& c : _sql )
            {
                if( c == '?' ) ++_vc;
            }
        }

        return true;
    }

    bool Query::execute( RowReaderType rr )
    {
        if( !_stmt )
        {
            CCLOG( "Query::execute failed. _stmt = nullptr" );
            return false;
        }
        if( _vc != _vi )
        {
            CCLOG( "Query::execute failed. parameter count is not valid. _vc = %d, _vi = %d", _vc, _vi );
            return false;
        }
        _vi = 0;
        int ok = sqlite3_step( _stmt );
        if( !( ok == SQLITE_OK || ok == SQLITE_DONE || ok == SQLITE_ROW ) )
        {
            CCLOG( "Query::execute sqlite3_step failed. code = %d, errmsg = %s", ok, sqlite3_errmsg( _conn->_db ) );
            return false;
        }
        if( rr )
        {
            DataReader dr( _stmt );
            while( ok == SQLITE_ROW )
            {
                dr.reset();
                rr( dr );
                ok = sqlite3_step( _stmt );
            }
        }
        ok = sqlite3_reset( _stmt );
        if( ok == SQLITE_OK || ok == SQLITE_DONE ) return true;
        CCLOG( "Query::execute sqlite3_reset failed. code = %d", ok );
        return false;
    }

    bool Query::add( int v )
    {
        return sqlite3_bind_int( _stmt, ++_vi, v ) == SQLITE_OK;
    }

    bool Query::add( int64 v )
    {
        return sqlite3_bind_int64( _stmt, ++_vi, v ) == SQLITE_OK;
    }

    bool Query::add( double v )
    {
        return sqlite3_bind_double( _stmt, ++_vi, v ) == SQLITE_OK;
    }

    bool Query::add( string const & v )
    {
        return sqlite3_bind_text( _stmt, ++_vi, v.c_str(), v.size(), SQLITE_STATIC ) == SQLITE_OK;
    }

    bool Query::add( char const * s )
    {
        if( s ) return sqlite3_bind_text( _stmt, ++_vi, s, strlen( s ), SQLITE_STATIC ) == SQLITE_OK;
        return sqlite3_bind_null( _stmt, ++_vi ) == SQLITE_OK;
    }

    bool Query::add( byte const * buf, size_t len )
    {
        return sqlite3_bind_blob( _stmt, ++_vi, buf, len, SQLITE_STATIC ) == SQLITE_OK;
    }











    Table::Table( string const & name )
    {
        this->name = name;
    }

    void Table::addColumn( string const & name, Column::DataTypes dataType, bool primaryKey /*= false*/, bool autoIncrease /*= false*/, bool nullable /*= false */ )
    {
        Column c = { name, dataType, primaryKey, autoIncrease, nullable };
        _columns.push_back( c );
    }

    void Table::clear()
    {
        _columns.clear();
    }








    DataReader::DataReader( sqlite3_stmt * stmt )
        : _stmt( stmt )
        , _index( 0 )
    {
    }

    bool DataReader::readAt( int columnIndex, char const * & v )
    {
        if( sqlite3_column_type( _stmt, columnIndex ) == SQLITE_NULL ) return false;
        v = (char const *)sqlite3_column_text( _stmt, columnIndex );
        return true;
    }

    bool DataReader::readAt( int columnIndex, string & v )
    {
        if( sqlite3_column_type( _stmt, columnIndex ) == SQLITE_NULL ) return false;
        v.assign( (char const *)sqlite3_column_text( _stmt, columnIndex ) );
        return true;
    }

    bool DataReader::readAt( int columnIndex, int & v )
    {
        if( sqlite3_column_type( _stmt, columnIndex ) == SQLITE_NULL ) return false;
        v = sqlite3_column_int( _stmt, columnIndex );
        return true;
    }

    bool DataReader::readAt( int columnIndex, int64 & v )
    {
        if( sqlite3_column_type( _stmt, columnIndex ) == SQLITE_NULL ) return false;
        v = sqlite3_column_int64( _stmt, columnIndex );
        return true;
    }

    bool DataReader::readAt( int columnIndex, double & v )
    {
        if( sqlite3_column_type( _stmt, columnIndex ) == SQLITE_NULL ) return false;
        v = sqlite3_column_double( _stmt, columnIndex );
        return true;
    }

    char const * DataReader::readCString()
    {
        char const * v = nullptr;
        readAt( _index++, v );
        return v;
    }

    std::string DataReader::readString()
    {
        string v;
        readAt( _index++, v );
        return v;
    }

    int DataReader::readInt32()
    {
        int v = 0;
        readAt( _index++, v );
        return v;
    }

    int64 DataReader::readInt64()
    {
        int64 v = 0;
        readAt( _index++, v );
        return v;
    }

    double DataReader::readDouble()
    {
        double v = 0;
        readAt( _index++, v );
        return v;
    }


    char const * DataReader::readCStringAt( int columnIndex )
    {
        char const * v = nullptr;
        readAt( columnIndex, v );
        return v;
    }

    std::string DataReader::readStringAt( int columnIndex )
    {
        string v;
        readAt( columnIndex, v );
        return v;
    }

    int DataReader::readInt32At( int columnIndex )
    {
        int v = 0;
        readAt( columnIndex, v );
        return v;
    }

    int64 DataReader::readInt64At( int columnIndex )
    {
        int64 v = 0;
        readAt( columnIndex, v );
        return v;
    }

    double DataReader::readDoubleAt( int columnIndex )
    {
        double v = 0;
        readAt( columnIndex, v );
        return v;
    }



    bool DataReader::read( char const * & v )
    {
        return readAt( _index++, v );
    }

    bool DataReader::read( string & v )
    {
        return readAt( _index++, v );
    }

    bool DataReader::read( int & v )
    {
        return readAt( _index++, v );
    }

    bool DataReader::read( int64 & v )
    {
        return readAt( _index++, v );
    }

    bool DataReader::read( double & v )
    {
        return readAt( _index++, v );
    }

    void DataReader::reset()
    {
        _index = 0;
    }





















}














/*
Parameter::Parameter()
: _vt( DataType::Unknown )
, _v64( 0 )
{
}

Parameter::Parameter( int v )
: _vt( DataType::Int32 )
, _v64( 0 )
{
_v32 = v;
}

Parameter::Parameter( int64 v )
: _vt( DataType::Int64 )
, _v64( v )
{
}

Parameter::Parameter( double v )
: _vt( DataType::Double )
, _vd( v )
{
}

Parameter::Parameter( string const & v )
: _vt( DataType::String )
, _vs( v )
{
}

Parameter::Parameter( Parameter const & other )
: _vt( other._vt )
, _vd( other._vd )
{
if( _vt == DataType::String ) _vs = other._vs;
else _vs.clear();
}

Parameter::Parameter( Parameter && other )
: _vt( other._vt )
, _vd( other._vd )
{
if( _vt == DataType::String ) _vs = move( other._vs );
else _vs.clear();
}

Parameter & Parameter::operator=( Parameter const & other )
{
_vt = other._vt;
_vd = other._vd;
if( _vt == DataType::String ) _vs = other._vs;
else _vs.clear();
return *this;
}

Parameter & Parameter::operator=( Parameter && other )
{
_vt = other._vt;
_vd = other._vd;
if( _vt == DataType::String ) _vs = move( other._vs );
else _vs.clear();
return *this;
}

void Parameter::assign( int v )
{
_vt = DataType::Int32;
_v64 = 0;
_v32 = v;
_vs.clear();
}

void Parameter::assign( int64 v )
{
_vt = DataType::Int32;
_v64 = v;
_vs.clear();
}

void Parameter::assign( double v )
{
_vt = DataType::Int32;
_vd = v;
_vs.clear();
}

void Parameter::assign( string const & v )
{
_vt = DataType::Int32;
_v64 = 0;
_vs = v;
}






void Parameters::addParameter( int v )
{
this->push_back( Parameter( v ) );
}

void Parameters::addParameter( int64 v )
{
this->push_back( Parameter( v ) );
}

void Parameters::addParameter( double v )
{
this->push_back( Parameter( v ) );
}

void Parameters::addParameter( string const & v )
{
this->push_back( Parameter( v ) );
}

void Parameters::addParameter( char const * s )
{
this->push_back( Parameter( s ) );
}

*/
