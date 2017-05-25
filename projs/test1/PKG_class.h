#include <xx_mempool.h>
#include <xx_bbuffer.h>

namespace PKG
{
    enum class E : uint8_t
    {
        A = 0,
        B = 1,
        C = 2,
    };
    struct Bar : xx::MPObject
    {
        typedef xx::MPObject BaseType;

	    Bar()
	    {
	    }
	    Bar(xx::BBuffer *bb)
	    {
	    }
	    ~Bar()
	    {
	    }

        // todo: inline virtual void ToString(xx::String &str) const override

        inline virtual void ToBBuffer(xx::BBuffer &bb) const override
        {
        }

        inline virtual int FromBBuffer(xx::BBuffer &bb) override
        {
            return 0;
        }

    };
    struct Bar1 : PKG::Bar
    {
        typedef PKG::Bar BaseType;
        int32_t f6;

	    Bar1()
            : BaseType()
	    {
	    }
	    Bar1(xx::BBuffer *bb)
            : BaseType(bb)
	    {
	    }
	    ~Bar1()
	    {
	    }

        // todo: inline virtual void ToString(xx::String &str) const override

        inline virtual void ToBBuffer(xx::BBuffer &bb) const override
        {
            this->BaseType::ToBBuffer(bb);
            bb.Write(this->f6);
        }

        inline virtual int FromBBuffer(xx::BBuffer &bb) override
        {
            this->BaseType::FromBBuffer(bb);
            bb.Read(this->f6);
            return 0;
        }

    };
    struct Bar2 : PKG::Bar
    {
        typedef PKG::Bar BaseType;
        xx::String* f10;

	    Bar2()
            : BaseType()
	    {
            mempool().CreateTo(f10);
	    }
	    Bar2(xx::BBuffer *bb)
            : BaseType(bb)
	    {
            if (auto rtv = bb->Read(f10)) throw rtv;
	    }
	    ~Bar2()
	    {
	    }

        // todo: inline virtual void ToString(xx::String &str) const override

        inline virtual void ToBBuffer(xx::BBuffer &bb) const override
        {
            this->BaseType::ToBBuffer(bb);
            bb.Write(this->f10);
        }

        inline virtual int FromBBuffer(xx::BBuffer &bb) override
        {
            this->BaseType::FromBBuffer(bb);
            bb.Read(this->f10);
            return 0;
        }

    };
    struct FooBase : xx::MPObject
    {
        typedef xx::MPObject BaseType;

	    FooBase()
	    {
	    }
	    FooBase(xx::BBuffer *bb)
	    {
	    }
	    ~FooBase()
	    {
	    }

        // todo: inline virtual void ToString(xx::String &str) const override

        inline virtual void ToBBuffer(xx::BBuffer &bb) const override
        {
        }

        inline virtual int FromBBuffer(xx::BBuffer &bb) override
        {
            return 0;
        }

    };
    struct Foo : PKG::FooBase
    {
        typedef PKG::FooBase BaseType;
        uint8_t f1;
        int8_t f2;
        uint16_t f3;
        int16_t f4;
        uint32_t f5;
        int32_t f6;
        uint64_t f7;
        int64_t f8;
        xx::String* f10;
        xx::BBuffer* f11;
        PKG::E f12;
        xx::List<PKG::E>* f13;
        xx::List<PKG::Bar*>* f14;
        xx::List<xx::List<PKG::E>*>* f15;
        xx::List<xx::List<PKG::Bar*>*>* f16;
        xx::List<xx::List<xx::List<PKG::E>*>*>* f17;
        xx::List<xx::List<xx::List<PKG::Bar*>*>*>* f18;
        PKG::FooBase* f19;

	    Foo()
            : BaseType()
	    {
            mempool().CreateTo(f10);
            mempool().CreateTo(f11);
            mempool().CreateTo(f13);
            mempool().CreateTo(f14);
            mempool().CreateTo(f15);
            mempool().CreateTo(f16);
            mempool().CreateTo(f17);
            mempool().CreateTo(f18);
            mempool().CreateTo(f19);
	    }
	    Foo(xx::BBuffer *bb)
            : BaseType(bb)
	    {
            if (auto rtv = bb->Read(f10)) throw rtv;
            if (auto rtv = bb->Read(f11)) throw rtv;
            if (auto rtv = bb->Read(f13)) throw rtv;
            if (auto rtv = bb->Read(f14)) throw rtv;
            if (auto rtv = bb->Read(f15)) throw rtv;
            if (auto rtv = bb->Read(f16)) throw rtv;
            if (auto rtv = bb->Read(f17)) throw rtv;
            if (auto rtv = bb->Read(f18)) throw rtv;
            if (auto rtv = bb->Read(f19)) throw rtv;
	    }
	    ~Foo()
	    {
            mempool().SafeRelease(f11);
            mempool().SafeRelease(f13);
            mempool().SafeRelease(f14);
            mempool().SafeRelease(f15);
            mempool().SafeRelease(f16);
            mempool().SafeRelease(f17);
            mempool().SafeRelease(f18);
            mempool().SafeRelease(f19);
	    }

