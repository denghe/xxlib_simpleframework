using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

public static class XxBBufferInterop
{

    /// <summary>
    /// 建 XxBBuffer
    /// </summary>
    [DllImport("xxnbsocketlib", CallingConvention = CallingConvention.Cdecl)]
    internal static extern IntPtr XxBBuffer_New(IntPtr mp, IntPtr buf, int dataLen);

    /// <summary>
    /// 杀 XxBBuffer
    /// </summary>
    [DllImport("xxnbsocketlib", CallingConvention = CallingConvention.Cdecl)]
    internal static extern void XxBBuffer_Delete(IntPtr bb);

    /// <summary>
    /// 扩容
    /// </summary>
    [DllImport("xxnbsocketlib", CallingConvention = CallingConvention.Cdecl)]
    internal static extern void XxBBuffer_Reserve(IntPtr bb, int capacity);

    /// <summary>
    /// 清长度和读偏移
    /// </summary>
    [DllImport("xxnbsocketlib", CallingConvention = CallingConvention.Cdecl)]
    internal static extern void XxBBuffer_Clear(IntPtr bb);

    /// <summary>
    /// 得到长度
    /// </summary>
    [DllImport("xxnbsocketlib", CallingConvention = CallingConvention.Cdecl)]
    internal static extern int XxBBuffer_GetDataLen(IntPtr bb);

    /// <summary>
    /// 得到读偏移
    /// </summary>
    [DllImport("xxnbsocketlib", CallingConvention = CallingConvention.Cdecl)]
    internal static extern int XxBBuffer_GetOffset(IntPtr bb);


    [DllImport("xxnbsocketlib", CallingConvention = CallingConvention.Cdecl)] internal static extern void XxBBuffer_WriteBoolean(IntPtr bb, bool v);
    [DllImport("xxnbsocketlib", CallingConvention = CallingConvention.Cdecl)] internal static extern void XxBBuffer_WriteInt8(IntPtr bb, sbyte v);
    [DllImport("xxnbsocketlib", CallingConvention = CallingConvention.Cdecl)] internal static extern void XxBBuffer_WriteInt16(IntPtr bb, short v);
    [DllImport("xxnbsocketlib", CallingConvention = CallingConvention.Cdecl)] internal static extern void XxBBuffer_WriteInt32(IntPtr bb, int v);
    [DllImport("xxnbsocketlib", CallingConvention = CallingConvention.Cdecl)] internal static extern void XxBBuffer_WriteInt64(IntPtr bb, long v);
    [DllImport("xxnbsocketlib", CallingConvention = CallingConvention.Cdecl)] internal static extern void XxBBuffer_WriteUInt8(IntPtr bb, byte v);
    [DllImport("xxnbsocketlib", CallingConvention = CallingConvention.Cdecl)] internal static extern void XxBBuffer_WriteUInt16(IntPtr bb, ushort v);
    [DllImport("xxnbsocketlib", CallingConvention = CallingConvention.Cdecl)] internal static extern void XxBBuffer_WriteUInt32(IntPtr bb, uint v);
    [DllImport("xxnbsocketlib", CallingConvention = CallingConvention.Cdecl)] internal static extern void XxBBuffer_WriteUInt64(IntPtr bb, ulong v);
    [DllImport("xxnbsocketlib", CallingConvention = CallingConvention.Cdecl)] internal static extern void XxBBuffer_WriteFloat(IntPtr bb, float v);
    [DllImport("xxnbsocketlib", CallingConvention = CallingConvention.Cdecl)] internal static extern void XxBBuffer_WriteDouble(IntPtr bb, double v);

    [DllImport("xxnbsocketlib", CallingConvention = CallingConvention.Cdecl)] internal static extern void XxBBuffer_WritePodBoolean(IntPtr bb, bool v);
    [DllImport("xxnbsocketlib", CallingConvention = CallingConvention.Cdecl)] internal static extern void XxBBuffer_WritePodInt8(IntPtr bb, sbyte v);
    [DllImport("xxnbsocketlib", CallingConvention = CallingConvention.Cdecl)] internal static extern void XxBBuffer_WritePodInt16(IntPtr bb, short v);
    [DllImport("xxnbsocketlib", CallingConvention = CallingConvention.Cdecl)] internal static extern void XxBBuffer_WritePodInt32(IntPtr bb, int v);
    [DllImport("xxnbsocketlib", CallingConvention = CallingConvention.Cdecl)] internal static extern void XxBBuffer_WritePodInt64(IntPtr bb, long v);
    [DllImport("xxnbsocketlib", CallingConvention = CallingConvention.Cdecl)] internal static extern void XxBBuffer_WritePodUInt8(IntPtr bb, byte v);
    [DllImport("xxnbsocketlib", CallingConvention = CallingConvention.Cdecl)] internal static extern void XxBBuffer_WritePodUInt16(IntPtr bb, ushort v);
    [DllImport("xxnbsocketlib", CallingConvention = CallingConvention.Cdecl)] internal static extern void XxBBuffer_WritePodUInt32(IntPtr bb, uint v);
    [DllImport("xxnbsocketlib", CallingConvention = CallingConvention.Cdecl)] internal static extern void XxBBuffer_WritePodUInt64(IntPtr bb, ulong v);
    [DllImport("xxnbsocketlib", CallingConvention = CallingConvention.Cdecl)] internal static extern void XxBBuffer_WritePodFloat(IntPtr bb, float v);
    [DllImport("xxnbsocketlib", CallingConvention = CallingConvention.Cdecl)] internal static extern void XxBBuffer_WritePodDouble(IntPtr bb, double v);

