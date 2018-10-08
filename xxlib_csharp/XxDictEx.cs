using System;
using System.Collections.Generic;
using System.Diagnostics;

namespace xx
{
    /// <summary>
    /// 多 key 字典之基类. 放点和 key 数量无关, key1 有关的东西 免得代码重复量太大
    /// </summary>
    public abstract class DictEx<K1, V>
    {
        protected Dict<K1, V> d1;
        protected Func<V, K1> getKey1;

        public bool Exists1(K1 key1)
        {
            return d1.Find(key1) != -1;
        }

        public bool TryGetValue1(K1 key1, out V value)
        {
            return d1.TryGetValue(key1, out value);
        }

        /// 数据总数
        /// </summary>
        public int Count
        {
            get { return d1.Count; }
        }

        /// <summary>
        /// 遍历. 如果 a 返回 false 将终止遍历
        /// </summary>
        public void ForEach(Func<V, bool> a)
        {
            d1.ForEach(new Dict<K1, V>.KVIterEx(d =>
            {
                return a(d.value);
            }));
        }

        /// <summary>
        /// 遍历
        /// </summary>
        public void ForEach(Action<V> a)
        {
            d1.ForEach(new Dict<K1, V>.KVIter(d =>
            {
                a(d.value);
            }));
        }
    }

    /// <summary>
    /// 多 key 字典之 2 个 key
    /// </summary>
    public partial class DictEx<K1, K2, V> : DictEx<K1, V>
    {
        // d1 在基类中
        protected Dict<K2, V> d2;

        protected Func<V, K2> getKey2;

        public DictEx(Func<V, K1> getKey1
            , Func<V, K2> getKey2
            , int capacity = 16)
        {
            d1 = new Dict<K1, V>(capacity);
            d2 = new Dict<K2, V>(capacity);

            this.getKey1 = getKey1;
            this.getKey2 = getKey2;
        }


        public bool Exists2(K2 key2)
        {
            return d2.Find(key2) != -1;
        }


        public bool TryGetValue2(K2 key2, out V value)
        {
            return d2.TryGetValue(key2, out value);
        }


        public bool Add(V value)
        {
            var r1 = d1.Add(getKey1(value), value);
            if (!r1.success)
            {
                return false;
            }

            var r2 = d2.Add(getKey2(value), value);
            if (!r2.success)
            {
                d1.RemoveAt(r1.index);
                return false;
            }

            return true;
        }


        public bool Remove1(K1 key1)
        {
            var idx = d1.Find(key1);
            if (idx == -1) return false;
            var value = d1.ValueAt(idx);
            d1.RemoveAt(idx);
            d2.Remove(getKey2(value));
            return true;
        }
        public bool Remove2(K2 key2)
        {
            var idx = d2.Find(key2);
            if (idx == -1) return false;
            var value = d2.ValueAt(idx);
            d1.Remove(getKey1(value));
            d2.RemoveAt(idx);
            return true;
        }

        public void Clear()
        {
            d1.Clear();
            d2.Clear();
        }
    }

    /// <summary>
    /// 多 key 字典之 3 个 key
    /// </summary>
    public partial class DictEx<K1, K2, K3, V> : DictEx<K1, V>
    {
        // d1 在基类中
        protected Dict<K2, V> d2;
        protected Dict<K3, V> d3;

        protected Func<V, K2> getKey2;
        protected Func<V, K3> getKey3;

        public DictEx(Func<V, K1> getKey1
            , Func<V, K2> getKey2
            , Func<V, K3> getKey3
            , int capacity = 16)
        {
            d1 = new Dict<K1, V>(capacity);
            d2 = new Dict<K2, V>(capacity);
            d3 = new Dict<K3, V>(capacity);

            this.getKey1 = getKey1;
            this.getKey2 = getKey2;
            this.getKey3 = getKey3;
        }


        public bool Exists2(K2 key2)
        {
            return d2.Find(key2) != -1;
        }
        public bool Exists3(K3 key3)
        {
            return d3.Find(key3) != -1;
        }


        public bool TryGetValue2(K2 key2, out V value)
        {
            return d2.TryGetValue(key2, out value);
        }
        public bool TryGetValue3(K3 key3, out V value)
        {
            return d3.TryGetValue(key3, out value);
        }


        public bool Add(V value)
        {
            var r1 = d1.Add(getKey1(value), value);
            if (!r1.success)
            {
                return false;
            }

            var r2 = d2.Add(getKey2(value), value);
            if (!r2.success)
            {
                d1.RemoveAt(r1.index);
                return false;
            }

            var r3 = d3.Add(getKey3(value), value);
            if (!r3.success)
            {
                d1.RemoveAt(r1.index);
                d2.RemoveAt(r2.index);
                return false;
            }

            return true;
        }


