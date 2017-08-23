#pragma warning disable CS0164
using System;
using System.Collections;
using System.Diagnostics;

namespace manage
{
    public class ClientPeer : xx.UVClientPeerWrapperEx
    {
        NetLooper looper;
        public ClientPeer(NetLooper looper) { this.looper = looper; }
        public override void OnConnect() { }
        public override void OnDisconnect() { }
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

            // 调用注册的包处理函数
            for (int i = 0; i < recvPkgs.dataLen; ++i)
            {
                var o = recvPkgs[i];
                if (!(o is PKG.Response)) throw new Exception();

                var idx = looper.serialHandlers.Find(((PKG.Response)o).requestSerial);
                if (idx != -1)
                {
                    looper.serialHandlers.ValueAt(idx)?.Invoke(o);
                    looper.serialHandlers.RemoveAt(idx);
                }
            }
        }
    }

    public class NetLooper : xx.UVTimerWrapper
    {
        xx.UVWrapper uv;
        MainWindow mw;
        ClientPeer peer;
        IEnumerator iter;

        /// <summary>
        /// 流水号 -- 处理函数映射
        /// </summary>
        public xx.Dict<int, Action<xx.IBBuffer>> serialHandlers = new xx.Dict<int, Action<xx.IBBuffer>>();

        /// <summary>
        /// 流水号循环生成变量
        /// </summary>
        public byte serial = 0;

        /// <summary>
        /// 指令队列
        /// </summary>
        public xx.Queue<object> cmds = new xx.Queue<object>();

        /// <summary>
        /// 状态变化相关日志( 主用于显示 )
        /// </summary>
        public xx.Queue<string> stateLogs = new xx.Queue<string>();

        public NetLooper(xx.UVWrapper uv, MainWindow mw)
        {
            this.uv = uv;
            this.mw = mw;
            peer = uv.CreateClientPeer(new ClientPeer(this));
            iter = Update().GetEnumerator();
        }
        public override void OnCreated()
        {
            Start(0, 1);
        }
        public override void OnTicks()
        {
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

        int Send(PKG.Request msg, Action<xx.IBBuffer> handler)
        {
            if (!peer.Alive) throw new Exception();
            msg.serial = unchecked(++serial);
            peer.SendPackages(msg);
            if (handler != null)
            {
                serialHandlers[msg.serial] = handler;
            }
            else
            {
                serialHandlers.Remove(msg.serial);
            }
            return msg.serial;
        }

        protected void StateLog(string log)
        {
            log = "\n" + DateTime.Now.ToLongTimeString() + ": " + log;
            //Debug.Write(log);
            stateLogs.Enqueue(log);
        }

        // 主控核心代码
        public IEnumerable Update()
        {
            LabInit:
            StateLog("init");
            // 清各种队列
            serialHandlers.Clear();
            cmds.Clear();

            LabGotoServerIP:
            StateLog("GotoServerIP");
            // 切到 "服务器 IP 地址填写窗口"
            mw.GotoServerIP((ip, port) =>
            {
                Connect(ip, port);
            });

            // 等待 "服务器 IP 地址填写窗口" 提交操作
            LabWaitServerIPSubmit:
            StateLog("wait ServerIP submit");
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
                goto LabInit;
            }
            StateLog("connected.");

            LabGotoLogin:
            // 显示登录窗口
            xx.IBBuffer loginResult = null;
            mw.GotoLogin((u, p) =>
            {
                var pkg = new PKG.Manage_DB.Login { username = u, password = p };
                Send(pkg, o =>
                {
                    loginResult = o;
                });
            });

            LabWaitLoginSubmit:
            StateLog("connected. wait Send msgs, Disconnect cmds, recv Response pkgs");
            while (true)
            {
                yield return null;
                if (peer.State != xx.NetStates.Connected) goto LabDisconnectedByServer;

                // 如果有收到登录返回结果
                if (loginResult != null)
                {
                    switch (loginResult)
                    {
                        case PKG.DB_Manage.LoginSuccess o:
                            goto LabMain;

                        case PKG.DB_Manage.LoginFail o:
                            ((UCLogin)mw.Content).Tips = o.reason;
                            loginResult = null;
                            goto LabDisconnectedByServer;

                        default:
                            throw new Exception();
                    }
                }
            }

            LabMain:;
            //mw.GotoMain(/**/);


            LabDisconnectedByServer:
            StateLog("disconnected by server.");
            var lastMS = currMS;
            while (currMS - lastMS < 2000) yield return null;       // 停 2 秒再跳, 让用户看清提示
            goto LabInit;


            //yield break;
        }
    }

    public class UVLooper
    {
        public xx.UVWrapper uv = new xx.UVWrapper();
        public NetLooper looper;

        public UVLooper(MainWindow mw)
        {
            PKG.AllTypes.Register();
            looper = uv.CreateTimer(new NetLooper(uv, mw));
            instance = this;
        }

        public static UVLooper instance;
    }

    public static class Extensions
    {
        public static long MS(this DateTime dt)
        {
            return dt.Ticks / 10000;
        }
    }
}
