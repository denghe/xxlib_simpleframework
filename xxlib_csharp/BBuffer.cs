using System;
using System.Text;

namespace xx
{
    public interface IBBuffer
    {
        // 获取包编号( 不能用 get, 无法虚覆盖 )
        ushort GetPackageId();

        // 序列化接口之序列化数据到 bb
        void ToBBuffer(BBuffer bb);

        // 序列化接口之从 bb 还原 /  填充数据( 这个需要 try )
        void FromBBuffer(BBuffer bb);

        // todo

        //// 方便 Dump 对象数据的高速 ToString 接口1 之输出 type 和外包围用
        //void ToStringBuilder(ref StringBuilder sb);

        //// 方便 Dump 对象数据的高速 ToString 接口2 之继承的情况下填充内容用
        //void ToStringBuilderCore(ref StringBuilder sb);
    }

    // C# safe 版的 BBuffer 并不方便在 buf 前面空出一片填充包头的区域

    public class BBuffer : IBBuffer
    {
        public byte[] buf;
        public int offset, dataLen;     // 读偏移, 数据长

        public int dataLenBak;          // 用于 WritePackage 过程中计算包长
        public int offsetRoot;          // offset值写入修正
        public int readLengthLimit;     // 主用于传递给容器类进行长度合法校验

        #region write & read funcs

        #region byte
        public void Write(byte v)
        {
            if (dataLen + 1 > buf.Length)
            {
                Reserve(dataLen + 1);
            }
            buf[dataLen++] = v;
        }
        public void Read(ref byte v)
        {
            v = buf[offset++];
        }
        #endregion

        #region sbyte
        public void Write(sbyte v)
        {
            Write((byte)v);
        }
        public void Read(ref sbyte v)
        {
            v = (sbyte)buf[offset++];
        }
        #endregion

        #region ushort
        public void Write(ushort v)
        {
            if (dataLen + 2 > buf.Length)
            {
                Reserve(dataLen + 3);
            }
            Bit7Write(buf, ref dataLen, v);
        }
        public void Read(ref ushort v)
        {
            Bit7Read(ref v, buf, ref offset, dataLen);
        }
        #endregion

        #region short
        public void Write(short v)
        {
            if (dataLen + 3 > buf.Length)
            {
                Reserve(dataLen + 3);
            }
            Bit7Write(buf, ref dataLen, ZigZagEncode(v));
        }
        public void Read(ref short v)
        {
            ushort tmp = 0;
            Bit7Read(ref tmp, buf, ref offset, dataLen);
            v = ZigZagDecode(tmp);
        }
        #endregion

        #region uint
        public void Write(uint v)
        {
            if (dataLen + 5 > buf.Length)
            {
                Reserve(dataLen + 5);
            }
            Bit7Write(buf, ref dataLen, v);
        }
        public void Read(ref uint v)
        {
            Bit7Read(ref v, buf, ref offset, dataLen);
        }
        #endregion

        #region int
        public void Write(int v)
        {
            if (dataLen + 5 > buf.Length)
            {
                Reserve(dataLen + 5);
            }
            Bit7Write(buf, ref dataLen, ZigZagEncode(v));
        }
        public void Read(ref int v)
        {
            uint tmp = 0;
            Bit7Read(ref tmp, buf, ref offset, dataLen);
            v = ZigZagDecode(tmp);
        }
        #endregion

        #region ulong
        public void Write(ulong v)
        {
            if (dataLen + 9 > buf.Length)
            {
                Reserve(dataLen + 9);
            }
            Bit7Write(buf, ref dataLen, v);
        }
        public void Read(ref ulong v)
        {
            Bit7Read(ref v, buf, ref offset, dataLen);
        }
        #endregion

        #region long
        public void Write(long v)
        {
            if (dataLen + 9 > buf.Length)
            {
                Reserve(dataLen + 9);
            }
            Bit7Write(buf, ref dataLen, ZigZagEncode(v));
        }
        public void Read(ref long v)
        {
            ulong tmp = 0;
            Bit7Read(ref tmp, buf, ref offset, dataLen);
            v = ZigZagDecode(tmp);
        }
        #endregion

