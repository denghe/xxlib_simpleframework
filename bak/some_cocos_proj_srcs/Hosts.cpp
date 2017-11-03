#include "Precompile.h"

MAKE_INSTANCE_CPP( Hosts );

Hosts::Hosts()
{
    _toForegroundListener = Director::getInstance()->getEventDispatcher()->addCustomEventListener( EVENT_COME_TO_FOREGROUND, [ this ]( EventCustom* event )
    {
    } );

    _toBackgroundListener = Director::getInstance()->getEventDispatcher()->addCustomEventListener( EVENT_COME_TO_BACKGROUND, [ this ]( EventCustom* event )
    {
    } );

    // register to schedule frame update 
    Director::getInstance()->getScheduler()->scheduleUpdate( this, -1, false );
}

Hosts::~Hosts()
{
    // unregister schedule frame update 
    Director::getInstance()->getScheduler()->unscheduleUpdate( this );

    Director::getInstance()->getEventDispatcher()->removeEventListener( _toForegroundListener );
    Director::getInstance()->getEventDispatcher()->removeEventListener( _toBackgroundListener );

    for( auto& o : _refContainer ) o->release();
}

// 存放 getTime 的值
int _currTime = 0;

void update_time()
{
    struct timeval now;
    gettimeofday( &now, nullptr );
    _currTime = int(now.tv_sec * 1000 + now.tv_usec / 1000);
}


void Hosts::update( float dt )
{
    // update getTime 的值
    update_time();
    
    for( auto &o : _handlers ) o.second( dt );

    NetEngine::getInstance()->update();
    FileManager::getInstance()->update(dt);
    CommandQueue::getInstance()->update();
    Purchase::getInstance()->update();
}

void Hosts::registerGameLoopEventHandler( string const &key, GameLoopEventHandler func )
{
    if( !_handlers.insert( make_pair( key, func ) ).second )
    {
        CCLOG( "Hosts::registerGameLoopEventHandler( string const &key, GameLoopEventHandler func ) key already exists, key = %s", key.c_str() );
    }
}

void Hosts::registerGameLoopEventHandler( string const &key, LUA_FUNCTION func )
{
    GameLoopEventHandler f = [ func ]( float t )
    {
        LuaHelper::push( t );
        LuaHelper::executeFunction( func, 1 );
    };
    registerGameLoopEventHandler( key, f );
}

bool Hosts::unregisterGameLoopEventHandler( string const &key )
{
    auto it = _handlers.find( key );
    if( it != _handlers.end() )
    {
        _handlers.erase( it );
        return true;
    }
    else
    {
        CCLOG( "Hosts::unregisterGameLoopEventHandler( string const &key ) can't find key = %s", key.c_str() );
        return false;
    }
}


int Hosts::getTime() const
{
    return _currTime;
}


void Hosts::restart()
{
    auto d = Director::getInstance();
    auto sc = d->getScheduler();
    auto tc = d->getTextureCache();
    auto sfc = SpriteFrameCache::getInstance();
    auto sae = SimpleAudioEngine::getInstance();
    auto fu = FileUtils::getInstance();
    auto ad = (AppDelegate*)Application::getInstance();

    this->_toBackgroundHandler = nullptr;
    this->_toForegroundHandler = nullptr;
    this->_handlers.clear();

    sc->schedule( [ = ]( float dt )
    {
        d->popToRootScene();

        sc->unschedule( "void Hosts::restart()_1", this );

        sc->schedule( [ = ]( float dt )
        {
            sc->unscheduleAllWithMinPriority( -1 );     // 有一个框架级的不能清

            d->getRunningScene()->removeAllChildren();
            d->getRunningScene()->cleanup();

            ScriptEngineManager::destroyInstance();
            AnimationCache::destroyInstance();

            ad->myFree();

            sfc->removeSpriteFrames();
            tc->removeUnusedTextures();
            if( tc->getTextureCount() > 1 )     // 剩的一个是显示 fps 用的
            {
                CCLOG( "error! the texture count must be 1!" );
                CCLOG( "%s", tc->getCachedTextureInfo().c_str() );
                tc->removeAllTextures();
            }
            d->resetTextureCache();
            sae->end();
            fu->purgeCachedEntries();

            ad->myInit();

        }, this, 0, 1, 0.0f, false, "void Hosts::restart()_2" );

    }, this, 0, 1, 0.0f, false, "void Hosts::restart()_1" );
}




void Hosts::addRef( Ref* o )
{
    if( _refContainer.insert( o ).second ) o->retain();
}

void Hosts::removeRef( Ref* o )
{
    auto it = _refContainer.find( o );
    if( it == _refContainer.end() ) return;
    o->release();
    _refContainer.erase( it );
}

string Hosts::sdkType()
{
#if CHANNEL_UC
    return "ucsdk";
#endif
#if CHANNEL_HUAWEI
    return "huawei";
#endif
#if CHANNEL_VIVO
    return "vivo";
#endif
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    return "anysdk";
#endif
    return "";
}

string Hosts::base64Encode(string &s)
{
    char *e = nullptr;
    ::base64Encode((unsigned char *)s.data(), s.length(), &e);
    Utils::ScopeGuard freeBuf([=] {
        if (e) ::free(e);
    });

    return string(e);
}

string Hosts::base64Decode(string &s)
{
    char *e = nullptr;
    ::base64Decode((unsigned char *)s.data(), s.length(), (unsigned char **)&e);
    Utils::ScopeGuard freeBuf([=] {
        if (e) ::free(e);
    });

    if (e) return string(e);

    return "";
}


