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
        public static xx.UvLoop uvLoop;
        public static DbClient dbClient;

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
            dt.Tick += (sender, e) =>
            {
                uvLoop.Run(xx.UvRunMode.NoWait);
            };
            dt.Start();

            // 初始化 db连接对象
            dbClient = new DbClient(uvLoop, "127.0.0.1", 12346);
        }
    }

    public class RecvPkg
    {
        public xx.IBBuffer ibb;
        public bool handled;
    }

    // 用来连 DB 服务的客户端. 如果连接失败, 超时会不断重试以维持连接. 每秒会发 1 个心跳包.
    public class DbClient : xx.UvTcpClient
    {
        // 起一个 timer, 每 1 秒检查一下连接状态, 发一个心跳包
        xx.UvTimer timer;

        // 简单的 ping pong 计数
        long pingCount = 0;
        long pingTotal = 0;

        // 包处理回调, 用于各窗口自己注册处理函数. Tuple 的第2个值如果为 true 表示已处理, 函数自己短路
        public event Action<RecvPkg> recvHandlers;

        public DbClient(xx.UvLoop loop, string ip, int port) : base(loop)
        {
            SetAddress(ip, port);

            OnDisconnect = () =>
            {
                Debug.WriteLine("disconnected from db...");
            };

            // 连接后逻辑握手, 自报家门
            OnConnect = status =>
            {
                if (alive)
                {
                    Debug.WriteLine("connected to db. send ServiceInfo...");
                    Send(new RPC.Generic.ServiceInfo
                    {
                        type = RPC.Generic.ServiceTypes.Manage
                    });
                }
            };

            // 收到数据时调用收包处理函数
            OnReceivePackage = bb =>
            {
                var ibb = bb.TryReadRoot<xx.IBBuffer>();
                if (ibb != null && recvHandlers != null)
                {
                    recvHandlers(new RecvPkg { ibb = ibb });
                }
            };

            // 起一个 timer, 每 1 秒检查一下连接状态, 发一个心跳包
            timer = new xx.UvTimer(loop, 0, 1000, () =>
            {
                Debug.Assert(!disposed);

                // 如果已断线 或 未连接, 就发起连接
                if (state == xx.UvTcpStates.Disconnected)
                {
                    Debug.WriteLine("connect to db...");
                    Connect();
                }
                else if (state == xx.UvTcpStates.Connected)
                {
                    //Debug.WriteLine("ping to db...");
                    //SendRequest(new RPC.Generic.Ping
                    //{
                    //    ticks = DateTime.Now.Ticks
                    //},
                    //(serial, bb) =>
                    //{
                    //    if (bb == null) return;
                    //    Debug.WriteLine("recv db's pong...");
                    //    var pong = bb.TryReadPackage<RPC.Generic.Pong>();
                    //    Debug.Assert(pong != null);

                    //    // ping 计数, 总 ping 值累加
                    //    ++pingCount;
                    //    pingTotal += DateTime.Now.Ticks - pong.ticks;
                    //});
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
