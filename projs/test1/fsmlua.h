struct FSMLua : FSMBase
{
	lua_State* co = nullptr;		// 保存协程指针
	xx::String* err;

	virtual int Update() override;
	lua_State* GetL() const;		// 用于定位到 owner-> scene 的状态机
	template<typename T>
	FSMLua(T* owner, char const* fn);
	~FSMLua();
};
