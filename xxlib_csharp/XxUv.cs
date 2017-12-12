using System;
using System.Collections.Concurrent;
using System.Runtime.InteropServices;
using System.Text;

// todo: 有子容器的 Dispose 时清子
// todo: c api 过来的 cb 可能要加 try

namespace xx
{
    public class UvLoop : IDisposable
    {
        public bool disposed;
        public List<UvTcpListener> listeners = new List<UvTcpListener>();
        public List<UvTcpClient> clients = new List<UvTcpClient>();
        public List<UvTimer> timers = new List<UvTimer>();
        public List<UvAsync> asyncs = new List<UvAsync>();

        public IntPtr ptr;
        public IntPtr handle;

        public UvLoop()
        {
            this.Handle(ref handle);
            ptr = UvInterop.xxuv_alloc_uv_loop_t(handle);
            if (ptr == IntPtr.Zero)
            {
                this.Unhandle(ref handle);
                throw new NullReferenceException();
            }

            int r = UvInterop.xxuv_loop_init(ptr);
            if (r != 0)
            {
                this.Free(ref ptr);
                this.Unhandle(ref handle);
                r.Throw();
            }
        }

        public void Run(UvRunMode mode = UvRunMode.Default)
        {
            if (disposed) throw new ObjectDisposedException("XxUvLoop");
            UvInterop.xxuv_run(ptr, mode).TryThrow();
        }

        #region Dispose

        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }

        protected virtual void Dispose(bool disposing)
        {
            if (!disposed)
            {
                // if (disposing) // Free other state (managed objects).
                asyncs.ForEachReverse(o => o.Dispose());
                timers.ForEachReverse(o => o.Dispose());
                clients.ForEachReverse(o => o.Dispose());
                listeners.ForEachReverse(o => o.Dispose());

                UvInterop.xxuv_loop_close(ptr).TryThrow();
                this.Free(ref ptr);
                this.Unhandle(ref handle);
                disposed = true;
            }
            //base.Dispose(disposing);
        }

        ~UvLoop()
        {
            Dispose(false);
        }

