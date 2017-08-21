using System;
using System.Collections.Generic;
using System.Configuration;
using System.Data;
using System.Diagnostics;
using System.Linq;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Threading;

namespace manage
{
    /// <summary>
    /// Interaction logic for App.xaml
    /// </summary>
    public partial class App : Application
    {
        UVLooper looper = new UVLooper();
        DispatcherTimer dt = new DispatcherTimer(DispatcherPriority.SystemIdle);
        public App()
        {
            dt.Interval = new TimeSpan(0,0,0,0,1);
            dt.Tick += Dt_Tick;
            dt.Start();
        }

        private void Dt_Tick(object sender, EventArgs e)
        {
            looper.uv.RunOnce();
        }
    }
}
