using System;
using System.Diagnostics;
using System.Runtime.InteropServices;
using xx;

public static class Program
{
    static void Main(string[] args)
    {
        // loop
        var loop = new UvLoop();

        // echo server
        var listener = new UvTcpListener(loop);
        listener.OnAccept = peer =>
        {
            peer.OnRecv = bytes =>
            {
                Console.WriteLine("peer: " + peer.ip + " read bytes " + BitConverter.ToString(bytes));
                if (bytes[0] == 32) peer.Dispose();             // SPACE disconnect
                else if (bytes[0] == 27) listener.Dispose();    // ESC exit
                else peer.Send(bytes);                          // echo
            };
            peer.OnDispose = () =>
            {
                Console.WriteLine("peer: " + peer.ip + " Disconnected.");
            };
            Console.WriteLine("listener: " + peer.ip + " connected.");
        };
        listener.Bind("0.0.0.0", 12345);
        listener.Listen();

        Console.WriteLine("begin.");
        loop.Run();
        Console.WriteLine("end.");
        Console.ReadKey();
    }
}
