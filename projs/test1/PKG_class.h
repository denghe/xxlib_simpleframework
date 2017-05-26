#include <xx_mempool.h>
#include <xx_bbuffer.h>

namespace PKG
{
    struct Pos;
    struct SceneObj;
    struct Monster;
    struct Deamon;
    struct Butcher;
    struct Scene;
    enum class Color : int32_t
    {
        Red = 0,
        Blue = 1,
    };
    struct Pos
    {
        float x = 0;
        float y = 0;
    };
    struct SceneObj : xx::MPObject
    {
        typedef xx::MPObject BaseType;
        // 所在场景
        PKG::Scene* scene = nullptr;
#pragma region ctor, interface impls

	    SceneObj()
	    {
	    }
	    SceneObj(xx::BBuffer *bb)
	    {
            if (auto rtv = bb->Read(scene)) throw rtv;
	    }
	    ~SceneObj()
	    {
            mempool().SafeRelease(scene);
	    }

        inline virtual void ToString(xx::String &str) const override
        {
        	if (tsFlags())
        	{
        		str.Append("[ \"***** recursived *****\" ]");
        		return;
        	}
        	else tsFlags() = 1;

            str.Append("{ \"type\" : \"SceneObj\"");
            ToStringCore(str);
            str.Append(" }");
        
        	tsFlags() = 0;
        }
        inline virtual void ToStringCore(xx::String &str) const override
        {
            this->BaseType::ToStringCore(str);
            str.Append(", \"scene\" : ", this->scene);
        }


        inline virtual void ToBBuffer(xx::BBuffer &bb) const override
        {
            bb.Write(this->scene);
        }

        inline virtual int FromBBuffer(xx::BBuffer &bb) override
        {
            bb.Read(this->scene);
            return 0;
        }

#pragma endregion
    };
    struct Monster : PKG::SceneObj
    {
        typedef PKG::SceneObj BaseType;
        // 位于 scene monster 容器中的下标 for 交换快删
        int32_t scene_monsters_index = 0;
        xx::String* name = nullptr;
        PKG::Color color = PKG::Color::Blue;
        PKG::Pos pos;
#pragma region ctor, interface impls

	    Monster()
            : BaseType()
	    {
            mempool().CreateTo(name);
	    }
	    Monster(xx::BBuffer *bb)
            : BaseType(bb)
	    {
            if (auto rtv = bb->Read(name)) throw rtv;
	    }
	    ~Monster()
	    {
            mempool().SafeRelease(name);
	    }

        inline virtual void ToString(xx::String &str) const override
        {
        	if (tsFlags())
        	{
        		str.Append("[ \"***** recursived *****\" ]");
        		return;
        	}
        	else tsFlags() = 1;

            str.Append("{ \"type\" : \"Monster\"");
            ToStringCore(str);
            str.Append(" }");
        
        	tsFlags() = 0;
        }
        inline virtual void ToStringCore(xx::String &str) const override
        {
            this->BaseType::ToStringCore(str);
            str.Append(", \"scene_monsters_index\" : ", this->scene_monsters_index);
            str.Append(", \"name\" : ", this->name);
            str.Append(", \"color\" : ", this->color);
            str.Append(", \"pos\" : ", this->pos);
        }


        inline virtual void ToBBuffer(xx::BBuffer &bb) const override
        {
            this->BaseType::ToBBuffer(bb);
            bb.Write(this->scene_monsters_index);
            bb.Write(this->name);
            bb.Write(this->color);
            bb.Write(this->pos);
        }

        inline virtual int FromBBuffer(xx::BBuffer &bb) override
        {
            this->BaseType::FromBBuffer(bb);
            bb.Read(this->scene_monsters_index);
            bb.Read(this->name);
            bb.Read(this->color);
            bb.Read(this->pos);
            return 0;
        }

#pragma endregion
    };
    struct Deamon : PKG::Monster
    {
        typedef PKG::Monster BaseType;
#pragma region ctor, interface impls

	    Deamon()
            : BaseType()
	    {
	    }
	    Deamon(xx::BBuffer *bb)
            : BaseType(bb)
	    {
	    }
	    ~Deamon()
	    {
	    }

        inline virtual void ToString(xx::String &str) const override
        {
        	if (tsFlags())
        	{
        		str.Append("[ \"***** recursived *****\" ]");
        		return;
        	}
        	else tsFlags() = 1;

            str.Append("{ \"type\" : \"Deamon\"");
            ToStringCore(str);
            str.Append(" }");
        
        	tsFlags() = 0;
        }
        inline virtual void ToStringCore(xx::String &str) const override
        {
            this->BaseType::ToStringCore(str);
        }


        inline virtual void ToBBuffer(xx::BBuffer &bb) const override
        {
            this->BaseType::ToBBuffer(bb);
        }

