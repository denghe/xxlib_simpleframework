using System;
using System.Diagnostics;
using System.Runtime.InteropServices;
using System.Threading;
using xx;

public static class Program
{

    // todo: 模拟 RPC 调用

    static void Main(string[] args)
    {
        long counter = 0;
        long successConns = 0;
        using (var loop = new UvLoop())
        {
            // timer 管理器: 1 秒一跳, 池长 6 跳, 默认 TimerStart 参数为 2
            var tm = new UvTimerManager(loop, 1000, 6, 2);

            for (int i = 0; i < 1; ++i)
            {
                var client = new UvTcpClient(loop);
                client.SetAddress("127.0.0.1", 12345);

                // 开始连( 实际执行是在 uv.loop 中 )
                client.Connect();

                // 绑进 timer 管理器
                client.BindTo(tm);

                // 设置时间到的事件: 超时就直接断开
                client.OnTimerFire = () => { if (!client.disposed) client.Disconnect(); };

                // 开始计时
                client.TimerStart();

                // 绑 连接事件回调
                client.OnConnect = status =>
                {
                    if (client.alive)                       // 连接成功
                    {
                        Console.WriteLine("connected.");
                        ++successConns;

                        // 关闭 timer
                        client.TimerStop();

                        // 随便发个包
                        client.Send(new byte[] { 0, 4, 1, 2, 3, 4 });
                    }
                    else                                    // 连接失败
                    {
                        Console.WriteLine("connect failed.");

                        // 启用或更新 timer
                        client.TimerStart();

                        // 再次发起连接
                        client.Connect();
                    }
                };

                // 绑 收到完整包 事件回调
                client.OnRecvPkg = (bb) =>
                {
                    client.Send(new byte[] { 0, 4, 1, 2, 3, 4 });
                    ++counter;
                };

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
