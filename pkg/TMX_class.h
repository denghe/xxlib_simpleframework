#pragma once
#include "xx.h"

namespace TMX
{
	struct PkgGenMd5
	{
		static constexpr char const* value = "95f3af1091a0990d61078c22f92da1b5";
    };

    struct Color4B;
    using Color4B_p = xx::Ptr<Color4B>;
    using Color4B_r = xx::Ref<Color4B>;

    class Map;
    using Map_p = xx::Ptr<Map>;
    using Map_r = xx::Ref<Map>;

    class Property;
    using Property_p = xx::Ptr<Property>;
    using Property_r = xx::Ref<Property>;

    class TileSet;
    using TileSet_p = xx::Ptr<TileSet>;
    using TileSet_r = xx::Ref<TileSet>;

    class Layer;
    using Layer_p = xx::Ptr<Layer>;
    using Layer_r = xx::Ref<Layer>;

    class Tile;
    using Tile_p = xx::Ptr<Tile>;
    using Tile_r = xx::Ref<Tile>;

    class ObjectGroup;
    using ObjectGroup_p = xx::Ptr<ObjectGroup>;
    using ObjectGroup_r = xx::Ref<ObjectGroup>;

    class Object;
    using Object_p = xx::Ptr<Object>;
    using Object_r = xx::Ref<Object>;

    enum class OrientationTypes : int32_t
    {
        Unknown = 0,
        Orthogonal = 1,
        Isometric = 2,
        Staggered = 3,
        Hexagonal = 4,
    };
    enum class RenderOrderTypes : int32_t
    {
        RightDown = 0,
        RightUp = 1,
        LeftDown = 2,
        LeftUp = 3,
    };
    enum class DrawOrderTypes : int32_t
    {
        Unknown = -1,
        TopDown = 0,
        Index = 1,
    };
    enum class ObjectTypes : int32_t
    {
        Basic = 0,
        Tile = 1,
        Ellipse = 2,
        Polygon = 3,
        Polyline = 4,
    };
    struct Color4B
    {
        uint8_t r = 0;
        uint8_t g = 0;
        uint8_t b = 0;
        uint8_t a = 0;
    };
    class Map : public xx::Object
    {
    public:
        xx::String_p version;
        xx::String_p tiledversion;
        TMX::OrientationTypes orientation = (TMX::OrientationTypes)0;
        TMX::RenderOrderTypes renderorder = (TMX::RenderOrderTypes)0;
        int32_t width = 0;
        int32_t height = 0;
        int32_t tilewidth = 0;
        int32_t tileheight = 0;
        std::optional<int32_t> hexsidelength;
        std::optional<int32_t> infinite;
        std::optional<int32_t> nextlayerid;
        std::optional<int32_t> nextobjectid;
        TMX::Color4B backgroundcolor;
        xx::List_p<TMX::TileSet_p> tilesets;
        xx::List_p<TMX::Layer_p> layers;
        xx::List_p<TMX::ObjectGroup_p> objectgroups;
        xx::List_p<TMX::Property_p> properties;

        typedef Map ThisType;
        typedef xx::Object BaseType;
	    Map(xx::MemPool* const& mempool) noexcept;
	    Map(xx::BBuffer* const& bb);
		Map(Map const&) = delete;
		Map& operator=(Map const&) = delete;
        void ToString(xx::String& s) const noexcept override;
        void ToStringCore(xx::String& s) const noexcept override;
        void ToBBuffer(xx::BBuffer& bb) const noexcept override;
        int FromBBuffer(xx::BBuffer& bb) noexcept override;
        int FromBBufferCore(xx::BBuffer& bb) noexcept;
        void CopyTo(Map* const& o) const noexcept;
        Map* MakeCopy() const noexcept;
        Map_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
    };
    class Property : public xx::Object
    {
    public:
        xx::String_p name;
        xx::String_p value;

