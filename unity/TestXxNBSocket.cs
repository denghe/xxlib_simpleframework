using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using UnityEngine;


public static class Program
{
    public static XxMemPool mp = new XxMemPool();
    public static XxNBSocket nbs = new XxNBSocket(mp);

    static Program()
    {
        XxNBSocket.SockInit();
    }
}

public class test : MonoBehaviour
{
    string log = "";
    XxNBSocket nbs = Program.nbs;

    // Use this for initialization
    void Start()
    {
        Dump();
        nbs.SetAddress("192.168.1.111", 12345);
        nbs.Connect();
        Dump();
    }

    void Dump()
    {
        //Debug.Log("state = " + nbs.state.ToString() + ", ticks = " + nbs.ticks);
        log += "state = " + nbs.state.ToString() + ", ticks = " + nbs.ticks + "\n";
    }

    bool running = true;
    // Update is called once per frame
    void Update()
    {
        if (!running) return;
        switch (nbs.state)
        {
            case XxNBSocket.States.Disconnected:
                running = false;
                break;
            case XxNBSocket.States.Connecting:
                if (nbs.ticks > 10)    // 1 秒连不上就算超时吧
                {
                    nbs.Disconnect();
                }
                break;
            case XxNBSocket.States.Connected:
                // 刚连上时发包
                if (nbs.ticks == 0) nbs.Send(new byte[]
                {
                                4, 0, 49, 50, 51, 52,
                                4, 0, 53, 54, 55, 56
                });

                // dump 收到的包
                LabRetry:
                var buf = nbs.PopRecv();
                if (buf != null)
                {
                    Debug.Log("recv pkg. len = " + buf.Length);
                    goto LabRetry;
                }

                if (nbs.ticks > 10) nbs.Disconnect(2);  // 存活 10 帧后, 延迟 2 帧杀掉
                break;
            case XxNBSocket.States.Disconnecting:
                break;

            default:
                break;
        }
        Dump();
        nbs.Update();
    }

    void OnGUI()
    {
        GUI.Label(new Rect(100, Screen.height / 2 - 100, 600, 400), log);
    }

}
