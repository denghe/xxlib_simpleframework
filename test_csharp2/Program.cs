using System;
using System.Diagnostics;
using System.Threading;
using System.Threading.Tasks;
using xx;

public static class Program
{
    static void Main(string[] args)
    {
        BBuffer.Register<BBuffer>(2);   // 要用到 BBuffer 的收发

        using (var loop = new UvLoop())
        {
            // timer 管理器: 1 秒一跳, 池长 6 跳, 默认 TimerStart 参数为 5
            var tm = new UvTimeouter(loop, 1000, 6, 5);
            var listener = new UvTcpListener(loop);
            listener.OnAccept = peer =>
            {
                //Console.WriteLine(peer.ip + " accepted");

                // 接入 timer 管理器, 如果 5 秒没收到数据就断开
                peer.BindTo(tm);
                peer.OnTimeout = () => peer.Dispose();
                peer.TimeoutReset();

                // RPC 请求事件
                peer.OnReceiveRequest = (serial, bb) =>
                {
                    peer.TimeoutReset();                  // 更新 timer
                    var b = bb.ReadPackage<BBuffer>();  // 读出 BB包
                    uint n = 0;
                    b.Read(ref n);                      // 读出 BB包 中的 counter
                    b.Clear();
                    b.Write(n + 1);                     // 将就 BB包 填充 counter + 1 用作应答
                    peer.SendResponse(serial, b);       // 应答
                };

                peer.OnDispose = () =>
                {
                    Console.WriteLine(peer.ip + " disposed");
                };
            };
            listener.Bind("0.0.0.0", 12345);
            listener.Listen();

            // 每秒输出一行当前的连接数
            new UvTimer(loop, 100, 1000).OnFire = () =>
            {
                Console.WriteLine("peers count = " + listener.peers.dataLen);
            };

            Console.WriteLine("listing");
            loop.Run();
        }
    }
}
