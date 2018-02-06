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
        BBuffer.RegisterInternals();

        using (var loop = new UvLoop(1000, 2))
        {
            var tcpListener = new UvTcpListener(loop);
            tcpListener.OnAccept = p =>
            {
                CW(p.ip);
                p.OnReceivePackage = pkg =>
                {
                    CW("listener recv " + p.ip + "'s pkg: " + pkg);

                    // echo
                    p.SendBytes(pkg);
                };
            };
            tcpListener.OnDispose = () =>
            {
                CW("listener disposed.");
            };
            tcpListener.Bind("0.0.0.0", 12345);
            tcpListener.Listen();

            loop.Run();
        }
    }
}
