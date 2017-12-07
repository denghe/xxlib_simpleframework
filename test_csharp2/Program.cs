using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Runtime.InteropServices;

public static class Program
{
    static void Main(string[] args)
    {
        var loop = new XxUvLoop();
        var listener = new XxUvTcpListener(loop);
        listener.peers = new XxSimpleList<XxUvTcpPeer>(1024);
        listener.OnAccept = peer =>
        {
            peer.index_at_container = listener.peers.bufLen;
            listener.peers.Add(peer);
            peer.OnRead = bytes =>
            {
                if (bytes[0] == 32) peer.Dispose();             // SPACE disconnect
                else if (bytes[0] == 27) listener.Dispose();    // ESC exit
                else peer.Send(bytes);                          // echo
            };
            peer.OnDispose = () =>
            {
                listener.peers.SwapRemoveAt(peer.index_at_container);
            };
            Console.WriteLine(peer.ip + " connected.");
        };
        listener.OnDispose = () =>
        {
            listener.peers.ForEach(c => c.Dispose());
        };
        listener.Bind("0.0.0.0", 12345);
        listener.Listen();
        Console.WriteLine("begin.");
        loop.Run();
        Console.WriteLine("end.");
        Console.Read();
    }
}
