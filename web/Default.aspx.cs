using FineUI;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;

namespace web
{
    public partial class Default : System.Web.UI.Page
    {
        protected void Page_Load(object sender, EventArgs e)
        {
            if (!IsPostBack)
            {
                btnRefresh_Click();                             // 首次加载生成验证码
            }
        }

        /// <summary>
        /// 验证码 刷新按钮 点击事件
        /// </summary>
        protected void btnRefresh_Click(object sender = null, EventArgs e = null)
        {
            // 创建一个 6 位的随机数并保存在 Session 对象中
            Session[Strings.CaptchaImageText] = Global.random.Value.Next(0, 1000000).ToString("D6");
            imgCaptcha.ImageUrl = Global.GetCaptchaImageUrl();
        }

        /// <summary>
        /// 登录按钮 点击事件
        /// </summary>
        protected void btnSubmit_Click(object sender, EventArgs e)
        {
            //if (tbxCaptcha.Text != (string)Session[Strings.CaptchaImageText])
            //{
            //    Alert.ShowInTop("验证码错误！");
            //    btnRefresh_Click();                             // 更换验证码
            //    return;
            //}

            // 去数据库中对比用户名和密码. 如果校验通过, m 将填充从数据库中读入的数据
            WEB.Manager m = null;
            Global.MySqlExecute(fs =>
            {
                m = fs.Manager_SelectByUsernamePassword<WEB.Manager>(tbxUserName.Text.Trim(), tbxPassword.Text);
            });
            if (m == null)
            {
                Alert.ShowInTop("用户名或密码错误！", MessageBoxIcon.Error);
                return;
            }

            // 继续向 m 填充本次生成的令牌( 放入在线管理员容器前需要填充, 以便判断是否踢人 )
            m.token = Guid.NewGuid().ToString();

            // 试着放入在线管理员容器
            var rtv = Global.onlineManagers.TryAdd(m);

            if (rtv == m)                           // 如果是新上线, 进一步取相关数据并填充
            {
                Global.MySqlExecute(fs =>
                {
                    var sets = fs.SelectRolesPermissionsByManagerId(m.id);
                    var count = sets.Item1.dataLen;
                    for (int i = 0; i < count; ++i)
                    {
                        var role = Global.allRoles.GetValue0(sets.Item1[i]);
                        m.roles.Add(role, role.id, role.name);
                    }
                    count = sets.Item2.dataLen;
                    for (int i = 0; i < count; ++i)
                    {
                        var perm = Global.allPermissions.GetValue0(sets.Item2[i]);
                        m.permissions.Add(perm, perm.id, perm.name);
                    }
                });

                // todo: 填充 ip, 浏览器 啥的信息?
            }
            else if (m != null)                     // 已在线, 顶下线
            {
                // todo: 更新 ip, 浏览器 啥的信息?
            }
            else
            {
                return;
            }

            // 填充本次生成的 token
            // 每个页面一开始都用 Session[SessionKeys.Token] 在 Global.onlineManagers 中定位自己
            Session[Strings.Token] = m.token;

            // 根据 permissions 来过滤 Global.rootMenu, 形成私人菜单
            Session[Strings.RootMenu] = Global.GetManagerMenu(m);

            // 导航到首页
            Response.Redirect("~/Main.aspx");
        }

    }
}
