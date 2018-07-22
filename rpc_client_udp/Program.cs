using System;
using System.Collections;
using System.Diagnostics;
using System.Runtime.InteropServices;
using System.Threading;
using System.Threading.Tasks;
using xx;


// 模拟 unity 场景代码
public class UnityScene
{
    public UnityScene()
    {
        iter = UpdateCore().GetEnumerator();
    }
    IEnumerator iter;
    public bool Update()
    {
        if (!iter.MoveNext()) return false;     // 驱动逻辑代码
        Program.uvLoop.Run(UvRunMode.NoWait);   // 驱动 uv 网络模块
        ++ticks;                                // 帧号递增( 可用于简单计时 )
        return true;
    }

    UvUdpClient uuc;
    Guid g;                                     // 当前 udp 连接码
    long ticks = 0;                             // 帧号
    public void Enter()
    {
        uuc = new UvUdpClient(Program.uvLoop);
        g = Guid.NewGuid();
    }
    IEnumerable UpdateCore()
    {

        LabConnect:                             // 循环切入点
        yield return null;                      // 避免后续 if + goto 中间缺驱动环节

        Console.WriteLine("set addr");
        uuc.Disconnect();                       // 防御性编程, 执行下面步骤前要清场
        uuc.SetAddress("127.0.0.1", 12344);     // 设置地址附带 ipv6 only 网络检查( 万一切换网络环境? ), 故每次重连时都应该设置
        uuc.Connect(g);                         // udp 并不会产生实际的连接行为, 这个函数只是将就一下使用习惯.

        LabLogin:
        yield return null;

        Console.WriteLine("request login");
        IBBuffer recv = BBuffer.instance;       // 用于请求回调中修改以便在 while 中扫描
        Exception e = null;
        try
        {
            var serial = uuc.SendRequest(       // 发登录请求
                new RPC.Client_Login.Login      // 构造一个登录包( 也可以用个静态的临时改值 )
                {
                    username = "a",             // 模拟读取用户输入
                    password = (ticks < 100) ? "asdf" : "11111"     // 一定机率输对密码
                }
                , (s, bb) =>                    // RPC 回调: 将结果存入上下文变量以便后续 while 中判断
                {
                    recv = bb.TryReadRoot<IBBuffer>();
                });
        }
        catch (Exception ex)                    // 如果发生 send 失败, 有可能是网络环境切换造成. 比如当前没网了, 当前网络由 ipv4 变 ipv6 了等等
        {
            e = ex;                             // 用个 e 将 ex 弄出来下面继续处理( catch 里面的代码无法 yield return )
        }
        if (e != null)
        {
            Console.WriteLine("send error: " + e.Message);
            var tarTicks = ticks + 30;
            while (ticks < tarTicks)            // sleep 1秒
            {
                yield return null;              // 这里可以放置其他判断以及时跳出
            }
            goto LabConnect;                    // 重连
        }

        Console.WriteLine("wait login response");
        while (recv == BBuffer.instance)        // 如果回调没发生就一直等
        {
            yield return null;
        }

        Console.WriteLine("response callback");
        if (recv == null)                       // RPC 超时, 进一步分析原因
        {
            Console.WriteLine("timeout");
            goto LabLogin;                      // 继续发起登录
        }
        else
        {
            if (recv.GetType() == typeof(RPC.Generic.Error))
            {
                Console.WriteLine("recv " + recv.GetType() + " errMsg = " + ((RPC.Generic.Error)recv).errMsg);
                var tarTicks = ticks + 30;
                while (ticks < tarTicks)        // sleep 1秒
                {
                    yield return null;          // 这里可以放置其他判断以及时跳出
                }
                goto LabLogin;                  // 有可能是 服务错误 或校验错误, 这里不作进一步展开, 先无脑重试
                //goto LabPing;
            }
            Console.WriteLine("login success");
            // 登录成功, 出循环继续执行. 暂时没别的可能了
        }

        LabPing:
        yield return null;

        Console.WriteLine("request ping");
        while (e == null)
        {
            yield return null;
            if (ticks % 30 == 0)            // 每秒 ping 一次
            {
                try
                {
                    uuc.SendRequest(
                        new RPC.Generic.Ping        // 构造一个 ping 包
                        {
                            ticks = DateTime.Now.Ticks
                        }
                        , (s, bb) =>
                        {
                            if (bb == null)        // 超时: 就当 ping lost. 如果是断线, 外围 while 能处理
                            {
                                Console.WriteLine("ping timeout");
                            }
                            else
                            {
                                var ibb = bb.TryReadRoot<IBBuffer>();
                                if (ibb == null)
                                {
                                    Console.WriteLine("recv bad data");
                                }
                                else
                                {
                                    Console.WriteLine("ping " + ((double)(DateTime.Now.Ticks - ((RPC.Generic.Pong)ibb).ticks) / 10000) + " ms");
                                }
                            }
                        });
                }
                catch (Exception ex)
                {
                    e = ex;
                }
            }
        }

        // 发送失败重连
        Console.WriteLine("disconnected");
        goto LabConnect;

        //yield break;  // 退出
    }
    public void Leave()
    {
        if (uuc != null)
        {
            uuc.Dispose();
            uuc = null;
        }
    }
}


public static class Program
{
    public static UvLoop uvLoop = new UvLoop();

    static void Main(string[] args)
    {
        RPC.AllTypes.Register();                    // 注册通信类 id 映射
        uvLoop.InitRpcManager();                    // 初始化 rpc 管理器, 设置默认计时粒度及超时时长
        uvLoop.InitKcpFlushInterval(1000 / 30);     // 初始化 udp kcp flush 数据周期时间粒度

        var us = new UnityScene();
        us.Enter();
        while (true)
        {
            if (!us.Update())
            {
                us.Leave();
                break;
            }
            Thread.Sleep(1000 / 30);   // 模拟帧刷新
        }
    }
}
