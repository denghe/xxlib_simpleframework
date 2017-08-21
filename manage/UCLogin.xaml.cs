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
    /// Interaction logic for UCLogin.xaml
    /// </summary>
    public partial class UCLogin : UserControl
    {

        public UCLogin()
        {
            InitializeComponent();
        }


        public string Username
        {
            get { return tb_username.Text.Trim(); }
            set { tb_username.Text = value; }
        }
        public string Password
        {
            get { return tb_password.Text; }
            set { tb_password.Text = value; }
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
                    t_tips.Text = value;
                }
            }
        }
        public event Action<string, string> Submit;


        private void b_submit_Click(object sender, RoutedEventArgs e)
        {
            Submit(Username, Password);
        }

    }
}
