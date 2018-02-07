using System;
using System.Diagnostics;
using System.Runtime.InteropServices;
using System.Threading;
using xx;

public static class Program
{
    static void Main(string[] args)
    {
        BBuffer.RegisterInternals();

        using (var loop = new UvLoop(1000, 2))
        {
            var tcpListener = new UvTcpListener(loop);
            tcpListener.OnAccept = p =>
            {
                Console.WriteLine(p.ip + " Accepted.");
                p.OnReceivePackage = pkg =>
                {
                    Console.WriteLine("listener recv " + p.ip + "'s pkg: " + pkg);

                    // echo
                    p.SendBytes(pkg);
                };
                p.OnDispose = () =>
                {
                    Console.WriteLine(p.ip + " Disconnected.");
                };
            };
            tcpListener.OnDispose = () =>
            {
                Console.WriteLine("listener disposed.");
            };
            tcpListener.Bind("::", 12345);
            //tcpListener.Bind("0.0.0.0", 12345);
            tcpListener.Listen();

            Console.WriteLine("loo.Running...");
            loop.Run();
        }
    }
}
