using System;
using System.Diagnostics;
using System.Threading;
using System.Threading.Tasks;
using xx;

public static class Program
{
    static void Main(string[] args)
    {
        using (var loop = new UvLoop())
        {
            // timer 管理器: 1 秒一跳, 池长 6 跳, 默认 TimerStart 参数为 5
            var tm = new UvTimerManager(loop, 1000, 6, 5);
            var listener = new UvTcpListener(loop);
            listener.OnAccept = peer =>
            {
                Console.WriteLine(peer.ip + " accepted");

                // 接入 timer 管理器, 如果 5 秒没收到数据就断开
                peer.BindTo(tm);
                peer.OnTimerFire = () => peer.Dispose();
                peer.TimerStart();

                peer.OnRecvPkg = bb =>
                {
                    peer.TimerStart();      // 更新 timer
                    peer.SendRecvPkg(bb);   // echo
                };

                peer.OnDispose = () =>
                {
                    Console.WriteLine(peer.ip + " disposed");
                };
            };
            listener.Bind("0.0.0.0", 12345);
            listener.Listen();

            // 每秒输出一行当前的连接数
            new UvTimer(loop, 100, 1000).OnFire = () =>
            {
                Console.WriteLine("peers count = " + listener.peers.dataLen);
            };

            Console.WriteLine("listing");
            loop.Run();
        }
    }
}
