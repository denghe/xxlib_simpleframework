using System;

public static class Program
{
    static void Main(string[] args)
    {
        PKG.AllTypes.Register();

        var f = new PKG.Foo();
        xx.Ref<PKG.Foo> refFoo = f;
        Console.WriteLine(refFoo);

        f.Release();
        Console.WriteLine(refFoo);

        //var bb = new xx.BBuffer();
        //{
        //    var f = new PKG.Foo();
        //    var f2 = new PKG.Foo();
        //    f.refFoo.pointer = f2;
        //    f2.refFoo.pointer = f;
        //    Console.WriteLine(f);
        //    bb.WriteRoot(f);
        //}
        //Console.WriteLine(bb);
        //{
        //    xx.IObject o = null;
        //    bb.ReadRoot(ref o);
        //    Console.WriteLine(o);
        //}
    }
}
