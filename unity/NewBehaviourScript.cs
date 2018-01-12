using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class NewBehaviourScript : MonoBehaviour
{
    void Start()
    {
        Application.targetFrameRate = 30;       // 限制帧数( 需要去 Unity 下改 Edit -- Project Settings -- Quality -- V Sync Count 为 Don't Sync )
        iter = UpdateCore().GetEnumerator();
        nbs = new xx.NBSocket(30 * 5);
    }
    IEnumerator iter;
    xx.NBSocket nbs;
    void Update()
    {
        iter.MoveNext();
        nbs.Update();
    }

    public UnityEngine.UI.Text txtOutput;
    void Output(object o)
    {
        //Output(o);
        txtOutput.text = o.ToString();
    }
    IEnumerable UpdateCore()
    {
        nbs.SetAddress("220.181.57.217", 80);   // www.baidu.com
        Output("init");

        LabConnect:                             // 循环切入点
        yield return null;                      // 避免后续 if + goto 中间缺驱动环节

        nbs.Connect();                          // 下达任务 并等结果
        Output("connecting...");
        while (true)
        {
            yield return null;                  // while 内必配一句 yield 防独占 cpu

            if (nbs.state != xx.NBSocketStates.Connecting) break;  // 连接成功或失败
            if (nbs.ticks > 30 * 2) break;      // 超时
        }

        Output("event occur");
        switch (nbs.state)                      // 处理结果
        {
            case xx.NBSocketStates.Disconnected:   // 超时时间内没连上, 可能是网络或系统故障或被目标服务器拒绝\
                var ticks = nbs.ticks + 30;
                while (nbs.ticks < ticks)       // sleep 1秒
                {
                    yield return null;          // 这里可以放置其他判断以及时跳出
                }
                Output("can't connect to server");
                goto LabConnect;                // 跳到开始连接处, 继续上述流程
            case xx.NBSocketStates.Connecting:     // 超时
                nbs.Disconnect();               // 立即断开
                Output("timeout");
                goto LabConnect;                // 跳到开始连接处, 继续上述流程 
        }
        Output("connect success");              // 已连上, 继续执行下面的逻辑代码


        {
            var ticks = nbs.ticks + 30;         // 连上后等一会儿
            while (nbs.ticks < ticks) yield return null;
        }

        nbs.Disconnect();                       // 断开
        goto LabConnect;                        // 跳至开头( 永不退出 )
    }

}
