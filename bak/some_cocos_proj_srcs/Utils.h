#ifndef _MY_UTILS_H_
#define _MY_UTILS_H_

namespace Utils
{

    // 文件复制( 都传绝对路径, 不会帮建目录. 也不会 check 文件是否存在. 直接创建或覆盖 )
    bool fileCopy( string const & src, string const & dest );

    // 检查文件是否存在
    bool fileExists( string const & fn );

    // 删文件, 成功返回 true
    bool fileDelete( string const & fn );

    // 文件改名 或 移动
    bool fileRename( string const & src, string const & dest );

    // 向文件写入 buf, 成功返回 true
    bool fileWriteBytes( string const & fn, char const * buf, int len );

    // 取文件尺寸( 传入一个未经打开的文件名 )
    int fileMoveSeekEnd( string const & fn );

    // 移动文件指针到最尾部 并返回文件尺寸 ( 传入一个已经打开的文件句柄 )
    int fileMoveSeekEnd( FILE* f );

    // 返回文件尺寸 ( 传入一个已经打开的文件句柄 )
    int fileGetSize( FILE* f );

    // 切割文件尺寸为指定值( 传入已打开的文件句柄 ). 主用于下载出错时将最后一段无效数据去掉
    bool fileCut( FILE* f, uint size );

    // 简单封装 FileUtils 的功能函数
    string fileGetFullPath( string const & fn );

    // path: 切掉文件名, 返回目录部分( 后面带 / )
    string filePathCutFileName( string const & fn );

    // 简单封装取文件数据的函数
    Data fileGetData( string const & fn );

    // 简单封装取文本文件内容的函数
    string fileGetText( string const & fn );

    // 对于 IOS 平台来讲, 针对目录或文件, 设置其 "不自动备份到 iCloud" 的属性
    void fileSkipBackup( string const & path );

    // 从 plist 字典中 取得材质图文件名
    string getTextureFileNameFromPlistDict( ValueMap const & plistDict );

    //// 返回位于 atlas 文件数据中的 图片文件名列表
    //vector<string> getFileNamesByAtlas( string const & data );

    // 从 plist 文件中 取得材质图文件名
    string getTextureFileNameFromPlistFile( string const & fn );

    // 字串之简单替换( 直接改源串 )
    void simpleReplace( string &s, char src, char dest );

    // 字串之简单替换( 返回替换之后的串 )
    string simpleReplace( string &s, char src, string const & dest );

    // 建目录( 不会一级一级补齐 ). 成功 或 已存在 均返回 true
    bool createDirectory( char const * tarPath );

    // 确保 目标目录 一定存在( 如果没有, 会一级一级补齐 )
    // 参数:
    //      fullPath: 位于 "可写区域" 的 "目录全路径"( 重要: 不能含有文件名 )
    bool ensureDirectory( string fullPath );

    // 简单的算 hash 的函数
    uint getHashCode( char const * buf, int len );

    // 简单的切割字串
    vector<string> split( string const & s, string const & delim );

    // 判断是否存在  位于 scrollview, clipping region node 中的父节点
    // 并判断 父节点可见性,  当前点击区是否位于　其显示区域, 如果是 btn 套 btn , 其是否 enable
    bool ensureTouch( Node * btn, Touch * touch );

    // 判断是否点击在内部区域
    bool isTouchInside( Node * o, Touch * touch );




    // 利用析构, 于出 {} 范围时自动执行 lambda
    class ScopeGuard
    {
    public:
        typedef function<void()> FT;

        // 保存 f
        ScopeGuard( FT f );

        // 执行 f
        ~ScopeGuard();

        // 执行 f, 令析构时不再执行 f
        void runAndCancel();

        // 执行 f
        void run();

        // 令析构时不再执行 f
        void cancel();

        // 保存 f
        void set( FT f );

    private:
        FT _f;
        ScopeGuard( const ScopeGuard & );             // delete
        ScopeGuard &operator=( const ScopeGuard & );  // delete
    };