    [DllImport("xxnbsocketlib", CallingConvention = CallingConvention.Cdecl)] internal static extern void XxBBuffer_WriteLenData(IntPtr bb, IntPtr s, int len);


    [DllImport("xxnbsocketlib", CallingConvention = CallingConvention.Cdecl)] internal static extern int XxBBuffer_ReadBoolean(IntPtr bb, ref bool v);
    [DllImport("xxnbsocketlib", CallingConvention = CallingConvention.Cdecl)] internal static extern int XxBBuffer_ReadInt8(IntPtr bb, ref sbyte v);
    [DllImport("xxnbsocketlib", CallingConvention = CallingConvention.Cdecl)] internal static extern int XxBBuffer_ReadInt16(IntPtr bb, ref short v);
    [DllImport("xxnbsocketlib", CallingConvention = CallingConvention.Cdecl)] internal static extern int XxBBuffer_ReadInt32(IntPtr bb, ref int v);
    [DllImport("xxnbsocketlib", CallingConvention = CallingConvention.Cdecl)] internal static extern int XxBBuffer_ReadInt64(IntPtr bb, ref long v);
    [DllImport("xxnbsocketlib", CallingConvention = CallingConvention.Cdecl)] internal static extern int XxBBuffer_ReadUInt8(IntPtr bb, ref byte v);
    [DllImport("xxnbsocketlib", CallingConvention = CallingConvention.Cdecl)] internal static extern int XxBBuffer_ReadUInt16(IntPtr bb, ref ushort v);
    [DllImport("xxnbsocketlib", CallingConvention = CallingConvention.Cdecl)] internal static extern int XxBBuffer_ReadUInt32(IntPtr bb, ref uint v);
    [DllImport("xxnbsocketlib", CallingConvention = CallingConvention.Cdecl)] internal static extern int XxBBuffer_ReadUInt64(IntPtr bb, ref ulong v);
    [DllImport("xxnbsocketlib", CallingConvention = CallingConvention.Cdecl)] internal static extern int XxBBuffer_ReadFloat(IntPtr bb, ref float v);
    [DllImport("xxnbsocketlib", CallingConvention = CallingConvention.Cdecl)] internal static extern int XxBBuffer_ReadDouble(IntPtr bb, ref double v);

    [DllImport("xxnbsocketlib", CallingConvention = CallingConvention.Cdecl)] internal static extern int XxBBuffer_ReadPodBoolean(IntPtr bb, ref bool v);
    [DllImport("xxnbsocketlib", CallingConvention = CallingConvention.Cdecl)] internal static extern int XxBBuffer_ReadPodInt8(IntPtr bb, ref sbyte v);
    [DllImport("xxnbsocketlib", CallingConvention = CallingConvention.Cdecl)] internal static extern int XxBBuffer_ReadPodInt16(IntPtr bb, ref short v);
    [DllImport("xxnbsocketlib", CallingConvention = CallingConvention.Cdecl)] internal static extern int XxBBuffer_ReadPodInt32(IntPtr bb, ref int v);
    [DllImport("xxnbsocketlib", CallingConvention = CallingConvention.Cdecl)] internal static extern int XxBBuffer_ReadPodInt64(IntPtr bb, ref long v);
    [DllImport("xxnbsocketlib", CallingConvention = CallingConvention.Cdecl)] internal static extern int XxBBuffer_ReadPodUInt8(IntPtr bb, ref byte v);
    [DllImport("xxnbsocketlib", CallingConvention = CallingConvention.Cdecl)] internal static extern int XxBBuffer_ReadPodUInt16(IntPtr bb, ref ushort v);
    [DllImport("xxnbsocketlib", CallingConvention = CallingConvention.Cdecl)] internal static extern int XxBBuffer_ReadPodUInt32(IntPtr bb, ref uint v);
    [DllImport("xxnbsocketlib", CallingConvention = CallingConvention.Cdecl)] internal static extern int XxBBuffer_ReadPodUInt64(IntPtr bb, ref ulong v);
    [DllImport("xxnbsocketlib", CallingConvention = CallingConvention.Cdecl)] internal static extern int XxBBuffer_ReadPodFloat(IntPtr bb, ref float v);
    [DllImport("xxnbsocketlib", CallingConvention = CallingConvention.Cdecl)] internal static extern int XxBBuffer_ReadPodDouble(IntPtr bb, ref double v);

