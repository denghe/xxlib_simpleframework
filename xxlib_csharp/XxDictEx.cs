using System;

namespace xx
{
    /// <summary>
    /// 多 key 字典之 2 个 key
    /// </summary>
    public partial class DictEx<V, K0, K1>
    {
        protected class Data
        {
            public V value;
        }

        protected Dict<K0, Data> d0;
        protected Dict<K1, Data> d1;

        public DictEx(int capacity = 16)
        {
            d0 = new Dict<K0, Data>(capacity);
            d1 = new Dict<K1, Data>(capacity);
        }

        public bool Exists0(K0 key0)
        {
            return d0.Find(key0) != -1;
        }
        public bool Exists1(K1 key1)
        {
            return d1.Find(key1) != -1;
        }

        public bool TryGetValue0(K0 key0, out V value)
        {
            int idx = d0.Find(key0);
            if (idx == -1)
            {
                value = default(V);
                return false;
            }
            value = d0.ValueAt(idx).value;
            return true;
        }
        public bool TryGetValue1(K1 key1, out V value)
        {
            int idx = d1.Find(key1);
            if (idx == -1)
            {
                value = default(V);
                return false;
            }
            value = d1.ValueAt(idx).value;
            return true;
        }

        public V GetValue0(K0 key0)
        {
            var d = d0[key0];
            return d == null ? default(V) : d.value;
        }
        public V GetValue1(K1 key1)
        {
            var d = d1[key1];
            return d == null ? default(V) : d.value;
        }

        public bool Add(V value, K0 key0, K1 key1)
        {
            var d = new Data { value = value };
            var r0 = d0.Add(key0, d);
            if (!r0.success)
            {
                return false;
            }

            var r1 = d1.Add(key1, d);
            if (!r1.success)
            {
                d0.RemoveAt(r0.index);
                return false;
            }

            return true;
        }

        public bool Remove0(K0 key0)
        {
            int idx = d0.Find(key0);
            if (idx == -1) return false;
            RemoveAt(idx);
            return true;
        }
        public bool Remove1(K1 key1)
        {
            int idx = d1.Find(key1);
            if (idx == -1) return false;
            RemoveAt(idx);
            return true;
        }

        public void Clear()
        {
            d0.Clear();
            d1.Clear();
        }

        public int Find0(K0 key0)
        {
            return d0.Find(key0);
        }
        public int Find1(K1 key1)
        {
            return d1.Find(key1);
        }

        public K0 KeyAt0(int idx)
        {
            return d0.KeyAt(idx);
        }
        public K1 KeyAt1(int idx)
        {
            return d1.KeyAt(idx);
        }

        public V ValueAt(int idx)
        {
            return d0.ValueAt(idx).value;
        }
        public void RemoveAt(int idx)
        {
            d0.RemoveAt(idx);
            d1.RemoveAt(idx);
        }

        public bool Update0(K0 oldKey0, K0 newKey0)
        {
            return d0.Update(oldKey0, newKey0);
        }
        public bool Update1(K1 oldKey1, K1 newKey1)
        {
            return d1.Update(oldKey1, newKey1);
        }

        public bool UpdateAt0(int idx, K0 newKey0)
        {
            return d0.UpdateAt(idx, newKey0);
        }
        public bool UpdateAt1(int idx, K1 newKey1)
        {
            return d1.UpdateAt(idx, newKey1);
        }

        public int Count
        {
            get { return d0.Count; }
        }

        public void ForEach(Func<int, V, bool> a)
        {
            d0.ForEachIndex(new Dict<K0, Data>.IdxIterEx(idx =>
            {
                return a(idx, d0.ValueAt(idx).value);
            }));
        }

        public void ForEach(Action<int, V> a)
        {
            d0.ForEachIndex(new Dict<K0, Data>.IdxIter(idx =>
            {
                a(idx, d0.ValueAt(idx).value);
            }));
        }
    }

