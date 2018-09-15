#pragma once
#include "xx.h"

namespace PKG
{
	struct PkgGenMd5
	{
		static constexpr char const* value = "267d82049bdda2a37af33059b40eaf53";
    };

    class Container;
    using Container_p = xx::Ptr<Container>;
    using Container_r = xx::Ref<Container>;

    class Foo;
    using Foo_p = xx::Ptr<Foo>;
    using Foo_r = xx::Ref<Foo>;

    class FooEx;
    using FooEx_p = xx::Ptr<FooEx>;
    using FooEx_r = xx::Ref<FooEx>;

    class Foo : public xx::Object
    {
    public:
        xx::List_p<PKG::Foo_r> childs;

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
    class Container : public xx::Object
    {
    public:
        xx::List_p<PKG::Foo_p> foos;

        typedef Container ThisType;
        typedef xx::Object BaseType;
	    Container(xx::MemPool* const& mempool) noexcept;
	    Container(xx::BBuffer* const& bb);
		Container(Container const&) = delete;
		Container& operator=(Container const&) = delete;
        void ToString(xx::String& s) const noexcept override;
        void ToStringCore(xx::String& s) const noexcept override;
        void ToBBuffer(xx::BBuffer& bb) const noexcept override;
        int FromBBuffer(xx::BBuffer& bb) noexcept override;
        int FromBBufferCore(xx::BBuffer& bb) noexcept;
        void CopyTo(Container* const& o) const noexcept;
        Container* MakeCopy() const noexcept;
        Container_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
    };
    class FooEx : public PKG::Foo
    {
    public:
        PKG::Foo_r parent;

        typedef FooEx ThisType;
        typedef PKG::Foo BaseType;
	    FooEx(xx::MemPool* const& mempool) noexcept;
	    FooEx(xx::BBuffer* const& bb);
		FooEx(FooEx const&) = delete;
		FooEx& operator=(FooEx const&) = delete;
        void ToString(xx::String& s) const noexcept override;
        void ToStringCore(xx::String& s) const noexcept override;
        void ToBBuffer(xx::BBuffer& bb) const noexcept override;
        int FromBBuffer(xx::BBuffer& bb) noexcept override;
        int FromBBufferCore(xx::BBuffer& bb) noexcept;
        void CopyTo(FooEx* const& o) const noexcept;
        FooEx* MakeCopy() const noexcept;
        FooEx_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
    };
}
namespace xx
{
	template<> struct TypeId<PKG::Container> { static const uint16_t value = 7; };
	template<> struct TypeId<xx::List<PKG::Foo_p>> { static const uint16_t value = 4; };
	template<> struct TypeId<xx::List<PKG::Foo_r>> { static const uint16_t value = 5; };
	template<> struct TypeId<PKG::Foo> { static const uint16_t value = 3; };
	template<> struct TypeId<PKG::FooEx> { static const uint16_t value = 6; };
}
namespace PKG
{
	inline Container::Container(xx::MemPool* const& mempool) noexcept
        : xx::Object(mempool)
	{
	}
	inline Container::Container(xx::BBuffer* const& bb)
        : xx::Object(bb)
	{
        if (int r = FromBBufferCore(*bb)) throw r;
	}
    inline void Container::ToBBuffer(xx::BBuffer& bb) const noexcept
    {
        bb.Write(this->foos);
    }
    inline int Container::FromBBuffer(xx::BBuffer& bb) noexcept
    {
        return this->FromBBufferCore(bb);
    }
    inline int Container::FromBBufferCore(xx::BBuffer& bb) noexcept
    {
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->foos)) return r;
        return 0;
    }

    inline void Container::ToString(xx::String& s) const noexcept
    {
        if (this->memHeader().flags)
        {
        	s.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else this->memHeader().flags = 1;

        s.Append("{ \"pkgTypeName\":\"Container\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(s);
        s.Append(" }");
        
        this->memHeader().flags = 0;
    }
    inline void Container::ToStringCore(xx::String& s) const noexcept
    {
        this->BaseType::ToStringCore(s);
        s.Append(", \"foos\":", this->foos);
    }
    inline void Container::CopyTo(Container* const& o) const noexcept
    {
        o->foos = this->foos;
    }
    inline Container* Container::MakeCopy() const noexcept
    {
        auto o = mempool->MPCreate<Container>();
        this->CopyTo(o);
        return o;
    }
    inline Container_p Container::MakePtrCopy() const noexcept
    {
        return Container_p(this->MakeCopy());
    }

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
        bb.Write(this->childs);
    }
    inline int Foo::FromBBuffer(xx::BBuffer& bb) noexcept
    {
        return this->FromBBufferCore(bb);
    }
    inline int Foo::FromBBufferCore(xx::BBuffer& bb) noexcept
    {
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->childs)) return r;
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
        s.Append(", \"childs\":", this->childs);
    }
    inline void Foo::CopyTo(Foo* const& o) const noexcept
    {
        o->childs = this->childs;
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

	inline FooEx::FooEx(xx::MemPool* const& mempool) noexcept
        : PKG::Foo(mempool)
	{
	}
	inline FooEx::FooEx(xx::BBuffer* const& bb)
        : PKG::Foo(bb)
	{
        if (int r = FromBBufferCore(*bb)) throw r;
	}
    inline void FooEx::ToBBuffer(xx::BBuffer& bb) const noexcept
    {
        this->BaseType::ToBBuffer(bb);
        bb.Write(this->parent);
    }
    inline int FooEx::FromBBuffer(xx::BBuffer& bb) noexcept
    {
        if (int r = this->BaseType::FromBBuffer(bb)) return r;
        return this->FromBBufferCore(bb);
    }
    inline int FooEx::FromBBufferCore(xx::BBuffer& bb) noexcept
    {
        if (int r = bb.Read(this->parent)) return r;
        return 0;
    }

    inline void FooEx::ToString(xx::String& s) const noexcept
    {
        if (this->memHeader().flags)
        {
        	s.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else this->memHeader().flags = 1;

        s.Append("{ \"pkgTypeName\":\"FooEx\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(s);
        s.Append(" }");
        
        this->memHeader().flags = 0;
    }
    inline void FooEx::ToStringCore(xx::String& s) const noexcept
    {
        this->BaseType::ToStringCore(s);
        s.Append(", \"parent\":", this->parent);
    }
    inline void FooEx::CopyTo(FooEx* const& o) const noexcept
    {
        this->BaseType::CopyTo(o);
        o->parent = this->parent;
    }
    inline FooEx* FooEx::MakeCopy() const noexcept
    {
        auto o = mempool->MPCreate<FooEx>();
        this->CopyTo(o);
        return o;
    }
    inline FooEx_p FooEx::MakePtrCopy() const noexcept
    {
        return FooEx_p(this->MakeCopy());
    }

}
namespace PKG
{
	inline void AllTypesRegister() noexcept
	{
        xx::MemPool::RegisterInternals();
	    xx::MemPool::Register<PKG::Container, xx::Object>();
	    xx::MemPool::Register<xx::List<PKG::Foo_p>, xx::Object>();
	    xx::MemPool::Register<xx::List<PKG::Foo_r>, xx::Object>();
	    xx::MemPool::Register<PKG::Foo, xx::Object>();
	    xx::MemPool::Register<PKG::FooEx, PKG::Foo>();
	}
}