    /************************************************************************/
    /* 下面是 fill string buffer 相关. 危险 须慎用                          */
    /************************************************************************/

    // 将 原生数据类型 转为字串 写入 buf, 返回填充长度
    int fillCore( char * buf, char v );
    int fillCore( char * buf, int v );
    int fillCore( char * buf, uint v );
    int fillCore( char * buf, float v );
    int fillCore( char * buf, double v );
    int fillCore( char * buf, string const & s );
    template<int len>
    int fillCore( char * buf, char const ( &s )[ len ] )
    {
        memcpy( buf, s, len - 1 );
        return len - 1;
    }

    // 将 wchar（只支持到 ucs2）转为 "\uXXXX" 的 16进制 字串 形态 写入 buf
    void fillU16( char * buf, ushort c );

    // 将 s: utf8 只支持到 ucs2）串转义为 json 字串填进 buf ( " -> \"  ,  宽字符-> \uXXXX ), 返回转换后的长
    int fillToJson( char * buf, int bufLen, char const * s, int sLen );

    // 将 1个 utf8 格式的字符 转为 1 个 wchar 填充到 buf. 返回填充长度
    int fillWChar( ushort * buf, char const * s );

    // 将 1 个 wchar 转为 utf8 填充到 buf, 返回填充长度
    int fillUtf8( char * buf, ushort c );

    // 向 buf 连续填充多个 参数 toString 串 并返回一共填充了多少字节
    template<typename T0>
    int fill( char * buf, T0 const & v0 )
    {
        int offset = fillCore( buf, v0 );
        buf[ offset ] = '\0';
        return offset;
    }
    template<typename T0, typename T1>
    int fill( char * buf, T0 const & v0, T1 const & v1 )
    {
        int offset = fillCore( buf, v0 );
        offset += fillCore( buf + offset, v1 );
        buf[ offset ] = '\0';
        return offset;
    }
    template<typename T0, typename T1, typename T2>
    int fill( char * buf, T0 const & v0, T1 const & v1, T2 const & v2 )
    {
        int offset = fillCore( buf, v0 );
        offset += fillCore( buf + offset, v1 );
        offset += fillCore( buf + offset, v2 );
        buf[ offset ] = '\0';
        return offset;
    }
    template<typename T0, typename T1, typename T2, typename T3>
    int fill( char * buf, T0 const & v0, T1 const & v1, T2 const & v2, T3 const & v3 )
    {
        int offset = fillCore( buf, v0 );
        offset += fillCore( buf + offset, v1 );
        offset += fillCore( buf + offset, v2 );
        offset += fillCore( buf + offset, v3 );
        buf[ offset ] = '\0';
        return offset;
    }
    template<typename T0, typename T1, typename T2, typename T3, typename T4>
    int fill( char * buf, T0 const & v0, T1 const & v1, T2 const & v2, T3 const & v3, T4 const & v4 )
    {
        int offset = fillCore( buf, v0 );
        offset += fillCore( buf + offset, v1 );
        offset += fillCore( buf + offset, v2 );
        offset += fillCore( buf + offset, v3 );
        offset += fillCore( buf + offset, v4 );
        buf[ offset ] = '\0';
        return offset;
    }
    template<typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
    int fill( char * buf, T0 const & v0, T1 const & v1, T2 const & v2, T3 const & v3, T4 const & v4, T5 const & v5 )
    {
        int offset = fillCore( buf, v0 );
        offset += fillCore( buf + offset, v1 );
        offset += fillCore( buf + offset, v2 );
        offset += fillCore( buf + offset, v3 );
        offset += fillCore( buf + offset, v4 );
        offset += fillCore( buf + offset, v5 );
        buf[ offset ] = '\0';
        return offset;
    }
    template<typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
    int fill( char * buf, T0 const & v0, T1 const & v1, T2 const & v2, T3 const & v3, T4 const & v4, T5 const & v5, T6 const & v6 )
    {
        int offset = fillCore( buf, v0 );
        offset += fillCore( buf + offset, v1 );
        offset += fillCore( buf + offset, v2 );
        offset += fillCore( buf + offset, v3 );
        offset += fillCore( buf + offset, v4 );
        offset += fillCore( buf + offset, v5 );
        offset += fillCore( buf + offset, v6 );
        buf[ offset ] = '\0';
        return offset;
    }
    template<typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
    int fill( char * buf, T0 const & v0, T1 const & v1, T2 const & v2, T3 const & v3, T4 const & v4, T5 const & v5, T6 const & v6, T7 const & v7 )
    {
        int offset = fillCore( buf, v0 );
        offset += fillCore( buf + offset, v1 );
        offset += fillCore( buf + offset, v2 );
        offset += fillCore( buf + offset, v3 );
        offset += fillCore( buf + offset, v4 );
        offset += fillCore( buf + offset, v5 );
        offset += fillCore( buf + offset, v6 );
        offset += fillCore( buf + offset, v7 );
        buf[ offset ] = '\0';
        return offset;
    }
    template<typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
    int fill( char * buf, T0 const & v0, T1 const & v1, T2 const & v2, T3 const & v3, T4 const & v4, T5 const & v5, T6 const & v6, T7 const & v7, T8 const & v8 )
    {
        int offset = fillCore( buf, v0 );
        offset += fillCore( buf + offset, v1 );
        offset += fillCore( buf + offset, v2 );
        offset += fillCore( buf + offset, v3 );
        offset += fillCore( buf + offset, v4 );
        offset += fillCore( buf + offset, v5 );
        offset += fillCore( buf + offset, v6 );
        offset += fillCore( buf + offset, v7 );
        offset += fillCore( buf + offset, v8 );
        buf[ offset ] = '\0';
        return offset;
    }
    template<typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
    int fill( char * buf, T0 const & v0, T1 const & v1, T2 const & v2, T3 const & v3, T4 const & v4, T5 const & v5, T6 const & v6, T7 const & v7, T8 const & v8, T9 const & v9 )
    {
        int offset = fillCore( buf, v0 );
        offset += fillCore( buf + offset, v1 );
        offset += fillCore( buf + offset, v2 );
        offset += fillCore( buf + offset, v3 );
        offset += fillCore( buf + offset, v4 );
        offset += fillCore( buf + offset, v5 );
        offset += fillCore( buf + offset, v6 );
        offset += fillCore( buf + offset, v7 );
        offset += fillCore( buf + offset, v8 );
        offset += fillCore( buf + offset, v9 );
        buf[ offset ] = '\0';
        return offset;
    }



