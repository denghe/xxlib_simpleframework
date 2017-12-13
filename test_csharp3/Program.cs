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
            for (int i = 0; i < 1; ++i)
            {
                var client = new UvTcpClient(loop);
                client.SetAddress("127.0.0.1", 12345);
                client.Connect();
                client.OnConnect = status =>
                {
                    if (client.state == UvTcpStates.Connected)
                    {
                        Console.WriteLine("connected.");
                        ++successConns;
                        client.Send(new byte[] { 4, 0, 1, 2, 3, 4 });
                    }
                    else
                    {
                        Console.WriteLine("connect failed.");
                        client.Connect();
                    }
                };
                client.OnRecvPkg = (bb) =>
                {
                    client.SendRecvPkg(bb);
                    ++counter;
                };
                client.OnDisconnect = () =>
                {
                    Console.WriteLine("disconnected.");
                    --successConns;
                    //client.Connect();
                };
            }
            var timer = new UvTimer(loop, 0, 1000);
            timer.OnFire = () => { Console.WriteLine(successConns + ", " + counter); };

            loop.Run();
        }
    }
}
