#pragma once
#include "xx.h"

namespace PKG
{
	struct PkgGenMd5
	{
		static constexpr char const* value = "bf0a438febf3b576bbe0485476196fd2";
    };

    class Foo;
    using Foo_p = xx::Ptr<Foo>;
    using Foo_r = xx::Ref<Foo>;

    class Foo : public xx::Object
    {
    public:
        PKG::Foo_p foo;
        xx::Ref<PKG::Foo> refFoo;
        xx::List_p<PKG::Foo_p> foos;
        xx::List_p<xx::Ref<PKG::Foo>> refFoos;

        typedef Foo ThisType;
        typedef xx::Object BaseType;
	    Foo(xx::MemPool* const& mempool) noexcept;
	    Foo(xx::BBuffer* const& bb);
		Foo(Foo const&) = delete;
		Foo& operator=(Foo const&) = delete;
        void ToString(xx::String& s) const noexcept override;
        void ToStringCore(xx::String& s) const noexcept override;
        void ToBBuffer(xx::BBuffer& bb) const noexcept override;
        int FromBBuffer(xx::BBuffer& bb) noexcept override;
        int FromBBufferCore(xx::BBuffer& bb) noexcept;
        void CopyTo(Foo* const& o) const noexcept;
        Foo* MakeCopy() const noexcept;
        Foo_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
    };
}
namespace xx
{
	template<> struct TypeId<PKG::Foo> { static const uint16_t value = 3; };
	template<> struct TypeId<xx::List<PKG::Foo_p>> { static const uint16_t value = 4; };
	template<> struct TypeId<xx::List<xx::Ref<PKG::Foo>>> { static const uint16_t value = 5; };
}
namespace PKG
{
	inline Foo::Foo(xx::MemPool* const& mempool) noexcept
        : xx::Object(mempool)
	{
	}
	inline Foo::Foo(xx::BBuffer* const& bb)
        : xx::Object(bb)
	{
        if (int r = FromBBufferCore(*bb)) throw r;
	}
    inline void Foo::ToBBuffer(xx::BBuffer& bb) const noexcept
    {
        bb.Write(this->foo);
        bb.Write(this->refFoo);
        bb.Write(this->foos);
        bb.Write(this->refFoos);
    }
    inline int Foo::FromBBuffer(xx::BBuffer& bb) noexcept
    {
        return this->FromBBufferCore(bb);
    }
    inline int Foo::FromBBufferCore(xx::BBuffer& bb) noexcept
    {
        if (int r = bb.Read(this->foo)) return r;
        if (int r = bb.Read(this->refFoo)) return r;
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->foos)) return r;
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->refFoos)) return r;
        return 0;
    }

    inline void Foo::ToString(xx::String& s) const noexcept
    {
        if (this->memHeader().flags)
        {
        	s.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else this->memHeader().flags = 1;

        s.Append("{ \"pkgTypeName\":\"Foo\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(s);
        s.Append(" }");
        
        this->memHeader().flags = 0;
    }
    inline void Foo::ToStringCore(xx::String& s) const noexcept
    {
        this->BaseType::ToStringCore(s);
        s.Append(", \"foo\":", this->foo);
        s.Append(", \"refFoo\":", this->refFoo);
        s.Append(", \"foos\":", this->foos);
        s.Append(", \"refFoos\":", this->refFoos);
    }
    inline void Foo::CopyTo(Foo* const& o) const noexcept
    {
        o->foo = this->foo;
        o->refFoo = this->refFoo;
        o->foos = this->foos;
        o->refFoos = this->refFoos;
    }
    inline Foo* Foo::MakeCopy() const noexcept
    {
        auto o = mempool->MPCreate<Foo>();
        this->CopyTo(o);
        return o;
    }
    inline Foo_p Foo::MakePtrCopy() const noexcept
    {
        return Foo_p(this->MakeCopy());
    }

}
namespace PKG
{
	inline void AllTypesRegister() noexcept
	{
        xx::MemPool::RegisterInternals();
	    xx::MemPool::Register<PKG::Foo, xx::Object>();
	    xx::MemPool::Register<xx::List<PKG::Foo_p>, xx::Object>();
	    xx::MemPool::Register<xx::List<xx::Ref<PKG::Foo>>, xx::Object>();
	}
}
