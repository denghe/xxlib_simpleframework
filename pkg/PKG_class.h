#pragma once
#include "xx.h"

namespace PKG
{
	struct PkgGenMd5
	{
		static constexpr char const* value = "5004aa29c60e22c1ff56b491076eb11a";
    };

    class Foo;
    using Foo_p = xx::Ptr<Foo>;
    using Foo_r = xx::Ref<Foo>;

    class DataSet;
    using DataSet_p = xx::Ptr<DataSet>;
    using DataSet_r = xx::Ref<DataSet>;

    class Table;
    using Table_p = xx::Ptr<Table>;
    using Table_r = xx::Ref<Table>;

    class TableColumn;
    using TableColumn_p = xx::Ptr<TableColumn>;
    using TableColumn_r = xx::Ref<TableColumn>;

    class TableRow;
    using TableRow_p = xx::Ptr<TableRow>;
    using TableRow_r = xx::Ref<TableRow>;

    class TableRowValue;
    using TableRowValue_p = xx::Ptr<TableRowValue>;
    using TableRowValue_r = xx::Ref<TableRowValue>;

    class TableRowValue_Int;
    using TableRowValue_Int_p = xx::Ptr<TableRowValue_Int>;
    using TableRowValue_Int_r = xx::Ref<TableRowValue_Int>;

    class TableRowValue_NullableInt;
    using TableRowValue_NullableInt_p = xx::Ptr<TableRowValue_NullableInt>;
    using TableRowValue_NullableInt_r = xx::Ref<TableRowValue_NullableInt>;

    class TableRowValue_String;
    using TableRowValue_String_p = xx::Ptr<TableRowValue_String>;
    using TableRowValue_String_r = xx::Ref<TableRowValue_String>;

    class TableRowValue : public xx::Object
    {
    public:

        typedef TableRowValue ThisType;
        typedef xx::Object BaseType;
	    TableRowValue(xx::MemPool* const& mempool) noexcept;
	    TableRowValue(xx::BBuffer* const& bb);
		TableRowValue(TableRowValue const&) = delete;
		TableRowValue& operator=(TableRowValue const&) = delete;
        void ToString(xx::String& s) const noexcept override;
        void ToStringCore(xx::String& s) const noexcept override;
        void ToBBuffer(xx::BBuffer& bb) const noexcept override;
        int FromBBuffer(xx::BBuffer& bb) noexcept override;
        int FromBBufferCore(xx::BBuffer& bb) noexcept;
        void CopyTo(TableRowValue* const& o) const noexcept;
        TableRowValue* MakeCopy() const noexcept;
        TableRowValue_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
    };
    class Foo : public xx::Object
    {
    public:
        int32_t id = 0;
        std::optional<int32_t> age;
        xx::String_p info;
        xx::List_p<PKG::Foo_p> childs;

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
    class DataSet : public xx::Object
    {
    public:
        xx::List_p<PKG::Table_p> tables;

        typedef DataSet ThisType;
        typedef xx::Object BaseType;
	    DataSet(xx::MemPool* const& mempool) noexcept;
	    DataSet(xx::BBuffer* const& bb);
		DataSet(DataSet const&) = delete;
		DataSet& operator=(DataSet const&) = delete;
        void ToString(xx::String& s) const noexcept override;
        void ToStringCore(xx::String& s) const noexcept override;
        void ToBBuffer(xx::BBuffer& bb) const noexcept override;
        int FromBBuffer(xx::BBuffer& bb) noexcept override;
        int FromBBufferCore(xx::BBuffer& bb) noexcept;
        void CopyTo(DataSet* const& o) const noexcept;
        DataSet* MakeCopy() const noexcept;
        DataSet_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
    };
    class Table : public xx::Object
    {
    public:
        PKG::DataSet_p parent;
        xx::String_p name;
        xx::List_p<PKG::TableColumn_p> columns;
        xx::List_p<PKG::TableRow_p> rows;

