using System;
using System.Collections.Generic;
using System.Diagnostics;

namespace xx
{
    // 以下代码 抄自 ms 官方 Dictionary, 精简加料
    // 增加了 TryGetValue, ValueAt, RemoveAt, ModifyKeyAt, ValidateKeyAt, ForEach (带 At 的都是指 index 方式访问 )
    // Add 会返回操作是否成功, 以及目标 index
    // Find 返回的是目标 index
    // 为亲和 Cache 而将 Node 一分为二, 存为了两个数组.

    /// <summary>
    /// Add 操作的结果
    /// </summary>
    public struct DictAddResult
    {
        public bool success;
        public int index;
    }

    public class Dict<K, V>
    {
        /// <summary>
        /// 用于存 Find 所需数据
        /// </summary>
        public struct Node
        {
            public uint hashCode;
            public int next;
        };

        /// <summary>
        /// 用于存本体数据
        /// </summary>
        public struct Data
        {
            public K key;
            public V value;
            public int prev;
        };

        /// <summary>
        /// 比较器( 提供 hash 和 equals 的功能需求. 如果是自定义 K, 则需要实现 IEqualityComparer<K> 接口 )
        /// </summary>
        protected IEqualityComparer<K> comparer = EqualityComparer<K>.Default;


        protected int[] buckets;              // 桶数组
        protected Node[] nodes;               // 节点数组
        protected Data[] items;               // 数据数组( 与节点数组同步下标 )

        protected int count;                  // 已使用空间数
        protected int freeList = -1;          // 自由空间链表头( next 指向下一个未使用单元 )
        protected int freeCount;              // 自由空间链长


        /// <summary>
        /// 预申请 默认 空间
        /// </summary>
        public Dict() : this(16) { }

        /// <summary>
        /// 按大于 capacity 的质数元素个数来预申请空间
        /// </summary>
        public Dict(int capacity)
        {
            var bucketsLen = HashHelpers.GetPrime(capacity);
            buckets = new int[bucketsLen];
            nodes = new Node[bucketsLen];
            items = new Data[bucketsLen];
            for (int i = 0; i < bucketsLen; i++)
            {
                buckets[i] = -1;  // -1 代表 "空"
            }
        }


        /// <summary>
        /// 可传入 是否覆盖旧值, 返回操作 是否成功, 以及存储编号
        /// </summary>
        public DictAddResult Add(K key, V value, bool isOverride = false)
        {
            // hash 按桶数取模 定位到具体 链表, 扫找
            var hashCode = (uint)comparer.GetHashCode(key);
            var targetBucket = hashCode % (uint)buckets.Length;
            for (int i = buckets[targetBucket]; i >= 0; i = nodes[i].next)
            {
                if (nodes[i].hashCode == hashCode && comparer.Equals(items[i].key, key))
                {
                    if (isOverride)                    // 允许覆盖 value
                    {
                        items[i].value = value;
                        return new DictAddResult { success = true, index = i };
                    }
                    return new DictAddResult { success = false, index = i };
                }
            }
            // 没找到则新增
            int index;
            if (freeCount > 0)                         // 如果 自由节点链表 不空, 取一个来当容器
            {                                           // 这些节点来自 Remove 操作. next 指向下一个
                index = freeList;
                freeList = nodes[index].next;
                freeCount--;
            }
            else
            {
                if (count == buckets.Length)               // 所有空节点都用光了, Resize
                {
                    Resize();
                    targetBucket = hashCode % (uint)buckets.Length;
                }
                index = count;                          // 指向 Resize 后面的空间起点
                count++;
            }

            // 执行到此处时, freeList 要么来自 自由节点链表, 要么为 Reserve 后新增的空间起点.
            nodes[index].hashCode = hashCode;
            nodes[index].next = buckets[targetBucket];

            // 如果当前 bucket 中有 index, 则令其 prev 等于即将添加的 index
            if (buckets[targetBucket] >= 0)
            {
                items[buckets[targetBucket]].prev = index;
            }
            buckets[targetBucket] = index;

            // 写 key, value
            items[index].key = key;
            items[index].value = value;
            items[index].prev = -1;

            return new DictAddResult { success = true, index = index };
        }

