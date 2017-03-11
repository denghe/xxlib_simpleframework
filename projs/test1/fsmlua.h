struct FSMLua : FSMBase
{
	lua_State* co = nullptr;		// 保存协程指针

	xx::MemHeader_MPObject errMH;	// 给下面这个类提供内存头
	xx::String err;

	virtual int Update() override;
	template<typename T>
	FSMLua(T* owner, char const* fn);
	~FSMLua();
};
