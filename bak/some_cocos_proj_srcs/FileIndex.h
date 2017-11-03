#ifndef __FILEINDEX_H__
#define __FILEINDEX_H__

// 文件下载器事件体
typedef GenericEvent<FileEventTypes> FileEvent;

class FileIndex;
class FileManager;

// 文件索引项( db 于内存中的映射 )
class FileIndexRow
{
public:
    string              name;                   // 路径 + 文件名( 前面不带/ )
    FileTypes           type;                   // 文件类型
    int                 size;                   // 文件字节长度
    int                 version;                // 版本号( 更新依据 )
    int                 crc32;                  // 文件内容的 crc32
    string              md5;                    // 文件内容的 md5
    string              desc;                   // 文件附加说明
    FileStates          state;                  // 当前文件状态( 只向 db 存入 NeedDownload, Finished. 其他中间态存内存, 方便回滚 )
    bool                in_pkg;                 // 文件是否存在于包内( true: 存在.     false: 位于可写区 )
    bool                noaa;                   // 是否不抗锯齿
    bool                in_base_pkg;            // 文件是否包含在基本包

    string              fullName;               // 用于存放完整路径( 该字段并不存在于数据库中, 而是加载后再填充 )

    inline void copyTo( FileIndexRow * other )
    {
        other->name = this->name;
        other->type = this->type;
        other->size = this->size;
        other->version = this->version;
        other->crc32 = this->crc32;
        other->md5 = this->md5;
        other->desc = this->desc;
        other->state = this->state;
        other->in_pkg = this->in_pkg;
        other->noaa = this->noaa;
        other->in_base_pkg = this->in_base_pkg;
        other->fullName = this->fullName;
    }

    ~FileIndexRow()
    {
    }

    // for lua
    inline int getVersion() { return version; };
    inline FileStates getState() { return state; };
    inline FileTypes getType() { return type; };
    inline int getSize() { return size; };
    inline string getFullName() { return fullName; };
    inline string getName() { return name; };
    inline bool getInBasePkg() { return in_base_pkg; };
};

// 文件索引( 索引文件 当前采用 sqlite3 db 文件格式, 运行时文件存在于游戏 可写区域根, 游戏启动时和 包内, 网上 的db 做 update 操作 )
// 初始流程: 要先 setRootPath, 再 setFileName
class FileIndex : public Sqlite::Connection
{
public:
    // 默认啥都不做, 接下来须调用 setFileName 来设置文件名
    FileIndex();

    // 设当前文件名( 全路径 )
    FileIndex( string const & fileName );

    // 关闭数据库, 回收 _rows 的内存
    ~FileIndex();

    // 设当前文件名( 全路径 )
    void setFileName( string const & fileName );

    // 在执行原有 clearQueries 的同时清一些指针
    virtual void clearQueries() override;

    // 清 _rows
    void clear();


    // 下面 几 个数据库操作函数, 如果 db 未打开, 则打开 db. 如果是函数自己打开的, 退出时将关闭 db
    bool create();                          // 创建一个空表( 通常发生于程序初次安装或清数据 )
    bool load();                            // clear() 并 select * 填充 _rows
    bool dbReplace( FileIndexRow* row );    // row 写库( replace )
    bool dbUpdateState( FileIndexRow* row );// row 写库( 只 update row 的 state )
    bool dbDelete( string const & name );   // 从 db 中 删掉一行 row 根据 name

    // 向 _rows 和 表 同步插入一行数据( 批量可 open, beginTransaction, add, commit, close  )
    bool insert( FileIndexRow* row );

    // 传入另一份索引, foreach row 按 路径+文件名 来匹配 并比对版本号. 另一份 row 的版本号更大则更新 row 信息到另一份. 
    // 更新前提条件是, 另一份索引的 最大版本号 须大于 本索引的最大版本号
    // inPkg: 该 db 是否位于安装包内 或是从安装包内复制出来的, 这关系到更新规则. false: 从网上下的
    // 注: 当前并不支持删掉多余的 row
    bool upgrade( FileIndex const & other, bool inPkg, int ver );

    // foreach _rows 填充 fullName 字段以方便程序执行过程中使用. 通常出现于 upgrade 步骤完毕之后
    void fillFullName();

    // 通过 文件名 从 _rows 检索 一行数据
    FileIndexRow* getRow( string const & name );

    // 通过 文件名列表 从 _rows 检索 多行数据( 不生成到 LUA )
    vector<FileIndexRow*> getRows( vector<string> const & names );

    // 返回所有文件名( 方便在 lua 中遍历 )
    vector<string> getNames();                  

    // 返回文件批号
    int getVersion();

    friend FileManager;
private:

    string _fileName;                                   // 存盘文件名( 全路径 ). 通过 setFileName 传入
    unordered_map<string, FileIndexRow*> _rows;         // 内存数据( key 为 name )

    Sqlite::Query * _selectQuery;                       // SELECT name, type, size, version, crc32, md5, desc, state FROM fileindex
    Sqlite::Query * _updateQuery;                       // UPDATE fileindex set state = ? where name = ?
    Sqlite::Query * _insertQuery;                       // REPLACE INTO fileindex ( name, type, size, version, crc32, md5, desc, state ) VALUES ( ?, ?, ?, ?, ?, ?, ?, ? )
    Sqlite::Query * _deleteQuery;                       // DELETE FROM fileindex WHERE name = ?

    Sqlite::Query * _setVerQuery;                       // REPLACE INTO string_int ( key, value ) VALUES ( 'fileindex_ver', ? )
    Sqlite::Query * _getVerQuery;                       // SELECT value FROM string_int WHERE key = 'fileindex_ver'

    int _ver;                                           // 当前数据的版本号( 与 string_int 表的 key: ver 的 value 值对应. upgrade 将刷新该值 )

    // = delete
    FileIndex( const FileIndex & );
    FileIndex & operator=( const FileIndex & );
};



#endif

