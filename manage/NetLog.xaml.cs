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
using System.Windows.Shapes;
using System.Windows.Threading;

namespace manage
{
    /// <summary>
    /// Interaction logic for NetLog.xaml
    /// </summary>
    public partial class NetLog : Window
    {
        NetLooper nl;
        DispatcherTimer dt = new DispatcherTimer(DispatcherPriority.SystemIdle);
        public NetLog()
        {
            InitializeComponent();

            nl = UVLooper.instance.looper;
            dt.Interval = new TimeSpan(0, 0, 0, 0, 1);
            dt.Tick += Dt_Tick;
            dt.Start();
        }

        private void Dt_Tick(object sender, EventArgs e)
        {
            while (nl.stateLogs.TryDequeue(out var s))
            {
                tb_log.AppendText(s);
            }
        }

    }
}
