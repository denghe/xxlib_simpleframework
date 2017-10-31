using System;
using xx;
namespace PKG2
{
    /// <summary>
    /// 测试一下各种数据类型
    /// </summary>
    public partial class 基类 : IBBuffer
    {
        public bool 不淋 = false;
        public byte 白特;
        public sbyte 撕白特;
        public short 吸哦特;
        public ushort 又吸哦特;
        public int 硬特;
        public uint 又硬特;
        public long 浪;
        public ulong 又浪;
        public float 扶裸特;
        public double 大波;
        public string 湿最硬;
        public BBuffer 屄拔扶儿;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<基类>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
            bb.Write(this.不淋);
            bb.Write(this.白特);
            bb.Write(this.撕白特);
            bb.Write(this.吸哦特);
            bb.Write(this.又吸哦特);
            bb.Write(this.硬特);
            bb.Write(this.又硬特);
            bb.Write(this.浪);
            bb.Write(this.又浪);
            bb.Write(this.扶裸特);
            bb.Write(this.大波);
            bb.Write(this.湿最硬);
            bb.Write(this.屄拔扶儿);
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
            bb.Read(ref this.不淋);
            bb.Read(ref this.白特);
            bb.Read(ref this.撕白特);
            bb.Read(ref this.吸哦特);
            bb.Read(ref this.又吸哦特);
            bb.Read(ref this.硬特);
            bb.Read(ref this.又硬特);
            bb.Read(ref this.浪);
            bb.Read(ref this.又浪);
            bb.Read(ref this.扶裸特);
            bb.Read(ref this.大波);
            bb.readLengthLimit = 0;
            bb.Read(ref this.湿最硬);
            bb.readLengthLimit = 0;
            bb.Read(ref this.屄拔扶儿);
        }

    }
    public partial class 派生类 : 基类
    {
        public List<byte> 立丝特白特;
        public List<sbyte> 立丝特撕白特;
        public List<short> 立丝特吸哦特;
        public List<ushort> 立丝特又吸哦特;
        public List<int> 立丝特硬特;
        public List<uint> 立丝特又硬特;
        public List<long> 立丝特浪;
        public List<ulong> 立丝特又浪;
        public List<float> 立丝特扶裸特;
        public List<double> 立丝特大波;
        public List<string> 立丝特湿最硬;
        public List<BBuffer> 立丝特屄拔扶儿;
        /// <summary>
        /// 可以放入 基类 或 派生类
        /// </summary>
        public List<基类> 立丝特基类;

        public override ushort GetPackageId()
        {
            return TypeIdMaps<派生类>.typeId;
        }

        public override void ToBBuffer(BBuffer bb)
        {
            base.ToBBuffer(bb);
            bb.Write(this.立丝特白特);
            bb.Write(this.立丝特撕白特);
            bb.Write(this.立丝特吸哦特);
            bb.Write(this.立丝特又吸哦特);
            bb.Write(this.立丝特硬特);
            bb.Write(this.立丝特又硬特);
            bb.Write(this.立丝特浪);
            bb.Write(this.立丝特又浪);
            bb.Write(this.立丝特扶裸特);
            bb.Write(this.立丝特大波);
            bb.Write(this.立丝特湿最硬);
            bb.Write(this.立丝特屄拔扶儿);
            bb.Write(this.立丝特基类);
        }

        public override void FromBBuffer(BBuffer bb)
        {
            base.FromBBuffer(bb);
            bb.readLengthLimit = 0;
            bb.Read(ref this.立丝特白特);
            bb.readLengthLimit = 0;
            bb.Read(ref this.立丝特撕白特);
            bb.readLengthLimit = 0;
            bb.Read(ref this.立丝特吸哦特);
            bb.readLengthLimit = 0;
            bb.Read(ref this.立丝特又吸哦特);
            bb.readLengthLimit = 0;
            bb.Read(ref this.立丝特硬特);
            bb.readLengthLimit = 0;
            bb.Read(ref this.立丝特又硬特);
            bb.readLengthLimit = 0;
            bb.Read(ref this.立丝特浪);
            bb.readLengthLimit = 0;
            bb.Read(ref this.立丝特又浪);
            bb.readLengthLimit = 0;
            bb.Read(ref this.立丝特扶裸特);
            bb.readLengthLimit = 0;
            bb.Read(ref this.立丝特大波);
            bb.readLengthLimit = 0;
            bb.Read(ref this.立丝特湿最硬);
            bb.readLengthLimit = 0;
            bb.Read(ref this.立丝特屄拔扶儿);
            bb.readLengthLimit = 0;
            bb.Read(ref this.立丝特基类);
        }

    }
    public static class AllTypes
    {
        public static void Register()
        {
            // BBuffer.Register<string>(1);
            BBuffer.Register<BBuffer>(2);
            BBuffer.Register<基类>(3);
            BBuffer.Register<派生类>(4);
            BBuffer.Register<List<byte>>(5);
            BBuffer.Register<List<sbyte>>(6);
            BBuffer.Register<List<short>>(7);
            BBuffer.Register<List<ushort>>(8);
            BBuffer.Register<List<int>>(9);
            BBuffer.Register<List<uint>>(10);
            BBuffer.Register<List<long>>(11);
            BBuffer.Register<List<ulong>>(12);
            BBuffer.Register<List<float>>(13);
            BBuffer.Register<List<double>>(14);
            BBuffer.Register<List<string>>(15);
            BBuffer.Register<List<BBuffer>>(16);
            BBuffer.Register<List<基类>>(17);

            BBuffer.handlers = new Action<IBBuffer>[17];
        }
    }
}