        typedef Table ThisType;
        typedef xx::Object BaseType;
	    Table(xx::MemPool* const& mempool) noexcept;
	    Table(xx::BBuffer* const& bb);
		Table(Table const&) = delete;
		Table& operator=(Table const&) = delete;
        void ToString(xx::String& s) const noexcept override;
        void ToStringCore(xx::String& s) const noexcept override;
        void ToBBuffer(xx::BBuffer& bb) const noexcept override;
        int FromBBuffer(xx::BBuffer& bb) noexcept override;
        int FromBBufferCore(xx::BBuffer& bb) noexcept;
        void CopyTo(Table* const& o) const noexcept;
        Table* MakeCopy() const noexcept;
        Table_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
    };
    class TableColumn : public xx::Object
    {
    public:
        xx::String_p name;

        typedef TableColumn ThisType;
        typedef xx::Object BaseType;
	    TableColumn(xx::MemPool* const& mempool) noexcept;
	    TableColumn(xx::BBuffer* const& bb);
		TableColumn(TableColumn const&) = delete;
		TableColumn& operator=(TableColumn const&) = delete;
        void ToString(xx::String& s) const noexcept override;
        void ToStringCore(xx::String& s) const noexcept override;
        void ToBBuffer(xx::BBuffer& bb) const noexcept override;
        int FromBBuffer(xx::BBuffer& bb) noexcept override;
        int FromBBufferCore(xx::BBuffer& bb) noexcept;
        void CopyTo(TableColumn* const& o) const noexcept;
        TableColumn* MakeCopy() const noexcept;
        TableColumn_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
    };
    class TableRow : public xx::Object
    {
    public:
        xx::List_p<PKG::TableRowValue_p> values;

        typedef TableRow ThisType;
        typedef xx::Object BaseType;
	    TableRow(xx::MemPool* const& mempool) noexcept;
	    TableRow(xx::BBuffer* const& bb);
		TableRow(TableRow const&) = delete;
		TableRow& operator=(TableRow const&) = delete;
        void ToString(xx::String& s) const noexcept override;
        void ToStringCore(xx::String& s) const noexcept override;
        void ToBBuffer(xx::BBuffer& bb) const noexcept override;
        int FromBBuffer(xx::BBuffer& bb) noexcept override;
        int FromBBufferCore(xx::BBuffer& bb) noexcept;
        void CopyTo(TableRow* const& o) const noexcept;
        TableRow* MakeCopy() const noexcept;
        TableRow_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
    };
    class TableRowValue_Int : public PKG::TableRowValue
    {
    public:
        int32_t value = 0;

        typedef TableRowValue_Int ThisType;
        typedef PKG::TableRowValue BaseType;
	    TableRowValue_Int(xx::MemPool* const& mempool) noexcept;
	    TableRowValue_Int(xx::BBuffer* const& bb);
		TableRowValue_Int(TableRowValue_Int const&) = delete;
		TableRowValue_Int& operator=(TableRowValue_Int const&) = delete;
        void ToString(xx::String& s) const noexcept override;
        void ToStringCore(xx::String& s) const noexcept override;
        void ToBBuffer(xx::BBuffer& bb) const noexcept override;
        int FromBBuffer(xx::BBuffer& bb) noexcept override;
        int FromBBufferCore(xx::BBuffer& bb) noexcept;
        void CopyTo(TableRowValue_Int* const& o) const noexcept;
        TableRowValue_Int* MakeCopy() const noexcept;
        TableRowValue_Int_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
    };
    class TableRowValue_NullableInt : public PKG::TableRowValue
    {
    public:
        std::optional<int32_t> value;

        typedef TableRowValue_NullableInt ThisType;
        typedef PKG::TableRowValue BaseType;
	    TableRowValue_NullableInt(xx::MemPool* const& mempool) noexcept;
	    TableRowValue_NullableInt(xx::BBuffer* const& bb);
		TableRowValue_NullableInt(TableRowValue_NullableInt const&) = delete;
		TableRowValue_NullableInt& operator=(TableRowValue_NullableInt const&) = delete;
        void ToString(xx::String& s) const noexcept override;
        void ToStringCore(xx::String& s) const noexcept override;
        void ToBBuffer(xx::BBuffer& bb) const noexcept override;
        int FromBBuffer(xx::BBuffer& bb) noexcept override;
        int FromBBufferCore(xx::BBuffer& bb) noexcept;
        void CopyTo(TableRowValue_NullableInt* const& o) const noexcept;
        TableRowValue_NullableInt* MakeCopy() const noexcept;
        TableRowValue_NullableInt_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
    };
    class TableRowValue_String : public PKG::TableRowValue
    {
    public:
        xx::String_p value;

