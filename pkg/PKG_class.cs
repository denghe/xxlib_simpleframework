using System;
using xx;
namespace PKG
{
    public static class PkgGenMd5
    {
        public const string value = "c4dfb0ecd26efc3cdfa643f478b61473"; 
    }

    public partial class Foo : xx.Object
    {
        public Ref<Foo> foo;
        public List<Ref<Foo>> foos;

        public override ushort GetPackageId()
        {
            return TypeId<Foo>.value;
        }

        public override void ToBBuffer(BBuffer bb)
        {
            bb.Write(this.foo);
            bb.Write(this.foos);
        }

        public override void FromBBuffer(BBuffer bb)
        {
            bb.Read(ref this.foo);
            bb.readLengthLimit = 0;
            bb.Read(ref this.foos);
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
            s.Append(", \"foo\":" + (!foo ? "nil" : foo.ToString()));
            s.Append(", \"foos\":" + (foos == null ? "nil" : foos.ToString()));
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
            xx.Object.Register<List<Ref<Foo>>>(4);
        }
    }
}
