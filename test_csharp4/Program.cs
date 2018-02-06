using System;
using System.Collections;
using System.Diagnostics;
using System.Runtime.InteropServices;
using System.Threading;


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

    xx.NBSocket nbs;
    public void Enter()
    {
        nbs = new xx.NBSocket(30 * 5);
    }
    IEnumerable UpdateCore()
    {
        nbs.SetAddress("127.0.0.1", 12345);
        Console.WriteLine("init");

        LabConnect:                             // 循环切入点
        yield return null;                      // 避免后续 if + goto 中间缺驱动环节

        nbs.Connect();                          // 下达任务 并等结果
        Console.WriteLine("connecting...");
        while (true)
        {
            yield return null;                  // while 内必配一句 yield 防独占 cpu

            if (nbs.state != xx.NBSocketStates.Connecting) break;  // 连接成功或失败
            if (nbs.ticks > 30 * 2) break;      // 超时
        }

        Console.WriteLine("event occur");
        switch (nbs.state)                      // 处理结果
        {
            case xx.NBSocketStates.Disconnected:   // 超时时间内没连上, 可能是网络或系统故障或被目标服务器拒绝\
                var ticks = nbs.ticks + 30;
                while (nbs.ticks < ticks)       // sleep 1秒
                {
                    yield return null;          // 这里可以放置其他判断以及时跳出
                }
                Console.WriteLine("can't connect to server");
                goto LabConnect;                // 跳到开始连接处, 继续上述流程
            case xx.NBSocketStates.Connecting:     // 超时
                nbs.Disconnect();               // 立即断开
                Console.WriteLine("timeout");
                goto LabConnect;                // 跳到开始连接处, 继续上述流程 
        }
        Console.WriteLine("connect success");   // 已连上, 继续执行下面的逻辑代码

        LabEcho:
        yield return null;

        Console.WriteLine("随便发点啥, 等 echo");

        // 收包容器
        xx.Queue<xx.IBBuffer> recvs = new xx.Queue<xx.IBBuffer>();

        // 设置收包事件
        nbs.OnReceivePackage = (ibb) => 
        {
            recvs.Push(ibb);
        };

        // 准备点要发的数据
        var bb = new xx.BBuffer();
        ulong count = 0;
        bb.Write(count);

        // 告知系统, 开始发bb
        nbs.Send(bb);

        Console.WriteLine("wait echo");
        while (nbs.alive)                       // 如果连接没断开就一直扫结果
        {
            yield return null;

            // 收到数据的处理
            xx.IBBuffer ibb;
            while(recvs.TryDequeue(out ibb))
            {
                // count 取出来 +1 再发出去
                var recv = ibb as xx.BBuffer;
                recv.Read(ref count);
                ++count;
                recv.Clear();
                recv.Write(count);
                nbs.Send(recv);
                Console.WriteLine(count);
            }
        }

        // 断线重连
        goto LabConnect;

        //yield break;  // 协程退出
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
        xx.BBuffer.RegisterInternals();
        xx.NBSocket.SockInit();

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
