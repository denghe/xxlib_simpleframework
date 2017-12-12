using System;
using System.Collections.Concurrent;
using System.Runtime.InteropServices;
using System.Text;

// todo: c api 过来的 cb 可能要加 try

public class XxUvLoop : IDisposable
{
    public bool disposed;
    public XxSimpleList<XxUvTcpListener> listeners = new XxSimpleList<XxUvTcpListener>();
    public XxSimpleList<XxUvTcpClient> clients = new XxSimpleList<XxUvTcpClient>();
    public XxSimpleList<XxUvTimer> timers = new XxSimpleList<XxUvTimer>();
    public XxSimpleList<XxUvAsync> asyncs = new XxSimpleList<XxUvAsync>();

    public IntPtr ptr;
    public IntPtr handle;

    public XxUvLoop()
    {
        this.Handle(ref handle);
        ptr = XxUvInterop.xxuv_alloc_uv_loop_t(handle);
        if (ptr == IntPtr.Zero)
        {
            this.Unhandle(ref handle);
            throw new NullReferenceException();
        }

        int r = XxUvInterop.xxuv_loop_init(ptr);
        if (r != 0)
        {
            this.Free(ref ptr);
            this.Unhandle(ref handle);
            r.Throw();
        }
    }

    public void Run(XxUvRunMode mode = XxUvRunMode.Default)
    {
        if (disposed) throw new ObjectDisposedException("XxUvLoop");
        XxUvInterop.xxuv_run(ptr, mode).TryThrow();
    }

    #region Dispose

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

            XxUvInterop.xxuv_loop_close(ptr).TryThrow();
            this.Free(ref ptr);
            this.Unhandle(ref handle);
            disposed = true;
        }
        // Call Dispose in the base class.
        //base.Dispose(disposing);
    }

    // Use C# destructor syntax for finalization code.
    ~XxUvLoop()
    {
        Dispose(false);
    }

    // The derived class does not have a Finalize method
    // or a Dispose method without parameters because it inherits
    // them from the base class.

    #endregion
}

public class XxUvTcpListener : IDisposable
{
    /******************************************************************************/
    // 用户事件绑定区
    public Func<XxUvTcpPeer> OnCreatePeer;
    public Action<XxUvTcpPeer> OnAccept;
    public Action OnDispose;
    /******************************************************************************/

    public bool disposed;
    public XxUvLoop loop;
    public XxSimpleList<XxUvTcpPeer> peers = new XxSimpleList<XxUvTcpPeer>();
    public int index_at_container;

    public IntPtr ptr;
    public IntPtr handle;
    public IntPtr addrPtr;

    public XxUvTcpListener(XxUvLoop loop)
    {
        this.Handle(ref handle);
        this.loop = loop;

        ptr = XxUvInterop.xxuv_alloc_uv_tcp_t(handle);
        if (ptr == IntPtr.Zero)
        {
            this.Unhandle(ref handle);
            throw new OutOfMemoryException();
        }

        int r = XxUvInterop.xxuv_tcp_init(loop.ptr, ptr);
        if (r != 0)
        {
            this.Free(ref ptr);
            this.Unhandle(ref handle);
            r.Throw();
        }

        addrPtr = XxUvInterop.xxuv_alloc_sockaddr_in(IntPtr.Zero);
        if (addrPtr == IntPtr.Zero)
        {
            XxUvInterop.xxuv_close_(ptr);
            this.Free(ref ptr);
            this.Unhandle(ref handle);
            throw new OutOfMemoryException();
        }

        index_at_container = loop.listeners.dataLen;
        loop.listeners.Add(this);
    }


    static XxUvInterop.uv_connection_cb OnAcceptCB = OnAcceptCBImpl;
    static void OnAcceptCBImpl(IntPtr stream, int status)
    {
        status.TryThrow();
        var listener = stream.To<XxUvTcpListener>();
        XxUvTcpPeer peer = null;
        try
        {
            if (listener.OnCreatePeer != null) peer = listener.OnCreatePeer();
            else peer = new XxUvTcpPeer(listener.loop, listener);
        }
        catch
        {
            return;
        }
        listener.OnAccept(peer);
    }
    public void Listen(int backlog = 128)
    {
        if (disposed) throw new ObjectDisposedException("XxUvTcpListener");
        XxUvInterop.xxuv_listen(ptr, backlog, OnAcceptCB).TryThrow();
    }