    /// <summary>
    /// 多 key 字典之 3 个 key
    /// </summary>
    public partial class DictEx<V, K0, K1, K2>
    {
        protected class Data
        {
            public V value;
        }

        protected Dict<K0, Data> d0;
        protected Dict<K1, Data> d1;
        protected Dict<K2, Data> d2;

        public DictEx(int capacity = 16)
        {
            d0 = new Dict<K0, Data>(capacity);
            d1 = new Dict<K1, Data>(capacity);
            d2 = new Dict<K2, Data>(capacity);
        }

        public bool Exists0(K0 key0)
        {
            return d0.Find(key0) != -1;
        }
        public bool Exists1(K1 key1)
        {
            return d1.Find(key1) != -1;
        }
        public bool Exists2(K2 key2)
        {
            return d2.Find(key2) != -1;
        }

        public bool TryGetValue0(K0 key0, out V value)
        {
            int idx = d0.Find(key0);
            if (idx == -1)
            {
                value = default(V);
                return false;
            }
            value = d0.ValueAt(idx).value;
            return true;
        }
        public bool TryGetValue1(K1 key1, out V value)
        {
            int idx = d1.Find(key1);
            if (idx == -1)
            {
                value = default(V);
                return false;
            }
            value = d1.ValueAt(idx).value;
            return true;
        }
        public bool TryGetValue2(K2 key2, out V value)
        {
            int idx = d2.Find(key2);
            if (idx == -1)
            {
                value = default(V);
                return false;
            }
            value = d2.ValueAt(idx).value;
            return true;
        }

        public V GetValue0(K0 key0)
        {
            var d = d0[key0];
            return d == null ? default(V) : d.value;
        }
        public V GetValue1(K1 key1)
        {
            var d = d1[key1];
            return d == null ? default(V) : d.value;
        }
        public V GetValue2(K2 key2)
        {
            var d = d2[key2];
            return d == null ? default(V) : d.value;
        }

        public bool Add(V value, K0 key0, K1 key1, K2 key2)
        {
            var d = new Data { value = value };
            var r0 = d0.Add(key0, d);
            if (!r0.success)
            {
                return false;
            }

            var r1 = d1.Add(key1, d);
            if (!r1.success)
            {
                d0.RemoveAt(r0.index);
                return false;
            }

            var r2 = d2.Add(key2, d);
            if (!r2.success)
            {
                d0.RemoveAt(r0.index);
                d1.RemoveAt(r1.index);
                return false;
            }

            return true;
        }

        public bool Remove0(K0 key0)
        {
            int idx = d0.Find(key0);
            if (idx == -1) return false;
            RemoveAt(idx);
            return true;
        }
        public bool Remove1(K1 key1)
        {
            int idx = d1.Find(key1);
            if (idx == -1) return false;
            RemoveAt(idx);
            return true;
        }
        public bool Remove2(K2 key2)
        {
            int idx = d2.Find(key2);
            if (idx == -1) return false;
            RemoveAt(idx);
            return true;
        }

        public void Clear()
        {
            d0.Clear();
            d1.Clear();
            d2.Clear();
        }

        public int Find0(K0 key0)
        {
            return d0.Find(key0);
        }
        public int Find1(K1 key1)
        {
            return d1.Find(key1);
        }
        public int Find2(K2 key2)
        {
            return d2.Find(key2);
        }

        public K0 KeyAt0(int idx)
        {
            return d0.KeyAt(idx);
        }
        public K1 KeyAt1(int idx)
        {
            return d1.KeyAt(idx);
        }
        public K2 KeyAt2(int idx)
        {
            return d2.KeyAt(idx);
        }

        public V ValueAt(int idx)
        {
            return d0.ValueAt(idx).value;
        }
        public void RemoveAt(int idx)
        {
            d0.RemoveAt(idx);
            d1.RemoveAt(idx);
            d2.RemoveAt(idx);
        }