        #region float
        public void WriteDirect(float v)
        {
            var tmpbuf = BitConverter.GetBytes(v);
            buf[dataLen + 0] = tmpbuf[0];
            buf[dataLen + 1] = tmpbuf[1];
            buf[dataLen + 2] = tmpbuf[2];
            buf[dataLen + 3] = tmpbuf[3];
            dataLen += 4;
        }
        public void Write(float v)
        {
            if (dataLen + 4 > buf.Length)
            {
                Reserve(dataLen + 4);
            }
            WriteDirect(v);
        }
        public void Read(ref float v)
        {
            v = BitConverter.ToSingle(buf, offset);
            offset += 4;
        }
        #endregion

        #region double
        public void WriteDirect(double v)
        {
            if (v == 0)
            {
                buf[dataLen++] = 0;
            }
            else
            {
                if (double.IsNaN(v))
                {
                    buf[dataLen++] = 1;
                }
                else if (double.IsNegativeInfinity(v))
                {
                    buf[dataLen++] = 2;
                }
                else if (double.IsPositiveInfinity(v))
                {
                    buf[dataLen++] = 3;
                }
                else
                {
                    int intv = (int)v;
                    if (v == (double)intv)
                    {
                        buf[dataLen++] = 4;
                        Bit7Write(buf, ref dataLen, ZigZagEncode(intv));    // Write(intv);
                    }
                    else
                    {
                        buf[dataLen++] = 5;
                        var tmpbuf = BitConverter.GetBytes(v);
                        Array.Copy(tmpbuf, 0, buf, dataLen, 8);
                        dataLen += 8;
                    }
                }
            }
        }
        public void Write(double v)
        {
            if (dataLen + 9 > buf.Length)
            {
                Reserve(dataLen + 9);
            }
            WriteDirect(v);
        }
        public void Read(ref double v)
        {
            switch (buf[offset++])
            {
                case 0:
                    v = 0;
                    break;
                case 1:
                    v = double.NaN;
                    break;
                case 2:
                    v = double.NegativeInfinity;
                    break;
                case 3:
                    v = double.PositiveInfinity;
                    break;
                case 4:
                    uint tmp = 0;
                    Bit7Read(ref tmp, buf, ref offset, dataLen);
                    v = ZigZagDecode(tmp);
                    break;
                case 5:
                    v = BitConverter.ToDouble(buf, offset);
                    offset += 8;
                    break;
                default:
                    throw new NotSupportedException();
            }
        }
        #endregion

        #region bool
        public void Write(bool v)
        {
            Write(v ? (byte)1 : (byte)0);
        }
        public void Read(ref bool v)
        {
            v = buf[offset++] == 1;
        }
        #endregion

        #region string
        public void WriteCore(string v)
        {
            var sbuf = Encoding.UTF8.GetBytes(v);
            int sbufLen = sbuf.Length;
            if (dataLen + 5 + sbufLen > buf.Length)
            {
                Reserve(dataLen + 5 + sbufLen);
            }
            Bit7Write(buf, ref dataLen, (uint)sbufLen);

            Array.Copy(sbuf, 0, buf, dataLen, sbufLen);
            dataLen += sbufLen;
        }
        public void Write(string v)
        {
            // 和 C++ 那边保持一致, typeId 打头. 空就是写入 0
            if (v == null)
            {
                Write((ushort)0);
                return;
            }
            else
            {
                Write((ushort)1);
            }
            if (ptrStore != null)
            {
                var rtv = ptrStore.Add(v, (uint)(dataLen - offsetRoot));       // 试将 v 和 相对offset 放入字典, 得到下标和是否成功
                Write(ptrStore.ValueAt(rtv.index));                             // 取 offset ( 不管是否成功 )
                if (rtv.success) WriteCore(v);
            }
            else
            {
                WriteCore(v);
            }
        }
        public void ReadCore(ref string v)
        {
            uint len = 0;
            Read(ref len);
            if (readLengthLimit != 0 && len > readLengthLimit) throw new Exception("overflow of limit");
            if (len == 0)
            {
                v = "";
            }
            else
            {
                v = Encoding.UTF8.GetString(buf, offset, (int)len);
                offset += (int)len;
            }
        }
        public void Read(ref string v)
        {
            var typeId = buf[offset++];
            if (typeId == 0)
            {
                v = null;
                return;
            }
            else if (typeId != 1)
            {
                throw new Exception("wrong typeId");
            }
            if (idxStore != null)
            {
                uint ptr_offset = 0, bb_offset_bak = (uint)this.offset - (uint)offsetRoot;
                Read(ref ptr_offset);
                if (ptr_offset == bb_offset_bak)
                {
                    ReadCore(ref v);
                    idxStore.Add(ptr_offset, v);
                }
                else
                {
                    int idx = idxStore.Find(ptr_offset);
                    if (idx == -1) throw new Exception("ptr_offset is not found");
                    v = (string)idxStore.ValueAt(idx);
                    System.Diagnostics.Debug.Assert(v != null);
                }
            }
            else
            {
                ReadCore(ref v);
            }
        }
        #endregion

