using System;
using System.Collections.Generic;
using FineUI;

namespace web
{
    public partial class Main : System.Web.UI.Page
    {
        protected void Page_Load(object sender, EventArgs e)
        {
            if (!IsPostBack)
            {
                // todo: 校验 Ticket
                // todo: 显示的菜单从 Session 拿

                var rootMenu = (DB.MenuNode)Session[Strings.RootMenu];
                leftMenuTree.Nodes.Clear();
                var n = new TreeNode();
                leftMenuTree.Nodes.Add(n);
                n.Text = rootMenu.text;
                n.NavigateUrl = rootMenu.navUrl;
                FillTreeNode(rootMenu, n);
            }
        }

        void FillTreeNode(DB.MenuNode mn, TreeNode tn)
        {
            tn.Expanded = true;
            var mnChildsCount = mn.childs.Count;
            for (int i = 0; i < mnChildsCount; ++i)
            {
                var cmn = mn.childs[i];
                var ctn = new TreeNode();
                ctn.Text = cmn.text;
                ctn.NavigateUrl = cmn.navUrl;
                tn.Nodes.Add(ctn);
                if (cmn.childs.Count > 0)
                {
                    FillTreeNode(cmn, ctn);
                }
            }
        }

        protected void btnLogout_Click(object sender, EventArgs e)
        {
            // todo: 从在线管理员中移除自己
            Session.Clear();
            Response.Redirect(Strings.Page_Default);
        }
    }
}
