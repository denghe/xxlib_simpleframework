using System;

public static class Utils
{
    public static long GetCurrentMS()
    {
        return (System.DateTime.Now.Ticks - 621355968000000000) / 10000;
    }
}


/// <summary>
/// 流水号 -- 上下文 映射管理器
/// </summary>
public class SerialContextManager
{
    /// <summary>
    /// 循环使用的自增流水号( 不直接使用字典下标是考虑到 超时晚回造成错位的情况 )
    /// </summary>
    protected int serial;

    /// <summary>
    /// 流水号 与 上下文 的映射
    /// </summary>
    protected xx.Dict<int, object> mapping = new xx.Dict<int, object>();

    /// <summary>
    /// 请求一个 流水号 并 映射 上下文
    /// </summary>
    public int Push(object p)
    {
        unchecked { ++serial; }                                                 // 循环自增
        var r = mapping.Add(serial, p, true);                                   // 建立 serial 与 peer 的映射
        serials.Push(new Tuple<int, long>(serial, Utils.GetCurrentMS()));      // 将 key 和 时间戳 放入队列
        return serial;
    }

    /// <summary>
    /// 根据 流水号 取出 上下文
    /// </summary>
    public object Pop(int serial)
    {
        int idx = mapping.Find(serial);
        if (idx == -1) return null;                                             // 可能是晚到的消息, 本地已超时清理
        var pc = mapping.ValueAt(idx);
        mapping.RemoveAt(idx);
        return pc;
    }

    /// <summary>
    /// 为保护字典空间, 用一个队列记录流水号的超时时间, 以便删掉超时的
    /// </summary>
    protected xx.Queue<Tuple<int, long>> serials = new xx.Queue<Tuple<int, long>>();

    /// <summary>
    /// 不断将超时的从字典移除, 直到 Pop 到未超时的停止.
    /// </summary>
    public void ClearTimeoutSerials(long currMS)
    {
        if (serials.IsEmpty) return;
        while (!serials.IsEmpty && currMS - serials.Top().Item2 > 5000)     // 清到超过 5 秒钟的流水号
        {
            mapping.Remove(serials.Top().Item1);
            serials.Pop();
        }
    }
}



public abstract class UVServerPeerWrapperEx : UVServerPeerWrapper
{
    /// <summary>
    /// 复用的一个壳. 每次用 Assign 来替换它的 buf
    /// </summary>
    public xx.BBuffer bbRecv = new xx.BBuffer();

    /// <summary>
    /// 发送复用
    /// </summary>
    public xx.BBuffer bbSend = new xx.BBuffer();

    /// <summary>
    /// 用于临时填充 recv 的包
    /// </summary>
    public xx.List<xx.IBBuffer> recvPkgs = new xx.List<xx.IBBuffer>();

    /// <summary>
    /// 发出多个包( 不合并, 每个类都打包成一个独立的包发出 ), 返回发出的总字节数( 含包头 )
    /// </summary>
    public int SendPackages(params xx.IBBuffer[] pkgs)
    {
        if (pkgs == null || pkgs.Length == 0) throw new Exception("can't be send null");
        var sum = 0;
        var ibbsLength = pkgs.Length;
        for (int i = 0; i < ibbsLength; ++i)
        {
            var pkg = pkgs[i];
            bbSend.Clear();
            bbSend.BeginWritePackage();
            bbSend.WriteRoot(pkg);
            if (!bbSend.EndWritePackage()) return -1;
            sum += bbSend.dataLen;
            if (!Send(bbSend.buf, 0, bbSend.dataLen)) return -2;
        }
        return sum;
    }

    /// <summary>
    /// 批量发包( 合并成一个包发出, 要小心超出尺寸 ), 返回发出的总字节数( 含包头 )
    /// </summary>
    public int SendCombine(params xx.IBBuffer[] ibbs)
    {
        bbSend.Clear();
        bbSend.BeginWritePackage();
        var ibbsLength = ibbs.Length;
        for (int i = 0; i < ibbsLength; ++i)
        {
            var ibb = ibbs[i];
            bbSend.WriteRoot(ibb);
        }
        if (!bbSend.EndWritePackage()) return -1;
        if (!Send(bbSend.buf, 0, bbSend.dataLen)) return -2;
        return bbSend.dataLen;
    }

}

public abstract class UVClientPeerWrapperEx : UVClientPeerWrapper
{
    /// <summary>
    /// 复用的一个壳. 每次用 Assign 来替换它的 buf
    /// </summary>
    public xx.BBuffer bbRecv = new xx.BBuffer();

    /// <summary>
    /// 发送复用
    /// </summary>
    public xx.BBuffer bbSend = new xx.BBuffer();

    /// <summary>
    /// 用于临时填充 recv 的包
    /// </summary>
    public xx.List<xx.IBBuffer> recvPkgs = new xx.List<xx.IBBuffer>();

    /// <summary>
    /// 发出多个包( 不合并, 每个类都打包成一个独立的包发出 ), 返回发出的总字节数( 含包头 )
    /// </summary>
    public int SendPackages(params xx.IBBuffer[] pkgs)
    {
        if (pkgs == null || pkgs.Length == 0) throw new Exception("can't be send null");
        var sum = 0;
        var ibbsLength = pkgs.Length;
        for (int i = 0; i < ibbsLength; ++i)
        {
            var pkg = pkgs[i];
            bbSend.Clear();
            bbSend.BeginWritePackage();
            bbSend.WriteRoot(pkg);
            if (!bbSend.EndWritePackage()) return -1;
            sum += bbSend.dataLen;
            if (!Send(bbSend.buf, 0, bbSend.dataLen)) return -2;
        }
        return sum;
    }

    /// <summary>
    /// 批量发包( 合并成一个包发出, 要小心超出尺寸 ), 返回发出的总字节数( 含包头 )
    /// </summary>
    public int SendCombine(params xx.IBBuffer[] ibbs)
    {
        bbSend.Clear();
        bbSend.BeginWritePackage();
        var ibbsLength = ibbs.Length;
        for (int i = 0; i < ibbsLength; ++i)
        {
            var ibb = ibbs[i];
            bbSend.WriteRoot(ibb);
        }
        if (!bbSend.EndWritePackage()) return -1;
        if (!Send(bbSend.buf, 0, bbSend.dataLen)) return -2;
        return bbSend.dataLen;
    }

}
