using System;
using System.Collections;
using System.Diagnostics;

namespace manage
{
    public class ClientPeer : xx.UVClientPeerWrapperEx
    {
        NetLooper looper;
        public xx.Queue<xx.IBBuffer> recvMsgs = new xx.Queue<xx.IBBuffer>();

        public ClientPeer(NetLooper looper)
        {
            this.looper = looper;
        }

        public override void OnConnect()
        {
        }

        public override void OnDisconnect()
        {
            recvMsgs.Clear();
        }

        public override void OnReceivePackage(byte[] data)
        {
            // 反序列化一到多个包出来
            bbRecv.Assign(data, data.Length);

            // 如果反序列化错误, 立即断开客户端
            if (bbRecv.ReadPackages(ref recvPkgs) <= 0)
            {
                Disconnect(true);
                return;
            }

            // 将包移到接收队列
            for (int i = 0; i < recvPkgs.dataLen; ++i)
            {
                recvMsgs.Push(recvPkgs[i]);
            }
        }
    }

    public class NetLooper : xx.UVTimerWrapper
    {
        xx.UVWrapper uv;
        ClientPeer peer;
        IEnumerator iter;

        /// <summary>
        /// 流水号 -- 处理函数映射
        /// </summary>
        public xx.Dict<long, Action<xx.IBBuffer>> serialHandlers = new xx.Dict<long, Action<xx.IBBuffer>>();

        /// <summary>
        /// 指令队列
        /// </summary>
        public xx.Queue<object> cmds = new xx.Queue<object>();

        /// <summary>
        /// 发送队列
        /// </summary>
        public xx.Queue<xx.IBBuffer> sendMsgs = new xx.Queue<xx.IBBuffer>();

        /// <summary>
        /// 状态变化相关日志( 主用于显示 )
        /// </summary>
        public xx.Queue<string> stateLogs = new xx.Queue<string>();

        public NetLooper(xx.UVWrapper uv)
        {
            this.uv = uv;
            peer = uv.CreateClientPeer(new ClientPeer(this));
            iter = Update().GetEnumerator();
        }
        public override void OnCreated()
        {
            Start(0, 1);
        }
        public override void OnTicks()
        {
            if (peer.Alive)
            {
                while (sendMsgs.TryDequeue(out var o))
                {
                    peer.SendPackages(o);
                }
            }

            if (!iter.MoveNext())
            {
                Dispose();
                uv.Stop();
            }
        }
        protected override void Dispose(bool A_0)
        {
            if (peer != null)
            {
                peer.Dispose();
                peer = null;
            }
        }

        public long currMS
        {
            get { return DateTime.Now.MS(); }
        }

        public void Connect(string ip, int port)
        {
            cmds.Enqueue(new Tuple<string, string, int>("Connect", ip, port));
        }

        public void Disconnect()
        {
            cmds.Enqueue("Disconnect");
        }

        public int serial = 0;
        void Send(PKG.Request msg, Action<xx.IBBuffer> handler = null)
        {
            msg.serial = ++serial;
            sendMsgs.Enqueue(msg);
            if (handler != null)
            {
                serialHandlers[msg.serial] = handler;
            }
        }
        void Send(xx.IBBuffer msg)
        {
            sendMsgs.Enqueue(msg);
        }
        protected void StateLog(string log)
        {
            log = "\n" + DateTime.Now.ToLongTimeString() + ": " + log;
            //Debug.Write(log);
            stateLogs.Enqueue(log);
        }

        public IEnumerable Update()
        {
            // 等指令
            LabWaitCommand:
            // 清各种队列
            serialHandlers.Clear();
            cmds.Clear();
            sendMsgs.Clear();

            StateLog("inited. wait Connect cmd");
            while (true)
            {
                yield return null;

                if (cmds.TryDequeue(out var o_))
                {
                    switch (o_)
                    {
                        case Tuple<string, string, int> o when (o.Item1 == "Connect"):
                            peer.SetAddress(o.Item2, o.Item3);
                            peer.Connect();
                            goto LabWaitConnect;

                        default:
                            throw new NotImplementedException();
                    }
                }
            }

            LabWaitConnect:
            StateLog("connecting...");
            while (peer.State == xx.NetStates.Connecting)
            {
                yield return null;
            }

            if (peer.State == xx.NetStates.Disconnecting || peer.State == xx.NetStates.Disconnected)
            {
                StateLog("connect failed.");
                goto LabWaitCommand;
            }

            StateLog("connected. wait Send msgs, Disconnect cmds, recv Response pkgs");
            while (peer.State == xx.NetStates.Connected)
            {
                yield return null;

                // 指令处理
                if (cmds.TryDequeue(out var o_))
                {
                    switch (o_)
                    {
                        case string o when (o == "Disconnect"):
                            peer.Disconnect(true);
                            StateLog("disconnected by client.");
                            goto LabWaitCommand;

                        default:
                            throw new NotImplementedException();
                    }
                }

                // 数据接收处理
                while (peer.recvMsgs.TryDequeue(out var o))
                {
                    if (!(o is PKG.Response)) throw new Exception();

                    var idx = serialHandlers.Find(((PKG.Response)o).requestSerial);
                    if (idx != -1)
                    {
                        serialHandlers.ValueAt(idx)?.Invoke(o);
                        serialHandlers.RemoveAt(idx);
                    }
                }

                // 数据发送处理
                while (sendMsgs.TryDequeue(out var o))
                {
                    peer.SendPackages(o);
                }
            }

            StateLog("disconnected by server.");
            goto LabWaitCommand;

            //yield break;
        }
    }

    public class UVLooper
    {
        public xx.UVWrapper uv = new xx.UVWrapper();
        public NetLooper looper;

        public UVLooper()
        {
            PKG.AllTypes.Register();
            looper = uv.CreateTimer(new NetLooper(uv));
            instance = this;
        }

        public static UVLooper instance;
    }

    public static class Extensions
    {
        public static long MS(this DateTime dt)
        {
            return dt.Ticks / 1000000;
        }
    }
}
