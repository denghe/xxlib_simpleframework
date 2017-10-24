#include <xx_mempool.h>
#include <xx_bbuffer.h>

namespace PKG2
{
    // 测试一下各种数据类型
    struct 基类;
    using 基类_p = xx::Ptr<基类>;
    using 基类_v = xx::Dock<基类>;

    struct 派生类;
    using 派生类_p = xx::Ptr<派生类>;
    using 派生类_v = xx::Dock<派生类>;

    // 测试一下各种数据类型
    struct 基类 : xx::Object
    {
        bool 不淋 = false;
        uint8_t 白特 = 0;
        int8_t 撕白特 = 0;
        int16_t 吸哦特 = 0;
        uint16_t 又吸哦特 = 0;
        int32_t 硬特 = 0;
        uint32_t 又硬特 = 0;
        int64_t 浪 = 0;
        uint64_t 又浪 = 0;
        float 扶裸特 = 0;
        double 大波 = 0;
        xx::String_p 湿最硬;
        xx::BBuffer_p 屄拔扶儿 = nullptr;

        typedef 基类 ThisType;
        typedef xx::Object BaseType;
	    基类();
	    基类(xx::BBuffer *bb);
		基类(基类 const&) = delete;
		基类& operator=(基类 const&) = delete;
        virtual void ToString(xx::String &str) const override;
        virtual void ToStringCore(xx::String &str) const override;
        virtual void ToBBuffer(xx::BBuffer &bb) const override;
        virtual int FromBBuffer(xx::BBuffer &bb) override;
    };
    struct 派生类 : PKG2::基类
    {
        xx::List_p<uint8_t> 立丝特白特;
        xx::List_p<int8_t> 立丝特撕白特;
        xx::List_p<int16_t> 立丝特吸哦特;
        xx::List_p<uint16_t> 立丝特又吸哦特;
        xx::List_p<int32_t> 立丝特硬特;
        xx::List_p<uint32_t> 立丝特又硬特;
        xx::List_p<int64_t> 立丝特浪;
        xx::List_p<uint64_t> 立丝特又浪;
        xx::List_p<float> 立丝特扶裸特;
        xx::List_p<double> 立丝特大波;
        xx::List_p<xx::String_p> 立丝特湿最硬;
        xx::List_p<xx::BBuffer_p> 立丝特屄拔扶儿;
        // 可以放入 基类 或 派生类
        xx::List_p<PKG2::基类_p> 立丝特基类;

