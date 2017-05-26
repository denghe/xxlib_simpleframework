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
        float x;
        float y;
    };
    struct SceneObj : xx::MPObject
    {
        typedef xx::MPObject BaseType;
        /// <summary>
        /// 所在场景
        /// </summary>
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

        // todo: inline virtual void ToString(xx::String &str) const override

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
        /// <summary>
        /// 位于 scene monster 容器中的下标 for 交换快删
        /// </summary>
        int32_t scene_monsters_index;
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
	    }

        // todo: inline virtual void ToString(xx::String &str) const override

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

        // todo: inline virtual void ToString(xx::String &str) const override

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

        // todo: inline virtual void ToString(xx::String &str) const override

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

        // todo: inline virtual void ToString(xx::String &str) const override

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
