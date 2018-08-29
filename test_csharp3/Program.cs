using System;
using System.Diagnostics;
using System.Runtime.InteropServices;
using System.Threading;
using MySql.Data.MySqlClient;

public static class Program
{
    /// <summary>
    /// 快速执行 sql 的简易封装( 针对 MySqlFuncs ). 返回异常.
    /// </summary>
    public static Exception CallMySqlFuncs(Action<PKG.MySqlFuncs> a)
    {
        try
        {
            using (var conn = new MySqlConnection($"server={"127.0.0.1"};user id={"root"};password={"1"};database={"catch_fish"};port={3306};charset=utf8;sslmode=none"))
            {
                conn.Open();
                var fs = new PKG.MySqlFuncs(conn);
                a(fs);
            }
        }
        catch (Exception ex)
        {
            return ex;
        }
        return null;
    }


    static void Main(string[] args)
    {
        var e = CallMySqlFuncs(fs =>
        {
            var r = fs.TestSql(123);
            Console.WriteLine(r);
        });
    }
}
