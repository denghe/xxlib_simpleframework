<%@ Page Language="C#" AutoEventWireup="true" CodeBehind="Managers.aspx.cs" Inherits="web.admin.Managers" %>

<!DOCTYPE html>

<html xmlns="http://www.w3.org/1999/xhtml">
<head runat="server">
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <title></title>
</head>
<body>
    <form id="form1" runat="server">
        <f:PageManager ID="PageManager1" runat="server" />
        <f:Button ID="btnSearch" Text="Search" OnClick="btnSearch_Click" runat="server" />
        <f:Grid ID="gridManagers" runat="server" Title="管理员列表" EnableCollapse="true" Width="520px" ShowBorder="true" ShowHeader="true"
            AllowPaging="true" IsDatabasePaging="true" OnPageIndexChange="gridManagers_PageIndexChange" PageSize="2"
            AllowSorting="true" SortField="id" SortDirection="ASC" OnSort="gridManagers_Sort"
            OnRowSelect="gridManagers_RowSelect" EnableRowSelectEvent="true" EnableCheckBoxSelect="false"
            DataKeyNames="id" >
            <Columns>
                <f:BoundField Width="50px" DataField="_id" SortField="id" DataFormatString="{0}" HeaderText="id" />
                <f:BoundField Width="200px" DataField="_username" SortField="username" HeaderText="登录名" />
                <f:BoundField Width="200px" DataField="_password" SortField="password" HeaderText="密码" />
            </Columns>
        </f:Grid>
    </form>
</body>
</html>
