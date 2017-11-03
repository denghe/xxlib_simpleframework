#ifndef __LRUCACHE_H__
#define __LRUCACHE_H__


/*

Least Recently Used 近期最少使用 cache 池

示例:

int main()
{
    // 限定 cache 长度为 3
    LRUCache<string, int> c( 3 );   // 下面的输出不一定符合预期排序

    c.add( "11", 123 );             // 输出 11
    c.dump();
    c.add( "22", 321 );             // 输出 11, 22
    c.dump();
    c.add( "33", 1212 );            // 输出 11, 22, 33
    c.dump();
    c.add( "44", 43254 );           // 输出 22, 33, 44    ( 11 被移除 )
    c.dump();
    c.add( "55", 321 );             // 输出 33, 44, 55    ( 22 被移除 )
    c.dump();
    c.get( "33" );                  // 提升了 33 的排位
    c.add( "77", 321 );             // 输出 33, 55, 77    ( 44 被移除 )
    c.dump();



示例 2:

class Foo
{
public:
    Foo() { cout << "Foo()" << endl; }
    ~Foo() { cout << "~Foo()" << endl; }
    Foo( Foo const & other ) { cout << "Foo( Foo const & other )" << endl; }
    Foo( Foo && other ) { cout << "Foo( Foo && other )" << endl; }
    Foo & operator=( Foo const & other ) { cout << "Foo & operator=( Foo const & other )" << endl; }
    Foo & operator=( Foo && other ) { cout << "Foo & operator=( Foo && other )" << endl; }
};


int main()
{
    LRUCache<string, shared_ptr<Foo>> c( 2 );
    c.add( "11", shared_ptr<Foo>( new Foo() ) );
    c.dump();
    c.add( "22", new Foo() );
    c.dump();
    c.add( "33", new Foo() );
    c.dump();
    cout << "before clear\n\n";
    c.clear();
    cout << "clear()\n\n";
    c.dump();


*/


template<typename KT, typename VT>
class LRUCache;


template<typename KT, typename VT>
class LRUCacheItem
{
public:
    template<typename PKT, typename PVT>
    LRUCacheItem( PKT && key, PVT && value )        // 放入容器之后再 link. 不然 head 的 prev next 指向的地址可能不正确
        : _next( nullptr )                          // 故意令指针为空, 因为复制后( 例如放入容器 ) 需要 link 到链表才有意义
        , _prev( nullptr )
        , _key( forward<PKT>( key ) )
        , _value( forward<PVT>( value ) )
    {
    }

    LRUCacheItem( LRUCacheItem const & other )
        : _next( nullptr )
        , _prev( nullptr )
        , _key( other._key )
        , _value( other._value )
    {
    }

    LRUCacheItem( LRUCacheItem && other )
        : _next( nullptr )
        , _prev( nullptr )
        , _key( move( other._key ) )
        , _value( move( other._value ) )
    {
    }

    LRUCacheItem & operator=( LRUCacheItem const & other )
    {
        _next = nullptr;
        _prev = nullptr;
        _key = other._key;
        _value = other._value;
    }

    LRUCacheItem & operator=( LRUCacheItem && other )
    {
        _next = nullptr;
        _prev = nullptr;
        _key = move( other._key );
        _value = move( other._value );
    }

    friend LRUCache<KT, VT>;
private:
    LRUCacheItem( LRUCacheItem * next, LRUCacheItem * prev )
        : _next( next )
        , _prev( prev )
        //, _key( KT() )            // 似乎不必初始化
        //, _value( VT() )
    {
    }

    void link( LRUCacheItem & head )
    {
        _next = &head;
        _prev = head._prev;
        head._prev->_next = this;
        head._prev = this;
    }

    void unlink()
    {
        _prev->_next = _next;
        _next->_prev = _prev;
    }

    void moveTo( LRUCacheItem & head )
    {
        unlink();
        this->_next = &head;
        this->_prev = head._prev;
        head._prev->_next = this;
        head._prev = this;
    }

    LRUCacheItem *  _next;
    LRUCacheItem *  _prev;
    KT              _key;
    VT              _value;
};


template<typename KT, typename VT>
class LRUCache
{
public:
    typedef LRUCacheItem<KT, VT> ITEM_T;

    // limit 为长度限定. 超限将移除
    LRUCache( int limit = 100 )
        : _limit( limit )
        , _head( &_head, &_head )
    {
        _data.reserve( limit + 1 );
    }

    // add. 返回指向 _data 中存储 value 的地址 和  true( add成功 ), false( 已存在 )
    template<typename PKT, typename PVT>
    pair<VT*, bool> add( PKT && key, PVT && value, bool override = true )
    {
        pair<VT*, bool> rtv;
        auto it = _data.insert( make_pair( forward<PKT>( key ), ITEM_T( key, forward<PVT>( value ) ) ) );
        if( it.second )
        {
            it.first->second.link( _head );
            if( (int)_data.size() > _limit )
            {
                evict();
            }
            rtv.second = false;
        }
        else if( override ) // 覆盖
        {
            it.first->second._value = forward<PVT>( value );
            rtv.second = true;
        }
        rtv.first = &it.first->second._value;
        return rtv;
    }

    // get. 顺便提升排名, 防移除
    VT * get( KT const & key )
    {
        auto it = _data.find( key );
        if( it == _data.end() )
        {
            return nullptr;
        }
        it->second.moveTo( _head );
        return &it->second._value;
    }

    // clear
    void clear()
    {
        _data.clear();
    }

    // dump
    void dump()
    {
        for( auto &o : _data ) cout << o.first << endl;
        cout << endl;
    }

    // todo: 缺批量移除? 缺 _data 暴露?

private:
    // 移除最后一个
    void evict()
    {
        auto lastItem = _head._next;
        lastItem->unlink();
        _data.erase( lastItem->_key );
    }

    int                         _limit;
    ITEM_T                      _head;
    unordered_map<KT, ITEM_T>   _data;
};


#endif

