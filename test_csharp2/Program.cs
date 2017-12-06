using System;
using System.Diagnostics;

public static class Program
{
    static void Main(string[] args)
    {
        using (var uvloop = new XxUvLoop())
        {
            Console.WriteLine("uv created");
        }
    }
}

