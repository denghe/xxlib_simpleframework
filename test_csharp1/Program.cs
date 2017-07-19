using System;
using System.Collections;
using System.Diagnostics;
using System.Threading;

// todo: 测试 client -> login -> db -> login -> client 的登录全流程

public class Connector : UVClientPeerWrapperEx
{
    public override void OnConnect()
    {
        Console.WriteLine("Connector.OnConnect(). connected = " + (State == UVPeerStates.Connected));
    }
    public override void OnDisconnect()
    {
        Console.WriteLine("Connector.OnDisconnect()");
    }

    public xx.Queue<xx.IBBuffer> recvs = new xx.Queue<xx.IBBuffer>();
    public override void OnReceivePackage(byte[] data)
    {
        Debug.Assert(State == UVPeerStates.Connected);
        Console.WriteLine("Connector.OnReceivePackage data.Length = " + data.Length + ", currMS = " + Utils.GetCurrentMS());

        bbRecv.Assign(data, data.Length);
        if (bbRecv.ReadPackages(ref recvPkgs) <= 0)
        {
            Disconnect(true);
        }
        else
        {
            for (int i = 0; i < recvPkgs.dataLen; ++i) recvs.Push(recvPkgs[i]);
        }
    }
}

public class Timer : UVTimerWrapper
{
    UVWrapper uv;
    public Timer(UVWrapper uv)
    {
        this.uv = uv;
    }

    Connector c;
    public override void OnCreated()
    {
        Start(0, 100);
        c = uv.CreateClientPeer<Connector>();
        c.SetAddress("127.0.0.1", 12345);
        iter = UpdateCore().GetEnumerator();
    }
    IEnumerator iter;
    public override void OnTicks()
    {
        if (!iter.MoveNext()) Dispose();
    }

