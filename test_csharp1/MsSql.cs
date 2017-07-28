using System;
using System.Data.SqlClient;

///// <summary>
///// MSSQL 的执行上下文工厂类
///// </summary>
//public static class MsSql
//{
//    /// <summary>
//    /// 以 lambda 方式执行一个需要数据库连接的函数, 返回 null 表示没错. 非 null 则为错误的详细内容( 方便调试 )
//    /// </summary>
//    public static void Exec(Action<DB.MsSqlFuncs> a)
//    {
//        // todo: get conn str from cfg file
//        using (var conn = new SqlConnection("Data Source=192.168.1.54;Initial Catalog=xxqp;User ID=sa;Password=1"))
//        {
//            conn.Open();
//            var fs = new DB.MsSqlFuncs(conn);
//            a(fs);
//        }
//    }

//}
