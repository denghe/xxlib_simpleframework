using System;

namespace xx
{
    /// <summary>
    /// 多 key 字典之 2 个 key
    /// </summary>
    public partial class DictEx<V, K1, K2>
    {
        protected class Data
        {
            public V value;
            public int idx1 = -1;
            public int idx2 = -1;
        }


        protected Dict<K1, Data> d1;
        protected Dict<K2, Data> d2;


        public DictEx(int capacity = 16)
        {
            d1 = new Dict<K1, Data>(capacity);
            d2 = new Dict<K2, Data>(capacity);
        }


        public bool Exists1(K1 key1)
        {
            return d1.Find(key1) != -1;
        }
        public bool Exists2(K2 key2)
        {
            return d2.Find(key2) != -1;
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


        public bool Add(V value, K1 key1, K2 key2)
        {
            var d = new Data { value = value };
            var r1 = d1.Add(key1, d);
            if (!r1.success)
            {
                return false;
            }

            var r2 = d2.Add(key2, d);
            if (!r2.success)
            {
                d1.RemoveAt(r1.index);
                return false;
            }

            d.idx1 = r1.index;
            d.idx2 = r2.index;

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
            d1.Clear();
            d2.Clear();
        }


        public int Find1(K1 key1)
        {
            return d1.Find(key1);
        }
        public int Find2(K2 key2)
        {
            var idx = d2.Find(key2);
            if (idx == -1) return -1;
            return d2.ValueAt(idx).idx1;
        }


        public V ValueAt(int idx)
        {
            return d1.ValueAt(idx).value;
        }
        public void RemoveAt(int idx)
        {
            RemoveAt(d1.ValueAt(idx));
        }
        protected void RemoveAt(Data d)
        {
            d1.RemoveAt(d.idx1);
            d2.RemoveAt(d.idx2);
        }


        public bool Update1(K1 oldKey1, K1 newKey1)
        {
            return d1.ModifyKey(oldKey1, newKey1);
        }
        public bool Update2(K2 oldKey2, K2 newKey2)
        {
            return d2.ModifyKey(oldKey2, newKey2);
        }
        public bool UpdateAt1(int idx, K1 newKey1)
        {
            return d1.ModifyKeyAt(idx, newKey1);
        }
        public bool UpdateAt2(int idx, K2 newKey2)
        {
            return d2.ModifyKeyAt(d1.ValueAt(idx).idx2, newKey2);
        }





        public int Count
        {
            get { return d1.Count; }
        }

        public void ForEach(Func<V, bool> a)
        {
            d1.ForEach(new Dict<K1, Data>.KVIterEx(d =>
            {
                return a(d.value.value);
            }));
        }

        public void ForEach(Action<V> a)
        {
            d1.ForEach(new Dict<K1, Data>.KVIter(d =>
            {
                a(d.value.value);
            }));
        }
    }


    /// <summary>
    /// 多 key 字典之 3 个 key
    /// </summary>
    public partial class DictEx<V, K1, K2, K3>
    {
        protected class Data
        {
            public V value;
            public int idx1 = -1;
            public int idx2 = -1;
            public int idx3 = -1;
        }


        protected Dict<K1, Data> d1;
        protected Dict<K2, Data> d2;
        protected Dict<K3, Data> d3;


        public DictEx(int capacity = 16)
        {
            d1 = new Dict<K1, Data>(capacity);
            d2 = new Dict<K2, Data>(capacity);
            d3 = new Dict<K3, Data>(capacity);
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



        public bool Add(V value, K1 key1, K2 key2, K3 key3)
        {
            var d = new Data { value = value };
            var r1 = d1.Add(key1, d);
            if (!r1.success)
            {
                return false;
            }

            var r2 = d2.Add(key2, d);
            if (!r2.success)
            {
                d1.RemoveAt(r1.index);
                return false;
            }

            var r3 = d3.Add(key3, d);
            if (!r3.success)
            {
                d1.RemoveAt(r1.index);
                d2.RemoveAt(r2.index);
                return false;
            }

            d.idx1 = r1.index;
            d.idx2 = r2.index;
            d.idx3 = r3.index;

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
            d1.Clear();
            d2.Clear();
            d3.Clear();
        }

        public int Find1(K1 key1)
        {
            return d1.Find(key1);
        }
        public int Find2(K2 key2)
        {
            var idx = d2.Find(key2);
            if (idx == -1) return -1;
            return d2.ValueAt(idx).idx1;
        }
        public int Find3(K3 key3)
        {
            var idx = d3.Find(key3);
            if (idx == -1) return -1;
            return d3.ValueAt(idx).idx1;
        }

        public V ValueAt(int idx)
        {
            return d1.ValueAt(idx).value;
        }
        public void RemoveAt(int idx)
        {
            RemoveAt(d1.ValueAt(idx));
        }
        protected void RemoveAt(Data d)
        {
            d1.RemoveAt(d.idx1);
            d2.RemoveAt(d.idx2);
            d3.RemoveAt(d.idx3);
        }


        public bool Update1(K1 oldKey1, K1 newKey1)
        {
            return d1.ModifyKey(oldKey1, newKey1);
        }
        public bool Update2(K2 oldKey2, K2 newKey2)
        {
            return d2.ModifyKey(oldKey2, newKey2);
        }
        public bool Update3(K3 oldKey3, K3 newKey3)
        {
            return d3.ModifyKey(oldKey3, newKey3);
        }
        public bool UpdateAt1(int idx, K1 newKey1)
        {
            return d1.ModifyKeyAt(idx, newKey1);
        }
        public bool UpdateAt2(int idx, K2 newKey2)
        {
            return d2.ModifyKeyAt(d1.ValueAt(idx).idx2, newKey2);
        }
        public bool UpdateAt3(int idx, K3 newKey3)
        {
            return d3.ModifyKeyAt(d1.ValueAt(idx).idx3, newKey3);
        }




        public int Count
        {
            get { return d1.Count; }
        }

        public void ForEach(Func<V, bool> a)
        {
            d1.ForEach(new Dict<K1, Data>.KVIterEx(d =>
            {
                return a(d.value.value);
            }));
        }

        public void ForEach(Action<V> a)
        {
            d1.ForEach(new Dict<K1, Data>.KVIter(d =>
            {
                a(d.value.value);
            }));
        }
    }