        #region DateTime
        public void WriteDirect(DateTime v)
        {
            short year = (short)v.Year;
            buf[dataLen + 0] = (byte)year;
            buf[dataLen + 1] = (byte)(year >> 8);
            buf[dataLen + 2] = (byte)v.Month;
            buf[dataLen + 3] = (byte)v.Day;
            buf[dataLen + 4] = (byte)v.Hour;
            buf[dataLen + 5] = (byte)v.Minute;
            buf[dataLen + 6] = (byte)v.Second;
            buf[dataLen + 7] = (byte)0;
            dataLen += 8;
        }
        public void Write(DateTime v)
        {
            if (dataLen + 8 > buf.Length)
            {
                Reserve(dataLen + 8);
            }
            WriteDirect(v);
        }
        public void Read(ref DateTime v)
        {
            v = new DateTime(buf[offset + 0] + (buf[offset + 1] << 8),
                buf[offset + 2],
                buf[offset + 3],
                buf[offset + 4],
                buf[offset + 5],
                buf[offset + 6]);
            offset += 8;
        }
        #endregion

        #region T : IBBuffer

        // 不带引用的类编码规则: 类型编号 + 类数据
        // 类型编号兼 null 表达, 0 代表 null

        // 引用类 编码规则: 类型编号 + offset数据 + [类数据]
        // 类型编号兼 null 表达, 0 代表 null
        // 如果 offset数据 == bb.offset 则表示当前类为首次序列化, 后面会跟 类数据. 否则就是引用

        // 当 objs / offsets 不为空时, 启用引用读写
        Dict<object, uint> ptrStore = null;
        Dict<uint, object> idxStore = null;

        // 写前准备( 创建指针字典, 存起始 offset )
        public void BeginWrite()
        {
            if (ptrStore == null) ptrStore = new Dict<object, uint>();
            else ptrStore.Clear();
            offsetRoot = dataLen;
        }
        // 写后收拾( 回收字典 )
        public void EndWrite()
        {
            ptrStore.Clear();
        }

        // 读前准备( 创建指针字典, 并不重置 读长度限制 )
        public void BeginRead()
        {
            if (idxStore == null) idxStore = new Dict<uint, object>();
            else idxStore.Clear();
            offsetRoot = offset;
        }
        // 读后收拾( 回收字典 )
        public void EndRead()
        {
            idxStore.Clear();
        }

        // 一波流, 写入一套引用类的根
        public void WriteRoot<T>(T v) where T : IBBuffer
        {
            BeginWrite();
            Write(v);
            EndWrite();
        }

        // 一波流, 读出一套引用类
        public void ReadRoot<T>(ref T v) where T : IBBuffer
        {
            BeginRead();
            Read(ref v);
            EndRead();
        }

        public void Write<T>(T v) where T : IBBuffer
        {
            if (v == null)
            {
                Write((ushort)0);
            }
            else
            {
                System.Diagnostics.Debug.Assert(v.GetPackageId() != ushort.MaxValue);
                Write(v.GetPackageId());
                if (ptrStore != null)
                {
                    var rtv = ptrStore.Add(v, (uint)(dataLen - offsetRoot));        // 试将 v 和 相对offset 放入字典, 得到下标和是否成功
                    Write(ptrStore.ValueAt(rtv.index));                             // 取 offset ( 不管是否成功 )
                    if (!rtv.success) return;                                       // 如果首次出现就序列化类本体
                }
                v.ToBBuffer(this);
            }
        }

