using System;
namespace TMX
{
    public static class PkgGenMd5
    {
        public const string value = "95f3af1091a0990d61078c22f92da1b5"; 
    }

    public enum OrientationTypes : int
    {
        Unknown = 0,
        Orthogonal = 1,
        Isometric = 2,
        Staggered = 3,
        Hexagonal = 4,
    }
    public enum RenderOrderTypes : int
    {
        RightDown = 0,
        RightUp = 1,
        LeftDown = 2,
        LeftUp = 3,
    }
    public enum DrawOrderTypes : int
    {
        Unknown = -1,
        TopDown = 0,
        Index = 1,
    }
    public enum ObjectTypes : int
    {
        Basic = 0,
        Tile = 1,
        Ellipse = 2,
        Polygon = 3,
        Polyline = 4,
    }
    public partial struct Color4B : xx.IObject
    {
        public byte r;
        public byte g;
        public byte b;
        public byte a;

        public ushort GetPackageId()
        {
            return xx.TypeId<Color4B>.value;
        }

        public void ToBBuffer(xx.BBuffer bb)
        {
            bb.Write(this.r);
            bb.Write(this.g);
            bb.Write(this.b);
            bb.Write(this.a);
        }

        public void FromBBuffer(xx.BBuffer bb)
        {
            bb.Read(ref this.r);
            bb.Read(ref this.g);
            bb.Read(ref this.b);
            bb.Read(ref this.a);
        }
        public void ToString(System.Text.StringBuilder s)
        {
            s.Append("{ \"pkgTypeName\":\"Color4B\", \"pkgTypeId\":" + GetPackageId());
            ToStringCore(s);
            s.Append(" }");
        }
        public void ToStringCore(System.Text.StringBuilder s)
        {
            s.Append(", \"r\":" + r.ToString());
            s.Append(", \"g\":" + g.ToString());
            s.Append(", \"b\":" + b.ToString());
            s.Append(", \"a\":" + a.ToString());
        }
        public override string ToString()
        {
            var sb = new System.Text.StringBuilder();
            ToString(sb);
            return sb.ToString();
        }
    }
    public partial class Map : xx.Object
    {
        public string version;
        public string tiledversion;
        public OrientationTypes orientation;
        public RenderOrderTypes renderorder;
        public int width;
        public int height;
        public int tilewidth;
        public int tileheight;
        public int? hexsidelength;
        public int? infinite;
        public int? nextlayerid;
        public int? nextobjectid;
        public Color4B backgroundcolor;
        public xx.List<TileSet> tilesets;
        public xx.List<Layer> layers;
        public xx.List<ObjectGroup> objectgroups;
        public xx.List<Property> properties;

        public override ushort GetPackageId()
        {
            return xx.TypeId<Map>.value;
        }

        public override void ToBBuffer(xx.BBuffer bb)
        {
            bb.Write(this.version);
            bb.Write(this.tiledversion);
            bb.Write((int)this.orientation);
            bb.Write((int)this.renderorder);
            bb.Write(this.width);
            bb.Write(this.height);
            bb.Write(this.tilewidth);
            bb.Write(this.tileheight);
            bb.Write(this.hexsidelength);
            bb.Write(this.infinite);
            bb.Write(this.nextlayerid);
            bb.Write(this.nextobjectid);
            ((xx.IObject)this.backgroundcolor).ToBBuffer(bb);
            bb.Write(this.tilesets);
            bb.Write(this.layers);
            bb.Write(this.objectgroups);
            bb.Write(this.properties);
        }

