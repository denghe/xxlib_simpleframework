using System;

public static class Program
{
    static void Main(string[] args)
    {
        xx.BBuffer.RegisterInternals();
        PKG.AllTypes.Register();

        var f = new PKG.Foo();
        Console.WriteLine(f);

        f.id = 1;
        f.age = 123;
        f.floats = new xx.List<float?>();
        f.floats.Add(1.2f);
        f.floats.Add(null);
        f.floats.Add(3.4f);

        var bb = new xx.BBuffer();
        bb.WriteRoot(f);
        Console.WriteLine(bb);

        PKG.Foo f2 = null;
        bb.ReadRoot(ref f2);
        Console.WriteLine(f2);
    }
}
