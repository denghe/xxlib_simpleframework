#ifndef __GLOBAL_H__
#define __GLOBAL_H__


// 各种全局, 单例相关之所在
class Global
{
public:
    static lua_State * L;               // 指向全局上下文那个
    static LuaStack * LS;               // 指向全局上下文那个
    static string WritablePath;         // 存可写目录
    static string ResourcePath;         // 存资源目录
    static vector<string> SearchPaths;

    // 启动后执行一把( 调用位置: AppDelegate::applicationDidFinishLaunching() 最后, lua 入口程序执行之前 )
    static void init( LuaStack * LS, lua_State * L, string && writablePath, string && resourcePath, vector<string> && SearchPaths );

    // 结束前执行一把( 调用位置: AppDelegate::~AppDelegate() 最后 )
    static void free();         
};


#endif