        /// <summary>
        /// 根据 key 来查找元素, 找到则返回 存储编号, 未找到返回 -1
        /// </summary>
        public int Find(K key)
        {
            var hashCode = (uint)comparer.GetHashCode(key);
            for (int i = buckets[hashCode % (uint)buckets.Length]; i >= 0; i = nodes[i].next)
            {
                if (nodes[i].hashCode == hashCode && comparer.Equals(items[i].key, key))
                {
                    return i;
                }
            }
            return -1;
        }

        /// <summary>
        /// 根据 key 来 移除元素
        /// </summary>
        public void Remove(K key)
        {
            var idx = Find(key);
            if (idx != -1)
            {
                RemoveAt(idx);
            }
        }

        /// <summary>
        /// 清空所有元素并释放引用
        /// </summary>
        public void Clear()
        {
            if (count == 0)
                return;
            for (int i = 0; i < count; ++i)
            {
                if (items[i].prev != -2)
                {
                    items[i].key = default(K);
                    items[i].value = default(V);
                }
            }
            for (int i = 0; i < buckets.Length; i++)
            {
                buckets[i] = -1;
            }
            freeList = -1;
            freeCount = 0;
            count = 0;
        }

        /// <summary>
        /// 返回当前元素个数
        /// </summary>
        public int Count
        {
            get
            {
                return count - freeCount;
            }
        }

        /// <summary>
        /// get: 如果未找到, 则返回 default(V). 
        /// set: 覆盖 Add
        /// </summary>
        public V this[K key]
        {
            get
            {
                var idx = Find(key);
                if (idx >= 0)
                {
                    return ValueAt(idx);
                }
                else
                {
                    return default(V);
                }
            }
            set
            {
                Add(key, value, true);
            }
        }

        /// <summary>
        /// 根据 存储编号 来快速移除元素
        /// </summary>
        public void RemoveAt(int idx)
        {
            Debug.Assert(idx >= 0 && idx < count && items[idx].prev != -2);
            if (items[idx].prev < 0)
            {
                buckets[nodes[idx].hashCode % (uint)buckets.Length] = nodes[idx].next;
            }
            else
            {
                nodes[items[idx].prev].next = nodes[idx].next;
            }
            if (nodes[idx].next >= 0)        // 如果存在当前节点的下一个节点, 令其 prev 指向 上一个节点
            {
                items[nodes[idx].next].prev = items[idx].prev;
            }

            nodes[idx].next = freeList;       // 当前节点已被移出链表, 令其 next 指向  自由节点链表头( next 有两种功用 )
            freeList = idx;
            freeCount++;

            items[idx].key = default(K);    // 析构
            items[idx].value = default(V);
            items[idx].prev = -2;             // foreach 时的无效标志
        }

        /// <summary>
        /// 是否没有数据
        /// </summary>
        public bool IsEmpty
        {
            get
            {
                return Count == 0;
            }
        }

        /// <summary>
        /// 试取 Value, key 不存在则返回 false
        /// </summary>
        public bool TryGetValue(K key, out V value)
        {
            int idx = Find(key);
            if (idx >= 0)
            {
                value = items[idx].value;
                return true;
            }
            value = default(V);
            return false;
        }

        /// <summary>
        /// 根据 存储编号 来返回值. 
        /// </summary>
        public V ValueAt(int idx)
        {
            Debug.Assert(idx >= 0 && idx < count && items[idx].prev != -2);
            return items[idx].value;
        }

        /// <summary>
        /// 同 ValueAt
        /// </summary>
        public V At(int idx)
        {
            Debug.Assert(idx >= 0 && idx < count && items[idx].prev != -2);
            return items[idx].value;
        }

        /// <summary>
        /// 根据 存储编号 来定位到存储体. 
        /// </summary>
        public Data DataAt(int idx)
        {
            Debug.Assert(idx >= 0 && idx < count && items[idx].prev != -2);
            return items[idx];
        }

        /// <summary>
        /// 根据 存储编号 来返回键. 
        /// </summary>
        public K KeyAt(int idx)
        {
            Debug.Assert(idx >= 0 && idx < count);
            Debug.Assert(items[idx].prev != -2);
            return items[idx].key;
        }

        /// <summary>
        /// 为 Debug.Assert 服务, 用以校验 存储编号 下的 key 是否正确
        /// </summary>
        public bool ValidateKeyAt(int idx, K key)
        {
            Debug.Assert(idx >= 0 && idx < count);
            if (items[idx].prev == -2)
            {
                return false;
            }
            return comparer.Equals(items[idx].key, key);
        }

