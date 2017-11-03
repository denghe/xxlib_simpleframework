#include "Precompile.h"


FileIndex::FileIndex()
    : Connection()
    , _selectQuery( nullptr )
    , _updateQuery( nullptr )
    , _insertQuery( nullptr )
    , _deleteQuery( nullptr )
    , _setVerQuery( nullptr )
    , _getVerQuery( nullptr )
    , _ver( 0 )
{
}

FileIndex::FileIndex( string const & fileName )
    : Connection()
    , _selectQuery( nullptr )
    , _updateQuery( nullptr )
    , _insertQuery( nullptr )
    , _deleteQuery( nullptr )
    , _setVerQuery( nullptr )
    , _getVerQuery( nullptr )
    , _ver( 0 )
{
    setFileName( fileName );
}


FileIndex::~FileIndex()
{
    clear();
}

void FileIndex::setFileName( string const & fileName )
{
    _fileName = fileName;
    assign( _fileName );
}



void FileIndex::clearQueries()
{
    Sqlite::Connection::clearQueries();
    _selectQuery = nullptr;
    _updateQuery = nullptr;
    _insertQuery = nullptr;
    _deleteQuery = nullptr;
    _setVerQuery = nullptr;
    _getVerQuery = nullptr;
}

void FileIndex::clear()
{
    for( auto& row : _rows ) delete row.second;
    _rows.clear();
}


#define AUTO_OPEN_CLOSE_DB                                              \
    bool needOpen = ( _db == nullptr );                                 \
    if( needOpen ) if ( !open() ) return false;                         \
    Utils::ScopeGuard autoClose( [ = ] { if( needOpen ) close(); } );


bool FileIndex::create()
{
    AUTO_OPEN_CLOSE_DB;
    Sqlite::Table t( "fileindex" );

    t.addColumn( "name", Sqlite::Column::DataTypes::TEXT, true, true );
    t.addColumn( "type", Sqlite::Column::DataTypes::INTEGER );
    t.addColumn( "size", Sqlite::Column::DataTypes::INTEGER );
    t.addColumn( "version", Sqlite::Column::DataTypes::INTEGER );
    t.addColumn( "crc32", Sqlite::Column::DataTypes::INTEGER );
    t.addColumn( "md5", Sqlite::Column::DataTypes::TEXT );
    t.addColumn( "desc", Sqlite::Column::DataTypes::TEXT );
    t.addColumn( "state", Sqlite::Column::DataTypes::INTEGER );
    t.addColumn( "in_pkg", Sqlite::Column::DataTypes::INTEGER );
    t.addColumn( "noaa", Sqlite::Column::DataTypes::INTEGER );
    t.addColumn( "in_base_pkg", Sqlite::Column::DataTypes::INTEGER );
    if( !createTable( t ) ) return false;

    t.name = "string_int";
    t.clear();
    t.addColumn( "key", Sqlite::Column::DataTypes::TEXT, true, true );
    t.addColumn( "value", Sqlite::Column::DataTypes::INTEGER );
    if( !createTable( t ) ) return false;

    return true;
}

bool FileIndex::load()
{
    AUTO_OPEN_CLOSE_DB;
    clear();
    _ver = 0;
    if( !_selectQuery )
    {
        _selectQuery = newQuery( "SELECT name, type, size, version, crc32, md5, desc, state, in_pkg, noaa, in_base_pkg FROM fileindex" );
    }
    if( !_selectQuery->execute( [ this ]( Sqlite::DataReader& dr )
    {
        auto r = new FileIndexRow();
        r->name = dr.readString();
        r->type = (FileTypes)dr.readInt32();
        r->size = dr.readInt32();
        r->version = dr.readInt32();
        r->crc32 = dr.readInt32();
        r->md5 = dr.readString();
        r->desc = dr.readString();
        r->state = (FileStates)dr.readInt32();
        r->in_pkg = dr.readInt32() > 0;
        r->noaa = dr.readInt32() > 0;
        r->in_base_pkg = dr.readInt32() > 0;

        _rows.insert( make_pair( r->name, r ) );
    } ) ) return false;

    if( !_getVerQuery )
    {
        _getVerQuery = newQuery( "SELECT value FROM string_int WHERE key = 'fileindex_ver'" );
    }
    if( !_getVerQuery->execute( [ this ]( Sqlite::DataReader& dr )
    {
        _ver = dr.readInt32();
    } ) ) return false;
    return true;
}

bool FileIndex::insert( FileIndexRow* row )
{
    if( !dbReplace( row ) ) return false;
    _rows.insert( make_pair( row->name, row ) );
    return true;
}

bool FileIndex::dbReplace( FileIndexRow* row )
{
    if( !row )
    {
        CCLOG( "dbReplace row == nullptr" );
        return false;
    }
    AUTO_OPEN_CLOSE_DB;
    if( !_insertQuery )
    {
        _insertQuery = newQuery( "REPLACE INTO fileindex ( name, type, size, version, crc32, md5, desc, state, in_pkg, noaa, in_base_pkg ) VALUES ( ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ? )" );
    }
    auto &r = *row;
    ( *_insertQuery ) << r.name << r.type << r.size << r.version << r.crc32 << r.md5 << r.desc << r.state << ( r.in_pkg ? 1 : 0 ) << ( r.noaa ? 1 : 0 ) << ( r.in_base_pkg ? 1 : 0 );
    return _insertQuery->execute();
}

