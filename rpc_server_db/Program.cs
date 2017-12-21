using System;
using System.Diagnostics;
using System.Threading;
using System.Threading.Tasks;
using xx;



// for service
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


// for service
public class Peer : UvTcpPeer
{
    public Service service;
    public Peer(Service service) : base(service.listener)
    {
        this.service = service;

        // 绑到超时管理器. 几秒内没有收到合法的包或请求就 T
        this.BindTo(service.timeouter);
        this.OnTimeout = Dispose;
        this.TimeoutReset();

        // 匿名连接处理: 收到认证信息后 绑相应上下文
        this.OnReceivePackage = (bb) =>
        {
            // 试解包, 如果失败直接断开
            var ibb = bb.TryReadPackage<IBBuffer>();
            if (ibb == null)
            {
                this.Dispose();
                return;
            }

            // 首包判断
            switch (ibb)
            {
                case RPC.Generic.ServiceInfo o:
                    {
                        if (o.type == RPC.Generic.ServiceTypes.Login)
                        {
                            // 试着绑上下文( 如果上下文已经绑有 peer, 绑定将失败 )
                            if (service.loginCtx.BindPeer(this))
                            {
                                // 刷新超时时间 防 T
                                this.TimeoutReset();

                                Console.WriteLine("login connected...");
                                return;
                            }
                        }
                        // else ...
                        break;
                    }
            }
            Dispose();
        };
    }

}



// login 服务的上下文
public class LoginContext : UvContextBase
{
    public Service service;
    public LoginContext(Service service)
    {
        this.service = service;
    }

    public override void HandlePackage(IBBuffer ibb)
    {
        Console.WriteLine("recv package: " + ibb.ToString());
    }

    public override void HandleRequest(uint serial, IBBuffer ibb)
    {
        Console.WriteLine("recv request: " + ibb.ToString());
        switch (ibb)
        {
            case RPC.Login_DB.Auth o:
                {
                    // 发起多线程查询
                    new Task(() =>
                    {
                        // 模拟一个耗时操作
                        Thread.Sleep(1000);

                        // 模拟查询结果对比
                        if (o.username == "a" && o.password == "11111")
                        {
                            // 封送到 uv 主线程去执行
                            service.Dispatch(() =>
                            {
                                // 检查连接的死活
                                if (peerAlive)
                                {
                                    // 发送回应: 校验成功
                                    peer.SendResponse(serial, new RPC.DB_Login.AuthSuccess
                                    {
                                        id = 1
                                    });
                                }
                            });
                        }
                        else
                        {
                            service.Dispatch(() =>
                            {
                                if (peerAlive)
                                {
                                    peer.SendResponse(serial, new RPC.Generic.Error
                                    {
                                        errNo = 1,
                                        errMsg = "bad username or password"
                                    });
                                }
                            });
                        }

                    }).Start();
                    break;
                }
            case RPC.Generic.Ping o:
                {
                    // 主线程直接投递, 不需要检测 peerAlive
                    peer.SendResponse(serial, new RPC.Generic.Pong
                    {
                        ticks = o.ticks
                    });
                    break;
                }
            // more case

            default:
                // 收到不适宜的包, 断开连接
                KickPeer();
                break;
        }
    }

    public override void HandleDisconnect()
    {
        Console.WriteLine("login server disconnected.");
    }
}



public class Service : UvLoop
{
    public UvTimeouter timeouter;
    public UvTcpListener listener;
    public UvAsync dispatcher;
    public LoginContext loginCtx;                                   // 登录服务上下文

    public Service()
    {
        timeouter = new UvTimeouter(this, 1000, 6, 5);              // 精度:秒, 最长计时 6 秒, 默认 5 秒超时
        listener = new Listener(this, "0.0.0.0", 12346);
        dispatcher = new UvAsync(this);
        loginCtx = new LoginContext(this);
    }

    // for easy use
    public void Dispatch(Action a)
    {
        dispatcher.Dispatch(a);
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
