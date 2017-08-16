using System;
using System.Diagnostics;
using System.Threading;

public static class Program
{
    static void Main(string[] args)
    {
        var sw = Stopwatch.StartNew();

        var log = new xx.ConcurrentLogger();
        log.batchSize = 10000;

        for (int j = 0; j < 1000000; j++)
        {
            log.WriteAll(xx.LogLevel.Info, DateTime.Now.Ticks,
                "pc1", "test_csharp3", Guid.NewGuid().ToString(),
                "asdfjlaskdjf", 22, @"hi 中文hello中文hello !"
            );
        }

        Console.WriteLine("logs filled.");

        while (log.counter < 1000000) Thread.Sleep(1);
        Console.WriteLine(sw.ElapsedMilliseconds);

        log.Dispose();
        Console.ReadLine();
    }
}

