#ifndef __DESTRUCTOR_H__
#define __DESTRUCTOR_H__

// 一批 lambda 的容器 for Destructor
class DestructorItem
{
public:
    typedef function<void()> FuncType;

    DestructorItem();
    ~DestructorItem();
    DestructorItem( DestructorItem const & other );
    DestructorItem( DestructorItem && other );

    void add( FuncType f );

    template<typename T>
    T* add( T* p )
    {
        _funcs.push( [ p ] 
        {
            delete p;
        } );
        return p;
    }

    void run();
private:
    stack<FuncType> _funcs;
    DestructorItem & operator=( DestructorItem const & );   // delete
};


// 主用于 堆栈式批量回收 的 lambda 容器( 示例在 cpp 中 )
class Destructor
{
public:
    MAKE_INSTANCE_H( Destructor )

    Destructor();
    ~Destructor();

    void add( DestructorItem::FuncType f );

    template<typename T>
    T* add( T* p )
    {
        if( !_top ) push();
        return _top->add( p );
    }

    void run();

    void push();

    void pop();

private:
    DestructorItem* _top;
    stack<DestructorItem> _destructors;

    Destructor( Destructor const & );
    Destructor & operator=( Destructor const & );
};


#endif
