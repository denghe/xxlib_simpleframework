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
using System.Windows.Threading;

namespace manage
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        UVLooper uvlooper;
        DispatcherTimer dt = new DispatcherTimer(DispatcherPriority.SystemIdle);
        NetLooper nl;

        public MainWindow()
        {
            uvlooper = new UVLooper(this);
            nl = UVLooper.instance.looper;
            dt.Interval = new TimeSpan(0, 0, 0, 0, 1);
            dt.Tick += Dt_Tick;
            dt.Start();

            InitializeComponent();

            var n = new NetLog();
            n.Show();
        }

        private void Dt_Tick(object sender, EventArgs e)
        {
            uvlooper.uv.RunOnce();
        }

        public void GotoLogin(Action<string,string> submitHandler)
        {
            var uc = new UCLogin();
            this.Content = uc;
            uc.Submit += submitHandler;
            uc.InitFocus();
        }

        public void GotoServerIP(Action<string, int> submitHandler)
        {
            var uc = new UCServerIP();
            this.Content = uc;
            uc.Submit += submitHandler;
            uc.InitFocus();
        }
        
    }
}
