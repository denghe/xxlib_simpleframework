using System;
using System.Diagnostics;

/// <summary>
/// 跨线程事件触发器. 当前用于在非 uv 线程中 向结果队列塞入 action 之后 发起通知, 令 uv 线程去执行
/// </summary>
public class Async : xx.UVAsyncWrapper
{
    Service owner;
    public Async(Service owner)
    {
        this.owner = owner;
    }

    bool executing = false;
    public override void OnFire()
    {
        Debug.Assert(executing == false);
        executing = true;
        while (owner.results.TryDequeue(out var a)) a();
        executing = false;
    }
}
