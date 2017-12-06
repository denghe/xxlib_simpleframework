using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

public enum XxUvRunMode
{
    Default = 0,
    Once,
    NoWait
}

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



    public static void TryThrow(int n)
    {
        if (n != 0) throw new Exception(XxUvInterop.xxuv_strerror(n));
    }
}


// todo: 加上 disposed throw 检测

public class XxUvLoop : IDisposable
{
    public object userData;                     // 随便填
    public XxSimpleList<XxUvTcp> listeners;     // 需要用就 new
    public XxSimpleList<XxUvTcp> clients;       // 需要用就 new

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


public class XxUvTcp : IDisposable
{
    public object userData;                 // 随便填
    public Action<byte[]> OnRead;
    public Action<XxUvTcp> OnConnect;
    // todo: OnDisconnect
    public Action OnDispose;
    public XxSimpleList<XxUvTcp> peers;     // 需要用就 new
    public int index_at_container;          // 于容器中的 下标. 需要就填


    // 内部变量
    public XxUvLoop loop;
    public XxUvTcp listener;                // 空: client, 非空: peer


    public IntPtr ptr;
    public IntPtr addrPtr;
    public XxUvTcp(XxUvLoop loop)
    {
        this.loop = loop;

        ptr = XxUvInterop.xxuv_alloc_uv_tcp_t();
        if (ptr == IntPtr.Zero) throw new OverflowException();

        XxUvInterop.TryThrow(XxUvInterop.xxuv_tcp_init(loop.ptr, ptr));

        XxUvInterop.xxuv_set_data(ptr, (IntPtr)GCHandle.Alloc(this));

        addrPtr = XxUvInterop.xxuv_alloc_sockaddr_in();
        if (addrPtr == IntPtr.Zero) throw new OverflowException();
    }



    // todo: SetAddress, Connect, Disconnect



    public void Bind(string ipv4, int port)
    {
        XxUvInterop.TryThrow(XxUvInterop.xxuv_ip4_addr(ipv4, port, addrPtr));
        XxUvInterop.TryThrow(XxUvInterop.xxuv_tcp_bind_(ptr, addrPtr));
    }



    static XxUvInterop.uv_connection_cb OnConnectCB = OnConnectCBImpl;
    static void OnConnectCBImpl(IntPtr stream, int status)
    {
        XxUvInterop.TryThrow(status);
        var listener = (XxUvTcp)((GCHandle)XxUvInterop.xxuv_get_data(stream)).Target;
        var peer = listener.Accept();
        if (peer != null)
        {
            listener.OnConnect(peer);
        }
    }
    public void Listen(int backlog = 128)
    {
        XxUvInterop.TryThrow(XxUvInterop.xxuv_listen(ptr, backlog, OnConnectCB));
    }



    static XxUvInterop.uv_read_cb OnReadCB = OnReadCBImpl;
    static void OnReadCBImpl(IntPtr stream, IntPtr nread, XxUvInterop.uv_buf_t buf)
    {
        var peer = (XxUvTcp)((GCHandle)XxUvInterop.xxuv_get_data(stream)).Target;
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
    public XxUvTcp Accept()
    {
        var peer = new XxUvTcp(loop);
        if (XxUvInterop.xxuv_accept(ptr, peer.ptr) == 0)
        {
            XxUvInterop.TryThrow(XxUvInterop.xxuv_read_start_(peer.ptr, OnReadCB));
            peer.listener = this;
            return peer;
        }
        else
        {
            peer.Dispose();
            return null;
        }
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
    ~XxUvTcp()
    {
        Dispose(false);
    }

    // The derived class does not have a Finalize method
    // or a Dispose method without parameters because it inherits
    // them from the base class.

    #endregion
}


public class XxSimpleList<T>
{
    public T[] buf;
    public int bufLen { get { return buf.Length; } }
    public int dataLen;

    private const int defaultCapacity = 4;
    static readonly T[] emptyBuf = new T[0];

    public XxSimpleList(int capacity = 0)
    {
        buf = emptyBuf;
        Reserve(capacity);
    }

    public T this[int index]
    {
        get { return buf[index]; }
        set { buf[index] = value; }
    }

    public void Add(T item)
    {
        if (dataLen == buf.Length) Reserve(dataLen + 1);
        buf[dataLen++] = item;
    }

    public void Reserve(int min)
    {
        if (buf.Length < min)
        {
            int newCapacity = buf.Length == 0 ? defaultCapacity : buf.Length * 2;
            if (newCapacity < min) newCapacity = min;
            if (newCapacity < buf.Length) return;
            T[] newItems = new T[newCapacity];
            if (dataLen > 0)
            {
                Array.Copy(buf, 0, newItems, 0, dataLen);
            }
            buf = newItems;
        }
    }

    public void SwapRemoveAt(int index)
    {
        if (index + 1 < dataLen)
        {
            buf[index] = buf[dataLen - 1];
        }
        dataLen--;
        buf[dataLen] = default(T);
    }

    public void ForEach(Action<T> action)
    {
        for (int i = dataLen - 1; i >= 0; --i)
        {
            action(buf[i]);
        }
    }
}
