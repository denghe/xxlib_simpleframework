using System;

/// <summary>
/// 跨线程事件触发器. 当前用于在非 uv 线程中 向结果队列塞入 action 之后 发起通知, 令 uv 线程去执行
/// </summary>
public class Async : UVAsyncWrapper
{
    Service owner;
    public Async(Service owner)
    {
        this.owner = owner;
    }

    public override void OnFire()
    {
        while (owner.results.TryDequeue(out var a)) a();
    }
}
