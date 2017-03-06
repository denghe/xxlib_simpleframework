#include "xx_scene.h"

struct Monster1;
// more predefine here...

struct Scene : SceneBase
{
	static const int64_t msPerFrame = 1000 / 20;
	int64_t ticks = 0;
	bool running = true;		// 可用于通知 Run 退出

	Scene();
	~Scene();

	void Update();				// call by Run
	int Run();


	/****************************************************************/
	// lua
	/****************************************************************/

	lua_State* L = nullptr;
	lua_State* co = nullptr;		// SceneManager 自己的脚本( 执行优先于 objs 之 Updates )
	xx::String* err = nullptr;

	// 方便在 lua 中创建 Monster1 对象
	xx::MPtr<Monster1> CreateMonster1(char const* luacode);
};
