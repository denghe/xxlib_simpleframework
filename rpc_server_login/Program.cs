using System;
using System.Diagnostics;
using System.Threading;
using System.Threading.Tasks;
using xx;


// for client
public class Listener : UvTcpListener
{
    Service service;
    public Listener(Service service, string ip, int port) : base(service)
    {
        this.service = service;
        this.OnCreatePeer = () => new Peer(service);
        this.Bind(ip, port);
        this.Listen();
    }
}


// for client
public class Peer : UvTcpPeer
{
    public Service service;
    public Peer(Service service) : base(service.listener)
    {
        this.service = service;

        // 绑到超时管理器. 几秒内没有收到合法的包或请求就 T
        this.BindTimeouter(service.timeouter);
        this.OnTimeout = Dispose;

        // 开始超时计算
        this.TimeoutReset();

        // 绑定请求处理函数
        this.OnReceiveRequest = (serial_client, bb__) =>
        {
            // 试解包, 如果失败直接断开
            var ibb__ = bb__.TryReadPackage<IBBuffer>();
            if (ibb__ == null)
            {
                this.Dispose();
                return;
            }

            switch (ibb__)
            {
                case RPC.Client_Login.Login login:
                    // 重置超时计算
                    this.TimeoutReset();
                    // 如果到 db 的连接活着就进一步发起请求
                    if (service.dbClient.alive)
                    {
                        // 向 db 发起 auth 请求
                        service.dbClient.SendRequest(new RPC.Login_DB.Auth
                        {
                            username = login.username,
                            password = login.password
                        }
                        , (serial_db, bb) =>
                        {
                            // 发生超时回调, 即未在规定时间内收到来自 db 的回应
                            if (bb == null)
                            {
                                // 当回调发生时, 有可能 peer 已 Disconnect / Disposed, 故需要检测
                                if (this.alive)
                                {
                                    this.SendResponse(serial_client, new RPC.Generic.Error
                                    {
                                        errNo = 2,
                                        errMsg = "db service Auth timeout"
                                    });
                                }
                            }
                            // 将回应包解出来, 进一步处理
                            else
                            {
                                var ibb = bb.TryReadPackage<IBBuffer>();

                                // 如果 ibb 为空, 则为内部 / 内网通信错误, 或协议错误 )
                                Debug.Assert(ibb != null);

                                // ... 其他处理

                                switch (ibb)
                                {
                                    case RPC.DB_Login.AuthSuccess authSuccess:
                                        if (this.alive)
                                        {
                                            this.SendResponse(serial_client, new RPC.Login_Client.LoginSuccess
                                            {
                                                id = authSuccess.id
                                            });
                                        }
                                        break;

                                    case RPC.Generic.Error err:
                                        if (this.alive)
                                        {
                                            this.SendResponse(serial_client, err);
                                        }
                                        break;
                                }
                            }
                        });
                    }
                    // 否则就直接返回相应的错误
                    else
                    {
                        this.SendResponse(serial_client, new RPC.Generic.Error
                        {
                            errNo = 3,
                            errMsg = "can't connect to db service"
                        });
                    }
                    break;

                case RPC.Generic.Ping o:
                    this.TimeoutReset();
                    // ticks 原样返回, 用于计算延迟
                    this.SendResponse(serial_client, new RPC.Generic.Pong
                    {
                        ticks = o.ticks
                    });
                    break;
            }
        };
    }
}



// 用来连 DB 服务的客户端. 如果连接失败, 超时会不断重试以维持连接. 每秒会发 1 个心跳包.
public class DbClient : UvTcpClient
{
    UvTimer timer;
    long pingCount = 0;
    long pingTotal = 0;

    public DbClient(UvLoop loop, string ip, int port) : base(loop)
    {
        this.SetAddress(ip, port);

        this.OnDisconnect = () =>
        {
            Console.WriteLine("disconnected from db...");
        };

        this.OnConnect = (status) =>
        {
            if (this.alive)
            {
                Console.WriteLine("connected to db. send ServiceInfo...");
                this.Send(new RPC.Generic.ServiceInfo
                {
                    type = RPC.Generic.ServiceTypes.Login
                });
            }
        };

        // 起一个 timer, 每 1 秒检查一下连接状态, 发一个心跳包
        timer = new UvTimer(loop, 0, 1000, () =>
        {
            Debug.Assert(!this.disposed);

            // 如果已断线 或 未连接, 就发起连接
            if (this.state == UvTcpStates.Disconnected)
            {
                Console.WriteLine("connect to db...");
                this.Connect();
            }
            else if (this.state == UvTcpStates.Connected)
            {
                Console.WriteLine("ping to db...");
                SendRequest(new RPC.Generic.Ping
                {
                    ticks = DateTime.Now.Ticks
                },
                (serial, bb) =>
                {
                    if (bb == null) return;
                    Console.WriteLine("recv db's pong...");
                    var pong = bb.TryReadPackage<RPC.Generic.Pong>();
                    Debug.Assert(pong != null);

                    // ping 计数, 总 ping 值累加
                    ++pingCount;
                    pingTotal += DateTime.Now.Ticks - pong.ticks;
                });
            }
        });
    }

    // 返回平均 ping 值( 毫秒 )
    public double GetPing()
    {
        return (double)pingTotal / (double)pingCount / 10000d;
    }

    // Dispose 时把 timer 杀了
    protected override void Dispose(bool disposing)
    {
        if (!disposed)
        {
            if (timer != null)
            {
                timer.Dispose();
                timer = null;
            }
        }
        base.Dispose(disposing);
    }
}



public class Service : UvLoop
{
    public UvTcpListener listener;
    public DbClient dbClient;

    public Service()
    {
        InitRpcManager(1000, 5);
        InitTimeouter(1000, 6, 5);                                  // 精度:秒, 最长计时 6 秒, 默认 5 秒超时
        listener = new Listener(this, "0.0.0.0", 12345);
        dbClient = new DbClient(this, "127.0.0.1", 12346);
    }
}



public static class Program
{
    static void Main(string[] args)
    {
        RPC.AllTypes.Register();            // 初始化 序列化时的 id:type 映射

        using (var service = new Service())
        {
            Console.WriteLine("running...");
            service.Run();
        }
    }
}