    public void Bind(string ipv4, int port)
    {
        if (disposed) throw new ObjectDisposedException("XxUvTcpListener");
        XxUvInterop.xxuv_ip4_addr(ipv4, port, addrPtr).TryThrow();
        XxUvInterop.xxuv_tcp_bind_(ptr, addrPtr).TryThrow();
    }

    #region Dispose

    //Implement IDisposable.
    public void Dispose()
    {
        if (OnDispose != null) OnDispose();
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

            peers.ForEach(c => c.Dispose());

            XxUvInterop.xxuv_close_(ptr);
            this.Free(ref ptr);
            this.Free(ref addrPtr);
            this.Unhandle(ref handle);

            loop.listeners.SwapRemoveAt(index_at_container);
            loop = null;
            disposed = true;
        }
        // Call Dispose in the base class.
        //base.Dispose(disposing);
    }

    // Use C# destructor syntax for finalization code.
    ~XxUvTcpListener()
    {
        Dispose(false);
    }

    // The derived class does not have a Finalize method
    // or a Dispose method without parameters because it inherits
    // them from the base class.

    #endregion
}

public abstract class XxUvTcpBase
{
    /******************************************************************************/
    // 用户事件绑定
    public Action<byte[]> OnRecv;
    public Action<byte[], int, int> OnRecvPkg;
    public Action OnDispose;
    /******************************************************************************/

    public bool disposed;
    public XxUvLoop loop;
    public int index_at_container;

    public IntPtr ptr;
    public IntPtr handle;
    public IntPtr addrPtr;

    protected static XxUvInterop.uv_read_cb OnReadCB = OnReadCBImpl;
    static void OnReadCBImpl(IntPtr stream, IntPtr nread, IntPtr buf_t)
    {
        var peer = stream.To<XxUvTcpPeer>();
        var bufPtr = XxUvInterop.xxuv_get_buf(buf_t);
        int len = (int)nread;
        if (len > 0)
        {
            var buf = new byte[len];
            Marshal.Copy(bufPtr, buf, 0, len);
            peer.OnRecv(buf);
        }
        else if (len < 0)
        {
            peer.Dispose();
        }
        XxUvInterop.xxuv_free(bufPtr);
    }

    protected byte[] buf;
    protected int offset;
    public void OnRecvImpl(byte[] bytes)
    {
        if (buf == null)    // likely
        {
            buf = bytes;
        }
        else
        {
            var left = buf.Length - offset;
            var newBuf = new byte[left + bytes.Length];
            Buffer.BlockCopy(buf, offset, newBuf, 0, left);
            Buffer.BlockCopy(bytes, 0, newBuf, left, bytes.Length);
            buf = newBuf;
        }

        offset = 0;
        while (buf.Length - offset >= 2)            // 确保 2字节 包头长度
        {
            // 读出头
            var dataLen = buf[offset] + (buf[offset + 1] << 8);
            offset += 2;

            // todo: 特殊判断: 如果 dataLen 为 0 ?? 后续跟控制包? 当前直接认为是出错
            if (dataLen == 0) throw new InvalidCastException();

            // 确保数据长
            if (buf.Length - offset < dataLen)
            {
                offset -= 2;                        // 回滚偏移量, 停在 包头 位置
                break;
            }

            // 发起包回调
            if (OnRecvPkg != null) OnRecvPkg(buf, offset, dataLen);

            // 继续处理剩余数据
            offset += dataLen;
        }
    }

    public void Send(byte[] data, uint len = 0)
    {
        if (disposed) throw new ObjectDisposedException("XxUvTcpBase");
        if (data == null || data.Length == 0) throw new NullReferenceException();
        if (len > data.Length) throw new IndexOutOfRangeException();
        if (len == 0) len = (uint)data.Length;
        var h = GCHandle.Alloc(data, GCHandleType.Pinned);
        XxUvInterop.xxuv_write_(ptr, h.AddrOfPinnedObject(), len);
        h.Free();
    }
}

