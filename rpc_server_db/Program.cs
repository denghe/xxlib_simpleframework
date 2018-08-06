using System;
using System.Diagnostics;
using System.Threading;
using System.Threading.Tasks;
using xx;


// for service
public class ServiceListener : UvTcpListener
{
    Service service;
    public ServiceListener(Service service, string ip, int port) : base(service)
    {
        this.service = service;
        this.OnCreatePeer = () => new ServicePeer(service);
        this.Bind(ip, port);
        this.Listen();
    }
}


// for service
public class ServicePeer : UvTcpPeer
{
    public Service service;
    public ServicePeer(Service service) : base(service.svcListener)
    {
        this.service = service;

        // 匿名连接处理: 收到认证信息后 绑相应上下文
        this.OnReceivePackage = (bb) =>
        {
            // 试解包, 如果失败直接断开
            var ibb = bb.TryReadRoot<IBBuffer>();
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
                        switch (o.type)
                        {
                            case RPC.Generic.ServiceTypes.Login:
                                {
                                    // 试着绑上下文( 如果上下文已经绑有 peer, 绑定将失败 )
                                    if (service.ctxLogin.BindPeer(this))
                                    {
                                        Console.WriteLine("login connected...");
                                        return;
                                    }
                                    break;
                                }
                            case RPC.Generic.ServiceTypes.Manage:
                                {
                                    if (service.ctxManage.BindPeer(this))
                                    {
                                        Console.WriteLine("manage connected...");
                                        return;
                                    }
                                    break;
                                }
                            default:
                                break;
                        }
                        break;
                    }
            }
            // 非法首包直接 T
            Dispose();
        };
    }

}


// login 服务的上下文
public class ServiceContext_Login : UvContextBase
{
    public Service service;
    public ServiceContext_Login(Service service)
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


// manage 端的上下文
public class ServiceContext_Manage : UvContextBase
{
    public Service service;
    public ServiceContext_Manage(Service service)
    {
        this.service = service;
    }

    // 处理一般推送
    public override void HandlePackage(IBBuffer ibb)
    {
        Console.WriteLine("recv package: " + ibb.ToString());
        switch (ibb)
        {
            case RPC.Manage_DB.Msg o:
                {
                    // 发起多线程查询
                    new Task(() =>
                    {
                        // 模拟一个耗时操作
                        Thread.Sleep(1000);

                        // 封送到 uv 主线程去执行
                        service.Dispatch(() =>
                        {
                            // 检查连接的死活
                            if (peerAlive)
                            {
                                // 发送处理结果
                                peer.Send(new RPC.DB_Manage.MsgResult
                                {
                                    txt = o.txt
                                });
                            }
                        });
                    }).Start();
                    break;
                }
            // more case

            default:
                // 收到不适宜的包, 断开连接
                KickPeer();
                break;
        }
    }

    // 处理回调请求
    public override void HandleRequest(uint serial, IBBuffer ibb)
    {
        Console.WriteLine("recv request: " + ibb.ToString());
        switch (ibb)
        {
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
    public ServiceListener svcListener;
    public UvAsync dispatcher;
    public ServiceContext_Login ctxLogin;                                   // 登录服务上下文
    public ServiceContext_Manage ctxManage;                                 // 管理端上下文

    public Service()
    {
        InitRpcManager(1000, 5);
        InitTimeoutManager(1000, 6, 5);                                          // 精度:秒, 最长计时 6 秒, 默认 5 秒超时

        svcListener = new ServiceListener(this, "0.0.0.0", 12346);
        dispatcher = new UvAsync(this);
        ctxLogin = new ServiceContext_Login(this);
        ctxManage = new ServiceContext_Manage(this);
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