        // todo: inline virtual void ToString(xx::String &str) const override

        inline virtual void ToBBuffer(xx::BBuffer &bb) const override
        {
            this->BaseType::ToBBuffer(bb);
            bb.Write(this->f1);
            bb.Write(this->f2);
            bb.Write(this->f3);
            bb.Write(this->f4);
            bb.Write(this->f5);
            bb.Write(this->f6);
            bb.Write(this->f7);
            bb.Write(this->f8);
            bb.Write(this->f10);
            bb.Write(this->f11);
            bb.Write(this->f12);
            bb.Write(this->f13);
            bb.Write(this->f14);
            bb.Write(this->f15);
            bb.Write(this->f16);
            bb.Write(this->f17);
            bb.Write(this->f18);
            bb.Write(this->f19);
        }

        inline virtual int FromBBuffer(xx::BBuffer &bb) override
        {
            this->BaseType::FromBBuffer(bb);
            bb.Read(this->f1);
            bb.Read(this->f2);
            bb.Read(this->f3);
            bb.Read(this->f4);
            bb.Read(this->f5);
            bb.Read(this->f6);
            bb.Read(this->f7);
            bb.Read(this->f8);
            bb.Read(this->f10);
            bb.Read(this->f11);
            bb.Read(this->f12);
            bb.Read(this->f13);
            bb.Read(this->f14);
            bb.Read(this->f15);
            bb.Read(this->f16);
            bb.Read(this->f17);
            bb.Read(this->f18);
            bb.Read(this->f19);
            return 0;
        }

    };
}
namespace xx
{
	template<> struct TypeId<PKG::Bar> { static const uint16_t value = 2; };
	template<> struct TypeId<PKG::Bar1> { static const uint16_t value = 3; };
	template<> struct TypeId<PKG::Bar2> { static const uint16_t value = 4; };
	template<> struct TypeId<PKG::FooBase> { static const uint16_t value = 5; };
	template<> struct TypeId<PKG::Foo> { static const uint16_t value = 6; };
	template<> struct TypeId<xx::List<PKG::E>> { static const uint16_t value = 7; };
	template<> struct TypeId<xx::List<PKG::Bar*>> { static const uint16_t value = 8; };
	template<> struct TypeId<xx::List<xx::List<PKG::E>*>> { static const uint16_t value = 9; };
	template<> struct TypeId<xx::List<xx::List<PKG::Bar*>*>> { static const uint16_t value = 10; };
	template<> struct TypeId<xx::List<xx::List<xx::List<PKG::E>*>*>> { static const uint16_t value = 11; };
	template<> struct TypeId<xx::List<xx::List<xx::List<PKG::Bar*>*>*>> { static const uint16_t value = 12; };
}
namespace PKG
{
	inline void RegisterTypes()
	{
	    xx::MemPool::Register<PKG::Bar, xx::MPObject>();
	    xx::MemPool::Register<PKG::Bar1, PKG::Bar>();
	    xx::MemPool::Register<PKG::Bar2, PKG::Bar>();
	    xx::MemPool::Register<PKG::FooBase, xx::MPObject>();
	    xx::MemPool::Register<PKG::Foo, PKG::FooBase>();
	    xx::MemPool::Register<xx::List<PKG::E>, xx::MPObject>();
	    xx::MemPool::Register<xx::List<PKG::Bar*>, xx::MPObject>();
	    xx::MemPool::Register<xx::List<xx::List<PKG::E>*>, xx::MPObject>();
	    xx::MemPool::Register<xx::List<xx::List<PKG::Bar*>*>, xx::MPObject>();
	    xx::MemPool::Register<xx::List<xx::List<xx::List<PKG::E>*>*>, xx::MPObject>();
	    xx::MemPool::Register<xx::List<xx::List<xx::List<PKG::Bar*>*>*>, xx::MPObject>();
	}
}
