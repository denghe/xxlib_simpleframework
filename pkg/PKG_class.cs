using System;
using xx;
namespace PKG
{
    public static class PkgGenMd5
    {
        public const string value = "e28748eae8453c2ad9510602b5ce2828"; 
    }

    public partial class Foo : IBBuffer
    {
        public int id;
        public int? age;
        public List<float?> floats;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<Foo>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
            bb.Write(this.id);
            bb.Write(this.age);
            bb.Write(this.floats);
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
            bb.Read(ref this.id);
            bb.Read(ref this.age);
            bb.readLengthLimit = 0;
            bb.Read(ref this.floats);
        }
        public virtual void ToString(ref System.Text.StringBuilder str)
        {
            if (GetToStringFlag())
            {
        	    str.Append("[ \"***** recursived *****\" ]");
        	    return;
            }
            else SetToStringFlag(true);

            str.Append("{ \"pkgTypeName\":\"Foo\", \"pkgTypeId\":" + GetPackageId());
            ToStringCore(ref str);
            str.Append(" }");
        
            SetToStringFlag(false);
        }
        public virtual void ToStringCore(ref System.Text.StringBuilder str)
        {
            str.Append(", \"id\":" + id);
            str.Append(", \"age\":" + (age.HasValue ? age.Value.ToString() : "nil"));
            str.Append(", \"floats\":" + (floats == null ? "nil" : floats.ToString()));
        }
        public override string ToString()
        {
            var sb = new System.Text.StringBuilder();
            ToString(ref sb);
            return sb.ToString();
        }
        bool toStringFlag;
        public void SetToStringFlag(bool doing)
        {
            toStringFlag = doing;
        }
        public bool GetToStringFlag()
        {
            return toStringFlag;
        }
    }
    public static class AllTypes
    {
        public static void Register()
        {
            BBuffer.Register<Foo>(3);
            BBuffer.Register<List<float?>>(4);
        }
    }
}
