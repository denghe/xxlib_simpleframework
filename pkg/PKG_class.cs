using System;
namespace PKG
{
    public static class PkgGenMd5
    {
        public const string value = "49cedfea12ae8f5fede5915c00b29d10"; 
    }

    public partial class Foo : xx.Object
    {
        public xx.Ref<Foo> refFoo;
        public xx.List<xx.Ref<Foo>> refFoos;

        public override ushort GetPackageId()
        {
            return xx.TypeId<Foo>.value;
        }

        public override void ToBBuffer(xx.BBuffer bb)
        {
            bb.Write(this.refFoo);
            bb.Write(this.refFoos);
        }

        public override void FromBBuffer(xx.BBuffer bb)
        {
            bb.Read(ref this.refFoo);
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
            s.Append(", \"refFoo\":" + refFoo.ToString());
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
            xx.Object.Register<xx.List<xx.Ref<Foo>>>(5);
        }
    }
}