        /// <summary>
        /// 将 存储编号 下的 key 替换为另外一个值, 返回是否替换成功( 新 key 已存在将返回 false )
        /// </summary>
        public bool UpdateAt(int idx, K newKey)
        {
            Debug.Assert(idx >= 0 && idx < count && items[idx].prev != -2);
            var node = nodes[idx];
            var item = items[idx];

            // 如果 hash 相等或位于相同 bucket, 可以直接改 key 并退出函数
            var newHashCode = (uint)comparer.GetHashCode(newKey);
            if (node.hashCode == newHashCode)
            {
                item.key = newKey;
                items[idx] = item;  // for struct
                return true;
            }
            var targetBucket = node.hashCode % (uint)buckets.Length;
            var newTargetBucket = newHashCode % (uint)buckets.Length;
            if (targetBucket == newTargetBucket)
            {
                node.hashCode = newHashCode;
                nodes[idx] = node;  // for struct

                item.key = newKey;
                items[idx] = item;  // for struct
                return true;
            }

            // 检查是否冲突
            for (int i = buckets[newTargetBucket]; i >= 0; i = nodes[i].next)
            {
                if (nodes[i].hashCode == newHashCode && comparer.Equals(items[i].key, newKey))
                {
                    return false;
                }
            }

            // 简化的 RemoveAt
            if (item.prev < 0)
            {
                buckets[targetBucket] = node.next;
            }
            else
            {
                nodes[item.prev].next = node.next;
            }
            if (node.next >= 0)
            {
                items[node.next].prev = item.prev;
            }

            // 简化的 Add 后半段
            node.hashCode = newHashCode;
            node.next = buckets[newTargetBucket];
            nodes[idx] = node;  // for struct

            if (buckets[newTargetBucket] >= 0)
            {
                items[buckets[newTargetBucket]].prev = idx;
            }
            buckets[newTargetBucket] = idx;

            item.key = newKey;
            item.prev = -1;
            items[idx] = item;  // for struct

            return true;
        }

        /// <summary>
        /// 将一个旧 key 替换为新 key. 如果旧 key 未找到或新 key 已存在将返回 false( 比 idx 版慢但安全方便 )
        /// </summary>
        public bool Update(K oldKey, K newKey)
        {
            int idx = Find(oldKey);
            if (idx == -1) return false;
            return UpdateAt(idx, newKey);
        }


        public delegate bool KVIterEx(Data kv);
        public delegate void KVIter(Data kv);
        public delegate bool IdxIterEx(int idx);
        public delegate void IdxIter(int idx);

        /// <summary>
        /// 提供一个遍历函数, handler 如果返回 false 将终止遍历
        /// </summary>
        public void ForEach(KVIterEx handler)
        {
            for (int i = 0; i < count; ++i)
            {
                if (items[i].prev != -2)
                {
                    if (!handler(items[i]))
                    {
                        break;
                    }
                }
            }
        }

        /// <summary>
        /// 提供一个遍历所有元素的函数, 无法中途终止
        /// </summary>
        public void ForEach(KVIter handler)
        {
            for (int i = 0; i < count; ++i)
            {
                if (items[i].prev != -2)
                {
                    handler(items[i]);
                }
            }
        }


        /// <summary>
        /// 提供一个遍历函数, handler 如果返回 false 将终止遍历
        /// </summary>
        public void ForEachIndex(IdxIterEx handler)
        {
            for (int i = 0; i < count; ++i)
            {
                if (items[i].prev != -2)
                {
                    if (!handler(i))
                    {
                        break;
                    }
                }
            }
        }

        /// <summary>
        /// 提供一个遍历所有元素的函数, 无法中途终止
        /// </summary>
        public void ForEachIndex(IdxIter handler)
        {
            for (int i = 0; i < count; ++i)
            {
                if (items[i].prev != -2)
                {
                    handler(i);
                }
            }
        }


        /// <summary>
        /// 将所有 values 填充到 List
        /// </summary>
        public void GetValues(ref List<V> outVals)
        {
            outVals.Clear();
            for (int i = 0; i < count; ++i)
            {
                if (items[i].prev != -2)
                {
                    outVals.Add(items[i].value);
                }
            }
        }

