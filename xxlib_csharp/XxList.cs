using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Text;

namespace xx
{
    // 抄自 MS 代码并删除小改加料( 支持多级序列化 )

    public class List<T> : IObject
    {
        public T[] buf;
        public int bufLen { get { return buf.Length; } }

        /// <summary>
        /// 数据长, 同原 List 的 Count
        /// </summary>
        public int dataLen;

        private const int defaultCapacity = 4;
        static readonly T[] emptyBuf = new T[0];

        /// <summary>
        /// 空数据构造
        /// </summary>
        public List()
        {
            buf = emptyBuf;
        }

        /// <summary>
        /// 元素直接构造函数
        /// </summary>
        public List(params T[] items)
        {
            buf = items;
            dataLen = buf.Length;
        }

        public T this[int index]
        {
            get
            {
                Debug.Assert(index >= 0 && index < dataLen);
                return buf[index];
            }

            set
            {
                Debug.Assert(index >= 0 && index < dataLen);
                buf[index] = value;
            }
        }

        public void Add(T item)
        {
            if (dataLen == buf.Length) Reserve(dataLen + 1);
            buf[dataLen++] = item;
        }
        public void AddRange(IEnumerable<T> collection)
        {
            InsertRange(dataLen, collection);
        }

        public void Clear()
        {
            if (dataLen > 0)
            {
                Array.Clear(buf, 0, dataLen); // Don't need to doc this but we clear the elements so that the gc can reclaim the references.
                dataLen = 0;
            }
        }
        public void CopyTo(int index, T[] array, int arrayIndex, int count)
        {
            Debug.Assert(dataLen - index >= count);
            Array.Copy(buf, index, array, arrayIndex, count);
        }
        public void Reserve(int min)
        {
            if (buf.Length < min)
            {
                int newCapacity = buf.Length == 0 ? defaultCapacity : buf.Length * 2;
                if (newCapacity < min) newCapacity = min;
                if (newCapacity < buf.Length) return;
                T[] newItems = new T[newCapacity];
                if (dataLen > 0)
                {
                    Array.Copy(buf, 0, newItems, 0, dataLen);
                }
                buf = newItems;
            }
        }


        public bool Contains(T item)
        {
            return Find(item) != -1;
        }
        public bool Exists(Predicate<T> match)
        {
            return Find(match) != -1;
        }
        /// <summary>
        /// 查找首个并返回下标. -1 表示没找到
        /// </summary>
        public int Find(Predicate<T> match)
        {
            Debug.Assert(match != null);
            for (int i = 0; i < dataLen; i++)
            {
                if (match(buf[i]))
                {
                    return i;
                }
            }
            return -1;
        }
        /// <summary>
        /// 查找首个并返回下标. -1 表示没找到
        /// </summary>
        public int Find(T item)
        {
            if ((object)item == null)
            {
                for (int i = 0; i < dataLen; i++)
                    if ((object)buf[i] == null)
                        return i;
            }
            else
            {
                var c = EqualityComparer<T>.Default;
                for (int i = 0; i < dataLen; i++)
                {
                    if (c.Equals(buf[i], item)) return i;
                }
            }
            return -1;
        }
        public void ForEach(Action<T> action)
        {
            Debug.Assert(action != null);
            for (int i = 0; i < dataLen; i++)
            {
                action(buf[i]);
            }
        }
        public void ForEachReverse(Action<T> action)
        {
            Debug.Assert(action != null);
            for (int i = dataLen - 1; i >= 0; --i)
            {
                action(buf[i]);
            }
        }

        public void Insert(int index, T item)
        {
            Debug.Assert((uint)index <= (uint)dataLen);
            if (dataLen == buf.Length) Reserve(dataLen + 1);
            if (index < dataLen)
            {
                Array.Copy(buf, index, buf, index + 1, dataLen - index);
            }
            buf[index] = item;
            dataLen++;
        }
        public void InsertRange(int index, IEnumerable<T> collection)
        {
            Debug.Assert(collection == null);
            Debug.Assert((uint)index <= (uint)dataLen);
            ICollection<T> c = collection as ICollection<T>;
            if (c != null)
            {    // if collection is ICollection<T>
                int count = c.Count;
                if (count > 0)
                {
                    Reserve(dataLen + count);
                    if (index < dataLen)
                    {
                        Array.Copy(buf, index, buf, index + count, dataLen - index);
                    }

                    // If we're inserting a List into itself, we want to be able to deal with that.
                    if (this == c)
                    {
                        // Copy first part of _items to insert location
                        Array.Copy(buf, 0, buf, index, index);
                        // Copy last part of _items back to inserted location
                        Array.Copy(buf, index + count, buf, index * 2, dataLen - index);
                    }
                    else
                    {
                        T[] itemsToInsert = new T[count];
                        c.CopyTo(itemsToInsert, 0);
                        itemsToInsert.CopyTo(buf, index);
                    }
                    dataLen += count;
                }
            }
            else
            {
                using (IEnumerator<T> en = collection.GetEnumerator())
                {
                    while (en.MoveNext())
                    {
                        Insert(index++, en.Current);
                    }
                }
            }
        }

        public bool Remove(T item)
        {
            int index = Find(item);
            if (index >= 0)
            {
                RemoveAt(index);
                return true;
            }

            return false;
        }
        public void RemoveAt(int index)
        {
            Debug.Assert((uint)index < (uint)dataLen);
            dataLen--;
            if (index < dataLen)
            {
                Array.Copy(buf, index + 1, buf, index, dataLen - index);
            }
            buf[dataLen] = default(T);
        }
        public void RemoveRange(int index, int count)
        {
            Debug.Assert(index >= 0);
            Debug.Assert(count >= 0);
            Debug.Assert(dataLen - index >= count);

            if (count > 0)
            {
                int i = dataLen;
                dataLen -= count;
                if (index < dataLen)
                {
                    Array.Copy(buf, index + count, buf, index, dataLen - index);
                }
                Array.Clear(buf, dataLen, count);
            }
        }


        public void Sort()
        {
            Sort(0, dataLen, null);
        }
        public void Sort(IComparer<T> comparer)
        {
            Sort(0, dataLen, comparer);
        }
        public void Sort(int index, int count, IComparer<T> comparer)
        {
            Debug.Assert(index >= 0);
            Debug.Assert(count >= 0);
            Debug.Assert(dataLen - index >= count);

            Array.Sort<T>(buf, index, count, comparer);
        }


        /****************************************************************/
        // 为方便加的料
        /****************************************************************/

        public void Resize(int len)
        {
            if (len == dataLen) return;
            else if (len < dataLen)
            {
                Array.Clear(buf, len, dataLen - len);
            }
            else // len > dataLen
            {
                Reserve(len);
            }
            dataLen = len;
        }
        public T Top()
        {
            Debug.Assert(dataLen > 0);
            return buf[dataLen - 1];
        }
        public void Pop()
        {
            Debug.Assert(dataLen > 0);
            buf[--dataLen] = default(T);
        }
        public T TopPop()
        {
            Debug.Assert(dataLen > 0);
            try
            {
                return buf[--dataLen];
            }
            finally
            {
                buf[dataLen] = default(T);
            }
        }

        /// <summary>
        /// 将最后一个覆盖至当前 index, 并令 dataLen--
        /// </summary>
        public void SwapRemoveAt(int index)
        {
            if (index + 1 < dataLen)
            {
                buf[index] = buf[dataLen - 1];
            }
            dataLen--;
            buf[dataLen] = default(T);
        }


