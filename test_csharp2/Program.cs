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

public static class Program
{
    static void Main(string[] args)
    {
        BBuffer.Register<BBuffer>(2);           // 要用到 BBuffer 的收发

        // 初始化 loop 的同时初始化 rpc 管理器. 计时精度 1 秒. 默认 2 秒超时
        using (var loop = new UvLoop(1000, 2))
        {
            // kcp 轮询间隔 ms
            loop.InitKcpFlushInterval(10);

            // server
            var udpListener = new UvUdpListener(loop);
            udpListener.OnAccept = p =>
            {
                CW(p.ip);
                p.OnReceivePackage = pkg =>
                {
                    //CW("listener recv " + p.ip + "'s pkg: " + pkg);

                    // echo
                    p.SendBytes(pkg);
                };
            };
            udpListener.OnDispose = () =>
            {
                CW("listener disposed.");
            };
            udpListener.Bind("0.0.0.0", 12345);
            udpListener.Listen();

            //// client
            //var udpClient = new UvUdpClient(loop);
            //udpClient.OnReceivePackage = pkg =>
            //{
            //    CW("client recv server pkg: " + pkg);
            //    udpClient.SendBytes(pkg);
            //};
            //udpClient.SetAddress("127.0.0.1", 12345);
            //udpClient.Connect(Guid.NewGuid());

            //// make some data for send
            //var bb = new BBuffer();
            //bb.Write((byte)1); bb.Write((byte)2);

            //// send
            //udpClient.Send(bb);

            // begin run
            CW("server: loop.Run();");
            loop.Run();
        }
    }
}
