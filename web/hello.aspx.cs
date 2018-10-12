using System;
using System.Collections.Generic;
using System.Text;
using System.Threading;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;
using FineUI;

namespace web
{
    public partial class hello : System.Web.UI.Page
    {
        /// <summary>
        /// 权限列表
        /// </summary>
        public static string[] permissions = new string[] { "p1", "p2" };

        protected void Page_Load(object sender, EventArgs e)
        {
        }

        Global.Client client = new Global.Client();

        protected void btn1_Click(object sender, EventArgs e)
        {
            var o = client.SendRequest(new WEB.WEB_testcpp3.Cmd1 { id = 123 });
            HnadlePkg(o);
        }

        protected void btn2_Click(object sender, EventArgs e)
        {
            var o = client.SendRequest(new WEB.WEB_testcpp3.Cmd2 { id = 123 });
            HnadlePkg(o);
        }

        protected void btn3_Click(object sender, EventArgs e)
        {
            var o = client.SendRequest(new xx.BBuffer());
            HnadlePkg(o);
        }

        void HnadlePkg(xx.IObject o_)
        {
            switch (o_)
            {
                case WEB.Generic.Success o:
                    Alert.Show("收到成功消息!", MessageBoxIcon.Warning);
                    break;

                case WEB.Generic.Error o:
                    Alert.Show("收到错误包:" + o.ToString(), MessageBoxIcon.Warning);
                    break;
                case null:
                    Alert.Show("连接失败或超时!", MessageBoxIcon.Warning);
                    break;
                default:
                    Alert.Show("收到未处理包: " + o_.ToString(), MessageBoxIcon.Warning);
                    break;
            }
        }

    }
}