        /// <summary>
        /// 查找所有满足条件的元素塞到新 List 并返回. 未找到返回元素个数为 0 的 List
        /// </summary>
        public List<T> Where(Predicate<T> match)
        {
            var rtv = new List<T>();
            Debug.Assert(match != null);
            for (int i = 0; i < dataLen; i++)
            {
                if (match(buf[i]))
                {
                    rtv.Add(buf[i]);
                }
            }
            return rtv;
        }

        /// <summary>
        /// 查找首个满足条件的元素并返回. 未返回将返回 T 的默认值
        /// </summary>
        public T FirstOrDefault(Predicate<T> match)
        {
            Debug.Assert(match != null);
            for (int i = 0; i < dataLen; i++)
            {
                if (match(buf[i]))
                {
                    return buf[i];
                }
            }
            return default(T);
        }




        /****************************************************************/
        // ToString, IBBuffer 相关适配
        /****************************************************************/

        public override string ToString()
        {
            var s = new StringBuilder();
            ToString(ref s);
            return s.ToString();
        }

        public ushort GetPackageId()
        {
            return TypeIdMaps<List<T>>.typeId;
        }

        public void ToBBuffer(BBuffer bb)
        {
            ListIBBufferImpl<T>.instance.ToBBuffer(bb, this);
        }

        public void FromBBuffer(BBuffer bb)
        {
            ListIBBufferImpl<T>.instance.FromBBuffer(bb, this);
        }

        public void ToString(ref StringBuilder s)
        {
            if (GetToStringFlag())
            {
                s.Append("[ \"***** recursived *****\" ]");
                return;
            }
            else SetToStringFlag(true);

            s.Append("[ ");
            for (var i = 0; i < dataLen; i++)
            {
                s.Append(ListIBBufferImpl<T>.instance.ToStringCore(this, i) + ", ");
            }
            if (dataLen > 0)
            {
                s.Length -= 2;
                s.Append(" ]");
            }
            else
            {
                s[s.Length - 1] = ']';
            }

            SetToStringFlag(false);
        }
        public void ToStringCore(ref StringBuilder s) { }

        bool toStringFlag;
        public void SetToStringFlag(bool doing)
        {
            toStringFlag = doing;
        }
        public bool GetToStringFlag()
        {
            return toStringFlag;
        }

        public void MySqlAppend(ref StringBuilder sb, bool ignoreReadOnly)
        {
            if (dataLen == 0) throw new Exception("List no data ??");
            ListIBBufferImpl<T>.instance.MySqlAppend(ref sb, ignoreReadOnly, this);
            sb.Length -= 2;
        }
    }

    public abstract partial class ListIBBufferImpl<T>
    {
        public abstract void ToBBuffer(BBuffer bb, List<T> vs);
        public abstract void FromBBuffer(BBuffer bb, List<T> vs);

        public virtual string ToStringCore(List<T> vs, int idx)
        {
            return vs[idx].ToString();
        }

        public virtual void MySqlAppend(ref StringBuilder sb, bool ignoreReadOnly, List<T> vs)
        {
            var vsBuf = vs.buf;
            for (int i = 0; i < vs.dataLen; i++)
            {
                sb.Append(vsBuf[i]);
                sb.Append(", ");
            }
        }


        public static ListIBBufferImpl<T> instance;

        static ListIBBufferImpl()
        {
            var t = typeof(T);
            if (t.IsGenericType && t.IsValueType)   // Nullable<T>
            {
                switch (Type.GetTypeCode(t.GetGenericArguments()[0]))
                {
                    //case TypeCode.Empty:
                    //case TypeCode.Object:
                    //case TypeCode.DBNull:
                    //case TypeCode.Char:
                    //case TypeCode.Decimal:
                    //case TypeCode.DateTime:
                    //case TypeCode.String:

                    case TypeCode.Boolean:
                        instance = new ListIBBufferImpl_NullableBoolean() as ListIBBufferImpl<T>;
                        break;
                    case TypeCode.SByte:
                        instance = new ListIBBufferImpl_NullableSByte() as ListIBBufferImpl<T>;
                        break;
                    case TypeCode.Byte:
                        instance = new ListIBBufferImpl_NullableByte() as ListIBBufferImpl<T>;
                        break;
                    case TypeCode.Int16:
                        instance = new ListIBBufferImpl_NullableInt16() as ListIBBufferImpl<T>;
                        break;
                    case TypeCode.UInt16:
                        instance = new ListIBBufferImpl_NullableUInt16() as ListIBBufferImpl<T>;
                        break;
                    case TypeCode.Int32:
                        instance = new ListIBBufferImpl_NullableInt32() as ListIBBufferImpl<T>;
                        break;
                    case TypeCode.UInt32:
                        instance = new ListIBBufferImpl_NullableUInt32() as ListIBBufferImpl<T>;
                        break;
                    case TypeCode.Int64:
                        instance = new ListIBBufferImpl_NullableInt64() as ListIBBufferImpl<T>;
                        break;
                    case TypeCode.UInt64:
                        instance = new ListIBBufferImpl_NullableUInt64() as ListIBBufferImpl<T>;
                        break;
                    case TypeCode.Single:
                        instance = new ListIBBufferImpl_NullableSingle() as ListIBBufferImpl<T>;
                        break;
                    case TypeCode.Double:
                        instance = new ListIBBufferImpl_NullableDouble() as ListIBBufferImpl<T>;
                        break;
                    default:
                        break;// throw new NotSupportedException();
                }
            }
            else if (t.IsEnum)
            {
                var et = Enum.GetUnderlyingType(t);
                if (et == typeof(byte)) instance = new ListIBBufferImpl_Enum_Byte<T>() as ListIBBufferImpl<T>;
                else if (et == typeof(sbyte)) instance = new ListIBBufferImpl_Enum_SByte<T>() as ListIBBufferImpl<T>;
                else if (et == typeof(ushort)) instance = new ListIBBufferImpl_Enum_UInt16<T>() as ListIBBufferImpl<T>;
                else if (et == typeof(short)) instance = new ListIBBufferImpl_Enum_Int16<T>() as ListIBBufferImpl<T>;
                else if (et == typeof(uint)) instance = new ListIBBufferImpl_Enum_UInt32<T>() as ListIBBufferImpl<T>;
                else if (et == typeof(int)) instance = new ListIBBufferImpl_Enum_Int32<T>() as ListIBBufferImpl<T>;
                else if (et == typeof(ulong)) instance = new ListIBBufferImpl_Enum_UInt64<T>() as ListIBBufferImpl<T>;
                else if (et == typeof(long)) instance = new ListIBBufferImpl_Enum_Int64<T>() as ListIBBufferImpl<T>;
            }
            else if (typeof(IObject).IsAssignableFrom(t))
            {
                instance = new ListIBBufferImpl_IBBuffer<T>() as ListIBBufferImpl<T>;
            }
            else
            {
                switch (Type.GetTypeCode(t))
                {
                    //case TypeCode.Empty:
                    //case TypeCode.Object:
                    //case TypeCode.DBNull:
                    //case TypeCode.Char:
                    //case TypeCode.Decimal:

                    case TypeCode.Boolean:
                        instance = new ListIBBufferImpl_Boolean() as ListIBBufferImpl<T>;
                        break;
                    case TypeCode.SByte:
                        instance = new ListIBBufferImpl_SByte() as ListIBBufferImpl<T>;
                        break;
                    case TypeCode.Byte:
                        instance = new ListIBBufferImpl_Byte() as ListIBBufferImpl<T>;
                        break;
                    case TypeCode.Int16:
                        instance = new ListIBBufferImpl_Int16() as ListIBBufferImpl<T>;
                        break;
                    case TypeCode.UInt16:
                        instance = new ListIBBufferImpl_UInt16() as ListIBBufferImpl<T>;
                        break;
                    case TypeCode.Int32:
                        instance = new ListIBBufferImpl_Int32() as ListIBBufferImpl<T>;
                        break;
                    case TypeCode.UInt32:
                        instance = new ListIBBufferImpl_UInt32() as ListIBBufferImpl<T>;
                        break;
                    case TypeCode.Int64:
                        instance = new ListIBBufferImpl_Int64() as ListIBBufferImpl<T>;
                        break;
                    case TypeCode.UInt64:
                        instance = new ListIBBufferImpl_UInt64() as ListIBBufferImpl<T>;
                        break;
                    case TypeCode.Single:
                        instance = new ListIBBufferImpl_Single() as ListIBBufferImpl<T>;
                        break;
                    case TypeCode.Double:
                        instance = new ListIBBufferImpl_Double() as ListIBBufferImpl<T>;
                        break;
                    case TypeCode.DateTime:
                        instance = new ListIBBufferImpl_DateTime() as ListIBBufferImpl<T>;
                        break;
                    case TypeCode.String:
                        instance = new ListIBBufferImpl_String() as ListIBBufferImpl<T>;
                        break;
                    default:
                        break;// throw new NotSupportedException(); // 似乎可以在运行时手工初始化 instance 以实现 xx.List< enum type > 的支持
                }
            }
        }
    }

