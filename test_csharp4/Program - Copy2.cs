using System;
using System.Diagnostics;

public static class Program
{
    static xx.BBuffer bbSend = new xx.BBuffer();
    static Stopwatch sw = new Stopwatch();
    static void HandlePkg(this xx.UvTcpUdpBase peer, xx.BBuffer recvData)
    {
        try
        {
            xx.IBBuffer pkg = null;
            recvData.ReadRoot(ref pkg);
            if (pkg == null) return;
            switch (pkg)
            {
                case xx.BBuffer bb:
                    {
                        int v = 0;
                        bb.Read(ref v);
                        v++;
                        if (v >= 100000)
                        {
                            Console.WriteLine(sw.ElapsedMilliseconds);
                            return;
                        }
                        bbSend.Clear();
                        bbSend.Write(v);
                        peer.Send(bbSend);
                    }
                    break;
                default:
                    break;
            }
        }
        catch
        {
            peer.Dispose();
        }
    }

    static void Main(string[] args)
    {
        xx.BBuffer.RegisterInternals();
        var loop = new xx.UvLoop();
        var listener = new xx.UvTcpListener(loop);
        listener.Bind("0.0.0.0", 12346);
        listener.OnAccept = peer =>
        {
            peer.OnReceivePackage = recvData =>
            {
                peer.HandlePkg(recvData);
            };
        };
        listener.Listen();

        var client = new xx.UvTcpClient(loop);
        client.OnConnect = state => 
        {
            if(client.alive)
            {
                int v = 0;
                bbSend.Clear();
                bbSend.Write(v);
                client.Send(bbSend);
            }
        };
        client.OnReceivePackage = recvData => 
        {
            client.HandlePkg(recvData);
        };
        client.TryConnect("127.0.0.1", 12346);

        sw.Restart();

        loop.Run();
    }
}
