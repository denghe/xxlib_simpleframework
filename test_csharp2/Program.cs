using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Diagnostics;
using System.Runtime.InteropServices;
using System.Threading;
using System.Threading.Tasks;
using xx;

public static class Program
{
    static object mtx = new object();
    static void Main(string[] args)
    {
        for (int ii = 10000; ii < 10020; ++ii)
        {
            int i = ii;
            new Task(() =>
            {
                // uv主循环体
                var loop = new UvLoop();

                var listener = new UvTcpListener(loop);
                listener.OnAccept = peer =>
                {
                    peer.OnRecv = bytes => peer.Send(bytes);
                };
                listener.Bind("0.0.0.0", i);
                listener.Listen();

                // timer test
                var timer = new UvTimer(loop, 100, 1000);
                timer.OnFire = () =>
                {
                    if (listener.peers.dataLen > 0)
                    {
                        lock (mtx)
                        {
                            Console.WriteLine("port: " + i + ", peers count = " + listener.peers.dataLen);
                        }
                    }
                };

                lock (mtx)
                {
                    Console.WriteLine("listing port = " + i);
                }
                loop.Run();
                loop.Dispose();
            })
            .Start();
        }
        Console.ReadLine();
    }
}
