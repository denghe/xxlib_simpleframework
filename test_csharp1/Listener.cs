using System;

/// <summary>
/// 监听器 for 其他服务接入
/// </summary>
public class Listener : UVListenerWrapper
{
    Service owner;
    public Listener(Service owner)
    {
        this.owner = owner;
    }

    public override UVServerPeerWrapper OnCreatePeer() => CreateServerPeer(new Peer(owner));
}
