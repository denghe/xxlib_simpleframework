using System;
using System.Diagnostics;

public static class Program
{
    static void Main(string[] args)
    {

        Console.WriteLine("TestC#");
        PKG2.AllTypes.Register();
        var bb = new xx.BBuffer();

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

        bb.WriteRoot(o);
        Console.WriteLine(bb);
    }
}

