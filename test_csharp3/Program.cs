using System;
using System.Diagnostics;
using System.Runtime.InteropServices;
using System.Threading;
using xx;
using static ConsoleHelpeer;
public static class ConsoleHelpeer
{
    public static void CW(object o)
    {
#if DEBUG
        Console.WriteLine(o);
#endif
    }
}

public class MyClient : UvTcpClient
{
    public MyClient(UvLoop loop) : base(loop) => OnConnect = OnConnectImpl;             // 绑 连接事件回调
    public MyClient(UvLoop loop, UvTimeouter tm) : this(loop) => this.BindTo(tm);    // 绑 timer管理器
    public void OnConnectImpl(int status)
    {
        if (alive)                              // 连接成功
        {
            CW("connected.");
            ++Program.successConns;             // 同步一下成功连接数统计
            TimerStop();                        // 关闭连接超时 timer
            //var b = new BBuffer();              // 造个 BB包
            bbRpc.Clear();
            bbRpc.Write((uint)0);
            SendRequest(bbRpc, OnReceiveResponse);  // 发起 RPC 请求
        }
        else                                    // 连接失败
        {
            CW("connect failed.");
            TimeoutReset();                       // 启用或更新 timer
            Connect();                          // 再次发起连接
        }
    }
    public void OnReceiveResponse(uint serial, BBuffer bb)
    {
        if (bb == null)
        {
            CW("rpc timeout. serial = " + serial);
            return; // 超时
        }
        ++Program.counter2;
        //var b = bb.ReadPackage<BBuffer>();      // 读出 BB包
        //uint n = 0;
        //b.Read(ref n);                          // 从 BB包中解出 n
        //b.Clear();
        //b.Write(n + 1);                         // 将就 BB包 做发送数据
        //SendRequest(b, OnReceiveResponse);      // 继续发起RPC请求
        //CW($"SendRequest serial = {serial}, n = {n}");
    }
    BBuffer bbRpc = new BBuffer();
    public void SendRpc()
    {
        if (state != UvTcpStates.Connected) return;
        if (Program.counter1 - Program.counter2 > 1000) return;
        if (loop.rpcMgr.count > 2000000) return;
        for (int i = 0; i < 1000; i++)
        {
            ++Program.counter1;
            SendRequest(bbRpc, OnReceiveResponse);  // 发起 RPC 请求
        }
    }
}

public static class Program
{
    public static long counter1 = 0;
    public static long counter2 = 0;
    public static long successConns = 0;

    static void Main(string[] args)
    {
        BBuffer.Register<BBuffer>(2);           // 要用到 BBuffer 的收发

        // 初始化 loop 的同时初始化 rpc 管理器. 计时精度 1 秒. 默认 2 秒超时
        using (var loop = new UvLoop(1000, 2))
        {
            // timer 管理器: 1 秒一跳, 池长 6 跳, 默认 TimerStart 参数为 2
            var tm = new UvTimeouter(loop, 1000, 6, 2);

            for (int i = 0; i < 1; ++i)     // 创建多个连接
            {
                var client = new MyClient(loop, tm);
                client.SetAddress("192.168.1.111", 12345);

                client.OnTimeout = () =>      // 设置时间到的事件: 超时就直接断开
                {
                    if (!client.disposed) client.Disconnect();
                };
                client.TimeoutReset();            // 开始计时
                client.Connect();               // 开始连( 实际执行是在 uv.loop 中 )

                client.OnDisconnect = () =>     // 绑 断开后 事件回调
                {
                    CW("disconnected.");
                    --successConns;
                    client.Connect();           // 再次重连
                };

                // 来个 timer 不停的从 client 发包
                new UvTimer(loop, 0, 1).OnFire = () =>
                {
                    client.SendRpc();
                };
            }

            // 每秒输出一次 连接成功数 & 当前已发送的包数
            new UvTimer(loop, 0, 1000).OnFire = () =>
            {
                Console.WriteLine($"{successConns}, {counter1}, {counter2}");
            };

            loop.Run();
        }
    }
}