        public bool Update0(K0 oldKey0, K0 newKey0)
        {
            return d0.Update(oldKey0, newKey0);
        }
        public bool Update1(K1 oldKey1, K1 newKey1)
        {
            return d1.Update(oldKey1, newKey1);
        }
        public bool Update2(K2 oldKey2, K2 newKey2)
        {
            return d2.Update(oldKey2, newKey2);
        }

        public bool UpdateAt0(int idx, K0 newKey0)
        {
            return d0.UpdateAt(idx, newKey0);
        }
        public bool UpdateAt1(int idx, K1 newKey1)
        {
            return d1.UpdateAt(idx, newKey1);
        }
        public bool UpdateAt2(int idx, K2 newKey2)
        {
            return d2.UpdateAt(idx, newKey2);
        }

        public int Count
        {
            get { return d0.Count; }
        }

        public void ForEach(Func<int, V, bool> a)
        {
            d0.ForEachIndex(new Dict<K0, Data>.IdxIterEx(idx =>
            {
                return a(idx, d0.ValueAt(idx).value);
            }));
        }

        public void ForEach(Action<int, V> a)
        {
            d0.ForEachIndex(new Dict<K0, Data>.IdxIter(idx =>
            {
                a(idx, d0.ValueAt(idx).value);
            }));
        }
    }


    /// <summary>
    /// 多 key 字典之 4 个 key
    /// </summary>
    public partial class DictEx<V, K0, K1, K2, K3>
    {
        protected class Data
        {
            public V value;
        }

        protected Dict<K0, Data> d0;
        protected Dict<K1, Data> d1;
        protected Dict<K2, Data> d2;
        protected Dict<K3, Data> d3;

        public DictEx(int capacity = 16)
        {
            d0 = new Dict<K0, Data>(capacity);
            d1 = new Dict<K1, Data>(capacity);
            d2 = new Dict<K2, Data>(capacity);
            d3 = new Dict<K3, Data>(capacity);
        }

        public bool Exists0(K0 key0)
        {
            return d0.Find(key0) != -1;
        }
        public bool Exists1(K1 key1)
        {
            return d1.Find(key1) != -1;
        }
        public bool Exists2(K2 key2)
        {
            return d2.Find(key2) != -1;
        }
        public bool Exists3(K3 key3)
        {
            return d3.Find(key3) != -1;
        }

        public bool TryGetValue0(K0 key0, out V value)
        {
            int idx = d0.Find(key0);
            if (idx == -1)
            {
                value = default(V);
                return false;
            }
            value = d0.ValueAt(idx).value;
            return true;
        }
        public bool TryGetValue1(K1 key1, out V value)
        {
            int idx = d1.Find(key1);
            if (idx == -1)
            {
                value = default(V);
                return false;
            }
            value = d1.ValueAt(idx).value;
            return true;
        }
        public bool TryGetValue2(K2 key2, out V value)
        {
            int idx = d2.Find(key2);
            if (idx == -1)
            {
                value = default(V);
                return false;
            }
            value = d2.ValueAt(idx).value;
            return true;
        }
        public bool TryGetValue3(K3 key3, out V value)
        {
            int idx = d3.Find(key3);
            if (idx == -1)
            {
                value = default(V);
                return false;
            }
            value = d3.ValueAt(idx).value;
            return true;
        }

        public V GetValue0(K0 key0)
        {
            var d = d0[key0];
            return d == null ? default(V) : d.value;
        }
        public V GetValue1(K1 key1)
        {
            var d = d1[key1];
            return d == null ? default(V) : d.value;
        }
        public V GetValue2(K2 key2)
        {
            var d = d2[key2];
            return d == null ? default(V) : d.value;
        }
        public V GetValue3(K3 key3)
        {
            var d = d3[key3];
            return d == null ? default(V) : d.value;
        }

