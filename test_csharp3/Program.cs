using System;
using System.Diagnostics;
using System.Runtime.InteropServices;
using System.Threading;
using xx;

public static class Program
{
    static void Main(string[] args)
    {
        long counter = 0;
        long successConns = 0;
        using (var loop = new UvLoop())
        {
            for (int i = 0; i < 10000; ++i)
            {
                var client = new UvTcpClient(loop);
                client.SetAddress("192.168.1.250", 10000 + (i % 20));
                client.Connect();
                client.OnConnect = status =>
                {
                    if (client.state == UvTcpStates.Connected)
                    {
                        ++successConns;
                        client.Send(new byte[] { 4, 0, 1, 2, 3, 4 });
                    }
                    else
                    {
                        Console.WriteLine("connect failed.");
                        client.Connect();
                    }
                };
                client.OnRecvPkg = (bs, offset, len) =>
                {
                    client.Send(new byte[] { 4, 0, 1, 2, 3, 4 });
                    ++counter;
                };
                client.OnDisconnect = () =>
                {
                    Console.WriteLine("disconnected.");
                    --successConns;
                    client.Connect();
                };
            }
            var timer = new UvTimer(loop, 0, 1000);
            timer.OnFire = () => { Console.WriteLine(successConns + ", " + counter); };

            loop.Run();
        }
    }
}
