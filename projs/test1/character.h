struct Character : SceneObjBase
{
	int x = 0;

	xx::List_v<FSMLua*> quests;

	Character();
	~Character();
};