        public bool Remove1(K1 key1)
        {
            var idx = d1.Find(key1);
            if (idx == -1) return false;
            var value = d1.ValueAt(idx);
            d1.RemoveAt(idx);
            d2.Remove(getKey2(value));
            d3.Remove(getKey3(value));
            return true;
        }
        public bool Remove2(K2 key2)
        {
            var idx = d2.Find(key2);
            if (idx == -1) return false;
            var value = d2.ValueAt(idx);
            d1.Remove(getKey1(value));
            d2.RemoveAt(idx);
            d3.Remove(getKey3(value));
            return true;
        }
        public bool Remove3(K3 key3)
        {
            var idx = d3.Find(key3);
            if (idx == -1) return false;
            var value = d3.ValueAt(idx);
            d1.Remove(getKey1(value));
            d2.Remove(getKey2(value));
            d3.RemoveAt(idx);
            return true;
        }

        public void Clear()
        {
            d1.Clear();
            d2.Clear();
            d3.Clear();
        }
    }

    /// <summary>
    /// 多 key 字典之 4 个 key
    /// </summary>
    public partial class DictEx<K1, K2, K3, K4, V> : DictEx<K1, V>
    {
        // d1 在基类中
        protected Dict<K2, V> d2;
        protected Dict<K3, V> d3;
        protected Dict<K4, V> d4;

        protected Func<V, K2> getKey2;
        protected Func<V, K3> getKey3;
        protected Func<V, K4> getKey4;

        public DictEx(Func<V, K1> getKey1
            , Func<V, K2> getKey2
            , Func<V, K3> getKey3
            , Func<V, K4> getKey4
            , int capacity = 16)
        {
            d1 = new Dict<K1, V>(capacity);
            d2 = new Dict<K2, V>(capacity);
            d3 = new Dict<K3, V>(capacity);
            d4 = new Dict<K4, V>(capacity);

            this.getKey1 = getKey1;
            this.getKey2 = getKey2;
            this.getKey3 = getKey3;
            this.getKey4 = getKey4;
        }


        public bool Exists2(K2 key2)
        {
            return d2.Find(key2) != -1;
        }
        public bool Exists3(K3 key3)
        {
            return d3.Find(key3) != -1;
        }
        public bool Exists4(K4 key4)
        {
            return d4.Find(key4) != -1;
        }


        public bool TryGetValue2(K2 key2, out V value)
        {
            return d2.TryGetValue(key2, out value);
        }
        public bool TryGetValue3(K3 key3, out V value)
        {
            return d3.TryGetValue(key3, out value);
        }
        public bool TryGetValue4(K4 key4, out V value)
        {
            return d4.TryGetValue(key4, out value);
        }


        public bool Add(V value)
        {
            var r1 = d1.Add(getKey1(value), value);
            if (!r1.success)
            {
                return false;
            }

            var r2 = d2.Add(getKey2(value), value);
            if (!r2.success)
            {
                d1.RemoveAt(r1.index);
                return false;
            }

            var r3 = d3.Add(getKey3(value), value);
            if (!r3.success)
            {
                d1.RemoveAt(r1.index);
                d2.RemoveAt(r2.index);
                return false;
            }

            var r4 = d4.Add(getKey4(value), value);
            if (!r4.success)
            {
                d1.RemoveAt(r1.index);
                d2.RemoveAt(r2.index);
                d3.RemoveAt(r3.index);
                return false;
            }

            return true;
        }


        public bool Remove1(K1 key1)
        {
            var idx = d1.Find(key1);
            if (idx == -1) return false;
            var value = d1.ValueAt(idx);
            d1.RemoveAt(idx);
            d2.Remove(getKey2(value));
            d3.Remove(getKey3(value));
            d4.Remove(getKey4(value));
            return true;
        }
        public bool Remove2(K2 key2)
        {
            var idx = d2.Find(key2);
            if (idx == -1) return false;
            var value = d2.ValueAt(idx);
            d1.Remove(getKey1(value));
            d2.RemoveAt(idx);
            d3.Remove(getKey3(value));
            d4.Remove(getKey4(value));
            return true;
        }
        public bool Remove3(K3 key3)
        {
            var idx = d3.Find(key3);
            if (idx == -1) return false;
            var value = d3.ValueAt(idx);
            d1.Remove(getKey1(value));
            d2.Remove(getKey2(value));
            d3.RemoveAt(idx);
            d4.Remove(getKey4(value));
            return true;
        }
        public bool Remove4(K4 key4)
        {
            var idx = d4.Find(key4);
            if (idx == -1) return false;
            var value = d4.ValueAt(idx);
            d1.Remove(getKey1(value));
            d2.Remove(getKey2(value));
            d3.Remove(getKey3(value));
            d4.RemoveAt(idx);
            return true;
        }

        public void Clear()
        {
            d1.Clear();
            d2.Clear();
            d3.Clear();
            d4.Clear();
        }
    }

    /// <summary>
    /// 多 key 字典之 5 个 key
    /// </summary>
    public partial class DictEx<K1, K2, K3, K4, K5, V> : DictEx<K1, V>
    {
        // d1 在基类中
        protected Dict<K2, V> d2;
        protected Dict<K3, V> d3;
        protected Dict<K4, V> d4;
        protected Dict<K5, V> d5;

