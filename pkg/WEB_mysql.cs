using System;
using xx;

namespace WEB
{
    public class MySqlFuncs
    {
        #region conn, cmd, sb, recordsAffecteds
        public MySql.Data.MySqlClient.MySqlConnection conn;
        public MySql.Data.MySqlClient.MySqlCommand cmd;
        public System.Text.StringBuilder sb = new System.Text.StringBuilder();
        private List<int> recordsAffecteds = new List<int>();

        public List<int> RecordsAffecteds
        {
            get { return recordsAffecteds; }
        }

        public int RecordsAffected
        {
            get { return recordsAffecteds[0]; }
        }

        public MySqlFuncs(MySql.Data.MySqlClient.MySqlConnection conn)
        {
            this.conn = conn;
            this.cmd = conn.CreateCommand();
        }
        #endregion


        /// <summary>
        /// 传入 where, orderBy 子句内容做查询, 返回符合条件的前 limit 行数据的 id. 传入的子句需要自带关键字
        /// </summary>
        public xx.List<int> Manager_SelectIds
        (
            int limit,
            string where,
            string orderBy
            , MySql.Data.MySqlClient.MySqlTransaction tran_ = null)
        {
            cmd.Transaction = tran_;
            recordsAffecteds.Clear();
            sb.Clear();
            sb.Append(@"
select `id`
  from `manager` ");
            sb.Append(where);
            sb.Append(@" ");
            sb.Append(orderBy);
            sb.Append(@"
 limit ");
            sb.Append(limit);
            cmd.CommandText = sb.ToString();

            var rtv = new List<int>();
            using (var r = cmd.ExecuteReader())
            {
                recordsAffecteds.Add(r.RecordsAffected);
                while (r.Read())
                {
                    rtv.Add(r.GetInt32(0));
                }
            }
            return rtv;
        }

        /// <summary>
        /// 传入 id 列表, 返回相应数据行( 可附带排序规则 )
        /// </summary>
        public List<T> Manager_SelectByIds<T>
        (
            xx.List<int> ids,
            string orderBy
            , MySql.Data.MySqlClient.MySqlTransaction tran_ = null) where T : Tables.manager, new()
        {
            cmd.Transaction = tran_;
            recordsAffecteds.Clear();
            sb.Clear();
            sb.Append(@"
select `id`, `username`, `password`
  from `manager`
 where `id` in ");
            ids.MySqlAppend(sb, false);
            sb.Append(@" ");
            sb.Append(orderBy);
            cmd.CommandText = sb.ToString();

            var rtv = new List<T>();
            using (var r = cmd.ExecuteReader())
            {
                recordsAffecteds.Add(r.RecordsAffected);
                while (r.Read())
                {
                    rtv.Add(new T
                    {
                        id = r.GetInt32(0),
                        username = r.IsDBNull(1) ? null : (string)r.GetString(1),
                        password = r.IsDBNull(2) ? null : (string)r.GetString(2)
                    });
                }
            }
            return rtv;
        }

        /// <summary>
        /// 传入 username, password, 返回 id. 如果返回 null 表示未找到.
        /// </summary>
        public int? Manager_GetIdByUsernamePassword
        (
            string username,
            string password
            , MySql.Data.MySqlClient.MySqlTransaction tran_ = null)
        {
            cmd.Transaction = tran_;
            recordsAffecteds.Clear();
            sb.Clear();
            sb.Append(@"
select `id`
  from `manager`
 where `username` = ");
            sb.Append(username == null ? "null" : ("'" + username.Replace("'", "''") + "'"));
            sb.Append(@" 
   and `password` = ");
            sb.Append(password == null ? "null" : ("'" + password.Replace("'", "''") + "'"));
            cmd.CommandText = sb.ToString();

            using (var r = cmd.ExecuteReader())
            {
                recordsAffecteds.Add(r.RecordsAffected);
                if (!r.Read())
                {
                    return default(int?);
                }
                return r.IsDBNull(0) ? null : (int?)r.GetInt32(0);
            }
        }

        /// <summary>
        /// 传入 username, password, 返回 一行 Manager 记录. 如果返回 null 表示未找到.
        /// </summary>
        public T Manager_SelectByUsernamePassword<T>
        (
            string username,
            string password
            , MySql.Data.MySqlClient.MySqlTransaction tran_ = null) where T : Tables.manager, new()
        {
            cmd.Transaction = tran_;
            recordsAffecteds.Clear();
            sb.Clear();
            sb.Append(@"
select `id`, `username`, `password`
  from `manager`
 where `username` = ");
            sb.Append(username == null ? "null" : ("'" + username.Replace("'", "''") + "'"));
            sb.Append(@" 
   and `password` = ");
            sb.Append(password == null ? "null" : ("'" + password.Replace("'", "''") + "'"));
            cmd.CommandText = sb.ToString();

            using (var r = cmd.ExecuteReader())
            {
                recordsAffecteds.Add(r.RecordsAffected);
                if (!r.Read())
                {
                    return null;
                }
                return new T
                {
                    id = r.GetInt32(0), 
                    username = r.IsDBNull(1) ? null : (string)r.GetString(1), 
                    password = r.IsDBNull(2) ? null : (string)r.GetString(2)
                };
            }
        }

        /// <summary>
        /// 插入一条. 可能因为 username 已存在而失败
        /// </summary>
        public void Manager_Insert
        (
            Tables.manager o
            , MySql.Data.MySqlClient.MySqlTransaction tran_ = null)
        {
            cmd.Transaction = tran_;
            recordsAffecteds.Clear();
            sb.Clear();
            sb.Append(@"
insert into `manager` ( `username`, `password` )
values ");
            o.MySqlAppend(sb, true);
            cmd.CommandText = sb.ToString();

            recordsAffecteds.Add(cmd.ExecuteNonQuery());
        }

        /// <summary>
        /// 改密码. 可能因 找不到 id 而失败
        /// </summary>
        public void Manager_UpdatePassword
        (
            int id,
            string newPassword
            , MySql.Data.MySqlClient.MySqlTransaction tran_ = null)
        {
            cmd.Transaction = tran_;
            recordsAffecteds.Clear();
            sb.Clear();
            sb.Append(@"update `manager` set `password` = ");
            sb.Append(newPassword == null ? "null" : ("'" + newPassword.Replace("'", "''") + "'"));
            sb.Append(@" where `id` = ");
            sb.Append(id);
            cmd.CommandText = sb.ToString();

            recordsAffecteds.Add(cmd.ExecuteNonQuery());
        }

        /// <summary>
        /// 改用户名. 可能因 找不到 id 或 新 username 已存在而失败
        /// </summary>
        public void Manager_UpdateUsername
        (
            int id,
            string newUsername
            , MySql.Data.MySqlClient.MySqlTransaction tran_ = null)
        {
            cmd.Transaction = tran_;
            recordsAffecteds.Clear();
            sb.Clear();
            sb.Append(@"
update `manager` 
   set `username` = ");
            sb.Append(newUsername == null ? "null" : ("'" + newUsername.Replace("'", "''") + "'"));
            sb.Append(@"
 where `id` = ");
            sb.Append(id);
            cmd.CommandText = sb.ToString();

            recordsAffecteds.Add(cmd.ExecuteNonQuery());
        }

        /// <summary>
        /// 删一条. 会同时删掉相关 bind
        /// </summary>
        public void Manager_Delete
        (
            int id
            , MySql.Data.MySqlClient.MySqlTransaction tran_ = null)
        {
            cmd.Transaction = tran_;
            recordsAffecteds.Clear();
            sb.Clear();
            sb.Append(@"
delete from `manager_role` where `manager_id` = ");
            sb.Append(id);
            sb.Append(@";
delete from `manager` where `id` = ");
            sb.Append(id);
            sb.Append(@"
");
            cmd.CommandText = sb.ToString();

            recordsAffecteds.Add(cmd.ExecuteNonQuery());
        }

        /// <summary>
        /// 批量重绑身份
        /// </summary>
        public void Manager_SetRoles
        (
            int id,
            xx.List<Tables.manager_role> binds
            , MySql.Data.MySqlClient.MySqlTransaction tran_ = null)
        {
            cmd.Transaction = tran_;
            recordsAffecteds.Clear();
            sb.Clear();
            sb.Append(@"
delete from `manager_role` where `manager_id` = ");
            sb.Append(id);
            sb.Append(@";

insert into `manager_role`
       ( `manager_id`, `role_id` )
values ");
            binds.MySqlAppend(sb, false);
            cmd.CommandText = sb.ToString();

            recordsAffecteds.Add(cmd.ExecuteNonQuery());
        }

        /// <summary>
        /// 读所有数据
        /// </summary>
        public List<T> Role_SelectAll<T>(MySql.Data.MySqlClient.MySqlTransaction tran_ = null) where T : Tables.role, new()
        {
            cmd.Transaction = tran_;
            cmd.CommandText = @"
select `id`, `name`, `desc`
  from `role`";
            var rtv = new List<T>();
            using (var r = cmd.ExecuteReader())
            {
                recordsAffecteds.Add(r.RecordsAffected);
                while (r.Read())
                {
                    rtv.Add(new T
                    {
                        id = r.GetInt32(0),
                        name = r.IsDBNull(1) ? null : (string)r.GetString(1),
                        desc = r.IsDBNull(2) ? null : (string)r.GetString(2)
                    });
                }
            }
            return rtv;
        }

        /// <summary>
        /// 读指定 id 列表数据. ids 必须有数据
        /// </summary>
        public List<T> Role_SelectByIds<T>
        (
            xx.List<int> ids
            , MySql.Data.MySqlClient.MySqlTransaction tran_ = null) where T : Tables.role, new()
        {
            cmd.Transaction = tran_;
            recordsAffecteds.Clear();
            sb.Clear();
            sb.Append(@"
select `id`, `name`, `desc` 
  from `role` 
 where `id` in ");
            ids.MySqlAppend(sb, false);
            cmd.CommandText = sb.ToString();

            var rtv = new List<T>();
            using (var r = cmd.ExecuteReader())
            {
                recordsAffecteds.Add(r.RecordsAffected);
                while (r.Read())
                {
                    rtv.Add(new T
                    {
                        id = r.GetInt32(0),
                        name = r.IsDBNull(1) ? null : (string)r.GetString(1),
                        desc = r.IsDBNull(2) ? null : (string)r.GetString(2)
                    });
                }
            }
            return rtv;
        }

        /// <summary>
        /// 读指定 managerId 列表数据
        /// </summary>
        public List<T> Role_SelectByManagerId<T>
        (
            int managerId
            , MySql.Data.MySqlClient.MySqlTransaction tran_ = null) where T : Tables.role, new()
        {
            cmd.Transaction = tran_;
            recordsAffecteds.Clear();
            sb.Clear();
            sb.Append(@"
select a.`id`, a.`name`, a.`desc`
  from `role` a
  join `manager_role` b
    on a.`id` = b.`role_id`
 where b.`manager_id` = ");
            sb.Append(managerId);
            cmd.CommandText = sb.ToString();

            var rtv = new List<T>();
            using (var r = cmd.ExecuteReader())
            {
                recordsAffecteds.Add(r.RecordsAffected);
                while (r.Read())
                {
                    rtv.Add(new T
                    {
                        id = r.GetInt32(0),
                        name = r.IsDBNull(1) ? null : (string)r.GetString(1),
                        desc = r.IsDBNull(2) ? null : (string)r.GetString(2)
                    });
                }
            }
            return rtv;
        }

        /// <summary>
        /// 插入一条. 可能因为 name 已存在而失败
        /// </summary>
        public void Role_Insert
        (
            Tables.manager o
            , MySql.Data.MySqlClient.MySqlTransaction tran_ = null)
        {
            cmd.Transaction = tran_;
            recordsAffecteds.Clear();
            sb.Clear();
            sb.Append(@"
insert into `role`
       ( `name`, `desc` )
values ");
            o.MySqlAppend(sb, true);
            cmd.CommandText = sb.ToString();

            recordsAffecteds.Add(cmd.ExecuteNonQuery());
        }

        /// <summary>
        /// 改名或备注. 可能因 找不到 id 或 新 name 已存在而失败
        /// </summary>
        public void Role_Update
        (
            int id,
            string newName,
            string newDesc
            , MySql.Data.MySqlClient.MySqlTransaction tran_ = null)
        {
            cmd.Transaction = tran_;
            recordsAffecteds.Clear();
            sb.Clear();
            sb.Append(@"
update `role`
   set `name` = ");
            sb.Append(newName == null ? "null" : ("'" + newName.Replace("'", "''") + "'"));
            sb.Append(@"
     , `desc` = ");
            sb.Append(newDesc == null ? "null" : ("'" + newDesc.Replace("'", "''") + "'"));
            sb.Append(@"
 where `id` = ");
            sb.Append(id);
            cmd.CommandText = sb.ToString();

            recordsAffecteds.Add(cmd.ExecuteNonQuery());
        }

        /// <summary>
        /// 删一条. 会同时删掉相关 bind
        /// </summary>
        public void Role_Delete
        (
            int id
            , MySql.Data.MySqlClient.MySqlTransaction tran_ = null)
        {
            cmd.Transaction = tran_;
            recordsAffecteds.Clear();
            sb.Clear();
            sb.Append(@"
delete from `manager_role` where `role_id` = ");
            sb.Append(id);
            sb.Append(@";
delete from `role_permission` where `role_id` = ");
            sb.Append(id);
            sb.Append(@";
delete from `role` where `id` = ");
            sb.Append(id);
            cmd.CommandText = sb.ToString();

            recordsAffecteds.Add(cmd.ExecuteNonQuery());
        }

        /// <summary>
        /// 批量重绑权限
        /// </summary>
        public void Role_SetPermissions
        (
            int id,
            xx.List<Tables.role_permission> binds
            , MySql.Data.MySqlClient.MySqlTransaction tran_ = null)
        {
            cmd.Transaction = tran_;
            recordsAffecteds.Clear();
            sb.Clear();
            sb.Append(@"
delete from `role_permission` where `role_id` = ");
            sb.Append(id);
            sb.Append(@";
insert into `role_permission` ( `role_id`, `permission_id` ) values ");
            binds.MySqlAppend(sb, false);
            cmd.CommandText = sb.ToString();

            recordsAffecteds.Add(cmd.ExecuteNonQuery());
        }

        /// <summary>
        /// 读所有数据
        /// </summary>
        public List<T> Permission_SelectAll<T>(MySql.Data.MySqlClient.MySqlTransaction tran_ = null) where T : Tables.permission, new()
        {
            cmd.Transaction = tran_;
            cmd.CommandText = @"
select `id`, `group`, `name`, `desc`
  from `permission`";
            var rtv = new List<T>();
            using (var r = cmd.ExecuteReader())
            {
                recordsAffecteds.Add(r.RecordsAffected);
                while (r.Read())
                {
                    rtv.Add(new T
                    {
                        id = r.GetInt32(0),
                        group = r.IsDBNull(1) ? null : (string)r.GetString(1),
                        name = r.IsDBNull(2) ? null : (string)r.GetString(2),
                        desc = r.IsDBNull(3) ? null : (string)r.GetString(3)
                    });
                }
            }
            return rtv;
        }

        /// <summary>
        /// 读指定 id 列表数据. ids 必须有数据
        /// </summary>
        public List<T> Permission_SelectByIds<T>
        (
            xx.List<int> ids
            , MySql.Data.MySqlClient.MySqlTransaction tran_ = null) where T : Tables.permission, new()
        {
            cmd.Transaction = tran_;
            recordsAffecteds.Clear();
            sb.Clear();
            sb.Append(@"
select `id`, `group`, `name`, `desc`
  from `permission`
 where `id` in ");
            ids.MySqlAppend(sb, false);
            cmd.CommandText = sb.ToString();

            var rtv = new List<T>();
            using (var r = cmd.ExecuteReader())
            {
                recordsAffecteds.Add(r.RecordsAffected);
                while (r.Read())
                {
                    rtv.Add(new T
                    {
                        id = r.GetInt32(0),
                        group = r.IsDBNull(1) ? null : (string)r.GetString(1),
                        name = r.IsDBNull(2) ? null : (string)r.GetString(2),
                        desc = r.IsDBNull(3) ? null : (string)r.GetString(3)
                    });
                }
            }
            return rtv;
        }

        /// <summary>
        /// 根据 managerId 获取去重后的权限列表
        /// </summary>
        public List<T> Permission_SelectByManagerId<T>
        (
            int managerId
            , MySql.Data.MySqlClient.MySqlTransaction tran_ = null) where T : Tables.permission, new()
        {
            cmd.Transaction = tran_;
            recordsAffecteds.Clear();
            sb.Clear();
            sb.Append(@"
select distinct c.`id`, c.`group`, c.`name`, c.`desc`
  from `manager_role` a
  join `role_permission` b
    on a.`role_id` = b.`role_id`
  join `permission` c
    on b.`permission_id` = c.`id`
 where a.`manager_id` = ");
            sb.Append(managerId);
            sb.Append(@"
");
            cmd.CommandText = sb.ToString();

            var rtv = new List<T>();
            using (var r = cmd.ExecuteReader())
            {
                recordsAffecteds.Add(r.RecordsAffected);
                while (r.Read())
                {
                    rtv.Add(new T
                    {
                        id = r.GetInt32(0),
                        group = r.IsDBNull(1) ? null : (string)r.GetString(1),
                        name = r.IsDBNull(2) ? null : (string)r.GetString(2),
                        desc = r.IsDBNull(3) ? null : (string)r.GetString(3)
                    });
                }
            }
            return rtv;
        }

        /// <summary>
        /// 插入一条. 可能因为 name 已存在而失败
        /// </summary>
        public void Permission_Insert
        (
            Tables.manager o
            , MySql.Data.MySqlClient.MySqlTransaction tran_ = null)
        {
            cmd.Transaction = tran_;
            recordsAffecteds.Clear();
            sb.Clear();
            sb.Append(@"
insert into `permission`
       ( `name`, `desc` )
values ");
            o.MySqlAppend(sb, true);
            cmd.CommandText = sb.ToString();

            recordsAffecteds.Add(cmd.ExecuteNonQuery());
        }

        /// <summary>
        /// 改内容. 可能因 找不到 id 或 新 name 已存在而失败
        /// </summary>
        public void Permission_Update
        (
            int id,
            string newGroup,
            string newName,
            string newDesc
            , MySql.Data.MySqlClient.MySqlTransaction tran_ = null)
        {
            cmd.Transaction = tran_;
            recordsAffecteds.Clear();
            sb.Clear();
            sb.Append(@"
update `permission`
   set `group` = ");
            sb.Append(newGroup == null ? "null" : ("'" + newGroup.Replace("'", "''") + "'"));
            sb.Append(@"
     , `name` = ");
            sb.Append(newName == null ? "null" : ("'" + newName.Replace("'", "''") + "'"));
            sb.Append(@"
     , `desc` = ");
            sb.Append(newDesc == null ? "null" : ("'" + newDesc.Replace("'", "''") + "'"));
            sb.Append(@"
 where `id` = ");
            sb.Append(id);
            cmd.CommandText = sb.ToString();

            recordsAffecteds.Add(cmd.ExecuteNonQuery());
        }

        /// <summary>
        /// 删一条. 会同时删掉相关 bind
        /// </summary>
        public void Permission_Delete
        (
            int id
            , MySql.Data.MySqlClient.MySqlTransaction tran_ = null)
        {
            cmd.Transaction = tran_;
            recordsAffecteds.Clear();
            sb.Clear();
            sb.Append(@"
delete from `permission` where `id` = ");
            sb.Append(id);
            cmd.CommandText = sb.ToString();

            recordsAffecteds.Add(cmd.ExecuteNonQuery());
        }

        /// <summary>
        /// 根据 managerId 获取 身份id列表 以及 去重后的 权限id列表
        /// </summary>
        public Tuple <List<int>, List<int>> SelectRolesPermissionsByManagerId
        (
            int managerId
            , MySql.Data.MySqlClient.MySqlTransaction tran_ = null)
        {
            cmd.Transaction = tran_;
            recordsAffecteds.Clear();
            sb.Clear();
            sb.Append(@"
select `role_id`
  from `manager_role`
 where `manager_id` = ");
            sb.Append(managerId);
            sb.Append(@";

select distinct b.`permission_id`
  from `manager_role` a
  join `role_permission` b
    on a.`role_id` = b.`role_id`
 where a.`manager_id` = ");
            sb.Append(managerId);
            sb.Append(@";");
            cmd.CommandText = sb.ToString();

            using (var r = cmd.ExecuteReader())
            {
                recordsAffecteds.Add(r.RecordsAffected);
                recordsAffecteds.Add(r.RecordsAffected);
                var rtv1 = new List<int>();
                while (r.Read())
                {
                    rtv1.Add(r.GetInt32(0));
                }
                r.NextResult();
                recordsAffecteds.Add(r.RecordsAffected);
                var rtv2 = new List<int>();
                while (r.Read())
                {
                    rtv2.Add(r.GetInt32(0));
                }
                return new Tuple <List<int>, List<int>>(rtv1, rtv2);
            }
        }
    }
}
