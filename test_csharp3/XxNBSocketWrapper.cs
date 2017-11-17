using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

public static class XxNBSocketInterop
{
    /// <summary>
    /// 初始化网络系统( WSAStartup / signal ). 只需要一开始执行一次.
    /// </summary>
    [DllImport("xxnbsocketlib", CallingConvention = CallingConvention.Cdecl)]
    internal static extern void SockInit();

    /// <summary>
    /// 建 XxMemPool
    /// </summary>
    /// <returns></returns>
    [DllImport("xxnbsocketlib", CallingConvention = CallingConvention.Cdecl)]
    internal static extern IntPtr NewXxMemPool();

    /// <summary>
    /// 建 XxNBSocket( 传入 XxMemPool )
    /// </summary>
    [DllImport("xxnbsocketlib", CallingConvention = CallingConvention.Cdecl)]
    internal static extern IntPtr NewXxNBSocket(IntPtr mp);

    /// <summary>
    /// 杀 XxNBSocket
    /// </summary>
    [DllImport("xxnbsocketlib", CallingConvention = CallingConvention.Cdecl)]
    internal static extern void DeleteXxNBSocket(IntPtr nbs);

    /// <summary>
    /// 杀 XxMemPool( 在 XxNBSocket 之后再杀 )
    /// </summary>
    [DllImport("xxnbsocketlib", CallingConvention = CallingConvention.Cdecl)]
    internal static extern void DeleteXxMemPool(IntPtr mp);

    /// <summary>
    /// 设 ip & port
    /// </summary>
    [DllImport("xxnbsocketlib", CallingConvention = CallingConvention.Cdecl)]
    internal static extern void SetAddress(IntPtr nbs, string ip, ushort port);

    /// <summary>
    /// 开始连接. 可传入阻塞时长
    /// 返回负数 表示出错. 0 表示没发生错误 但也没连上. 1 表示连接成功
    /// </summary>
    [DllImport("xxnbsocketlib", CallingConvention = CallingConvention.Cdecl)]
    internal static extern int Connect(IntPtr nbs, int sec, int usec);

    /// <summary>
    /// 断开连接. 可传入延迟多少 ticks 断开. 0 立即断开.
    /// </summary>
    [DllImport("xxnbsocketlib", CallingConvention = CallingConvention.Cdecl)]
    internal static extern void Disconnect(IntPtr nbs, int delayTicks);

    /// <summary>
    /// 帧驱动. 可传入阻塞时长( 仅对 Connecting 状态有效 )
    /// 返回负数表示出错. 0 表示没发生错误
    /// </summary>
    [DllImport("xxnbsocketlib", CallingConvention = CallingConvention.Cdecl)]
    internal static extern int Update(IntPtr nbs, int sec, int usec);

    /// <summary>
    /// 取当前状态. 
    /// Disconnected,		// 0: 初始状态 / 出错 / 成功断开后
    /// Connecting,			// 1: 执行 Connect 之后
    /// Connected,			// 2: 握手并进入可收发状态之后
    /// Disconnecting,		// 3: 执行 Disconnect( 延迟断开时长 ) 之后
    /// </summary>
    [DllImport("xxnbsocketlib", CallingConvention = CallingConvention.Cdecl)]
    internal static extern int GetState(IntPtr nbs);

    /// <summary>
    /// 取当前状态已持续的 ticks( Disconnecting 除外 )
    /// </summary>
    [DllImport("xxnbsocketlib", CallingConvention = CallingConvention.Cdecl)]
    internal static extern int GetTicks(IntPtr nbs);

    /// <summary>
    /// 发一段数据. 复制模式. 发送方不需要保持该数据不变.
    /// 返回负数表示出错. 0 表示成功放入待发送队列或无数据可发. > 0 表示已立刻发送成功的长度.
    /// 如果有剩下部分, 会放入待发送队列, 在下次 Update 时继续发
    /// </summary>
    [DllImport("xxnbsocketlib", CallingConvention = CallingConvention.Cdecl)]
    internal static extern int Send(IntPtr nbs, IntPtr buf, int dataLen);

    /// <summary>
    /// 如果接收缓冲区有包, 将返回 buf 指针填充长度. 否则返回 null( 表示无数据可取 )
    /// 流程: while( buf = PeekRecv( &dataLen ) ) {  ... PopRecv(); }
    /// </summary>
    [DllImport("xxnbsocketlib", CallingConvention = CallingConvention.Cdecl)]
    internal static extern IntPtr PeekRecv(IntPtr nbs, ref int dataLen);

    /// <summary>
    /// 对于已处理的 PeekRecv 的数据, 用这个函数来弹出删掉, 以便继续 Peek 下一条.
    /// </summary>
    [DllImport("xxnbsocketlib", CallingConvention = CallingConvention.Cdecl)]
    internal static extern void PopRecv(IntPtr nbs);
}


public class XxNBSocket : IDisposable
{
    /// <summary>
    /// 连接状态
    /// </summary>
    public enum States
    {
        /// <summary>
        /// 初始状态 / 出错 / 成功断开后
        /// </summary>
        Disconnected,
        /// <summary>
        /// 执行 Connect 之后
        /// </summary>
        Connecting,
        /// <summary>
        /// 握手并进入可收发状态之后
        /// </summary>
        Connected,
        /// <summary>
        /// 执行 Disconnect( 延迟断开时长 ) 之后
        /// </summary>
        Disconnecting,
    }

    /// <summary>
    /// 内存池 指针
    /// </summary>
    IntPtr mp;

    /// <summary>
    /// XxNBSocket 指针
    /// </summary>
    IntPtr nbs;

