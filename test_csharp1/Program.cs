public static class Program
{
    // 加持服务避免内存回收
    public static Service service;

    static void Main(string[] args)
    {
        PKG.AllTypes.Register();

        var uv = new UVWrapper();
        service = new Service(uv);
        uv.Run();
    }
}
