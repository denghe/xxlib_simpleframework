#ifndef _THREADSAFEQUEUE_HPP_
#define _THREADSAFEQUEUE_HPP_


template<typename T>
ThreadSafeQueue<T>::ThreadSafeQueue()
{
}

template<typename T>
ThreadSafeQueue<T>::ThreadSafeQueue( ThreadSafeQueue const & other )
{
    lock_guard<mutex> g( other._m );
    _q = other._q;
}

template<typename T>
ThreadSafeQueue<T>::ThreadSafeQueue( ThreadSafeQueue && other )
{
    lock_guard<mutex> g( other._m );
    _q = move( other._q );
}

template<typename T>
template <typename VT>
void ThreadSafeQueue<T>::push( VT && item )
{
    lock_guard<mutex> g( _m );
    _q.push_back( forward<VT>( item ) );
    _c.notify_one();
}

template<typename T>
bool ThreadSafeQueue<T>::pop( T & outVal, function<bool( T const& )> f )
{
    lock_guard<mutex> g( _m );
    if( _q.empty() ) return false;
    outVal = _q.front();
    if( f && !f( outVal ) ) return false;
    _q.pop_front();
    return true;
}

template<typename T>
shared_ptr<T> ThreadSafeQueue<T>::pop()
{
    lock_guard<mutex> g( _m );
    if( _q.empty() ) return shared_ptr<T>();
    shared_ptr<T> r( make_shared<T>( _q.front() ) );
    _q.pop_front();
    return r;
}

template<typename T>
shared_ptr<T> ThreadSafeQueue<T>::popWait()
{
    unique_lock<mutex> g( _m );
    _c.wait( g, [ this ] { return !_q.empty(); } );
    shared_ptr<T> r( make_shared<T>( _q.front() ) );
    _q.pop_front();
    return r;
}

template<typename T>
void ThreadSafeQueue<T>::popWait( T& outVal )
{
    unique_lock<mutex> g( _m );
    _c.wait( g, [ this ] { return !_q.empty(); } );
    outVal = _q.front();
    _q.pop_front();
}

template<typename T>
bool ThreadSafeQueue<T>::empty() const
{
    lock_guard<mutex> g( _m );
    return _q.empty();
}

template<typename T>
void ThreadSafeQueue<T>::clear()
{
    lock_guard<mutex> g( _m );
    _q.clear();
}

template<typename T>
int ThreadSafeQueue<T>::size() const
{
    lock_guard<mutex> g( _m );
    return _q.size();
}

template<typename T>
void ThreadSafeQueue<T>::clear( function<void( T& )> f )
{
    lock_guard<mutex> g( _m );
    for( auto &o : _q ) f( o );
    _q.clear();
}

template<typename T>
int ThreadSafeQueue<T>::erase( function<bool( T& )> f )
{
    lock_guard<mutex> g( _m );
    int count = 0;
    for( auto it = _q.begin(); it != _q.end(); )
    {
        if( f( *it ) )
        {
            ++count;
            auto next = _q.erase( it );
            if( next == _q.end() ) break;
            it = next;
        }
        else ++it;
    }
    return count;
}

template<typename T>
bool ThreadSafeQueue<T>::exists( function<bool( T const & )> f )
{
    lock_guard<mutex> g( _m );
    for( auto &o : _q ) if( f( o ) ) return true;
    return false;
}


template<typename T>
void ThreadSafeQueue<T>::foreach( function<void( T& )> f )
{
    lock_guard<mutex> g( _m );
    for( auto &o : _q ) f( o );
}


/*

erase ≤‚ ‘¥˙¬Î

ThreadSafeQueue<int> q;
q.push( 1 );
q.push( 2 );
q.push( 3 );
q.push( 4 );
q.push( 5 );

q.erase( []( int & v )
{
return ( v == 1 || v == 3 || v == 5 );
} );

while( auto p = q.pop() )
{
CCLOG( "%d", *p );
}


exists ≤‚ ‘¥˙¬Î

ThreadSafeQueue<int> q;
q.push( 1 );
q.push( 2 );
q.push( 3 );
CCLOG( "%d", q.exists( []( int v ) { return v == 4; } ) );

*/



#endif