    [DllImport("xxnbsocketlib", CallingConvention = CallingConvention.Cdecl)] internal static extern int XxBBuffer_ReadLenData(IntPtr bb, ref IntPtr s, ref int len);

}



public class XxBBuffer : IDisposable
{
    public XxBBuffer(XxMemPool mempool)
    {
        this.mempool = mempool;
        pointer = XxNBSocketInterop.XxNBSocket_New(mempool.pointer);
        if (pointer == IntPtr.Zero)
        {
            throw new Exception("XxNBSocket_New failed.");
        }
    }

    // 直接用指针构造
    public XxBBuffer(XxMemPool mp, IntPtr bb)
    {
        mempool = mp;
        pointer = bb;
    }

    /// <summary>
    /// 指向内存池
    /// </summary>
    XxMemPool mempool;

    /// <summary>
    /// this 指针
    /// </summary>
    IntPtr pointer;


    public void Write(bool v) { XxBBufferInterop.XxBBuffer_WriteBoolean(pointer, v); }
    public void Write(sbyte v) { XxBBufferInterop.XxBBuffer_WriteInt8(pointer, v); }
    public void Write(short v) { XxBBufferInterop.XxBBuffer_WriteInt16(pointer, v); }
    public void Write(int v) { XxBBufferInterop.XxBBuffer_WriteInt32(pointer, v); }
    public void Write(long v) { XxBBufferInterop.XxBBuffer_WriteInt64(pointer, v); }
    public void Write(byte v) { XxBBufferInterop.XxBBuffer_WriteUInt8(pointer, v); }
    public void Write(ushort v) { XxBBufferInterop.XxBBuffer_WriteUInt16(pointer, v); }
    public void Write(uint v) { XxBBufferInterop.XxBBuffer_WriteUInt32(pointer, v); }
    public void Write(ulong v) { XxBBufferInterop.XxBBuffer_WriteUInt64(pointer, v); }
    public void Write(float v) { XxBBufferInterop.XxBBuffer_WriteFloat(pointer, v); }
    public void Write(double v) { XxBBufferInterop.XxBBuffer_WriteDouble(pointer, v); }

    public void WritePod(bool v) { XxBBufferInterop.XxBBuffer_WritePodBoolean(pointer, v); }
    public void WritePod(sbyte v) { XxBBufferInterop.XxBBuffer_WritePodInt8(pointer, v); }
    public void WritePod(short v) { XxBBufferInterop.XxBBuffer_WritePodInt16(pointer, v); }
    public void WritePod(int v) { XxBBufferInterop.XxBBuffer_WritePodInt32(pointer, v); }
    public void WritePod(long v) { XxBBufferInterop.XxBBuffer_WritePodInt64(pointer, v); }
    public void WritePod(byte v) { XxBBufferInterop.XxBBuffer_WritePodUInt8(pointer, v); }
    public void WritePod(ushort v) { XxBBufferInterop.XxBBuffer_WritePodUInt16(pointer, v); }
    public void WritePod(uint v) { XxBBufferInterop.XxBBuffer_WritePodUInt32(pointer, v); }
    public void WritePod(ulong v) { XxBBufferInterop.XxBBuffer_WritePodUInt64(pointer, v); }
    public void WritePod(float v) { XxBBufferInterop.XxBBuffer_WritePodFloat(pointer, v); }
    public void WritePod(double v) { XxBBufferInterop.XxBBuffer_WritePodDouble(pointer, v); }

    public void WriteLenData(byte[] buf, int dataLen)
    {
        if (dataLen == 0)
        {
            XxBBufferInterop.XxBBuffer_WriteLenData(pointer, IntPtr.Zero, 0);
        }
        else
        {
            var h = GCHandle.Alloc(buf, GCHandleType.Pinned);
            XxBBufferInterop.XxBBuffer_WriteLenData(pointer, h.AddrOfPinnedObject(), dataLen);
            h.Free();
        }
    }

