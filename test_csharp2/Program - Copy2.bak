using System;
using System.Diagnostics;

public static class Program
{
    static void Main(string[] args)
    {
        Console.WriteLine("TestC#");
        PKG2.AllTypes.Register();
        var bb = new xx.BBuffer();
        var d1 = new PKG2.Derive1
        {
            i1 = 1,
            i2 = 2,
            d1 = 1,
            d2 = 2,
            d3 = 3
        };
        bb.WriteRoot(d1);
        Console.WriteLine(bb);

        if (bb.SeekToFirstField())
        {
            int i1 = 0, i2 = 0;
            bb.Read(ref i1);
            bb.Read(ref i2);
            Console.WriteLine(i1 + ", " + i2);
        }
        return;











        var o = new PKG2.基类();
        o.不淋 = true; //Boolean
        o.白特 = 1; //Byte
        o.撕白特 = -2; //SByte
        o.吸哦特 = -3; //Int16
        o.又吸哦特 = 4; //UInt16
        o.硬特 = -5; //Int32
        o.又硬特 = 6; //UInt32
        o.浪 = -7; //Int64
        o.又浪 = 8; //UInt64
        o.扶裸特 = 9.1f; //Single
        o.大波 = 10.2; //Double
        o.湿最硬 = "这是个串儿"; // String
        o.屄拔扶儿 = new xx.BBuffer(); // BBuffer
        o.屄拔扶儿.Write((byte)1);
        o.屄拔扶儿.Write((byte)2);
        o.屄拔扶儿.Write((byte)3);

        var p = new PKG2.派生类();
        p.立丝特基类 = new xx.List<PKG2.基类>();
        p.立丝特基类.Add(p);
        p.立丝特基类.Add(o);
        p.立丝特屄拔扶儿 = new xx.List<xx.BBuffer>();
        p.立丝特屄拔扶儿.Add(new xx.BBuffer());
        p.立丝特白特 = new xx.List<byte>();
        p.立丝特白特.Add(1);
        p.立丝特白特.Add(2);
        p.立丝特白特.Add(3);
        bb.Clear();
        bb.WriteRoot(p);
        Console.WriteLine(bb);

        //bb.WriteRoot(o);
        //Console.WriteLine(bb);
    }
}