        typedef Property ThisType;
        typedef xx::Object BaseType;
	    Property(xx::MemPool* const& mempool) noexcept;
	    Property(xx::BBuffer* const& bb);
		Property(Property const&) = delete;
		Property& operator=(Property const&) = delete;
        void ToString(xx::String& s) const noexcept override;
        void ToStringCore(xx::String& s) const noexcept override;
        void ToBBuffer(xx::BBuffer& bb) const noexcept override;
        int FromBBuffer(xx::BBuffer& bb) noexcept override;
        int FromBBufferCore(xx::BBuffer& bb) noexcept;
        void CopyTo(Property* const& o) const noexcept;
        Property* MakeCopy() const noexcept;
        Property_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
    };
    class TileSet : public xx::Object
    {
    public:
        int32_t firstgid = 0;
        xx::String_p name;
        int32_t tilewidth = 0;
        int32_t tileheight = 0;
        std::optional<int32_t> spacing;
        std::optional<int32_t> margin;
        std::optional<int32_t> tilecount;
        std::optional<int32_t> columns;
        std::optional<int32_t> tileoffsetx;
        std::optional<int32_t> tileoffsety;
        xx::String_p source;
        int32_t width = 0;
        int32_t height = 0;
        TMX::Color4B trans;

        typedef TileSet ThisType;
        typedef xx::Object BaseType;
	    TileSet(xx::MemPool* const& mempool) noexcept;
	    TileSet(xx::BBuffer* const& bb);
		TileSet(TileSet const&) = delete;
		TileSet& operator=(TileSet const&) = delete;
        void ToString(xx::String& s) const noexcept override;
        void ToStringCore(xx::String& s) const noexcept override;
        void ToBBuffer(xx::BBuffer& bb) const noexcept override;
        int FromBBuffer(xx::BBuffer& bb) noexcept override;
        int FromBBufferCore(xx::BBuffer& bb) noexcept;
        void CopyTo(TileSet* const& o) const noexcept;
        TileSet* MakeCopy() const noexcept;
        TileSet_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
    };
    class Layer : public xx::Object
    {
    public:
        std::optional<int32_t> id;
        xx::String_p name;
        int32_t width = 0;
        int32_t height = 0;
        double opacity = 0;
        bool visible = false;
        double offsetx = 0;
        double offsety = 0;
        xx::List_p<TMX::Tile_p> data;

        typedef Layer ThisType;
        typedef xx::Object BaseType;
	    Layer(xx::MemPool* const& mempool) noexcept;
	    Layer(xx::BBuffer* const& bb);
		Layer(Layer const&) = delete;
		Layer& operator=(Layer const&) = delete;
        void ToString(xx::String& s) const noexcept override;
        void ToStringCore(xx::String& s) const noexcept override;
        void ToBBuffer(xx::BBuffer& bb) const noexcept override;
        int FromBBuffer(xx::BBuffer& bb) noexcept override;
        int FromBBufferCore(xx::BBuffer& bb) noexcept;
        void CopyTo(Layer* const& o) const noexcept;
        Layer* MakeCopy() const noexcept;
        Layer_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
    };
    class Tile : public xx::Object
    {
    public:
        int32_t gid = 0;

        typedef Tile ThisType;
        typedef xx::Object BaseType;
	    Tile(xx::MemPool* const& mempool) noexcept;
	    Tile(xx::BBuffer* const& bb);
		Tile(Tile const&) = delete;
		Tile& operator=(Tile const&) = delete;
        void ToString(xx::String& s) const noexcept override;
        void ToStringCore(xx::String& s) const noexcept override;
        void ToBBuffer(xx::BBuffer& bb) const noexcept override;
        int FromBBuffer(xx::BBuffer& bb) noexcept override;
        int FromBBufferCore(xx::BBuffer& bb) noexcept;
        void CopyTo(Tile* const& o) const noexcept;
        Tile* MakeCopy() const noexcept;
        Tile_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
    };
    class ObjectGroup : public xx::Object
    {
    public:
        std::optional<int32_t> id;
        xx::String_p name;
        TMX::Color4B color;
        std::optional<double> opacity;
        std::optional<bool> visible;
        std::optional<double> offsetx;
        std::optional<double> offsety;
        TMX::DrawOrderTypes draworder = (TMX::DrawOrderTypes)0;
        xx::List_p<TMX::Object_p> objects;
        xx::List_p<TMX::Property_p> properties;

        typedef ObjectGroup ThisType;
        typedef xx::Object BaseType;
	    ObjectGroup(xx::MemPool* const& mempool) noexcept;
	    ObjectGroup(xx::BBuffer* const& bb);
		ObjectGroup(ObjectGroup const&) = delete;
		ObjectGroup& operator=(ObjectGroup const&) = delete;
        void ToString(xx::String& s) const noexcept override;
        void ToStringCore(xx::String& s) const noexcept override;
        void ToBBuffer(xx::BBuffer& bb) const noexcept override;
        int FromBBuffer(xx::BBuffer& bb) noexcept override;
        int FromBBufferCore(xx::BBuffer& bb) noexcept;
        void CopyTo(ObjectGroup* const& o) const noexcept;
        ObjectGroup* MakeCopy() const noexcept;
        ObjectGroup_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
    };
    class Object : public xx::Object
    {
    public:
        int32_t id = 0;
        std::optional<int32_t> gid;
        double x = 0;
        double y = 0;
        double width = 0;
        double height = 0;
        xx::String_p name;
        xx::String_p type;
        xx::String_p text;
        bool visible = false;
        double rotation = 0;
        xx::List_p<TMX::Property_p> properties;

