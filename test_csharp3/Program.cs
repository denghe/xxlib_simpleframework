using System;
using System.Diagnostics;
using System.Runtime.InteropServices;
using System.Threading;
using xx;

public class MyClient : UvTcpClient
{
    public MyClient(UvLoop loop) : base(loop)
    {
        // 绑 连接事件回调
        OnConnect = OnConnectImpl;
    }
    public void OnConnectImpl(int status)
    {
        if (alive)                       // 连接成功
        {
            Console.WriteLine("connected.");
            ++Program.successConns;

            // 关闭 timer
            TimerStop();

            // 造个 BB包
            var b = new BBuffer();
            b.Write(Program.counter);

            // 发起 RPC 请求
            SendRequest(b, OnReceiveResponse);
        }
        else                                    // 连接失败
        {
            Console.WriteLine("connect failed.");

            // 启用或更新 timer
            TimerStart();

            // 再次发起连接
            Connect();
        }
    }

    public void OnReceiveResponse(uint serial, BBuffer bb)
    {
        if (bb == null)
        {
            Console.WriteLine("rpc timeout. serial = " + serial);
            return; // 超时
        }

        // 读出 BB包
        var b = bb.ReadPackage<BBuffer>();

        // 从 BB包中解出 counter
        b.Read(ref Program.counter);

        // 将就 BB包 做发送数据
        b.Clear();
        b.Write(Program.counter + 1);

        // 继续发起RPC请求
        SendRequest(b, OnReceiveResponse);
    }
}

public static class Program
{
    public static long counter = 0;
    public static long successConns = 0;

    static void Main(string[] args)
    {
        BBuffer.Register<BBuffer>(2);   // 要用到 BBuffer 的收发

        using (var loop = new UvLoop())
        {
            // 初始化 rpc 管理器. 默认超时 5 秒
            loop.rpcMgr = new UvRpcManager(loop, 1000, 5);

            // timer 管理器: 1 秒一跳, 池长 6 跳, 默认 TimerStart 参数为 2
            var tm = new UvTimerManager(loop, 1000, 6, 2);

            for (int i = 0; i < 1; ++i)
            {
                var client = new MyClient(loop);
                client.SetAddress("127.0.0.1", 12345);

                // 开始连( 实际执行是在 uv.loop 中 )
                client.Connect();

                // 绑进 timer 管理器
                client.BindTo(tm);

                // 设置时间到的事件: 超时就直接断开
                client.OnTimerFire = () => { if (!client.disposed) client.Disconnect(); };

                // 开始计时
                client.TimerStart();

                // 绑 断开后 事件回调
                client.OnDisconnect = () =>
                {
                    Console.WriteLine("disconnected.");
                    --successConns;
                    //client.Connect();
                };
            }

            // 每秒输出一次 连接成功数 & 当前已发送的包数
            new UvTimer(loop, 0, 1000).OnFire = () =>
            {
                Console.WriteLine(successConns + ", " + counter);
            };

            loop.Run();
        }
    }
}