    inline int toStringGetMaxLength( char v ) { return 4; }
    inline int toStringGetMaxLength( int v ) { return 11; }
    inline int toStringGetMaxLength( uint v ) { return 10; }
    inline int toStringGetMaxLength( float v ) { return 17; }
    inline int toStringGetMaxLength( double v ) { return 20; }
    inline int toStringGetMaxLength( string const & s ) { return s.size(); }

    template<typename T0>
    std::string toString( T0 const & v0 )
    {
        std::string s;
        s.resize( toStringGetMaxLength( v0 ) );
        s.resize( fill( (char*)s.data(), v0 ) );
        return s;
    }
    template<typename T0, typename T1>
    std::string toString( T0 const & v0, T1 const & v1 )
    {
        std::string s;
        s.resize( toStringGetMaxLength( v0 )
                  + toStringGetMaxLength( v1 )
                  );
        s.resize( fill( (char*)s.data(), v0, v1 ) );
        return s;
    }
    template<typename T0, typename T1, typename T2>
    std::string toString( T0 const & v0, T1 const & v1, T2 const & v2 )
    {
        std::string s;
        s.resize( toStringGetMaxLength( v0 )
                  + toStringGetMaxLength( v1 ) 
                  + toStringGetMaxLength( v2 )
                  );
        s.resize( fill( (char*)s.data(), v0, v1, v2 ) );
        return s;
    }
    template<typename T0, typename T1, typename T2, typename T3>
    std::string toString( T0 const & v0, T1 const & v1, T2 const & v2, T3 const & v3 )
    {
        std::string s;
        s.resize( toStringGetMaxLength( v0 )
                  + toStringGetMaxLength( v1 ) 
                  + toStringGetMaxLength( v2 ) 
                  + toStringGetMaxLength( v3 )
                  );
        s.resize( fill( (char*)s.data(), v0, v1, v2, v3 ) );
        return s;
    }
    template<typename T0, typename T1, typename T2, typename T3, typename T4>
    std::string toString( T0 const & v0, T1 const & v1, T2 const & v2, T3 const & v3, T4 const & v4 )
    {
        std::string s;
        s.resize( toStringGetMaxLength( v0 )
                  + toStringGetMaxLength( v1 )
                  + toStringGetMaxLength( v2 )
                  + toStringGetMaxLength( v3 )
                  + toStringGetMaxLength( v4 )
                  );
        s.resize( fill( (char*)s.data(), v0, v1, v2, v3, v4 ) );
        return s;
    }
    template<typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
    std::string toString( T0 const & v0, T1 const & v1, T2 const & v2, T3 const & v3, T4 const & v4, T5 const & v5 )
    {
        std::string s;
        s.resize( toStringGetMaxLength( v0 )
                  + toStringGetMaxLength( v1 )
                  + toStringGetMaxLength( v2 )
                  + toStringGetMaxLength( v3 )
                  + toStringGetMaxLength( v4 )
                  + toStringGetMaxLength( v5 )
                  );
        s.resize( fill( (char*)s.data(), v0, v1, v2, v3, v4, v5 ) );
        return s;
    }
    template<typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
    std::string toString( T0 const & v0, T1 const & v1, T2 const & v2, T3 const & v3, T4 const & v4, T5 const & v5, T6 const & v6 )
    {
        std::string s;
        s.resize( toStringGetMaxLength( v0 )
                  + toStringGetMaxLength( v1 )
                  + toStringGetMaxLength( v2 )
                  + toStringGetMaxLength( v3 )
                  + toStringGetMaxLength( v4 )
                  + toStringGetMaxLength( v5 )
                  + toStringGetMaxLength( v6 )
                  );
        s.resize( fill( (char*)s.data(), v0, v1, v2, v3, v4, v5, v6 ) );
        return s;
    }
    template<typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
    std::string toString( T0 const & v0, T1 const & v1, T2 const & v2, T3 const & v3, T4 const & v4, T5 const & v5, T6 const & v6, T7 const & v7 )
    {
        std::string s;
        s.resize( toStringGetMaxLength( v0 )
                  + toStringGetMaxLength( v1 )
                  + toStringGetMaxLength( v2 )
                  + toStringGetMaxLength( v3 )
                  + toStringGetMaxLength( v4 )
                  + toStringGetMaxLength( v5 )
                  + toStringGetMaxLength( v6 )
                  + toStringGetMaxLength( v7 )
                  );
        s.resize( fill( (char*)s.data(), v0, v1, v2, v3, v4, v5, v6, v7 ) );
        return s;
    }
    template<typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
    std::string toString( T0 const & v0, T1 const & v1, T2 const & v2, T3 const & v3, T4 const & v4, T5 const & v5, T6 const & v6, T7 const & v7, T8 const & v8 )
    {
        std::string s;
        s.resize( toStringGetMaxLength( v0 )
                  + toStringGetMaxLength( v1 )
                  + toStringGetMaxLength( v2 )
                  + toStringGetMaxLength( v3 )
                  + toStringGetMaxLength( v4 )
                  + toStringGetMaxLength( v5 )
                  + toStringGetMaxLength( v6 )
                  + toStringGetMaxLength( v7 )
                  + toStringGetMaxLength( v8 )
                  );
        s.resize( fill( (char*)s.data(), v0, v1, v2, v3, v4, v5, v6, v7, v8 ) );
        return s;
    }
    template<typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
    std::string toString( T0 const & v0, T1 const & v1, T2 const & v2, T3 const & v3, T4 const & v4, T5 const & v5, T6 const & v6, T7 const & v7, T8 const & v8, T9 const & v9 )
    {
        std::string s;
        s.resize( toStringGetMaxLength( v0 )
                  + toStringGetMaxLength( v1 )
                  + toStringGetMaxLength( v2 )
                  + toStringGetMaxLength( v3 )
                  + toStringGetMaxLength( v4 )
                  + toStringGetMaxLength( v5 )
                  + toStringGetMaxLength( v6 )
                  + toStringGetMaxLength( v7 )
                  + toStringGetMaxLength( v8 )
                  + toStringGetMaxLength( v9 )
                  );
        s.resize( fill( (char*)s.data(), v0, v1, v2, v3, v4, v5, v6, v7, v8, v9 ) );
        return s;
    }






