using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Runtime.InteropServices;

public static class Program
{
    static void Main(string[] args)
    {
        // loop
        var loop = new XxUvLoop();
        var timer = new XxUvTimer(loop, 1000, 100);
        timer.OnFire = () => Console.Write(".");
        loop.Run();
    }
}
