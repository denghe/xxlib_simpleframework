using System;
using System.Collections;
using System.Diagnostics;
using System.Runtime.InteropServices;
using System.Threading;
using xx;

public static class Program
{
    const int frameMS = 100;
    const int secFrames = 1000 / frameMS;

    static void Main(string[] args)
    {
        RPC.AllTypes.Register();
        NBSocket.SockInit();

        using (var nbs = new NBSocket(secFrames * 5))      // RPC 5 秒超时
        {
            var dump = new Action(() =>
            {
                Console.WriteLine("state = " + nbs.state.ToString() + ", ticks = " + nbs.ticks);
            });
            dump();

            nbs.SetAddress("127.0.0.1", 12345);
            nbs.Connect();
            dump();

            while (true)
            {
                switch (nbs.state)
                {
                    case NBSocketStates.Connecting:
                        if (nbs.ticks > secFrames * 1)     // 1 秒连不上就算超时吧
                        {
                            Console.WriteLine("connect timeout.");
                            nbs.Disconnect();
                        }
                        break;

                    case NBSocketStates.Connected:

                        if (nbs.ticks == 0)     // 刚连上时发包
                        {
                            nbs.SendRequest(new RPC.Client_Login.Login
                            {
                                username = "a",
                                password = "b"
                            }
                            , (serial, ibb) =>
                            {
                                var t = ibb.GetType();
                                Console.WriteLine("recv response: " + t.FullName);
                                if (t == typeof(RPC.Login_Client.LoginSuccess))
                                {
                                    var o = (RPC.Login_Client.LoginSuccess)ibb;
                                    Console.WriteLine("id = " + o.id);
                                }
                                else if (t == typeof(RPC.Generic.Error))
                                {
                                    var o = (RPC.Generic.Error)ibb;
                                    Console.WriteLine("errNo = " + o.errNo + ", errMsg = " + o.errMsg);
                                }
                            });
                        }

                        if (nbs.ticks > secFrames * 3) nbs.Disconnect(secFrames);  // 存活 3 秒后, 延迟 1 秒杀掉
                        break;
                    case NBSocketStates.Disconnecting:
                        break;

                    case NBSocketStates.Disconnected:
                        goto LabEnd;            // 连不上就出循环

                    default:
                        break;
                }

                Thread.Sleep(frameMS);          // 模拟帧延迟
                nbs.Update();
                dump();
            }
            LabEnd:
            dump();

            Console.WriteLine("press any key to continue...");
            Console.ReadKey();
        }
    }

}