        public void Read<T>(ref T v) where T : IBBuffer
        {
            var typeId = (ushort)0;
            Read(ref typeId);
            if (typeId == 0)
            {
                v = default(T);
                return;
            }
            if (idxStore != null)
            {
                uint ptr_offset = 0, bb_offset_bak = (uint)this.offset - (uint)offsetRoot;
                Read(ref ptr_offset);
                if (ptr_offset == bb_offset_bak)
                {
                    v = (T)CreateByTypeId(typeId);
                    System.Diagnostics.Debug.Assert(v != null);
                    idxStore.Add(ptr_offset, v);
                    v.FromBBuffer(this);
                }
                else
                {
                    int idx = idxStore.Find(ptr_offset);
                    if (idx == -1) throw new Exception("ptr_offset is not found");
                    v = (T)idxStore.ValueAt(idx);
                    System.Diagnostics.Debug.Assert(v != null);
                }
            }
            else
            {
                v = (T)CreateByTypeId(typeId);
                System.Diagnostics.Debug.Assert(v != null);
                v.FromBBuffer(this);
            }
        }

        /// <summary>
        /// 自定义序列化函数的类型
        /// </summary>
        /// <param name="bb">当前 BBuffer</param>
        /// <param name="c">字段所在容器类/结构体</param>
        /// <param name="fn">字段名</param>
        public delegate void CustomWriteDelegate(BBuffer bb, object c, string fn);

        /// <summary>
        /// 自定义序列化函数支持. 用前必须赋值. 用后建议清除.
        /// </summary>
        public CustomWriteDelegate CustomWrite;


        // 对于结构体来讲, 无法提供这样的函数. Write 会造成复制, Read 在结构体转换后 只能填充到临时变量. 只能靠生成器识别并生成直接调其成员函数的代码.
        //public void WriteStruct<T>(T v) where T : /*struct, */IBBuffer
        //{
        //    v.ToBBuffer(this);
        //}
        //public void ReadStruct<T>(ref T v)// where T : /*struct, */IBBuffer
        //{
        //    ((IBBuffer)v).FromBBuffer(this);
        //}



        #endregion

        #region Package

        // 开始写一个包( 特指 2 字节长度头的, 保留出包头区域 )
        public void BeginWritePackage()
        {
            dataLenBak = dataLen;
            Reserve(dataLen + 2);
            dataLen += 2;
        }

        // 结束写一个包, 返回长度是否在包头表达范围内( 如果 true 则会填充包头, false 则回滚长度 )
        public bool EndWritePackage()
        {
            var pkgLen = dataLen - dataLenBak - 2;
            if (pkgLen > ushort.MaxValue)
            {
                dataLen = dataLenBak;
                return false;
            }
            buf[dataLenBak] = (byte)(pkgLen);
            buf[dataLenBak + 1] = (byte)(pkgLen >> 8);
            return true;
        }

        #endregion

        #region byte[]

        /// <summary>
        /// 直接追加写入一段2进制数据
        /// </summary>
        public void WriteBuf(byte[] buf, int offset, int dataLen)
        {
            Reserve(offset + dataLen);
            Array.Copy(buf, offset, this.buf, this.dataLen, dataLen);
            this.dataLen += dataLen;
        }
        #endregion

        #region ToPackages

        /// <summary>
        /// 尝试一次性反序列化一到多个包, 将结果填充到 outPkgs, 返回包个数. 不需要 try. 返回 0 或负数都有问题
        /// </summary>
        public int ReadPackages(ref List<IBBuffer> outPkgs)
        {
            outPkgs.Clear();
            while (offset < dataLen)
            {
                IBBuffer ibb = null;
                try
                {
                    ReadRoot(ref ibb);
                }
                catch (Exception)
                {
                    return -1;
                }
                if (ibb == null) return -2;
                outPkgs.Add(ibb);
            }
            return outPkgs.dataLen;
        }

        #endregion

        #endregion

        #region misc funcs

