using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace manage
{
    /// <summary>
    /// Interaction logic for UCServerIP.xaml
    /// </summary>
    public partial class UCServerIP : UserControl
    {
        public UCServerIP()
        {
            InitializeComponent();
            IP = "127.0.0.1";
            Port = "12345";
            Tips = "";
        }


        public void InitFocus()
        {
            tb_ip.Focus();
        }


        public string IP
        {
            get { return tb_ip.Text.Trim(); }
            set { tb_ip.Text = value; }
        }

        public string Port
        {
            get { return tb_port.Text; }
            set { tb_port.Text = value; }
        }


        public string Tips
        {
            set
            {
                if (string.IsNullOrWhiteSpace(value))
                {
                    t_tips.Visibility = Visibility.Hidden;
                }
                else
                {
                    t_tips.Visibility = Visibility.Visible;
                }
                t_tips.Text = value;
            }
        }


        public event Action<string, int> Submit;
        private void b_submit_Click(object sender, RoutedEventArgs e)
        {
            Submit(IP, int.Parse(Port));
        }

    }
}