    public partial class ListIBBufferImpl_Enum_Byte<T> : ListIBBufferImpl<T>
    {
        public override void ToBBuffer(BBuffer bb, List<T> vs)
        {
            var vsDataLen = vs.dataLen;
            bb.WriteLength(vsDataLen);
            if (vsDataLen == 0) return;

            bb.Reserve(bb.dataLen + vsDataLen);
            var bbBuf = bb.buf;
            var bbDataLen = bb.dataLen;
            for (int i = 0; i < vsDataLen; i++)
            {
                bbBuf[bbDataLen + i] = (byte)(object)vs[i];
            }
            bb.dataLen += vsDataLen;
        }
        public override void FromBBuffer(BBuffer bb, List<T> vs)
        {
            int len = bb.ReadLength();
            if (bb.readLengthLimit != 0 && len > bb.readLengthLimit) throw new Exception("overflow of limit");
            vs.Resize(len);
            if (len == 0) return;

            var bbBuf = bb.buf;
            var vsBuf = vs.buf;
            var bbOffset = bb.offset;
            for (int i = 0; i < len; i++)
            {
                vsBuf[i] = (T)(object)bbBuf[bbOffset + i];
            }
            bb.offset += len;
        }
        public override string ToStringCore(List<T> vs, int idx)
        {
            return ((byte)(object)vs[idx]).ToString();
        }
    }
    public partial class ListIBBufferImpl_Enum_SByte<T> : ListIBBufferImpl<T>
    {
        public override void ToBBuffer(BBuffer bb, List<T> vs)
        {
            var vsDataLen = vs.dataLen;
            bb.WriteLength(vsDataLen);
            if (vsDataLen == 0) return;

            bb.Reserve(bb.dataLen + vsDataLen);
            var bbBuf = bb.buf;
            var bbDataLen = bb.dataLen;
            for (int i = 0; i < vsDataLen; i++)
            {
                bbBuf[bbDataLen + i] = (byte)(sbyte)(object)vs[i];
            }
            bb.dataLen += vsDataLen;
        }
        public override void FromBBuffer(BBuffer bb, List<T> vs)
        {
            int len = bb.ReadLength();
            if (bb.readLengthLimit != 0 && len > bb.readLengthLimit) throw new Exception("overflow of limit");
            vs.Resize(len);
            if (len == 0) return;

            var bbBuf = bb.buf;
            var vsBuf = vs.buf;
            var bbOffset = bb.offset;
            for (int i = 0; i < len; i++)
            {
                vsBuf[i] = (T)(object)(sbyte)bbBuf[bbOffset + i];
            }
            bb.offset += len;
        }
        public override string ToStringCore(List<T> vs, int idx)
        {
            return ((sbyte)(object)vs[idx]).ToString();
        }
    }
    public partial class ListIBBufferImpl_Enum_UInt16<T> : ListIBBufferImpl<T>
    {
        public override void ToBBuffer(BBuffer bb, List<T> vs)
        {
            var vsDataLen = vs.dataLen;
            bb.WriteLength(vsDataLen);
            if (vsDataLen == 0) return;

            bb.Reserve(bb.dataLen + vsDataLen * 3);
            var bbBuf = bb.buf;
            var vsBuf = vs.buf;
            for (int i = 0; i < vsDataLen; i++)
            {
                BBuffer.Bit7Write(bbBuf, ref bb.dataLen, (ushort)(object)vsBuf[i]);
            }
        }
        public override void FromBBuffer(BBuffer bb, List<T> vs)
        {
            int len = bb.ReadLength();
            if (bb.readLengthLimit != 0 && len > bb.readLengthLimit) throw new Exception("overflow of limit");
            vs.Resize(len);
            if (len == 0) return;

            ushort tmp = 0;
            var vsBuf = vs.buf;
            for (int i = 0; i < len; i++)
            {
                bb.Read(ref tmp);
                vsBuf[i] = (T)(object)tmp;
            }
        }
        public override string ToStringCore(List<T> vs, int idx)
        {
            return ((ushort)(object)vs[idx]).ToString();
        }
    }
    public partial class ListIBBufferImpl_Enum_Int16<T> : ListIBBufferImpl<T>
    {
        public override void ToBBuffer(BBuffer bb, List<T> vs)
        {
            var vsDataLen = vs.dataLen;
            bb.WriteLength(vsDataLen);
            if (vsDataLen == 0) return;

            bb.Reserve(bb.dataLen + vsDataLen * 3);
            var bbBuf = bb.buf;
            var vsBuf = vs.buf;
            for (int i = 0; i < vsDataLen; i++)
            {
                BBuffer.Bit7Write(bbBuf, ref bb.dataLen, BBuffer.ZigZagEncode((short)(object)vsBuf[i]));
            }
        }
        public override void FromBBuffer(BBuffer bb, List<T> vs)
        {
            int len = bb.ReadLength();
            if (bb.readLengthLimit != 0 && len > bb.readLengthLimit) throw new Exception("overflow of limit");
            vs.Resize(len);
            if (len == 0) return;

            short tmp = 0;
            var vsBuf = vs.buf;
            for (int i = 0; i < len; i++)
            {
                bb.Read(ref tmp);
                vsBuf[i] = (T)(object)tmp;
            }
        }
        public override string ToStringCore(List<T> vs, int idx)
        {
            return ((short)(object)vs[idx]).ToString();
        }
    }
    public partial class ListIBBufferImpl_Enum_UInt32<T> : ListIBBufferImpl<T>
    {
        public override void ToBBuffer(BBuffer bb, List<T> vs)
        {
            var vsDataLen = vs.dataLen;
            bb.WriteLength(vsDataLen);
            if (vsDataLen == 0) return;

            bb.Reserve(bb.dataLen + vsDataLen * 5);
            var bbBuf = bb.buf;
            var vsBuf = vs.buf;
            for (int i = 0; i < vsDataLen; i++)
            {
                BBuffer.Bit7Write(bbBuf, ref bb.dataLen, (uint)(object)vsBuf[i]);
            }
        }
        public override void FromBBuffer(BBuffer bb, List<T> vs)
        {
            int len = bb.ReadLength();
            if (bb.readLengthLimit != 0 && len > bb.readLengthLimit) throw new Exception("overflow of limit");
            vs.Resize(len);
            if (len == 0) return;

            uint tmp = 0;
            var vsBuf = vs.buf;
            for (int i = 0; i < len; i++)
            {
                bb.Read(ref tmp);
                vsBuf[i] = (T)(object)tmp;
            }
        }
        public override string ToStringCore(List<T> vs, int idx)
        {
            return ((uint)(object)vs[idx]).ToString();
        }
    }
    public partial class ListIBBufferImpl_Enum_Int32<T> : ListIBBufferImpl<T>
    {
        public override void ToBBuffer(BBuffer bb, List<T> vs)
        {
            var vsDataLen = vs.dataLen;
            bb.WriteLength(vsDataLen);
            if (vsDataLen == 0) return;

            bb.Reserve(bb.dataLen + vsDataLen * 5);
            var bbBuf = bb.buf;
            var vsBuf = vs.buf;
            for (int i = 0; i < vsDataLen; i++)
            {
                BBuffer.Bit7Write(bbBuf, ref bb.dataLen, BBuffer.ZigZagEncode((int)(object)vs[i]));
            }
        }
        public override void FromBBuffer(BBuffer bb, List<T> vs)
        {
            int len = bb.ReadLength();
            if (bb.readLengthLimit != 0 && len > bb.readLengthLimit) throw new Exception("overflow of limit");
            vs.Resize(len);
            if (len == 0) return;

            int tmp = 0;
            var vsBuf = vs.buf;
            for (int i = 0; i < len; i++)
            {
                bb.Read(ref tmp);
                vsBuf[i] = (T)(object)tmp;
            }
        }
        public override string ToStringCore(List<T> vs, int idx)
        {
            return ((int)(object)vs[idx]).ToString();
        }
    }
    public partial class ListIBBufferImpl_Enum_UInt64<T> : ListIBBufferImpl<T>
    {
        public override void ToBBuffer(BBuffer bb, List<T> vs)
        {
            var vsDataLen = vs.dataLen;
            bb.WriteLength(vsDataLen);
            if (vsDataLen == 0) return;

            bb.Reserve(bb.dataLen + vsDataLen * 9);
            var bbBuf = bb.buf;
            var vsBuf = vs.buf;
            for (int i = 0; i < vsDataLen; i++)
            {
                BBuffer.Bit7Write(bbBuf, ref bb.dataLen, (ulong)(object)vsBuf[i]);
            }
        }
        public override void FromBBuffer(BBuffer bb, List<T> vs)
        {
            int len = bb.ReadLength();
            if (bb.readLengthLimit != 0 && len > bb.readLengthLimit) throw new Exception("overflow of limit");
            vs.Resize(len);
            if (len == 0) return;

            ulong tmp = 0;
            var vsBuf = vs.buf;
            for (int i = 0; i < len; i++)
            {
                bb.Read(ref tmp);
                vsBuf[i] = (T)(object)tmp;
            }
        }
        public override string ToStringCore(List<T> vs, int idx)
        {
            return ((ulong)(object)vs[idx]).ToString();
        }
    }
    public partial class ListIBBufferImpl_Enum_Int64<T> : ListIBBufferImpl<T>
    {
        public override void ToBBuffer(BBuffer bb, List<T> vs)
        {
            var vsDataLen = vs.dataLen;
            bb.WriteLength(vsDataLen);
            if (vsDataLen == 0) return;

            bb.Reserve(bb.dataLen + vsDataLen * 9);
            var bbBuf = bb.buf;
            var vsBuf = vs.buf;
            for (int i = 0; i < vsDataLen; i++)
            {
                BBuffer.Bit7Write(bbBuf, ref bb.dataLen, BBuffer.ZigZagEncode((long)(object)vsBuf[i]));
            }
        }
        public override void FromBBuffer(BBuffer bb, List<T> vs)
        {
            int len = bb.ReadLength();
            if (bb.readLengthLimit != 0 && len > bb.readLengthLimit) throw new Exception("overflow of limit");
            vs.Resize(len);
            if (len == 0) return;

            long tmp = 0;
            var vsBuf = vs.buf;
            for (int i = 0; i < len; i++)
            {
                bb.Read(ref tmp);
                vsBuf[i] = (T)(object)tmp;
            }
        }
        public override string ToStringCore(List<T> vs, int idx)
        {
            return ((long)(object)vs[idx]).ToString();
        }
    }