        typedef TableRowValue_String ThisType;
        typedef PKG::TableRowValue BaseType;
	    TableRowValue_String(xx::MemPool* const& mempool) noexcept;
	    TableRowValue_String(xx::BBuffer* const& bb);
		TableRowValue_String(TableRowValue_String const&) = delete;
		TableRowValue_String& operator=(TableRowValue_String const&) = delete;
        void ToString(xx::String& s) const noexcept override;
        void ToStringCore(xx::String& s) const noexcept override;
        void ToBBuffer(xx::BBuffer& bb) const noexcept override;
        int FromBBuffer(xx::BBuffer& bb) noexcept override;
        int FromBBufferCore(xx::BBuffer& bb) noexcept;
        void CopyTo(TableRowValue_String* const& o) const noexcept;
        TableRowValue_String* MakeCopy() const noexcept;
        TableRowValue_String_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
    };
}
namespace xx
{
	template<> struct TypeId<PKG::Foo> { static const uint16_t value = 3; };
	template<> struct TypeId<xx::List<PKG::Foo_p>> { static const uint16_t value = 5; };
	template<> struct TypeId<PKG::DataSet> { static const uint16_t value = 6; };
	template<> struct TypeId<xx::List<PKG::Table_p>> { static const uint16_t value = 7; };
	template<> struct TypeId<PKG::Table> { static const uint16_t value = 8; };
	template<> struct TypeId<xx::List<PKG::TableColumn_p>> { static const uint16_t value = 9; };
	template<> struct TypeId<PKG::TableColumn> { static const uint16_t value = 10; };
	template<> struct TypeId<xx::List<PKG::TableRow_p>> { static const uint16_t value = 11; };
	template<> struct TypeId<PKG::TableRow> { static const uint16_t value = 12; };
	template<> struct TypeId<xx::List<PKG::TableRowValue_p>> { static const uint16_t value = 13; };
	template<> struct TypeId<PKG::TableRowValue> { static const uint16_t value = 14; };
	template<> struct TypeId<PKG::TableRowValue_Int> { static const uint16_t value = 15; };
	template<> struct TypeId<PKG::TableRowValue_NullableInt> { static const uint16_t value = 16; };
	template<> struct TypeId<PKG::TableRowValue_String> { static const uint16_t value = 17; };
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
        bb.Write(this->id);
        bb.Write(this->age);
        bb.Write(this->info);
        bb.Write(this->childs);
    }
    inline int Foo::FromBBuffer(xx::BBuffer& bb) noexcept
    {
        return this->FromBBufferCore(bb);
    }
    inline int Foo::FromBBufferCore(xx::BBuffer& bb) noexcept
    {
        if (int r = bb.Read(this->id)) return r;
        if (int r = bb.Read(this->age)) return r;
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->info)) return r;
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
        s.Append(", \"id\":", this->id);
        s.Append(", \"age\":", this->age);
        if (this->info) s.Append(", \"info\":\"", this->info, "\"");
        else s.Append(", \"info\":nil");
        s.Append(", \"childs\":", this->childs);
    }
    inline void Foo::CopyTo(Foo* const& o) const noexcept
    {
        o->id = this->id;
        o->age = this->age;
        o->info = this->info;
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

	inline DataSet::DataSet(xx::MemPool* const& mempool) noexcept
        : xx::Object(mempool)
	{
	}
	inline DataSet::DataSet(xx::BBuffer* const& bb)
        : xx::Object(bb)
	{
        if (int r = FromBBufferCore(*bb)) throw r;
	}
    inline void DataSet::ToBBuffer(xx::BBuffer& bb) const noexcept
    {
        bb.Write(this->tables);
    }
    inline int DataSet::FromBBuffer(xx::BBuffer& bb) noexcept
    {
        return this->FromBBufferCore(bb);
    }
    inline int DataSet::FromBBufferCore(xx::BBuffer& bb) noexcept
    {
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->tables)) return r;
        return 0;
    }

    inline void DataSet::ToString(xx::String& s) const noexcept
    {
        if (this->memHeader().flags)
        {
        	s.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else this->memHeader().flags = 1;

        s.Append("{ \"pkgTypeName\":\"DataSet\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(s);
        s.Append(" }");
        
        this->memHeader().flags = 0;
    }
    inline void DataSet::ToStringCore(xx::String& s) const noexcept
    {
        this->BaseType::ToStringCore(s);
        s.Append(", \"tables\":", this->tables);
    }
    inline void DataSet::CopyTo(DataSet* const& o) const noexcept
    {
        o->tables = this->tables;
    }
    inline DataSet* DataSet::MakeCopy() const noexcept
    {
        auto o = mempool->MPCreate<DataSet>();
        this->CopyTo(o);
        return o;
    }
    inline DataSet_p DataSet::MakePtrCopy() const noexcept
    {
        return DataSet_p(this->MakeCopy());
    }

	inline Table::Table(xx::MemPool* const& mempool) noexcept
        : xx::Object(mempool)
	{
	}
	inline Table::Table(xx::BBuffer* const& bb)
        : xx::Object(bb)
	{
        if (int r = FromBBufferCore(*bb)) throw r;
	}
    inline void Table::ToBBuffer(xx::BBuffer& bb) const noexcept
    {
        bb.Write(this->parent);
        bb.Write(this->name);
        bb.Write(this->columns);
        bb.Write(this->rows);
    }
    inline int Table::FromBBuffer(xx::BBuffer& bb) noexcept
    {
        return this->FromBBufferCore(bb);
    }
    inline int Table::FromBBufferCore(xx::BBuffer& bb) noexcept
    {
        if (int r = bb.Read(this->parent)) return r;
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->name)) return r;
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->columns)) return r;
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->rows)) return r;
        return 0;
    }

    inline void Table::ToString(xx::String& s) const noexcept
    {
        if (this->memHeader().flags)
        {
        	s.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else this->memHeader().flags = 1;

        s.Append("{ \"pkgTypeName\":\"Table\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(s);
        s.Append(" }");
        
        this->memHeader().flags = 0;
    }
    inline void Table::ToStringCore(xx::String& s) const noexcept
    {
        this->BaseType::ToStringCore(s);
        s.Append(", \"parent\":", this->parent);
        if (this->name) s.Append(", \"name\":\"", this->name, "\"");
        else s.Append(", \"name\":nil");
        s.Append(", \"columns\":", this->columns);
        s.Append(", \"rows\":", this->rows);
    }
    inline void Table::CopyTo(Table* const& o) const noexcept
    {
        o->parent = this->parent;
        o->name = this->name;
        o->columns = this->columns;
        o->rows = this->rows;
    }
    inline Table* Table::MakeCopy() const noexcept
    {
        auto o = mempool->MPCreate<Table>();
        this->CopyTo(o);
        return o;
    }
    inline Table_p Table::MakePtrCopy() const noexcept
    {
        return Table_p(this->MakeCopy());
    }

	inline TableColumn::TableColumn(xx::MemPool* const& mempool) noexcept
        : xx::Object(mempool)
	{
	}
	inline TableColumn::TableColumn(xx::BBuffer* const& bb)
        : xx::Object(bb)
	{
        if (int r = FromBBufferCore(*bb)) throw r;
	}
    inline void TableColumn::ToBBuffer(xx::BBuffer& bb) const noexcept
    {
        bb.Write(this->name);
    }
    inline int TableColumn::FromBBuffer(xx::BBuffer& bb) noexcept
    {
        return this->FromBBufferCore(bb);
    }
    inline int TableColumn::FromBBufferCore(xx::BBuffer& bb) noexcept
    {
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->name)) return r;
        return 0;
    }

    inline void TableColumn::ToString(xx::String& s) const noexcept
    {
        if (this->memHeader().flags)
        {
        	s.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else this->memHeader().flags = 1;

        s.Append("{ \"pkgTypeName\":\"TableColumn\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(s);
        s.Append(" }");
        
        this->memHeader().flags = 0;
    }
    inline void TableColumn::ToStringCore(xx::String& s) const noexcept
    {
        this->BaseType::ToStringCore(s);
        if (this->name) s.Append(", \"name\":\"", this->name, "\"");
        else s.Append(", \"name\":nil");
    }
    inline void TableColumn::CopyTo(TableColumn* const& o) const noexcept
    {
        o->name = this->name;
    }
    inline TableColumn* TableColumn::MakeCopy() const noexcept
    {
        auto o = mempool->MPCreate<TableColumn>();
        this->CopyTo(o);
        return o;
    }
    inline TableColumn_p TableColumn::MakePtrCopy() const noexcept
    {
        return TableColumn_p(this->MakeCopy());
    }

	inline TableRow::TableRow(xx::MemPool* const& mempool) noexcept
        : xx::Object(mempool)
	{
	}
	inline TableRow::TableRow(xx::BBuffer* const& bb)
        : xx::Object(bb)
	{
        if (int r = FromBBufferCore(*bb)) throw r;
	}
    inline void TableRow::ToBBuffer(xx::BBuffer& bb) const noexcept
    {
        bb.Write(this->values);
    }
    inline int TableRow::FromBBuffer(xx::BBuffer& bb) noexcept
    {
        return this->FromBBufferCore(bb);
    }
    inline int TableRow::FromBBufferCore(xx::BBuffer& bb) noexcept
    {
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->values)) return r;
        return 0;
    }

    inline void TableRow::ToString(xx::String& s) const noexcept
    {
        if (this->memHeader().flags)
        {
        	s.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else this->memHeader().flags = 1;

        s.Append("{ \"pkgTypeName\":\"TableRow\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(s);
        s.Append(" }");
        
        this->memHeader().flags = 0;
    }
    inline void TableRow::ToStringCore(xx::String& s) const noexcept
    {
        this->BaseType::ToStringCore(s);
        s.Append(", \"values\":", this->values);
    }
    inline void TableRow::CopyTo(TableRow* const& o) const noexcept
    {
        o->values = this->values;
    }
    inline TableRow* TableRow::MakeCopy() const noexcept
    {
        auto o = mempool->MPCreate<TableRow>();
        this->CopyTo(o);
        return o;
    }
    inline TableRow_p TableRow::MakePtrCopy() const noexcept
    {
        return TableRow_p(this->MakeCopy());
    }

	inline TableRowValue::TableRowValue(xx::MemPool* const& mempool) noexcept
        : xx::Object(mempool)
	{
	}
	inline TableRowValue::TableRowValue(xx::BBuffer* const& bb)
        : xx::Object(bb)
	{
        if (int r = FromBBufferCore(*bb)) throw r;
	}
    inline void TableRowValue::ToBBuffer(xx::BBuffer& bb) const noexcept
    {
    }
    inline int TableRowValue::FromBBuffer(xx::BBuffer& bb) noexcept
    {
        return this->FromBBufferCore(bb);
    }
    inline int TableRowValue::FromBBufferCore(xx::BBuffer& bb) noexcept
    {
        return 0;
    }

    inline void TableRowValue::ToString(xx::String& s) const noexcept
    {
        if (this->memHeader().flags)
        {
        	s.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else this->memHeader().flags = 1;

        s.Append("{ \"pkgTypeName\":\"TableRowValue\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(s);
        s.Append(" }");
        
        this->memHeader().flags = 0;
    }
    inline void TableRowValue::ToStringCore(xx::String& s) const noexcept
    {
        this->BaseType::ToStringCore(s);
    }
    inline void TableRowValue::CopyTo(TableRowValue* const& o) const noexcept
    {
    }
    inline TableRowValue* TableRowValue::MakeCopy() const noexcept
    {
        auto o = mempool->MPCreate<TableRowValue>();
        this->CopyTo(o);
        return o;
    }
    inline TableRowValue_p TableRowValue::MakePtrCopy() const noexcept
    {
        return TableRowValue_p(this->MakeCopy());
    }

	inline TableRowValue_Int::TableRowValue_Int(xx::MemPool* const& mempool) noexcept
        : PKG::TableRowValue(mempool)
	{
	}
	inline TableRowValue_Int::TableRowValue_Int(xx::BBuffer* const& bb)
        : PKG::TableRowValue(bb)
	{
        if (int r = FromBBufferCore(*bb)) throw r;
	}
    inline void TableRowValue_Int::ToBBuffer(xx::BBuffer& bb) const noexcept
    {
        this->BaseType::ToBBuffer(bb);
        bb.Write(this->value);
    }
    inline int TableRowValue_Int::FromBBuffer(xx::BBuffer& bb) noexcept
    {
        if (int r = this->BaseType::FromBBuffer(bb)) return r;
        return this->FromBBufferCore(bb);
    }
    inline int TableRowValue_Int::FromBBufferCore(xx::BBuffer& bb) noexcept
    {
        if (int r = bb.Read(this->value)) return r;
        return 0;
    }

    inline void TableRowValue_Int::ToString(xx::String& s) const noexcept
    {
        if (this->memHeader().flags)
        {
        	s.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else this->memHeader().flags = 1;

        s.Append("{ \"pkgTypeName\":\"TableRowValue_Int\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(s);
        s.Append(" }");
        
        this->memHeader().flags = 0;
    }
    inline void TableRowValue_Int::ToStringCore(xx::String& s) const noexcept
    {
        this->BaseType::ToStringCore(s);
        s.Append(", \"value\":", this->value);
    }
    inline void TableRowValue_Int::CopyTo(TableRowValue_Int* const& o) const noexcept
    {
        this->BaseType::CopyTo(o);
        o->value = this->value;
    }
    inline TableRowValue_Int* TableRowValue_Int::MakeCopy() const noexcept
    {
        auto o = mempool->MPCreate<TableRowValue_Int>();
        this->CopyTo(o);
        return o;
    }
    inline TableRowValue_Int_p TableRowValue_Int::MakePtrCopy() const noexcept
    {
        return TableRowValue_Int_p(this->MakeCopy());
    }

	inline TableRowValue_NullableInt::TableRowValue_NullableInt(xx::MemPool* const& mempool) noexcept
        : PKG::TableRowValue(mempool)
	{
	}
	inline TableRowValue_NullableInt::TableRowValue_NullableInt(xx::BBuffer* const& bb)
        : PKG::TableRowValue(bb)
	{
        if (int r = FromBBufferCore(*bb)) throw r;
	}
    inline void TableRowValue_NullableInt::ToBBuffer(xx::BBuffer& bb) const noexcept
    {
        this->BaseType::ToBBuffer(bb);
        bb.Write(this->value);
    }
    inline int TableRowValue_NullableInt::FromBBuffer(xx::BBuffer& bb) noexcept
    {
        if (int r = this->BaseType::FromBBuffer(bb)) return r;
        return this->FromBBufferCore(bb);
    }
    inline int TableRowValue_NullableInt::FromBBufferCore(xx::BBuffer& bb) noexcept
    {
        if (int r = bb.Read(this->value)) return r;
        return 0;
    }

    inline void TableRowValue_NullableInt::ToString(xx::String& s) const noexcept
    {
        if (this->memHeader().flags)
        {
        	s.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else this->memHeader().flags = 1;

        s.Append("{ \"pkgTypeName\":\"TableRowValue_NullableInt\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(s);
        s.Append(" }");
        
        this->memHeader().flags = 0;
    }
    inline void TableRowValue_NullableInt::ToStringCore(xx::String& s) const noexcept
    {
        this->BaseType::ToStringCore(s);
        s.Append(", \"value\":", this->value);
    }
    inline void TableRowValue_NullableInt::CopyTo(TableRowValue_NullableInt* const& o) const noexcept
    {
        this->BaseType::CopyTo(o);
        o->value = this->value;
    }
    inline TableRowValue_NullableInt* TableRowValue_NullableInt::MakeCopy() const noexcept
    {
        auto o = mempool->MPCreate<TableRowValue_NullableInt>();
        this->CopyTo(o);
        return o;
    }
    inline TableRowValue_NullableInt_p TableRowValue_NullableInt::MakePtrCopy() const noexcept
    {
        return TableRowValue_NullableInt_p(this->MakeCopy());
    }

	inline TableRowValue_String::TableRowValue_String(xx::MemPool* const& mempool) noexcept
        : PKG::TableRowValue(mempool)
	{
	}
	inline TableRowValue_String::TableRowValue_String(xx::BBuffer* const& bb)
        : PKG::TableRowValue(bb)
	{
        if (int r = FromBBufferCore(*bb)) throw r;
	}
    inline void TableRowValue_String::ToBBuffer(xx::BBuffer& bb) const noexcept
    {
        this->BaseType::ToBBuffer(bb);
        bb.Write(this->value);
    }
    inline int TableRowValue_String::FromBBuffer(xx::BBuffer& bb) noexcept
    {
        if (int r = this->BaseType::FromBBuffer(bb)) return r;
        return this->FromBBufferCore(bb);
    }
    inline int TableRowValue_String::FromBBufferCore(xx::BBuffer& bb) noexcept
    {
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->value)) return r;
        return 0;
    }

    inline void TableRowValue_String::ToString(xx::String& s) const noexcept
    {
        if (this->memHeader().flags)
        {
        	s.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else this->memHeader().flags = 1;

        s.Append("{ \"pkgTypeName\":\"TableRowValue_String\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(s);
        s.Append(" }");
        
        this->memHeader().flags = 0;
    }
    inline void TableRowValue_String::ToStringCore(xx::String& s) const noexcept
    {
        this->BaseType::ToStringCore(s);
        if (this->value) s.Append(", \"value\":\"", this->value, "\"");
        else s.Append(", \"value\":nil");
    }
    inline void TableRowValue_String::CopyTo(TableRowValue_String* const& o) const noexcept
    {
        this->BaseType::CopyTo(o);
        o->value = this->value;
    }
    inline TableRowValue_String* TableRowValue_String::MakeCopy() const noexcept
    {
        auto o = mempool->MPCreate<TableRowValue_String>();
        this->CopyTo(o);
        return o;
    }
    inline TableRowValue_String_p TableRowValue_String::MakePtrCopy() const noexcept
    {
        return TableRowValue_String_p(this->MakeCopy());
    }

}
namespace PKG
{
	inline void AllTypesRegister() noexcept
	{
        xx::MemPool::RegisterInternals();
	    xx::MemPool::Register<PKG::Foo, xx::Object>();
	    xx::MemPool::Register<xx::List<PKG::Foo_p>, xx::Object>();
	    xx::MemPool::Register<PKG::DataSet, xx::Object>();
	    xx::MemPool::Register<xx::List<PKG::Table_p>, xx::Object>();
	    xx::MemPool::Register<PKG::Table, xx::Object>();
	    xx::MemPool::Register<xx::List<PKG::TableColumn_p>, xx::Object>();
	    xx::MemPool::Register<PKG::TableColumn, xx::Object>();
	    xx::MemPool::Register<xx::List<PKG::TableRow_p>, xx::Object>();
	    xx::MemPool::Register<PKG::TableRow, xx::Object>();
	    xx::MemPool::Register<xx::List<PKG::TableRowValue_p>, xx::Object>();
	    xx::MemPool::Register<PKG::TableRowValue, xx::Object>();
	    xx::MemPool::Register<PKG::TableRowValue_Int, PKG::TableRowValue>();
	    xx::MemPool::Register<PKG::TableRowValue_NullableInt, PKG::TableRowValue>();
	    xx::MemPool::Register<PKG::TableRowValue_String, PKG::TableRowValue>();
	}
}
