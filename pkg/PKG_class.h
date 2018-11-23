#pragma once
#include "xx.h"

namespace PKG
{
	struct PkgGenMd5
	{
		static constexpr char const* value = "cbccc319ed09be93c69184ff1bf22cc9";
    };

    class Player;
    using Player_p = xx::Ptr<Player>;
    using Player_r = xx::Ref<Player>;

    class Scene;
    using Scene_p = xx::Ptr<Scene>;
    using Scene_r = xx::Ref<Scene>;

    class Player : public xx::Object
    {
    public:
        int32_t id = 0;
        xx::String_p name;
        PKG::Scene_p owner;

        typedef Player ThisType;
        typedef xx::Object BaseType;
	    Player(xx::MemPool* const& mempool) noexcept;
	    Player(xx::BBuffer* const& bb);
		Player(Player const&) = delete;
		Player& operator=(Player const&) = delete;
        void ToString(xx::String& s) const noexcept override;
        void ToStringCore(xx::String& s) const noexcept override;
        void ToBBuffer(xx::BBuffer& bb) const noexcept override;
        int FromBBuffer(xx::BBuffer& bb) noexcept override;
        int FromBBufferCore(xx::BBuffer& bb) noexcept;
        void CopyTo(Player* const& o) const noexcept;
        Player* MakeCopy() const noexcept;
        Player_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
    };
    class Scene : public xx::Object
    {
    public:
        xx::List_p<xx::Ref<PKG::Player>> players;

        typedef Scene ThisType;
        typedef xx::Object BaseType;
	    Scene(xx::MemPool* const& mempool) noexcept;
	    Scene(xx::BBuffer* const& bb);
		Scene(Scene const&) = delete;
		Scene& operator=(Scene const&) = delete;
        void ToString(xx::String& s) const noexcept override;
        void ToStringCore(xx::String& s) const noexcept override;
        void ToBBuffer(xx::BBuffer& bb) const noexcept override;
        int FromBBuffer(xx::BBuffer& bb) noexcept override;
        int FromBBufferCore(xx::BBuffer& bb) noexcept;
        void CopyTo(Scene* const& o) const noexcept;
        Scene* MakeCopy() const noexcept;
        Scene_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
    };
}
namespace xx
{
	template<> struct TypeId<PKG::Player> { static const uint16_t value = 3; };
	template<> struct TypeId<PKG::Scene> { static const uint16_t value = 4; };
	template<> struct TypeId<xx::List<xx::Ref<PKG::Player>>> { static const uint16_t value = 5; };
}
namespace PKG
{
	inline Player::Player(xx::MemPool* const& mempool) noexcept
        : xx::Object(mempool)
	{
	}
	inline Player::Player(xx::BBuffer* const& bb)
        : xx::Object(bb)
	{
        if (int r = FromBBufferCore(*bb)) throw r;
	}
    inline void Player::ToBBuffer(xx::BBuffer& bb) const noexcept
    {
        bb.Write(this->id);
        bb.Write(this->name);
        bb.Write(this->owner);
    }
    inline int Player::FromBBuffer(xx::BBuffer& bb) noexcept
    {
        return this->FromBBufferCore(bb);
    }
    inline int Player::FromBBufferCore(xx::BBuffer& bb) noexcept
    {
        if (int r = bb.Read(this->id)) return r;
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->name)) return r;
        if (int r = bb.Read(this->owner)) return r;
        return 0;
    }

    inline void Player::ToString(xx::String& s) const noexcept
    {
        if (this->memHeader().flags)
        {
        	s.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else this->memHeader().flags = 1;

        s.Append("{ \"pkgTypeName\":\"Player\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(s);
        s.Append(" }");
        
        this->memHeader().flags = 0;
    }
    inline void Player::ToStringCore(xx::String& s) const noexcept
    {
        this->BaseType::ToStringCore(s);
        s.Append(", \"id\":", this->id);
        if (this->name) s.Append(", \"name\":\"", this->name, "\"");
        else s.Append(", \"name\":nil");
        s.Append(", \"owner\":", this->owner);
    }
    inline void Player::CopyTo(Player* const& o) const noexcept
    {
        o->id = this->id;
        o->name = this->name;
        o->owner = this->owner;
    }
    inline Player* Player::MakeCopy() const noexcept
    {
        auto o = mempool->MPCreate<Player>();
        this->CopyTo(o);
        return o;
    }
    inline Player_p Player::MakePtrCopy() const noexcept
    {
        return Player_p(this->MakeCopy());
    }

	inline Scene::Scene(xx::MemPool* const& mempool) noexcept
        : xx::Object(mempool)
	{
	}
	inline Scene::Scene(xx::BBuffer* const& bb)
        : xx::Object(bb)
	{
        if (int r = FromBBufferCore(*bb)) throw r;
	}
    inline void Scene::ToBBuffer(xx::BBuffer& bb) const noexcept
    {
        bb.Write(this->players);
    }
    inline int Scene::FromBBuffer(xx::BBuffer& bb) noexcept
    {
        return this->FromBBufferCore(bb);
    }
    inline int Scene::FromBBufferCore(xx::BBuffer& bb) noexcept
    {
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->players)) return r;
        return 0;
    }

    inline void Scene::ToString(xx::String& s) const noexcept
    {
        if (this->memHeader().flags)
        {
        	s.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else this->memHeader().flags = 1;

        s.Append("{ \"pkgTypeName\":\"Scene\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(s);
        s.Append(" }");
        
        this->memHeader().flags = 0;
    }
    inline void Scene::ToStringCore(xx::String& s) const noexcept
    {
        this->BaseType::ToStringCore(s);
        s.Append(", \"players\":", this->players);
    }
    inline void Scene::CopyTo(Scene* const& o) const noexcept
    {
        o->players = this->players;
    }
    inline Scene* Scene::MakeCopy() const noexcept
    {
        auto o = mempool->MPCreate<Scene>();
        this->CopyTo(o);
        return o;
    }
    inline Scene_p Scene::MakePtrCopy() const noexcept
    {
        return Scene_p(this->MakeCopy());
    }

}
namespace PKG
{
	inline void AllTypesRegister() noexcept
	{
        xx::MemPool::RegisterInternals();
	    xx::MemPool::Register<PKG::Player, xx::Object>();
	    xx::MemPool::Register<PKG::Scene, xx::Object>();
	    xx::MemPool::Register<xx::List<xx::Ref<PKG::Player>>, xx::Object>();
	}
}
