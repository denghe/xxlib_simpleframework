using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Runtime.InteropServices;
using System.Text;

namespace xx
{
    public class UTF8Marshaler : ICustomMarshaler
    {
        public void CleanUpManagedData(object managedObj)
        {
        }

        public void CleanUpNativeData(IntPtr pNativeData)
        {
            Marshal.FreeHGlobal(pNativeData);
        }

        public int GetNativeDataSize()
        {
            return -1;
        }

        public IntPtr MarshalManagedToNative(object managedObj)
        {
            if (object.ReferenceEquals(managedObj, null))
                return IntPtr.Zero;
            if (!(managedObj is string))
                throw new InvalidOperationException();

            byte[] utf8bytes = Encoding.UTF8.GetBytes(managedObj as string);
            IntPtr ptr = Marshal.AllocHGlobal(utf8bytes.Length + 1);
            Marshal.Copy(utf8bytes, 0, ptr, utf8bytes.Length);
            Marshal.WriteByte(ptr, utf8bytes.Length, 0);
            return ptr;
        }

        public object MarshalNativeToManaged(IntPtr pNativeData)
        {
            if (pNativeData == IntPtr.Zero)
                return null;

            var bytes = new List<byte>();
            for (int offset = 0; ; offset++)
            {
                byte b = Marshal.ReadByte(pNativeData, offset);
                if (b == 0)
                    break;
                else
                    bytes.Add(b);
            }
            return Encoding.UTF8.GetString(bytes.ToArray(), 0, bytes.Count);
        }

        private static UTF8Marshaler instance = new UTF8Marshaler();
        public static ICustomMarshaler GetInstance(string cookie)
        {
            return instance;
        }
    }

    public static class LogInterop
    {
        [DllImport("xxloglib", CallingConvention = CallingConvention.Cdecl)]
        internal static extern IntPtr XxLogNew(string fn);

        [return: MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(UTF8Marshaler))]
        [DllImport("xxloglib", CallingConvention = CallingConvention.Cdecl)]
        internal static extern string XxLogWriteAll(IntPtr ctx,
            int level,
            long time,
            [MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(UTF8Marshaler))]
            string machine,
            [MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(UTF8Marshaler))]
            string service,
            [MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(UTF8Marshaler))]
            string instanceId,
            [MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(UTF8Marshaler))]
            string title,
            long opcode,
            [MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(UTF8Marshaler))]
            string desc);

        [DllImport("xxloglib", CallingConvention = CallingConvention.Cdecl)]
        internal static extern void XxLogSetDefaultValue(IntPtr ctx,
            [MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(UTF8Marshaler))]
            string machine,
            [MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(UTF8Marshaler))]
            string service,
            [MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(UTF8Marshaler))]
            string instanceId);

        [return: MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(UTF8Marshaler))]
        [DllImport("xxloglib", CallingConvention = CallingConvention.Cdecl)]
        internal static extern string XxLogWrite(IntPtr ctx,
            int level,
            [MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(UTF8Marshaler))]
            string title,
            long opcode,
            [MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(UTF8Marshaler))]
            string desc);

        [DllImport("xxloglib", CallingConvention = CallingConvention.Cdecl)]
        internal static extern void XxLogDelete(IntPtr ctx);
    }

    public class Logger : IDisposable
    {
        public enum Level
        {
            Off = 0,
            Fatal = 1,
            Error = 2,
            Warning = 3,
            Info = 4,
            Debug = 5,
            All = 6,
        };

        IntPtr ctx;

        public Logger(string fn = null)
        {
            if (string.IsNullOrWhiteSpace(fn))
            {
                fn = Process.GetCurrentProcess().MainModule.FileName + ".log.db";
            }
            ctx = LogInterop.XxLogNew(fn);
            if (ctx == null)
            {
                throw new Exception("XxLogNew failed. fn = " + fn);
            }
        }

        public string WriteAll(Level level, long time, string machine, string service, string instanceId, string title, long opcode, string desc)
        {
            if (disposed) throw new ObjectDisposedException("Log");
            return LogInterop.XxLogWriteAll(ctx, (int)level, time, machine, service, instanceId, title, opcode, desc);
        }

        public void SetDefaultValue(string machine, string service, string instanceId)
        {
            if (disposed) throw new ObjectDisposedException("Log");
            LogInterop.XxLogSetDefaultValue(ctx, machine, service, instanceId);
        }

        public string Write(Level level, string title, long opcode, string desc)
        {
            if (disposed) throw new ObjectDisposedException("Log");
            return LogInterop.XxLogWrite(ctx, (int)level, title, opcode, desc);
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
                LogInterop.XxLogDelete(ctx);
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
}

public static class Program
{
    static void Main(string[] args)
    {
        var sw = Stopwatch.StartNew();
        using (var log = new xx.Logger())
        {
            for (int i = 0; i < 1000000; i++)
            {
                log.WriteAll(xx.Logger.Level.Info, DateTime.Now.Ticks, "pc1", "test_csharp3"
                    , Guid.NewGuid().ToString(), "asdfjlaskdjf", 22, "hi 中文hello中文hello !");
            }
        }
        Console.WriteLine(sw.ElapsedMilliseconds);
    }
}

