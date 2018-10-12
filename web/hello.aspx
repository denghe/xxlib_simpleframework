<%@ Page Language="C#" AutoEventWireup="True" CodeBehind="hello.aspx.cs" Inherits="web.hello" %>

<!DOCTYPE html>
<html xmlns="http://www.w3.org/1999/xhtml">
<head runat="server">
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <title></title>
</head>
<body>
    <form id="form1" runat="server">
        <f:PageManager ID="PageManager1" runat="server" />
        <f:Button Text="send Cmd1" runat="server" ID="btn1" OnClick="btn1_Click" />
        <f:Button Text="send Cmd2" runat="server" ID="btn2" OnClick="btn2_Click" />
        <f:Button Text="send unknown cmd" runat="server" ID="btn3" OnClick="btn3_Click" />
    </form>
</body>
</html>
