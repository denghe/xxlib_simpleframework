using System;
using System.Runtime.InteropServices;
using System.Text;

// todo: 加上 disposed throw 检测
// todo: 构造函数中的失败回滚回收内存, 不可滥用 XxUvInterop.TryThrow
// todo: dispose 时的 ptr Zero 检测
// todo: ptr 状态跟踪( 记录是否需要 close + free 或只是 free )
// todo: XxUvTcpClient 的生命周期并不会跟随 ptr, 运行时 ptr 可以为空. 构造函数中不建. Connect 时检测并创建 tcp.

public enum XxUvRunMode
{
    Default = 0,
    Once,
    NoWait
}

public enum XxUvTcpStates
{
    Disconnected,
    Connecting,
    Connected,
    Disconnecting,
};

public static class XxUvInterop
{
#if !UNITY_EDITOR && UNITY_IPHONE
        const string DLL_NAME = "__Internal";
#else
    const string DLL_NAME = "xxuvlib";
#endif

    [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    public static extern IntPtr xxuv_alloc_uv_loop_t();

    [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    public static extern IntPtr xxuv_alloc_uv_tcp_t();

    [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    public static extern IntPtr xxuv_alloc_sockaddr_in();

    [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    public static extern void xxuv_free(IntPtr ptr);




    [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    public static extern string xxuv_strerror(int n);

    [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    public static extern string xxuv_err_name(int n);




    [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    public static extern void xxuv_set_data(IntPtr handle, IntPtr data);

    [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    public static extern IntPtr xxuv_get_data(IntPtr handle);




    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void uv_close_cb(IntPtr handle);

    [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    public static extern void xxuv_close(IntPtr handle, uv_close_cb close_cb);

    [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    public static extern void xxuv_close_(IntPtr handle);




    [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    public static extern int xxuv_loop_init(IntPtr loop);




    [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    public static extern int xxuv_run(IntPtr loop, XxUvRunMode mode);

    [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    public static extern int xxuv_loop_close(IntPtr loop);




    [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    public static extern int xxuv_ip4_addr(string ip, int port, IntPtr addr);




    [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    public static extern int xxuv_tcp_init(IntPtr loop, IntPtr tcp);

    //[DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    //public static extern int xxuv_tcp_bind(IntPtr tcp, IntPtr addr, uint flags);

    [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    public static extern int xxuv_tcp_bind_(IntPtr listener, IntPtr addr);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void uv_connection_cb(IntPtr listener, int status);

    [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    public static extern int xxuv_listen(IntPtr listener, int backlog, uv_connection_cb cb);

    [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    public static extern int xxuv_accept(IntPtr listener, IntPtr peer);

    [StructLayout(LayoutKind.Sequential)]
    public struct uv_buf_t
    {
        public IntPtr f0;
        public IntPtr f1;

        public uv_buf_t(IntPtr buf, int len)
        {
            if (Environment.OSVersion.Platform == PlatformID.Unix || Environment.OSVersion.Platform == PlatformID.MacOSX)
            {
                f0 = buf;
                f1 = (IntPtr)len;
            }
            else
            {
                f0 = (IntPtr)len;
                f1 = buf;
            }
        }

        // for uv_read_cb
        public byte[] ToBytes(int len)
        {
            IntPtr buf = (Environment.OSVersion.Platform == PlatformID.Unix
                || Environment.OSVersion.Platform == PlatformID.MacOSX) ? f0 : f1;
            var rtv = new byte[len];
            Marshal.Copy(buf, rtv, 0, len);
            return rtv;
        }

        public void Free()
        {
            if (Environment.OSVersion.Platform == PlatformID.Unix || Environment.OSVersion.Platform == PlatformID.MacOSX)
            {
                xxuv_free(f0);
            }
            else
            {
                xxuv_free(f1);
            }
        }
    }
    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void uv_read_cb(IntPtr stream, IntPtr nread, uv_buf_t buf);

    //public delegate void uv_alloc_cb (IntPtr handle, IntPtr suggested_size, IntPtr buf);
    //[DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    //public static extern int xxuv_read_start(IntPtr client, uv_alloc_cb alloc_cb, uv_read_cb read_cb);

    [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    public static extern int xxuv_read_start_(IntPtr stream, uv_read_cb read_cb);

    //public delegate void uv_write_cb(IntPtr req, int status);
    //[DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    //public static extern int xxuv_write(IntPtr req, IntPtr stream, uv_buf_t[] bufs, uint nbufs, uv_write_cb cb);

    [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    public static extern int xxuv_write_(IntPtr stream, IntPtr buf, uint len);

    [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    public static extern int xxuv_fill_client_ip(IntPtr stream, IntPtr buf, int buf_len, ref int data_len);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void uv_connect_cb(IntPtr req, int status);

    //[DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    //public static extern int xxuv_tcp_connect(IntPtr req, IntPtr stream, IntPtr addr, uv_connect_cb cb);
    [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    public static extern int xxuv_tcp_connect(IntPtr stream, IntPtr addr, uv_connect_cb cb);


    public static void TryThrow(int n)
    {
        if (n != 0) throw new Exception(XxUvInterop.xxuv_strerror(n));
    }
}

public class XxUvLoop : IDisposable
{
    public object userData;                             // 随便填
    public XxSimpleList<XxUvTcpListener> listeners;     // 需要用就 new
    public XxSimpleList<XxUvTcpClient> clients;         // 需要用就 new

    public IntPtr ptr;
    public XxUvLoop()
    {
        ptr = XxUvInterop.xxuv_alloc_uv_loop_t();
        if (ptr == IntPtr.Zero) throw new NullReferenceException();

        XxUvInterop.TryThrow(XxUvInterop.xxuv_loop_init(ptr));

        XxUvInterop.xxuv_set_data(ptr, (IntPtr)GCHandle.Alloc(this));
    }

    public void Run(XxUvRunMode mode = XxUvRunMode.Default)
    {
        XxUvInterop.TryThrow(XxUvInterop.xxuv_run(ptr, mode));
    }

    #region Dispose

    private bool disposed;

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

            var p = XxUvInterop.xxuv_get_data(ptr);
            if (p == IntPtr.Zero) throw new NullReferenceException();
            ((GCHandle)p).Free();

            int r = XxUvInterop.xxuv_loop_close(ptr);
            if (r != 0) throw new Exception(XxUvInterop.xxuv_strerror(r));

            XxUvInterop.xxuv_free(ptr);
            ptr = IntPtr.Zero;
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
    public object userData;                 // 随便填
    public Action<XxUvTcpPeer> OnAccept;
    public Action OnDispose;
    public XxSimpleList<XxUvTcpPeer> peers; // 需要用就 new
    public int index_at_container;          // 于容器中的 下标. 需要就填

    // 只读
    public XxUvLoop loop;

    public IntPtr ptr;
    public IntPtr addrPtr;
    public XxUvTcpListener(XxUvLoop loop)
    {
        this.loop = loop;

        ptr = XxUvInterop.xxuv_alloc_uv_tcp_t();
        if (ptr == IntPtr.Zero) throw new OutOfMemoryException();

        XxUvInterop.TryThrow(XxUvInterop.xxuv_tcp_init(loop.ptr, ptr));

        XxUvInterop.xxuv_set_data(ptr, (IntPtr)GCHandle.Alloc(this));

        addrPtr = XxUvInterop.xxuv_alloc_sockaddr_in();
        if (addrPtr == IntPtr.Zero) throw new OutOfMemoryException();

        OnAcceptCB = OnAcceptCBImpl;
    }

    public void Bind(string ipv4, int port)
    {
        XxUvInterop.TryThrow(XxUvInterop.xxuv_ip4_addr(ipv4, port, addrPtr));
        XxUvInterop.TryThrow(XxUvInterop.xxuv_tcp_bind_(ptr, addrPtr));
    }

    XxUvInterop.uv_connection_cb OnAcceptCB;
    void OnAcceptCBImpl(IntPtr stream, int status)
    {
        XxUvInterop.TryThrow(status);
        XxUvTcpPeer peer = null;
        try
        {
            peer = new XxUvTcpPeer(loop, this);
        }
        catch
        {
            return;
        }
        OnAccept(peer);
    }
    public void Listen(int backlog = 128)
    {
        XxUvInterop.TryThrow(XxUvInterop.xxuv_listen(ptr, backlog, OnAcceptCB));
    }

    #region Dispose

    private bool disposed;

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
            var p = XxUvInterop.xxuv_get_data(ptr);
            if (p == IntPtr.Zero) throw new NullReferenceException();
            ((GCHandle)p).Free();

            XxUvInterop.xxuv_close_(ptr);
            ptr = IntPtr.Zero;

            XxUvInterop.xxuv_free(addrPtr);
            addrPtr = IntPtr.Zero;

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

public class XxUvTcpPeer : IDisposable
{
    public object userData;                 // 随便填
    public Action<byte[]> OnRead;
    public Action OnDispose;
    public int index_at_container;          // 于容器中的 下标. 需要就填

    // 只读
    public XxUvLoop loop;
    public XxUvTcpListener listener;

    public IntPtr ptr;
    public IntPtr addrPtr;
    public XxUvTcpPeer(XxUvLoop loop, XxUvTcpListener listener)
    {
        this.loop = loop;
        this.listener = listener;

        ptr = XxUvInterop.xxuv_alloc_uv_tcp_t();
        if (ptr == IntPtr.Zero) throw new OutOfMemoryException();

        XxUvInterop.TryThrow(XxUvInterop.xxuv_tcp_init(loop.ptr, ptr));
        XxUvInterop.TryThrow(XxUvInterop.xxuv_accept(listener.ptr, ptr));
        XxUvInterop.TryThrow(XxUvInterop.xxuv_read_start_(ptr, OnReadCB));

        addrPtr = XxUvInterop.xxuv_alloc_sockaddr_in();
        if (addrPtr == IntPtr.Zero) throw new OutOfMemoryException();

        XxUvInterop.xxuv_set_data(ptr, (IntPtr)GCHandle.Alloc(this));
    }

    static XxUvInterop.uv_read_cb OnReadCB = OnReadCBImpl;
    static void OnReadCBImpl(IntPtr stream, IntPtr nread, XxUvInterop.uv_buf_t buf)
    {
        var peer = (XxUvTcpPeer)((GCHandle)XxUvInterop.xxuv_get_data(stream)).Target;
        int len = (int)nread;
        if (len > 0)
        {
            peer.OnRead(buf.ToBytes(len));
        }
        else if (len < 0)
        {
            peer.Dispose();
        }
        buf.Free();
    }

    public void Send(byte[] data, uint len = 0)
    {
        if (listener == null) throw new InvalidOperationException();
        if (data == null || data.Length == 0) throw new NullReferenceException();
        if (len > data.Length) throw new IndexOutOfRangeException();
        if (len == 0) len = (uint)data.Length;
        var h = GCHandle.Alloc(data, GCHandleType.Pinned);
        XxUvInterop.xxuv_write_(ptr, h.AddrOfPinnedObject(), len);
        h.Free();
    }

    byte[] ipBuf = new byte[64];
    string ip_ = null;
    public string ip
    {
        get
        {
            if (ip_ != null) return ip_;
            int len = 0;
            var h = GCHandle.Alloc(ipBuf, GCHandleType.Pinned);
            try
            {
                XxUvInterop.TryThrow(XxUvInterop.xxuv_fill_client_ip(ptr, h.AddrOfPinnedObject(), ipBuf.Length, ref len));
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

    private bool disposed;

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
            var p = XxUvInterop.xxuv_get_data(ptr);
            if (p == IntPtr.Zero) throw new NullReferenceException();
            ((GCHandle)p).Free();

            XxUvInterop.xxuv_close_(ptr);
            ptr = IntPtr.Zero;

            XxUvInterop.xxuv_free(addrPtr);
            addrPtr = IntPtr.Zero;

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

public class XxUvTcpClient : IDisposable
{
    public object userData;                 // 随便填
    public Action<byte[]> OnRead;
    public Action<int> OnConnect;
    public Action OnDispose;
    public int index_at_container;          // 于容器中的 下标. 需要就填

    // 只读
    public XxUvLoop loop;
    public XxUvTcpStates state;


    public IntPtr handle;
    void Handle()
    {
        if (handle != IntPtr.Zero) throw new InvalidOperationException();
        handle = (IntPtr)GCHandle.Alloc(this);
    }
    void Unhandle()
    {
        if (handle != IntPtr.Zero) return;
        ((GCHandle)handle).Free();
        handle = IntPtr.Zero;
    }


    public IntPtr addrPtr;
    void CreateAddrPtr()
    {
        if (addrPtr != IntPtr.Zero) throw new InvalidOperationException();
        addrPtr = XxUvInterop.xxuv_alloc_sockaddr_in();
        if (addrPtr == IntPtr.Zero) throw new OutOfMemoryException();
    }
    void ReleaseAddrPtr()
    {
        if (addrPtr != IntPtr.Zero)
        {
            XxUvInterop.xxuv_free(addrPtr);
            addrPtr = IntPtr.Zero;
        }
    }


    public IntPtr ptr;
    void CreatePtr()
    {
        if (ptr != IntPtr.Zero) throw new InvalidOperationException();

        ptr = XxUvInterop.xxuv_alloc_uv_tcp_t();
        if (ptr == IntPtr.Zero) throw new OutOfMemoryException();

        int r = XxUvInterop.xxuv_tcp_init(loop.ptr, ptr);
        if (r != 0)
        {
            XxUvInterop.xxuv_free(ptr);
            ptr = IntPtr.Zero;
            throw new Exception(XxUvInterop.xxuv_strerror(r));
        }

        XxUvInterop.xxuv_set_data(ptr, handle);
    }
    void ReleasePtr()
    {
        if (ptr != IntPtr.Zero)
        {
            XxUvInterop.xxuv_close_(ptr);
            ptr = IntPtr.Zero;
        }
    }


    public XxUvTcpClient(XxUvLoop loop)
    {
        CreateAddrPtr();
        this.loop = loop;
        OnConnectCB = OnConnectCBImpl;
        Handle();
    }

    public void SetAddress(string ipv4, int port)
    {
        XxUvInterop.TryThrow(XxUvInterop.xxuv_ip4_addr(ipv4, port, addrPtr));
    }

    XxUvInterop.uv_connect_cb OnConnectCB;
    public void OnConnectCBImpl(IntPtr req, int status)
    {
        XxUvInterop.xxuv_free(req);
        if (status < 0)
        {
            state = XxUvTcpStates.Disconnected;
            ReleasePtr();
        }
        else
        {
            state = XxUvTcpStates.Connected;
            XxUvInterop.xxuv_read_start_(ptr, OnReadCB);
        }
        if (OnConnect != null) OnConnect(status);
    }

    public void Connect()
    {
        if (state != XxUvTcpStates.Disconnected) throw new InvalidOperationException();

        CreatePtr();

        var r = XxUvInterop.xxuv_tcp_connect(ptr, addrPtr, OnConnectCB);
        if (r != 0)
        {
            ReleasePtr();
            throw new Exception(XxUvInterop.xxuv_strerror(r));
        }
        state = XxUvTcpStates.Connecting;
    }

    static XxUvInterop.uv_read_cb OnReadCB = OnReadCBImpl;
    static void OnReadCBImpl(IntPtr stream, IntPtr nread, XxUvInterop.uv_buf_t buf)
    {
        var client = (XxUvTcpClient)((GCHandle)XxUvInterop.xxuv_get_data(stream)).Target;
        int len = (int)nread;
        if (len > 0)
        {
            client.OnRead(buf.ToBytes(len));
        }
        else if (len < 0)
        {
            client.Dispose();
        }
        buf.Free();
    }

    public void Send(byte[] data, uint len = 0)
    {
        if (data == null || data.Length == 0) throw new NullReferenceException();
        if (len > data.Length) throw new IndexOutOfRangeException();
        if (len == 0) len = (uint)data.Length;
        var h = GCHandle.Alloc(data, GCHandleType.Pinned);
        XxUvInterop.xxuv_write_(ptr, h.AddrOfPinnedObject(), len);
        h.Free();
    }

    public void Disconnect()
    {
        if (state == XxUvTcpStates.Disconnected) throw new InvalidOperationException();
        ReleasePtr();
        state = XxUvTcpStates.Disconnected;
    }

    #region Dispose

    private bool disposed;

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
            Unhandle();
            if (state != XxUvTcpStates.Disconnected) Disconnect();
            ReleaseAddrPtr();
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