    public XxNBSocket()
    {
        mp = XxNBSocketInterop.NewXxMemPool();
        if (mp == IntPtr.Zero)
        {
            throw new Exception("NewXxMemPool failed.");
        }
        nbs = XxNBSocketInterop.NewXxNBSocket(mp);
        if (nbs == IntPtr.Zero)
        {
            throw new Exception("NewXxNBSocket failed.");
        }
    }

    /// <summary>
    /// 初始化网络系统( WSAStartup / signal ). 只需要一开始执行一次.
    /// </summary>
    public static void SockInit()
    {
        XxNBSocketInterop.SockInit();
    }

    /// <summary>
    /// 设 ip & port
    /// </summary>
    public void SetAddress(string ip, ushort port)
    {
        if (disposed) throw new ObjectDisposedException("NBSocket is disposed.");
        XxNBSocketInterop.SetAddress(nbs, ip, port);
    }

    /// <summary>
    /// 开始连接. 可传入阻塞时长
    /// 返回负数 表示出错. 0 表示没发生错误 但也没连上. 1 表示连接成功
    /// </summary>
    public int Connect(int sec = 0, int usec = 0)
    {
        if (disposed) throw new ObjectDisposedException("NBSocket is disposed.");
        return XxNBSocketInterop.Connect(nbs, sec, usec);
    }

    /// <summary>
    /// 断开连接. 可传入延迟多少 ticks 断开. 0 立即断开.
    /// </summary>
    public void Disconnect(int delayTicks = 0)
    {
        if (disposed) throw new ObjectDisposedException("NBSocket is disposed.");
        XxNBSocketInterop.Disconnect(nbs, delayTicks);
    }

    /// <summary>
    /// 帧驱动. 可传入阻塞时长( 仅对 Connecting 状态有效 )
    /// 返回负数表示出错. 0 表示没发生错误
    /// </summary>
    public int Update(int sec = 0, int usec = 0)
    {
        if (disposed) throw new ObjectDisposedException("NBSocket is disposed.");
        return XxNBSocketInterop.Update(nbs, sec, usec);
    }

    /// <summary>
    /// 取当前状态. 
    /// </summary>
    public States GetState()
    {
        if (disposed) throw new ObjectDisposedException("NBSocket is disposed.");
        return (States)XxNBSocketInterop.GetState(nbs);
    }

    /// <summary>
    /// 取当前状态. 
    /// </summary>
    public States state { get { return GetState(); } }

    /// <summary>
    /// 取当前状态已持续的 ticks( Disconnecting 除外 )
    /// </summary>
    public int GetTicks()
    {
        if (disposed) throw new ObjectDisposedException("NBSocket is disposed.");
        return XxNBSocketInterop.GetTicks(nbs);
    }

    /// <summary>
    /// 取当前状态已持续的 ticks( Disconnecting 除外 )
    /// </summary>
    public int ticks { get { return GetTicks(); } }

    /// <summary>
    /// 发一段数据. 复制模式. 发送方不需要保持该数据不变.
    /// 返回负数表示出错. 0 表示成功放入待发送队列或无数据可发. > 0 表示已立刻发送成功的长度.
    /// 如果有剩下部分, 会放入待发送队列, 在下次 Update 时继续发
    /// </summary>
    public int Send(byte[] buf, int dataLen = 0)
    {
        if (disposed) throw new ObjectDisposedException("NBSocket is disposed.");
        if (dataLen == 0) dataLen = buf.Length;
        var h = GCHandle.Alloc(buf, GCHandleType.Pinned);
        var r = XxNBSocketInterop.Send(nbs, h.AddrOfPinnedObject(), dataLen);
        h.Free();
        return r;
    }

    /// <summary>
    /// 如果接收缓冲区有包, 将返回 buf 指针填充长度. 否则返回 null( 表示无数据可取 )
    /// 流程: while( buf = PeekRecv( &dataLen ) ) {  ... PopRecv(); }
    /// </summary>
    public byte[] PeekRecv()
    {
        if (disposed) throw new ObjectDisposedException("NBSocket is disposed.");
        int dataLen = 0;
        var bufPtr = XxNBSocketInterop.PeekRecv(nbs, ref dataLen);
        if (bufPtr == IntPtr.Zero) return null;
        var rtv = new byte[dataLen];
        Marshal.Copy(bufPtr, rtv, 0, dataLen);
        return rtv;
    }

    /// <summary>
    /// 对于已处理的 PeekRecv 的数据, 用这个函数来弹出删掉, 以便继续 Peek 下一条.
    /// </summary>
    public void PopRecv()
    {
        if (disposed) throw new ObjectDisposedException("NBSocket is disposed.");
        XxNBSocketInterop.PopRecv(nbs);
    }

    #region Dispose

    private bool disposed = false;

    //Implement IDisposable.
    public void Dispose()
    {
        Dispose(true);
        GC.SuppressFinalize(this);
    }

    protected virtual void Dispose(bool disposing)
    {
        if (!disposed)
        {
            if (disposing)
            {
                // Free other state (managed objects).
            }
            // Free your own state (unmanaged objects).
            // Set large fields to null.
            XxNBSocketInterop.DeleteXxNBSocket(nbs);
            nbs = IntPtr.Zero;
            XxNBSocketInterop.DeleteXxMemPool(mp);
            mp = IntPtr.Zero;
            disposed = true;
        }
        // Call Dispose in the base class.
        //base.Dispose(disposing);
    }

    // Use C# destructor syntax for finalization code.
    ~XxNBSocket()
    {
        Dispose(false);
    }

    // The derived class does not have a Finalize method
    // or a Dispose method without parameters because it inherits
    // them from the base class.

    #endregion
}
