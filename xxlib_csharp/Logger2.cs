using System;
using System.Collections.Concurrent;
using System.Diagnostics;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;

namespace xx
{
    public static class Log2Interop
    {
        [DllImport("xxloglib", CallingConvention = CallingConvention.Cdecl)]
        internal static extern IntPtr xxlog2New(IntPtr fn);


        [DllImport("xxloglib", CallingConvention = CallingConvention.Cdecl)]
        internal static extern void xxlog2WriteAll(IntPtr ctx, int level, long time
            , IntPtr machine, IntPtr service, IntPtr instanceId
            , IntPtr title, long opcode, IntPtr desc);


        [DllImport("xxloglib", CallingConvention = CallingConvention.Cdecl)]
        internal static extern void xxlog2Write(IntPtr ctx, int level, IntPtr title, long opcode, IntPtr desc);


        [DllImport("xxloglib", CallingConvention = CallingConvention.Cdecl)]
        internal static extern void xxlog2SetDefaultValue(IntPtr ctx, IntPtr machine, IntPtr service, IntPtr instanceId);


        [DllImport("xxloglib", CallingConvention = CallingConvention.Cdecl)]
        internal static extern long xxlog2GetCounter(IntPtr ctx);


        [DllImport("xxloglib", CallingConvention = CallingConvention.Cdecl)]
        internal static extern void xxlog2Delete(IntPtr ctx);
    }

    public class Logger2 : IDisposable
    {
        IntPtr ctx;

        /// <summary>
        /// 取已写入数据条数
        /// </summary>
        public long counter { get { return Log2Interop.xxlog2GetCounter(ctx); } }

        /// <summary>
        /// 根据文件名 创建日志 db 文件. 成功返回 Logger 的指针 ctx. 失败返回 空
        /// </summary>
        public Logger2(string fn = null)
        {
            if (string.IsNullOrWhiteSpace(fn))
            {
                fn = Process.GetCurrentProcess().MainModule.FileName + ".log.db";
            }

            var h = GCHandle.Alloc(Encoding.UTF8.GetBytes(fn), GCHandleType.Pinned);
            ctx = Log2Interop.xxlog2New(h.AddrOfPinnedObject());
            h.Free();

            if (ctx == IntPtr.Zero)
            {
                throw new Exception("New failed. fn = " + fn);
            }
        }

        /// <summary>
        /// 直接传所有日志参数, 记录日志. 返回 nullptr 表示成功( 失败返回错误信息 )
        /// </summary>
        public void WriteAll(LogLevel level, long time, string machine, string service, string instanceId, string title, long opcode, string desc)
        {
            Debug.Assert(!disposed);

            var hMachine = GCHandle.Alloc(Encoding.UTF8.GetBytes(machine), GCHandleType.Pinned);
            var hService = GCHandle.Alloc(Encoding.UTF8.GetBytes(service), GCHandleType.Pinned);
            var hInstanceId = GCHandle.Alloc(Encoding.UTF8.GetBytes(instanceId), GCHandleType.Pinned);
            var hTitle = GCHandle.Alloc(Encoding.UTF8.GetBytes(title), GCHandleType.Pinned);
            var hDesc = GCHandle.Alloc(Encoding.UTF8.GetBytes(desc), GCHandleType.Pinned);
            try
            {
                Log2Interop.xxlog2WriteAll(ctx, (int)level, time, hMachine.AddrOfPinnedObject(), hService.AddrOfPinnedObject()
                    , hInstanceId.AddrOfPinnedObject(), hTitle.AddrOfPinnedObject(), opcode, hDesc.AddrOfPinnedObject());
            }
            finally
            {
                hMachine.Free();
                hService.Free();
                hInstanceId.Free();
                hTitle.Free();
                hDesc.Free();
            }
        }

        /// <summary>
        /// 记录日志( 相比 WriteAll 缺失的参数 会去读默认值 ). 返回 nullptr 表示成功( 失败返回错误信息 )
        /// </summary>
        public void Write(LogLevel level, string title, long opcode, string desc)
        {
            Debug.Assert(!disposed);

            var hMachine = GCHandle.Alloc(Encoding.UTF8.GetBytes(title), GCHandleType.Pinned);
            var hService = GCHandle.Alloc(Encoding.UTF8.GetBytes(desc), GCHandleType.Pinned);
            try
            {
                Log2Interop.xxlog2Write(ctx, (int)level, hMachine.AddrOfPinnedObject()
                    , opcode, hService.AddrOfPinnedObject());
            }
            finally
            {
                hMachine.Free();
                hService.Free();
            }
        }

        /// <summary>
        /// 设置一些不容易变化的项目的默认值, 以便使用 Write 时能反复写入这些数据 而不需要再传
        /// </summary>
        public void SetDefaultValue(string machine, string service, string instanceId)
        {
            Debug.Assert(!disposed);

            var hMachine = GCHandle.Alloc(Encoding.UTF8.GetBytes(machine), GCHandleType.Pinned);
            var hService = GCHandle.Alloc(Encoding.UTF8.GetBytes(service), GCHandleType.Pinned);
            var hInstanceId = GCHandle.Alloc(Encoding.UTF8.GetBytes(instanceId), GCHandleType.Pinned);
            try
            {
                Log2Interop.xxlog2SetDefaultValue(ctx, hMachine.AddrOfPinnedObject()
                    , hService.AddrOfPinnedObject(), hInstanceId.AddrOfPinnedObject());
            }
            finally
            {
                hMachine.Free();
                hService.Free();
                hInstanceId.Free();
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
                Log2Interop.xxlog2Delete(ctx);
                disposed = true;
            }
            // Call Dispose in the base class.
            //base.Dispose(disposing);
        }

        // Use C# destructor syntax for finalization code.
        ~Logger2()
        {
            Dispose(false);
        }

        // The derived class does not have a Finalize method
        // or a Dispose method without parameters because it inherits
        // them from the base class.

        #endregion
    }

}