    /************************************************************************/
    /* 常用字串处理相关                                                     */
    /************************************************************************/

    void trim( string& s );








    /************************************************************************/
    /* utf8 相关代码                                                        */
    /************************************************************************/

    // 得到含有 指定数量 的半角个数的 utf8 串的切割点( 末尾的半个字将不被包含 )
    int getCutIndex( char const * s, int numHalfChars );

    // 统计 utf8 串的字数( 宽字符算 1 长 )
    int getWCharCount( char const * s );

    // 统计 utf8 串中 宽字符 的个数
    int getOnlyWCharCount( char const * s );

    // 统计 utf8 串的字数( 宽字符算 2 长 )
    int getCharCount( char const * s );












    /************************************************************************/
    /* 快捷绘制相关                                                         */
    /************************************************************************/

    // 图片内容为 左右 结构的 [ ] 和 [v] 两张图拼接
    Sprite * createSwitchButtonSprite( string const & fileName );

    // 根据传入节点的 pos, contentSize, 在节点外面显示个框( 当前只支持锚点为 0.5, 0.5 )
    Node * drawRect( Node * n );
    Node * drawRichTextRect( Node * n );

    // 创建一个可拖拽的精灵
    Sprite * createDragableSprite( string const & fileName );









    /************************************************************************/
    /* 下面是些半成品或设计初步的代码                                       */
    /************************************************************************/



