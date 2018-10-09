using System;
using System.Collections.Generic;
using System.Text;
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
            Global.MySqlExecute(fs =>
            {
                var sb = new StringBuilder();
                var ids = fs.Manager_SelectIds(999, "", "order by `id` ASC");
                //gridManagers.RecordCount = Math.Min(999, ids.dataLen);
               
                var pageIds = ids.ToGenericList().GetRange(GridView1.PageIndex * GridView1.PageSize, System.Math.Min(GridView1.PageSize, ids.dataLen));
                var tmp = new xx.List<int>();
                tmp.AddRange(pageIds);
                var ms = fs.Manager_SelectByIds<WEB.Manager>(tmp, "order by `id` ASC");
                GridView1.DataSource = ms.ToGenericList();
            });
            GridView1.DataBind();
        }


        //protected void btnHello_Click(object sender, EventArgs e)
        //{
        //    Alert.Show("你好 FineUI！", MessageBoxIcon.Warning);
        //}

    }
}
