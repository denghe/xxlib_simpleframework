using System.Diagnostics;
using System.Windows;

namespace rpc_manage
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            var c = App.dbClient;
            if (!c.alive)
            {
                MessageBox.Show("network error.");
            }
            else
            {
                c.Send(new RPC.Manage_DB.Msg
                {
                    txt = tb1.Text
                });
                var w1 = new Window1();
                var r = w1.ShowDialog();
                Debug.Print("r = " + r);
            }
        }
    }
}
