using System;
using System.Collections.Concurrent;

/// <summary>
/// 整个服务的上下文
/// </summary>
public class Service
{
    /// <summary>
    /// uv 上下文
    /// </summary>
    public UVWrapper uv;

    /// <summary>
    /// 监听器 for server
    /// </summary>
    public Listener listener;

    /// <summary>
    /// 在非 UV 线程产生结果处理通知的信号量
    /// </summary>
    public Async dispatcher;

    /// <summary>
    /// 结果函数队列( 从非 uv 线程压入后 call asyncResult.Fire 来 Dispatch 到 uv 线程执行 )
    /// </summary>
    public ConcurrentQueue<Action> results = new ConcurrentQueue<Action>();

    /// <summary>
    /// 从非 uv 线程向 results 压入结果函数并通知 uv 线程执行
    /// </summary>
    public void PushResult(Action a)
    {
        results.Enqueue(a);
        dispatcher.Fire();      // 通知 uv 线程
    }

    /// <summary>
    /// 在线用户列表. key 为 id
    /// </summary>
    public xx.Dict<int, PKG.UserInfo> users = new xx.Dict<int, PKG.UserInfo>();

    public Service(UVWrapper uv)
    {
        this.uv = uv;
        dispatcher = uv.CreateAsync(new Async(this));
        listener = uv.CreateListener(new Listener(this), 12345, 128);
        // todo: 判断是否创建成功
        System.Console.WriteLine("chat server running...");
    }
}
