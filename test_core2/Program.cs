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

        // client
        var client = new XxUvTcpClient(loop);
        client.OnConnect = status =>
        {
            Console.WriteLine("client: " + client.state);
        };
        client.SetAddress("127.0.0.1", 12345);
        client.Connect();

        Console.WriteLine("begin.");
        loop.Run();
        Console.WriteLine("end.");
        Console.Read();
    }
}
