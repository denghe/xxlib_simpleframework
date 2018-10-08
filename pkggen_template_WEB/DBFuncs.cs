#pragma warning disable 0169, 0414
using TemplateLibrary;

[MySql]
partial interface MySqlFuncs
{
    [Desc(@"查 manager 表所有数据")]
    [Sql(@"
select `id`, `username`, `password`
  from `manager`")]
    List<Tables.manager> manager__SelectAll();

    [Desc(@"往 manager 表插入单条数据")]
    [Sql(@"
insert into `manager` (`username`, `password`)
values ({0}, {1})")]
    void manager__Insert(string username, string password);

    [Desc(@"往 manager 表插入单条数据对象")]
    [Sql(@"
insert into `manager` (`username`, `password`)
values {0}")]
    void manager__Insert([SkipReadOnly] Tables.manager row);

    [Desc(@"往 manager 表插入多条数据对象")]
    [Sql(@"
insert into `manager` (`username`, `password`)
values {0}")]
    void manager__Insert([SkipReadOnly] List<Tables.manager> rows);

    [Desc(@"根据主键查 manager 表指定数据行")]
    [Sql(@"
select `username`, `password`
  from `manager`
 where `id` = 0")]
    Tables.manager manager__SelectByPrimaryKey(int id);

    [Desc(@"查 manager_role 表所有数据")]
    [Sql(@"
select `manager_id`, `role_id`
  from `manager_role`")]
    List<Tables.manager_role> manager_role__SelectAll();

    [Desc(@"往 manager_role 表插入单条数据")]
    [Sql(@"
insert into `manager_role` (`manager_id`, `role_id`)
values ({0}, {1})")]
    void manager_role__Insert(int manager_id, int role_id);

    [Desc(@"往 manager_role 表插入单条数据对象")]
    [Sql(@"
insert into `manager_role` (`manager_id`, `role_id`)
values {0}")]
    void manager_role__Insert([SkipReadOnly] Tables.manager_role row);

    [Desc(@"往 manager_role 表插入多条数据对象")]
    [Sql(@"
insert into `manager_role` (`manager_id`, `role_id`)
values {0}")]
    void manager_role__Insert([SkipReadOnly] List<Tables.manager_role> rows);

    [Desc(@"根据主键查 manager_role 表指定数据行")]
    [Sql(@"
select `manager_id`, `role_id`
  from `manager_role`
 where `manager_id` = 0 and `role_id` = 1")]
    Tables.manager_role manager_role__SelectByPrimaryKey(int manager_id, int role_id);

    [Desc(@"查 permission 表所有数据")]
    [Sql(@"
select `id`, `group`, `name`, `desc`
  from `permission`")]
    List<Tables.permission> permission__SelectAll();

    [Desc(@"往 permission 表插入单条数据")]
    [Sql(@"
insert into `permission` (`id`, `group`, `name`, `desc`)
values ({0}, {1}, {2}, {3})")]
    void permission__Insert(int id, string group, string name, string desc);

    [Desc(@"往 permission 表插入单条数据对象")]
    [Sql(@"
insert into `permission` (`id`, `group`, `name`, `desc`)
values {0}")]
    void permission__Insert([SkipReadOnly] Tables.permission row);

    [Desc(@"往 permission 表插入多条数据对象")]
    [Sql(@"
insert into `permission` (`id`, `group`, `name`, `desc`)
values {0}")]
    void permission__Insert([SkipReadOnly] List<Tables.permission> rows);

    [Desc(@"根据主键查 permission 表指定数据行")]
    [Sql(@"
select `id`, `group`, `name`, `desc`
  from `permission`
 where `id` = 0")]
    Tables.permission permission__SelectByPrimaryKey(int id);

    [Desc(@"查 role 表所有数据")]
    [Sql(@"
select `id`, `name`, `desc`
  from `role`")]
    List<Tables.role> role__SelectAll();

    [Desc(@"往 role 表插入单条数据")]
    [Sql(@"
insert into `role` (`name`, `desc`)
values ({0}, {1})")]
    void role__Insert(string name, string desc);

    [Desc(@"往 role 表插入单条数据对象")]
    [Sql(@"
insert into `role` (`name`, `desc`)
values {0}")]
    void role__Insert([SkipReadOnly] Tables.role row);

    [Desc(@"往 role 表插入多条数据对象")]
    [Sql(@"
insert into `role` (`name`, `desc`)
values {0}")]
    void role__Insert([SkipReadOnly] List<Tables.role> rows);

    [Desc(@"根据主键查 role 表指定数据行")]
    [Sql(@"
select `name`, `desc`
  from `role`
 where `id` = 0")]
    Tables.role role__SelectByPrimaryKey(int id);

    [Desc(@"查 role_permission 表所有数据")]
    [Sql(@"
select `role_id`, `permission_id`
  from `role_permission`")]
    List<Tables.role_permission> role_permission__SelectAll();

    [Desc(@"往 role_permission 表插入单条数据")]
    [Sql(@"
insert into `role_permission` (`role_id`, `permission_id`)
values ({0}, {1})")]
    void role_permission__Insert(int role_id, int permission_id);

    [Desc(@"往 role_permission 表插入单条数据对象")]
    [Sql(@"
insert into `role_permission` (`role_id`, `permission_id`)
values {0}")]
    void role_permission__Insert([SkipReadOnly] Tables.role_permission row);

    [Desc(@"往 role_permission 表插入多条数据对象")]
    [Sql(@"
insert into `role_permission` (`role_id`, `permission_id`)
values {0}")]
    void role_permission__Insert([SkipReadOnly] List<Tables.role_permission> rows);

    [Desc(@"根据主键查 role_permission 表指定数据行")]
    [Sql(@"
select `role_id`, `permission_id`
  from `role_permission`
 where `role_id` = 0 and `permission_id` = 1")]
    Tables.role_permission role_permission__SelectByPrimaryKey(int role_id, int permission_id);

}
