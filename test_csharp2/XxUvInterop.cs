using System;
using System.Runtime.InteropServices;

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
    public static extern IntPtr xxuv_alloc_uv_timer_t(IntPtr ud);

    [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    public static extern IntPtr xxuv_alloc_uv_async_t(IntPtr ud);



    [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    public static extern void xxuv_free(IntPtr p);

    [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    public static extern IntPtr xxuv_get_ud(IntPtr p);

    [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    public static extern IntPtr xxuv_get_ud_from_uv_connect_t(IntPtr req);

    [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    public static extern IntPtr xxuv_get_buf(IntPtr buf_t);



    [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    public static extern IntPtr xxuv_strerror(int n);


    [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    public static extern IntPtr xxuv_err_name(int n);



    //[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    //public delegate void uv_close_cb(IntPtr handle);

    //[DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    //public static extern void xxuv_close(IntPtr handle, uv_close_cb close_cb);

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

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void uv_read_cb(IntPtr stream, IntPtr nread, IntPtr buf_t);

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
    public static extern int xxuv_tcp_connect_(IntPtr stream, IntPtr addr, uv_connect_cb cb);





    [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    public static extern int xxuv_timer_init(IntPtr loop, IntPtr timer_req);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void uv_timer_cb(IntPtr req);

    [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    public static extern int xxuv_timer_start(IntPtr timer_req, uv_timer_cb cb, ulong timeoutMS, ulong repeatIntervalMS);

    [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    public static extern void xxuv_timer_set_repeat(IntPtr timer_req, ulong repeatIntervalMS);

    [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    public static extern int xxuv_timer_again(IntPtr timer_req);

    [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    public static extern int xxuv_timer_stop(IntPtr timer_req);





    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void uv_async_cb(IntPtr req);

    [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    public static extern int xxuv_async_init(IntPtr loop, IntPtr async_req, uv_async_cb cb);

    [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    public static extern int xxuv_async_send(IntPtr async_req);






    public static void Throw(this int n)
    {
        var p = XxUvInterop.xxuv_strerror(n);
        var s = Marshal.PtrToStringAnsi(p);
        throw new Exception("uv exception: " + s);
    }

    public static void TryThrow(this int n)
    {
        if (n != 0) Throw(n);
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