        public BBuffer()
        {
            Assign(new byte[32], 0);
        }
        public BBuffer(int capacity)
        {
            Assign(new byte[capacity < 32 ? 32 : Round2n((uint)capacity)], 0);
        }

        public BBuffer(byte[] buf, int dataLen = 0)
        {
            Assign(buf, dataLen);
        }

        public static implicit operator BBuffer(byte[] buf)
        {
            return new BBuffer(buf, buf.Length);
        }

        public void Assign(byte[] buf, int dataLen = 0)
        {
            offset = 0;
            this.buf = buf;
            this.dataLen = dataLen;
        }

        public void Clear()
        {
            dataLen = offset = 0;
        }

        public bool Empty()
        {
            return dataLen == 0;
        }

        public bool Full()
        {
            return dataLen == offset;
        }

        public byte[] Collapse()
        {
            Array.Resize(ref buf, dataLen);
            return buf;
        }

        public byte[] DumpData()
        {
            var rtv = new byte[dataLen];
            Array.Copy(buf, rtv, dataLen);
            return rtv;
        }

        public void Ensure(int len)
        {
            if (dataLen + len > buf.Length)
            {
                Reserve(dataLen + len);
            }
        }

        public void Reserve(int capacity)
        {
            if (capacity <= buf.Length) return;
            Array.Resize(ref buf, (int)Round2n((uint)(capacity * 2)));
        }

        public byte At(int idx)
        {
            return buf[idx];
        }

        public void Dump(ref StringBuilder s)
        {
            Dump(ref s, buf, dataLen);
        }

        public void Dump()
        {
            var s = new StringBuilder();
            Dump(ref s);
            Console.Write(s);
        }

        public override string ToString()
        {
            var s = new StringBuilder();
            s.Append("{ \"len\" : "+ dataLen + ", \"offset\" : "+ offset + ", \"data\" : [");
            for (int i = 0; i < dataLen; ++i)
            {
                s.Append(i > 0 ? ", " : " ");
                s.Append(buf[i]);
            }
            s.Append(dataLen > 0 ? " ]" : "]");
            s.Append(" }");
            return s.ToString();
        }

        public void WriteLength(int len)
        {
            Write((uint)len);
        }
        public int ReadLength()
        {
            uint len = 0;
            Bit7Read(ref len, buf, ref offset, dataLen);
            return (int)len;
        }
        public int ReadLength(int minLen, int maxLen)
        {
            var len = ReadLength();
            if (len < minLen || (maxLen > 0 && len > maxLen))
            {
                throw new OverflowException();
            }
            return len;
        }
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


        // interface impl

        public ushort GetPackageId() { return TypeIdMaps<BBuffer>.typeId; }

        public void ToBBuffer(BBuffer bb)
        {
            var len = dataLen;
            bb.WriteLength(len);
            if (len == 0) return;
            bb.Ensure(len);
            Array.Copy(buf, 0, bb.buf, bb.dataLen, len);
            bb.dataLen += len;
        }

        public void FromBBuffer(BBuffer bb)
        {
            int len = bb.ReadLength();
            if (bb.readLengthLimit != 0 && len > bb.readLengthLimit) throw new Exception("overflow of limit");
            Resize(len);
            offset = 0;
            if (len == 0) return;
            Array.Copy(bb.buf, bb.offset, buf, 0, len);
            bb.offset += len;
        }

        #endregion

        #region static utils

        /// <summary>
        /// 统计有多少个 1
        /// </summary>
        public static int Popcnt(uint x)
        {
            x -= ((x >> 1) & 0x55555555);
            x = (((x >> 2) & 0x33333333) + (x & 0x33333333));
            x = (((x >> 4) + x) & 0x0f0f0f0f);
            x += (x >> 8);
            x += (x >> 16);
            return (int)(x & 0x0000003f);
        }

        /// <summary>
        /// 统计高位有多少个 0
        /// </summary>
        public static int Clz(uint x)
        {
            x |= (x >> 1);
            x |= (x >> 2);
            x |= (x >> 4);
            x |= (x >> 8);
            x |= (x >> 16);
            return (int)(32 - Popcnt(x));
        }

