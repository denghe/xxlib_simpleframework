using System;
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
public class FooDict : xx.DictEx<int, string, string, string, Foo>
{
    public FooDict() : base(o => o.id, o => o.name, o => o.phone, o => o.token) { }
}
public static class Program
{
    static void Main(string[] args)
    {
        var fd = new FooDict();
        Console.WriteLine(fd.Add(new Foo { id = 1, name = "a", phone = "123", token = Guid.NewGuid().ToString() }));  // true
        Console.WriteLine(fd.Add(new Foo { id = 2, name = "b", phone = "456", token = Guid.NewGuid().ToString() }));  // true
        Console.WriteLine(fd.Add(new Foo { id = 3, name = "c", phone = "789", token = Guid.NewGuid().ToString() }));  // true

        Console.WriteLine(fd.Add(new Foo { id = 1, name = "d" }));  // false
        Console.WriteLine(fd.Add(new Foo { id = 4, name = "a" }));  // false

        Console.WriteLine(fd.Exists1(1));       // true
        Console.WriteLine(fd.Exists2("b"));     // true
        Console.WriteLine(fd.Exists1(3));       // true
        Console.WriteLine(fd.Exists3("123"));   // true
        Console.WriteLine(fd.Exists2("d"));     // false
        Console.WriteLine(fd.Exists3("234"));   // false

        fd.Remove2("a");
        fd.Remove1(2);
        Console.WriteLine(fd.Exists1(1));       // false
        Console.WriteLine(fd.Exists2("b"));     // false

        fd.ForEach(f => { Console.WriteLine(f); }); // 3, c
    }
}
