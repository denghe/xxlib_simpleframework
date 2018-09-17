﻿using System;

public static class Program
{
    static void Main(string[] args)
    {
        PKG.AllTypes.Register();

        var f = new PKG.Foo();
        //f.foo.pointer = f;
        //f.foos = new xx.List<xx.Ref<PKG.Foo>>();
        //f.foos.Add(new xx.Ref<PKG.Foo> { pointer = f });
        Console.WriteLine(f);



        var bb = new xx.BBuffer();
        bb.WriteRoot(f);
        Console.WriteLine(bb);

        PKG.Foo f2 = null;
        bb.ReadRoot(ref f2);
        Console.WriteLine(f2);

        f2.Release();
        Console.WriteLine(f2);
    }
}