    public void Read(ref bool v) { if (XxBBufferInterop.XxBBuffer_ReadBoolean(pointer, ref v) != 0) throw new Exception(); }
    public void Read(ref sbyte v) { if (XxBBufferInterop.XxBBuffer_ReadInt8(pointer, ref v) != 0) throw new Exception(); }
    public void Read(ref short v) { if (XxBBufferInterop.XxBBuffer_ReadInt16(pointer, ref v) != 0) throw new Exception(); }
    public void Read(ref int v) { if (XxBBufferInterop.XxBBuffer_ReadInt32(pointer, ref v) != 0) throw new Exception(); }
    public void Read(ref long v) { if (XxBBufferInterop.XxBBuffer_ReadInt64(pointer, ref v) != 0) throw new Exception(); }
    public void Read(ref byte v) { if (XxBBufferInterop.XxBBuffer_ReadUInt8(pointer, ref v) != 0) throw new Exception(); }
    public void Read(ref ushort v) { if (XxBBufferInterop.XxBBuffer_ReadUInt16(pointer, ref v) != 0) throw new Exception(); }
    public void Read(ref uint v) { if (XxBBufferInterop.XxBBuffer_ReadUInt32(pointer, ref v) != 0) throw new Exception(); }
    public void Read(ref ulong v) { if (XxBBufferInterop.XxBBuffer_ReadUInt64(pointer, ref v) != 0) throw new Exception(); }
    public void Read(ref float v) { if (XxBBufferInterop.XxBBuffer_ReadFloat(pointer, ref v) != 0) throw new Exception(); }
    public void Read(ref double v) { if (XxBBufferInterop.XxBBuffer_ReadDouble(pointer, ref v) != 0) throw new Exception(); }

    public void ReadPod(ref bool v) { if (XxBBufferInterop.XxBBuffer_ReadPodBoolean(pointer, ref v) != 0) throw new Exception(); }
    public void ReadPod(ref sbyte v) { if (XxBBufferInterop.XxBBuffer_ReadPodInt8(pointer, ref v) != 0) throw new Exception(); }
    public void ReadPod(ref short v) { if (XxBBufferInterop.XxBBuffer_ReadPodInt16(pointer, ref v) != 0) throw new Exception(); }
    public void ReadPod(ref int v) { if (XxBBufferInterop.XxBBuffer_ReadPodInt32(pointer, ref v) != 0) throw new Exception(); }
    public void ReadPod(ref long v) { if (XxBBufferInterop.XxBBuffer_ReadPodInt64(pointer, ref v) != 0) throw new Exception(); }
    public void ReadPod(ref byte v) { if (XxBBufferInterop.XxBBuffer_ReadPodUInt8(pointer, ref v) != 0) throw new Exception(); }
    public void ReadPod(ref ushort v) { if (XxBBufferInterop.XxBBuffer_ReadPodUInt16(pointer, ref v) != 0) throw new Exception(); }
    public void ReadPod(ref uint v) { if (XxBBufferInterop.XxBBuffer_ReadPodUInt32(pointer, ref v) != 0) throw new Exception(); }
    public void ReadPod(ref ulong v) { if (XxBBufferInterop.XxBBuffer_ReadPodUInt64(pointer, ref v) != 0) throw new Exception(); }
    public void ReadPod(ref float v) { if (XxBBufferInterop.XxBBuffer_ReadPodFloat(pointer, ref v) != 0) throw new Exception(); }
    public void ReadPod(ref double v) { if (XxBBufferInterop.XxBBuffer_ReadPodDouble(pointer, ref v) != 0) throw new Exception(); }

    public void ReadLenData(ref byte[] buf, ref int dataLen)
    {
        IntPtr bufPtr = IntPtr.Zero;
        if (XxBBufferInterop.XxBBuffer_ReadLenData(pointer, ref bufPtr, ref dataLen) != 0) throw new Exception();
        if (bufPtr == IntPtr.Zero) throw new Exception();
        if (dataLen > 0)
        {
            if (buf.Length < dataLen) buf = new byte[dataLen];
            Marshal.Copy(bufPtr, buf, 0, dataLen);
        }
    }

    // 清长度和读偏移
    public void Clear()
    {
        XxBBufferInterop.XxBBuffer_Clear(pointer);
    }

    // 返回数据长度
    public int dataLen
    {
        get { return XxBBufferInterop.XxBBuffer_GetDataLen(pointer); }
    }

    // 返回读偏移量
    public int offset
    {
        get { return XxBBufferInterop.XxBBuffer_GetOffset(pointer); }
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
            XxNBSocketInterop.XxNBSocket_Delete(pointer);
            pointer = IntPtr.Zero;
            mempool = null;
            disposed = true;
        }
        // Call Dispose in the base class.
        //base.Dispose(disposing);
    }

    // Use C# destructor syntax for finalization code.
    ~XxBBuffer()
    {
        Dispose(false);
    }

    // The derived class does not have a Finalize method
    // or a Dispose method without parameters because it inherits
    // them from the base class.

    #endregion
}
