<%@ Page Language="C#" AutoEventWireup="true" CodeBehind="Main.aspx.cs" Inherits="web.Main" %>

<!DOCTYPE html>
<html xmlns="http://www.w3.org/1999/xhtml">
<head runat="server">
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <title>web</title>
</head>
<body>
    <form id="form1" runat="server">
        <f:PageManager ID="PageManager1" AutoSizePanelID="RegionPanel1" runat="server"></f:PageManager>
        <f:RegionPanel ID="RegionPanel1" ShowBorder="false" runat="server">
            <Regions>
                <f:Region ID="Region1" ShowBorder="false" Height="36px" ShowHeader="false"
                    Position="Top" Layout="Fit" runat="server">
                    <Toolbars>
                        <f:Toolbar runat="server">
                            <Items>
                                <f:ToolbarText runat="server" Text="标题" />
                                <f:ToolbarFill runat="server" />
                                <f:Button runat="server" ID="btnLogout" OnClick="btnLogout_Click" Text="Logout" />
                            </Items>
                        </f:Toolbar>
                    </Toolbars>
                </f:Region>
                <f:Region ID="Region2" Split="true" Width="200px" ShowHeader="true" Title="菜单"
                    EnableCollapse="true" Layout="Fit" Position="Left" runat="server">
                    <Items>
                        <f:Tree runat="server" ShowBorder="false" ShowHeader="false" EnableArrows="true" EnableLines="true" ID="leftMenuTree">
                            <Nodes>
                                <f:TreeNode Text="默认分类" Expanded="true">
                                    <f:TreeNode Text="开始页面" NavigateUrl="~/hello.aspx"></f:TreeNode>
                                    <f:TreeNode Text="登录页面" NavigateUrl="~/login.aspx"></f:TreeNode>
                                </f:TreeNode>
                            </Nodes>
                        </f:Tree>
                    </Items>
                </f:Region>
                <f:Region ID="mainRegion" ShowHeader="false" Layout="Fit" Position="Center"
                    runat="server">
                    <Items>
                        <f:TabStrip ID="mainTabStrip" EnableTabCloseMenu="true" ShowBorder="false" runat="server">
                            <Tabs>
                                <f:Tab ID="Tab1" Title="首页" Layout="Fit" Icon="House" runat="server">
                                    <Items>
                                        <f:ContentPanel ID="ContentPanel2" ShowBorder="false" BodyPadding="10px" ShowHeader="false" AutoScroll="true"
                                            runat="server">
                                            fineui 控件示例在 http://demo.fineui.com
                                        </f:ContentPanel>
                                    </Items>
                                </f:Tab>
                            </Tabs>
                        </f:TabStrip>
                    </Items>
                </f:Region>
            </Regions>
        </f:RegionPanel>
    </form>
    <script>
        var menuClientID = '<%= leftMenuTree.ClientID %>';
        var tabStripClientID = '<%= mainTabStrip.ClientID %>';

        // 页面控件初始化完毕后，会调用用户自定义的onReady函数
        F.ready(function () {

            var treeMenu = F(menuClientID);
            var mainTabStrip = F(tabStripClientID);
<%
// 初始化主框架中的树(或者Accordion+Tree)和选项卡互动，以及地址栏的更新
// treeMenu： 主框架中的树控件实例，或者内嵌树控件的手风琴控件实例
// mainTabStrip： 选项卡实例
// updateHash: 切换Tab时，是否更新地址栏Hash值（默认值：true）
// refreshWhenExist： 添加选项卡时，如果选项卡已经存在，是否刷新内部IFrame（默认值：false）
// refreshWhenTabChange: 切换选项卡时，是否刷新内部IFrame（默认值：false）
// maxTabCount: 最大允许打开的选项卡数量
// maxTabMessage: 超过最大允许打开选项卡数量时的提示信息
        %>
            F.initTreeTabStrip(treeMenu, mainTabStrip, {
                maxTabCount: 20,
                maxTabMessage: '请先关闭一些选项卡（最多允许打开 20 个）！'
            });

        });
    </script>
</body>
</html>
