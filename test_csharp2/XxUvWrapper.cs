using System;
using System.Collections.Generic;
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
    public static extern int xxuv_is_unix();

    [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    public static extern IntPtr xxuv_alloc_uv_loop_t();

    [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    public static extern IntPtr xxuv_alloc_uv_tcp_t();

    [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    public static extern IntPtr xxuv_alloc_sockaddr_in();

    [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    public static extern void xxuv_free(IntPtr p);



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
    public static extern int xxuv_tcp_bind_(IntPtr tcp, IntPtr addr);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void uv_connection_cb(IntPtr server, int status);

    [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    public static extern int xxuv_listen(IntPtr tcp, int backlog, uv_connection_cb cb);

    [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    public static extern int xxuv_accept(IntPtr server, IntPtr client);

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
        public byte[] ToBytes()
        {
            IntPtr buf;
            int len;
            if (Environment.OSVersion.Platform == PlatformID.Unix || Environment.OSVersion.Platform == PlatformID.MacOSX)
            {
                buf = f0;
                len = (int)f1;
            }
            else
            {
                len = (int)f0;
                buf = f1;
            }
            if (len == 0) return null;
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
    public static extern int xxuv_read_start_(IntPtr client, uv_read_cb read_cb);

    //public delegate void uv_write_cb(IntPtr req, int status);
    //[DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    //public static extern int xxuv_write(IntPtr req, IntPtr stream, uv_buf_t[] bufs, uint nbufs, uv_write_cb cb);

    [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    public static extern int xxuv_write_(IntPtr stream, IntPtr buf, uint len);
}

/*
 
    // object to IntPtr (before calling WinApi):
List<string> list1 = new List<string>();
GCHandle handle1 = GCHandle.Alloc(list1);
IntPtr parameter = (IntPtr) handle1;
// call WinAPi and pass the parameter here
// then free the handle when not needed:
handle1.Free();

// back to object (in callback function):
GCHandle handle2 = (GCHandle) parameter;
List<string> list2 = (handle2.Target as List<string>);
list2.Add("hello world");

     */

public class XxUvLoop : IDisposable
{
    public IntPtr loop;

    public XxUvLoop()
    {
        loop = XxUvInterop.xxuv_alloc_uv_loop_t();
        if (loop == IntPtr.Zero)
        {
            throw new Exception("xxuv_alloc_uv_loop_t failed.");
        }

        int r = XxUvInterop.xxuv_loop_init(loop);
        if (r != 0)
        {
            throw new Exception(XxUvInterop.xxuv_strerror(r));
        }

        var h = GCHandle.Alloc(this);
        XxUvInterop.xxuv_set_data(loop, (IntPtr)h);
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

            XxUvInterop.xxuv_loop_close(loop);
            var p = XxUvInterop.xxuv_get_data(loop);
            ((GCHandle)p).Free();
            XxUvInterop.xxuv_free(loop);
            loop = IntPtr.Zero;
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
