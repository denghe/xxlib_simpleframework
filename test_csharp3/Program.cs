using System;
using System.Diagnostics;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Threading;
using MySql.Data.MySqlClient;

public static class Dll
{
#if !UNITY_EDITOR && UNITY_IPHONE
        const string DLL_NAME = "__Internal";
#else
    const string DLL_NAME = "libtest1";
#endif

    //[MethodImpl(256)]
    [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    public static unsafe extern void* create_mp();

    [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    public static unsafe extern void release_mp(void* mp);

    [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    public static unsafe extern byte* mp_alloc(void* mp, IntPtr len);

    [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    public static unsafe extern void mp_free(void* mp, byte* buf);

    [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    public static unsafe extern byte* mp_resize(void* mp, byte* buf, IntPtr newBuflen, IntPtr dataLen);

    [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    public static unsafe extern void xxmemcpy(byte* src, byte* dst, IntPtr dataLen);


    public static unsafe void* mp = create_mp();   // todo: free
    public static unsafe byte* mpAlloc(int len)
    {
        return mp_alloc(mp, (IntPtr)len);
    }
    public static unsafe byte* mpAlloc(long len)
    {
        return mp_alloc(mp, (IntPtr)len);
    }
    public static unsafe void mpFree(byte* buf)
    {
        mp_free(mp, buf);
    }
    public static unsafe byte* mpResize(byte* buf, int newBuflen, int dataLen)
    {
        return mp_resize(mp, buf, (IntPtr)newBuflen, (IntPtr)dataLen);
    }
    public static unsafe byte* mpResize(byte* buf, long newBuflen, long dataLen)
    {
        return mp_resize(mp, buf, (IntPtr)newBuflen, (IntPtr)dataLen);
    }
}

public unsafe class BB : IDisposable
{
    public byte* buf;
    public int bufLen, dataLen;

    public BB()
    {
        buf = Dll.mpAlloc(32);
        bufLen = 32;
    }

    public void Reserve(int capacity)
    {
        if (capacity <= bufLen) return;
        bufLen = capacity * 2;
        buf = Dll.mpResize(buf, bufLen, dataLen);
    }

    public void Write(int v)
    {
        if (dataLen + 5 > bufLen) Reserve(dataLen + 5);
        Bit7Write(ZigZagEncode(v));
    }

    public static uint ZigZagEncode(int v) { return (uint)((v << 1) ^ (v >> 31)); }
    public void Bit7Write(uint v)
    {
        Lab1:
        byte b7 = (byte)v;
        v >>= 7;
        if (v > 0)
        {
            buf[dataLen++] = (byte)(b7 | (byte)0x80);
            goto Lab1;
        }
        buf[dataLen++] = b7;
    }

    public void Clear()
    {
        dataLen = 0;
    }

    public void Dispose()
    {
        Dll.mpFree(buf);
    }
}

public static class Program
{
    static unsafe void Main(string[] args)
    {
        var outBuf = Dll.mpAlloc(512);  // todo: free
        for (int k = 0; k < 10; k++)
        {

            var sw = Stopwatch.StartNew();
            {
                var count = 0;
                var bb = new xx.BBuffer();
                for (int j = 0; j < 10000000; j++)
                {
                    bb.Clear();
                    for (int i = 0; i < 20; i++)
                    {
                        bb.Write(1234567);
                    }
                    count += bb.dataLen;
                    Marshal.Copy(bb.buf, 0, (IntPtr)outBuf, bb.dataLen);
                }
                Console.WriteLine("write 10000000 times, count = " + count + ", ms = " + sw.ElapsedMilliseconds);
            }

            sw.Restart();
            {
                var count = 0;
                using (var bb = new BB())
                {
                    for (int j = 0; j < 10000000; j++)
                    {
                        bb.Clear();
                        for (int i = 0; i < 20; i++)
                        {
                            bb.Write(1234567);
                        }
                        count += bb.dataLen;
                        //Dll.xxmemcpy(bb.buf, outBuf, (IntPtr)bb.dataLen);
                    }
                }
                Console.WriteLine("write 10000000 times, count = " + count + ", ms = " + sw.ElapsedMilliseconds);
            }
        }
    }
}


//var sw = Stopwatch.StartNew();
//{
//    for (int i = 0; i < 100000000; i++)
//    {
//        var ptr = Marshal.AllocHGlobal(4);
//        Marshal.FreeHGlobal(ptr);
//        //Marshal.ReAllocHGlobal
//    }
//    Console.WriteLine("new + del 100000000 times, ms = " + sw.ElapsedMilliseconds);
//}

//sw.Restart();
//unsafe
//{
//    var mp = Dll.create_mp();
//    for (int i = 0; i < 100000000; i++)
//    {
//        var ptr = Dll.mp_alloc(mp, (IntPtr)4);
//        Dll.mp_free(mp, ptr);
//    }
//    Dll.release_mp(mp);
//    Console.WriteLine("mp.alloc + free 100000000 times, ms = " + sw.ElapsedMilliseconds);
//}