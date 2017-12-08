using System;
using System.Runtime.InteropServices;
using System.Text;

// todo: 加上 disposed throw 检测

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
    public static extern IntPtr xxuv_alloc_uv_loop_t(IntPtr ud);

    [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    public static extern IntPtr xxuv_alloc_uv_tcp_t(IntPtr ud);

    [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    public static extern IntPtr xxuv_alloc_sockaddr_in(IntPtr ud);



    [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    public static extern void xxuv_free(IntPtr p);

    [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    public static extern IntPtr xxuv_get_ud(IntPtr p);

    [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    public static extern IntPtr xxuv_get_ud_from_uv_connect_t(IntPtr req);



    [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    public static extern string xxuv_strerror(int n);


    [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    public static extern string xxuv_err_name(int n);



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


    public static void TryThrow(this int n)
    {
        if (n != 0) throw new Exception(XxUvInterop.xxuv_strerror(n));
    }

    public static void Throw(this int n)
    {
        throw new Exception(XxUvInterop.xxuv_strerror(n));
    }

    public static T To<T>(this IntPtr p)
    {
        return (T)((GCHandle)xxuv_get_ud(p)).Target;
    }

    public static void Handle(this IDisposable self, ref IntPtr handle)
    {
        handle = (IntPtr)GCHandle.Alloc(self);
    }

    // self 用不到. 只是为便于写扩展, 说明语义
    public static void Unhandle(this IDisposable self, ref IntPtr handle)
    {
        if (handle != IntPtr.Zero)
        {
            ((GCHandle)handle).Free();
            handle = IntPtr.Zero;
        }
    }

    // self 用不到. 只是为便于写扩展, 说明语义
    public static void Free(this IDisposable self, ref IntPtr ptr)
    {
        if (ptr != IntPtr.Zero)
        {
            xxuv_free(ptr);
            ptr = IntPtr.Zero;
        }
    }
}

public class XxUvLoop : IDisposable
{
    public XxSimpleList<XxUvTcpListener> listeners;     // 需要用就 new
    public XxSimpleList<XxUvTcpClient> clients;         // 需要用就 new

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
    public Action<XxUvTcpPeer> OnAccept;
    public Action OnDispose;
    public XxSimpleList<XxUvTcpPeer> peers; // 需要用就 new
    public int index_at_container;          // 于容器中的 下标. 需要就填

    // 只读
    public XxUvLoop loop;

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
            throw new Exception(XxUvInterop.xxuv_strerror(r));
        }

        addrPtr = XxUvInterop.xxuv_alloc_sockaddr_in(IntPtr.Zero);
        if (addrPtr == IntPtr.Zero)
        {
            XxUvInterop.xxuv_close_(ptr);
            this.Free(ref ptr);
            this.Unhandle(ref handle);
            throw new OutOfMemoryException();
        }
    }


    static XxUvInterop.uv_connection_cb OnAcceptCB = OnAcceptCBImpl;
    static void OnAcceptCBImpl(IntPtr stream, int status)
    {
        status.TryThrow();
        var listener = stream.To<XxUvTcpListener>();
        XxUvTcpPeer peer = null;
        try
        {
            peer = new XxUvTcpPeer(listener.loop, listener);
        }
        catch
        {
            return;
        }
        listener.OnAccept(peer);
    }
    public void Listen(int backlog = 128)
    {
        XxUvInterop.TryThrow(XxUvInterop.xxuv_listen(ptr, backlog, OnAcceptCB));
    }


    public void Bind(string ipv4, int port)
    {
        XxUvInterop.TryThrow(XxUvInterop.xxuv_ip4_addr(ipv4, port, addrPtr));
        XxUvInterop.TryThrow(XxUvInterop.xxuv_tcp_bind_(ptr, addrPtr));
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

            XxUvInterop.xxuv_close_(ptr);
            this.Free(ref ptr);
            this.Free(ref addrPtr);
            this.Unhandle(ref handle);
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
    public IntPtr handle;
    public IntPtr addrPtr;
    public XxUvTcpPeer(XxUvLoop loop, XxUvTcpListener listener)
    {
        this.Handle(ref handle);
        this.loop = loop;
        this.listener = listener;

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
    }

    static XxUvInterop.uv_read_cb OnReadCB = OnReadCBImpl;
    static void OnReadCBImpl(IntPtr stream, IntPtr nread, XxUvInterop.uv_buf_t buf)
    {
        var peer = stream.To<XxUvTcpPeer>();
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

            XxUvInterop.xxuv_close_(ptr);
            this.Free(ref ptr);
            this.Free(ref addrPtr);
            this.Unhandle(ref handle);

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

    public IntPtr ptr;
    public IntPtr handle;
    public IntPtr addrPtr;
    public XxUvTcpClient(XxUvLoop loop)
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
    }

    public void SetAddress(string ipv4, int port)
    {
        XxUvInterop.TryThrow(XxUvInterop.xxuv_ip4_addr(ipv4, port, addrPtr));
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
        if (state != XxUvTcpStates.Disconnected) throw new InvalidOperationException();
        if (ptr != IntPtr.Zero) throw new InvalidOperationException();

        ptr = XxUvInterop.xxuv_alloc_uv_tcp_t(handle);
        if (ptr == IntPtr.Zero)
        {
            throw new OutOfMemoryException();
        }

        int r = XxUvInterop.xxuv_tcp_init(loop.ptr, ptr);
        if (r != 0)
        {
            this.Free(ref ptr);
            r.Throw();
        }

        r = XxUvInterop.xxuv_tcp_connect(ptr, addrPtr, OnConnectCB);
        if (r != 0)
        {
            XxUvInterop.xxuv_close_(ptr);
            this.Free(ref ptr);
            r.Throw();
        }

        state = XxUvTcpStates.Connecting;
    }

    static XxUvInterop.uv_read_cb OnReadCB = OnReadCBImpl;
    static void OnReadCBImpl(IntPtr stream, IntPtr nread, XxUvInterop.uv_buf_t buf)
    {
        var client = stream.To<XxUvTcpClient>();
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
        XxUvInterop.xxuv_close_(ptr);
        this.Free(ref ptr);
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

            if (state != XxUvTcpStates.Disconnected) Disconnect();
            this.Free(ref addrPtr);
            this.Unhandle(ref handle);
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