        protected Func<V, K2> getKey2;
        protected Func<V, K3> getKey3;
        protected Func<V, K4> getKey4;
        protected Func<V, K5> getKey5;

        public DictEx(Func<V, K1> getKey1
            , Func<V, K2> getKey2
            , Func<V, K3> getKey3
            , Func<V, K4> getKey4
            , Func<V, K5> getKey5
            , int capacity = 16)
        {
            d1 = new Dict<K1, V>(capacity);
            d2 = new Dict<K2, V>(capacity);
            d3 = new Dict<K3, V>(capacity);
            d4 = new Dict<K4, V>(capacity);
            d5 = new Dict<K5, V>(capacity);

            this.getKey1 = getKey1;
            this.getKey2 = getKey2;
            this.getKey3 = getKey3;
            this.getKey4 = getKey4;
            this.getKey5 = getKey5;
        }


        public bool Exists2(K2 key2)
        {
            return d2.Find(key2) != -1;
        }
        public bool Exists3(K3 key3)
        {
            return d3.Find(key3) != -1;
        }
        public bool Exists4(K4 key4)
        {
            return d4.Find(key4) != -1;
        }
        public bool Exists5(K5 key5)
        {
            return d5.Find(key5) != -1;
        }


        public bool TryGetValue2(K2 key2, out V value)
        {
            return d2.TryGetValue(key2, out value);
        }
        public bool TryGetValue3(K3 key3, out V value)
        {
            return d3.TryGetValue(key3, out value);
        }
        public bool TryGetValue4(K4 key4, out V value)
        {
            return d4.TryGetValue(key4, out value);
        }
        public bool TryGetValue5(K5 key5, out V value)
        {
            return d5.TryGetValue(key5, out value);
        }


        public bool Add(V value)
        {
            var r1 = d1.Add(getKey1(value), value);
            if (!r1.success)
            {
                return false;
            }

            var r2 = d2.Add(getKey2(value), value);
            if (!r2.success)
            {
                d1.RemoveAt(r1.index);
                return false;
            }

            var r3 = d3.Add(getKey3(value), value);
            if (!r3.success)
            {
                d1.RemoveAt(r1.index);
                d2.RemoveAt(r2.index);
                return false;
            }

            var r4 = d4.Add(getKey4(value), value);
            if (!r4.success)
            {
                d1.RemoveAt(r1.index);
                d2.RemoveAt(r2.index);
                d3.RemoveAt(r3.index);
                return false;
            }
            var r5 = d5.Add(getKey5(value), value);
            if (!r5.success)
            {
                d1.RemoveAt(r1.index);
                d2.RemoveAt(r2.index);
                d3.RemoveAt(r3.index);
                d4.RemoveAt(r4.index);
                return false;
            }

            return true;
        }


        public bool Remove1(K1 key1)
        {
            var idx = d1.Find(key1);
            if (idx == -1) return false;
            var value = d1.ValueAt(idx);
            d1.RemoveAt(idx);
            d2.Remove(getKey2(value));
            d3.Remove(getKey3(value));
            d4.Remove(getKey4(value));
            d5.Remove(getKey5(value));
            return true;
        }
        public bool Remove2(K2 key2)
        {
            var idx = d2.Find(key2);
            if (idx == -1) return false;
            var value = d2.ValueAt(idx);
            d1.Remove(getKey1(value));
            d2.RemoveAt(idx);
            d3.Remove(getKey3(value));
            d4.Remove(getKey4(value));
            d5.Remove(getKey5(value));
            return true;
        }
        public bool Remove3(K3 key3)
        {
            var idx = d3.Find(key3);
            if (idx == -1) return false;
            var value = d3.ValueAt(idx);
            d1.Remove(getKey1(value));
            d2.Remove(getKey2(value));
            d3.RemoveAt(idx);
            d4.Remove(getKey4(value));
            d5.Remove(getKey5(value));
            return true;
        }
        public bool Remove4(K4 key4)
        {
            var idx = d4.Find(key4);
            if (idx == -1) return false;
            var value = d4.ValueAt(idx);
            d1.Remove(getKey1(value));
            d2.Remove(getKey2(value));
            d3.Remove(getKey3(value));
            d4.RemoveAt(idx);
            d5.Remove(getKey5(value));
            return true;
        }
        public bool Remove5(K5 key5)
        {
            var idx = d5.Find(key5);
            if (idx == -1) return false;
            var value = d5.ValueAt(idx);
            d1.Remove(getKey1(value));
            d2.Remove(getKey2(value));
            d3.Remove(getKey3(value));
            d4.Remove(getKey4(value));
            d5.RemoveAt(idx);
            return true;
        }


        public void Clear()
        {
            d1.Clear();
            d2.Clear();
            d3.Clear();
            d4.Clear();
            d5.Clear();
        }
    }
}