    public partial class ListIBBufferImpl_IBBuffer<T> : ListIBBufferImpl<T>
    {
        public override void ToBBuffer(BBuffer bb, List<T> vs)
        {
            var vsDataLen = vs.dataLen;
            bb.WriteLength(vsDataLen);
            if (vsDataLen == 0) return;

            if (typeof(T).IsValueType)
            {
                for (int i = 0; i < vsDataLen; i++)
                {
                    ((IObject)vs[i]).ToBBuffer(bb);
                }
            }
            else
            {
                for (int i = 0; i < vsDataLen; i++)
                {
                    bb.Write((IObject)vs[i]);
                }
            }
        }
        public override void FromBBuffer(BBuffer bb, List<T> vs)
        {
            int len = bb.ReadLength();
            if (bb.readLengthLimit != 0 && len > bb.readLengthLimit) throw new Exception("overflow of limit");
            vs.Clear();
            vs.Resize(len);
            if (len == 0) return;

            if (typeof(T).IsValueType)
            {
                for (int i = 0; i < len; i++)
                {
                    var tmp = (IObject)default(T);
                    tmp.FromBBuffer(bb);
                    vs[i] = (T)tmp;
                }
            }
            else
            {
                for (int i = 0; i < len; i++)
                {
                    IObject tmp = null;
                    bb.Read(ref tmp);
                    vs[i] = (T)tmp;
                }
            }
        }

        public override void MySqlAppend(ref StringBuilder sb, bool ignoreReadOnly, List<T> vs)
        {
            var vsBuf = vs.buf;
            for (int i = 0; i < vs.dataLen; i++)
            {
                ((IObject)vs[i]).MySqlAppend(ref sb, ignoreReadOnly);
                sb.Append(", ");
            }
        }
    }

    public partial class ListIBBufferImpl_Boolean : ListIBBufferImpl<bool>
    {
        public override void ToBBuffer(BBuffer bb, List<bool> vs)
        {
            var vsDataLen = vs.dataLen;
            bb.WriteLength(vsDataLen);
            if (vsDataLen == 0) return;

            bb.Reserve(bb.dataLen + vsDataLen);
            var vsBuf = vs.buf;
            for (int i = 0; i < vsDataLen; i++)
            {
                bb.Write(vsBuf[i]);     // 先用着这个不优化的方案
            }
        }
        public override void FromBBuffer(BBuffer bb, List<bool> vs)
        {
            int len = bb.ReadLength();
            if (bb.readLengthLimit != 0 && len > bb.readLengthLimit) throw new Exception("overflow of limit");
            vs.Resize(len);
            if (len == 0) return;

            bool tmp = false;
            var vsBuf = vs.buf;
            for (int i = 0; i < len; i++)
            {
                bb.Read(ref tmp);
                vsBuf[i] = tmp;
            }
        }

