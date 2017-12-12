using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Diagnostics;
using System.Runtime.InteropServices;
using System.Threading;
using System.Threading.Tasks;

public static class Program
{
    static void Main(string[] args)
    {
        // uv主循环体
        var loop = new XxUvLoop();

        // 非主线程事件通知器
        var async = new XxUvAsync(loop);

        // lag echo server
        var listener = new XxUvTcpListener(loop);
        listener.OnAccept = peer =>
        {
            Console.WriteLine("listener: " + peer.ip + " connected.");
            peer.OnRecv = bytes =>
            {
                new Task(() =>  // 模拟一个耗时异步操作之后回发
                {
                    Thread.Sleep(1000);
                    async.Fire(() => { if (!peer.disposed) peer.Send(bytes); });
                }).Start();
            };
            peer.OnDispose = () => Console.WriteLine("peer: " + peer.ip + " Disconnected.");
        };
        listener.Bind("0.0.0.0", 12345);
        listener.Listen();

        // timer test
        var timer = new XxUvTimer(loop, 100, 1000);
        timer.OnFire = () => Console.Write(".");

        loop.Run();
    }
}