public class XxUvTcpPeer : XxUvTcpBase, IDisposable
{
    public XxUvTcpListener listener;
    public XxUvTcpPeer(XxUvLoop loop, XxUvTcpListener listener)
    {
        this.Handle(ref handle);
        this.loop = loop;
        this.listener = listener;
        this.OnRecv = this.OnRecvImpl;

        ptr = XxUvInterop.xxuv_alloc_uv_tcp_t(handle);
        if (ptr == IntPtr.Zero)
        {
            this.Unhandle(ref handle);
            throw new OutOfMemoryException();
        }

        int r = XxUvInterop.xxuv_tcp_init(loop.ptr, ptr);
        if (r != 0)
        {
            this.Free(ref ptr);
            this.Unhandle(ref handle);
            r.Throw();
        }

        r = XxUvInterop.xxuv_accept(listener.ptr, ptr);
        if (r != 0)
        {
            XxUvInterop.xxuv_close_(ptr);
            this.Free(ref ptr);
            this.Unhandle(ref handle);
            r.Throw();
        }

        r = XxUvInterop.xxuv_read_start_(ptr, OnReadCB);
        if (r != 0)
        {
            XxUvInterop.xxuv_close_(ptr);
            this.Free(ref ptr);
            this.Unhandle(ref handle);
            r.Throw();
        }

        addrPtr = XxUvInterop.xxuv_alloc_sockaddr_in(IntPtr.Zero);
        if (addrPtr == IntPtr.Zero)
        {
            XxUvInterop.xxuv_close_(ptr);
            this.Free(ref ptr);
            this.Unhandle(ref handle);
            throw new OutOfMemoryException();
        }

        index_at_container = listener.peers.dataLen;
        listener.peers.Add(this);
    }

    byte[] ipBuf = new byte[64];
    string ip_ = null;
    public string ip
    {
        get
        {
            if (disposed) throw new ObjectDisposedException("XxUvTcpPeer");
            if (ip_ != null) return ip_;
            int len = 0;
            var h = GCHandle.Alloc(ipBuf, GCHandleType.Pinned);
            try
            {
                XxUvInterop.xxuv_fill_client_ip(ptr, h.AddrOfPinnedObject(), ipBuf.Length, ref len).TryThrow();
                ip_ = Encoding.ASCII.GetString(ipBuf, 0, len);
                return ip_;
            }
            finally
            {
                h.Free();
            }
        }
    }

    #region Dispose

    //Implement IDisposable.
    public void Dispose()
    {
        if (OnDispose != null) OnDispose();
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

            XxUvInterop.xxuv_close_(ptr);
            this.Free(ref ptr);
            this.Free(ref addrPtr);
            this.Unhandle(ref handle);

            listener.peers.SwapRemoveAt(index_at_container);
            listener = null;
            loop = null;
            disposed = true;
        }
        // Call Dispose in the base class.
        //base.Dispose(disposing);
    }

    // Use C# destructor syntax for finalization code.
    ~XxUvTcpPeer()
    {
        Dispose(false);
    }

    // The derived class does not have a Finalize method
    // or a Dispose method without parameters because it inherits
    // them from the base class.

    #endregion
}

public class XxUvTcpClient : XxUvTcpBase, IDisposable
{
    /******************************************************************************/
    // 用户事件绑定
    public Action<int> OnConnect;
    /******************************************************************************/

    public XxUvTcpStates state;

    public XxUvTcpClient(XxUvLoop loop)
    {
        this.Handle(ref handle);
        this.loop = loop;
        this.OnRecv = this.OnRecvImpl;

        ptr = XxUvInterop.xxuv_alloc_uv_tcp_t(handle);
        if (ptr == IntPtr.Zero)
        {
            this.Unhandle(ref handle);
            throw new OutOfMemoryException();
        }

        addrPtr = XxUvInterop.xxuv_alloc_sockaddr_in(IntPtr.Zero);
        if (addrPtr == IntPtr.Zero)
        {
            this.Free(ref ptr);
            this.Unhandle(ref handle);
            throw new OutOfMemoryException();
        }

        index_at_container = loop.clients.dataLen;
        loop.clients.Add(this);
    }

