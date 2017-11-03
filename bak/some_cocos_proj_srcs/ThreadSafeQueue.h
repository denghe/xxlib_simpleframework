#ifndef _THREADSAFEQUEUE_H_
#define _THREADSAFEQUEUE_H_

template<typename T>
class ThreadSafeQueue
{
public:
    ThreadSafeQueue();
    ThreadSafeQueue( ThreadSafeQueue const & other );
    ThreadSafeQueue( ThreadSafeQueue && other );

    // 压入一个 item
    template<typename TV>
    void push( TV && item );                        

    // 取出一个 item( 如果最前端 item 令 f 返回 true 的话. 如果返回 false 表明没有东西取了或不满足 f 。 outVal 可能被污染
    bool pop( T & outVal, function<bool(T const& )> f = nullptr );

    // 取出一个 item. 如果返回值 == nullptr 表明没有东西取了。
    shared_ptr<T> pop();

    // 等待并取出一个 item. todo: 设超时时长
    shared_ptr<T> popWait();

    // 等到有元素可以 pop 为止
    void popWait( T& outVal );

    // 返回是否已空
    bool empty() const;

    // 清队列
    void clear();

    // 返回队列长度
    int size() const;

    // 下面这几个函数 须小心使用 谨防死锁

    // 清队列( 每个 item 依次用传入的 function 过一次 )
    void clear( function<void( T& )> f );

    // foreach 扫, 删掉传入函数返回 true 的元素, 返回一共删了多少个
    int  erase( function<bool( T& )> f );

    // foreach 带入传入函数. 如果任何一个返回 true 则返回 true. 否则返回 false
    bool exists( function<bool( T const& )> f );

    // 纯 foreach ( 整个过程 mutex lock )
    void foreach( function<void( T& )> f );

private:
    mutable mutex           _m;
    deque<T>                _q;
    condition_variable      _c;

    ThreadSafeQueue& operator=( const ThreadSafeQueue& );   // delete
};


#endif
