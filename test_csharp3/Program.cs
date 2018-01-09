using System;
using System.Diagnostics;
using System.Runtime.InteropServices;
using System.Threading;
using xx;
using static ConsoleHelpeer;
public static class ConsoleHelpeer
{
    public static void CW(object o)
    {
#if DEBUG
        Console.WriteLine(o);
#endif
    }
}

public static class Program
{
    static void Main(string[] args)
    {
        BBuffer.Register<BBuffer>(2);           // 要用到 BBuffer 的收发

        // 初始化 loop 的同时初始化 rpc 管理器. 计时精度 1 秒. 默认 2 秒超时
        using (var loop = new UvLoop(1000, 2))
        {

            loop.Run();
        }
    }
}
