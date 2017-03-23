struct FSMLua : FSMBase
{
	lua_State* co = nullptr;		// 保存协程指针
	xx::String_v err;

	virtual int Update() override;
	template<typename T>
	FSMLua(T* owner, char const* codeOrFile, bool isFile = true);
	~FSMLua();
};
