using System;
using xx;
namespace PKG
{
    public static class PkgGenMd5
    {
        public const string value = "267d82049bdda2a37af33059b40eaf53"; 
    }

    public partial class Container : IObject
    {
        public List<Foo> foos;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<Container>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
            bb.Write(this.foos);
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
            bb.readLengthLimit = 0;
            bb.Read(ref this.foos);
        }
        public virtual void ToString(ref System.Text.StringBuilder str)
        {
            if (GetToStringFlag())
            {
        	    str.Append("[ \"***** recursived *****\" ]");
        	    return;
            }
            else SetToStringFlag(true);

            str.Append("{ \"pkgTypeName\":\"Container\", \"pkgTypeId\":" + GetPackageId());
            ToStringCore(ref str);
            str.Append(" }");
        
            SetToStringFlag(false);
        }
        public virtual void ToStringCore(ref System.Text.StringBuilder str)
        {
            str.Append(", \"foos\":" + (foos == null ? "nil" : foos.ToString()));
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
        public virtual void MySqlAppend(ref System.Text.StringBuilder sb, bool ignoreReadOnly)
        {
        }
    }
    public partial class Foo : IObject
    {
        public List<Foo> childs;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<Foo>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
            bb.Write(this.childs);
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
            bb.readLengthLimit = 0;
            bb.Read(ref this.childs);
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
            str.Append(", \"childs\":" + (childs == null ? "nil" : childs.ToString()));
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
        public virtual void MySqlAppend(ref System.Text.StringBuilder sb, bool ignoreReadOnly)
        {
        }
    }
    public partial class FooEx : Foo
    {
        public Foo parent;

        public override ushort GetPackageId()
        {
            return TypeIdMaps<FooEx>.typeId;
        }

        public override void ToBBuffer(BBuffer bb)
        {
            base.ToBBuffer(bb);
            bb.Write(this.parent);
        }

        public override void FromBBuffer(BBuffer bb)
        {
            base.FromBBuffer(bb);
            bb.Read(ref this.parent);
        }
        public override void ToString(ref System.Text.StringBuilder str)
        {
            if (GetToStringFlag())
            {
        	    str.Append("[ \"***** recursived *****\" ]");
        	    return;
            }
            else SetToStringFlag(true);

            str.Append("{ \"pkgTypeName\":\"FooEx\", \"pkgTypeId\":" + GetPackageId());
            ToStringCore(ref str);
            str.Append(" }");
        
            SetToStringFlag(false);
        }
        public override void ToStringCore(ref System.Text.StringBuilder str)
        {
            base.ToStringCore(ref str);
            str.Append(", \"parent\":" + (parent == null ? "nil" : parent.ToString()));
        }
        public override string ToString()
        {
            var sb = new System.Text.StringBuilder();
            ToString(ref sb);
            return sb.ToString();
        }
        public override void MySqlAppend(ref System.Text.StringBuilder sb, bool ignoreReadOnly)
        {
            base.MySqlAppend(ref sb, ignoreReadOnly);
        }
    }
    public static class AllTypes
    {
        public static void Register()
        {
            xx.BBuffer.RegisterInternals();
            BBuffer.Register<Container>(7);
            BBuffer.Register<List<Foo>>(4);
            BBuffer.Register<Foo>(3);
            BBuffer.Register<FooEx>(6);
        }
    }
}
