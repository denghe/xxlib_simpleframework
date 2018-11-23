#pragma warning disable 0169, 0414
using TemplateLibrary;

// tiled map 存档对应结构( 基于 1.2+ )

public enum OrientationTypes
{
    Unknown,
    Orthogonal,
    Isometric,
    Staggered,
    Hexagonal
}

public enum RenderOrderTypes
{
    RightDown,
    RightUp,
    LeftDown,
    LeftUp
}

public enum DrawOrderTypes
{
    Unknown = -1,
    TopDown,
    Index
}

public enum ObjectTypes
{
    Basic,
    Tile,
    Ellipse,
    Polygon,
    Polyline
}

struct Color4B
{
    byte r, g, b, a;
}

class Map
{
    string version;
    string tiledversion;
    OrientationTypes orientation;
    RenderOrderTypes renderorder;
    int width, height;
    int tilewidth, tileheight;
    int? hexsidelength;
    int? infinite;
    int? nextlayerid;
    int? nextobjectid;
    Color4B backgroundcolor;
    List<TileSet> tilesets;
    List<Layer> layers;
    List<ObjectGroup> objectgroups;
    // imagelayer, group
    List<Property> properties;
}

class Property
{
    string name, value;
}

class TileSet   // 简化版
{
    int firstgid;
    string name;
    int tilewidth, tileheight;
    int? spacing, margin;
    int? tilecount, columns;
    int? tileoffsetx, tileoffsety;

    // Image 内容展开
    string source;          // 图片文件名
    int width, height;
    Color4B trans;

    // terraintypes, tiles 省略
}
// TilesetTile 省略


class Layer
{
    int? id;
    string name;
    int width, height;
    double opacity;
    bool visible;
    double offsetx, offsety;
    List<Tile> data;
}

class Tile
{
    int gid;
    // x, y, flipx, flipy, DiagonalFlip
}

class ObjectGroup
{
    int? id;
    string name;
    Color4B color;
    double? opacity;
    bool? visible;
    double? offsetx, offsety;
    DrawOrderTypes draworder;
    List<Object> objects;
    List<Property> properties;
}

class Object
{
    int id;
    int? gid;
    double x, y;
    double width, height;
    string name;
    string type;
    string text;
    bool visible;
    double rotation;
    List<Property> properties;
    //ObjectTypes objtype;
}
