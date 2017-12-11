using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Diagnostics;
using System.Runtime.InteropServices;
using System.Threading;
using System.Threading.Tasks;

public static class Program
{
    static ConcurrentQueue<Action> actions = new ConcurrentQueue<Action>();
    static void Main(string[] args)
    {
        // uv主循环体
        var loop = new XxUvLoop();

        // 非主线程事件通知器
        var async = new XxUvAsync(loop);
        async.OnFire = () => { while (actions.TryDequeue(out var a)) a(); };

        // echo server
        var listener = new XxUvTcpListener(loop);
        listener.OnAccept = peer =>
        {
            Console.WriteLine("listener: " + peer.ip + " connected.");
            peer.OnRead = bytes =>
            {
                new Task(() =>  // 模拟一个耗时异步操作之后发包
                {
                    Thread.Sleep(1000);
                    actions.Enqueue(() => { if (!peer.disposed) peer.Send(bytes); });
                    async.Fire();
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