        public override void FromBBuffer(xx.BBuffer bb)
        {
            bb.readLengthLimit = 0;
            bb.Read(ref this.version);
            bb.readLengthLimit = 0;
            bb.Read(ref this.tiledversion);
            {
                int tmp = 0;
                bb.Read(ref tmp);
                this.orientation = (OrientationTypes)tmp;
            }
            {
                int tmp = 0;
                bb.Read(ref tmp);
                this.renderorder = (RenderOrderTypes)tmp;
            }
            bb.Read(ref this.width);
            bb.Read(ref this.height);
            bb.Read(ref this.tilewidth);
            bb.Read(ref this.tileheight);
            bb.Read(ref this.hexsidelength);
            bb.Read(ref this.infinite);
            bb.Read(ref this.nextlayerid);
            bb.Read(ref this.nextobjectid);
            ((xx.IObject)this.backgroundcolor).FromBBuffer(bb);
            bb.readLengthLimit = 0;
            bb.Read(ref this.tilesets);
            bb.readLengthLimit = 0;
            bb.Read(ref this.layers);
            bb.readLengthLimit = 0;
            bb.Read(ref this.objectgroups);
            bb.readLengthLimit = 0;
            bb.Read(ref this.properties);
        }
        public override void ToString(System.Text.StringBuilder s)
        {
            if (__toStringing)
            {
        	    s.Append("[ \"***** recursived *****\" ]");
        	    return;
            }
            else __toStringing = true;

            s.Append("{ \"pkgTypeName\":\"Map\", \"pkgTypeId\":" + GetPackageId());
            ToStringCore(s);
            s.Append(" }");

            __toStringing = false;
        }
        public override void ToStringCore(System.Text.StringBuilder s)
        {
            if (version != null) s.Append(", \"version\":\"" + version.ToString() + "\"");
            else s.Append(", \"version\":nil");
            if (tiledversion != null) s.Append(", \"tiledversion\":\"" + tiledversion.ToString() + "\"");
            else s.Append(", \"tiledversion\":nil");
            s.Append(", \"orientation\":" + orientation.ToString());
            s.Append(", \"renderorder\":" + renderorder.ToString());
            s.Append(", \"width\":" + width.ToString());
            s.Append(", \"height\":" + height.ToString());
            s.Append(", \"tilewidth\":" + tilewidth.ToString());
            s.Append(", \"tileheight\":" + tileheight.ToString());
            s.Append(", \"hexsidelength\":" + (hexsidelength.HasValue ? hexsidelength.Value.ToString() : "nil"));
            s.Append(", \"infinite\":" + (infinite.HasValue ? infinite.Value.ToString() : "nil"));
            s.Append(", \"nextlayerid\":" + (nextlayerid.HasValue ? nextlayerid.Value.ToString() : "nil"));
            s.Append(", \"nextobjectid\":" + (nextobjectid.HasValue ? nextobjectid.Value.ToString() : "nil"));
            s.Append(", \"backgroundcolor\":" + backgroundcolor.ToString());
            s.Append(", \"tilesets\":" + (tilesets == null ? "nil" : tilesets.ToString()));
            s.Append(", \"layers\":" + (layers == null ? "nil" : layers.ToString()));
            s.Append(", \"objectgroups\":" + (objectgroups == null ? "nil" : objectgroups.ToString()));
            s.Append(", \"properties\":" + (properties == null ? "nil" : properties.ToString()));
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
    public partial class Property : xx.Object
    {
        public string name;
        public string value;

        public override ushort GetPackageId()
        {
            return xx.TypeId<Property>.value;
        }

        public override void ToBBuffer(xx.BBuffer bb)
        {
            bb.Write(this.name);
            bb.Write(this.value);
        }

        public override void FromBBuffer(xx.BBuffer bb)
        {
            bb.readLengthLimit = 0;
            bb.Read(ref this.name);
            bb.readLengthLimit = 0;
            bb.Read(ref this.value);
        }
        public override void ToString(System.Text.StringBuilder s)
        {
            if (__toStringing)
            {
        	    s.Append("[ \"***** recursived *****\" ]");
        	    return;
            }
            else __toStringing = true;

            s.Append("{ \"pkgTypeName\":\"Property\", \"pkgTypeId\":" + GetPackageId());
            ToStringCore(s);
            s.Append(" }");

            __toStringing = false;
        }
        public override void ToStringCore(System.Text.StringBuilder s)
        {
            if (name != null) s.Append(", \"name\":\"" + name.ToString() + "\"");
            else s.Append(", \"name\":nil");
            if (value != null) s.Append(", \"value\":\"" + value.ToString() + "\"");
            else s.Append(", \"value\":nil");
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
    public partial class TileSet : xx.Object
    {
        public int firstgid;
        public string name;
        public int tilewidth;
        public int tileheight;
        public int? spacing;
        public int? margin;
        public int? tilecount;
        public int? columns;
        public int? tileoffsetx;
        public int? tileoffsety;
        public string source;
        public int width;
        public int height;
        public Color4B trans;

        public override ushort GetPackageId()
        {
            return xx.TypeId<TileSet>.value;
        }

        public override void ToBBuffer(xx.BBuffer bb)
        {
            bb.Write(this.firstgid);
            bb.Write(this.name);
            bb.Write(this.tilewidth);
            bb.Write(this.tileheight);
            bb.Write(this.spacing);
            bb.Write(this.margin);
            bb.Write(this.tilecount);
            bb.Write(this.columns);
            bb.Write(this.tileoffsetx);
            bb.Write(this.tileoffsety);
            bb.Write(this.source);
            bb.Write(this.width);
            bb.Write(this.height);
            ((xx.IObject)this.trans).ToBBuffer(bb);
        }

        public override void FromBBuffer(xx.BBuffer bb)
        {
            bb.Read(ref this.firstgid);
            bb.readLengthLimit = 0;
            bb.Read(ref this.name);
            bb.Read(ref this.tilewidth);
            bb.Read(ref this.tileheight);
            bb.Read(ref this.spacing);
            bb.Read(ref this.margin);
            bb.Read(ref this.tilecount);
            bb.Read(ref this.columns);
            bb.Read(ref this.tileoffsetx);
            bb.Read(ref this.tileoffsety);
            bb.readLengthLimit = 0;
            bb.Read(ref this.source);
            bb.Read(ref this.width);
            bb.Read(ref this.height);
            ((xx.IObject)this.trans).FromBBuffer(bb);
        }
        public override void ToString(System.Text.StringBuilder s)
        {
            if (__toStringing)
            {
        	    s.Append("[ \"***** recursived *****\" ]");
        	    return;
            }
            else __toStringing = true;

            s.Append("{ \"pkgTypeName\":\"TileSet\", \"pkgTypeId\":" + GetPackageId());
            ToStringCore(s);
            s.Append(" }");

            __toStringing = false;
        }
        public override void ToStringCore(System.Text.StringBuilder s)
        {
            s.Append(", \"firstgid\":" + firstgid.ToString());
            if (name != null) s.Append(", \"name\":\"" + name.ToString() + "\"");
            else s.Append(", \"name\":nil");
            s.Append(", \"tilewidth\":" + tilewidth.ToString());
            s.Append(", \"tileheight\":" + tileheight.ToString());
            s.Append(", \"spacing\":" + (spacing.HasValue ? spacing.Value.ToString() : "nil"));
            s.Append(", \"margin\":" + (margin.HasValue ? margin.Value.ToString() : "nil"));
            s.Append(", \"tilecount\":" + (tilecount.HasValue ? tilecount.Value.ToString() : "nil"));
            s.Append(", \"columns\":" + (columns.HasValue ? columns.Value.ToString() : "nil"));
            s.Append(", \"tileoffsetx\":" + (tileoffsetx.HasValue ? tileoffsetx.Value.ToString() : "nil"));
            s.Append(", \"tileoffsety\":" + (tileoffsety.HasValue ? tileoffsety.Value.ToString() : "nil"));
            if (source != null) s.Append(", \"source\":\"" + source.ToString() + "\"");
            else s.Append(", \"source\":nil");
            s.Append(", \"width\":" + width.ToString());
            s.Append(", \"height\":" + height.ToString());
            s.Append(", \"trans\":" + trans.ToString());
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
    public partial class Layer : xx.Object
    {
        public int? id;
        public string name;
        public int width;
        public int height;
        public double opacity;
        public bool visible = false;
        public double offsetx;
        public double offsety;
        public xx.List<Tile> data;

        public override ushort GetPackageId()
        {
            return xx.TypeId<Layer>.value;
        }

        public override void ToBBuffer(xx.BBuffer bb)
        {
            bb.Write(this.id);
            bb.Write(this.name);
            bb.Write(this.width);
            bb.Write(this.height);
            bb.Write(this.opacity);
            bb.Write(this.visible);
            bb.Write(this.offsetx);
            bb.Write(this.offsety);
            bb.Write(this.data);
        }

        public override void FromBBuffer(xx.BBuffer bb)
        {
            bb.Read(ref this.id);
            bb.readLengthLimit = 0;
            bb.Read(ref this.name);
            bb.Read(ref this.width);
            bb.Read(ref this.height);
            bb.Read(ref this.opacity);
            bb.Read(ref this.visible);
            bb.Read(ref this.offsetx);
            bb.Read(ref this.offsety);
            bb.readLengthLimit = 0;
            bb.Read(ref this.data);
        }
        public override void ToString(System.Text.StringBuilder s)
        {
            if (__toStringing)
            {
        	    s.Append("[ \"***** recursived *****\" ]");
        	    return;
            }
            else __toStringing = true;

            s.Append("{ \"pkgTypeName\":\"Layer\", \"pkgTypeId\":" + GetPackageId());
            ToStringCore(s);
            s.Append(" }");

            __toStringing = false;
        }
        public override void ToStringCore(System.Text.StringBuilder s)
        {
            s.Append(", \"id\":" + (id.HasValue ? id.Value.ToString() : "nil"));
            if (name != null) s.Append(", \"name\":\"" + name.ToString() + "\"");
            else s.Append(", \"name\":nil");
            s.Append(", \"width\":" + width.ToString());
            s.Append(", \"height\":" + height.ToString());
            s.Append(", \"opacity\":" + opacity.ToString());
            s.Append(", \"visible\":" + visible.ToString());
            s.Append(", \"offsetx\":" + offsetx.ToString());
            s.Append(", \"offsety\":" + offsety.ToString());
            s.Append(", \"data\":" + (data == null ? "nil" : data.ToString()));
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
    public partial class Tile : xx.Object
    {
        public int gid;

        public override ushort GetPackageId()
        {
            return xx.TypeId<Tile>.value;
        }

        public override void ToBBuffer(xx.BBuffer bb)
        {
            bb.Write(this.gid);
        }

        public override void FromBBuffer(xx.BBuffer bb)
        {
            bb.Read(ref this.gid);
        }
        public override void ToString(System.Text.StringBuilder s)
        {
            if (__toStringing)
            {
        	    s.Append("[ \"***** recursived *****\" ]");
        	    return;
            }
            else __toStringing = true;

            s.Append("{ \"pkgTypeName\":\"Tile\", \"pkgTypeId\":" + GetPackageId());
            ToStringCore(s);
            s.Append(" }");

            __toStringing = false;
        }
        public override void ToStringCore(System.Text.StringBuilder s)
        {
            s.Append(", \"gid\":" + gid.ToString());
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
    public partial class ObjectGroup : xx.Object
    {
        public int? id;
        public string name;
        public Color4B color;
        public double? opacity;
        public bool? visible;
        public double? offsetx;
        public double? offsety;
        public DrawOrderTypes draworder;
        public xx.List<Object> objects;
        public xx.List<Property> properties;

        public override ushort GetPackageId()
        {
            return xx.TypeId<ObjectGroup>.value;
        }

        public override void ToBBuffer(xx.BBuffer bb)
        {
            bb.Write(this.id);
            bb.Write(this.name);
            ((xx.IObject)this.color).ToBBuffer(bb);
            bb.Write(this.opacity);
            bb.Write(this.visible);
            bb.Write(this.offsetx);
            bb.Write(this.offsety);
            bb.Write((int)this.draworder);
            bb.Write(this.objects);
            bb.Write(this.properties);
        }

        public override void FromBBuffer(xx.BBuffer bb)
        {
            bb.Read(ref this.id);
            bb.readLengthLimit = 0;
            bb.Read(ref this.name);
            ((xx.IObject)this.color).FromBBuffer(bb);
            bb.Read(ref this.opacity);
            bb.Read(ref this.visible);
            bb.Read(ref this.offsetx);
            bb.Read(ref this.offsety);
            {
                int tmp = 0;
                bb.Read(ref tmp);
                this.draworder = (DrawOrderTypes)tmp;
            }
            bb.readLengthLimit = 0;
            bb.Read(ref this.objects);
            bb.readLengthLimit = 0;
            bb.Read(ref this.properties);
        }
        public override void ToString(System.Text.StringBuilder s)
        {
            if (__toStringing)
            {
        	    s.Append("[ \"***** recursived *****\" ]");
        	    return;
            }
            else __toStringing = true;

            s.Append("{ \"pkgTypeName\":\"ObjectGroup\", \"pkgTypeId\":" + GetPackageId());
            ToStringCore(s);
            s.Append(" }");

            __toStringing = false;
        }
        public override void ToStringCore(System.Text.StringBuilder s)
        {
            s.Append(", \"id\":" + (id.HasValue ? id.Value.ToString() : "nil"));
            if (name != null) s.Append(", \"name\":\"" + name.ToString() + "\"");
            else s.Append(", \"name\":nil");
            s.Append(", \"color\":" + color.ToString());
            s.Append(", \"opacity\":" + (opacity.HasValue ? opacity.Value.ToString() : "nil"));
            s.Append(", \"visible\":" + (visible.HasValue ? visible.Value.ToString() : "nil"));
            s.Append(", \"offsetx\":" + (offsetx.HasValue ? offsetx.Value.ToString() : "nil"));
            s.Append(", \"offsety\":" + (offsety.HasValue ? offsety.Value.ToString() : "nil"));
            s.Append(", \"draworder\":" + draworder.ToString());
            s.Append(", \"objects\":" + (objects == null ? "nil" : objects.ToString()));
            s.Append(", \"properties\":" + (properties == null ? "nil" : properties.ToString()));
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
    public partial class Object : xx.Object
    {
        public int id;
        public int? gid;
        public double x;
        public double y;
        public double width;
        public double height;
        public string name;
        public string type;
        public string text;
        public bool visible = false;
        public double rotation;
        public xx.List<Property> properties;

        public override ushort GetPackageId()
        {
            return xx.TypeId<Object>.value;
        }

        public override void ToBBuffer(xx.BBuffer bb)
        {
            bb.Write(this.id);
            bb.Write(this.gid);
            bb.Write(this.x);
            bb.Write(this.y);
            bb.Write(this.width);
            bb.Write(this.height);
            bb.Write(this.name);
            bb.Write(this.type);
            bb.Write(this.text);
            bb.Write(this.visible);
            bb.Write(this.rotation);
            bb.Write(this.properties);
        }

        public override void FromBBuffer(xx.BBuffer bb)
        {
            bb.Read(ref this.id);
            bb.Read(ref this.gid);
            bb.Read(ref this.x);
            bb.Read(ref this.y);
            bb.Read(ref this.width);
            bb.Read(ref this.height);
            bb.readLengthLimit = 0;
            bb.Read(ref this.name);
            bb.readLengthLimit = 0;
            bb.Read(ref this.type);
            bb.readLengthLimit = 0;
            bb.Read(ref this.text);
            bb.Read(ref this.visible);
            bb.Read(ref this.rotation);
            bb.readLengthLimit = 0;
            bb.Read(ref this.properties);
        }
        public override void ToString(System.Text.StringBuilder s)
        {
            if (__toStringing)
            {
        	    s.Append("[ \"***** recursived *****\" ]");
        	    return;
            }
            else __toStringing = true;

            s.Append("{ \"pkgTypeName\":\"Object\", \"pkgTypeId\":" + GetPackageId());
            ToStringCore(s);
            s.Append(" }");

            __toStringing = false;
        }
        public override void ToStringCore(System.Text.StringBuilder s)
        {
            s.Append(", \"id\":" + id.ToString());
            s.Append(", \"gid\":" + (gid.HasValue ? gid.Value.ToString() : "nil"));
            s.Append(", \"x\":" + x.ToString());
            s.Append(", \"y\":" + y.ToString());
            s.Append(", \"width\":" + width.ToString());
            s.Append(", \"height\":" + height.ToString());
            if (name != null) s.Append(", \"name\":\"" + name.ToString() + "\"");
            else s.Append(", \"name\":nil");
            if (type != null) s.Append(", \"type\":\"" + type.ToString() + "\"");
            else s.Append(", \"type\":nil");
            if (text != null) s.Append(", \"text\":\"" + text.ToString() + "\"");
            else s.Append(", \"text\":nil");
            s.Append(", \"visible\":" + visible.ToString());
            s.Append(", \"rotation\":" + rotation.ToString());
            s.Append(", \"properties\":" + (properties == null ? "nil" : properties.ToString()));
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
            xx.Object.Register<Map>(3);
            xx.Object.Register<xx.List<TileSet>>(4);
            xx.Object.Register<TileSet>(5);
            xx.Object.Register<xx.List<Layer>>(6);
            xx.Object.Register<Layer>(7);
            xx.Object.Register<xx.List<ObjectGroup>>(8);
            xx.Object.Register<ObjectGroup>(9);
            xx.Object.Register<xx.List<Property>>(10);
            xx.Object.Register<Property>(11);
            xx.Object.Register<xx.List<Tile>>(15);
            xx.Object.Register<Tile>(12);
            xx.Object.Register<xx.List<Object>>(13);
            xx.Object.Register<Object>(14);
        }
    }
}
