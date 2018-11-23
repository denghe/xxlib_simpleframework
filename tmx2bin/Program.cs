using System.Reflection;
using System.IO;
using System.Windows.Forms;
using System.Text;
using System.Security.Cryptography;
using System.Xml.Linq;
using System.Globalization;

public static class Program
{
    public static string rootDir = "E:\\he\\Desktop\\";
    public static string tmxFileName = "untitled.tmx";

    static void Main(string[] args)
    {
        var xDoc = XDocument.Load(Path.Combine(rootDir, tmxFileName));
        var xMap = xDoc.Element("map");
        var tMap = new TMX.Map();
        tMap.version = (string)xMap.Attribute("version");
        tMap.tiledversion = (string)xMap.Attribute("tiledversion");
        tMap.orientation = xMap.Attribute_orientation();
        tMap.renderorder = xMap.Attribute_renderorder();
        tMap.width = (int)xMap.Attribute("width");
        tMap.height = (int)xMap.Attribute("height");
        tMap.tilewidth = (int)xMap.Attribute("tilewidth");
        tMap.tileheight = (int)xMap.Attribute("tileheight");
        tMap.hexsidelength = (int?)xMap.Attribute("hexsidelength");
        tMap.infinite = (int?)xMap.Attribute("infinite");
        tMap.nextlayerid = (int?)xMap.Attribute("nextlayerid");
        tMap.nextobjectid = (int?)xMap.Attribute("nextobjectid");
        tMap.backgroundcolor = xMap.Attribute_Color4B("backgroundcolor");
        tMap.tilesets = xMap.Attribute_tileset();
        tMap.layers = xMap.Attribute_layer();
        tMap.objectgroups = xMap.Attribute_objectgroup();
        tMap.properties = xMap.Attribute_properties();
    }

    public static TMX.OrientationTypes Attribute_orientation(this XElement xe)
    {
        switch ((string)xe.Attribute("orientation"))
        {
            case "unknown": return TMX.OrientationTypes.Unknown;
            case "orthogonal": return TMX.OrientationTypes.Orthogonal;
            case "isometric": return TMX.OrientationTypes.Isometric;
            case "staggered": return TMX.OrientationTypes.Staggered;
            case "hexagonal": return TMX.OrientationTypes.Hexagonal;
            default: throw new System.Exception();
        }
    }

    public static TMX.RenderOrderTypes Attribute_renderorder(this XElement xe)
    {
        switch ((string)xe.Attribute("renderorder"))
        {
            case "right-down": return TMX.RenderOrderTypes.RightDown;
            case "right-up": return TMX.RenderOrderTypes.RightUp;
            case "left-down": return TMX.RenderOrderTypes.LeftDown;
            case "left-up": return TMX.RenderOrderTypes.LeftUp;
            default: throw new System.Exception();
        }
    }

    public static TMX.DrawOrderTypes Attribute_draworder(this XElement xe)
    {
        var v = (string)xe.Attribute("draworder");
        if (v == null) return TMX.DrawOrderTypes.TopDown;
        switch (v)
        {
            case "unknown": return TMX.DrawOrderTypes.Unknown;
            case "topdown": return TMX.DrawOrderTypes.TopDown;
            case "index": return TMX.DrawOrderTypes.Index;
            default: throw new System.Exception();
        }
    }

    public static TMX.Color4B Attribute_Color4B(this XElement xe, string attrName)
    {
        var c = new TMX.Color4B();
        var v = (string)xe.Attribute(attrName);
        if (v == null) return new TMX.Color4B { r = 0, g = 0, b = 0, a = 255 };
        var colorStr = v.TrimStart("#".ToCharArray());
        c.r = byte.Parse(colorStr.Substring(0, 2), NumberStyles.HexNumber);
        c.g = byte.Parse(colorStr.Substring(2, 2), NumberStyles.HexNumber);
        c.b = byte.Parse(colorStr.Substring(4, 2), NumberStyles.HexNumber);
        c.a = colorStr.Length > 6 ? byte.Parse(colorStr.Substring(6, 2), NumberStyles.HexNumber) : (byte)255;
        return c;
    }