        // 先禁用 bool 数组的压缩序列化以保持与 cpp 端一致
        //public override void ToBBuffer(BBuffer bb, List<bool> vs)
        //{
        //    var bits = vs.dataLen;
        //    bb.WriteLength(bits);
        //    if (bits == 0) return;

        //    var bytes = (bits + 7) >> 3;
        //    bb.Reserve(bb.dataLen + bytes);
        //    var bbBuf = bb.buf;

        //    var mod = bits % 8;
        //    if (mod > 0)
        //    {
        //        --bytes;
        //    }
        //    byte n = 0;
        //    for (var i = 0; i < bytes; ++i)
        //    {
        //        var p = i << 3;
        //        if (vs[p + 0]) n |= 1;
        //        if (vs[p + 1]) n |= 2;
        //        if (vs[p + 2]) n |= 4;
        //        if (vs[p + 3]) n |= 8;
        //        if (vs[p + 4]) n |= 16;
        //        if (vs[p + 5]) n |= 32;
        //        if (vs[p + 6]) n |= 64;
        //        if (vs[p + 7]) n |= 128;
        //        bbBuf[bb.dataLen++] = n;
        //    }
        //    if (mod > 0)
        //    {
        //        n = 0;
        //        var offset = bytes << 3;
        //        for (var i = 0; i < mod; ++i)
        //        {
        //            if (vs[offset + i])
        //            {
        //                n |= (byte)(1 << i);
        //            }
        //        }
        //        bbBuf[bb.dataLen++] = n;
        //    }
        //}

        //public override void FromBBuffer(BBuffer bb, List<bool> vs)
        //{
        //    int len = bb.ReadLength();
        //    if (bb.readLengthLimit != 0 && len > bb.readLengthLimit) throw new Exception("overflow of limit");
        //    var bytes = (len + 7) >> 3;
        //    vs.Resize(len);
        //    if (len == 0) return;

        //    var bbOffset = bb.offset;
        //    var bbBuf = bb.buf;

