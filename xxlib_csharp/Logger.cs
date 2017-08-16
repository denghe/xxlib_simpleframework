using System;
using System.Collections.Concurrent;
using System.Diagnostics;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;

namespace xx
{
    public static class LogInterop
    {
        [DllImport("xxloglib", CallingConvention = CallingConvention.Cdecl)]
        internal static extern IntPtr xxlogNew(IntPtr fn);

        [DllImport("xxloglib", CallingConvention = CallingConvention.Cdecl)]
        internal static extern IntPtr xxlogWriteAll(IntPtr ctx, int level, long time
            , IntPtr machine, IntPtr service, IntPtr instanceId
            , IntPtr title, long opcode, IntPtr desc);

        [DllImport("xxloglib", CallingConvention = CallingConvention.Cdecl)]
        internal static extern void xxlogSetDefaultValue(IntPtr ctx, IntPtr machine, IntPtr service, IntPtr instanceId);

        [DllImport("xxloglib", CallingConvention = CallingConvention.Cdecl)]
        internal static extern IntPtr xxlogWrite(IntPtr ctx, int level, IntPtr title, long opcode, IntPtr desc);

        [DllImport("xxloglib", CallingConvention = CallingConvention.Cdecl)]
        internal static extern void xxlogBeginTrans(IntPtr ctx);

        [DllImport("xxloglib", CallingConvention = CallingConvention.Cdecl)]
        internal static extern void xxlogEndTrans(IntPtr ctx);

        [DllImport("xxloglib", CallingConvention = CallingConvention.Cdecl)]
        internal static extern void xxlogDelete(IntPtr ctx);

        internal static string Utf8IntPtrToString(this IntPtr p)
        {
            if (p == IntPtr.Zero) return null;
            var bytes = new List<byte>();
            for (int offset = 0; ; offset++)
            {
                byte b = Marshal.ReadByte(p, offset);
                if (b == 0) break;
                else bytes.Add(b);
            }
            return Encoding.UTF8.GetString(bytes.buf, 0, bytes.dataLen);
        }
    }

    /// <summary>
    /// 日志级别
    /// </summary>
    public enum LogLevel : int
    {
        Off = 0,
        Fatal = 1,
        Error = 2,
        Warning = 3,
        Info = 4,
        Debug = 5,
        All = 6,
    }
    /// <summary>
    /// 对应 log 日志表
    /// </summary>
    public partial class Log
    {
        /// <summary>
        /// 自增主键
        /// </summary>
        public long id;
        /// <summary>
        /// 日志级别
        /// </summary>
        public LogLevel level;
        /// <summary>
        /// 发生时间
        /// </summary>
        public long time;
        /// <summary>
        /// 机器名
        /// </summary>
        public string machine;
        /// <summary>
        /// 进程/服务名
        /// </summary>
        public string service;
        /// <summary>
        /// 进程/实例id
        /// </summary>
        public string instanceId;
        /// <summary>
        /// 标题
        /// </summary>
        public string title;
        /// <summary>
        /// 操作代码
        /// </summary>
        public long opcode;
        /// <summary>
        /// 日志明细
        /// </summary>
        public string desc;
    }


    public class Logger : IDisposable
    {
        IntPtr ctx;

        /// <summary>
        /// 根据文件名 创建日志 db 文件. 成功返回 Logger 的指针 ctx. 失败返回 空
        /// </summary>
        public Logger(string fn = null)
        {
            if (string.IsNullOrWhiteSpace(fn))
            {
                fn = Process.GetCurrentProcess().MainModule.FileName + ".log.db";
            }

            var h = GCHandle.Alloc(Encoding.UTF8.GetBytes(fn), GCHandleType.Pinned);
            ctx = LogInterop.xxlogNew(h.AddrOfPinnedObject());
            h.Free();

            if (ctx == IntPtr.Zero)
            {
                throw new Exception("New failed. fn = " + fn);
            }
        }

