using System.Diagnostics;
using System.Threading;
using System.Threading.Tasks;

public class Worker
{
    public volatile int i1, i2;

    void WorkCB()
    {
        System.Threading.Interlocked.Increment(ref i1);
        //Thread.Sleep(1000);
        //System.Console.WriteLine("sleep");
    }
    void AfterWorkCB()
    {
        System.Threading.Interlocked.Increment(ref i2);
    }
    public void Execute()
    {
        for (int i = 0; i < 10000000; ++i)
        {
            var t = new Task(() => { WorkCB(); });
            t.Start();
            Task.WaitAll(t);                        // 测试结果为约 50 万次 每秒
            AfterWorkCB();
        }
    }
}

public static class Program
{
    static void Main(string[] args)
    {
        var w = new Worker();
        var sw = Stopwatch.StartNew();
        w.Execute();
        System.Console.WriteLine(sw.ElapsedMilliseconds +  " i1 = " + w.i1 + " i2 = " + w.i2);
    }
}
