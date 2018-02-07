using System;
using System.Runtime.InteropServices;

namespace xx
{

    public class NBSocket : IDisposable
    {
        /******************************************************************************/
        // 用户事件绑定区
        public Action<bool> OnConnect;
        public Action OnDisconnect;
        public Action<IBBuffer> OnReceivePackage;
        public Action<uint, IBBuffer> OnReceiveRequest;
        /******************************************************************************/

        public bool disposed;
        IntPtr ptr;
        IntPtr handle;
        BBuffer bbRecv = new BBuffer();
        BBuffer bbSend = new BBuffer();

        /// <summary>
        /// 可初始化默认的 rpc 超时时长( 每秒帧数 * 秒数 )
        /// </summary>
        public NBSocket(int rpcDefaultInterval)
        {
            this.rpcDefaultInterval = rpcDefaultInterval;
            ptr = NBSocketInterop.xxnbs_new();
            handle = (IntPtr)GCHandle.Alloc(this);
        }

        /// <summary>
        /// 初始化网络系统( WSAStartup / signal ). 只需要一开始执行一次.
        /// </summary>
        public static void SockInit()
        {
            NBSocketInterop.xxnbs_init();
        }

        /// <summary>
        /// 设 ip & port
        /// </summary>
        public void SetAddress(string ip, ushort port)
        {
            if (disposed) throw new ObjectDisposedException("XxUvTcpClient");
            NBSocketInterop.xxnbs_set_address(ptr, ip, port);
        }

        /// <summary>
        /// 开始连接. 可传入阻塞时长( 通常都是不传的 )
        /// 返回负数 表示出错. 0 表示没发生错误 但也没连上. 1 表示连接成功.
        /// </summary>
        public int Connect(int sec = 0, int usec = 0)
        {
            if (disposed) throw new ObjectDisposedException("XxUvTcpClient");
            if (state != NBSocketStates.Disconnected) throw new InvalidOperationException();
            lastState = NBSocketStates.Connecting;
            return NBSocketInterop.xxnbs_connect(ptr, sec, usec);
        }

        /// <summary>
        /// 断开连接. 可传入延迟多少 ticks 断开. 0 立即断开.
        /// </summary>
        public void Disconnect(int delayTicks = 0)
        {
            if (disposed) throw new ObjectDisposedException("XxUvTcpClient");
            NBSocketInterop.xxnbs_disconnect(ptr, delayTicks);
        }

        /// <summary>
        /// 取当前状态. 
        /// </summary>
        public NBSocketStates state
        {
            get
            {
                if (disposed) throw new ObjectDisposedException("XxUvTcpClient");
                return (NBSocketStates)NBSocketInterop.xxnbs_get_state(ptr);
            }
        }
        NBSocketStates lastState;


        /// <summary>
        /// 判断组件是否未析构且已连接
        /// </summary>
        public bool alive { get { return (!disposed) && state == NBSocketStates.Connected; } }

        /// <summary>
        /// 取当前状态已持续的 ticks( Disconnecting 除外 )
        /// </summary>
        public int ticks
        {
            get
            {
                if (disposed) throw new ObjectDisposedException("XxUvTcpClient");
                return NBSocketInterop.xxnbs_get_ticks(ptr);
            }
        }


        /// <summary>
        /// 如果接收缓冲区有包, 将返回 buf 指针填充长度. 否则返回 null( 表示无数据可取 )
        /// 流程: Begin: buf = PopRecv(); if buf is null break; else ....... goto Begin;
        /// </summary>
        byte[] PopRecv()
        {
            int dataLen = 0;
            var bufPtr = NBSocketInterop.xxnbs_peek(ptr, ref dataLen);
            if (bufPtr == IntPtr.Zero) return null;
            var rtv = new byte[dataLen];
            Marshal.Copy(bufPtr, rtv, 0, dataLen);
            NBSocketInterop.xxnbs_pop(ptr);
            return rtv;
        }


        /// <summary>
        /// 帧驱动. 可传入阻塞时长( 仅对 Connecting 状态有效 )
        /// 返回负数表示出错. 0 表示没发生错误
        /// </summary>
        public int Update(int sec = 0, int usec = 0)
        {
            var r = NBSocketInterop.xxnbs_update(ptr, sec, usec);

            // 连接/断开 事件模拟
            var state = this.state;
            if (state != lastState)
            {
                if (lastState == NBSocketStates.Connecting && OnConnect != null)
                {
                    OnConnect(state == NBSocketStates.Connected);
                }
                else if (lastState == NBSocketStates.Connected && OnDisconnect != null)
                {
                    OnDisconnect();
                }
                lastState = state;
            }

            // 数据接收事件模拟
            if (r == 0)
            {
                while (true)
                {
                    var buf = PopRecv();
                    if (buf == null) break;
                    var pkgTypeId = buf[0];
                    bbRecv.Assign(buf, 3, buf.Length - 3);
                    uint serial = 0;
                    if (pkgTypeId == 1 || pkgTypeId == 2)
                    {
                        bbRecv.Read(ref serial);
                    }
                    var ibb = bbRecv.TryReadPackage<IBBuffer>();
                    if (ibb == null)
                    {
                        Disconnect();
                        return -1;
                    }
                    switch (pkgTypeId)
                    {
                        case 0:
                            if (OnReceivePackage != null) OnReceivePackage(ibb);
                            break;
                        case 1:
                            if (OnReceiveRequest != null) OnReceiveRequest(serial, ibb);
                            break;
                        case 2:
                            Callback(serial, ibb);
                            break;
                        default:
                            Disconnect();
                            return -1;
                    }
                }
            }

            Process();          // rpc 驱动
            return r;
        }