        typedef 派生类 ThisType;
        typedef PKG2::基类 BaseType;
	    派生类();
	    派生类(xx::BBuffer *bb);
		派生类(派生类 const&) = delete;
		派生类& operator=(派生类 const&) = delete;
        virtual void ToString(xx::String &str) const override;
        virtual void ToStringCore(xx::String &str) const override;
        virtual void ToBBuffer(xx::BBuffer &bb) const override;
        virtual int FromBBuffer(xx::BBuffer &bb) override;
    };
	inline 基类::基类()
	{
	}
	inline 基类::基类(xx::BBuffer *bb)
	{
	    int rtv = 0;
        if (rtv = bb->Read(不淋)) throw rtv;
        if (rtv = bb->Read(白特)) throw rtv;
        if (rtv = bb->Read(撕白特)) throw rtv;
        if (rtv = bb->Read(吸哦特)) throw rtv;
        if (rtv = bb->Read(又吸哦特)) throw rtv;
        if (rtv = bb->Read(硬特)) throw rtv;
        if (rtv = bb->Read(又硬特)) throw rtv;
        if (rtv = bb->Read(浪)) throw rtv;
        if (rtv = bb->Read(又浪)) throw rtv;
        if (rtv = bb->Read(扶裸特)) throw rtv;
        if (rtv = bb->Read(大波)) throw rtv;
        bb->readLengthLimit = 0;
        if (rtv = bb->Read(湿最硬)) throw rtv;
        bb->readLengthLimit = 0;
        if (rtv = bb->Read(屄拔扶儿)) throw rtv;
	}
    inline void 基类::ToBBuffer(xx::BBuffer &bb) const
    {
        bb.Write(this->不淋);
        bb.Write(this->白特);
        bb.Write(this->撕白特);
        bb.Write(this->吸哦特);
        bb.Write(this->又吸哦特);
        bb.Write(this->硬特);
        bb.Write(this->又硬特);
        bb.Write(this->浪);
        bb.Write(this->又浪);
        bb.Write(this->扶裸特);
        bb.Write(this->大波);
        bb.Write(this->湿最硬);
        bb.Write(this->屄拔扶儿);
    }
    inline int 基类::FromBBuffer(xx::BBuffer &bb)
    {
        int rtv = 0;
        if (rtv = bb.Read(this->不淋)) return rtv;
        if (rtv = bb.Read(this->白特)) return rtv;
        if (rtv = bb.Read(this->撕白特)) return rtv;
        if (rtv = bb.Read(this->吸哦特)) return rtv;
        if (rtv = bb.Read(this->又吸哦特)) return rtv;
        if (rtv = bb.Read(this->硬特)) return rtv;
        if (rtv = bb.Read(this->又硬特)) return rtv;
        if (rtv = bb.Read(this->浪)) return rtv;
        if (rtv = bb.Read(this->又浪)) return rtv;
        if (rtv = bb.Read(this->扶裸特)) return rtv;
        if (rtv = bb.Read(this->大波)) return rtv;
        bb.readLengthLimit = 0;
        if (rtv = bb.Read(this->湿最硬)) return rtv;
        bb.readLengthLimit = 0;
        if (rtv = bb.Read(this->屄拔扶儿)) return rtv;
        return rtv;
    }

