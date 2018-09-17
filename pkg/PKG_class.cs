using System;
using xx;
namespace PKG
{
    public static class PkgGenMd5
    {
        public const string value = "bf0a438febf3b576bbe0485476196fd2"; 
    }

    public partial class Foo : xx.Object
    {
        public Foo foo;
        public Ref<Foo> refFoo;
        public List<Foo> foos;
        public List<Ref<Foo>> refFoos;

        public override ushort GetPackageId()
        {
            return TypeId<Foo>.value;
        }

        public override void ToBBuffer(BBuffer bb)
        {
            bb.Write(this.foo);
            bb.Write(this.refFoo);
            bb.Write(this.foos);
            bb.Write(this.refFoos);
        }

        public override void FromBBuffer(BBuffer bb)
        {
            bb.Read(ref this.foo);
            bb.Read(ref this.refFoo);
            bb.readLengthLimit = 0;
            bb.Read(ref this.foos);
            bb.readLengthLimit = 0;
            bb.Read(ref this.refFoos);
        }
        public override void ToString(System.Text.StringBuilder s)
        {
            if (__toStringing)
            {
        	    s.Append("[ \"***** recursived *****\" ]");
        	    return;
            }
            else __toStringing = true;

            s.Append("{ \"pkgTypeName\":\"Foo\", \"pkgTypeId\":" + GetPackageId());
            ToStringCore(s);
            s.Append(" }");

            __toStringing = false;
        }
        public override void ToStringCore(System.Text.StringBuilder s)
        {
            s.Append(", \"foo\":" + (foo == null ? "nil" : foo.ToString()));
            s.Append(", \"refFoo\":" + refFoo);
            s.Append(", \"foos\":" + (foos == null ? "nil" : foos.ToString()));
            s.Append(", \"refFoos\":" + (refFoos == null ? "nil" : refFoos.ToString()));
        }
        public override string ToString()
        {
            var sb = new System.Text.StringBuilder();
            ToString(sb);
            return sb.ToString();
        }
        public override void MySqlAppend(System.Text.StringBuilder sb, bool ignoreReadOnly)
        {
        }
    }
    public static class AllTypes
    {
        public static void Register()
        {
            xx.Object.RegisterInternals();
            xx.Object.Register<Foo>(3);
            xx.Object.Register<List<Foo>>(4);
            xx.Object.Register<List<Ref<Foo>>>(5);
        }
    }
}