    protected IEnumerable UpdateCore()
    {
        // 备用
        long currTicks = 0;
        int retryCount = 0;

        // 第一步, 开始连接
        LabConnect:
        Console.WriteLine("LabConnect:");

        // 允许重试的次数
        for (; retryCount < 5; retryCount++)
        {
            // 如果连接立即失败, 等 1 秒再重试
            if (c.Connect() != 0)
            {
                currTicks = Utils.GetCurrentMS();
                while (Utils.GetCurrentMS() - currTicks < 1000) yield return null;    // 睡 1 秒
            }
            // 成功则跳至下一步
            else goto LabConnecting;
        }
        // 重启数次后无法连接, 退出
        goto LabEnd;

        // 第二步, 等待连接成功或超时
        LabConnecting:
        Console.WriteLine("LabConnecting:");

        // 超时检测
        currTicks = Utils.GetCurrentMS();
        while (Utils.GetCurrentMS() - currTicks < 5000)           // 等 5 秒
        {
            yield return null;
            if (c.State == UVPeerStates.Connected) goto LabConneced;
            else if (c.State == UVPeerStates.Disconnected) goto LabConnect;
        }
        // 超时
        c.Disconnect(true);
        goto LabEnd;

        LabConneced:
        Console.WriteLine("LabConneced:");

        c.recvs.Clear();

        // 发登录包
        var rtv = c.SendPackages(new PKG.Client_Login.Login
        {
            username = "a",
            password = "a"
        });
        if (rtv <= 0)
        {
            Console.WriteLine("Send error!");
            goto LabEnd;
        }

        // 等着收到数据
        Console.WriteLine("Waiting for Data Recv...");
        while (c.recvs.IsEmpty)
        {
            yield return null;
            // todo: 超时判断
        }

        // 暂存用户上下文信息
        PKG.Login_Client.LoginSuccess ls = null;

        // 应该收到一条回应( 此时可能已经断开 )
        Debug.Assert(c.recvs.Count == 1);
        switch (c.recvs.Top())
        {
            case PKG.Login_Client.LoginSuccess o:
                {
                    Console.WriteLine("LoginSuccess.");
                    ls = o;             // 成功就给 ls 赋值
                }
                break;
            case PKG.Login_Client.LoginFail o:
                {
                    Console.WriteLine("LoginFail");
                    goto LabEnd;        // 失败退出
                }
            default:
                // todo: unhandled msg log ?
                break;
        }


        // 向 Lobby 发起连接
        c.SetAddress("127.0.0.1", 12300);
        retryCount = 0;

        // 开始连接 Lobby
        LabLobbyConnect:
        Console.WriteLine("LabLobbyConnect:");

        // 允许重试的次数
        for (; retryCount < 5; retryCount++)
        {
            // 如果连接立即失败, 等 1 秒再重试
            if (c.Connect() != 0)
            {
                currTicks = Utils.GetCurrentMS();
                while (Utils.GetCurrentMS() - currTicks < 1000) yield return null;    // 睡 1 秒
            }
            // 成功则跳至下一步
            else goto LabLobbyConnecting;
        }
        // 重启数次后无法连接, 退出
        goto LabEnd;

        // 等待连接成功或超时
        LabLobbyConnecting:
        Console.WriteLine("LabLobbyConnecting:");

        // 超时检测
        currTicks = Utils.GetCurrentMS();
        while (Utils.GetCurrentMS() - currTicks < 5000)           // 等 5 秒
        {
            yield return null;
            if (c.State == UVPeerStates.Connected) goto LabLobbyConneced;
            else if (c.State == UVPeerStates.Disconnected) goto LabLobbyConnect;
        }
        // 超时
        c.Disconnect(true);
        goto LabEnd;

        LabLobbyConneced:
        Console.WriteLine("LabLobbyConneced:");

        c.recvs.Clear();


        // 发 Join 包
        rtv = c.SendPackages(new PKG.Client_Lobby.Join
        {
            accountId = ls.accountId,
            token = ls.token
        });
        if (rtv <= 0)
        {
            Console.WriteLine("Send error!");
            goto LabEnd;
        }

        // 等着收到数据
        Console.WriteLine("Waiting for Data Recv...");
        while (c.recvs.IsEmpty)
        {
            yield return null;
            // todo: 超时判断
            if (c.State != UVPeerStates.Connected) goto LabEnd;
        }

        PKG.Lobby_Client.JoinSuccess js = null;

        // 处理所有消息
        // 可能收到不止一条数据, 例如 join 后面有可能跟有 SyncCoin
        xx.IBBuffer o_;
        while (c.recvs.TryDequeue(out o_))
        {
            switch (o_)
            {
                case PKG.Lobby_Client.JoinSuccess o:
                    {
                        Console.WriteLine("JoinSuccess.");
                        js = o;
                    }
                    break;
                case PKG.Lobby_Client.SyncCoin o:
                    {
                        Console.WriteLine("SyncCoin");
                        Debug.Assert(js != null);
                        js.coin = o.curr;
                    }
                    break;
                default:
                    Console.WriteLine("unhandled message type: " + c.recvs.Top().GetType());
                    // todo: unhandled msg log ?
                    goto LabEnd;
            }
        }

        // join 成功
        Console.WriteLine("coin = " + js.coin);


        // 接着测试 ChangeCoin
        Console.WriteLine("Send pkg: ChangeCoin");
        rtv = c.SendPackages(new PKG.Client_Lobby.ChangeCoin
        {
            original = js.coin,
            val = 1,
            reason = "test"
        });
        if (rtv <= 0)
        {
            Console.WriteLine("Send error!");
            goto LabEnd;
        }

        // 等在这处理所有消息
        while (c.State == UVPeerStates.Connected || !c.recvs.IsEmpty)
        {
            yield return null;
            if (c.recvs.TryDequeue(out o_))
            {
                switch (o_)
                {
                    case PKG.Lobby_Client.SyncCoin o:
                        {
                            Console.WriteLine("SyncCoin");
                            js.coin = o.curr;
                            Console.WriteLine("coin = " + o.curr);
                        }
                        break;
                    case PKG.Lobby_Client.ChangeCoinSuccess o:
                        {
                            Console.WriteLine("ChangeCoinSuccess");
                        }
                        break;
                    case PKG.Lobby_Client.ChangeCoinFail o:
                        {
                            Console.WriteLine("ChangeCoinFail");
                        }
                        break;
                    default:
                        Console.WriteLine("unhandled message type: " + c.recvs.Top().GetType());
                        // todo: unhandled msg log ?
                        break;
                }
            }
        }


        LabEnd:

        //c.Disconnect(true);
        while (c.State != UVPeerStates.Disconnected)
        {
            yield return null;
            // todo: 超时判断
        }

        c.Dispose();
        c = null;
        yield break;
    }
}

class Program
{
    static void Main(string[] args)
    {
        PKG.AllTypes.Register();
        var uv = new UVWrapper();
        uv.CreateTimer(new Timer(uv));
        uv.Run();
        Console.WriteLine("end.");
        Console.ReadLine();
    }
}