bool FileIndex::dbUpdateState( FileIndexRow* row )
{
    if( !row ) return false;
    AUTO_OPEN_CLOSE_DB;
    if( !_updateQuery )
    {
        _updateQuery = newQuery( "UPDATE fileindex set state = ? where name = ?" );
    }
    ( *_updateQuery ) << row->state << row->name;
    return _updateQuery->execute();
}

bool FileIndex::dbDelete( string const & name )
{
    if( !name.size() ) return false;
    AUTO_OPEN_CLOSE_DB;
    if( !_deleteQuery )
    {
        _deleteQuery = newQuery( "DELETE FROM fileindex WHERE name = ?" );
    }
    _deleteQuery->add( name );
    return _deleteQuery->execute();
}




// 下面的代码注释掉了 事务相关( 当前 android 下开启事务会报 error code 14 的错 )
bool FileIndex::upgrade( FileIndex const & other, bool inPkg, int ver )
{
    AUTO_OPEN_CLOSE_DB;
    //beginTransaction();
    //Utils::ScopeGuard sg( [ = ] { endTransaction(); } );

    // 根据文件列表去除老文件及数据项
    {
        vector<string> deleteNames;
        for( auto& it_thisRow : this->_rows )
        {
            auto& thisRow = it_thisRow.second;
            // 定位到相应行
            auto it_otherRow = other._rows.find( thisRow->name );
            if( it_otherRow == other._rows.end() )
            {
                // 如果找不到, 则表示需要删掉
                if( !thisRow->in_pkg )
                {
                    // 如果文件是下载的, 删
                    Utils::fileDelete( Global::WritablePath + thisRow->name );
                }
                // 放入待删队列
                deleteNames.push_back( thisRow->name );
            }
        }
        // 删记录
        for( auto& name : deleteNames )
        {
            _rows.erase( name );
            if( !dbDelete( name ) ) return false;
        }
    }

    // 开始更新更高版本的记录
    for( auto it_otherRow : other._rows )
    {
        auto& otherRow = it_otherRow.second;

        // 定位到本 db 的相应行( 这里不能直接 insert. 因为 otherRow 位于另一个容器 )
        auto it_thisRow = this->_rows.find( otherRow->name );

        if( it_thisRow != this->_rows.end() )
        {
            auto& thisRow = it_thisRow->second;

            // 如果有定位到, 继续对比版本号
            if( otherRow->version > thisRow->version )
            {
                // 版本号更高, 需要更新: 复制存盘, 试删残留
                
                // 试删残留( 如果之前的文件是下载的 )
                if( !thisRow->in_pkg )
                {
                    Utils::fileDelete( Global::WritablePath + thisRow->name );
                }

                // 复制
                otherRow->copyTo( thisRow );

                // 判断文件位置
                if( !inPkg )
                {
                    // web db: 中的新条目 始终需要下载, 文件不在包内
                    thisRow->state = FileStates::NeedDownload;
                    thisRow->in_pkg = false;
                }

                // 写库
                dbReplace( thisRow );
            }
            else
            {
                if( !thisRow->in_pkg && inPkg && otherRow->in_base_pkg )
                {
                    Utils::fileDelete( Global::WritablePath + thisRow->name );

                    // 复制
                    otherRow->copyTo( thisRow );

                    thisRow->in_pkg = true;
                    // 写库
                    dbReplace( thisRow );
                }            
            }
        }
        else
        {
            // 未定位到, 新增
            auto thisRow = new FileIndexRow();
            otherRow->copyTo( thisRow );
            if( inPkg )
            {
                thisRow->in_pkg = ( otherRow->state == FileStates::Finished );
            }
            else
            {
                thisRow->state = FileStates::NeedDownload;
                thisRow->in_pkg = false;
            }
            if( !insert( thisRow ) ) return false;
        }
    }

    if( !_setVerQuery )
    {
        _setVerQuery = newQuery( "REPLACE INTO string_int ( key, value ) VALUES ( 'fileindex_ver', ? )" );
    }
    _setVerQuery->add( ver );
    if( !_setVerQuery->execute() ) return false;

    //commit();

    _ver = ver;
    return true;
}




void FileIndex::fillFullName()
{
    for( auto& row : _rows )
    {
        row.second->fullName = ( row.second->in_pkg ? Global::ResourcePath : Global::WritablePath ) + row.second->name;
    }
}

FileIndexRow* FileIndex::getRow( string const & name )
{
    auto it = _rows.find( name );
    return it == _rows.end() ? nullptr : it->second;
}

vector<FileIndexRow*> FileIndex::getRows( vector<string> const & names )
{
    vector<FileIndexRow*> rtv;
    for( auto name : names )
    {
        if( auto fir = getRow( name ) )
        {
            rtv.push_back( fir );
        }
    }
    return rtv;
}

vector<string> FileIndex::getNames()
{
    vector<string> rtv;
    for( auto& o : _rows )
    {
        rtv.push_back( o.second->name );
    }
    return rtv;
}

int FileIndex::getVersion()
{
    return _ver;
}



