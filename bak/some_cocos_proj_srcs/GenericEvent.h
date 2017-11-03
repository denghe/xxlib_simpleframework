#ifndef _GENERICEVENT_H_
#define _GENERICEVENT_H_

// 通用事件参数( 只支持 double, string 两种数据类型 )
struct GenericEventParameter
{
    GenericEventParameter( double value_numeric );
    GenericEventParameter( string value_string );
    GenericEventParameter( GenericEventParameter const & other );
    GenericEventParameter( GenericEventParameter && other );
    GenericEventParameter & operator=( GenericEventParameter const & other );
    GenericEventParameter & operator=( GenericEventParameter && other );
    bool isNumeric;             // 通过这个 bool 来路由下面的值
    double value_numeric;
    string value_string;
};


// 通用事件体( 便于向 lua 抛变长参数. 多于7个参数 分行 push )
template<typename ET>
struct GenericEvent
{
    GenericEvent() : userData( nullptr ) {}
    GenericEvent( ET const & eventType ) : eventType( eventType ), userData( nullptr ) {}
    GenericEvent( ET const & eventType
                  , GenericEventParameter p0 )
                  : eventType( eventType )
                  , userData( nullptr )
    {
        parameters.push_back( p0 );
    }
    GenericEvent( ET const & eventType
                  , GenericEventParameter p0
                  , GenericEventParameter p1 )
                  : eventType( eventType )
                  , userData( nullptr )
    {
        parameters.push_back( p0 );
        parameters.push_back( p1 );
    }
    GenericEvent( ET const & eventType
                  , GenericEventParameter p0
                  , GenericEventParameter p1
                  , GenericEventParameter p2 )
                  : eventType( eventType )
                  , userData( nullptr )
    {
        parameters.push_back( p0 );
        parameters.push_back( p1 );
        parameters.push_back( p2 );
    }
    GenericEvent( ET const & eventType
                  , GenericEventParameter p0
                  , GenericEventParameter p1
                  , GenericEventParameter p2
                  , GenericEventParameter p3 )
                  : eventType( eventType )
                  , userData( nullptr )
    {
        parameters.push_back( p0 );
        parameters.push_back( p1 );
        parameters.push_back( p2 );
        parameters.push_back( p3 );
    }
    GenericEvent( ET const & eventType
                  , GenericEventParameter p0
                  , GenericEventParameter p1
                  , GenericEventParameter p2
                  , GenericEventParameter p3
                  , GenericEventParameter p4 )
                  : eventType( eventType )
                  , userData( nullptr )
    {
        parameters.push_back( p0 );
        parameters.push_back( p1 );
        parameters.push_back( p2 );
        parameters.push_back( p3 );
        parameters.push_back( p4 );
    }
    GenericEvent( ET const & eventType
                  , GenericEventParameter p0
                  , GenericEventParameter p1
                  , GenericEventParameter p2
                  , GenericEventParameter p3
                  , GenericEventParameter p4
                  , GenericEventParameter p5 )
                  : eventType( eventType )
                  , userData( nullptr )
    {
        parameters.push_back( p0 );
        parameters.push_back( p1 );
        parameters.push_back( p2 );
        parameters.push_back( p3 );
        parameters.push_back( p4 );
        parameters.push_back( p5 );
    }
    GenericEvent( ET const & eventType
                  , GenericEventParameter p0
                  , GenericEventParameter p1
                  , GenericEventParameter p2
                  , GenericEventParameter p3
                  , GenericEventParameter p4
                  , GenericEventParameter p5
                  , GenericEventParameter p6 )
                  : eventType( eventType )
                  , userData( nullptr )
    {
        parameters.push_back( p0 );
        parameters.push_back( p1 );
        parameters.push_back( p2 );
        parameters.push_back( p3 );
        parameters.push_back( p4 );
        parameters.push_back( p5 );
        parameters.push_back( p6 );
    }

    GenericEvent( GenericEvent const & other )
        : eventType( other.eventType )
        , userData( other.userData )
        , parameters( other.parameters )
    {
    }

    GenericEvent( GenericEvent && other )
        : eventType( other.eventType )
        , userData( other.userData )
        , parameters( move( other.parameters ) )
    {
    }

    GenericEvent & operator=( GenericEvent const & other )
    {
        eventType = other.eventType;
        userData = other.userData;
        parameters = other.parameters;
        return *this;
    }

    GenericEvent & operator=( GenericEvent && other )
    {
        eventType = other.eventType;
        userData = other.userData;
        parameters = move( other.parameters );
        return *this;
    }

    // 跟在构造函数后面接着用 从而实现单行附加 userData
    GenericEvent && attachUserData( void* userData )
    {
        this->userData = userData;
        return move( *this );
    }

    ET                              eventType;
    void *                          userData;
    vector<GenericEventParameter>   parameters;
};



#endif
