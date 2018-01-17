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
            loop.InitUdpTimer(10);

            var udpListener = new UvUdpListener(loop);
            udpListener.OnAccept = p => 
            {
                CW(p.ip);
            };
            udpListener.Bind("0.0.0.0", 12345);
            udpListener.RecvStart();


            var udpClient = new UvUdpClient(loop);
            udpClient.SetAddress("127.0.0.1", 12345);
            udpClient.Prepare(Guid.NewGuid());

            var bb = new BBuffer();
            bb.Write((byte)1);
            bb.Write((byte)2);
            bb.Write((byte)3);
            udpClient.Send(bb);

            loop.Run();
        }
    }
}
