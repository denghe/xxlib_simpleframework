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
        nbs.Update();                           // 驱动网络模块
        return true;
    }

    NBSocket nbs;
    public void Enter()
    {
        nbs = new NBSocket(30 * 5);
    }
    IEnumerable UpdateCore()
    {
        Console.WriteLine("init");

        LabConnect:                             // 循环切入点
        yield return null;                      // 避免后续 if + goto 中间缺驱动环节

        nbs.Disconnect();                       // 防御性编程
        nbs.SetAddress("127.0.0.1", 12345);     // 每次连接前都设置地址有助于检测网络环境在 ipv4/6 间切换的问题
        nbs.Connect();                          // 下达任务 并等结果
        Console.WriteLine("connecting...");
        while (true)
        {
            yield return null;                  // while 内必配一句 yield 防独占 cpu

            if (nbs.state != NBSocketStates.Connecting) break;  // 连接成功或失败
            if (nbs.ticks > 30 * 2) break;      // 超时
        }

        Console.WriteLine("event occur");
        switch (nbs.state)                      // 处理结果
        {
            case NBSocketStates.Disconnected:   // 超时时间内没连上, 可能是网络或系统故障或被目标服务器拒绝\
                var ticks = nbs.ticks + 30;
                while (nbs.ticks < ticks)       // sleep 1秒
                {
                    yield return null;          // 这里可以放置其他判断以及时跳出
                }
                Console.WriteLine("can't connect to server");
                goto LabConnect;                // 跳到开始连接处, 继续上述流程
            case NBSocketStates.Connecting:     // 超时
                nbs.Disconnect();               // 立即断开
                Console.WriteLine("timeout");
                goto LabConnect;                // 跳到开始连接处, 继续上述流程 
        }
        Console.WriteLine("connect success");   // 已连上, 继续执行下面的逻辑代码

        LabLogin:
        yield return null;

        Console.WriteLine("request login");
        IBBuffer recv = BBuffer.instance;       // 用于请求回调中修改以便在 while 中扫描
        var serial = nbs.SendRequest(           // 发登录请求
            new RPC.Client_Login.Login          // 构造一个登录包( 也可以用个静态的临时改值 )
            {
                username = "a",                 // 模拟读取用户输入
                password = ((nbs.ticks % 10 > 1)) ? "asdf" : "11111"     // 一定机率输对密码
            }
            , (s, ibb) => { recv = ibb; });     // RPC 回调: 将结果存入上下文变量以便后续 while 中判断

        Console.WriteLine("wait login response");
        while (recv == BBuffer.instance)        // 如果回调没发生就一直等
        {
            yield return null;
            if (!nbs.alive)                     // 等待期间断线
            {
                Console.WriteLine("disconnect");
                nbs.Unregister(serial);         // 反注册事件, 免得后续干扰( 除非每次对比 serial )
                goto LabConnect;
            }
        }

        Console.WriteLine("response callback");
        if (recv == null)                       // RPC 超时, 进一步分析原因
        {
            if (nbs.alive)
            {
                Console.WriteLine("timeout");
                goto LabLogin;                  // 网络正常, 服务器返回超时, 继续发起登录
            }
            else
            {
                Console.WriteLine("disconnected");
                goto LabConnect;                // 网络断线超时: 跳到连接环节重连
            }
        }
        else
        {
            if (recv.GetType() == typeof(RPC.Generic.Error))
            {
                Console.WriteLine("recv " + recv.GetType() + " errMsg = " + ((RPC.Generic.Error)recv).errMsg);
                var ticks = nbs.ticks + 30;
                while (nbs.ticks < ticks)       // sleep 1秒( 带断线判断 )
                {
                    yield return null;
                    if (!nbs.alive) goto LabConnect;    // 在等待过程中如果断线, 跳转到重连
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
        while (nbs.alive)
        {
            yield return null;
            if (nbs.ticks % 30 == 0)            // 每秒 ping 一次
            {
                nbs.SendRequest(
                    new RPC.Generic.Ping        // 构造一个 ping 包
                    {
                        ticks = DateTime.Now.Ticks
                    }
                    , (s, ibb) =>
                    {
                        if (ibb == null)        // 超时: 就当 ping lost. 如果是断线, 外围 while 能处理
                        {
                            Console.WriteLine("ping timeout");
                        }
                        else
                        {
                            Console.WriteLine("ping " + ((double)(DateTime.Now.Ticks - ((RPC.Generic.Pong)ibb).ticks) / 10000) + " ms");
                        }
                    });
            }
        }

        // 断线重连
        Console.WriteLine("disconnected");
        goto LabConnect;

        //yield break;  // 退出
    }
    public void Leave()
    {
        if (nbs != null)
        {
            nbs.Dispose();
            nbs = null;
        }
    }
}


public static class Program
{
    static void Main(string[] args)
    {
        RPC.AllTypes.Register();                // 注册通信类 id 映射
        NBSocket.SockInit();                    // 初始化相关系统环境

        var us = new UnityScene();
        us.Enter();
        while (true)
        {
            if (!us.Update())
            {
                us.Leave();
                break;
            }
            Thread.Sleep(33);   // 模拟 30 fps 帧刷新
        }
    }
}
