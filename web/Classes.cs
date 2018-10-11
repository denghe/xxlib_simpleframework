using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

// 手工附加部分

namespace WEB
{
    /// <summary>
    /// 主菜单--网站节点( 这个类和数据库无关. 放在这, 以便于其他类引用 )
    /// </summary>
    public class MenuNode
    {
        public MenuNode(MenuNode parent, string text, string navUrl, List<Permission> permissions)
        {
            this.parent = parent;
            this.text = text;
            this.navUrl = navUrl;
            this.permissions = permissions;
            this.permissionIds = permissions == null ? null : permissions.Select(p => p.id).ToArray();
            if (parent != null)
            {
                parent.childs.Add(this);
            }
        }
        public MenuNode parent;                                // 这个为空表示根
        public List<MenuNode> childs = new List<MenuNode>();   // 这个.Counbt为 0 表示叶子
        public string text;
        public string navUrl;
        public List<Permission> permissions;
        public int[] permissionIds;
    }


    public partial class Permission
    {
        /// <summary>
        /// 附加指向菜单的 field
        /// </summary>
        public MenuNode menuNode;
    }


    public partial class OnlineManagers
    {
        /// <summary>
        /// 所有在线管理员信息集合. key 分别为 id, username, token
        /// </summary>
        xx.DictEx<Manager, int, string, string> onlineManagers = new xx.DictEx<Manager, int, string, string>();

        /// <summary>
        /// 传入只填有 keys 的 Manager, 看看能否 Add 成功
        /// 如果相同 id 的 manager 已存在, 将替换其 token 并返回已存在的 manager
        /// 如果不存在, 则试着 Add. 如果 Add 成功, 返回传入的 m. 否则返回 null
        /// </summary>
        public Manager TryAdd(Manager m)
        {
            lock (this)
            {
                var idx = onlineManagers.Find0(m.id);
                if (idx != -1)
                {
                    var om = onlineManagers.ValueAt(idx);
                    return onlineManagers.UpdateAt2(idx, m.token) ? om : null;
                }
                else
                {
                    return onlineManagers.Add(m, m.id, m.username, m.token) ? m : null;
                }
            }
        }

        public void Remove(Manager m)
        {
            lock (this)
            {
                onlineManagers.Remove0(m.id);
            }
        }

        public Manager FindById(int id)
        {
            lock (this)
            {
                var idx = onlineManagers.Find0(id);
                return idx != -1 ? onlineManagers.ValueAt(idx) : null;
            }
        }
        public Manager FindByUsername(string username)
        {
            lock (this)
            {
                var idx = onlineManagers.Find1(username);
                return idx != -1 ? onlineManagers.ValueAt(idx) : null;
            }
        }


        public Manager FindByToken(string token)
        {
            lock (this)
            {
                var idx = onlineManagers.Find2(token);
                return idx != -1 ? onlineManagers.ValueAt(idx) : null;
            }
        }

        public List<Manager> GetOnlineManagers()
        {
            lock (this)
            {
                var rtv = new List<Manager>();
                onlineManagers.ForEach(m =>
                {
                    rtv.Add(m);
                });
                return rtv;
            }
        }

    }

    public partial class Manager
    {
        /// <summary>
        /// 管理员在登录之后, 从数据库加载当前具备的 所有权限 塞入这个多主键字典备查.
        /// key 为 id, name
        /// </summary>
        public xx.DictEx<Permission, int, string> permissions = new xx.DictEx<Permission, int, string>();

        /// <summary>
        /// 管理员在登录之后, 从数据库加载当前具备的 所有身份 塞入这个多主键字典备查
        /// key 为 id, name
        /// </summary>
        public xx.DictEx<Role, int, string> roles = new xx.DictEx<Role, int, string>();


        // 数据 bind 需要 property, 故附加一些
        public int _id
        {
            get { return id; }
            set { id = value; }
        }
        public string _username
        {
            get { return username; }
            set { username = value; }
        }
        public string _password
        {
            get { return password; }
            set { password = value; }
        }
    }

}




public static class Exts
{
    public static List<WEB.Permission> GetByIds(this xx.DictEx<WEB.Permission, int, string> self, params int[] keys)
    {
        if (keys == null) return null;
        var rtv = new List<WEB.Permission>();
        var keysLength = keys.Length;
        for (int i = 0; i < keysLength; ++i)
        {
            var idx = self.Find0(keys[i]);
            if (idx != -1)
            {
                rtv.Add(self.ValueAt(idx));
            }
        }
        return rtv;
    }

    public static List<WEB.Permission> GetByNames(this xx.DictEx<WEB.Permission, int, string> self, params string[] keys)
    {
        if (keys == null) return null;
        var rtv = new List<WEB.Permission>();
        var keysLength = keys.Length;
        for (int i = 0; i < keysLength; ++i)
        {
            var idx = self.Find1(keys[i]);
            if (idx != -1)
            {
                rtv.Add(self.ValueAt(idx));
            }
        }
        return rtv;
    }

    public static void AddRange(this xx.DictEx<WEB.Permission, int, string> self, xx.List<WEB.Permission> os)
    {
        var size = os.dataLen;
        if (size <= 0) return;

        for (int i = 0; i < size; i++)
        {
            var item = os[i];
            self.Add(item, item.id, item.name);
        }
    }

    public static void AddRange(this xx.DictEx<WEB.Role, int, string> self, xx.List<WEB.Role> os)
    {
        var size = os.dataLen;
        if (size <= 0) return;

        for (int i = 0; i < size; i++)
        {
            var item = os[i];
            self.Add(item, item.id, item.name);
        }
    }

    // 用于向 ViewState 写可序列化对象
    public static void SetValue(this System.Web.UI.StateBag self, string key, xx.Object value)
    {
        var bb = new xx.BBuffer();
        bb.WriteRoot(value);
        self[key] = Convert.ToBase64String(bb.buf, 0, bb.dataLen);
    }

    // 用于从 ViewState 还原出可序列化对象
    public static xx.Object GetValue(this System.Web.UI.StateBag self, string key)
    {
        var str = (string)self[key];
        if (str == null) return null;
        var bb = new xx.BBuffer();
        bb.buf = Convert.FromBase64String(str);
        bb.dataLen = bb.buf.Length;
        xx.Object rtv = null;
        bb.ReadRoot<xx.Object>(ref rtv);
        return rtv;
    }
}
