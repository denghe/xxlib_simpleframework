using System;
using System.Collections;
using System.Diagnostics;
using System.IO;
using System.Runtime.InteropServices;
using System.Threading;
using System.Threading.Tasks;
using xx;
using static LuaInterface.LuaDLL;

// todo: 需要用 lua 来实现 NBSocket 的 rpc 部分

// 模拟 unity 场景代码
public class UnityScene
{
    IntPtr L;
    public UnityScene()
    {
        L = luaL_newstate();
        tolua_openlibs(L);
        xxlua_openxx(L);
        var src = File.ReadAllText("src2.lua");
        var b = luaL_dostring(L, src);
        Console.WriteLine(b);
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
        //us.Enter();
        //while (true)
        //{
        //    if (!us.Update())
        //    {
        //        us.Leave();
        //        break;
        //    }
        //    Thread.Sleep(33);   // 模拟 30 fps 帧刷新
        //}
    }
}