        #endregion
    }

    public class UvTcpListener : IDisposable
    {
        /******************************************************************************/
        // 用户事件绑定区
        public Func<UvTcpPeer> OnCreatePeer;
        public Action<UvTcpPeer> OnAccept;
        public Action OnDispose;
        /******************************************************************************/

        public bool disposed;
        public UvLoop loop;
        public List<UvTcpPeer> peers = new List<UvTcpPeer>();
        public int index_at_container;

        public IntPtr ptr;
        public IntPtr handle;
        public IntPtr addrPtr;

        public UvTcpListener(UvLoop loop)
        {
            this.Handle(ref handle);
            this.loop = loop;

            ptr = UvInterop.xxuv_alloc_uv_tcp_t(handle);
            if (ptr == IntPtr.Zero)
            {
                this.Unhandle(ref handle);
                throw new OutOfMemoryException();
            }

            int r = UvInterop.xxuv_tcp_init(loop.ptr, ptr);
            if (r != 0)
            {
                this.Free(ref ptr);
                this.Unhandle(ref handle);
                r.Throw();
            }

            addrPtr = UvInterop.xxuv_alloc_sockaddr_in(IntPtr.Zero);
            if (addrPtr == IntPtr.Zero)
            {
                UvInterop.xxuv_close_(ptr);
                this.Free(ref ptr);
                this.Unhandle(ref handle);
                throw new OutOfMemoryException();
            }

            index_at_container = loop.listeners.dataLen;
            loop.listeners.Add(this);
        }


        static UvInterop.uv_connection_cb OnAcceptCB = OnAcceptCBImpl;
        static void OnAcceptCBImpl(IntPtr stream, int status)
        {
            status.TryThrow();
            var listener = stream.To<UvTcpListener>();
            UvTcpPeer peer = null;
            try
            {
                if (listener.OnCreatePeer != null) peer = listener.OnCreatePeer();
                else peer = new UvTcpPeer(listener.loop, listener);
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
            UvInterop.xxuv_listen(ptr, backlog, OnAcceptCB).TryThrow();
        }


        public void Bind(string ipv4, int port)
        {
            if (disposed) throw new ObjectDisposedException("XxUvTcpListener");
            UvInterop.xxuv_ip4_addr(ipv4, port, addrPtr).TryThrow();
            UvInterop.xxuv_tcp_bind_(ptr, addrPtr).TryThrow();
        }

        #region Dispose

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
                // if (disposing) // Free other state (managed objects).

                peers.ForEachReverse(o => o.Dispose());

                UvInterop.xxuv_close_(ptr);
                this.Free(ref ptr);
                this.Free(ref addrPtr);
                this.Unhandle(ref handle);

                loop.listeners.SwapRemoveAt(index_at_container);
                loop = null;
                disposed = true;
            }
            //base.Dispose(disposing);
        }

        ~UvTcpListener()
        {
            Dispose(false);
        }

        #endregion
    }

    public abstract class UvTcpBase
    {
        /******************************************************************************/
        // 用户事件绑定
        public Action<byte[]> OnRecv;
        public Action<byte[], int, int> OnRecvPkg;
        public Action OnDispose;
        /******************************************************************************/

        public bool disposed;
        public UvLoop loop;
        public int index_at_container;

        public IntPtr ptr;
        public IntPtr handle;
        public IntPtr addrPtr;

        protected static UvInterop.uv_read_cb OnReadCB = OnReadCBImpl;
        static void OnReadCBImpl(IntPtr stream, IntPtr nread, IntPtr buf_t)
        {
            var peer = stream.To<UvTcpPeer>();
            var bufPtr = UvInterop.xxuv_get_buf(buf_t);
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
            UvInterop.xxuv_free(bufPtr);
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
            UvInterop.xxuv_write_(ptr, h.AddrOfPinnedObject(), len);
            h.Free();
        }
    }

    public class UvTcpPeer : UvTcpBase, IDisposable
    {
        public UvTcpListener listener;
        public UvTcpPeer(UvLoop loop, UvTcpListener listener)
        {
            this.Handle(ref handle);
            this.loop = loop;
            this.listener = listener;
            this.OnRecv = this.OnRecvImpl;

            ptr = UvInterop.xxuv_alloc_uv_tcp_t(handle);
            if (ptr == IntPtr.Zero)
            {
                this.Unhandle(ref handle);
                throw new OutOfMemoryException();
            }

            int r = UvInterop.xxuv_tcp_init(loop.ptr, ptr);
            if (r != 0)
            {
                this.Free(ref ptr);
                this.Unhandle(ref handle);
                r.Throw();
            }

            r = UvInterop.xxuv_accept(listener.ptr, ptr);
            if (r != 0)
            {
                UvInterop.xxuv_close_(ptr);
                this.Free(ref ptr);
                this.Unhandle(ref handle);
                r.Throw();
            }

            r = UvInterop.xxuv_read_start_(ptr, OnReadCB);
            if (r != 0)
            {
                UvInterop.xxuv_close_(ptr);
                this.Free(ref ptr);
                this.Unhandle(ref handle);
                r.Throw();
            }

            addrPtr = UvInterop.xxuv_alloc_sockaddr_in(IntPtr.Zero);
            if (addrPtr == IntPtr.Zero)
            {
                UvInterop.xxuv_close_(ptr);
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
                    UvInterop.xxuv_fill_client_ip(ptr, h.AddrOfPinnedObject(), ipBuf.Length, ref len).TryThrow();
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

                UvInterop.xxuv_close_(ptr);
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
        ~UvTcpPeer()
        {
            Dispose(false);
        }

        // The derived class does not have a Finalize method
        // or a Dispose method without parameters because it inherits
        // them from the base class.

        #endregion
    }

    public class UvTcpClient : UvTcpBase, IDisposable
    {
        /******************************************************************************/
        // 用户事件绑定
        public Action<int> OnConnect;
        /******************************************************************************/

        public UvTcpStates state;

        public UvTcpClient(UvLoop loop)
        {
            this.Handle(ref handle);
            this.loop = loop;
            this.OnRecv = this.OnRecvImpl;

            ptr = UvInterop.xxuv_alloc_uv_tcp_t(handle);
            if (ptr == IntPtr.Zero)
            {
                this.Unhandle(ref handle);
                throw new OutOfMemoryException();
            }

            addrPtr = UvInterop.xxuv_alloc_sockaddr_in(IntPtr.Zero);
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
            UvInterop.xxuv_ip4_addr(ipv4, port, addrPtr).TryThrow();
        }

        static UvInterop.uv_connect_cb OnConnectCB = OnConnectCBImpl;
        static void OnConnectCBImpl(IntPtr req, int status)
        {
            var client = (UvTcpClient)((GCHandle)UvInterop.xxuv_get_ud_from_uv_connect_t(req)).Target;
            UvInterop.xxuv_free(req);
            if (status < 0)
            {
                client.Disconnect();
            }
            else
            {
                client.state = UvTcpStates.Connected;
                UvInterop.xxuv_read_start_(client.ptr, OnReadCB);
            }
            if (client.OnConnect != null) client.OnConnect(status);
        }

        public void Connect()
        {
            if (disposed) throw new ObjectDisposedException("XxUvTcpClient");
            if (state != UvTcpStates.Disconnected) throw new InvalidOperationException();

            UvInterop.xxuv_tcp_init(loop.ptr, ptr).TryThrow();

            int r = UvInterop.xxuv_tcp_connect_(ptr, addrPtr, OnConnectCB);
            if (r != 0)
            {
                UvInterop.xxuv_close_(ptr);
                r.Throw();
            }

            state = UvTcpStates.Connecting;
        }

        public void Disconnect()
        {
            if (disposed) throw new ObjectDisposedException("XxUvTcpClient");
            if (state == UvTcpStates.Disconnected) throw new InvalidOperationException();
            UvInterop.xxuv_close_(ptr);
            state = UvTcpStates.Disconnected;
        }

        #region Dispose

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
                // if (disposing) // Free other state (managed objects).

                UvInterop.xxuv_close_(ptr);
                this.Free(ref ptr);
                this.Free(ref addrPtr);
                this.Unhandle(ref handle);

                loop.clients.SwapRemoveAt(index_at_container);
                loop = null;
                disposed = true;
            }
            //base.Dispose(disposing);
        }

        ~UvTcpClient()
        {
            Dispose(false);
        }

        #endregion
    }

    public class UvTimer : IDisposable
    {
        /******************************************************************************/
        // 用户事件绑定
        public Action OnFire;
        public Action OnDispose;
        /******************************************************************************/

        public bool disposed;
        public UvLoop loop;
        public int index_at_container;

        public IntPtr ptr;
        public IntPtr handle;

        public UvTimer(UvLoop loop, ulong timeoutMS, ulong repeatIntervalMS)
        {
            this.Handle(ref handle);
            this.loop = loop;

            ptr = UvInterop.xxuv_alloc_uv_timer_t(handle);
            if (ptr == IntPtr.Zero)
            {
                this.Unhandle(ref handle);
                throw new OutOfMemoryException();
            }

            int r = UvInterop.xxuv_timer_init(loop.ptr, ptr);
            if (r != 0)
            {
                this.Free(ref ptr);
                this.Unhandle(ref handle);
                r.Throw();
            }

            r = UvInterop.xxuv_timer_start(ptr, OnTimerCB, timeoutMS, repeatIntervalMS);
            if (r != 0)
            {
                UvInterop.xxuv_close_(ptr);
                this.Free(ref ptr);
                this.Unhandle(ref handle);
                r.Throw();
            }

            index_at_container = loop.timers.dataLen;
            loop.timers.Add(this);
        }

        static UvInterop.uv_timer_cb OnTimerCB = OnTimerCBImpl;
        static void OnTimerCBImpl(IntPtr handle)
        {
            var timer = handle.To<UvTimer>();
            timer.OnFire();
        }

        public void SetRepeat(ulong repeatIntervalMS)
        {
            if (disposed) throw new ObjectDisposedException("XxUvTimer");
            UvInterop.xxuv_timer_set_repeat(ptr, repeatIntervalMS);
        }

        public void Again()
        {
            if (disposed) throw new ObjectDisposedException("XxUvTimer");
            UvInterop.xxuv_timer_again(ptr).TryThrow();
        }

        public void Stop()
        {
            if (disposed) throw new ObjectDisposedException("XxUvTimer");
            UvInterop.xxuv_timer_stop(ptr).TryThrow();
        }

        #region Dispose

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
                // if (disposing) // Free other state (managed objects).

                UvInterop.xxuv_close_(ptr);
                this.Free(ref ptr);
                this.Unhandle(ref handle);

                loop.timers.SwapRemoveAt(index_at_container);
                loop = null;
                disposed = true;
            }
            //base.Dispose(disposing);
        }

        ~UvTimer()
        {
            Dispose(false);
        }

        #endregion
    }

    public class UvAsync : IDisposable
    {
        /******************************************************************************/
        // 用户事件绑定
        public Action OnFire;
        public Action OnDispose;
        /******************************************************************************/

        public bool disposed;
        public UvLoop loop;
        public int index_at_container;
        public ConcurrentQueue<Action> actions = new ConcurrentQueue<Action>();

        public IntPtr ptr;
        public IntPtr handle;
        public UvAsync(UvLoop loop)
        {
            this.Handle(ref handle);
            this.loop = loop;
            this.OnFire = this.OnFireImpl;

            ptr = UvInterop.xxuv_alloc_uv_async_t(handle);
            if (ptr == IntPtr.Zero)
            {
                this.Unhandle(ref handle);
                throw new OutOfMemoryException();
            }

            int r = UvInterop.xxuv_async_init(loop.ptr, ptr, AsyncCB);
            if (r != 0)
            {
                this.Free(ref ptr);
                this.Unhandle(ref handle);
                r.Throw();
            }

            index_at_container = loop.asyncs.dataLen;
            loop.asyncs.Add(this);
        }

        static UvInterop.uv_async_cb AsyncCB = OnAsyncCBImpl;
        static void OnAsyncCBImpl(IntPtr handle)
        {
            var self = handle.To<UvAsync>();
            self.OnFire();
        }

        public void Fire(Action a)
        {
            if (disposed) throw new ObjectDisposedException("XxUvAsync");
            actions.Enqueue(a);
            UvInterop.xxuv_async_send(ptr).TryThrow();
        }

        public void OnFireImpl()
        {
            while (actions.TryDequeue(out var a)) a();
        }

        #region Dispose

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
                // if (disposing) // Free other state (managed objects).
                actions = null;

                UvInterop.xxuv_close_(ptr);
                this.Free(ref ptr);
                this.Unhandle(ref handle);

                loop.asyncs.SwapRemoveAt(index_at_container);
                loop = null;
                disposed = true;
            }
            //base.Dispose(disposing);
        }

        ~UvAsync()
        {
            Dispose(false);
        }

        #endregion
    }


    public enum UvRunMode
    {
        Default = 0,
        Once,
        NoWait
    }

    public enum UvTcpStates
    {
        Disconnected,
        Connecting,
        Connected,
        Disconnecting,
    };

    public static class UvInterop
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
        public static extern int xxuv_run(IntPtr loop, UvRunMode mode);

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
            var p = UvInterop.xxuv_strerror(n);
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

}
