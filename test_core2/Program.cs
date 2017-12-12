using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Runtime.InteropServices;
using xx;

public static class Program
{
    static void Main(string[] args)
    {
        // loop
        var loop = new UvLoop();
        var timer = new UvTimer(loop, 1000, 100);
        timer.OnFire = () => Console.Write(".");
        loop.Run();
    }
}