        /// <summary>
        /// 将所有 values 填充到 List
        /// </summary>
        public void GetValues(ref System.Collections.Generic.List<V> outVals)
        {
            outVals.Clear();
            for (int i = 0; i < count; ++i)
            {
                if (items[i].prev != -2)
                {
                    outVals.Add(items[i].value);
                }
            }
        }

        /// <summary>
        /// 将所有 values 填充到 xx.List 并返回
        /// </summary>
        public List<V> GetValues()
        {
            var rtv = new List<V>();
            GetValues(ref rtv);
            return rtv;
        }



        protected void Resize()
        {
            Debug.Assert(count == buckets.Length);
            var bucketsLen = HashHelpers.ExpandPrime(buckets.Length);

            // 桶扩容并全部初始化( 后面会重新映射一次 )
            buckets = new int[bucketsLen];
            for (int i = 0; i < bucketsLen; i++)
            {
                buckets[i] = -1;
            }

            // 节点数组扩容( 保留老数据 )
            Array.Resize<Node>(ref nodes, bucketsLen);
            Array.Resize<Data>(ref items, bucketsLen);

            // 遍历所有节点, 重构桶及链表( 扩容情况下没有节点空洞 )
            for (int i = 0; i < count; i++)
            {
                var targetBucket = nodes[i].hashCode % (uint)bucketsLen;
                if (buckets[targetBucket] >= 0)
                {
                    items[buckets[targetBucket]].prev = i;
                }
                items[i].prev = -1;
                nodes[i].next = buckets[targetBucket];
                buckets[targetBucket] = i;
            }
        }
    }





    // 以下代码 几乎完抄自 ms 官方( 官方代码为 internal 没办法直接 call, 所以... )
    public static class HashHelpers
    {
        public static readonly int[] primes = {
            3, 7, 11, 17, 23, 29, 37, 47, 59, 71, 89, 107, 131, 163, 197, 239, 293, 353, 431, 521, 631, 761, 919,
            1103, 1327, 1597, 1931, 2333, 2801, 3371, 4049, 4861, 5839, 7013, 8419, 10103, 12143, 14591,
            17519, 21023, 25229, 30293, 36353, 43627, 52361, 62851, 75431, 90523, 108631, 130363, 156437,
            187751, 225307, 270371, 324449, 389357, 467237, 560689, 672827, 807403, 968897, 1162687, 1395263,
            1674319, 2009191, 2411033, 2893249, 3471899, 4166287, 4999559, 5999471, 7199369};

        public static bool IsPrime(int candidate)
        {
            if ((candidate & 1) != 0)
            {
                int limit = (int)Math.Sqrt(candidate);
                for (int divisor = 3; divisor <= limit; divisor += 2)
                {
                    if ((candidate % divisor) == 0)
                        return false;
                }
                return true;
            }
            return (candidate == 2);
        }

        public static int GetPrime(int min)
        {
            Debug.Assert(min >= 0, "Arg_HTCapacityOverflow");

            for (int i = 0; i < primes.Length; i++)
            {
                int prime = primes[i];
                if (prime >= min)
                    return prime;
            }

            //outside of our predefined table. 
            //compute the hard way. 
            for (int i = (min | 1); i < Int32.MaxValue; i += 2)
            {
                if (IsPrime(i) && ((i - 1) % 101 != 0))  // Hashtable.HashPrime = 101
                    return i;
            }
            return min;
        }

        public static int GetMinPrime()
        {
            return primes[0];
        }

        // This is the maximum prime smaller than Array.MaxArrayLength
        public const int MaxPrimeArrayLength = 0x7FEFFFFD;

        // Returns size of hashtable to grow to.
        public static int ExpandPrime(int oldSize)
        {
            int newSize = 2 * oldSize;

            // Allow the hashtables to grow to maximum possible size (~2G elements) before encoutering capacity overflow.
            // Note that this check works even when _items.Length overflowed thanks to the (uint) cast
            if ((uint)newSize > MaxPrimeArrayLength && MaxPrimeArrayLength > oldSize)
            {
                Debug.Assert(MaxPrimeArrayLength == GetPrime(MaxPrimeArrayLength), "Invalid MaxPrimeArrayLength");
                return MaxPrimeArrayLength;
            }

            return GetPrime(newSize);
        }
    }
}