        /// <summary>
        /// 直接传所有日志参数, 记录日志. 返回 nullptr 表示成功( 失败返回错误信息 )
        /// </summary>
        public string WriteAll(LogLevel level, long time, string machine, string service, string instanceId, string title, long opcode, string desc)
        {
            Debug.Assert(disposed);

            var hMachine = GCHandle.Alloc(Encoding.UTF8.GetBytes(machine), GCHandleType.Pinned);
            var hService = GCHandle.Alloc(Encoding.UTF8.GetBytes(service), GCHandleType.Pinned);
            var hInstanceId = GCHandle.Alloc(Encoding.UTF8.GetBytes(instanceId), GCHandleType.Pinned);
            var hTitle = GCHandle.Alloc(Encoding.UTF8.GetBytes(title), GCHandleType.Pinned);
            var hDesc = GCHandle.Alloc(Encoding.UTF8.GetBytes(desc), GCHandleType.Pinned);
            try
            {
                return LogInterop.xxlogWriteAll(ctx, (int)level, time, hMachine.AddrOfPinnedObject(), hService.AddrOfPinnedObject()
                    , hInstanceId.AddrOfPinnedObject(), hTitle.AddrOfPinnedObject(), opcode, hDesc.AddrOfPinnedObject()).Utf8IntPtrToString();
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
        public string Write(LogLevel level, string title, long opcode, string desc)
        {
            Debug.Assert(disposed);

            var hMachine = GCHandle.Alloc(Encoding.UTF8.GetBytes(title), GCHandleType.Pinned);
            var hService = GCHandle.Alloc(Encoding.UTF8.GetBytes(desc), GCHandleType.Pinned);
            try
            {
                return LogInterop.xxlogWrite(ctx, (int)level, hMachine.AddrOfPinnedObject()
                    , opcode, hService.AddrOfPinnedObject()).Utf8IntPtrToString();
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
            Debug.Assert(disposed);

            var hMachine = GCHandle.Alloc(Encoding.UTF8.GetBytes(machine), GCHandleType.Pinned);
            var hService = GCHandle.Alloc(Encoding.UTF8.GetBytes(service), GCHandleType.Pinned);
            var hInstanceId = GCHandle.Alloc(Encoding.UTF8.GetBytes(instanceId), GCHandleType.Pinned);
            try
            {
                LogInterop.xxlogSetDefaultValue(ctx, hMachine.AddrOfPinnedObject()
                    , hService.AddrOfPinnedObject(), hInstanceId.AddrOfPinnedObject());
            }
            finally
            {
                hMachine.Free();
                hService.Free();
                hInstanceId.Free();
            }
        }

        /// <summary>
        /// 开始批写( 启动事务, 合并写入批次, 能提升性能, 但增加了数据损坏的可能性 ). 返回 nullptr 表示成功( 失败返回错误信息 )
        /// </summary>
        public void BeginTrans()
        {
            Debug.Assert(disposed);
            LogInterop.xxlogBeginTrans(ctx);
        }

        /// <summary>
        /// 结束批写( 提交事务 ). 返回 nullptr 表示成功( 失败返回错误信息 )
        /// </summary>
        public void EndTrans()
        {
            Debug.Assert(disposed);
            LogInterop.xxlogEndTrans(ctx);
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
                LogInterop.xxlogDelete(ctx);
                disposed = true;
            }
            // Call Dispose in the base class.
            //base.Dispose(disposing);
        }

        // Use C# destructor syntax for finalization code.
        ~Logger()
        {
            Dispose(false);
        }

        // The derived class does not have a Finalize method
        // or a Dispose method without parameters because it inherits
        // them from the base class.

        #endregion
    }

    public class ConcurrentLogger : IDisposable
    {
        public ConcurrentLogger()
        {
            var t = new Thread(Process);
            t.Start();
        }

        public long counter = 0;
        public int batchSize = 1000;
        public ConcurrentQueue<Log> logMsgs = new ConcurrentQueue<Log>();

        protected void Process()
        {
            int tran = 0;
            using (var log = new xx.Logger())
            {
                while (!disposed)
                {
                    while (logMsgs.TryDequeue(out var o))
                    {
                        if (o.id == 2)
                        {
                            log.SetDefaultValue(o.machine, o.service, o.instanceId);
                            continue;
                        }

                        if (tran == 0)
                        {
                            log.BeginTrans();
                        }

                        if (o.id == 1)
                        {
                            log.Write(o.level, o.title, o.opcode, o.desc);
                        }
                        else
                        {
                            log.WriteAll(o.level, o.time, o.machine, o.service
                                , o.instanceId, o.title, o.opcode, o.desc);
                        }

                        ++tran;
                        if (tran > batchSize)
                        {
                            log.EndTrans();
                            tran = 0;
                        }

                        ++counter;
                    }

                    if (tran > 0)
                    {
                        log.EndTrans();
                        tran = 0;
                    }
                    Thread.Sleep(1);
                }
            }
            Console.WriteLine("Process Quited.");
        }

        public void WriteAll(LogLevel level, long time, string machine, string service, string instanceId, string title, long opcode, string desc)
        {
            logMsgs.Enqueue(new Log
            {
                id = 1,             // 用来标记是通过 WaitAll 写入的
                level = level,
                time = time,
                machine = machine,
                service = service,
                instanceId = instanceId,
                title = title,
                opcode = opcode,
                desc = desc
            });
        }

        public void Write(LogLevel level, string title, long opcode, string desc)
        {
            logMsgs.Enqueue(new Log
            {
                id = 0,             // 用来标记是通过 Write 写入的
                level = level,
                time = DateTime.Now.Ticks,
                title = title,
                opcode = opcode,
                desc = desc
            });
        }

        public void SetDefaultValue(string machine, string service, string instanceId)
        {
            logMsgs.Enqueue(new Log
            {
                id = 2,             // 用来标记是通过 SetDefaultValue 写入的
                machine = machine,
                service = service,
                instanceId = instanceId
            });
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
                disposed = true;
            }
            // Call Dispose in the base class.
            //base.Dispose(disposing);
        }

        // Use C# destructor syntax for finalization code.
        ~ConcurrentLogger()
        {
            Dispose(false);
        }

        // The derived class does not have a Finalize method
        // or a Dispose method without parameters because it inherits
        // them from the base class.

        #endregion
    }
}

