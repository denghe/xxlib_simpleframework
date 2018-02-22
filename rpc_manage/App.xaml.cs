using System;
using System.Collections.Generic;
using System.Configuration;
using System.Data;
using System.Diagnostics;
using System.Linq;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Threading;

namespace rpc_manage
{
    /// <summary>
    /// Interaction logic for App.xaml
    /// </summary>
    public partial class App : Application
    {
        public xx.UvLoop uvLoop;
        public DbClient dbClient;

        DispatcherTimer dt = new DispatcherTimer(DispatcherPriority.SystemIdle);
        public App()
        {
            // 初始化通信包类
            RPC.AllTypes.Register();

            // 初始化 uvLoop
            uvLoop = new xx.UvLoop();
            uvLoop.InitRpcManager();

            // 初始化 loop update驱动用 timer
            dt.Interval = new TimeSpan(0, 0, 0, 0, 1);
            dt.Tick += (sender, e)=> 
            {
                uvLoop.Run(xx.UvRunMode.NoWait);
            };
            dt.Start();

            // 初始化 db连接对象
            dbClient = new DbClient(uvLoop, "127.0.0.1", 12346);
        }
    }


    // 用来连 DB 服务的客户端. 如果连接失败, 超时会不断重试以维持连接. 每秒会发 1 个心跳包.
    public class DbClient : xx.UvTcpClient
    {
        xx.UvTimer timer;
        long pingCount = 0;
        long pingTotal = 0;

        public DbClient(xx.UvLoop loop, string ip, int port) : base(loop)
        {
            this.SetAddress(ip, port);

            this.OnDisconnect = () =>
            {
                Console.WriteLine("disconnected from db...");
            };

            this.OnConnect = (status) =>
            {
                if (this.alive)
                {
                    Console.WriteLine("connected to db. send ServiceInfo...");
                    this.Send(new RPC.Generic.ServiceInfo
                    {
                        type = RPC.Generic.ServiceTypes.Manage
                    });
                }
            };

            // 起一个 timer, 每 1 秒检查一下连接状态, 发一个心跳包
            timer = new xx.UvTimer(loop, 0, 1000, () =>
            {
                Debug.Assert(!this.disposed);

                // 如果已断线 或 未连接, 就发起连接
                if (this.state == xx.UvTcpStates.Disconnected)
                {
                    Console.WriteLine("connect to db...");
                    this.Connect();
                }
                else if (this.state == xx.UvTcpStates.Connected)
                {
                    Console.WriteLine("ping to db...");
                    SendRequest(new RPC.Generic.Ping
                    {
                        ticks = DateTime.Now.Ticks
                    },
                    (serial, bb) =>
                    {
                        if (bb == null) return;
                        Console.WriteLine("recv db's pong...");
                        var pong = bb.TryReadPackage<RPC.Generic.Pong>();
                        Debug.Assert(pong != null);

                        // ping 计数, 总 ping 值累加
                        ++pingCount;
                        pingTotal += DateTime.Now.Ticks - pong.ticks;
                    });
                }
            });
        }

        // 返回平均 ping 值( 毫秒 )
        public double GetPing()
        {
            return (double)pingTotal / (double)pingCount / 10000d;
        }

        // Dispose 时把 timer 杀了
        protected override void Dispose(bool disposing)
        {
            if (!disposed)
            {
                if (timer != null)
                {
                    timer.Dispose();
                    timer = null;
                }
            }
            base.Dispose(disposing);
        }
    }
}