        /***********************************************************************************/
        // Send 相关
        /***********************************************************************************/

        public void SendBytes(byte[] data, int offset = 0, int len = 0)
        {
            if (disposed) throw new ObjectDisposedException("XxUvTcpBase");
            if (data == null || data.Length == 0) throw new NullReferenceException();
            if (offset + len > data.Length) throw new IndexOutOfRangeException();
            if (data.Length == offset) throw new NullReferenceException();
            if (len == 0) len = data.Length - offset;
            var h = GCHandle.Alloc(data, GCHandleType.Pinned);
            NBSocketInterop.xxnbs_send(ptr, h.AddrOfPinnedObject(), offset, len);
            h.Free();
        }

        public void SendBytes(BBuffer bb)
        {
            if (disposed) throw new ObjectDisposedException("XxUvTcpBase");
            if (bb.dataLen == 0) throw new NullReferenceException();
            var h = GCHandle.Alloc(bb.buf, GCHandleType.Pinned);
            NBSocketInterop.xxnbs_send(ptr, h.AddrOfPinnedObject(), 0, bb.dataLen);
            h.Free();
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
        public uint SendRequest(xx.IBBuffer pkg, Action<uint, IBBuffer> cb, int interval = 0)
        {
            if (disposed) throw new ObjectDisposedException("XxUvTcpBase");
            if (bbSend == null) bbSend = new BBuffer(65536);
            var serial = Register(cb, interval);
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



        /***********************************************************************************/
        // RPC 相关
        /***********************************************************************************/

        // 默认 rpc 超时间隔
        int rpcDefaultInterval;

        // 循环使用的自增流水号
        uint serial;

        // 流水号 与 cb(bb) 回调 的映射. bb 为空表示超时调用
        Dict<uint, Action<uint, IBBuffer>> mapping = new Dict<uint, Action<uint, IBBuffer>>();

        // 用一个队列记录流水号的超时时间, 以便删掉超时的. first: timeout ticks
        Queue<Pair<int, uint>> serials = new Queue<Pair<int, uint>>();


        // 不断将超时的从字典移除( 以 false 参数 call 之 ), 直到 Pop 到未超时的停止.
        void Process()
        {
            var ticks = this.ticks;
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
        uint Register(Action<uint, IBBuffer> cb, int interval = 0)
        {
            if (interval == 0) interval = rpcDefaultInterval;
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
        void Callback(uint serial, IBBuffer ibb)
        {
            int idx = mapping.Find(serial);
            if (idx == -1) return;              // 已超时移除
            var a = mapping.ValueAt(idx);
            mapping.RemoveAt(idx);
            if (a != null) a(serial, ibb);
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
                //if (disposing) // Free other state (managed objects).

                NBSocketInterop.xxnbs_delete(ptr);
                ptr = IntPtr.Zero;
                ((GCHandle)handle).Free();
                handle = IntPtr.Zero;
                disposed = true;
            }
            //base.Dispose(disposing);
        }

        ~NBSocket()
        {
            Dispose(false);
        }

        #endregion
    }


    /// <summary>
    /// 连接状态
    /// </summary>
    public enum NBSocketStates
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


    public static class NBSocketInterop
    {
#if !UNITY_EDITOR && UNITY_IPHONE
        const string DLL_NAME = "__Internal";
#else
        const string DLL_NAME = "xxnbsocketlib";
#endif

        [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void xxnbs_init();

        [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
        internal static extern IntPtr xxnbs_new();

        [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void xxnbs_delete(IntPtr nbs);

        [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void xxnbs_set_userdata(IntPtr nbs, IntPtr userdata);

        [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
        internal static extern IntPtr xxnbs_get_userdata(IntPtr nbs);

        [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void xxnbs_set_address(IntPtr nbs, string ip, ushort port);

        [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
        internal static extern int xxnbs_connect(IntPtr nbs, int sec, int usec);

        [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void xxnbs_disconnect(IntPtr nbs, int delayTicks);

        [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
        internal static extern int xxnbs_update(IntPtr nbs, int sec, int usec);

        [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
        internal static extern int xxnbs_get_state(IntPtr nbs);

        [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
        internal static extern int xxnbs_get_ticks(IntPtr nbs);

        [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
        internal static extern int xxnbs_send(IntPtr nbs, IntPtr buf, int offset, int dataLen);

        [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
        internal static extern IntPtr xxnbs_peek(IntPtr nbs, ref int dataLen);

        [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void xxnbs_pop(IntPtr nbs);
    }

}
