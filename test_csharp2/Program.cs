using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Runtime.InteropServices;

public static class Program
{
    static void Main(string[] args)
    {
        var loop = new XxUvLoop();
        var listener = new XxUvTcp(loop);
        listener.clients = new XxSimpleList<XxUvTcp>(1024);
        listener.OnConnect = client =>
        {
            client.index_at_clients = listener.clients.bufLen;
            listener.clients.Add(client);
            client.OnRead = bytes =>
            {
                if (bytes[0] == 32) client.Dispose();           // SPACE disconnect
                else if (bytes[0] == 27) listener.Dispose();    // ESC exit
                else client.Send(bytes);                        // echo
            };
            client.OnDispose = () =>
            {
                listener.clients.SwapRemoveAt(client.index_at_clients);
            };
            Console.WriteLine(client.ip + " connected.");
        };
        listener.OnDispose = () =>
        {
            listener.clients.ForEach(c => c.Dispose());
        };
        listener.Bind("0.0.0.0", 12345);
        listener.Listen();
        loop.Run();
        Console.WriteLine("end.");
        Console.Read();
    }
}