    inline void 基类::ToString(xx::String &str) const
    {
        if (tsFlags())
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else tsFlags() = 1;

        str.Append("{ \"type\" : \"基类\"");
        ToStringCore(str);
        str.Append(" }");
        
        tsFlags() = 0;
    }
    inline void 基类::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"不淋\" : ", this->不淋);
        str.Append(", \"白特\" : ", this->白特);
        str.Append(", \"撕白特\" : ", this->撕白特);
        str.Append(", \"吸哦特\" : ", this->吸哦特);
        str.Append(", \"又吸哦特\" : ", this->又吸哦特);
        str.Append(", \"硬特\" : ", this->硬特);
        str.Append(", \"又硬特\" : ", this->又硬特);
        str.Append(", \"浪\" : ", this->浪);
        str.Append(", \"又浪\" : ", this->又浪);
        str.Append(", \"扶裸特\" : ", this->扶裸特);
        str.Append(", \"大波\" : ", this->大波);
        str.Append(", \"湿最硬\" : ", this->湿最硬);
        str.Append(", \"屄拔扶儿\" : ", this->屄拔扶儿);
    }


	inline 派生类::派生类()
        : PKG2::基类()
	{
	}
	inline 派生类::派生类(xx::BBuffer *bb)
        : PKG2::基类(bb)
	{
	    int rtv = 0;
        bb->readLengthLimit = 0;
        if (rtv = bb->Read(立丝特白特)) throw rtv;
        bb->readLengthLimit = 0;
        if (rtv = bb->Read(立丝特撕白特)) throw rtv;
        bb->readLengthLimit = 0;
        if (rtv = bb->Read(立丝特吸哦特)) throw rtv;
        bb->readLengthLimit = 0;
        if (rtv = bb->Read(立丝特又吸哦特)) throw rtv;
        bb->readLengthLimit = 0;
        if (rtv = bb->Read(立丝特硬特)) throw rtv;
        bb->readLengthLimit = 0;
        if (rtv = bb->Read(立丝特又硬特)) throw rtv;
        bb->readLengthLimit = 0;
        if (rtv = bb->Read(立丝特浪)) throw rtv;
        bb->readLengthLimit = 0;
        if (rtv = bb->Read(立丝特又浪)) throw rtv;
        bb->readLengthLimit = 0;
        if (rtv = bb->Read(立丝特扶裸特)) throw rtv;
        bb->readLengthLimit = 0;
        if (rtv = bb->Read(立丝特大波)) throw rtv;
        bb->readLengthLimit = 0;
        if (rtv = bb->Read(立丝特湿最硬)) throw rtv;
        bb->readLengthLimit = 0;
        if (rtv = bb->Read(立丝特屄拔扶儿)) throw rtv;
        bb->readLengthLimit = 0;
        if (rtv = bb->Read(立丝特基类)) throw rtv;
	}
    inline void 派生类::ToBBuffer(xx::BBuffer &bb) const
    {
        this->BaseType::ToBBuffer(bb);
        bb.Write(this->立丝特白特);
        bb.Write(this->立丝特撕白特);
        bb.Write(this->立丝特吸哦特);
        bb.Write(this->立丝特又吸哦特);
        bb.Write(this->立丝特硬特);
        bb.Write(this->立丝特又硬特);
        bb.Write(this->立丝特浪);
        bb.Write(this->立丝特又浪);
        bb.Write(this->立丝特扶裸特);
        bb.Write(this->立丝特大波);
        bb.Write(this->立丝特湿最硬);
        bb.Write(this->立丝特屄拔扶儿);
        bb.Write(this->立丝特基类);
    }
    inline int 派生类::FromBBuffer(xx::BBuffer &bb)
    {
        int rtv = 0;
        if (rtv = this->BaseType::FromBBuffer(bb)) return rtv;
        bb.readLengthLimit = 0;
        if (rtv = bb.Read(this->立丝特白特)) return rtv;
        bb.readLengthLimit = 0;
        if (rtv = bb.Read(this->立丝特撕白特)) return rtv;
        bb.readLengthLimit = 0;
        if (rtv = bb.Read(this->立丝特吸哦特)) return rtv;
        bb.readLengthLimit = 0;
        if (rtv = bb.Read(this->立丝特又吸哦特)) return rtv;
        bb.readLengthLimit = 0;
        if (rtv = bb.Read(this->立丝特硬特)) return rtv;
        bb.readLengthLimit = 0;
        if (rtv = bb.Read(this->立丝特又硬特)) return rtv;
        bb.readLengthLimit = 0;
        if (rtv = bb.Read(this->立丝特浪)) return rtv;
        bb.readLengthLimit = 0;
        if (rtv = bb.Read(this->立丝特又浪)) return rtv;
        bb.readLengthLimit = 0;
        if (rtv = bb.Read(this->立丝特扶裸特)) return rtv;
        bb.readLengthLimit = 0;
        if (rtv = bb.Read(this->立丝特大波)) return rtv;
        bb.readLengthLimit = 0;
        if (rtv = bb.Read(this->立丝特湿最硬)) return rtv;
        bb.readLengthLimit = 0;
        if (rtv = bb.Read(this->立丝特屄拔扶儿)) return rtv;
        bb.readLengthLimit = 0;
        if (rtv = bb.Read(this->立丝特基类)) return rtv;
        return rtv;
    }

    inline void 派生类::ToString(xx::String &str) const
    {
        if (tsFlags())
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else tsFlags() = 1;

        str.Append("{ \"type\" : \"派生类\"");
        ToStringCore(str);
        str.Append(" }");
        
        tsFlags() = 0;
    }
    inline void 派生类::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"立丝特白特\" : ", this->立丝特白特);
        str.Append(", \"立丝特撕白特\" : ", this->立丝特撕白特);
        str.Append(", \"立丝特吸哦特\" : ", this->立丝特吸哦特);
        str.Append(", \"立丝特又吸哦特\" : ", this->立丝特又吸哦特);
        str.Append(", \"立丝特硬特\" : ", this->立丝特硬特);
        str.Append(", \"立丝特又硬特\" : ", this->立丝特又硬特);
        str.Append(", \"立丝特浪\" : ", this->立丝特浪);
        str.Append(", \"立丝特又浪\" : ", this->立丝特又浪);
        str.Append(", \"立丝特扶裸特\" : ", this->立丝特扶裸特);
        str.Append(", \"立丝特大波\" : ", this->立丝特大波);
        str.Append(", \"立丝特湿最硬\" : ", this->立丝特湿最硬);
        str.Append(", \"立丝特屄拔扶儿\" : ", this->立丝特屄拔扶儿);
        str.Append(", \"立丝特基类\" : ", this->立丝特基类);
    }


}
namespace xx
{
	template<> struct TypeId<xx::String> { static const uint16_t value = 1; };
	template<> struct TypeId<xx::BBuffer> { static const uint16_t value = 2; };
	template<> struct TypeId<PKG2::基类> { static const uint16_t value = 3; };
	template<> struct TypeId<PKG2::派生类> { static const uint16_t value = 4; };
	template<> struct TypeId<xx::List<uint8_t>> { static const uint16_t value = 5; };
	template<> struct TypeId<xx::List<int8_t>> { static const uint16_t value = 6; };
	template<> struct TypeId<xx::List<int16_t>> { static const uint16_t value = 7; };
	template<> struct TypeId<xx::List<uint16_t>> { static const uint16_t value = 8; };
	template<> struct TypeId<xx::List<int32_t>> { static const uint16_t value = 9; };
	template<> struct TypeId<xx::List<uint32_t>> { static const uint16_t value = 10; };
	template<> struct TypeId<xx::List<int64_t>> { static const uint16_t value = 11; };
	template<> struct TypeId<xx::List<uint64_t>> { static const uint16_t value = 12; };
	template<> struct TypeId<xx::List<float>> { static const uint16_t value = 13; };
	template<> struct TypeId<xx::List<double>> { static const uint16_t value = 14; };
	template<> struct TypeId<xx::List<xx::String_p>> { static const uint16_t value = 15; };
	template<> struct TypeId<xx::List<xx::BBuffer_p>> { static const uint16_t value = 16; };
	template<> struct TypeId<xx::List<PKG2::基类_p>> { static const uint16_t value = 17; };
}
namespace PKG2
{
	inline void AllTypesRegister()
	{
	    xx::MemPool::Register<xx::String, xx::Object>();
	    xx::MemPool::Register<xx::BBuffer, xx::Object>();
	    xx::MemPool::Register<PKG2::基类, xx::Object>();
	    xx::MemPool::Register<PKG2::派生类, PKG2::基类>();
	    xx::MemPool::Register<xx::List<uint8_t>, xx::Object>();
	    xx::MemPool::Register<xx::List<int8_t>, xx::Object>();
	    xx::MemPool::Register<xx::List<int16_t>, xx::Object>();
	    xx::MemPool::Register<xx::List<uint16_t>, xx::Object>();
	    xx::MemPool::Register<xx::List<int32_t>, xx::Object>();
	    xx::MemPool::Register<xx::List<uint32_t>, xx::Object>();
	    xx::MemPool::Register<xx::List<int64_t>, xx::Object>();
	    xx::MemPool::Register<xx::List<uint64_t>, xx::Object>();
	    xx::MemPool::Register<xx::List<float>, xx::Object>();
	    xx::MemPool::Register<xx::List<double>, xx::Object>();
	    xx::MemPool::Register<xx::List<xx::String_p>, xx::Object>();
	    xx::MemPool::Register<xx::List<xx::BBuffer_p>, xx::Object>();
	    xx::MemPool::Register<xx::List<PKG2::基类_p>, xx::Object>();
	}
}