    public static xx.List<TMX.TileSet> Attribute_tileset(this XElement xe)
    {
        var rtv = new xx.List<TMX.TileSet>();
        foreach (var e_ in xe.Elements("tileset"))
        {
            var e = e_;
            var ts = new TMX.TileSet();
            ts.firstgid = (int)e.Attribute("firstgid");
            ts.source = (string)e.Attribute("source");
            if (ts.source != null)
            {
                var xDoc = XDocument.Load(Path.Combine(rootDir, ts.source));
                e = xDoc.Element("tileset");
            }
            ts.name = (string)e.Attribute("name");
            ts.tilewidth = (int)e.Attribute("tilewidth");
            ts.tileheight = (int)e.Attribute("tileheight");
            ts.spacing = (int?)e.Attribute("spacing") ?? 0;
            ts.margin = (int?)e.Attribute("margin") ?? 0;
            ts.tilecount = (int?)e.Attribute("tilecount") ?? 0;
            ts.columns = (int?)e.Attribute("columns") ?? 0;
            var xy = e.Element("tileoffset");
            if (xy != null)
            {
                ts.tileoffsetx = (int)xy.Attribute("x");
                ts.tileoffsety = (int)xy.Attribute("y");
            }

            e = e.Element("image");
            ts.source = (string)e.Attribute("source");
            // 不支持内嵌图片
            ts.trans = e.Attribute_Color4B("trans");
            ts.width = (int)e.Attribute("width");
            ts.height = (int)e.Attribute("height");

            // terraintypes tile... 省略

            rtv.Add(ts);
        }
        return rtv;
    }

    public static xx.List<TMX.Layer> Attribute_layer(this XElement xe)
    {
        var rtv = new xx.List<TMX.Layer>();
        foreach (var e in xe.Elements("layer"))
        {
            var L = new TMX.Layer();
            L.id = (int?)e.Attribute("id");
            L.name = (string)e.Attribute("name");
            L.width = (int)e.Attribute("width");
            L.height = (int)e.Attribute("height");
            L.opacity = (double?)e.Attribute("opacity") ?? 1.0f;
            L.visible = (bool?)e.Attribute("opacity") ?? true;
            L.offsetx = (double?)e.Attribute("offsetx") ?? 0.0f;
            L.offsety = (double?)e.Attribute("offsety") ?? 0.0f;
        }
        return rtv;
    }

    public static xx.List<TMX.ObjectGroup> Attribute_objectgroup(this XElement xe)
    {
        var rtv = new xx.List<TMX.ObjectGroup>();
        foreach (var e in xe.Elements("objectgroup"))
        {
            var og = new TMX.ObjectGroup();
            og.id = (int?)e.Attribute("id");
            og.name = (string)e.Attribute("name");
            og.opacity = (double?)e.Attribute("opacity") ?? 1.0f;
            og.visible = (bool?)e.Attribute("opacity") ?? true;
            og.offsetx = (double?)e.Attribute("offsetx") ?? 0.0f;
            og.offsety = (double?)e.Attribute("offsety") ?? 0.0f;
            og.draworder = e.Attribute_draworder();
            og.objects = e.Attribute_object();
            og.properties = e.Attribute_properties();
        }
        return rtv;
    }

    public static xx.List<TMX.Property> Attribute_properties(this XElement xe)
    {
        var rtv = new xx.List<TMX.Property>();
        foreach (var e in xe.Elements("properties"))
        {
            //var p = new TMX.Property
            //rtv.Add()
        }
        return rtv;
    }

    public static xx.List<TMX.Object> Attribute_object(this XElement xe)
    {
        var rtv = new xx.List<TMX.Object>();
        foreach (var e in xe.Elements("object"))
        {
            var o = new TMX.Object();
            o.id = (int?)e.Attribute("id") ?? 0;
            o.name = (string)e.Attribute("name") ?? string.Empty;
            o.type = (string)e.Attribute("type") ?? string.Empty;
            o.visible = (bool?)e.Attribute("visible") ?? true;
            o.x = (double)e.Attribute("x");
            o.y = (double)e.Attribute("y");
            o.width = (int?)e.Attribute("width") ?? 0.0;
            o.height = (int?)e.Attribute("height") ?? 0.0;
            o.properties = e.Attribute_properties();

            var xGid = e.Attribute("gid");
            var xEllipse = e.Element("ellipse");
            var xPolygon = e.Element("polygon");
            var xPolyline = e.Element("polyline");

            //if (xGid != null)
            //{
            //    Tile = new TmxLayerTile((uint)xGid,
            //        Convert.ToInt32(Math.Round(X)),
            //        Convert.ToInt32(Math.Round(X)));
            //    ObjectType = TmxObjectType.Tile;
            //}
            //else if (xEllipse != null)
            //{
            //    ObjectType = TmxObjectType.Ellipse;
            //}
            //else if (xPolygon != null)
            //{
            //    Points = ParsePoints(xPolygon);
            //    ObjectType = TmxObjectType.Polygon;
            //}
            //else if (xPolyline != null)
            //{
            //    Points = ParsePoints(xPolyline);
            //    ObjectType = TmxObjectType.Polyline;
            //}
            //else ObjectType = TmxObjectType.Basic;

            //var xText = xObject.Element("text");
            //if (xText != null)
            //{
            //    Text = new TmxText(xText);
            //}

            //o.objtype = 

            rtv.Add(o);
        }
        return rtv;
    }
}