        //    var mod = len % 8;
        //    if (mod > 0)
        //    {
        //        --bytes;
        //    }
        //    for (var i = 0; i < bytes; ++i)
        //    {
        //        var n = bbBuf[bbOffset + i];
        //        vs.Add((n & 1) > 0);
        //        vs.Add((n & 2) > 0);
        //        vs.Add((n & 4) > 0);
        //        vs.Add((n & 8) > 0);
        //        vs.Add((n & 16) > 0);
        //        vs.Add((n & 32) > 0);
        //        vs.Add((n & 64) > 0);
        //        vs.Add((n & 128) > 0);
        //    }
        //    if (mod > 0)
        //    {
        //        var n = bbBuf[bbOffset + bytes];
        //        for (var i = 0; i < mod; ++i)
        //        {
        //            vs.Add((n & (1 << i)) > 0);
        //        }
        //    }
        //    bb.offset += bytes;
        //}
    }
    public partial class ListIBBufferImpl_SByte : ListIBBufferImpl<sbyte>
    {
        public override void ToBBuffer(BBuffer bb, List<sbyte> vs)
        {
            var vsDataLen = vs.dataLen;
            bb.WriteLength(vsDataLen);
            if (vsDataLen == 0) return;

            bb.Reserve(bb.dataLen + vsDataLen * 1);
            Buffer.BlockCopy(vs.buf, 0, bb.buf, bb.dataLen, vsDataLen);
            bb.dataLen += vsDataLen;
        }
        public override void FromBBuffer(BBuffer bb, List<sbyte> vs)
        {
            int len = bb.ReadLength();
            if (bb.readLengthLimit != 0 && len > bb.readLengthLimit) throw new Exception("overflow of limit");
            vs.Resize(len);
            if (len == 0) return;

            Buffer.BlockCopy(bb.buf, bb.offset, vs.buf, 0, len);
            bb.offset += len;
        }
    }
    public partial class ListIBBufferImpl_Byte : ListIBBufferImpl<byte>
    {
        public override void ToBBuffer(BBuffer bb, List<byte> vs)
        {
            var vsDataLen = vs.dataLen;
            bb.WriteLength(vsDataLen);
            if (vsDataLen == 0) return;

            bb.Reserve(bb.dataLen + vsDataLen * 1);
            Buffer.BlockCopy(vs.buf, 0, bb.buf, bb.dataLen, vsDataLen);
            bb.dataLen += vsDataLen;
        }
        public override void FromBBuffer(BBuffer bb, List<byte> vs)
        {
            int len = bb.ReadLength();
            if (bb.readLengthLimit != 0 && len > bb.readLengthLimit) throw new Exception("overflow of limit");
            vs.Resize(len);
            if (len == 0) return;

            Buffer.BlockCopy(bb.buf, bb.offset, vs.buf, 0, len);
            bb.offset += len;
        }
    }
    public partial class ListIBBufferImpl_Int16 : ListIBBufferImpl<short>
    {
        public override void ToBBuffer(BBuffer bb, List<short> vs)
        {
            var vsDataLen = vs.dataLen;
            bb.WriteLength(vsDataLen);
            if (vsDataLen == 0) return;

            bb.Reserve(bb.dataLen + vsDataLen * 3);
            var bbBuf = bb.buf;
            var vsBuf = vs.buf;
            for (int i = 0; i < vsDataLen; i++)
            {
                BBuffer.Bit7Write(bbBuf, ref bb.dataLen, BBuffer.ZigZagEncode(vsBuf[i]));
            }
        }
        public override void FromBBuffer(BBuffer bb, List<short> vs)
        {
            int len = bb.ReadLength();
            if (bb.readLengthLimit != 0 && len > bb.readLengthLimit) throw new Exception("overflow of limit");
            vs.Resize(len);
            if (len == 0) return;

            short tmp = 0;
            var vsBuf = vs.buf;
            for (int i = 0; i < len; i++)
            {
                bb.Read(ref tmp);
                vsBuf[i] = tmp;
            }
        }
    }
    public partial class ListIBBufferImpl_UInt16 : ListIBBufferImpl<ushort>
    {
        public override void ToBBuffer(BBuffer bb, List<ushort> vs)
        {
            var vsDataLen = vs.dataLen;
            bb.WriteLength(vsDataLen);
            if (vsDataLen == 0) return;

            bb.Reserve(bb.dataLen + vsDataLen * 3);
            var bbBuf = bb.buf;
            var vsBuf = vs.buf;
            for (int i = 0; i < vsDataLen; i++)
            {
                BBuffer.Bit7Write(bbBuf, ref bb.dataLen, vsBuf[i]);
            }
        }
        public override void FromBBuffer(BBuffer bb, List<ushort> vs)
        {
            int len = bb.ReadLength();
            if (bb.readLengthLimit != 0 && len > bb.readLengthLimit) throw new Exception("overflow of limit");
            vs.Resize(len);
            if (len == 0) return;

            ushort tmp = 0;
            var vsBuf = vs.buf;
            for (int i = 0; i < len; i++)
            {
                bb.Read(ref tmp);
                vsBuf[i] = tmp;
            }
        }
    }
    public partial class ListIBBufferImpl_Int32 : ListIBBufferImpl<int>
    {
        public override void ToBBuffer(BBuffer bb, List<int> vs)
        {
            var vsDataLen = vs.dataLen;
            bb.WriteLength(vsDataLen);
            if (vsDataLen == 0) return;

            bb.Reserve(bb.dataLen + vsDataLen * 5);
            var bbBuf = bb.buf;
            var vsBuf = vs.buf;
            for (int i = 0; i < vsDataLen; i++)
            {
                BBuffer.Bit7Write(bbBuf, ref bb.dataLen, BBuffer.ZigZagEncode(vsBuf[i]));
            }
        }
        public override void FromBBuffer(BBuffer bb, List<int> vs)
        {
            int len = bb.ReadLength();
            if (bb.readLengthLimit != 0 && len > bb.readLengthLimit) throw new Exception("overflow of limit");
            vs.Resize(len);
            if (len == 0) return;

            int tmp = 0;
            var vsBuf = vs.buf;
            for (int i = 0; i < len; i++)
            {
                bb.Read(ref tmp);
                vsBuf[i] = tmp;
            }
        }
    }
    public partial class ListIBBufferImpl_UInt32 : ListIBBufferImpl<uint>
    {
        public override void ToBBuffer(BBuffer bb, List<uint> vs)
        {
            var vsDataLen = vs.dataLen;
            bb.WriteLength(vsDataLen);
            if (vsDataLen == 0) return;

            bb.Reserve(bb.dataLen + vsDataLen * 5);
            var bbBuf = bb.buf;
            var vsBuf = vs.buf;
            for (int i = 0; i < vsDataLen; i++)
            {
                BBuffer.Bit7Write(bbBuf, ref bb.dataLen, vsBuf[i]);
            }
        }
        public override void FromBBuffer(BBuffer bb, List<uint> vs)
        {
            int len = bb.ReadLength();
            if (bb.readLengthLimit != 0 && len > bb.readLengthLimit) throw new Exception("overflow of limit");
            vs.Resize(len);
            if (len == 0) return;

            uint tmp = 0;
            var vsBuf = vs.buf;
            for (int i = 0; i < len; i++)
            {
                bb.Read(ref tmp);
                vsBuf[i] = tmp;
            }
        }
    }
    public partial class ListIBBufferImpl_Int64 : ListIBBufferImpl<long>
    {
        public override void ToBBuffer(BBuffer bb, List<long> vs)
        {
            var vsDataLen = vs.dataLen;
            bb.WriteLength(vsDataLen);
            if (vsDataLen == 0) return;

            bb.Reserve(bb.dataLen + vsDataLen * 9);
            var bbBuf = bb.buf;
            var vsBuf = vs.buf;
            for (int i = 0; i < vsDataLen; i++)
            {
                BBuffer.Bit7Write(bbBuf, ref bb.dataLen, BBuffer.ZigZagEncode(vsBuf[i]));
            }
        }
        public override void FromBBuffer(BBuffer bb, List<long> vs)
        {
            int len = bb.ReadLength();
            if (bb.readLengthLimit != 0 && len > bb.readLengthLimit) throw new Exception("overflow of limit");
            vs.Resize(len);
            if (len == 0) return;

            long tmp = 0;
            var vsBuf = vs.buf;
            for (int i = 0; i < len; i++)
            {
                bb.Read(ref tmp);
                vsBuf[i] = tmp;
            }
        }
    }
    public partial class ListIBBufferImpl_UInt64 : ListIBBufferImpl<ulong>
    {
        public override void ToBBuffer(BBuffer bb, List<ulong> vs)
        {
            var vsDataLen = vs.dataLen;
            bb.WriteLength(vsDataLen);
            if (vsDataLen == 0) return;

            bb.Reserve(bb.dataLen + vsDataLen * 9);
            var bbBuf = bb.buf;
            var vsBuf = vs.buf;
            for (int i = 0; i < vsDataLen; i++)
            {
                BBuffer.Bit7Write(bbBuf, ref bb.dataLen, vsBuf[i]);
            }
        }
        public override void FromBBuffer(BBuffer bb, List<ulong> vs)
        {
            int len = bb.ReadLength();
            if (bb.readLengthLimit != 0 && len > bb.readLengthLimit) throw new Exception("overflow of limit");
            vs.Resize(len);
            if (len == 0) return;

            ulong tmp = 0;
            var vsBuf = vs.buf;
            for (int i = 0; i < len; i++)
            {
                bb.Read(ref tmp);
                vsBuf[i] = tmp;
            }
        }
    }
    public partial class ListIBBufferImpl_Single : ListIBBufferImpl<float>
    {
        public override void ToBBuffer(BBuffer bb, List<float> vs)
        {
            var vsDataLen = vs.dataLen;
            bb.WriteLength(vsDataLen);
            if (vsDataLen == 0) return;

            bb.Reserve(bb.dataLen + vsDataLen * 4);
            Buffer.BlockCopy(vs.buf, 0, bb.buf, bb.dataLen, vsDataLen * 4);
            bb.dataLen += vsDataLen * 4;
        }
        public override void FromBBuffer(BBuffer bb, List<float> vs)
        {
            int len = bb.ReadLength();
            if (bb.readLengthLimit != 0 && len > bb.readLengthLimit) throw new Exception("overflow of limit");
            vs.Resize(len);
            if (len == 0) return;

            Buffer.BlockCopy(bb.buf, bb.offset, vs.buf, 0, len * 4);
            bb.offset += len * 4;
        }
    }
    public partial class ListIBBufferImpl_Double : ListIBBufferImpl<double>
    {
        public override void ToBBuffer(BBuffer bb, List<double> vs)
        {
            var vsDataLen = vs.dataLen;
            bb.WriteLength(vsDataLen);
            if (vsDataLen == 0) return;

            bb.Reserve(bb.dataLen + vsDataLen * 9);
            for (int i = 0; i < vsDataLen; i++)
            {
                bb.WriteDirect(vs[i]);
            }
        }
        public override void FromBBuffer(BBuffer bb, List<double> vs)
        {
            int len = bb.ReadLength();
            if (bb.readLengthLimit != 0 && len > bb.readLengthLimit) throw new Exception("overflow of limit");
            vs.Resize(len);
            if (len == 0) return;

            double tmp = 0;
            var vsBuf = vs.buf;
            for (int i = 0; i < len; i++)
            {
                bb.Read(ref tmp);
                vsBuf[i] = tmp;
            }
        }
    }
    public partial class ListIBBufferImpl_DateTime : ListIBBufferImpl<DateTime>
    {
        public override void ToBBuffer(BBuffer bb, List<DateTime> vs)
        {
            var vsDataLen = vs.dataLen;
            bb.WriteLength(vsDataLen);
            if (vsDataLen == 0) return;

            bb.Reserve(bb.dataLen + vsDataLen * 8);
            for (int i = 0; i < vsDataLen; i++)
            {
                bb.WriteDirect(vs[i]);
            }
        }
        public override void FromBBuffer(BBuffer bb, List<DateTime> vs)
        {
            int len = bb.ReadLength();
            if (bb.readLengthLimit != 0 && len > bb.readLengthLimit) throw new Exception("overflow of limit");
            vs.Resize(len);
            if (len == 0) return;

            var tmp = DateTime.MinValue;
            var vsBuf = vs.buf;
            for (int i = 0; i < len; i++)
            {
                bb.Read(ref tmp);
                vsBuf[i] = tmp;
            }
        }
    }
    public partial class ListIBBufferImpl_String : ListIBBufferImpl<string>
    {
        public override void ToBBuffer(BBuffer bb, List<string> vs)
        {
            var vsDataLen = vs.dataLen;
            bb.WriteLength(vsDataLen);
            if (vsDataLen == 0) return;

            for (int i = 0; i < vsDataLen; i++)
            {
                bb.Write(vs[i]);
            }
        }
        public override void FromBBuffer(BBuffer bb, List<string> vs)
        {
            int len = bb.ReadLength();
            if (bb.readLengthLimit != 0 && len > bb.readLengthLimit) throw new Exception("overflow of limit");
            vs.Resize(len);
            if (len == 0) return;

            string tmp = null;
            var vsBuf = vs.buf;
            for (int i = 0; i < len; i++)
            {
                bb.Read(ref tmp);
                vsBuf[i] = tmp;
            }
        }
        public override string ToStringCore(List<string> vs, int idx)
        {
            var s = vs[idx];
            return s == null ? "nil" : ("\"" + s + "\"");   // todo: 对内容做 json 转义?
        }

