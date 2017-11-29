using System;
using System.Runtime.InteropServices;

public static class XxMemPoolInterop
{
#if !UNITY_EDITOR && UNITY_IPHONE
        const string DLL_NAME = "__Internal";
#else
    const string DLL_NAME = "xxnbsocketlib";
#endif

    /// <summary>
    /// 建 XxMemPool
    /// </summary>
    /// <returns></returns>
    [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    internal static extern IntPtr XxMemPool_New();

    /// <summary>
    /// 杀 XxMemPool( 最后再杀 )
    /// </summary>
    [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    internal static extern void XxMemPool_Delete(IntPtr mp);

}

public class XxMemPool : IDisposable
{
    /// <summary>
    /// 内存池 指针
    /// </summary>
    public IntPtr pointer;

    public XxMemPool()
    {
        pointer = XxMemPoolInterop.XxMemPool_New();
        if (pointer == IntPtr.Zero)
        {
            throw new Exception("XxMemPool_New failed.");
        }
    }

    #region Dispose

    private bool disposed = false;

    //Implement IDisposable.
    public void Dispose()
    {
        Dispose(true);
        GC.SuppressFinalize(this);
    }

    protected virtual void Dispose(bool disposing)
    {
        if (!disposed)
        {
            if (disposing)
            {
                // Free other state (managed objects).
            }
            // Free your own state (unmanaged objects).
            // Set large fields to null.
            XxMemPoolInterop.XxMemPool_Delete(pointer);
            pointer = IntPtr.Zero;
            disposed = true;
        }
        // Call Dispose in the base class.
        //base.Dispose(disposing);
    }

    // Use C# destructor syntax for finalization code.
    ~XxMemPool()
    {
        Dispose(false);
    }

    // The derived class does not have a Finalize method
    // or a Dispose method without parameters because it inherits
    // them from the base class.

    #endregion
}
