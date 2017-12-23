using System;
using System.Collections;
using System.Diagnostics;
using System.Runtime.InteropServices;
using System.Threading;
using System.Threading.Tasks;
using xx;

// todo: 拿 tolua 的 dll 来 import 后简单测试 lua 部分

// 模拟 unity 场景代码
public class UnityScene
{
    public UnityScene()
    {
    }
    public bool Update()
    {
        return false;
    }

    public void Enter()
    {
    }
    public void Leave()
    {
    }
}


public static class Program
{
    static void Main(string[] args)
    {
        // todo

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