    // 可作为 map 之类容器的 key 值的模板( 初稿 )
    template<typename T>
    class Hash
    {
    public:
        typedef Hash<T> CT;
        Hash<T>( T && t ) : _t( t )
        {
            // todo: type trits check, 
            // 如果 T 是内置数据类型( 各种整数浮点数或枚举 ), size 小于等于 sizeof(_hash) 的直接存, 大的当 char* 算
            // 如果 T 是类, 且实现了 int hash() 函数, 调之. 否则将类硬转为 char* 来算.
            // 如果 T 是指针, 则先转为类, 同上处理
            _hash = Utils::getHashCode( (char*)&t, sizeof( t ) );
        }
        bool operator<( CT const & other ) const
        {
            if( this->_hash < other._hash ) return true;
            else if( this->_hash == other._hash )
            {
                // todo: type trits check, 如果 T 是指针, 转为类再 ...,  这要求类实现 operator < 
                return *this < other;
            }
            return false;
        }
        T   _t;
        int _hash;
    };



    //// 简单的按单个字符切割字串 并返回 pair<char*, size_t> 指针, 长度 集 ( 还没测 )
    //inline vector<pair<char const *, size_t>> splite( string const &s, char sc )
    //{
    //    auto p = s.c_str();                             // 方便用
    //    vector<pair<char const *, size_t>> ss;
    //    ss.reserve( 16 );                               // 经验数据
    //    int prev = 0;
    //    size_t i = 0;
    //    for( ; i < s.size(); ++i )
    //    {
    //        if( p[ i ] == sc )
    //        {
    //            ss.push_back( make_pair( p + prev, i - prev ) );
    //            prev = i + 1;
    //        }
    //    }
    //    if( i < s.size() )
    //    {
    //        ss.push_back( make_pair( p + i, ss.size() - i ) );
    //    }
    //    return ss;
    //}


}

#endif