        typedef Object ThisType;
        typedef xx::Object BaseType;
	    Object(xx::MemPool* const& mempool) noexcept;
	    Object(xx::BBuffer* const& bb);
		Object(Object const&) = delete;
		Object& operator=(Object const&) = delete;
        void ToString(xx::String& s) const noexcept override;
        void ToStringCore(xx::String& s) const noexcept override;
        void ToBBuffer(xx::BBuffer& bb) const noexcept override;
        int FromBBuffer(xx::BBuffer& bb) noexcept override;
        int FromBBufferCore(xx::BBuffer& bb) noexcept;
        void CopyTo(Object* const& o) const noexcept;
        Object* MakeCopy() const noexcept;
        Object_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
    };
}
namespace xx
{
	template<>
	struct BytesFunc<TMX::Color4B, void>
	{
		static inline void WriteTo(BBuffer& bb, TMX::Color4B const& in) noexcept
		{
			bb.Write(in.r, in.g, in.b, in.a);
		}
		static inline int ReadFrom(BBuffer& bb, TMX::Color4B& out) noexcept
		{
			return bb.Read(out.r, out.g, out.b, out.a);
		}
	};
	template<>
	struct StrFunc<TMX::Color4B, void>
	{
		static inline void WriteTo(xx::String& s, TMX::Color4B const& in) noexcept
		{
			s.Append("{ \"structTypeName\":\"Color4B\"", ", \"r\":", in.r, ", \"g\":", in.g, ", \"b\":", in.b, ", \"a\":", in.a, " }");
        }
    };
	template<> struct TypeId<TMX::Map> { static const uint16_t value = 3; };
	template<> struct TypeId<xx::List<TMX::TileSet_p>> { static const uint16_t value = 4; };
	template<> struct TypeId<TMX::TileSet> { static const uint16_t value = 5; };
	template<> struct TypeId<xx::List<TMX::Layer_p>> { static const uint16_t value = 6; };
	template<> struct TypeId<TMX::Layer> { static const uint16_t value = 7; };
	template<> struct TypeId<xx::List<TMX::ObjectGroup_p>> { static const uint16_t value = 8; };
	template<> struct TypeId<TMX::ObjectGroup> { static const uint16_t value = 9; };
	template<> struct TypeId<xx::List<TMX::Property_p>> { static const uint16_t value = 10; };
	template<> struct TypeId<TMX::Property> { static const uint16_t value = 11; };
	template<> struct TypeId<xx::List<TMX::Tile_p>> { static const uint16_t value = 15; };
	template<> struct TypeId<TMX::Tile> { static const uint16_t value = 12; };
	template<> struct TypeId<xx::List<TMX::Object_p>> { static const uint16_t value = 13; };
	template<> struct TypeId<TMX::Object> { static const uint16_t value = 14; };
}
namespace TMX
{
	inline Map::Map(xx::MemPool* const& mempool) noexcept
        : xx::Object(mempool)
	{
	}
	inline Map::Map(xx::BBuffer* const& bb)
        : xx::Object(bb)
	{
        if (int r = FromBBufferCore(*bb)) throw r;
	}
    inline void Map::ToBBuffer(xx::BBuffer& bb) const noexcept
    {
        bb.Write(this->version);
        bb.Write(this->tiledversion);
        bb.Write(this->orientation);
        bb.Write(this->renderorder);
        bb.Write(this->width);
        bb.Write(this->height);
        bb.Write(this->tilewidth);
        bb.Write(this->tileheight);
        bb.Write(this->hexsidelength);
        bb.Write(this->infinite);
        bb.Write(this->nextlayerid);
        bb.Write(this->nextobjectid);
        bb.Write(this->backgroundcolor);
        bb.Write(this->tilesets);
        bb.Write(this->layers);
        bb.Write(this->objectgroups);
        bb.Write(this->properties);
    }
    inline int Map::FromBBuffer(xx::BBuffer& bb) noexcept
    {
        return this->FromBBufferCore(bb);
    }
    inline int Map::FromBBufferCore(xx::BBuffer& bb) noexcept
    {
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->version)) return r;
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->tiledversion)) return r;
        if (int r = bb.Read(this->orientation)) return r;
        if (int r = bb.Read(this->renderorder)) return r;
        if (int r = bb.Read(this->width)) return r;
        if (int r = bb.Read(this->height)) return r;
        if (int r = bb.Read(this->tilewidth)) return r;
        if (int r = bb.Read(this->tileheight)) return r;
        if (int r = bb.Read(this->hexsidelength)) return r;
        if (int r = bb.Read(this->infinite)) return r;
        if (int r = bb.Read(this->nextlayerid)) return r;
        if (int r = bb.Read(this->nextobjectid)) return r;
        if (int r = bb.Read(this->backgroundcolor)) return r;
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->tilesets)) return r;
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->layers)) return r;
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->objectgroups)) return r;
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->properties)) return r;
        return 0;
    }

    inline void Map::ToString(xx::String& s) const noexcept
    {
        if (this->memHeader().flags)
        {
        	s.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else this->memHeader().flags = 1;

        s.Append("{ \"pkgTypeName\":\"Map\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(s);
        s.Append(" }");
        
        this->memHeader().flags = 0;
    }
    inline void Map::ToStringCore(xx::String& s) const noexcept
    {
        this->BaseType::ToStringCore(s);
        if (this->version) s.Append(", \"version\":\"", this->version, "\"");
        else s.Append(", \"version\":nil");
        if (this->tiledversion) s.Append(", \"tiledversion\":\"", this->tiledversion, "\"");
        else s.Append(", \"tiledversion\":nil");
        s.Append(", \"orientation\":", this->orientation);
        s.Append(", \"renderorder\":", this->renderorder);
        s.Append(", \"width\":", this->width);
        s.Append(", \"height\":", this->height);
        s.Append(", \"tilewidth\":", this->tilewidth);
        s.Append(", \"tileheight\":", this->tileheight);
        s.Append(", \"hexsidelength\":", this->hexsidelength);
        s.Append(", \"infinite\":", this->infinite);
        s.Append(", \"nextlayerid\":", this->nextlayerid);
        s.Append(", \"nextobjectid\":", this->nextobjectid);
        s.Append(", \"backgroundcolor\":", this->backgroundcolor);
        s.Append(", \"tilesets\":", this->tilesets);
        s.Append(", \"layers\":", this->layers);
        s.Append(", \"objectgroups\":", this->objectgroups);
        s.Append(", \"properties\":", this->properties);
    }
    inline void Map::CopyTo(Map* const& o) const noexcept
    {
        o->version = this->version;
        o->tiledversion = this->tiledversion;
        o->orientation = this->orientation;
        o->renderorder = this->renderorder;
        o->width = this->width;
        o->height = this->height;
        o->tilewidth = this->tilewidth;
        o->tileheight = this->tileheight;
        o->hexsidelength = this->hexsidelength;
        o->infinite = this->infinite;
        o->nextlayerid = this->nextlayerid;
        o->nextobjectid = this->nextobjectid;
        o->backgroundcolor = this->backgroundcolor;
        o->tilesets = this->tilesets;
        o->layers = this->layers;
        o->objectgroups = this->objectgroups;
        o->properties = this->properties;
    }
    inline Map* Map::MakeCopy() const noexcept
    {
        auto o = mempool->MPCreate<Map>();
        this->CopyTo(o);
        return o;
    }
    inline Map_p Map::MakePtrCopy() const noexcept
    {
        return Map_p(this->MakeCopy());
    }

	inline Property::Property(xx::MemPool* const& mempool) noexcept
        : xx::Object(mempool)
	{
	}
	inline Property::Property(xx::BBuffer* const& bb)
        : xx::Object(bb)
	{
        if (int r = FromBBufferCore(*bb)) throw r;
	}
    inline void Property::ToBBuffer(xx::BBuffer& bb) const noexcept
    {
        bb.Write(this->name);
        bb.Write(this->value);
    }
    inline int Property::FromBBuffer(xx::BBuffer& bb) noexcept
    {
        return this->FromBBufferCore(bb);
    }
    inline int Property::FromBBufferCore(xx::BBuffer& bb) noexcept
    {
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->name)) return r;
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->value)) return r;
        return 0;
    }

    inline void Property::ToString(xx::String& s) const noexcept
    {
        if (this->memHeader().flags)
        {
        	s.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else this->memHeader().flags = 1;

        s.Append("{ \"pkgTypeName\":\"Property\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(s);
        s.Append(" }");
        
        this->memHeader().flags = 0;
    }
    inline void Property::ToStringCore(xx::String& s) const noexcept
    {
        this->BaseType::ToStringCore(s);
        if (this->name) s.Append(", \"name\":\"", this->name, "\"");
        else s.Append(", \"name\":nil");
        if (this->value) s.Append(", \"value\":\"", this->value, "\"");
        else s.Append(", \"value\":nil");
    }
    inline void Property::CopyTo(Property* const& o) const noexcept
    {
        o->name = this->name;
        o->value = this->value;
    }
    inline Property* Property::MakeCopy() const noexcept
    {
        auto o = mempool->MPCreate<Property>();
        this->CopyTo(o);
        return o;
    }
    inline Property_p Property::MakePtrCopy() const noexcept
    {
        return Property_p(this->MakeCopy());
    }

	inline TileSet::TileSet(xx::MemPool* const& mempool) noexcept
        : xx::Object(mempool)
	{
	}
	inline TileSet::TileSet(xx::BBuffer* const& bb)
        : xx::Object(bb)
	{
        if (int r = FromBBufferCore(*bb)) throw r;
	}
    inline void TileSet::ToBBuffer(xx::BBuffer& bb) const noexcept
    {
        bb.Write(this->firstgid);
        bb.Write(this->name);
        bb.Write(this->tilewidth);
        bb.Write(this->tileheight);
        bb.Write(this->spacing);
        bb.Write(this->margin);
        bb.Write(this->tilecount);
        bb.Write(this->columns);
        bb.Write(this->tileoffsetx);
        bb.Write(this->tileoffsety);
        bb.Write(this->source);
        bb.Write(this->width);
        bb.Write(this->height);
        bb.Write(this->trans);
    }
    inline int TileSet::FromBBuffer(xx::BBuffer& bb) noexcept
    {
        return this->FromBBufferCore(bb);
    }
    inline int TileSet::FromBBufferCore(xx::BBuffer& bb) noexcept
    {
        if (int r = bb.Read(this->firstgid)) return r;
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->name)) return r;
        if (int r = bb.Read(this->tilewidth)) return r;
        if (int r = bb.Read(this->tileheight)) return r;
        if (int r = bb.Read(this->spacing)) return r;
        if (int r = bb.Read(this->margin)) return r;
        if (int r = bb.Read(this->tilecount)) return r;
        if (int r = bb.Read(this->columns)) return r;
        if (int r = bb.Read(this->tileoffsetx)) return r;
        if (int r = bb.Read(this->tileoffsety)) return r;
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->source)) return r;
        if (int r = bb.Read(this->width)) return r;
        if (int r = bb.Read(this->height)) return r;
        if (int r = bb.Read(this->trans)) return r;
        return 0;
    }

    inline void TileSet::ToString(xx::String& s) const noexcept
    {
        if (this->memHeader().flags)
        {
        	s.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else this->memHeader().flags = 1;

        s.Append("{ \"pkgTypeName\":\"TileSet\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(s);
        s.Append(" }");
        
        this->memHeader().flags = 0;
    }
    inline void TileSet::ToStringCore(xx::String& s) const noexcept
    {
        this->BaseType::ToStringCore(s);
        s.Append(", \"firstgid\":", this->firstgid);
        if (this->name) s.Append(", \"name\":\"", this->name, "\"");
        else s.Append(", \"name\":nil");
        s.Append(", \"tilewidth\":", this->tilewidth);
        s.Append(", \"tileheight\":", this->tileheight);
        s.Append(", \"spacing\":", this->spacing);
        s.Append(", \"margin\":", this->margin);
        s.Append(", \"tilecount\":", this->tilecount);
        s.Append(", \"columns\":", this->columns);
        s.Append(", \"tileoffsetx\":", this->tileoffsetx);
        s.Append(", \"tileoffsety\":", this->tileoffsety);
        if (this->source) s.Append(", \"source\":\"", this->source, "\"");
        else s.Append(", \"source\":nil");
        s.Append(", \"width\":", this->width);
        s.Append(", \"height\":", this->height);
        s.Append(", \"trans\":", this->trans);
    }
    inline void TileSet::CopyTo(TileSet* const& o) const noexcept
    {
        o->firstgid = this->firstgid;
        o->name = this->name;
        o->tilewidth = this->tilewidth;
        o->tileheight = this->tileheight;
        o->spacing = this->spacing;
        o->margin = this->margin;
        o->tilecount = this->tilecount;
        o->columns = this->columns;
        o->tileoffsetx = this->tileoffsetx;
        o->tileoffsety = this->tileoffsety;
        o->source = this->source;
        o->width = this->width;
        o->height = this->height;
        o->trans = this->trans;
    }
    inline TileSet* TileSet::MakeCopy() const noexcept
    {
        auto o = mempool->MPCreate<TileSet>();
        this->CopyTo(o);
        return o;
    }
    inline TileSet_p TileSet::MakePtrCopy() const noexcept
    {
        return TileSet_p(this->MakeCopy());
    }

	inline Layer::Layer(xx::MemPool* const& mempool) noexcept
        : xx::Object(mempool)
	{
	}
	inline Layer::Layer(xx::BBuffer* const& bb)
        : xx::Object(bb)
	{
        if (int r = FromBBufferCore(*bb)) throw r;
	}
    inline void Layer::ToBBuffer(xx::BBuffer& bb) const noexcept
    {
        bb.Write(this->id);
        bb.Write(this->name);
        bb.Write(this->width);
        bb.Write(this->height);
        bb.Write(this->opacity);
        bb.Write(this->visible);
        bb.Write(this->offsetx);
        bb.Write(this->offsety);
        bb.Write(this->data);
    }
    inline int Layer::FromBBuffer(xx::BBuffer& bb) noexcept
    {
        return this->FromBBufferCore(bb);
    }
    inline int Layer::FromBBufferCore(xx::BBuffer& bb) noexcept
    {
        if (int r = bb.Read(this->id)) return r;
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->name)) return r;
        if (int r = bb.Read(this->width)) return r;
        if (int r = bb.Read(this->height)) return r;
        if (int r = bb.Read(this->opacity)) return r;
        if (int r = bb.Read(this->visible)) return r;
        if (int r = bb.Read(this->offsetx)) return r;
        if (int r = bb.Read(this->offsety)) return r;
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->data)) return r;
        return 0;
    }

    inline void Layer::ToString(xx::String& s) const noexcept
    {
        if (this->memHeader().flags)
        {
        	s.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else this->memHeader().flags = 1;

        s.Append("{ \"pkgTypeName\":\"Layer\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(s);
        s.Append(" }");
        
        this->memHeader().flags = 0;
    }
    inline void Layer::ToStringCore(xx::String& s) const noexcept
    {
        this->BaseType::ToStringCore(s);
        s.Append(", \"id\":", this->id);
        if (this->name) s.Append(", \"name\":\"", this->name, "\"");
        else s.Append(", \"name\":nil");
        s.Append(", \"width\":", this->width);
        s.Append(", \"height\":", this->height);
        s.Append(", \"opacity\":", this->opacity);
        s.Append(", \"visible\":", this->visible);
        s.Append(", \"offsetx\":", this->offsetx);
        s.Append(", \"offsety\":", this->offsety);
        s.Append(", \"data\":", this->data);
    }
    inline void Layer::CopyTo(Layer* const& o) const noexcept
    {
        o->id = this->id;
        o->name = this->name;
        o->width = this->width;
        o->height = this->height;
        o->opacity = this->opacity;
        o->visible = this->visible;
        o->offsetx = this->offsetx;
        o->offsety = this->offsety;
        o->data = this->data;
    }
    inline Layer* Layer::MakeCopy() const noexcept
    {
        auto o = mempool->MPCreate<Layer>();
        this->CopyTo(o);
        return o;
    }
    inline Layer_p Layer::MakePtrCopy() const noexcept
    {
        return Layer_p(this->MakeCopy());
    }

	inline Tile::Tile(xx::MemPool* const& mempool) noexcept
        : xx::Object(mempool)
	{
	}
	inline Tile::Tile(xx::BBuffer* const& bb)
        : xx::Object(bb)
	{
        if (int r = FromBBufferCore(*bb)) throw r;
	}
    inline void Tile::ToBBuffer(xx::BBuffer& bb) const noexcept
    {
        bb.Write(this->gid);
    }
    inline int Tile::FromBBuffer(xx::BBuffer& bb) noexcept
    {
        return this->FromBBufferCore(bb);
    }
    inline int Tile::FromBBufferCore(xx::BBuffer& bb) noexcept
    {
        if (int r = bb.Read(this->gid)) return r;
        return 0;
    }

    inline void Tile::ToString(xx::String& s) const noexcept
    {
        if (this->memHeader().flags)
        {
        	s.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else this->memHeader().flags = 1;

        s.Append("{ \"pkgTypeName\":\"Tile\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(s);
        s.Append(" }");
        
        this->memHeader().flags = 0;
    }
    inline void Tile::ToStringCore(xx::String& s) const noexcept
    {
        this->BaseType::ToStringCore(s);
        s.Append(", \"gid\":", this->gid);
    }
    inline void Tile::CopyTo(Tile* const& o) const noexcept
    {
        o->gid = this->gid;
    }
    inline Tile* Tile::MakeCopy() const noexcept
    {
        auto o = mempool->MPCreate<Tile>();
        this->CopyTo(o);
        return o;
    }
    inline Tile_p Tile::MakePtrCopy() const noexcept
    {
        return Tile_p(this->MakeCopy());
    }

	inline ObjectGroup::ObjectGroup(xx::MemPool* const& mempool) noexcept
        : xx::Object(mempool)
	{
	}
	inline ObjectGroup::ObjectGroup(xx::BBuffer* const& bb)
        : xx::Object(bb)
	{
        if (int r = FromBBufferCore(*bb)) throw r;
	}
    inline void ObjectGroup::ToBBuffer(xx::BBuffer& bb) const noexcept
    {
        bb.Write(this->id);
        bb.Write(this->name);
        bb.Write(this->color);
        bb.Write(this->opacity);
        bb.Write(this->visible);
        bb.Write(this->offsetx);
        bb.Write(this->offsety);
        bb.Write(this->draworder);
        bb.Write(this->objects);
        bb.Write(this->properties);
    }
    inline int ObjectGroup::FromBBuffer(xx::BBuffer& bb) noexcept
    {
        return this->FromBBufferCore(bb);
    }
    inline int ObjectGroup::FromBBufferCore(xx::BBuffer& bb) noexcept
    {
        if (int r = bb.Read(this->id)) return r;
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->name)) return r;
        if (int r = bb.Read(this->color)) return r;
        if (int r = bb.Read(this->opacity)) return r;
        if (int r = bb.Read(this->visible)) return r;
        if (int r = bb.Read(this->offsetx)) return r;
        if (int r = bb.Read(this->offsety)) return r;
        if (int r = bb.Read(this->draworder)) return r;
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->objects)) return r;
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->properties)) return r;
        return 0;
    }

    inline void ObjectGroup::ToString(xx::String& s) const noexcept
    {
        if (this->memHeader().flags)
        {
        	s.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else this->memHeader().flags = 1;

        s.Append("{ \"pkgTypeName\":\"ObjectGroup\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(s);
        s.Append(" }");
        
        this->memHeader().flags = 0;
    }
    inline void ObjectGroup::ToStringCore(xx::String& s) const noexcept
    {
        this->BaseType::ToStringCore(s);
        s.Append(", \"id\":", this->id);
        if (this->name) s.Append(", \"name\":\"", this->name, "\"");
        else s.Append(", \"name\":nil");
        s.Append(", \"color\":", this->color);
        s.Append(", \"opacity\":", this->opacity);
        s.Append(", \"visible\":", this->visible);
        s.Append(", \"offsetx\":", this->offsetx);
        s.Append(", \"offsety\":", this->offsety);
        s.Append(", \"draworder\":", this->draworder);
        s.Append(", \"objects\":", this->objects);
        s.Append(", \"properties\":", this->properties);
    }
    inline void ObjectGroup::CopyTo(ObjectGroup* const& o) const noexcept
    {
        o->id = this->id;
        o->name = this->name;
        o->color = this->color;
        o->opacity = this->opacity;
        o->visible = this->visible;
        o->offsetx = this->offsetx;
        o->offsety = this->offsety;
        o->draworder = this->draworder;
        o->objects = this->objects;
        o->properties = this->properties;
    }
    inline ObjectGroup* ObjectGroup::MakeCopy() const noexcept
    {
        auto o = mempool->MPCreate<ObjectGroup>();
        this->CopyTo(o);
        return o;
    }
    inline ObjectGroup_p ObjectGroup::MakePtrCopy() const noexcept
    {
        return ObjectGroup_p(this->MakeCopy());
    }

	inline Object::Object(xx::MemPool* const& mempool) noexcept
        : xx::Object(mempool)
	{
	}
	inline Object::Object(xx::BBuffer* const& bb)
        : xx::Object(bb)
	{
        if (int r = FromBBufferCore(*bb)) throw r;
	}
    inline void Object::ToBBuffer(xx::BBuffer& bb) const noexcept
    {
        bb.Write(this->id);
        bb.Write(this->gid);
        bb.Write(this->x);
        bb.Write(this->y);
        bb.Write(this->width);
        bb.Write(this->height);
        bb.Write(this->name);
        bb.Write(this->type);
        bb.Write(this->text);
        bb.Write(this->visible);
        bb.Write(this->rotation);
        bb.Write(this->properties);
    }
    inline int Object::FromBBuffer(xx::BBuffer& bb) noexcept
    {
        return this->FromBBufferCore(bb);
    }
    inline int Object::FromBBufferCore(xx::BBuffer& bb) noexcept
    {
        if (int r = bb.Read(this->id)) return r;
        if (int r = bb.Read(this->gid)) return r;
        if (int r = bb.Read(this->x)) return r;
        if (int r = bb.Read(this->y)) return r;
        if (int r = bb.Read(this->width)) return r;
        if (int r = bb.Read(this->height)) return r;
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->name)) return r;
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->type)) return r;
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->text)) return r;
        if (int r = bb.Read(this->visible)) return r;
        if (int r = bb.Read(this->rotation)) return r;
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->properties)) return r;
        return 0;
    }

    inline void Object::ToString(xx::String& s) const noexcept
    {
        if (this->memHeader().flags)
        {
        	s.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else this->memHeader().flags = 1;

        s.Append("{ \"pkgTypeName\":\"Object\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(s);
        s.Append(" }");
        
        this->memHeader().flags = 0;
    }
    inline void Object::ToStringCore(xx::String& s) const noexcept
    {
        this->BaseType::ToStringCore(s);
        s.Append(", \"id\":", this->id);
        s.Append(", \"gid\":", this->gid);
        s.Append(", \"x\":", this->x);
        s.Append(", \"y\":", this->y);
        s.Append(", \"width\":", this->width);
        s.Append(", \"height\":", this->height);
        if (this->name) s.Append(", \"name\":\"", this->name, "\"");
        else s.Append(", \"name\":nil");
        if (this->type) s.Append(", \"type\":\"", this->type, "\"");
        else s.Append(", \"type\":nil");
        if (this->text) s.Append(", \"text\":\"", this->text, "\"");
        else s.Append(", \"text\":nil");
        s.Append(", \"visible\":", this->visible);
        s.Append(", \"rotation\":", this->rotation);
        s.Append(", \"properties\":", this->properties);
    }
    inline void Object::CopyTo(Object* const& o) const noexcept
    {
        o->id = this->id;
        o->gid = this->gid;
        o->x = this->x;
        o->y = this->y;
        o->width = this->width;
        o->height = this->height;
        o->name = this->name;
        o->type = this->type;
        o->text = this->text;
        o->visible = this->visible;
        o->rotation = this->rotation;
        o->properties = this->properties;
    }
    inline Object* Object::MakeCopy() const noexcept
    {
        auto o = mempool->MPCreate<Object>();
        this->CopyTo(o);
        return o;
    }
    inline Object_p Object::MakePtrCopy() const noexcept
    {
        return Object_p(this->MakeCopy());
    }

}
namespace TMX
{
	inline void AllTypesRegister() noexcept
	{
        xx::MemPool::RegisterInternals();
	    xx::MemPool::Register<TMX::Map, xx::Object>();
	    xx::MemPool::Register<xx::List<TMX::TileSet_p>, xx::Object>();
	    xx::MemPool::Register<TMX::TileSet, xx::Object>();
	    xx::MemPool::Register<xx::List<TMX::Layer_p>, xx::Object>();
	    xx::MemPool::Register<TMX::Layer, xx::Object>();
	    xx::MemPool::Register<xx::List<TMX::ObjectGroup_p>, xx::Object>();
	    xx::MemPool::Register<TMX::ObjectGroup, xx::Object>();
	    xx::MemPool::Register<xx::List<TMX::Property_p>, xx::Object>();
	    xx::MemPool::Register<TMX::Property, xx::Object>();
	    xx::MemPool::Register<xx::List<TMX::Tile_p>, xx::Object>();
	    xx::MemPool::Register<TMX::Tile, xx::Object>();
	    xx::MemPool::Register<xx::List<TMX::Object_p>, xx::Object>();
	    xx::MemPool::Register<TMX::Object, xx::Object>();
	}
}