        public override void MySqlAppend(ref StringBuilder sb, bool ignoreReadOnly, List<string> vs)
        {
            var vsBuf = vs.buf;
            for (int i = 0; i < vs.dataLen; i++)
            {
                sb.Append("'");
                sb.Append(vsBuf[i].Replace("'","''"));
                sb.Append("', ");
            }
        }
    }



    public partial class ListIBBufferImpl_NullableBoolean : ListIBBufferImpl<bool?>
    {
        public override void ToBBuffer(BBuffer bb, List<bool?> vs)
        {
            var vsDataLen = vs.dataLen;
            bb.WriteLength(vsDataLen);
            if (vsDataLen == 0) return;

            bb.Reserve(bb.dataLen + vsDataLen * 2);
            var vsBuf = vs.buf;
            for (int i = 0; i < vsDataLen; i++)
            {
                bb.Write(vsBuf[i]);
            }
        }
        public override void FromBBuffer(BBuffer bb, List<bool?> vs)
        {
            int len = bb.ReadLength();
            if (bb.readLengthLimit != 0 && len > bb.readLengthLimit) throw new Exception("overflow of limit");
            vs.Resize(len);
            if (len == 0) return;

            bool? tmp = null;
            var vsBuf = vs.buf;
            for (int i = 0; i < len; i++)
            {
                bb.Read(ref tmp);
                vsBuf[i] = tmp;
            }
        }
        public override string ToStringCore(List<bool?> vs, int idx)
        {
            return vs[idx].HasValue ? vs[idx].Value.ToString() : "nil";
        }
    }
    public partial class ListIBBufferImpl_NullableSByte : ListIBBufferImpl<sbyte?>
    {
        public override void ToBBuffer(BBuffer bb, List<sbyte?> vs)
        {
            var vsDataLen = vs.dataLen;
            bb.WriteLength(vsDataLen);
            if (vsDataLen == 0) return;

            bb.Reserve(bb.dataLen + vsDataLen * 2);
            var vsBuf = vs.buf;
            for (int i = 0; i < vsDataLen; i++)
            {
                bb.Write(vsBuf[i]);
            }
        }
        public override void FromBBuffer(BBuffer bb, List<sbyte?> vs)
        {
            int len = bb.ReadLength();
            if (bb.readLengthLimit != 0 && len > bb.readLengthLimit) throw new Exception("overflow of limit");
            vs.Resize(len);
            if (len == 0) return;

            sbyte? tmp = null;
            var vsBuf = vs.buf;
            for (int i = 0; i < len; i++)
            {
                bb.Read(ref tmp);
                vsBuf[i] = tmp;
            }
        }
        public override string ToStringCore(List<sbyte?> vs, int idx)
        {
            return vs[idx].HasValue ? vs[idx].Value.ToString() : "nil";
        }
    }
    public partial class ListIBBufferImpl_NullableByte : ListIBBufferImpl<byte?>
    {
        public override void ToBBuffer(BBuffer bb, List<byte?> vs)
        {
            var vsDataLen = vs.dataLen;
            bb.WriteLength(vsDataLen);
            if (vsDataLen == 0) return;

            bb.Reserve(bb.dataLen + vsDataLen * 2);
            var vsBuf = vs.buf;
            for (int i = 0; i < vsDataLen; i++)
            {
                bb.Write(vsBuf[i]);
            }
        }
        public override void FromBBuffer(BBuffer bb, List<byte?> vs)
        {
            int len = bb.ReadLength();
            if (bb.readLengthLimit != 0 && len > bb.readLengthLimit) throw new Exception("overflow of limit");
            vs.Resize(len);
            if (len == 0) return;

            byte? tmp = null;
            var vsBuf = vs.buf;
            for (int i = 0; i < len; i++)
            {
                bb.Read(ref tmp);
                vsBuf[i] = tmp;
            }
        }
        public override string ToStringCore(List<byte?> vs, int idx)
        {
            return vs[idx].HasValue ? vs[idx].Value.ToString() : "nil";
        }
    }
    public partial class ListIBBufferImpl_NullableInt16 : ListIBBufferImpl<short?>
    {
        public override void ToBBuffer(BBuffer bb, List<short?> vs)
        {
            var vsDataLen = vs.dataLen;
            bb.WriteLength(vsDataLen);
            if (vsDataLen == 0) return;

            bb.Reserve(bb.dataLen + vsDataLen * 4);
            var vsBuf = vs.buf;
            for (int i = 0; i < vsDataLen; i++)
            {
                bb.Write(vsBuf[i]);
            }
        }
        public override void FromBBuffer(BBuffer bb, List<short?> vs)
        {
            int len = bb.ReadLength();
            if (bb.readLengthLimit != 0 && len > bb.readLengthLimit) throw new Exception("overflow of limit");
            vs.Resize(len);
            if (len == 0) return;

            short? tmp = null;
            var vsBuf = vs.buf;
            for (int i = 0; i < len; i++)
            {
                bb.Read(ref tmp);
                vsBuf[i] = tmp;
            }
        }
        public override string ToStringCore(List<short?> vs, int idx)
        {
            return vs[idx].HasValue ? vs[idx].Value.ToString() : "nil";
        }
    }
    public partial class ListIBBufferImpl_NullableUInt16 : ListIBBufferImpl<ushort?>
    {
        public override void ToBBuffer(BBuffer bb, List<ushort?> vs)
        {
            var vsDataLen = vs.dataLen;
            bb.WriteLength(vsDataLen);
            if (vsDataLen == 0) return;

            bb.Reserve(bb.dataLen + vsDataLen * 4);
            var vsBuf = vs.buf;
            for (int i = 0; i < vsDataLen; i++)
            {
                bb.Write(vsBuf[i]);
            }
        }
        public override void FromBBuffer(BBuffer bb, List<ushort?> vs)
        {
            int len = bb.ReadLength();
            if (bb.readLengthLimit != 0 && len > bb.readLengthLimit) throw new Exception("overflow of limit");
            vs.Resize(len);
            if (len == 0) return;

            ushort? tmp = null;
            var vsBuf = vs.buf;
            for (int i = 0; i < len; i++)
            {
                bb.Read(ref tmp);
                vsBuf[i] = tmp;
            }
        }
        public override string ToStringCore(List<ushort?> vs, int idx)
        {
            return vs[idx].HasValue ? vs[idx].Value.ToString() : "nil";
        }
    }
    public partial class ListIBBufferImpl_NullableInt32 : ListIBBufferImpl<int?>
    {
        public override void ToBBuffer(BBuffer bb, List<int?> vs)
        {
            var vsDataLen = vs.dataLen;
            bb.WriteLength(vsDataLen);
            if (vsDataLen == 0) return;

            bb.Reserve(bb.dataLen + vsDataLen * 6);
            var vsBuf = vs.buf;
            for (int i = 0; i < vsDataLen; i++)
            {
                bb.Write(vsBuf[i]);
            }
        }
        public override void FromBBuffer(BBuffer bb, List<int?> vs)
        {
            int len = bb.ReadLength();
            if (bb.readLengthLimit != 0 && len > bb.readLengthLimit) throw new Exception("overflow of limit");
            vs.Resize(len);
            if (len == 0) return;

            int? tmp = null;
            var vsBuf = vs.buf;
            for (int i = 0; i < len; i++)
            {
                bb.Read(ref tmp);
                vsBuf[i] = tmp;
            }
        }
        public override string ToStringCore(List<int?> vs, int idx)
        {
            return vs[idx].HasValue ? vs[idx].Value.ToString() : "nil";
        }
    }
    public partial class ListIBBufferImpl_NullableUInt32 : ListIBBufferImpl<uint?>
    {
        public override void ToBBuffer(BBuffer bb, List<uint?> vs)
        {
            var vsDataLen = vs.dataLen;
            bb.WriteLength(vsDataLen);
            if (vsDataLen == 0) return;

            bb.Reserve(bb.dataLen + vsDataLen * 6);
            var vsBuf = vs.buf;
            for (int i = 0; i < vsDataLen; i++)
            {
                bb.Write(vsBuf[i]);
            }
        }
        public override void FromBBuffer(BBuffer bb, List<uint?> vs)
        {
            int len = bb.ReadLength();
            if (bb.readLengthLimit != 0 && len > bb.readLengthLimit) throw new Exception("overflow of limit");
            vs.Resize(len);
            if (len == 0) return;

            uint? tmp = null;
            var vsBuf = vs.buf;
            for (int i = 0; i < len; i++)
            {
                bb.Read(ref tmp);
                vsBuf[i] = tmp;
            }
        }
        public override string ToStringCore(List<uint?> vs, int idx)
        {
            return vs[idx].HasValue ? vs[idx].Value.ToString() : "nil";
        }
    }
    public partial class ListIBBufferImpl_NullableInt64 : ListIBBufferImpl<long?>
    {
        public override void ToBBuffer(BBuffer bb, List<long?> vs)
        {
            var vsDataLen = vs.dataLen;
            bb.WriteLength(vsDataLen);
            if (vsDataLen == 0) return;

            bb.Reserve(bb.dataLen + vsDataLen * 10);
            var vsBuf = vs.buf;
            for (int i = 0; i < vsDataLen; i++)
            {
                bb.Write(vsBuf[i]);
            }
        }
        public override void FromBBuffer(BBuffer bb, List<long?> vs)
        {
            int len = bb.ReadLength();
            if (bb.readLengthLimit != 0 && len > bb.readLengthLimit) throw new Exception("overflow of limit");
            vs.Resize(len);
            if (len == 0) return;

            long? tmp = null;
            var vsBuf = vs.buf;
            for (int i = 0; i < len; i++)
            {
                bb.Read(ref tmp);
                vsBuf[i] = tmp;
            }
        }
        public override string ToStringCore(List<long?> vs, int idx)
        {
            return vs[idx].HasValue ? vs[idx].Value.ToString() : "nil";
        }
    }
    public partial class ListIBBufferImpl_NullableUInt64 : ListIBBufferImpl<ulong?>
    {
        public override void ToBBuffer(BBuffer bb, List<ulong?> vs)
        {
            var vsDataLen = vs.dataLen;
            bb.WriteLength(vsDataLen);
            if (vsDataLen == 0) return;

            bb.Reserve(bb.dataLen + vsDataLen * 10);
            var vsBuf = vs.buf;
            for (int i = 0; i < vsDataLen; i++)
            {
                bb.Write(vsBuf[i]);
            }
        }
        public override void FromBBuffer(BBuffer bb, List<ulong?> vs)
        {
            int len = bb.ReadLength();
            if (bb.readLengthLimit != 0 && len > bb.readLengthLimit) throw new Exception("overflow of limit");
            vs.Resize(len);
            if (len == 0) return;

            ulong? tmp = null;
            var vsBuf = vs.buf;
            for (int i = 0; i < len; i++)
            {
                bb.Read(ref tmp);
                vsBuf[i] = tmp;
            }
        }
        public override string ToStringCore(List<ulong?> vs, int idx)
        {
            return vs[idx].HasValue ? vs[idx].Value.ToString() : "nil";
        }
    }
    public partial class ListIBBufferImpl_NullableSingle : ListIBBufferImpl<float?>
    {
        public override void ToBBuffer(BBuffer bb, List<float?> vs)
        {
            var vsDataLen = vs.dataLen;
            bb.WriteLength(vsDataLen);
            if (vsDataLen == 0) return;

            bb.Reserve(bb.dataLen + vsDataLen * 10);
            var vsBuf = vs.buf;
            for (int i = 0; i < vsDataLen; i++)
            {
                bb.Write(vsBuf[i]);
            }
        }
        public override void FromBBuffer(BBuffer bb, List<float?> vs)
        {
            int len = bb.ReadLength();
            if (bb.readLengthLimit != 0 && len > bb.readLengthLimit) throw new Exception("overflow of limit");
            vs.Resize(len);
            if (len == 0) return;

            float? tmp = null;
            var vsBuf = vs.buf;
            for (int i = 0; i < len; i++)
            {
                bb.Read(ref tmp);
                vsBuf[i] = tmp;
            }
        }
        public override string ToStringCore(List<float?> vs, int idx)
        {
            return vs[idx].HasValue ? vs[idx].Value.ToString() : "nil";
        }
    }
    public partial class ListIBBufferImpl_NullableDouble : ListIBBufferImpl<double?>
    {
        public override void ToBBuffer(BBuffer bb, List<double?> vs)
        {
            var vsDataLen = vs.dataLen;
            bb.WriteLength(vsDataLen);
            if (vsDataLen == 0) return;

            bb.Reserve(bb.dataLen + vsDataLen * 10);
            var vsBuf = vs.buf;
            for (int i = 0; i < vsDataLen; i++)
            {
                bb.Write(vsBuf[i]);
            }
        }
        public override void FromBBuffer(BBuffer bb, List<double?> vs)
        {
            int len = bb.ReadLength();
            if (bb.readLengthLimit != 0 && len > bb.readLengthLimit) throw new Exception("overflow of limit");
            vs.Resize(len);
            if (len == 0) return;

            double? tmp = null;
            var vsBuf = vs.buf;
            for (int i = 0; i < len; i++)
            {
                bb.Read(ref tmp);
                vsBuf[i] = tmp;
            }
        }
        public override string ToStringCore(List<double?> vs, int idx)
        {
            return vs[idx].HasValue ? vs[idx].Value.ToString() : "nil";
        }
    }
}