    public void SetAddress(string ipv4, int port)
    {
        if (disposed) throw new ObjectDisposedException("XxUvTcpClient");
        XxUvInterop.xxuv_ip4_addr(ipv4, port, addrPtr).TryThrow();
    }

    static XxUvInterop.uv_connect_cb OnConnectCB = OnConnectCBImpl;
    static void OnConnectCBImpl(IntPtr req, int status)
    {
        var client = (XxUvTcpClient)((GCHandle)XxUvInterop.xxuv_get_ud_from_uv_connect_t(req)).Target;
        XxUvInterop.xxuv_free(req);
        if (status < 0)
        {
            client.Disconnect();
        }
        else
        {
            client.state = XxUvTcpStates.Connected;
            XxUvInterop.xxuv_read_start_(client.ptr, OnReadCB);
        }
        if (client.OnConnect != null) client.OnConnect(status);
    }

    public void Connect()
    {
        if (disposed) throw new ObjectDisposedException("XxUvTcpClient");
        if (state != XxUvTcpStates.Disconnected) throw new InvalidOperationException();

        XxUvInterop.xxuv_tcp_init(loop.ptr, ptr).TryThrow();

        int r = XxUvInterop.xxuv_tcp_connect_(ptr, addrPtr, OnConnectCB);
        if (r != 0)
        {
            XxUvInterop.xxuv_close_(ptr);
            r.Throw();
        }

        state = XxUvTcpStates.Connecting;
    }

    public void Disconnect()
    {
        if (disposed) throw new ObjectDisposedException("XxUvTcpClient");
        if (state == XxUvTcpStates.Disconnected) throw new InvalidOperationException();
        XxUvInterop.xxuv_close_(ptr);
        state = XxUvTcpStates.Disconnected;
    }

    #region Dispose

    //Implement IDisposable.
    public void Dispose()
    {
        if (OnDispose != null) OnDispose();
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

            XxUvInterop.xxuv_close_(ptr);
            this.Free(ref ptr);
            this.Free(ref addrPtr);
            this.Unhandle(ref handle);

            loop.clients.SwapRemoveAt(index_at_container);
            loop = null;
            disposed = true;
        }
        // Call Dispose in the base class.
        //base.Dispose(disposing);
    }

    // Use C# destructor syntax for finalization code.
    ~XxUvTcpClient()
    {
        Dispose(false);
    }

    // The derived class does not have a Finalize method
    // or a Dispose method without parameters because it inherits
    // them from the base class.

    #endregion
}

public class XxUvTimer : IDisposable
{
    /******************************************************************************/
    // 用户事件绑定
    public Action OnFire;
    public Action OnDispose;
    /******************************************************************************/

    public bool disposed;
    public XxUvLoop loop;
    public int index_at_container;

    public IntPtr ptr;
    public IntPtr handle;

    public XxUvTimer(XxUvLoop loop, ulong timeoutMS, ulong repeatIntervalMS)
    {
        this.Handle(ref handle);
        this.loop = loop;

        ptr = XxUvInterop.xxuv_alloc_uv_timer_t(handle);
        if (ptr == IntPtr.Zero)
        {
            this.Unhandle(ref handle);
            throw new OutOfMemoryException();
        }

        int r = XxUvInterop.xxuv_timer_init(loop.ptr, ptr);
        if (r != 0)
        {
            this.Free(ref ptr);
            this.Unhandle(ref handle);
            r.Throw();
        }

        r = XxUvInterop.xxuv_timer_start(ptr, OnTimerCB, timeoutMS, repeatIntervalMS);
        if (r != 0)
        {
            XxUvInterop.xxuv_close_(ptr);
            this.Free(ref ptr);
            this.Unhandle(ref handle);
            r.Throw();
        }

        index_at_container = loop.timers.dataLen;
        loop.timers.Add(this);
    }

