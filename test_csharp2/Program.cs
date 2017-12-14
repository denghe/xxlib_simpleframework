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
            var tm = new UvTimerManager(loop, 1000, 6, 5);
            var listener = new UvTcpListener(loop);
            listener.OnAccept = peer =>
            {
                peer.BindTo(tm);
                //Console.WriteLine(peer.ip + " accepted");

                peer.OnTimerFire = () => peer.Dispose();
                peer.TimerStart();

                peer.OnRecv = (bytes) =>
                {
                    peer.TimerStart();
                    peer.Send(bytes);    // echo
                };

                //peer.OnDispose = () =>
                //{
                //    Console.WriteLine(peer.ip + " disposed");
                //};
            };
            listener.Bind("0.0.0.0", 12345);
            listener.Listen();

            new UvTimer(loop, 100, 1000).OnFire = () =>
            {
                Console.WriteLine("peers count = " + listener.peers.dataLen);
            };

            Console.WriteLine("listing");
            loop.Run();
        }
    }
}