        /// <summary>
        /// 求大于 v 的 2^n 值
        /// </summary>
        public static uint Round2n(uint v)
        {
            int bits = 31 - Clz(v);
            var rtv = (uint)(1u << bits);
            if (rtv == v) return v;
            return rtv << 1;
        }

        // 负转正：利用单数来存负数，双数来存正数
        // 等效代码： if( v < 0 ) return -v * 2 - 1; else return v * 2;

        public static ushort ZigZagEncode(short v) { return (ushort)((v << 1) ^ (v >> 15)); }

        public static uint ZigZagEncode(int v) { return (uint)((v << 1) ^ (v >> 31)); }

        public static ulong ZigZagEncode(long v) { return (ulong)((v << 1) ^ (v >> 63)); }

        // 等效代码： if( (v & 1) > 0 ) return -(v + 1) / 2; else return v / 2;

        public static short ZigZagDecode(ushort v) { return (short)((short)(v >> 1) ^ (-(short)(v & 1))); }

        public static int ZigZagDecode(uint v) { return (int)(v >> 1) ^ (-(int)(v & 1)); }

        public static long ZigZagDecode(ulong v) { return (long)(v >> 1) ^ (-(long)(v & 1)); }

        // need ensure 3
        public static void Bit7Write(byte[] buf, ref int offset, ushort v)
        {
            Lab1:
            byte b7 = (byte)v;
            v >>= 7;
            if (v > 0)
            {
                buf[offset++] = (byte)(b7 | (byte)0x80);
                goto Lab1;
            }
            buf[offset++] = b7;
        }

        // need ensure 5
        public static void Bit7Write(byte[] buf, ref int offset, uint v)
        {
            Lab1:
            byte b7 = (byte)v;
            v >>= 7;
            if (v > 0)
            {
                buf[offset++] = (byte)(b7 | (byte)0x80);
                goto Lab1;
            }
            buf[offset++] = b7;
        }

        // need ensure 9
        public static void Bit7Write(byte[] buf, ref int offset, ulong v)
        {
            var idx8 = offset + 8;
            Lab1:
            byte b7 = (byte)v;
            v >>= 7;
            if (v > 0)
            {
                buf[offset++] = (byte)(b7 | (byte)0x80);
                if (offset == idx8)
                    buf[offset++] = (byte)v;
                else
                    goto Lab1;
            }
            else
            {
                buf[offset++] = b7;
            }
        }

        public static void Bit7Read(ref ulong v, byte[] buf, ref int offset, int dataLen)
        {
            var idx8 = offset + 8;
            v = 0;
            int lshift = 0;
            Lab1:
            ulong b7 = buf[offset++];
            if (b7 > 0x7F)
            {
                if (offset < idx8)
                {
                    v |= (b7 & 0x7F) << lshift;
                    lshift += 7;
                    goto Lab1;
                }
                else
                    v |= ((b7 & 0x7F) << lshift) | ((ulong)buf[offset++] << 28 << 28);
            }
            else
                v |= b7 << lshift;
        }

        public static void Bit7Read(ref uint v, byte[] buf, ref int offset, int dataLen)
        {
            var idx5 = offset + 5;
            int lshift = 0;
            v = 0;
            Lab1:
            uint b7 = buf[offset++];
            if (b7 > 0x7F)
            {
                if (offset == idx5)
                    throw new OverflowException();
                v |= (b7 & 0x7F) << lshift;
                lshift += 7;
                goto Lab1;
            }
            else
            {
                if (offset == idx5 && b7 > 15)
                    throw new OverflowException();
                else
                    v |= b7 << lshift;
            }
        }

        public static void Bit7Read(ref ushort v, byte[] buf, ref int offset, int dataLen)
        {
            var idx3 = offset + 3;
            int lshift = 0;
            uint vv = 0;
            Lab1:
            uint b7 = buf[offset++];
            if (b7 > 0x7F)
            {
                if (offset == idx3)
                    throw new OverflowException();
                vv |= (b7 & 0x7F) << lshift;
                lshift += 7;
                goto Lab1;
            }
            else
            {
                if (offset == idx3 && b7 > 3)
                    throw new OverflowException();
                else
                    vv |= b7 << lshift;
            }
            v = (ushort)vv;
        }

