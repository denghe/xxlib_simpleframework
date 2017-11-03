#ifndef __HOSTS_H__
#define __HOSTS_H__


// 平台相关, 时间, 游戏循环事件处理
// 不支持在 执行 GameLoopEvent 的时候 register 或 unregister
// 继承自 Node 是为了 每 frame 结束前批量执行一些东西 方便注册
class Hosts : public cocos2d::Node
{
public:
    MAKE_INSTANCE_H( Hosts );

    Hosts();
    ~Hosts();

    // for c2dx schedule register
    virtual void update( float t ) override;

    // 游戏循环事件处理
    typedef function<void( float t )> GameLoopEventHandler;

    // 游戏循环事件处理
    typedef function<void()> GenericEventHandler;

    // key 重复会打日志, 不会覆盖
    void registerGameLoopEventHandler( string const &key, GameLoopEventHandler func );

    // key 重复会打日志, 不会覆盖( for lua )
    void registerGameLoopEventHandler( string const &key, LUA_FUNCTION func );

    // key 找不到会返回 false
    bool unregisterGameLoopEventHandler( string const &key );


    // 注册程序（由后台）切换到前台时执行的代码
    void registerToForegroundEventHandler( GenericEventHandler func );

    // 注册程序（由前台）切换到后台时执行的代码
    void registerToBackgroundEventHandler( GenericEventHandler func );

    // 注册程序（由后台）切换到前台时执行的代码( for lua )
    void registerToForegroundEventHandler( LUA_FUNCTION func );

    // 注册程序（由前台）切换到后台时执行的代码( for lua )
    void registerToBackgroundEventHandler( LUA_FUNCTION func );





    // 返回当前时间的 毫秒 值( 该值每 game loop 仅更新一次 )
    int getTime() const;

    // 重启Lua，清空Cocos2d-x状态
    void restart();

    // 野对象通过这个来记录到容器以加持
    void addRef( Ref* o );
    void removeRef( Ref* o );



    string sdkType();
    string base64Encode(string &s);
    string base64Decode(string &s);

    //**********************************
    // 下列函数的实现 位于平台代码中
    //**********************************

    // 返回设备 UUID
    string const & getUDID() const;

    // 返回设备 MAC Address
    string const & getMAC() const;

    // 返回 hosts 版本号 first: minor    second: major
    // LUA 将得到表 { minor = xxxx, major = xxxx } ( 自己写 lua bind )
    pair<int,int> const & getVersion() const;

    void enabledNotify();

    // 令设备于多少秒之后发起通知( 组号重复的如果未产生通知将覆盖. 组号只在 android 上有 )
    void notify( int groupId, int seconds, string const & content, string const & title = "", string const & soundFn = "" );

	// 打开url
	void openUrl(string url);

    // 于 appDelegate 中相应部位 call
    GenericEventHandler _toForegroundHandler;
    GenericEventHandler _toBackgroundHandler;

private:
    //bool _callCBing;                                        // 正在 call 回调的标志位
    unordered_map<string, GameLoopEventHandler> _handlers;  // 所有回调
    set<Ref*> _refContainer;                                // 代码中原本希望 retain 的都放入
    EventListenerCustom* _toForegroundListener;
    EventListenerCustom* _toBackgroundListener;
};


#endif
