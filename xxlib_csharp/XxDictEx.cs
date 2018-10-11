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
            public int idx0 = -1;
            public int idx1 = -1;
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
            Data d;
            var r = d0.TryGetValue(key0, out d);
            value = d.value;
            return r;
        }
        public bool TryGetValue1(K1 key1, out V value)
        {
            Data d;
            var r = d1.TryGetValue(key1, out d);
            value = d.value;
            return r;
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

            d.idx0 = r0.index;
            d.idx1 = r1.index;

            return true;
        }


        public bool Remove0(K0 key0)
        {
            Data d;
            if (!d0.TryGetValue(key0, out d)) return false;
            RemoveAt(d);
            return true;
        }
        public bool Remove1(K1 key1)
        {
            Data d;
            if (!d1.TryGetValue(key1, out d)) return false;
            RemoveAt(d);
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
            var idx = d1.Find(key1);
            if (idx == -1) return -1;
            return d1.ValueAt(idx).idx0;
        }


        public V ValueAt(int idx)
        {
            return d0.ValueAt(idx).value;
        }
        public void RemoveAt(int idx)
        {
            RemoveAt(d0.ValueAt(idx));
        }
        protected void RemoveAt(Data d)
        {
            d0.RemoveAt(d.idx0);
            d1.RemoveAt(d.idx1);
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
            return d1.UpdateAt(d0.ValueAt(idx).idx1, newKey1);
        }





        public int Count
        {
            get { return d0.Count; }
        }

        public void ForEach(Func<V, bool> a)
        {
            d0.ForEach(new Dict<K0, Data>.KVIterEx(d =>
            {
                return a(d.value.value);
            }));
        }

        public void ForEach(Action<V> a)
        {
            d0.ForEach(new Dict<K0, Data>.KVIter(d =>
            {
                a(d.value.value);
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
            public int idx0 = -1;
            public int idx1 = -1;
            public int idx2 = -1;
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
            Data d;
            var r = d0.TryGetValue(key0, out d);
            value = d.value;
            return r;
        }
        public bool TryGetValue1(K1 key1, out V value)
        {
            Data d;
            var r = d1.TryGetValue(key1, out d);
            value = d.value;
            return r;
        }
        public bool TryGetValue2(K2 key2, out V value)
        {
            Data d;
            var r = d2.TryGetValue(key2, out d);
            value = d.value;
            return r;
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

            d.idx0 = r0.index;
            d.idx1 = r1.index;
            d.idx2 = r2.index;

            return true;
        }


        public bool Remove0(K0 key0)
        {
            Data d;
            if (!d0.TryGetValue(key0, out d)) return false;
            RemoveAt(d);
            return true;
        }
        public bool Remove1(K1 key1)
        {
            Data d;
            if (!d1.TryGetValue(key1, out d)) return false;
            RemoveAt(d);
            return true;
        }
        public bool Remove2(K2 key2)
        {
            Data d;
            if (!d2.TryGetValue(key2, out d)) return false;
            RemoveAt(d);
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
            var idx = d1.Find(key1);
            if (idx == -1) return -1;
            return d1.ValueAt(idx).idx0;
        }
        public int Find2(K2 key2)
        {
            var idx = d2.Find(key2);
            if (idx == -1) return -1;
            return d2.ValueAt(idx).idx0;
        }

        public V ValueAt(int idx)
        {
            return d0.ValueAt(idx).value;
        }
        public void RemoveAt(int idx)
        {
            RemoveAt(d0.ValueAt(idx));
        }
        protected void RemoveAt(Data d)
        {
            d0.RemoveAt(d.idx0);
            d1.RemoveAt(d.idx1);
            d2.RemoveAt(d.idx2);
        }


        public bool Update0(K0 oldKey1, K0 newKey0)
        {
            return d0.Update(oldKey1, newKey0);
        }
        public bool Update1(K1 oldKey2, K1 newKey1)
        {
            return d1.Update(oldKey2, newKey1);
        }
        public bool Update2(K2 oldKey3, K2 newKey2)
        {
            return d2.Update(oldKey3, newKey2);
        }
        public bool UpdateAt0(int idx, K0 newKey0)
        {
            return d0.UpdateAt(idx, newKey0);
        }
        public bool UpdateAt1(int idx, K1 newKey1)
        {
            return d1.UpdateAt(d0.ValueAt(idx).idx1, newKey1);
        }
        public bool UpdateAt2(int idx, K2 newKey2)
        {
            return d2.UpdateAt(d0.ValueAt(idx).idx2, newKey2);
        }




        public int Count
        {
            get { return d0.Count; }
        }

        public void ForEach(Func<V, bool> a)
        {
            d0.ForEach(new Dict<K0, Data>.KVIterEx(d =>
            {
                return a(d.value.value);
            }));
        }

        public void ForEach(Action<V> a)
        {
            d0.ForEach(new Dict<K0, Data>.KVIter(d =>
            {
                a(d.value.value);
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
            public int idx0 = -1;
            public int idx1 = -1;
            public int idx2 = -1;
            public int idx3 = -1;
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
            Data d;
            var r = d0.TryGetValue(key0, out d);
            value = d.value;
            return r;
        }
        public bool TryGetValue1(K1 key1, out V value)
        {
            Data d;
            var r = d1.TryGetValue(key1, out d);
            value = d.value;
            return r;
        }
        public bool TryGetValue2(K2 key2, out V value)
        {
            Data d;
            var r = d2.TryGetValue(key2, out d);
            value = d.value;
            return r;
        }
        public bool TryGetValue3(K3 key3, out V value)
        {
            Data d;
            var r = d3.TryGetValue(key3, out d);
            value = d.value;
            return r;
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

            d.idx0 = r0.index;
            d.idx1 = r1.index;
            d.idx2 = r2.index;
            d.idx3 = r3.index;

            return true;
        }


        public bool Remove0(K0 key0)
        {
            Data d;
            if (!d0.TryGetValue(key0, out d)) return false;
            RemoveAt(d);
            return true;
        }
        public bool Remove1(K1 key1)
        {
            Data d;
            if (!d1.TryGetValue(key1, out d)) return false;
            RemoveAt(d);

            return true;
        }
        public bool Remove2(K2 key2)
        {
            Data d;
            if (!d2.TryGetValue(key2, out d)) return false;
            RemoveAt(d);

            return true;
        }
        public bool Remove3(K3 key3)
        {
            Data d;
            if (!d3.TryGetValue(key3, out d)) return false;
            RemoveAt(d);
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
            var idx = d1.Find(key1);
            if (idx == -1) return -1;
            return d1.ValueAt(idx).idx0;
        }
        public int Find2(K2 key2)
        {
            var idx = d2.Find(key2);
            if (idx == -1) return -1;
            return d2.ValueAt(idx).idx0;
        }
        public int Find3(K3 key3)
        {
            var idx = d3.Find(key3);
            if (idx == -1) return -1;
            return d3.ValueAt(idx).idx0;
        }


        public V ValueAt(int idx)
        {
            return d0.ValueAt(idx).value;
        }
        public void RemoveAt(int idx)
        {
            RemoveAt(d0.ValueAt(idx));
        }
        protected void RemoveAt(Data d)
        {
            d0.RemoveAt(d.idx0);
            d1.RemoveAt(d.idx1);
            d2.RemoveAt(d.idx2);
            d3.RemoveAt(d.idx3);
        }


        public bool Update0(K0 oldKey1, K0 newKey0)
        {
            return d0.Update(oldKey1, newKey0);
        }
        public bool Update1(K1 oldKey2, K1 newKey1)
        {
            return d1.Update(oldKey2, newKey1);
        }
        public bool Update2(K2 oldKey3, K2 newKey2)
        {
            return d2.Update(oldKey3, newKey2);
        }
        public bool Update3(K3 oldKey4, K3 newKey3)
        {
            return d3.Update(oldKey4, newKey3);
        }
        public bool UpdateAt0(int idx, K0 newKey0)
        {
            return d0.UpdateAt(idx, newKey0);
        }
        public bool UpdateAt1(int idx, K1 newKey1)
        {
            return d1.UpdateAt(d0.ValueAt(idx).idx1, newKey1);
        }
        public bool UpdateAt2(int idx, K2 newKey2)
        {
            return d2.UpdateAt(d0.ValueAt(idx).idx2, newKey2);
        }
        public bool UpdateAt3(int idx, K3 newKey3)
        {
            return d3.UpdateAt(d0.ValueAt(idx).idx3, newKey3);
        }



        public int Count
        {
            get { return d0.Count; }
        }

        public void ForEach(Func<V, bool> a)
        {
            d0.ForEach(new Dict<K0, Data>.KVIterEx(d =>
            {
                return a(d.value.value);
            }));
        }

        public void ForEach(Action<V> a)
        {
            d0.ForEach(new Dict<K0, Data>.KVIter(d =>
            {
                a(d.value.value);
            }));
        }
    }
}