void Hosts::registerToForegroundEventHandler( GenericEventHandler func )
{
    _toForegroundHandler = func;
}

void Hosts::registerToForegroundEventHandler( LUA_FUNCTION func )
{
    if( func ) _toForegroundHandler = [ func ]
    {
        LuaHelper::executeFunction( func );
    };
    else _toForegroundHandler = nullptr;
}


void Hosts::registerToBackgroundEventHandler( GenericEventHandler func )
{
    _toBackgroundHandler = func;
}

void Hosts::registerToBackgroundEventHandler( LUA_FUNCTION func )
{
    if( func ) _toBackgroundHandler = [ func ]
    {
        LuaHelper::executeFunction( func );
    };
    else _toBackgroundHandler = nullptr;
}
























//
//string const & Hosts::getUDID() const
//{
//    return _getUUIDFunc();
//}
//
//pair<int, int> const & Hosts::getVersion() const
//{
//    // static pair<int, int> _tmp;
//    // _tmp = make_pair( 1, 0 );
//    // return _tmp;
//    CCLOG( "before return _getVersionFunc();" );
//    return _getVersionFunc();
//}
//
//function<pair<int, int> const & ( void )> Hosts::_getVersionFunc;
//
//function<string const & ( void )> Hosts::_getUUIDFunc;



/*


void Hosts::update( float dt )
{
// 清掉 getTime 的值 以便重算
_currTime = 0;

//if( _f )
//{
//    LuaHelper::push( dt );
//    LuaHelper::executeFunction( _f, 1 );
//}
//    NetEngine::getInstance()->_clients.foreach( []( NetClient*& c )
//    {
//        c->send();
//        c->update();
//    } );
//    WWCore::getInstance()->update( dt );
//    FileManager::getInstance()->update();
//
//    if( _cbs.size() )
//    {
//        _callCBing = true;                          // 开始 call
//        for( auto &cb : _cbs ) cb.second();
//        _callCBing = false;
//
//        for( auto &task : _tasks )                  // 如果 call 的过程中发生 regCB 或 unregCB 操作, 依次处理
//        {
//            if( task.second )                       // 如果 second 为空就是删除操作
//            {
//                regCB( task.first, task.second );
//            }
//            else
//            {
//                unregCB( task.first );
//            }
//        }
//        _tasks.clear();
//    }
//
//    CommandQueue::getInstance()->update();
}

void Hosts::registerGameLoopEventHandler( string const &key, GameLoopEventHandler func )
{
//    if( _callCBing )
//    {
//        _tasks.push_back( make_pair( key, cb ) );
//    }
//    else
//    {
//        if( !_cbs.insert( make_pair( key, cb ) ).second )
//        {
//            CCLOG( "FrameUpdater::regCB( string const &key, FT cb ) key already exists, key = %s", key.c_str() );
//        }
//    }
}

void Hosts::registerGameLoopEventHandler( string const &key, LUA_FUNCTION func )
{

}

bool Hosts::unregisterGameLoopEventHandler( string const &key )
{
//    if( _callCBing )
//    {
//        _tasks.push_back( make_pair( key, nullptr ) );
//    }
//    else
//    {
//        auto it = _cbs.find( key );
//        if( it != _cbs.end() )
//        {
//            _cbs.erase( it );
//        }
//        else
//        {
//            CCLOG( "FrameUpdater::unregCB( string const &key ) can't find key = %s", key.c_str() );
//        }
//    }
}




// 这种方式导致测试用例中的 button 事件第2次注册失败的样子
//
//class RestartScene : public Scene
//{
//public:
//    inline static RestartScene* create()
//    {
//        auto rtv = new RestartScene();
//        if( !rtv->init() )
//        {
//            delete rtv;
//            return nullptr;
//        }
//        rtv->autorelease();
//        rtv->_restarted = false;
//        return rtv;
//    }
//    static bool _restarted;
//    inline void onEnter()
//    {
//        if( _restarted ) return;
//        _restarted = true;
//
//        auto d = Director::getInstance();
//        auto sc = d->getScheduler();
//        auto tc = d->getTextureCache();
//        auto sfc = SpriteFrameCache::getInstance();
//        auto sae = SimpleAudioEngine::getInstance();
//        auto fu = FileUtils::getInstance();
//        auto ad = (AppDelegate*)Application::getInstance();
//
//        sc->unscheduleAllWithMinPriority( -1 );     // 有一个框架级的不能清
//
//        ScriptEngineManager::destroyInstance();
//        AnimationCache::destroyInstance();
//
//        ad->myFree();
//
//        sfc->removeSpriteFrames();
//        tc->removeUnusedTextures();
//        if( tc->getTextureCount() > 1 )     // 剩的一个是显示 fps 用的
//        {
//            CCLOG( "error! the texture count must be 1!" );
//            CCLOG( "%s", tc->getCachedTextureInfo().c_str() );
//            tc->removeAllTextures();
//        }
//        sae->end();
//        fu->purgeCachedEntries();
//
//#ifdef ENABLE_REF_COUNTER
//        CCLOG( "restarting... num ref objs = %d", Ref::getCount() );
//        auto& refs = Ref::getRefs();
//#endif
//        auto ed = d->getEventDispatcher();
//        ed->removeAllEventListeners();
//
//        ad->myInit();
//    }
//};
//
//bool RestartScene::_restarted;
//
//void Hosts::restart()
//{
//    auto d = Director::getInstance();
//    d->popToRootScene();
//    d->replaceScene( RestartScene::create() );
//}



*/