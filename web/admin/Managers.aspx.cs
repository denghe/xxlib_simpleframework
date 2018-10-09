using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;
using System.Text;

namespace web.admin
{
    public partial class Managers : System.Web.UI.Page
    {
        /// <summary>
        /// 权限列表
        /// </summary>
        public static string[] permissions = new string[] { "p1", "p2" };

        // todo: 
        // 页面操作流程为: 
        // 先显示查询条件 form. 点击 查询 之后, 下方出现 Grid. 随后基于 Grid 的一切操作, 均针对本次查询之结果
        // Grid 头部可显示一些统计信息, 诸如符合条件的总行数( 不精确值, 999 就显示 999+ )
        // 查询结果为最多 999 个 id 

        protected void Page_Load(object sender, EventArgs e)
        {
            if (!IsPostBack)
            {
            }
        }

        void Search()
        {
            Global.MySqlExecute(fs =>
            {
                var sb = new StringBuilder();
                var ids = fs.Manager_SelectIds(999, "", "order by [" + gridManagers.SortField + "] " + gridManagers.SortDirection);
                gridManagers.RecordCount = Math.Min(999, ids.dataLen);
                ViewState["ids"] = ids;
            });
        }
        void Bind()
        {
            Global.MySqlExecute(fs =>
            {
                var ids = (List<int>)ViewState["ids"];
                var pageIds = ids.GetRange(gridManagers.PageIndex * gridManagers.PageSize, gridManagers.PageSize);
                var tmp = new xx.List<int>();
                tmp.AddRange(pageIds);
                var ms = fs.Manager_SelectByIds<WEB.Manager>(tmp, "order by [" + gridManagers.SortField + "] " + gridManagers.SortDirection);
                gridManagers.DataSource = ms;
            });
            gridManagers.DataBind();
        }

        protected void gridManagers_PageIndexChange(object sender, FineUI.GridPageEventArgs e)
        {
            Bind();
        }

        protected void gridManagers_Sort(object sender, FineUI.GridSortEventArgs e)
        {
            Search();
            Bind();
        }

        protected void btnSearch_Click(object sender, EventArgs e)
        {
            Search();
            Bind();
        }

        protected void gridManagers_RowSelect(object sender, FineUI.GridRowSelectEventArgs e)
        {

        }
    }
}