    static XxUvInterop.uv_timer_cb OnTimerCB = OnTimerCBImpl;
    static void OnTimerCBImpl(IntPtr handle)
    {
        var timer = handle.To<XxUvTimer>();
        timer.OnFire();
    }

    public void SetRepeat(ulong repeatIntervalMS)
    {
        if (disposed) throw new ObjectDisposedException("XxUvTimer");
        XxUvInterop.xxuv_timer_set_repeat(ptr, repeatIntervalMS);
    }

    public void Again()
    {
        if (disposed) throw new ObjectDisposedException("XxUvTimer");
        XxUvInterop.xxuv_timer_again(ptr).TryThrow();
    }

    public void Stop()
    {
        if (disposed) throw new ObjectDisposedException("XxUvTimer");
        XxUvInterop.xxuv_timer_stop(ptr).TryThrow();
    }

    #region Dispose

    //Implement IDisposable.
    public void Dispose()
    {
        if (OnDispose != null) OnDispose();
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

            XxUvInterop.xxuv_close_(ptr);
            this.Free(ref ptr);
            this.Unhandle(ref handle);

            loop.timers.SwapRemoveAt(index_at_container);
            loop = null;
            disposed = true;
        }
        // Call Dispose in the base class.
        //base.Dispose(disposing);
    }

    // Use C# destructor syntax for finalization code.
    ~XxUvTimer()
    {
        Dispose(false);
    }

    // The derived class does not have a Finalize method
    // or a Dispose method without parameters because it inherits
    // them from the base class.

    #endregion
}

public class XxUvAsync : IDisposable
{
    /******************************************************************************/
    // 用户事件绑定
    public Action OnFire;
    public Action OnDispose;
    /******************************************************************************/

    public bool disposed;
    public XxUvLoop loop;
    public int index_at_container;
    public ConcurrentQueue<Action> actions = new ConcurrentQueue<Action>();

    public IntPtr ptr;
    public IntPtr handle;
    public XxUvAsync(XxUvLoop loop)
    {
        this.Handle(ref handle);
        this.loop = loop;
        this.OnFire = this.OnFireImpl;

        ptr = XxUvInterop.xxuv_alloc_uv_async_t(handle);
        if (ptr == IntPtr.Zero)
        {
            this.Unhandle(ref handle);
            throw new OutOfMemoryException();
        }

        int r = XxUvInterop.xxuv_async_init(loop.ptr, ptr, AsyncCB);
        if (r != 0)
        {
            this.Free(ref ptr);
            this.Unhandle(ref handle);
            r.Throw();
        }

        index_at_container = loop.asyncs.dataLen;
        loop.asyncs.Add(this);
    }

    static XxUvInterop.uv_async_cb AsyncCB = OnAsyncCBImpl;
    static void OnAsyncCBImpl(IntPtr handle)
    {
        var self = handle.To<XxUvAsync>();
        self.OnFire();
    }

    public void Fire(Action a)
    {
        if (disposed) throw new ObjectDisposedException("XxUvAsync");
        actions.Enqueue(a);
        XxUvInterop.xxuv_async_send(ptr).TryThrow();
    }

    public void OnFireImpl()
    {
        while (actions.TryDequeue(out var a)) a();
    }

    #region Dispose

    //Implement IDisposable.
    public void Dispose()
    {
        if (OnDispose != null) OnDispose();
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

            XxUvInterop.xxuv_close_(ptr);
            this.Free(ref ptr);
            this.Unhandle(ref handle);

            loop.asyncs.SwapRemoveAt(index_at_container);
            loop = null;
            disposed = true;
        }
        // Call Dispose in the base class.
        //base.Dispose(disposing);
    }

    // Use C# destructor syntax for finalization code.
    ~XxUvAsync()
    {
        Dispose(false);
    }

    // The derived class does not have a Finalize method
    // or a Dispose method without parameters because it inherits
    // them from the base class.

    #endregion
}
