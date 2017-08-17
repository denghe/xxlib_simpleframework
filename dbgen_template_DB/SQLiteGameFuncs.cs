#pragma warning disable 0169, 0414
using TemplateLibrary;

[SQLite, Desc("游戏相关")]
partial interface SQLiteGameFuncs
{
    [Sql(@"
CREATE TABLE [game_account](
    [id] INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE, 
    [username] TEXT(50) NOT NULL UNIQUE, 
    [password] TEXT(50) NOT NULL
);")]
    void CreateTable_game_account();


    [Desc("根据用户名查找并返回账号. 未找到将返回 null")]
    [Sql(@"
    select [id], [username], [password]
      from [game_account]
     where [username] = {0}")]
    Game.Account SelectAccountByUsername(string username);
}
