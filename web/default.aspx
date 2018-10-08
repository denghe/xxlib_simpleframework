<%@ Page Language="C#" AutoEventWireup="true" CodeBehind="Default.aspx.cs" Inherits="web.Default" %>

<!DOCTYPE html>
<html xmlns="http://www.w3.org/1999/xhtml">
<head runat="server">
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <title>系统登陆</title>
    <script type="text/javascript">
        if (top.window != window) {
            top.window.location.href = "./Default.aspx";
        }
        else if (window.location.href.indexOf('/Default.aspx') < 0) {
            window.location.href = "./Default.aspx";
        }
    </script>
</head>
<body>
    <form id="form1" runat="server">
        <f:PageManager ID="PageManager1" runat="server"></f:PageManager>
        <f:Window ID="Window1" runat="server" IsModal="false" Hidden="false" EnableClose="false"
            EnableMaximize="false" WindowPosition="GoldenSection" Icon="Key" Title="系统登陆"
            Layout="HBox" BoxConfigAlign="Stretch" BoxConfigPosition="Start" Width="500px">
            <Items>
                <f:Image Margin="15px" ID="imageLogin" ImageWidth="122" ImageHeight="122" ImageUrl="~/res/images/login_2.png" runat="server"
                    CssClass="login-image">
                </f:Image>
                <f:SimpleForm ID="SimpleForm1" LabelAlign="Top" BoxFlex="1" runat="server"
                    BodyPadding="30px 20px" ShowBorder="false" ShowHeader="false">
                    <Items>
                        <f:TextBox ID="tbxUserName" FocusOnPageLoad="true" runat="server" Label="用户名" Required="true" ShowRedStar="true" Text="" />
                        <f:TextBox ID="tbxPassword" TextMode="Password" runat="server" Required="true" ShowRedStar="true" Label="密码" Text="" />
                        <f:TextBox ID="tbxCaptcha" runat="server" Label="验证码" Required="true" ShowRedStar="true" Text="" />
                        <f:Panel ShowBorder="false" ShowHeader="false" runat="server">
                            <Items>
                                <f:Image ID="imgCaptcha" CssStyle="float:left;width:160px;" runat="server" />
                                <f:LinkButton CssStyle="float:left;margin-top:8px;" ID="btnRefresh" Text="看不清？" runat="server" OnClick="btnRefresh_Click" />
                            </Items>
                        </f:Panel>
                    </Items>
                </f:SimpleForm>
            </Items>
            <Toolbars>
                <f:Toolbar runat="server" Position="Bottom">
                    <Items>
                        <f:ToolbarText runat="server" Text="管理员账号: a/a"></f:ToolbarText>
                        <f:ToolbarFill runat="server"></f:ToolbarFill>
                        <f:Button ID="btnSubmit" Icon="LockOpen" Type="Submit" runat="server" ValidateForms="SimpleForm1" OnClick="btnSubmit_Click" Text="登陆">
                        </f:Button>
                    </Items>
                </f:Toolbar>
            </Toolbars>
        </f:Window>
    </form>

</body>
</html>