        inline virtual int FromBBuffer(xx::BBuffer &bb) override
        {
            this->BaseType::FromBBuffer(bb);
            return 0;
        }

#pragma endregion
    };
    struct Butcher : PKG::Monster
    {
        typedef PKG::Monster BaseType;
#pragma region ctor, interface impls

	    Butcher()
            : BaseType()
	    {
	    }
	    Butcher(xx::BBuffer *bb)
            : BaseType(bb)
	    {
	    }
	    ~Butcher()
	    {
	    }

        inline virtual void ToString(xx::String &str) const override
        {
        	if (tsFlags())
        	{
        		str.Append("[ \"***** recursived *****\" ]");
        		return;
        	}
        	else tsFlags() = 1;

            str.Append("{ \"type\" : \"Butcher\"");
            ToStringCore(str);
            str.Append(" }");
        
        	tsFlags() = 0;
        }
        inline virtual void ToStringCore(xx::String &str) const override
        {
            this->BaseType::ToStringCore(str);
        }


        inline virtual void ToBBuffer(xx::BBuffer &bb) const override
        {
            this->BaseType::ToBBuffer(bb);
        }

        inline virtual int FromBBuffer(xx::BBuffer &bb) override
        {
            this->BaseType::FromBBuffer(bb);
            return 0;
        }

#pragma endregion
    };
    struct Scene : xx::MPObject
    {
        typedef xx::MPObject BaseType;
        xx::List<PKG::Monster*>* monsters = nullptr;
#pragma region ctor, interface impls

	    Scene()
	    {
            mempool().CreateTo(monsters);
	    }
	    Scene(xx::BBuffer *bb)
	    {
            if (auto rtv = bb->Read(monsters)) throw rtv;
	    }
	    ~Scene()
	    {
            mempool().SafeRelease(monsters);
	    }

        inline virtual void ToString(xx::String &str) const override
        {
        	if (tsFlags())
        	{
        		str.Append("[ \"***** recursived *****\" ]");
        		return;
        	}
        	else tsFlags() = 1;

            str.Append("{ \"type\" : \"Scene\"");
            ToStringCore(str);
            str.Append(" }");
        
        	tsFlags() = 0;
        }
        inline virtual void ToStringCore(xx::String &str) const override
        {
            this->BaseType::ToStringCore(str);
            str.Append(", \"monsters\" : ", this->monsters);
        }


        inline virtual void ToBBuffer(xx::BBuffer &bb) const override
        {
            bb.Write(this->monsters);
        }

        inline virtual int FromBBuffer(xx::BBuffer &bb) override
        {
            bb.Read(this->monsters);
            return 0;
        }

#pragma endregion
    };
}
namespace xx
{
	template<>
	struct BytesFunc<PKG::Pos, void>
	{
		static inline uint32_t Calc(PKG::Pos const &in)
		{
			return BBCalc(in.x, in.y);
		}
		static inline uint32_t WriteTo(char *dstBuf, PKG::Pos const &in)
		{
			return BBWriteTo(dstBuf, in.x, in.y);
		}
		static inline int ReadFrom(char const *srcBuf, uint32_t const &dataLen, uint32_t &offset, PKG::Pos &out)
		{
			return BBReadFrom(srcBuf, dataLen, offset, out.x, out.y);
		}
	};
	template<>
	struct StrFunc<PKG::Pos, void>
	{
		static inline uint32_t Calc(PKG::Pos const &in)
		{
			return StrCalc(in.x, in.y + 1010);
		}
		static inline uint32_t WriteTo(char *dstBuf, PKG::Pos const &in)
		{
			return StrWriteTo(dstBuf, "{ \"type\" : \"Pos\"", ", \"x\" : ", in.x, ", \"y\" : ", in.y, " }");
        }
    };
	template<> struct TypeId<PKG::SceneObj> { static const uint16_t value = 2; };
	template<> struct TypeId<PKG::Scene> { static const uint16_t value = 3; };
	template<> struct TypeId<PKG::Monster> { static const uint16_t value = 4; };
	template<> struct TypeId<PKG::Deamon> { static const uint16_t value = 5; };
	template<> struct TypeId<PKG::Butcher> { static const uint16_t value = 6; };
}
namespace PKG
{
	inline void RegisterTypes()
	{
	    xx::MemPool::Register<PKG::SceneObj, xx::MPObject>();
	    xx::MemPool::Register<PKG::Scene, xx::MPObject>();
	    xx::MemPool::Register<PKG::Monster, PKG::SceneObj>();
	    xx::MemPool::Register<PKG::Deamon, PKG::Monster>();
	    xx::MemPool::Register<PKG::Butcher, PKG::Monster>();
	}
}
