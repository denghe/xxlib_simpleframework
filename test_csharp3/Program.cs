using System;
using System.Diagnostics;
using System.Threading;

public static class Program
{
    static void Main(string[] args)
    {
        XxNBSocket.SockInit();

        using (var nbs = new XxNBSocket())
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
                    case XxNBSocket.States.Disconnected:
                        {
                            goto LabEnd;
                        }
                    case XxNBSocket.States.Connecting:
                        {
                            if (nbs.ticks > 10)    // 1 秒连不上就算超时吧
                            {
                                nbs.Disconnect();
                            }
                            break;
                        }
                    case XxNBSocket.States.Connected:
                        {
                            // 刚连上时发包
                            if (nbs.ticks == 0) nbs.Send(new byte[] {
                                4, 0, 49, 50, 51, 52,
                                4, 0, 53, 54, 55, 56
                            });

                            // dump 收到的包
                            LabRetry:
                            var buf = nbs.PeekRecv();
                            if (buf != null)
                            {
                                Console.WriteLine("recv pkg. len = " + buf.Length);
                                nbs.PopRecv();
                                goto LabRetry;
                            }

                            if (nbs.ticks > 10) nbs.Disconnect(2);  // 存活 10 帧后, 延迟 2 帧杀掉
                            break;
                        }
                    case XxNBSocket.States.Disconnecting:
                        break;

                    default:
                        break;
                }
                dump();

                Thread.Sleep(100);
                nbs.Update();
            }
            LabEnd:
            dump();

            Console.WriteLine("press any key to continue...");
            Console.ReadKey();
        }

    }
}

