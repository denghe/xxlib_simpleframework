using System;
using System.Runtime.InteropServices;

public static class XxNBSocketInterop
{
#if !UNITY_EDITOR && UNITY_IPHONE
        const string DLL_NAME = "__Internal";
#else
    const string DLL_NAME = "xxnbsocketlib";
#endif

    /// <summary>
    /// 初始化网络系统( WSAStartup / signal ). 只需要一开始执行一次.
    /// </summary>
    [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    internal static extern void Init_Sock();

    /// <summary>
    /// 建 XxNBSocket( 传入 XxMemPool )
    /// </summary>
    [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    internal static extern IntPtr XxNBSocket_New(IntPtr mp);

    /// <summary>
    /// 杀 XxNBSocket
    /// </summary>
    [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    internal static extern void XxNBSocket_Delete(IntPtr nbs);

    /// <summary>
    /// 设 ip & port
    /// </summary>
    [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    internal static extern void XxNBSocket_SetAddress(IntPtr nbs, string ip, ushort port);

    /// <summary>
    /// 开始连接. 可传入阻塞时长
    /// 返回负数 表示出错. 0 表示没发生错误 但也没连上. 1 表示连接成功
    /// </summary>
    [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    internal static extern int XxNBSocket_Connect(IntPtr nbs, int sec, int usec);

    /// <summary>
    /// 断开连接. 可传入延迟多少 ticks 断开. 0 立即断开.
    /// </summary>
    [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    internal static extern void XxNBSocket_Disconnect(IntPtr nbs, int delayTicks);

    /// <summary>
    /// 帧驱动. 可传入阻塞时长( 仅对 Connecting 状态有效 )
    /// 返回负数表示出错. 0 表示没发生错误
    /// </summary>
    [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    internal static extern int XxNBSocket_Update(IntPtr nbs, int sec, int usec);

    /// <summary>
    /// 取当前状态. 
    /// Disconnected,		// 0: 初始状态 / 出错 / 成功断开后
    /// Connecting,			// 1: 执行 Connect 之后
    /// Connected,			// 2: 握手并进入可收发状态之后
    /// Disconnecting,		// 3: 执行 Disconnect( 延迟断开时长 ) 之后
    /// </summary>
    [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    internal static extern int XxNBSocket_GetState(IntPtr nbs);

    /// <summary>
    /// 取当前状态已持续的 ticks( Disconnecting 除外 )
    /// </summary>
    [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    internal static extern int XxNBSocket_GetTicks(IntPtr nbs);

    /// <summary>
    /// 发一段数据. 复制模式. 发送方不需要保持该数据不变.
    /// 返回负数表示出错. 0 表示成功放入待发送队列或无数据可发. > 0 表示已立刻发送成功的长度.
    /// 如果有剩下部分, 会放入待发送队列, 在下次 Update 时继续发
    /// </summary>
    [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    internal static extern int XxNBSocket_Send(IntPtr nbs, IntPtr buf, int dataLen);

    /// <summary>
    /// 如果接收缓冲区有包, 将返回 buf 指针填充长度. 否则返回 null( 表示无数据可取 )
    /// 流程: while( buf = PeekRecv( &dataLen ) ) {  ... PopRecv(); }
    /// </summary>
    [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    internal static extern IntPtr XxNBSocket_PeekRecv(IntPtr nbs, ref int dataLen);

    /// <summary>
    /// 对于已处理的 PeekRecv 的数据, 用这个函数来弹出删掉, 以便继续 Peek 下一条.
    /// </summary>
    [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    internal static extern void XxNBSocket_PopRecv(IntPtr nbs);

    /// <summary>
    /// XxNBSocket_Send 的 XxBBuffer 版
    /// </summary>
    [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    internal static extern int XxNBSocket_SendXxBBuffer(IntPtr nbs, IntPtr bb);

    /// <summary>
    /// XxNBSocket_PeekRecv 的 XxBBuffer 版
    /// </summary>
    [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    internal static extern IntPtr XxNBSocket_PeekRecvXxBBuffer(IntPtr nbs);

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
    /// 指向内存池
    /// </summary>
    XxMemPool mempool;

    /// <summary>
    /// XxNBSocket 指针
    /// </summary>
    IntPtr pointer;

    public XxNBSocket(XxMemPool mempool)
    {
        this.mempool = mempool;
        pointer = XxNBSocketInterop.XxNBSocket_New(mempool.pointer);
        if (pointer == IntPtr.Zero)
        {
            throw new Exception("XxNBSocket_New failed.");
        }
    }

    /// <summary>
    /// 初始化网络系统( WSAStartup / signal ). 只需要一开始执行一次.
    /// </summary>
    public static void SockInit()
    {
        XxNBSocketInterop.Init_Sock();
    }

    /// <summary>
    /// 设 ip & port
    /// </summary>
    public void SetAddress(string ip, ushort port)
    {
        XxNBSocketInterop.XxNBSocket_SetAddress(pointer, ip, port);
    }

    /// <summary>
    /// 开始连接. 可传入阻塞时长
    /// 返回负数 表示出错. 0 表示没发生错误 但也没连上. 1 表示连接成功
    /// </summary>
    public int Connect(int sec = 0, int usec = 0)
    {
        return XxNBSocketInterop.XxNBSocket_Connect(pointer, sec, usec);
    }

    /// <summary>
    /// 断开连接. 可传入延迟多少 ticks 断开. 0 立即断开.
    /// </summary>
    public void Disconnect(int delayTicks = 0)
    {
        XxNBSocketInterop.XxNBSocket_Disconnect(pointer, delayTicks);
    }

    /// <summary>
    /// 帧驱动. 可传入阻塞时长( 仅对 Connecting 状态有效 )
    /// 返回负数表示出错. 0 表示没发生错误
    /// </summary>
    public int Update(int sec = 0, int usec = 0)
    {
        return XxNBSocketInterop.XxNBSocket_Update(pointer, sec, usec);
    }

    /// <summary>
    /// 取当前状态. 
    /// </summary>
    public States state
    {
        get
        {
            return (States)XxNBSocketInterop.XxNBSocket_GetState(pointer);
        }
    }

    /// <summary>
    /// 取当前状态已持续的 ticks( Disconnecting 除外 )
    /// </summary>
    public int ticks
    {
        get
        {
            return XxNBSocketInterop.XxNBSocket_GetTicks(pointer);
        }
    }

    /// <summary>
    /// 发一段数据. 复制模式. 发送方不需要保持该数据不变.
    /// 返回负数表示出错. 0 表示成功放入待发送队列或无数据可发. > 0 表示已立刻发送成功的长度.
    /// 如果有剩下部分, 会放入待发送队列, 在下次 Update 时继续发
    /// </summary>
    public int Send(byte[] buf, int dataLen = 0)
    {
        if (dataLen == 0) dataLen = buf.Length;
        var h = GCHandle.Alloc(buf, GCHandleType.Pinned);
        var r = XxNBSocketInterop.XxNBSocket_Send(pointer, h.AddrOfPinnedObject(), dataLen);
        h.Free();
        return r;
    }

    /// <summary>
    /// 如果接收缓冲区有包, 将返回 buf 指针填充长度. 否则返回 null( 表示无数据可取 )
    /// 流程: Begin: buf = PopRecv(); if buf is null break; else ....... goto Begin;
    /// </summary>
    public byte[] PopRecv()
    {
        int dataLen = 0;
        var bufPtr = XxNBSocketInterop.XxNBSocket_PeekRecv(pointer, ref dataLen);
        if (bufPtr == IntPtr.Zero) return null;
        var rtv = new byte[dataLen];
        Marshal.Copy(bufPtr, rtv, 0, dataLen);
        XxNBSocketInterop.XxNBSocket_PopRecv(pointer);
        return rtv;
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
            XxNBSocketInterop.XxNBSocket_Delete(pointer);
            pointer = IntPtr.Zero;
            mempool = null;
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
