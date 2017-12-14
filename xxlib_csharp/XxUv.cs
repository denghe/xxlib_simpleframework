using System;
using System.Collections.Concurrent;
using System.Diagnostics;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;

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
            int r = UvInterop.xxuv_run(ptr, mode);
            if (r != 0 && r != 1) r.Throw();    // 1: uv_stop( loop )
        }

        public void Stop()
        {
            if (disposed) throw new ObjectDisposedException("XxUvLoop");
            UvInterop.xxuv_stop(ptr);
        }

        public bool alive
        {
            get
            {
                if (disposed) throw new ObjectDisposedException("XxUvLoop");
                return UvInterop.xxuv_loop_alive(ptr) != 0;
            }
        }

        #region Dispose

        public void Dispose()
        {
            if (disposed) return;
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

                if (UvInterop.xxuv_loop_close(ptr) != 0)                    // busy
                {
                    UvInterop.xxuv_run(ptr, UvRunMode.Default).TryThrow();  // success
                    UvInterop.xxuv_loop_close(ptr).TryThrow();              // success
                }
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
                ptr = IntPtr.Zero;
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
            if (disposed) return;
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
                ptr = IntPtr.Zero;
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

    public class UvTimerBase
    {
        // TimerManager 于 Add 时填充下列成员
        public UvTimerManager timerManager;         // 指向时间管理( 初始为空 )
        public UvTimerBase timerPrev;               // 指向同一 ticks 下的上一 timer
        public UvTimerBase timerNext;               // 指向同一 ticks 下的下一 timer
        public int timerIndex = -1;                 // 位于管理器 timerss 数组的下标
        public Action OnTimerFire;                  // 时间到达后要执行的函数

        public void TimerClear()
        {
            timerPrev = null;
            timerNext = null;
            timerIndex = -1;
        }

        public void TimerStart(int interval = 0)
        {
            if (timerManager == null) throw new InvalidOperationException();
            timerManager.AddOrUpdate(this, interval);
        }
        public void TimerStop()
        {
            if (timerManager == null) throw new InvalidOperationException();
            if (timering) timerManager.Remove(this);
        }

        public void BindTo(UvTimerManager tm)
        {
            if (timerManager != null) throw new InvalidOperationException();
            timerManager = tm;
        }

        public void UnbindTimerManager()
        {
            if (timering) timerManager.Remove(this);
            timerManager = null;
        }

        public bool timering { get { return timerManager != null && (timerIndex != -1 || timerPrev != null); } }
    }

    public abstract class UvTcpBase : UvTimerBase
    {
        /******************************************************************************/
        // 用户事件绑定
        public Action<BBuffer> OnRecvPkg;
        public Action OnDispose;
        public Action<byte[]> OnRecv;   // 默认以 2字节包头 拆解数据, 发起 OnRecvPkg
        /******************************************************************************/

        public bool disposed;
        public UvLoop loop;
        public int index_at_container;

        public IntPtr ptr;
        public IntPtr handle;
        public IntPtr addrPtr;

        protected abstract void DisconnectImpl();

        protected static UvInterop.uv_read_cb OnReadCB = OnReadCBImpl;
        static void OnReadCBImpl(IntPtr stream, IntPtr nread, IntPtr buf_t)
        {
            var tcp = stream.To<UvTcpBase>();
            var bufPtr = UvInterop.xxuv_get_buf(buf_t);
            int len = (int)nread;
            if (len > 0)
            {
                var buf = new byte[len];
                Marshal.Copy(bufPtr, buf, 0, len);
                tcp.OnRecv(buf);
            }
            else if (len < 0)
            {
                tcp.DisconnectImpl();
            }
            UvInterop.xxuv_free(bufPtr);
        }

        protected byte[] buf;
        protected int offset;
        protected BBuffer bbRecv = new BBuffer();
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
                if (dataLen == 0)
                {
                    buf = null;
                    DisconnectImpl();
                    break;
                }

                // 确保数据长
                if (buf.Length - offset < dataLen)
                {
                    offset -= 2;                        // 回滚偏移量, 停在 包头 位置
                    break;
                }

                // 发起包回调
                if (OnRecvPkg != null)
                {
                    bbRecv.Assign(buf, offset, dataLen);
                    OnRecvPkg(bbRecv);
                }

                // 继续处理剩余数据
                offset += dataLen;
            }
        }


        public void Send(byte[] data, int offset = 0, int len = 0)
        {
            if (disposed) throw new ObjectDisposedException("XxUvTcpBase");
            if (data == null || data.Length == 0) throw new NullReferenceException();
            if (offset + len > data.Length) throw new IndexOutOfRangeException();
            if (data.Length == offset) throw new NullReferenceException();
            if (len == 0) len = data.Length - offset;
            var h = GCHandle.Alloc(data, GCHandleType.Pinned);
            UvInterop.xxuv_write_(ptr, h.AddrOfPinnedObject(), (uint)offset, (uint)len);
            h.Free();
        }

        public void Send(BBuffer bb)
        {
            if (bb.dataLen == 0) throw new NullReferenceException();
            var h = GCHandle.Alloc(bb.buf, GCHandleType.Pinned);
            UvInterop.xxuv_write_(ptr, h.AddrOfPinnedObject(), 0, (uint)bb.dataLen);
            h.Free();
        }


        protected BBuffer bbSend;
        public int SendPackages(params xx.IBBuffer[] pkgs)
        {
            if (pkgs == null || pkgs.Length == 0) throw new NullReferenceException();
            if (bbSend == null) bbSend = new BBuffer(65536);
            var sum = 0;
            var len = pkgs.Length;
            for (int i = 0; i < len; ++i)
            {
                var pkg = pkgs[i];
                bbSend.Clear();
                bbSend.BeginWritePackage();
                bbSend.WriteRoot(pkg);
                if (!bbSend.EndWritePackage()) throw new OverflowException();
                sum += bbSend.dataLen;
                Send(bbSend.buf, 0, bbSend.dataLen);
            }
            return sum;
        }

        public int SendCombine(params xx.IBBuffer[] pkgs)
        {
            if (bbSend == null) bbSend = new BBuffer(65536);
            bbSend.Clear();
            bbSend.BeginWritePackage();
            var len = pkgs.Length;
            for (int i = 0; i < len; ++i)
            {
                var ibb = pkgs[i];
                bbSend.WriteRoot(ibb);
            }
            if (!bbSend.EndWritePackage()) throw new OverflowException();
            Send(bbSend.buf, 0, bbSend.dataLen);
            return bbSend.dataLen;
        }


        // 专为 echo 回发包数据而写( 非 常用 )
        public void SendRecvPkg(BBuffer bb)
        {
            Send(bb.buf, bb.offset - 2, bb.dataLen - bb.offset + 2);
        }
    }

    public class UvTcpPeer : UvTcpBase, IDisposable
    {
        public bool alive { get { return !disposed; } }

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
                ptr = IntPtr.Zero;
                this.Unhandle(ref handle);
                r.Throw();
            }

            r = UvInterop.xxuv_read_start_(ptr, OnReadCB);
            if (r != 0)
            {
                UvInterop.xxuv_close_(ptr);
                ptr = IntPtr.Zero;
                this.Unhandle(ref handle);
                r.Throw();
            }

            addrPtr = UvInterop.xxuv_alloc_sockaddr_in(IntPtr.Zero);
            if (addrPtr == IntPtr.Zero)
            {
                UvInterop.xxuv_close_(ptr);
                ptr = IntPtr.Zero;
                this.Unhandle(ref handle);
                throw new OutOfMemoryException();
            }

            index_at_container = listener.peers.dataLen;
            listener.peers.Add(this);
        }

        protected override void DisconnectImpl()
        {
            Dispose();
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

        public void Dispose()
        {
            if (disposed) return;
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
                ptr = IntPtr.Zero;
                this.Free(ref addrPtr);
                this.Unhandle(ref handle);

                UnbindTimerManager();
                OnTimerFire = null;

                buf = null;
                bbSend = null;
                bbRecv = null;
                listener.peers.SwapRemoveAt(index_at_container);
                listener = null;
                loop = null;
                disposed = true;
            }
            //base.Dispose(disposing);
        }

        ~UvTcpPeer()
        {
            Dispose(false);
        }

        #endregion
    }

    public class UvTcpClient : UvTcpBase, IDisposable
    {
        /******************************************************************************/
        // 用户事件绑定
        public Action<int> OnConnect;
        public Action OnDisconnect;
        /******************************************************************************/

        public UvTcpStates state;
        public bool alive { get { return !disposed && state == UvTcpStates.Connected; } }

        public UvTcpClient(UvLoop loop)
        {
            this.Handle(ref handle);
            this.loop = loop;
            this.OnRecv = this.OnRecvImpl;

            addrPtr = UvInterop.xxuv_alloc_sockaddr_in(IntPtr.Zero);
            if (addrPtr == IntPtr.Zero)
            {
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

            ptr = UvInterop.xxuv_alloc_uv_tcp_t(handle);
            if (ptr == IntPtr.Zero) throw new OutOfMemoryException();

            int r = UvInterop.xxuv_tcp_init(loop.ptr, ptr);
            if (r != 0)
            {
                this.Free(ref ptr);
                r.Throw();
            }

            r = UvInterop.xxuv_tcp_connect_(ptr, addrPtr, OnConnectCB);
            if (r != 0)
            {
                UvInterop.xxuv_close_(ptr);
                ptr = IntPtr.Zero;
                r.Throw();
            }

            state = UvTcpStates.Connecting;
        }

        public void Disconnect()
        {
            if (disposed) throw new ObjectDisposedException("XxUvTcpClient");
            if (state == UvTcpStates.Disconnected) return;
            UvInterop.xxuv_close_(ptr);
            ptr = IntPtr.Zero;
            state = UvTcpStates.Disconnected;
        }

        protected override void DisconnectImpl()
        {
            Disconnect();
            if (OnDisconnect != null) OnDisconnect();
        }

        #region Dispose

        public void Dispose()
        {
            if (disposed) return;
            if (OnDispose != null) OnDispose();
            Dispose(true);
            GC.SuppressFinalize(this);
        }

        protected virtual void Dispose(bool disposing)
        {
            if (!disposed)
            {
                // if (disposing) // Free other state (managed objects).

                state = UvTcpStates.Disconnected;
                if (ptr != IntPtr.Zero)
                {
                    UvInterop.xxuv_close_(ptr);
                    ptr = IntPtr.Zero;
                }
                this.Free(ref addrPtr);
                this.Unhandle(ref handle);

                UnbindTimerManager();
                OnTimerFire = null;

                buf = null;
                bbSend = null;
                bbRecv = null;
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

        public UvTimer(UvLoop loop, ulong timeoutMS, ulong repeatIntervalMS, Action OnFire)
            : this(loop, timeoutMS, repeatIntervalMS)
        {
            this.OnFire = OnFire;
        }

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
                ptr = IntPtr.Zero;
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
            if (disposed) return;
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
                ptr = IntPtr.Zero;
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
            if (disposed) return;
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
                ptr = IntPtr.Zero;
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

    public class UvTimerManager
    {
        UvTimer timer;
        List<UvTimerBase> timerss = new List<UvTimerBase>();
        int cursor = 0;                         // 环形游标
        int defaultInterval;

        public UvTimerManager(UvLoop loop, ulong intervalMS, int wheelLen, int defaultInterval)
        {
            timer = new UvTimer(loop, 0, intervalMS, Process);
            timerss.Resize(wheelLen);
            this.defaultInterval = defaultInterval;
        }

        public void Process()
        {
            var t = timerss[cursor];            // 遍历当前 ticks 链表
            while (t != null)
            {
                t.OnTimerFire();                // 执行
                var nt = t.timerNext;
                t.TimerClear();
                t = nt;
            };
            timerss[cursor] = null;
            cursor++;                           // 环移游标
            if (cursor == timerss.dataLen) cursor = 0;
        }

        // 不触发 OnTimerFire
        public void Clear()
        {
            for (int i = 0; i < timerss.dataLen; ++i)
            {
                var t = timerss[i];
                while (t != null)               // 遍历链表
                {
                    var nt = t.timerNext;
                    t.TimerClear();             // 清理关联
                    t = nt;
                };
                timerss[i] = null;              // 清空链表头
            }
            cursor = 0;
        }

        // 于指定 interval 所在 timers 链表处放入一个 timer
        public void Add(UvTimerBase t, int interval = 0)
        {
            if (t.timering) throw new InvalidOperationException();
            var timerssLen = timerss.dataLen;
            if (t == null || (interval < 0 && interval >= timerss.dataLen)) throw new ArgumentException();
            if (interval == 0) interval = defaultInterval;

            // 环形定位到 timers 下标
            interval += cursor;
            if (interval >= timerssLen) interval -= timerssLen;

            // 填充 链表信息
            t.timerPrev = null;
            t.timerIndex = interval;
            t.timerNext = timerss[interval];
            if (t.timerNext != null)            // 有就链起来
            {
                t.timerNext.timerPrev = t;
            }
            timerss[interval] = t;              // 成为链表头
        }

        // 移除
        public void Remove(UvTimerBase t)
        {
            if (!t.timering) throw new InvalidOperationException();
            if (t.timerNext != null) t.timerNext.timerPrev = t.timerPrev;
            if (t.timerPrev != null) t.timerPrev.timerNext = t.timerNext;
            else timerss[t.timerIndex] = t.timerNext;
            t.TimerClear();
        }

        // 如果存在就移除并放置到新的时间点
        public void AddOrUpdate(UvTimerBase t, int interval = 0)
        {
            if (t.timering) Remove(t);
            Add(t, interval);
        }

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
        public static extern IntPtr xxuv_alloc_uv_signal_t(IntPtr ud);



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
        public static extern void xxuv_close_(IntPtr handle);   // auto free




        [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern int xxuv_loop_init(IntPtr loop);

        [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern int xxuv_run(IntPtr loop, UvRunMode mode);

        [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern void xxuv_stop(IntPtr loop);

        [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern int xxuv_loop_close(IntPtr loop);

        [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern int xxuv_loop_alive(IntPtr loop);




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
        public static extern int xxuv_write_(IntPtr stream, IntPtr buf, uint offset, uint len);

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
