using System;
using System.Collections.Generic;

namespace DB
{
    public partial class Account
    {
        public Account_MultiKeyDict __owner;

        public int __index_id;
        public bool MK_SetId(int val)
        {
            if (id == val) return true;
            if (__owner.ModifyKeyAt(__index_id, val))
            {
                id = val;
                return true;
            }
            return false;
        }

        public int __index_username;
        public bool MK_SetUsername(string val)
        {
            if (username == val) return true;
            if (__owner.__dict_username.ModifyKeyAt(__index_username, val))
            {
                username = val;
                return true;
            }
            return false;
        }

    }

    public class Account_MultiKeyDict : xx.Dict<int, Account>
    {

        public int MK_FindById(int key)
        {
            return this.Find(key);
        }
        public Account MK_GetById(int key)
        {
            var idx = this.Find(key);
            if (idx != -1)
            {
                return this.ValueAt(idx);
            }
            return default(Account);
        }
        public List<Account> MK_GetByIds(params int[] keys)
        {
            if (keys == null) return null;
            var rtv = new List<Account>();
            var keysLength = keys.Length;
            for (int i = 0; i < keysLength; ++i)
            {
                var idx = this.Find(keys[i]);
                if (idx != -1)
                {
                    rtv.Add(this.ValueAt(idx));
                }
            }
            return rtv;
        }
        public bool MK_ExistsByIds(params int[] keys)
        {
            if (keys == null) return false;
            var keysLength = keys.Length;
            for (int i = 0; i < keysLength; ++i)
            {
                if (this.Find(keys[i]) != -1) return true;
            }
            return false;
        }

        public xx.Dict<string, int> __dict_username = new xx.Dict<string, int>();
        public int MK_FindByUsername(string key)
        {
            return __dict_username.Find(key);
        }
        public Account MK_GetByUsername(string key)
        {
            var idx = __dict_username.Find(key);
            if (idx != -1)
            {
                return this.ValueAt(idx);
            }
            return default(Account);
        }
        public List<Account> MK_GetByUsernames(params string[] keys)
        {
            if (keys == null) return null;
            var rtv = new List<Account>();
            var keysLength = keys.Length;
            for (int i = 0; i < keysLength; ++i)
            {
                var idx = __dict_username.Find(keys[i]);
                if (idx != -1)
                {
                    rtv.Add(this.ValueAt(idx));
                }
            }
            return rtv;
        }
        public bool MK_ExistsByUsernames(params string[] keys)
        {
            if (keys == null) return false;
            var keysLength = keys.Length;
            for (int i = 0; i < keysLength; ++i)
            {
                if (__dict_username.Find(keys[i]) != -1) return true;
            }
            return false;
        }


        List<Action> rollbacks = new List<Action>();
        public bool MK_Add(Account o)
        {
            var rtv = Add(o.id, o);
            if (!rtv.success) return false;
            o.__index_id = rtv.index;
            rollbacks.Add(() => { RemoveAt(o.__index_id); });


            rtv = __dict_username.Add(o.username, rtv.index);
            if (!rtv.success) goto LabRollback;
            o.__index_username = rtv.index;

            rollbacks.Clear();
            o.__owner = this;
            return true;

            LabRollback:
            foreach (var a in rollbacks) { a(); }
            rollbacks.Clear();
            return false;
        }

        public bool MK_AddRange(IEnumerable<Account> os)
        {
            foreach (var o in os)
            {
                if (!MK_Add(o)) return false;
            }
            return true;
        }

        public void MK_Remove(Account o)
        {
            System.Diagnostics.Debug.Assert(o.__owner != null);
            RemoveAt(o.__index_id);
            __dict_username.RemoveAt(o.__index_username);
        }

        public void MK_ForEach(Func<Account, bool> handler)
        {
            for (int i = 0; i < count; ++i)
            {
                if (items[i].prev != -2 && !handler(items[i].value)) break;
            }
        }
        public void MK_ForEach(Action<Account> handler)
        {
            for (int i = 0; i < count; ++i)
            {
                if (items[i].prev != -2)
                {
                    handler(items[i].value);
                }
            }
        }
    }

}