using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Runtime.InteropServices;

public static class Program
{
    static void Main(string[] args)
    {
        // loop
        var loop = new XxUvLoop();

        // echo server
        var listener = new XxUvTcpListener(loop);
        listener.peers = new XxSimpleList<XxUvTcpPeer>(1024);
        listener.OnAccept = peer =>
        {
            peer.index_at_container = listener.peers.bufLen;
            listener.peers.Add(peer);
            peer.OnRead = bytes =>
            {
                Console.WriteLine("peer: " + peer.ip + " read bytes " + BitConverter.ToString(bytes));
                if (bytes[0] == 32) peer.Dispose();             // SPACE disconnect
                else if (bytes[0] == 27) listener.Dispose();    // ESC exit
                else peer.Send(bytes);                          // echo
            };
            peer.OnDispose = () =>
            {
                Console.WriteLine("peer: " + peer.ip + " Disconnected.");
                listener.peers.SwapRemoveAt(peer.index_at_container);
            };
            Console.WriteLine("listener: " + peer.ip + " connected.");
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