        public static void DumpAscII(ref StringBuilder s, byte[] buf, int offset, int len)
        {
            for (int i = offset; i < offset + len; ++i)
            {
                byte c = buf[i];
                if (c < 32 || c > 126)
                    s.Append('.');
                else
                    s.Append((char)c);
            }
        }

        // write buf's binary dump text to s
        public static void Dump(ref StringBuilder s, byte[] buf, int len = 0)
        {
            if (buf == null || buf.Length == 0)
                return;
            if (len == 0)
                len = buf.Length;
            s.Append("--------  0  1  2  3 | 4  5  6  7 | 8  9  A  B | C  D  E  F");
            s.Append("   dataLen = " + len);
            int i = 0;
            for (; i < len; ++i)
            {
                if ((i % 16) == 0)
                {
                    if (i > 0)
                    {           // refput ascii to the end of the line
                        s.Append("  ");
                        DumpAscII(ref s, buf, i - 16, 16);
                    }
                    s.Append('\n');
                    s.Append(i.ToString("x8"));
                    s.Append("  ");
                }
                else if ((i > 0 && (i % 4 == 0)))
                {
                    s.Append("  ");
                }
                else
                    s.Append(' ');
                s.Append(BitConverter.ToString(buf, i, 1));
            }
            int left = i % 16;
            if (left == 0)
            {
                left = 16;
            }
            if (left > 0)
            {
                len = len + 16 - left;
                for (; i < len; ++i)
                {
                    if (i > 0 && (i % 4 == 0))
                        s.Append("  ");
                    else
                        s.Append(' ');
                    s.Append("  ");
                }
                s.Append("  ");
                DumpAscII(ref s, buf, i - 16, left);
            }
            s.Append('\n');
        }

        public static string Dump(byte[] buf, int len = 0)
        {
            var sb = new StringBuilder();
            Dump(ref sb, buf, len);
            return sb.ToString();
        }

        #endregion

        #region type id creator mappings

        public delegate IBBuffer TypeIdCreatorFunc();
        public static TypeIdCreatorFunc[] typeIdCreatorMappings = new TypeIdCreatorFunc[ushort.MaxValue];
        public static Dict<Type, ushort> typeTypeIdMappings = new Dict<Type, ushort>();

        public static void Register<T>(ushort typeId) where T : IBBuffer, new()
        {
            var r = typeTypeIdMappings.Add(typeof(T), typeId);
            System.Diagnostics.Debug.Assert(r.success || typeTypeIdMappings.ValueAt(r.index) == typeId);
            TypeIdMaps<T>.typeId = typeId;
            typeIdCreatorMappings[typeId] = () => { return new T(); };
        }

        public static IBBuffer CreateByTypeId(ushort typeId)
        {
            var f = typeIdCreatorMappings[typeId];
            if (f != null) return f();
            return null;
        }

        public static Action<IBBuffer>[] handlers;  // 这个需要外部创建

        public static void RegisterHandler<T>(Action<IBBuffer> a)
        {
            handlers[TypeIdMaps<T>.typeId] = a;
        }

        public static void ClearHandlers()
        {
            Array.Clear(handlers, 0, handlers.Length);
        }

        public static List<IBBuffer> ibbs = new List<IBBuffer>();
        public static bool Handle(BBuffer bb)
        {
            // 先一次全解出来, 如果中途解失败就认为整个失败
            ibbs.Clear();
            try                                                         // 大 try
            {
                while (bb.offset < bb.dataLen)
                {
                    IBBuffer ibb = null;
                    bb.ReadRoot(ref ibb);                                // 这里可能 throw
                    if (ibb == null) throw new ArgumentNullException();
                    if (handlers[ibb.GetPackageId()] != null)           // 这里可能越界, handlers 也可能为空
                    {
                        ibbs.Add(ibb);
                    }
                }
                if (ibbs.dataLen == 0) return false;

                for (int i = 0; i < ibbs.dataLen; i++)
                {
                    var ibb = ibbs[i];
                    var pkgId = ibb.GetPackageId();
                    handlers[pkgId](ibb);
                }
                return true;
            }
            catch (Exception)
            {
                // todo: log?
            }
            return false;
        }

        #endregion
    }
}
