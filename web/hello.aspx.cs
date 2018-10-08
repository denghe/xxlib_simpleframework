using System;
using System.Collections.Generic;
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
            // todo: session ckeck
        }

        protected void btnHello_Click(object sender, EventArgs e)
        {
            Alert.Show("你好 FineUI！", MessageBoxIcon.Warning);
        }

    }
}