        public bool Add(V value, K0 key0, K1 key1, K2 key2, K3 key3)
        {
            var d = new Data { value = value };
            var r0 = d0.Add(key0, d);
            if (!r0.success)
            {
                return false;
            }

            var r1 = d1.Add(key1, d);
            if (!r1.success)
            {
                d0.RemoveAt(r0.index);
                return false;
            }

            var r2 = d2.Add(key2, d);
            if (!r2.success)
            {
                d0.RemoveAt(r0.index);
                d1.RemoveAt(r1.index);
                return false;
            }

            var r3 = d3.Add(key3, d);
            if (!r3.success)
            {
                d0.RemoveAt(r0.index);
                d1.RemoveAt(r1.index);
                d2.RemoveAt(r2.index);
                return false;
            }

            return true;
        }

        public bool Remove0(K0 key0)
        {
            int idx = d0.Find(key0);
            if (idx == -1) return false;
            RemoveAt(idx);
            return true;
        }
        public bool Remove1(K1 key1)
        {
            int idx = d1.Find(key1);
            if (idx == -1) return false;
            RemoveAt(idx);
            return true;
        }
        public bool Remove2(K2 key2)
        {
            int idx = d2.Find(key2);
            if (idx == -1) return false;
            RemoveAt(idx);
            return true;
        }
        public bool Remove3(K3 key3)
        {
            int idx = d3.Find(key3);
            if (idx == -1) return false;
            RemoveAt(idx);
            return true;
        }

        public void Clear()
        {
            d0.Clear();
            d1.Clear();
            d2.Clear();
            d3.Clear();
        }

        public int Find0(K0 key0)
        {
            return d0.Find(key0);
        }
        public int Find1(K1 key1)
        {
            return d1.Find(key1);
        }
        public int Find2(K2 key2)
        {
            return d2.Find(key2);
        }
        public int Find3(K3 key3)
        {
            return d3.Find(key3);
        }

        public K0 KeyAt0(int idx)
        {
            return d0.KeyAt(idx);
        }
        public K1 KeyAt1(int idx)
        {
            return d1.KeyAt(idx);
        }
        public K2 KeyAt2(int idx)
        {
            return d2.KeyAt(idx);
        }
        public K3 KeyAt3(int idx)
        {
            return d3.KeyAt(idx);
        }

        public V ValueAt(int idx)
        {
            return d0.ValueAt(idx).value;
        }
        public void RemoveAt(int idx)
        {
            d0.RemoveAt(idx);
            d1.RemoveAt(idx);
            d2.RemoveAt(idx);
            d3.RemoveAt(idx);
        }

        public bool Update0(K0 oldKey0, K0 newKey0)
        {
            return d0.Update(oldKey0, newKey0);
        }
        public bool Update1(K1 oldKey1, K1 newKey1)
        {
            return d1.Update(oldKey1, newKey1);
        }
        public bool Update2(K2 oldKey2, K2 newKey2)
        {
            return d2.Update(oldKey2, newKey2);
        }
        public bool Update3(K3 oldKey3, K3 newKey3)
        {
            return d3.Update(oldKey3, newKey3);
        }

        public bool UpdateAt0(int idx, K0 newKey0)
        {
            return d0.UpdateAt(idx, newKey0);
        }
        public bool UpdateAt1(int idx, K1 newKey1)
        {
            return d1.UpdateAt(idx, newKey1);
        }
        public bool UpdateAt2(int idx, K2 newKey2)
        {
            return d2.UpdateAt(idx, newKey2);
        }
        public bool UpdateAt3(int idx, K3 newKey3)
        {
            return d3.UpdateAt(idx, newKey3);
        }

        public int Count
        {
            get { return d0.Count; }
        }

        public void ForEach(Func<int, V, bool> a)
        {
            d0.ForEachIndex(new Dict<K0, Data>.IdxIterEx(idx =>
            {
                return a(idx, d0.ValueAt(idx).value);
            }));
        }

        public void ForEach(Action<int, V> a)
        {
            d0.ForEachIndex(new Dict<K0, Data>.IdxIter(idx =>
            {
                a(idx, d0.ValueAt(idx).value);
            }));
        }
    }
}
