using System;
using System.Diagnostics;

public class Foo
{
    public int id;
    public string name;
    public string phone;
    public string token;
    public override string ToString()
    {
        return "foo: id = " + id + ", name = `" + name + "`" + ", phone = `" + phone + "`" + ", token = `" + token + "`";
    }
}

public static class Program
{
    static void Main(string[] args)
    {
        var sw = Stopwatch.StartNew();
        var bb = new xx.BBuffer();
	    int n = 1000000;
        var f = 1.23f;
        for (int j = 0; j < n; j++)
        {
            bb.Clear();
            for (int i = 0; i < 1000; i++)
            {
                bb.Write(f);
            }
        }
        Console.WriteLine((double)sw.ElapsedMilliseconds * 1000000 / n); 
        return;

        //var fd = new xx.DictEx<Foo, int, string>();
        //var f1 = new Foo { id = 1, name = "a", phone = "123", token = Guid.NewGuid().ToString() };
        //Console.WriteLine(fd.Add(f1, f1.id, f1.name));  // true
        //var f2 = new Foo { id = 2, name = "b", phone = "456", token = Guid.NewGuid().ToString() };
        //Console.WriteLine(fd.Add(f2, f2.id, f2.name));  // true
        //var f3 = new Foo { id = 3, name = "c", phone = "789", token = Guid.NewGuid().ToString() };
        //Console.WriteLine(fd.Add(f3, f3.id, f3.name));  // true

        //Console.WriteLine(fd.Add(f1, f1.id, f1.name));  // false
        //Console.WriteLine(fd.Add(f3, f3.id, f3.name));  // false

        //Console.WriteLine(fd.Exists0(1));       // true
        //Console.WriteLine(fd.Exists1("b"));     // true
        //Console.WriteLine(fd.Exists0(3));       // true
        //Console.WriteLine(fd.Exists1("d"));     // false

        //fd.Remove1("a");
        //fd.Remove0(2);
        //Console.WriteLine(fd.Exists0(1));       // false
        //Console.WriteLine(fd.Exists1("b"));     // false

        //fd.ForEach((i, f) => { Console.WriteLine("key0 = " + fd.KeyAt0(i) + ", key1 = " + fd.KeyAt1(i) + ", f = " + f); });
        //fd.Update0(3, 2);
        //fd.ForEach((i, f) => { Console.WriteLine("key0 = " + fd.KeyAt0(i) + ", key1 = " + fd.KeyAt1(i) + ", f = " + f); });



        //var fd = new xx.DictEx<Foo, int, string, string, string>();
        //var f1 = new Foo { id = 1, name = "a", phone = "123", token = Guid.NewGuid().ToString() };
        //Console.WriteLine(fd.Add(f1, f1.id, f1.name, f1.phone, f1.token));  // true
        //var f2 = new Foo { id = 2, name = "b", phone = "456", token = Guid.NewGuid().ToString() };
        //Console.WriteLine(fd.Add(f2, f2.id, f2.name, f2.phone, f2.token));  // true
        //var f3 = new Foo { id = 3, name = "c", phone = "789", token = Guid.NewGuid().ToString() };
        //Console.WriteLine(fd.Add(f3, f3.id, f3.name, f3.phone, f3.token));  // true

        //Console.WriteLine(fd.Add(f1, f1.id, f1.name, f1.phone, f1.token));  // false
        //Console.WriteLine(fd.Add(f3, f3.id, f3.name, f3.phone, f3.token));  // false

        //Console.WriteLine(fd.Exists0(1));       // true
        //Console.WriteLine(fd.Exists1("b"));     // true
        //Console.WriteLine(fd.Exists0(3));       // true
        //Console.WriteLine(fd.Exists2("123"));   // true
        //Console.WriteLine(fd.Exists1("d"));     // false
        //Console.WriteLine(fd.Exists2("234"));   // false

        //fd.Remove1("a");
        //fd.Remove0(2);
        //Console.WriteLine(fd.Exists0(1));       // false
        //Console.WriteLine(fd.Exists1("b"));     // false

        //fd.ForEach((i, f) => { Console.WriteLine("key0 = " + fd.KeyAt0(i) + ", key1 = " + fd.KeyAt1(i) + ", f = " + f); });
        //fd.Update0(3, 2);
        //fd.ForEach((i, f) => { Console.WriteLine("key0 = " + fd.KeyAt0(i) + ", key1 = " + fd.KeyAt1(i) + ", f = " + f); });
    }
}
