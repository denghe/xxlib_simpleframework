#include "xx_scene.h"

struct Monster1;
struct Monster2;
// more predefine here...

struct Scene : SceneBase
{
	static const int64_t msPerFrame = 1000 / 20;
	int64_t ticks = 0;

	Scene();
	~Scene();

	int Update();				// call by Run
	int Run();


	/****************************************************************/
	// lua
	/****************************************************************/

	void SetLuaCode(char const* luacode);
	lua_State* L = nullptr;
	lua_State* co = nullptr;		// SceneManager 自己的脚本( 执行优先于 objs 之 Updates )
	xx::String* err = nullptr;

	// 方便在 lua 中创建 Monster1 对象
	xx::MPtr<Monster1> CreateMonster1(char const* luacode);
	xx::MPtr<Monster2> CreateMonster2();
	
};
