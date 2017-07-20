using System;

namespace PKG
{
    partial class UserInfo
    {
        /// <summary>
        /// 指向服务上下文
        /// </summary>
        public Service owner;

        /// <summary>
        /// 保存自己位于服务中的容器中的下标
        /// </summary>
        public int owner_users_index;

        /// <summary>
        /// 与连接双向 bind
        /// </summary>
        public Peer peer;

        /// <summary>
        /// 检查连接是否不空, 是否活着, 是否连通状态
        /// </summary>
        public bool PeerAlive
        {
            get
            {
                return peer != null && peer.Alive;
            }
        }

        // todo: 其他

        // 拿来除下 bug
        public xx.List<object> sentMsgs = new xx.List<object>();
    }
}
