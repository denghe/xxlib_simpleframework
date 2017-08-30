using System;
using System.Collections.Concurrent;
using System.Diagnostics;
using System.Threading;
using System.Threading.Tasks;

/// <summary>
/// 客户端接入时会创建一份
/// </summary>
public class Peer : xx.UVServerPeerWrapperEx
{
    Service owner;
    public Peer(Service owner)
    {
        this.owner = owner;
    }

    /// <summary>
    /// 与用户上下文双向 bind 的变量
    /// </summary>
    PKG.UserInfo user = null;


    string addr;
    public override void OnConnect()
    {
        addr = PeerName;
        Console.WriteLine(addr + " connected!");
    }

    /// <summary>
    /// 断线时解除双向绑定并回收内存
    /// </summary>
    public override void OnDisconnect()
    {
        if (user != null)
        {
            // 解除双向 bind
            Debug.Assert(user.peer == this);
            user.peer = null;
            user = null;
        }
        Console.WriteLine(addr + " disconnected!");

        Dispose();      // 回收托管内存
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

        // for easy use
        var users = owner.users;

        for (int i = 0; i < recvPkgs.dataLen; ++i)
        {
            var recvPkg = recvPkgs[i];
            if (user == null)
            {
                // 首包应该收到 Join, 然后根据这个来 创建 & bind 或 rebind 到某个 UserInfo 上下文
                switch (recvPkg)
                {
                    case PKG.Client_Server.Join o:
                        {
                            Console.WriteLine("listener: receive PKG.Client_Server.Join, un & pw = " + o.username + " " + o.password);

                            // 向后台任务线程压入 数据库查询
                            new Task(() =>
                            {
                                string err = "no mssql";  // null
                                DB.Game.Account acc = null;
                                //Console.WriteLine("handler: begin exec SQL: Account_GetByUsername");
                                //try
                                //{
                                //    MsSql.Exec(funcs =>
                                //    {
                                //        acc = funcs.Account_GetByUsername(o.username);
                                //    });
                                //}
                                //catch (Exception ex)
                                //{
                                //    err = ex.Message;
                                //}
                                //Console.WriteLine("handler: end exec SQL: Account_GetByUsername");
                                //if (err != null)
                                //{
                                //    Console.WriteLine("handler: exec SQL err = " + err);
                                //}

#if true
                                /***********************************************************/
                                // 为方便运行, 在这里模拟一下 SQL 执行成功                 //
                                /***********************************************************/
                                if (err != null)
                                {
                                    err = null;
                                    switch (o.username)
                                    {
                                        case "a":
                                            acc = new DB.Game.Account { id = 1, password = "a" };
                                            break;
                                        case "b":
                                            acc = new DB.Game.Account { id = 2, password = "b" };
                                            break;
                                    }
                                }
                                /***********************************************************/
                                /***********************************************************/
#endif

                                // 向 uv 主循环线程压入 根据数据库查询返回结果
                                owner.PushResult(() =>
                                    {
                                        Console.WriteLine("handler: begin exec send result for: PKG.Client_Server.Join");

                                        // 如果连接已失效, 直接退出
                                        if (!Alive)
                                        {
                                            Console.WriteLine("handler: end exec send result for: PKG.Client_Server.Join");
                                            return;
                                        }

                                        // 如果 SQL 执行成功
                                        if (err == null)
                                        {
                                            // 如果 acc 已得到( username 对得上 )
                                            if (acc != null)
                                            {
                                                // 如果密码一致: 顶掉 或创建 user
                                                if (acc.password == o.password)
                                                {
                                                    var idx = users.Find(acc.id);
                                                    // 如果没找到: 创建并广播
                                                    if (idx == -1)
                                                    {
                                                        // 在创建 user info 之前, 先构造广播人员名单和数据, 自然排除掉当前 user
                                                        // 如果要进一步纠结, 可以调用 Listener 上的 Send( peers, pkg ) 函数, 引用计数, 0 复制( 还没做 ))
                                                        var pushMsg = GetPackagesData(new PKG.Server_Client.PushJoin
                                                        {
                                                            id = acc.id
                                                        });

                                                        // 广播给其他人
                                                        users.ForEach((kv) =>
                                                        {
                                                            var u = kv.value;
                                                            if (u.PeerAlive)
                                                            {
                                                                u.peer.Send(pushMsg, 0, pushMsg.Length);
                                                            }
                                                        });

                                                        // 创建 user info 
                                                        user = new PKG.UserInfo
                                                        {
                                                            id = acc.id,
                                                            owner = owner,
                                                            peer = this,
                                                            props = null
                                                        };
                                                        // 放进字典, 将所在字典 index 存起来
                                                        user.owner_users_index = users.Add(acc.id, user).index;


                                                        // 向当前客户端发送 join 成功, 并附带自己和所有人的信息
                                                        SendPackages(new PKG.Server_Client.JoinSuccess
                                                        {
                                                            requestSerial = o.serial,
                                                            self = user,
                                                            users = users.GetValues()
                                                        });

                                                    }
                                                    else
                                                    {
                                                        // 如果有找到: 顶下线
                                                        user = users.ValueAt(idx);
                                                        user.peer.user = null;              // 解绑
                                                        user.peer.Disconnect(true);         // 断开
                                                        user.peer = this;                   // 重新绑定

                                                        // 向当前客户端发送 join 成功
                                                        SendPackages(new PKG.Server_Client.JoinSuccess
                                                        {
                                                            requestSerial = o.serial,
                                                            self = user,
                                                            users = users.GetValues()
                                                        });
                                                    }
                                                }
                                            }
                                            else
                                            {
                                                // acc 未找到: 用户名对不上
                                                SendPackages(new PKG.Server_Client.JoinFail
                                                {
                                                    requestSerial = o.serial,
                                                    reason = "bad username"
                                                });
                                            }
                                        }
                                        else
                                        {
                                            // SQL 执行异常
                                            SendPackages(new PKG.Server_Client.JoinFail
                                            {
                                                requestSerial = o.serial,
                                                reason = err
                                            });
                                        }
                                        Console.WriteLine("handler: end exec send result for: PKG.Client_Server.Join");
                                    });

                            }).Start();

                            break;
                        }

                    default:
                        Disconnect(true);
                        return;
                }
            }
            else
            {
                // 在已经 bind 的情况下, 只应该收到下面的消息
                switch (recvPkg)
                {
                    case PKG.Client_Server.Message o:
                        {
                            Console.WriteLine("listener: receive user id = " + user.id + " PKG.Client_Server.Message, text = " + o.text);

                            // 先准备广播数据
                            var buf = GetPackagesData(new PKG.Server_Client.PushMessage
                            {
                                id = user.id,
                                text = o.text
                            });

                            // 如果构造成功, 广播
                            if (buf != null)
                            {
                                users.ForEach((kv) =>
                                {
                                    if (kv.value.PeerAlive) kv.value.peer.Send(buf, 0, buf.Length);
                                });
                            }
                            break;
                        }

                    case PKG.Client_Server.Logout o:
                        {
                            Console.WriteLine("listener: receive user id = " + user.id + " PKG.Client_Server.Logout");

                            // 先准备广播数据
                            var buf = GetPackagesData(new PKG.Server_Client.PushLogout
                            {
                                id = user.id,
                            });

                            // 如果构造成功, 广播
                            if (buf != null)
                            {
                                users.ForEach((kv) =>
                                {
                                    if (kv.value.PeerAlive) kv.value.peer.Send(buf, 0, buf.Length);
                                });
                            }
                            break;
                        }

                    default:
                        Disconnect(true);
                        return;
                }
            }
        }
    }
}
