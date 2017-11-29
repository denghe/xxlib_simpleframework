using System;
using System.Net;
using System.Net.Sockets;
using System.Threading;

namespace xx
{

    /// <summary>
    /// socket 状态列表
    /// </summary>
    public enum NetStates
    {
        Disconnected,
        Connecting,
        Connected,
        Disconnecting
    }

    public class UnityClientPeer : IDisposable
    {
        readonly object syncer = new object();
        bool disposed = false;
        NetStates state = NetStates.Disconnected;
        Socket sock = null;
        IPAddress ipAddress;
        int port;
        xx.BBuffer bbReceive;
        public Action<byte[]> OnReceivePackage;

        /// <summary>
        /// 返回 socket 状态
        /// </summary>
        public NetStates State { get { return state; } }

        /// <summary>
        /// 设置 ip & port, 返回 0 表示成功
        /// </summary>
        public int SetAddress(IPAddress ipAddress, int port)
        {
            if (state != NetStates.Disconnected) return -1;
            this.ipAddress = ipAddress;
            this.port = port;
            return 0;
        }

        /// <summary>
        /// 连接经由 SetAddress 设置的目标地址( 返回 true 表示开始连接了 )
        /// </summary>
        public int Connect()
        {
            if (state != NetStates.Disconnected) return -1;
            state = NetStates.Connecting;
            new Thread(new ThreadStart(Process))
            {
                Name = "connect & read",
                IsBackground = true
            }.Start();
            return 0;
        }

        /// <summary>
        /// 连接并阻塞 read ( for 后台线程 )
        /// </summary>
        void Process()
        {
            sock = new Socket(ipAddress.AddressFamily, SocketType.Stream, ProtocolType.Tcp)
            {
                NoDelay = true
            };
            try
            {
                sock.Connect(ipAddress, port);
            }
            catch (Exception /*ex*/)
            {
                // todo: log
                return;
            }
            if (state != NetStates.Connecting) return;   // Connect 的过程中感觉超时了, Close 掉?
            state = NetStates.Connected;
            if (bbReceive == null) bbReceive = new xx.BBuffer(1024 * 512);

            while (state == NetStates.Connected)
            {
                try
                {
                    // 收数据, 追加到接收容器
                    var recvLen = sock.Receive(bbReceive.buf, bbReceive.dataLen, bbReceive.buf.Length - bbReceive.dataLen, SocketFlags.None);
                    if (recvLen == 0)
                    {
                        throw new SocketException(10054);
                    };
                    bbReceive.dataLen += recvLen;
                    bbReceive.offset = 0;

                    LabBegin:

                    // 如果包头没收齐, 将剩余数据移到最前面跳出
                    if (bbReceive.dataLen < bbReceive.offset + 2)
                    {
                        bbReceive.buf[0] = bbReceive.buf[bbReceive.offset];
                        bbReceive.dataLen = 1;
                        continue;
                    }

                    // 如果包头已收齐, 解析, 得到长度
                    var dataLen = bbReceive.buf[bbReceive.offset] + (bbReceive.buf[bbReceive.offset + 1] << 8);
                    bbReceive.offset += 2;

                    // 简单合法判断
                    if (dataLen == 0)
                        break; // todo: log

                    // 如果包数据区没收够, 将剩余数据移到最前面跳出
                    var left = bbReceive.dataLen - bbReceive.offset;
                    if (left < dataLen)
                    {
                        left += 2;
                        Array.Copy(bbReceive.buf, bbReceive.offset - 2, bbReceive.buf, 0, left);
                        bbReceive.dataLen = left;
                        continue;
                    }

                    // 如果包数据区长度足够, 来一发 OnReceivePackage
                    if (OnReceivePackage != null)
                    {
                        var pkgBuf = new byte[dataLen];         // 理论上讲这里可以不用复制, 直接抛 buf + offset + len, 但感觉不够安全. 客户端也不纠结这点性能
                        Array.Copy(bbReceive.buf, bbReceive.offset, pkgBuf, 0, dataLen);
                        OnReceivePackage(pkgBuf);
                    }

                    // 跳过已处理过的数据段并继续拆后面的包( 如果有的话 )
                    bbReceive.offset += dataLen;
                    if (bbReceive.dataLen > bbReceive.offset) goto LabBegin;
                    else
                    {
                        // 刚好没有数据了
                        bbReceive.dataLen = 0;
                    }
                }
                catch (Exception)
                {
                    break;
                }
            }
            this.Disconnect();
        }

        /// <summary>
        /// 直接发送一段数据
        /// </summary>
        public bool Send(byte[] data, int offset, int length)
        {
            try
            {
                if (!sock.Connected) return false;
                sock.Send(data, 0, length, SocketFlags.None);
                return true;
            }
            catch (Exception /*ex*/)
            {
                return false;
            }
        }

        /// <summary>
        /// 直接发送 bb 中的数据
        /// </summary>
        public bool Send(xx.BBuffer bb)
        {
            return Send(bb.buf, 0, bb.dataLen);
        }

        /// <summary>
        /// buf for 发包复用
        /// </summary>
        public xx.BBuffer bbSend = new xx.BBuffer();

        /// <summary>
        /// 发出多个包( 不合并, 每个类都打包成一个独立的包发出 ), 返回发出的总字节数( 含包头 )
        /// </summary>
        public int SendPackages(params xx.IBBuffer[] ibbs)
        {
            var sum = 0;
            var ibbsLength = ibbs.Length;
            for (int i = 0; i < ibbsLength; ++i)
            {
                var ibb = ibbs[i];
                bbSend.Clear();
                bbSend.BeginWritePackage();
                bbSend.WriteRoot(ibb);
                if (!bbSend.EndWritePackage()) return -1;
                sum += bbSend.dataLen;
                if (!Send(bbSend.buf, 0, bbSend.dataLen)) return -2;
            }
            return sum;
        }

        /// <summary>
        /// 批量发包( 合并成一个包发出, 要小心超出尺寸 ), 返回发出的总字节数( 含包头 )
        /// </summary>
        public int SendCombine(params xx.IBBuffer[] ibbs)
        {
            bbSend.Clear();
            bbSend.BeginWritePackage();
            var ibbsLength = ibbs.Length;
            for (int i = 0; i < ibbsLength; ++i)
            {
                var ibb = ibbs[i];
                bbSend.WriteRoot(ibb);
            }
            if (!bbSend.EndWritePackage()) return -1;
            if (!Send(bbSend.buf, 0, bbSend.dataLen)) return -2;
            return bbSend.dataLen;
        }

        /// <summary>
        /// 断开( immediately 当前未用到 )
        /// </summary>
        public void Disconnect(bool immediately = true)
        {
            state = NetStates.Disconnecting;
            lock (this.syncer)
            {
                if (sock != null)
                {
                    try
                    {
                        sock.Close();
                    }
                    catch (Exception) { }
                    sock = null;
                }
            }
            state = NetStates.Disconnected;
        }

        /// <summary>
        /// 实现接口
        /// </summary>
        public void Dispose()
        {
            if (disposed) return;
            state = NetStates.Disconnecting;
            if (sock != null)
            {
                try
                {
                    sock.Close();
                }
                catch (Exception) { }
                sock = null;
            }
            state = NetStates.Disconnected;
            disposed = true;
        }
    }


}

