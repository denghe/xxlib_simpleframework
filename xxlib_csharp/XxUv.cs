using System;
using System.Runtime.InteropServices;
using System.Collections.Concurrent;
using System.Text;

namespace xx
{
    public struct Pair<First, Second>
    {
        public First first;
        public Second second;
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

    public class UvLoop : IDisposable
    {
        public bool disposed;
        public List<UvUdpListener> udpListeners = new List<UvUdpListener>();
        public List<UvTcpListener> tcpListeners = new List<UvTcpListener>();
        public List<UvTcpClient> tcpClients = new List<UvTcpClient>();
        public List<UvTimer> timers = new List<UvTimer>();
        public List<UvAsync> asyncs = new List<UvAsync>();
        public UvRpcManager rpcMgr;

        public IntPtr ptr;
        public IntPtr handle;

        public UvLoop(ulong rpcIntervalMS = 1000, int rpcDefaultInterval = 5)
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

            rpcMgr = new UvRpcManager(this, rpcIntervalMS, rpcDefaultInterval);
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
                tcpClients.ForEachReverse(o => o.Dispose());
                tcpListeners.ForEachReverse(o => o.Dispose());

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

            index_at_container = loop.tcpListeners.dataLen;
            loop.tcpListeners.Add(this);
        }


        static UvInterop.uv_connection_cb OnAcceptCB = OnAcceptCBImpl;
        static void OnAcceptCBImpl(IntPtr stream, int status)
        {
            if (status != 0) return;
            var listener = stream.To<UvTcpListener>();
            UvTcpPeer peer = null;
            try
            {
                if (listener.OnCreatePeer != null) peer = listener.OnCreatePeer();
                else peer = new UvTcpPeer(listener);
            }
            catch
            {
                return;
            }
            if (listener.OnAccept != null) listener.OnAccept(peer);
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

                loop.tcpListeners.SwapRemoveAt(index_at_container);
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

    // 包头设计: mask( 1 byte ) + len( 2 bytes ) + [serial( varlen uinteger )] + data( byte[len - sizeof(serial)] )
    // 首字节为类型路由: 0: 一般数据包   1: RPC请求包   2: RPC回应包   3+: 其他( custom )
    // RPC包将会在 data 的起始区域夹带一个变长无符号整数 作为 流水号
    public abstract class UvTcpUdpBase : UvTimerBase
    {
        /******************************************************************************/
        // 用户事件绑定
        public Action<BBuffer> OnReceivePackage;
        public Action<uint, BBuffer> OnReceiveRequest;
        public Action OnDispose;
        /******************************************************************************/

        public bool disposed;
        public UvLoop loop;
        public int index_at_container;

        public IntPtr ptr;
        public IntPtr handle;
        public IntPtr addrPtr;

        protected BBuffer bbRecv = new BBuffer();               // 复用的接收缓冲区
        protected BBuffer bbSend;                               // 复用

        protected abstract void DisconnectImpl();
        protected abstract void ReceivePackageImpl(BBuffer bb);
        protected abstract void ReceiveRequestImpl(uint serial, BBuffer bb);
        protected abstract void ReceiveResponseImpl(uint serial, BBuffer bb);
        protected abstract bool Disconnected();


        // 基础收数据处理, 投递到事件函数
        public void ReceiveImpl(IntPtr bufPtr, int len)
        {
            bbRecv.WriteBuf(bufPtr, len);                   // 追加收到的数据到接收缓冲区

            var buf = bbRecv.buf;                           // 方便使用
            int offset = 0;                                 // 提速
            while (offset + 3 <= bbRecv.dataLen)            // 确保 3字节 包头长度
            {
                var typeId = buf[offset];                   // 读出头
                var dataLen = buf[offset + 1] + (buf[offset + 2] << 8);
                if (dataLen <= 0)                           // 数据异常
                {
                    DisconnectImpl();
                    return;
                }
                if (offset + 3 + dataLen > bbRecv.dataLen) break;   // 确保数据长
                offset += 3;

                bbRecv.offset = offset;
                if (typeId == 0)
                {
                    ReceivePackageImpl(bbRecv);
                    if (Disconnected())
                    {
                        bbRecv.Clear();
                        return;
                    }
                }
                else
                {
                    uint serial = 0;
                    if (!bbRecv.TryRead(ref serial))
                    {
                        DisconnectImpl();
                        return;
                    }
                    if (typeId == 1)
                    {
                        ReceiveRequestImpl(serial, bbRecv);
                        if (Disconnected())
                        {
                            bbRecv.Clear();
                            return;
                        }
                    }
                    else if (typeId == 2)
                    {
                        ReceiveResponseImpl(serial, bbRecv);
                        if (Disconnected())
                        {
                            bbRecv.Clear();
                            return;
                        }
                    }
                }
                offset += dataLen;                          // 继续处理剩余数据
            }
            if (offset < bbRecv.dataLen)                    // 还有剩余的数据: 移到最前面
            {
                Buffer.BlockCopy(buf, offset, buf, 0, bbRecv.dataLen - offset);
            }
            bbRecv.dataLen -= offset;
        }

        // 取没发出去的数据队列长度( 便于逻辑上及时 Dispose, 避免目标无接收能力而堆积数据吃内存 )
        public abstract int GetSendQueueSize();

        // 原始数据发送
        public abstract void SendBytes(byte[] data, int offset = 0, int len = 0);

        // 原始数据发送之 BBuffer 版( 发送 BBuffer 中的数据 )
        public void SendBytes(BBuffer bb)
        {
            SendBytes(bb.buf, 0, bb.dataLen);
        }

        // 每个类一个包, 返回总字节数
        public int Send(params xx.IBBuffer[] pkgs)
        {
            if (disposed) throw new ObjectDisposedException("XxUvTcpBase");
            if (pkgs == null || pkgs.Length == 0) throw new NullReferenceException();
            if (bbSend == null) bbSend = new BBuffer(65536);
            var sum = 0;
            var len = pkgs.Length;
            for (int i = 0; i < len; ++i)
            {
                var pkg = pkgs[i];
                bbSend.Clear();
                bbSend.BeginWritePackageEx();
                bbSend.WriteRoot(pkg);
                bbSend.EndWritePackageEx();
                sum += bbSend.dataLen;
                SendBytes(bbSend.buf, 0, bbSend.dataLen);
            }
            return sum;
        }

        // 合并所有类一个包, 返回总字节数
        public int SendCombine(params xx.IBBuffer[] pkgs)
        {
            if (disposed) throw new ObjectDisposedException("XxUvTcpBase");
            if (bbSend == null) bbSend = new BBuffer(65536);
            bbSend.Clear();
            bbSend.BeginWritePackageEx();
            var len = pkgs.Length;
            for (int i = 0; i < len; ++i)
            {
                var ibb = pkgs[i];
                bbSend.WriteRoot(ibb);
            }
            bbSend.EndWritePackageEx();
            SendBytes(bbSend.buf, 0, bbSend.dataLen);
            return bbSend.dataLen;
        }

        // 只能单发一个类, 返回流水号
        public uint SendRequest(xx.IBBuffer pkg, Action<uint, BBuffer> cb, int interval = 0)
        {
            if (disposed) throw new ObjectDisposedException("XxUvTcpBase");
            if (loop.rpcMgr == null) throw new NullReferenceException();
            if (bbSend == null) bbSend = new BBuffer(65536);
            var serial = loop.rpcMgr.Register(cb, interval);
            bbSend.Clear();
            bbSend.BeginWritePackageEx(true, serial);
            bbSend.WriteRoot(pkg);
            bbSend.EndWritePackageEx(1);
            SendBytes(bbSend.buf, 0, bbSend.dataLen);
            return serial;
        }

        // 发送 RPC 的应答包, 返回字节数
        public int SendResponse(uint serial, xx.IBBuffer pkg)
        {
            if (disposed) throw new ObjectDisposedException("XxUvTcpBase");
            if (bbSend == null) bbSend = new BBuffer(65536);
            bbSend.Clear();
            bbSend.BeginWritePackageEx(true, serial);
            bbSend.WriteRoot(pkg);
            bbSend.EndWritePackageEx(2);
            SendBytes(bbSend.buf, 0, bbSend.dataLen);
            return bbSend.dataLen;
        }
    }

    public abstract class UvTcpBase : UvTcpUdpBase
    {
        /******************************************************************************/
        // 用户事件绑定
        public Action<BBuffer> OnReceivePackage;
        public Action<uint, BBuffer> OnReceiveRequest;
        /******************************************************************************/

        protected override void ReceivePackageImpl(BBuffer bb)
        {
            if (OnReceivePackage != null) OnReceivePackage(bb);
        }
        protected override void ReceiveRequestImpl(uint serial, BBuffer bb)
        {
            if (OnReceiveRequest != null) OnReceiveRequest(serial, bb);
        }
        protected override void ReceiveResponseImpl(uint serial, BBuffer bb)
        {
            loop.rpcMgr.Callback(serial, bb);
        }
        protected override bool Disconnected()
        {
            return ptr == IntPtr.Zero;
        }

        protected static UvInterop.uv_read_cb OnReadCB = OnReadCBImpl;
        static void OnReadCBImpl(IntPtr stream, IntPtr nread, IntPtr buf_t)
        {
            var tcp = stream.To<UvTcpBase>();
            var loopPtr = tcp.loop.ptr;        // 防事件 Dispose 先取出来
            var bufPtr = UvInterop.xxuv_get_buf(buf_t);
            int len = (int)nread;
            if (len > 0)
            {
                tcp.ReceiveImpl(bufPtr, len);
            }
            UvInterop.xxuv_pool_free(loopPtr, bufPtr);
            if (len < 0)
            {
                tcp.DisconnectImpl();
            }
        }

        public override void SendBytes(byte[] data, int offset = 0, int len = 0)
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

        // 取待发送队列长度
        // 如果遇到发送量较大的场景, 应该抽空做这个检查, 及时 Dispose, 避免目标无接收能力而堆积数据吃内存.
        public override int GetSendQueueSize()
        {
            if (disposed) throw new ObjectDisposedException("XxUvTcpBase");
            return (int)UvInterop.xxuv_stream_get_write_queue_size(ptr);
        }
    }

    public class UvTcpPeer : UvTcpBase, IDisposable
    {
        public bool alive { get { return !disposed; } }

        public UvTcpListener listener;
        public UvTcpPeer(UvTcpListener listener)
        {
            this.Handle(ref handle);
            this.listener = listener;
            this.loop = listener.loop;

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
                OnTimeout = null;

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

            addrPtr = UvInterop.xxuv_alloc_sockaddr_in(IntPtr.Zero);
            if (addrPtr == IntPtr.Zero)
            {
                this.Unhandle(ref handle);
                throw new OutOfMemoryException();
            }

            index_at_container = loop.tcpClients.dataLen;
            loop.tcpClients.Add(this);
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
                OnTimeout = null;

                bbSend = null;
                bbRecv = null;
                loop.tcpClients.SwapRemoveAt(index_at_container);
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

    public class UvTimerBase
    {
        // TimerManager 于 Add 时填充下列成员
        public UvTimeouter timerManager;         // 指向时间管理( 初始为空 )
        public UvTimerBase timerPrev;               // 指向同一 ticks 下的上一 timer
        public UvTimerBase timerNext;               // 指向同一 ticks 下的下一 timer
        public int timerIndex = -1;                 // 位于管理器 timerss 数组的下标
        public Action OnTimeout;                  // 时间到达后要执行的函数

        public void TimerClear()
        {
            timerPrev = null;
            timerNext = null;
            timerIndex = -1;
        }

        public void TimeoutReset(int interval = 0)
        {
            if (timerManager == null) throw new InvalidOperationException();
            timerManager.AddOrUpdate(this, interval);
        }
        public void TimerStop()
        {
            if (timerManager == null) throw new InvalidOperationException();
            if (timering) timerManager.Remove(this);
        }

        public void BindTo(UvTimeouter tm)
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

    public class UvTimeouter
    {
        UvTimer timer;
        List<UvTimerBase> timerss = new List<UvTimerBase>();
        int cursor = 0;                         // 环形游标
        int defaultInterval;

        // intervalMS: 帧间隔毫秒数;    wheelLen: 轮子尺寸( 需求的最大计时帧数 + 1 );    defaultInterval: 默认计时帧数
        public UvTimeouter(UvLoop loop, ulong intervalMS, int wheelLen, int defaultInterval)
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
                t.OnTimeout();                // 执行
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

        public void Dispatch(Action a)
        {
            if (disposed) throw new ObjectDisposedException("XxUvAsync");
            actions.Enqueue(a);
            UvInterop.xxuv_async_send(ptr).TryThrow();
        }

        public void OnFireImpl()
        {
            Action a;
            while (actions.TryDequeue(out a)) a();
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

    public class UvRpcManager
    {
        // 已知问题: 当 uv 太繁忙时, timer 可能无法及时回调, 导致超时和队列清理行为无法及时发生
        UvTimer timer;

        // 循环使用的自增流水号
        uint serial;

        // 流水号 与 cb(bb) 回调 的映射. bb 为空表示超时调用
        Dict<uint, Action<uint, BBuffer>> mapping = new Dict<uint, Action<uint, BBuffer>>();

        // 用一个队列记录流水号的超时时间, 以便删掉超时的. first: timeout ticks
        Queue<Pair<int, uint>> serials = new Queue<Pair<int, uint>>();

        // 默认计时帧数
        int defaultInterval;

        // 帧步进值
        int ticks;

        // 返回队列深度
        public int count { get { return serials.Count; } }

        // intervalMS: 帧间隔毫秒数;    defaultInterval: 默认计时帧数
        public UvRpcManager(UvLoop loop, ulong intervalMS, int defaultInterval)
        {
            if (defaultInterval <= 0) throw new ArgumentException();
            timer = new UvTimer(loop, 0, intervalMS, Process);
            this.defaultInterval = defaultInterval;
        }

        // 不断将超时的从字典移除( 以 false 参数 call 之 ), 直到 Pop 到未超时的停止.
        public void Process()
        {
            ++ticks;
            if (serials.IsEmpty) return;
            while (!serials.IsEmpty && serials.Top().first <= ticks)
            {
                var idx = mapping.Find(serials.Top().second);
                if (idx != -1)
                {
                    var a = mapping.ValueAt(idx);
                    mapping.RemoveAt(idx);
                    a(serial, null);
                }
                serials.Pop();
            }
        }

        // 放入上下文, 返回流水号
        public uint Register(Action<uint, BBuffer> cb, int interval = 0)
        {
            if (interval == 0) interval = defaultInterval;
            unchecked { ++serial; }                     // 循环自增
            var r = mapping.Add(serial, cb, true);
            serials.Push(new Pair<int, uint>
            {
                first = ticks + interval,       // 算出超时 ticks
                second = serial
            });
            return serial;
        }

        // 根据流水号 反注册回调事件( 通常出现于提前断线或退出之后不想收到相关回调 )
        // 这种情况不产生回调
        public void Unregister(uint serial)
        {
            mapping.Remove(serial);
        }

        // 根据 流水号 定位到 回调函数并调用( 由 UvTcpXxxx 来 call )
        public void Callback(uint serial, BBuffer bb)
        {
            int idx = mapping.Find(serial);
            if (idx == -1) return;              // 已超时移除
            var a = mapping.ValueAt(idx);
            mapping.RemoveAt(idx);
            if (a != null) a(serial, bb);
        }
    }

    public abstract class UvContextBase
    {
        public UvTcpPeer peer;
        public bool peerAlive { get { return peer != null && peer.alive; } }

        // 绑连接. 成功返回 true
        public bool BindPeer(UvTcpPeer p)
        {
            if (this.peer != null) return false;
            p.OnReceiveRequest = OnPeerReceiveRequest;
            p.OnReceivePackage = OnPeerReceivePackage;
            p.OnDispose = OnPeerDisconnect;
            this.peer = p;
            return true;
        }

        // T 掉已有连接, 不会产生 OnDispose 调用( 如果非立即断开, 则会在几秒后被 timeouter 干掉 )
        public void KickPeer(bool immediately = true)
        {
            if (peer != null)
            {
                peer.OnDispose = null;              // 防止产生 OnDispose 调用
                peer.OnReceivePackage = null;       // 清空收发包回调
                peer.OnReceiveRequest = null;

                if (immediately)
                {
                    peer.Dispose();                 // 立即断开
                }
                peer = null;
            }
        }

        // RPC 请求解包, 调处理函数
        public void OnPeerReceiveRequest(uint serial, BBuffer bb)
        {
            var ibb = bb.TryReadPackage<IBBuffer>();
            if (ibb == null)
            {
                KickPeer();
                return;
            }
            peer.TimeoutReset();
            HandleRequest(serial, ibb);
        }

        // 普通 解包, 调处理函数
        public void OnPeerReceivePackage(BBuffer bb)
        {
            var ibb = bb.TryReadPackage<IBBuffer>();
            if (ibb == null)
            {
                KickPeer();
                return;
            }
            peer.TimeoutReset();
            HandlePackage(ibb);
        }

        public void OnPeerDisconnect()
        {
            KickPeer(false);
            HandleDisconnect();
        }


        // 需要实现的函数些
        public abstract void HandleRequest(uint serial, IBBuffer ibb);
        public abstract void HandlePackage(IBBuffer ibb);
        public abstract void HandleDisconnect();
    }

    public class UvUdpListener : IDisposable
    {
        /******************************************************************************/
        // 用户事件绑定区
        public Func<UvUdpPeer> OnCreatePeer;
        public Action<UvUdpPeer> OnAccept;
        public Action OnDispose;
        /******************************************************************************/

        public Dict<Guid, UvUdpPeer> peers = new Dict<Guid, UvUdpPeer>();
        // todo: timer for call peers updates every 10 ms

        public bool disposed;
        public UvLoop loop;
        public int index_at_container;

        public IntPtr ptr;
        public IntPtr handle;
        public IntPtr addrPtr;

        public byte[] guid = new byte[16];

        public UvUdpListener(UvLoop loop)
        {
            this.Handle(ref handle);
            this.loop = loop;

            ptr = UvInterop.xxuv_alloc_uv_udp_t(handle);
            if (ptr == IntPtr.Zero)
            {
                this.Unhandle(ref handle);
                throw new OutOfMemoryException();
            }

            int r = UvInterop.xxuv_udp_init(loop.ptr, ptr);
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

            index_at_container = loop.udpListeners.dataLen;
            loop.udpListeners.Add(this);
        }

        static UvInterop.uv_udp_recv_cb OnRecvCB = OnRecvCBImpl;
        static void OnRecvCBImpl(IntPtr udp, IntPtr nread, IntPtr buf_t, IntPtr addr, uint flags)
        {
            var listener = udp.To<UvUdpListener>();
            var loopPtr = listener.loop.ptr;        // 防事件 Dispose 先取出来
            var bufPtr = UvInterop.xxuv_get_buf(buf_t);
            var len = (int)nread;
            if (len == 0) throw new Exception();
            listener.OnReceiveImpl(bufPtr, len, addr);
            UvInterop.xxuv_pool_free(loopPtr, bufPtr);
        }

        public void OnReceiveImpl(IntPtr bufPtr, int len, IntPtr addr)
        {
            // 所有消息长度至少都有 36 字节长( Guid conv 的 kcp 头 )
            if (len < 36) return;

            // 前 16 字节转为 Guid 在字节中建连接
            Marshal.Copy(bufPtr, guid, 0, 16);
            var g = new Guid(guid);

            // 去字典中找. 没有就新建.
            int idx = peers.Find(g);
            UvUdpPeer p = null;
            if (idx < 0)
            {
                try
                {
                    p = new UvUdpPeer(this, g, bufPtr, addr);
                }
                catch
                {
                    return;
                }
                peers.Add(g, p);
            }
            else
            {
                p = peers.ValueAt(idx);
            }
            // 无脑更新 peer 的目标 ip 地址
            UvInterop.xxuv_addr_copy(addr, p.addrPtr);

            // call peer 的函数
            p.ReceiveImpl(bufPtr, len);
        }

        public void RecvStart()
        {
            if (disposed) throw new ObjectDisposedException("XxUvUdpListener");
            UvInterop.xxuv_udp_recv_start_(ptr, OnRecvCB).TryThrow();
        }

        public void RecvStop()
        {
            if (disposed) throw new ObjectDisposedException("XxUvUdpListener");
            UvInterop.xxuv_udp_recv_stop(ptr).TryThrow();
        }

        public void Bind(string ipv4, int port)
        {
            if (disposed) throw new ObjectDisposedException("XxUvUdpListener");
            UvInterop.xxuv_ip4_addr(ipv4, port, addrPtr).TryThrow();
            UvInterop.xxuv_udp_bind_(ptr, addrPtr).TryThrow();
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
                peers.ForEach(kv => kv.value.Dispose());
                peers.Clear();

                UvInterop.xxuv_close_(ptr);
                ptr = IntPtr.Zero;
                this.Free(ref addrPtr);
                this.Unhandle(ref handle);

                loop.udpListeners.SwapRemoveAt(index_at_container);
                loop = null;
                disposed = true;
            }
            //base.Dispose(disposing);
        }

        ~UvUdpListener()
        {
            Dispose(false);
        }

        #endregion
    }

    public class UvUdpPeer : UvTcpUdpBase, IDisposable
    {
        UvUdpListener listener;
        Guid guid;

        public UvUdpPeer(UvUdpListener listener, Guid guid, IntPtr rawData, IntPtr addr)
        {
            this.listener = listener;
            this.guid = guid;
            this.loop = listener.loop;
            this.Handle(ref handle);

            ptr = UvInterop.xx_ikcp_create(rawData, handle, loop.ptr);
            if (ptr == IntPtr.Zero)
            {
                this.Unhandle(ref handle);
                throw new OutOfMemoryException();
            }
            UvInterop.xx_ikcp_setoutput(ptr, OutputCB);
            // todo: 继续初始化 kcp 参数

            addrPtr = UvInterop.xxuv_alloc_sockaddr_in(IntPtr.Zero);
            if (addrPtr == IntPtr.Zero)
            {
                UvInterop.xx_ikcp_release(ptr);
                ptr = IntPtr.Zero;
                this.Unhandle(ref handle);
                throw new OutOfMemoryException();
            }

            index_at_container = listener.peers.Add(guid, this).index;
        }

        static UvInterop.ikcp_output_cb OutputCB = OutputImpl;
        static int OutputImpl(IntPtr buf, int len, IntPtr kcp)
        {
            var peer = (UvUdpPeer)((GCHandle)UvInterop.xx_ikcp_get_ud(kcp)).Target;
            UvInterop.xxuv_udp_send_(peer.listener.ptr, buf, 0, (uint)len, peer.addrPtr);
            return 0;
        }

        public void Update(uint current)
        {
            if (disposed) throw new ObjectDisposedException("UvUdpPeer");
            UvInterop.xx_ikcp_update(ptr, current);
        }

        protected override void DisconnectImpl()
        {
            Dispose();
        }

        public override int GetSendQueueSize()
        {
            return (int)UvInterop.xxuv_udp_get_send_queue_size(listener.ptr);   // todo
        }

        protected override bool Disconnected()
        {
            return disposed;
        }

        public override void SendBytes(byte[] data, int offset = 0, int len = 0)
        {
            if (disposed) throw new ObjectDisposedException("UvUdpPeer");
            if (data == null || data.Length == 0) throw new NullReferenceException();
            if (offset + len > data.Length) throw new IndexOutOfRangeException();
            if (data.Length == offset) throw new NullReferenceException();
            if (len == 0) len = data.Length - offset;
            var h = GCHandle.Alloc(data, GCHandleType.Pinned);
            UvInterop.xx_ikcp_send(ptr, h.AddrOfPinnedObject(), offset, len);
            h.Free();
        }

        protected override void ReceivePackageImpl(BBuffer bb)
        {
            if (OnReceivePackage != null) OnReceivePackage(bb);
        }
        protected override void ReceiveRequestImpl(uint serial, BBuffer bb)
        {
            if (OnReceiveRequest != null) OnReceiveRequest(serial, bb);
        }
        protected override void ReceiveResponseImpl(uint serial, BBuffer bb)
        {
            loop.rpcMgr.Callback(serial, bb);
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
                UvInterop.xx_ikcp_release(ptr);
                ptr = IntPtr.Zero;
                this.Free(ref addrPtr);
                this.Unhandle(ref handle);


                UnbindTimerManager();
                OnTimeout = null;

                bbSend = null;
                bbRecv = null;
                listener.peers.RemoveAt(index_at_container);
                index_at_container = -1;
                listener = null;
                loop = null;
                disposed = true;
            }
            //base.Dispose(disposing);
        }

        ~UvUdpPeer()
        {
            Dispose(false);
        }

        #endregion
    }

    // todo: UvUdpClient

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
        public static extern IntPtr xxuv_alloc_uv_udp_t(IntPtr ud);

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
        public static extern void xxuv_pool_free(IntPtr loop, IntPtr p);



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
        public static extern int xxuv_udp_init(IntPtr loop, IntPtr udp);

        //[DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
        //public static extern int xxuv_udp_bind(IntPtr udp, IntPtr addr, uint flags);

        [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern int xxuv_udp_bind_(IntPtr udp, IntPtr addr);

        //[DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
        //public static extern int xxuv_udp_recv_start(IntPtr udp, uv_alloc_cb alloc_cb, uv_udp_recv_cb recv_cb);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate void uv_udp_recv_cb(IntPtr udp, IntPtr nread, IntPtr buf_t, IntPtr addr, uint flags);

        [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern int xxuv_udp_recv_start_(IntPtr udp, uv_udp_recv_cb recv_cb);

        [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern int xxuv_udp_recv_stop(IntPtr udp);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate void uv_udp_send_cb(IntPtr req, int status);

        //[DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
        //public static extern int xxuv_udp_send(IntPtr req, IntPtr udp, uv_buf_t bufs[], uint nbufs, IntPtr addr, uv_udp_send_cb send_cb);

        [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern int xxuv_udp_send_(IntPtr udp, IntPtr buf, uint offset, uint len, IntPtr addr);

        [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr xxuv_udp_get_send_queue_size(IntPtr udp);





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





        [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern int xxuv_is_readable(IntPtr stream);

        [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern int xxuv_is_writable(IntPtr stream);

        [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr xxuv_stream_get_write_queue_size(IntPtr stream);

        [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern int xxuv_try_write_(IntPtr stream, IntPtr buf, uint len);





        [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr xx_ikcp_create(IntPtr conv, IntPtr ud, IntPtr loop);

        [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern void xx_ikcp_release(IntPtr kcp);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int ikcp_output_cb(IntPtr buf, int len, IntPtr kcp);

        [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern void xx_ikcp_setoutput(IntPtr kcp, ikcp_output_cb cb);

        [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr xx_ikcp_get_ud(IntPtr kcp);

        [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern int xx_ikcp_input(IntPtr kcp, IntPtr data, int size);

        [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern int xx_ikcp_send(IntPtr kcp, IntPtr buffer, int offset, int len);

        [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern void xx_ikcp_update(IntPtr kcp, uint current);

        [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern uint xx_ikcp_check(IntPtr kcp, uint current);

        [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern void xxuv_addr_copy(IntPtr from, IntPtr to);

        [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern int xxuv_fill_ip(IntPtr addr, IntPtr buf, int buf_len, ref int data_len);





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

    // u3d 下 模拟 ConcurrentQueue 的用法, 用 lock + queue 浅封 for unity
#if NET_2_0 || NET_2_0_SUBSET
    public class ConcurrentQueue<T>
    {
        protected xx.Queue<T> queue;

        public void Enqueue(T a)
        {
            lock (queue) queue.Enqueue(a);
        }

        public bool TryDequeue(out T a)
        {
            lock (queue) return queue.TryDequeue(out a);
        }
    }
#endif
}
