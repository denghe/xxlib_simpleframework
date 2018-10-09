#pragma warning disable 0169, 0414
using TemplateLibrary;

// 下面是一些从表结构派生出来的业务类. 运行时还会进一步继续附加其他东西

[Desc("管理人员")]
class Manager : Tables.manager
{
    [Key, Desc("当前令牌( 页面每次 Load 时与存放于 Session 中的值做对比用 )")]
    string token;

    [Desc("最后访问的时间点( 页面每次 Load 时更新该值, 用于超时判断 )")]
    DateTime lastVisitTime;
}


[Desc("身份")]
class Role : Tables.role
{
}


[Desc("权限")]
class Permission : Tables.permission
{
}


[Desc("管理人员_身份_绑定")]
class BindManagerRole : Tables.manager_role
{
}


[Desc("身份_权限_绑定")]
class BindRolePermission : Tables.role_permission
{
}




[MySql]
partial interface MySqlFuncs
{

    [Desc("传入 where, orderBy 子句内容做查询, 返回符合条件的前 limit 行数据的 id. 传入的子句需要自带关键字")]
    [Sql(@"
select `id`
  from `manager` {1} {2}
 limit {0}")]
    List<int> Manager_SelectIds(int limit, [Literal] string where, [Literal] string orderBy);


    [Desc("传入 id 列表, 返回相应数据行( 可附带排序规则 )")]
    [Sql(@"
select `id`, `username`, `password`
  from `manager`
 where `id` in {0} {1}")]
    List<Tables.manager> Manager_SelectByIds(List<int> ids, [Literal] string orderBy);



    [Desc("传入 username, password, 返回 id. 如果返回 null 表示未找到.")]
    [Sql(@"
select `id`
  from `manager`
 where `username` = {0} 
   and `password` = {1}")]
    int? Manager_GetIdByUsernamePassword(string username, string password);



    [Desc("传入 username, password, 返回 一行 Manager 记录. 如果返回 null 表示未找到.")]
    [Sql(@"
select `id`, `username`, `password`
  from `manager`
 where `username` = {0} 
   and `password` = {1}")]
    Tables.manager Manager_SelectByUsernamePassword(string username, string password);



    [Desc("插入一条. 可能因为 username 已存在而失败")]
    [Sql(@"
insert into `manager` ( `username`, `password` )
values {0}")]
    void Manager_Insert([SkipReadOnly] Tables.manager o);



    [Desc("改密码. 可能因 找不到 id 而失败")]
    [Sql("update `manager` set `password` = {1} where `id` = {0}")]
    void Manager_UpdatePassword(int id, string newPassword);



    [Desc("改用户名. 可能因 找不到 id 或 新 username 已存在而失败")]
    [Sql(@"
update `manager` 
   set `username` = {1}
 where `id` = {0}")]
    void Manager_UpdateUsername(int id, string newUsername);



    [Desc("删一条. 会同时删掉相关 bind")]
    [Sql(@"
delete from `manager_role` where `manager_id` = {0};
delete from `manager` where `id` = {0}
")]
    void Manager_Delete(int id);



    [Desc("批量重绑身份")]
    [Sql(@"
delete from `manager_role` where `manager_id` = {0};

insert into `manager_role`
       ( `manager_id`, `role_id` )
values {1}")]
    void Manager_SetRoles(int id, List<Tables.manager_role> binds);



    [Desc("读所有数据")]
    [Sql(@"
select `id`, `name`, `desc`
  from `role`")]
    List<Tables.role> Role_SelectAll();



    [Desc("读指定 id 列表数据. ids 必须有数据")]
    [Sql(@"
select `id`, `name`, `desc` 
  from `role` 
 where `id` in {0}")]
    List<Tables.role> Role_SelectByIds(List<int> ids);



    [Desc("读指定 managerId 列表数据")]
    [Sql(@"
select a.`id`, a.`name`, a.`desc`
  from `role` a
  join `manager_role` b
    on a.`id` = b.`role_id`
 where b.`manager_id` = {0}")]
    List<Tables.role> Role_SelectByManagerId(int managerId);



    [Desc("插入一条. 可能因为 name 已存在而失败")]
    [Sql(@"
insert into `role`
       ( `name`, `desc` )
values {0}")]
    void Role_Insert([SkipReadOnly] Tables.manager o);



    [Desc("改名或备注. 可能因 找不到 id 或 新 name 已存在而失败")]
    [Sql(@"
update `role`
   set `name` = {1}
     , `desc` = {2}
 where `id` = {0}")]
    void Role_Update(int id, string newName, string newDesc);



    [Desc("删一条. 会同时删掉相关 bind")]
    [Sql(@"
delete from `manager_role` where `role_id` = {0};
delete from `role_permission` where `role_id` = {0};
delete from `role` where `id` = {0}")]
    void Role_Delete(int id);



    [Desc("批量重绑权限")]
    [Sql(@"
delete from `role_permission` where `role_id` = {0};
insert into `role_permission` ( `role_id`, `permission_id` ) values {1}")]
    void Role_SetPermissions(int id, List<Tables.role_permission> binds);



    [Desc("读所有数据")]
    [Sql(@"
select `id`, `group`, `name`, `desc`
  from `permission`")]
    List<Tables.permission> Permission_SelectAll();



    [Desc("读指定 id 列表数据. ids 必须有数据")]
    [Sql(@"
select `id`, `group`, `name`, `desc`
  from `permission`
 where `id` in {0}")]
    List<Tables.permission> Permission_SelectByIds(List<int> ids);



    [Desc("根据 managerId 获取去重后的权限列表")]
    [Sql(@"
select distinct c.`id`, c.`group`, c.`name`, c.`desc`
  from `manager_role` a
  join `role_permission` b
    on a.`role_id` = b.`role_id`
  join `permission` c
    on b.`permission_id` = c.`id`
 where a.`manager_id` = {0}
")]
    List<Tables.permission> Permission_SelectByManagerId(int managerId);



    [Desc("插入一条. 可能因为 name 已存在而失败")]
    [Sql(@"
insert into `permission`
       ( `name`, `desc` )
values {0}")]
    void Permission_Insert([SkipReadOnly] Tables.manager o);



    [Desc("改内容. 可能因 找不到 id 或 新 name 已存在而失败")]
    [Sql(@"
update `permission`
   set `group` = {1}
     , `name` = {2}
     , `desc` = {3}
 where `id` = {0}")]
    void Permission_Update(int id, string newGroup, string newName, string newDesc);



    [Desc("删一条. 会同时删掉相关 bind")]
    [Sql(@"
delete from `permission` where `id` = {0}")]
    void Permission_Delete(int id);




    [Desc("根据 managerId 获取 身份id列表 以及 去重后的 权限id列表")]
    [Sql(@"
select `role_id`
  from `manager_role`
 where `manager_id` = {0};

select distinct b.`permission_id`
  from `manager_role` a
  join `role_permission` b
    on a.`role_id` = b.`role_id`
 where a.`manager_id` = {0};")]
    Tuple<List<int>, List<int>> SelectRolesPermissionsByManagerId(int managerId);

}
