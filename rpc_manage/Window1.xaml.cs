using System;
using System.Diagnostics;
using System.Windows;

namespace rpc_manage
{
    /// <summary>
    /// Interaction logic for Window1.xaml
    /// </summary>
    public partial class Window1 : Window
    {
        public Window1()
        {
            InitializeComponent();
        }

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            // 注册网络包处理函数
            App.dbClient.recvHandlers += DbClient_recvHandlers;
        }

        private void Window_Unloaded(object sender, RoutedEventArgs e)
        {
            // 反注册网络包处理函数
            App.dbClient.recvHandlers -= DbClient_recvHandlers;
        }

        private void DbClient_recvHandlers(RecvPkg rp)
        {
            if (rp.handled) return;
            switch (rp.ibb)
            {
                case RPC.DB_Manage.MsgResult o:
                    Debug.Print("Window1 recv RPC.DB_Manage.MsgResult, txt = " + o.txt);
                    tb1.Text = o.txt;
                    rp.handled = true;
                    break;
                // todo: more case here
                default:
                    throw new Exception("收到未经处理的包: " + rp.ibb.ToString());
            }
        }

        private void Click_b1(object sender, RoutedEventArgs e)
        {
            // 关闭对话框并设置返回值
            DialogResult = true;
        }

        private void Click_b2(object sender, RoutedEventArgs e)
        {
            // 关闭对话框并设置返回值
            DialogResult = false;
        }
    }
}