    /// <summary>
    /// 多 key 字典之 4 个 key
    /// </summary>
    public partial class DictEx<V, K1, K2, K3, K4>
    {
        protected class Data
        {
            public V value;
            public int idx1 = -1;
            public int idx2 = -1;
            public int idx3 = -1;
            public int idx4 = -1;
        }


        protected Dict<K1, Data> d1;
        protected Dict<K2, Data> d2;
        protected Dict<K3, Data> d3;
        protected Dict<K4, Data> d4;


        public DictEx(int capacity = 16)
        {
            d1 = new Dict<K1, Data>(capacity);
            d2 = new Dict<K2, Data>(capacity);
            d3 = new Dict<K3, Data>(capacity);
            d4 = new Dict<K4, Data>(capacity);
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
        public bool Exists4(K4 key4)
        {
            return d4.Find(key4) != -1;
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
        public bool TryGetValue4(K4 key4, out V value)
        {
            Data d;
            var r = d4.TryGetValue(key4, out d);
            value = d.value;
            return r;
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
        public V GetValue4(K4 key4)
        {
            var d = d4[key4];
            return d == null ? default(V) : d.value;
        }



        public bool Add(V value, K1 key1, K2 key2, K3 key3, K4 key4)
        {
            var d = new Data { value = value };
            var r1 = d1.Add(key1, d);
            if (!r1.success)
            {
                return false;
            }

            var r2 = d2.Add(key2, d);
            if (!r2.success)
            {
                d1.RemoveAt(r1.index);
                return false;
            }

            var r3 = d3.Add(key3, d);
            if (!r3.success)
            {
                d1.RemoveAt(r1.index);
                d2.RemoveAt(r2.index);
                return false;
            }

            var r4 = d4.Add(key4, d);
            if (!r4.success)
            {
                d1.RemoveAt(r1.index);
                d2.RemoveAt(r2.index);
                d3.RemoveAt(r3.index);
                return false;
            }

            d.idx1 = r1.index;
            d.idx2 = r2.index;
            d.idx3 = r3.index;
            d.idx4 = r4.index;

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
        public bool Remove4(K4 key4)
        {
            Data d;
            if (!d4.TryGetValue(key4, out d)) return false;
            RemoveAt(d);
            return true;
        }


        public void Clear()
        {
            d1.Clear();
            d2.Clear();
            d3.Clear();
            d4.Clear();
        }


        public int Find1(K1 key1)
        {
            return d1.Find(key1);
        }
        public int Find2(K2 key2)
        {
            var idx = d2.Find(key2);
            if (idx == -1) return -1;
            return d2.ValueAt(idx).idx1;
        }
        public int Find3(K3 key3)
        {
            var idx = d3.Find(key3);
            if (idx == -1) return -1;
            return d3.ValueAt(idx).idx1;
        }
        public int Find4(K4 key4)
        {
            var idx = d4.Find(key4);
            if (idx == -1) return -1;
            return d4.ValueAt(idx).idx1;
        }


        public V ValueAt(int idx)
        {
            return d1.ValueAt(idx).value;
        }
        public void RemoveAt(int idx)
        {
            RemoveAt(d1.ValueAt(idx));
        }
        protected void RemoveAt(Data d)
        {
            d1.RemoveAt(d.idx1);
            d2.RemoveAt(d.idx2);
            d3.RemoveAt(d.idx3);
            d4.RemoveAt(d.idx4);
        }


        public bool Update1(K1 oldKey1, K1 newKey1)
        {
            return d1.ModifyKey(oldKey1, newKey1);
        }
        public bool Update2(K2 oldKey2, K2 newKey2)
        {
            return d2.ModifyKey(oldKey2, newKey2);
        }
        public bool Update3(K3 oldKey3, K3 newKey3)
        {
            return d3.ModifyKey(oldKey3, newKey3);
        }
        public bool Update4(K4 oldKey4, K4 newKey4)
        {
            return d4.ModifyKey(oldKey4, newKey4);
        }
        public bool UpdateAt1(int idx, K1 newKey1)
        {
            return d1.ModifyKeyAt(idx, newKey1);
        }
        public bool UpdateAt2(int idx, K2 newKey2)
        {
            return d2.ModifyKeyAt(d1.ValueAt(idx).idx2, newKey2);
        }
        public bool UpdateAt3(int idx, K3 newKey3)
        {
            return d3.ModifyKeyAt(d1.ValueAt(idx).idx3, newKey3);
        }
        public bool UpdateAt4(int idx, K4 newKey4)
        {
            return d4.ModifyKeyAt(d1.ValueAt(idx).idx4, newKey4);
        }



        public int Count
        {
            get { return d1.Count; }
        }

        public void ForEach(Func<V, bool> a)
        {
            d1.ForEach(new Dict<K1, Data>.KVIterEx(d =>
            {
                return a(d.value.value);
            }));
        }

        public void ForEach(Action<V> a)
        {
            d1.ForEach(new Dict<K1, Data>.KVIter(d =>
            {
                a(d.value.value);
            }));
        }
    }
}
