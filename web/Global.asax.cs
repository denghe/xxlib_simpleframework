using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading;
using System.Web;
using System.Web.Configuration;
using System.Web.Security;
using System.Web.SessionState;
using MySql.Data.MySqlClient;

namespace web
{
    public static class Strings
    {
        public const string Page_Default = "~/Default.aspx";
        public const string CaptchaImageText = "CaptchaImageText";
        public const string SqlConnectionString = "mysql";
        public const string AdminUsername = "admin";
        public const string AdminPassword = "admin";
        public const string Token = "token";
        public const string RootMenu = "menu";

        // todo: more keys here
    }


    public class Global : HttpApplication
    {
        /// <summary>
        /// 在线用户管理
        /// </summary>
        public static WEB.OnlineManagers onlineManagers = new WEB.OnlineManagers();

        /// <summary>
        /// 全局随机量( 线程访问时创建 )
        /// </summary>
        public static ThreadLocal<Random> random = new ThreadLocal<Random>(() => { return new Random(); });

        /// <summary>
        /// MSSQL 的数据连接串( 网站启动时加载 )
        /// </summary>
        public static string sqlConnStr = WebConfigurationManager.ConnectionStrings[Strings.SqlConnectionString].ConnectionString;

        /// <summary>
        /// 根菜单( 网站启动时加载 )
        /// </summary>
        public static WEB.MenuNode rootMenu = new WEB.MenuNode(null, "主选单", "", null);

        /// <summary>
        /// 所有菜单( 网站启动时加载 )
        /// </summary>
        public static List<WEB.MenuNode> allMenu = new List<WEB.MenuNode>();

        /// <summary>
        /// 所有权限( 网站启动时加载 )
        /// </summary>
        public static xx.DictEx<WEB.Permission, int, string> allPermissions = new xx.DictEx<WEB.Permission, int, string>();

        /// <summary>
        /// 所有身份( 网站启动时加载 )
        /// </summary>
        public static xx.DictEx<WEB.Role, int, string> allRoles = new xx.DictEx<WEB.Role, int, string>();


        /// <summary>
        /// 根据连接串来创建一个连接并返回
        /// </summary>
        /// <returns></returns>
        public static MySqlConnection CreateMsSqlConnection()
        {
            return new MySqlConnection(sqlConnStr);
        }

        /// <summary>
        /// 以 lambda 方式执行一个需要数据库连接的函数
        /// </summary>
        public static void MySqlExecute(Action<WEB.MySqlFuncs> a)
        {
            using (var conn = CreateMsSqlConnection())
            {
                conn.Open();
                var fs = new WEB.MySqlFuncs(conn);
                a(fs);
            }
        }

        /// <summary>
        /// 获取 CAPTCHA 生成链接
        /// </summary>
        public static string GetCaptchaImageUrl()
        {
            return "~/Captcha.ashx?w=150&h=30&t=" + Guid.NewGuid();
        }


        // 每个模块 page load 里应该检查 Session[Ticket] 是否为空, 是否位于 onlineManagers, 定位到之后, 进一步取出模块子树, 
        // 模块检测自己是否位于树中, 并将相应的属于当前模块的 权限List 放置于当前模块变量, 以供具体的功能 / 事件函数进一步判断
        // 上述步骤任何一步失败, 都可视作是校验失败, 直接退出或跳到登录之类都是可行的.
        // 对于一些复杂的, 计划性的, 耗时的任务, 如果因为一些意外情况跳到登录, 状态不可恢复, 对于使用者来讲可能无法接受, 
        // 这些模块应该特殊设计, 将操作步骤持久化存储下来, 或形成一些 task, 可通过正常途径恢复操作, 或是查看结果之类


        /// <summary>
        /// 根据个人权限过滤菜单项, 最后返回根节点
        /// </summary>
        public static WEB.MenuNode GetManagerMenu(WEB.Manager m)
        {
            // 递归生成个人菜单项
            void Fill(WEB.MenuNode mn, WEB.MenuNode tn)
            {
                var mnChildsCount = mn.childs.Count;
                for (int i = 0; i < mnChildsCount; ++i)
                {
                    var cmn = mn.childs[i];
                    var ctn = new WEB.MenuNode(null, cmn.text, cmn.navUrl, m.permissions.GetByIds(cmn.permissionIds))
                    {
                        parent = tn             // 并不急着加进 parent 的子, 故传入 null, 在外面赋值
                    };
                    if (cmn.childs.Count > 0)
                    {
                        Fill(cmn, ctn);
                    }
                    if (ctn.childs.Count > 0 || ctn.permissions != null && ctn.permissions.Count > 0)
                    {
                        tn.childs.Add(ctn);     // 有效的子才放入 parent 的 childs
                    }
                }
            }
            var rtv = new WEB.MenuNode(null, rootMenu.text, rootMenu.navUrl, null);
            Fill(rootMenu, rtv);
            return rtv;
        }



        protected void Application_Start(object sender, EventArgs e)
        {
            WEB.AllTypes.Register();

            // 连接数据库, 读取所有常驻数据, 填充多键字典( 后期只读 )
            MySqlExecute(fs =>
            {
                allPermissions.AddRange(fs.Permission_SelectAll<WEB.Permission>());
                allRoles.AddRange(fs.Role_SelectAll<WEB.Role>());
            });

            // 在这里构造与网站页面对应的完整菜单
            // 权限列表可以放到具体页面对应的类的静态成员中以便直接修改
            var menuHello = NewMenu(rootMenu, "hello", "~/hello.aspx", allPermissions.GetByNames(hello.permissions));
            var menuLogin = NewMenu(rootMenu, "managers", "~/admin/Managers.aspx", allPermissions.GetByNames(admin.Managers.permissions));
            var menuNode1 = NewMenu(rootMenu, "node1");
            var menuNode1_1 = NewMenu(menuNode1, "node1_1", "", allPermissions.GetByNames("p4"));
            var menuNode1_2 = NewMenu(menuNode1, "node1_2", "", allPermissions.GetByNames("p5"));
        }

        /// <summary>
        /// 创建一个菜单( 菜单本身会在创建时放入 parent ), 并放入 allMenu 容器
        /// </summary>
        public static WEB.MenuNode NewMenu(WEB.MenuNode parent, string text, string navUrl = "", List<WEB.Permission> permissions = null)
        {
            var n = new WEB.MenuNode(parent, text, navUrl, permissions);
            allMenu.Add(n);
            if (permissions != null)
            {
                foreach (var p in permissions)
                {
                    p.menuNode = n;
                }
            }
            return n;
        }


        #region unused

        protected void Session_Start(object sender, EventArgs e)
        {
        }

        protected void Application_BeginRequest(object sender, EventArgs e)
        {
        }

        protected void Application_AuthenticateRequest(object sender, EventArgs e)
        {
        }

        protected void Application_Error(object sender, EventArgs e)
        {
        }

        protected void Session_End(object sender, EventArgs e)
        {
        }

        protected void Application_End(object sender, EventArgs e)
        {
        }

        #endregion
    }
}
