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

        using (var loop = new UvLoop())
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



            //var tcpClient = new xx.UvTcpClient(loop);
            //int counter = 0;
            //tcpClient.OnReceivePackage = pkg =>
            //{
            //    ++counter;
            //    tcpClient.SendBytes(pkg);
            //};
            //tcpClient.OnConnect = status => 
            //{
            //    Console.WriteLine("status = " + status);
            //    if (!tcpClient.alive) return;
            //    // make some data for send
            //    var bb = new xx.BBuffer();
            //    bb.Write((byte)1);
            //    bb.Write((byte)2);

            //    tcpClient.Send(bb);
            //};
            //tcpClient.SetAddress("192.168.1.111", 12345);
            //tcpClient.Connect();

            Console.WriteLine("loo.Running...");
            loop.Run();
        }
    }
}
