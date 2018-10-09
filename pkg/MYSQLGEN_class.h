#pragma once
#include "xx.h"

namespace MYSQLGEN
{
	struct PkgGenMd5
	{
		static constexpr char const* value = "bb226d2a0d2ae293b28be6a67bf1a219";
    };

    class DbTable;
    using DbTable_p = xx::Ptr<DbTable>;
    using DbTable_r = xx::Ref<DbTable>;

    class DbColumn;
    using DbColumn_p = xx::Ptr<DbColumn>;
    using DbColumn_r = xx::Ref<DbColumn>;

namespace Tables
{
    class TABLES;
    using TABLES_p = xx::Ptr<TABLES>;
    using TABLES_r = xx::Ref<TABLES>;

    class COLUMNS;
    using COLUMNS_p = xx::Ptr<COLUMNS>;
    using COLUMNS_r = xx::Ref<COLUMNS>;

    class show_create_table;
    using show_create_table_p = xx::Ptr<show_create_table>;
    using show_create_table_r = xx::Ref<show_create_table>;

}
namespace Tables
{
    class TABLES : public xx::Object
    {
    public:
        xx::String_p TABLE_CATALOG;
        xx::String_p TABLE_SCHEMA;
        xx::String_p TABLE_NAME;
        xx::String_p TABLE_TYPE;
        xx::String_p ENGINE;
        std::optional<int64_t> VERSION;
        xx::String_p ROW_FORMAT;
        std::optional<int64_t> TABLE_ROWS;
        std::optional<int64_t> AVG_ROW_LENGTH;
        std::optional<int64_t> DATA_LENGTH;
        std::optional<int64_t> MAX_DATA_LENGTH;
        std::optional<int64_t> INDEX_LENGTH;
        std::optional<int64_t> DATA_FREE;
        std::optional<int64_t> AUTO_INCREMENT;
        xx::String_p CREATE_TIME;
        xx::String_p UPDATE_TIME;
        xx::String_p CHECK_TIME;
        xx::String_p TABLE_COLLATION;
        std::optional<int64_t> CHECKSUM;
        xx::String_p CREATE_OPTIONS;
        xx::String_p TABLE_COMMENT;

        typedef TABLES ThisType;
        typedef xx::Object BaseType;
	    TABLES(xx::MemPool* const& mempool) noexcept;
	    TABLES(xx::BBuffer* const& bb);
		TABLES(TABLES const&) = delete;
		TABLES& operator=(TABLES const&) = delete;
        void ToString(xx::String& s) const noexcept override;
        void ToStringCore(xx::String& s) const noexcept override;
        void ToBBuffer(xx::BBuffer& bb) const noexcept override;
        int FromBBuffer(xx::BBuffer& bb) noexcept override;
        int FromBBufferCore(xx::BBuffer& bb) noexcept;
        void CopyTo(TABLES* const& o) const noexcept;
        TABLES* MakeCopy() const noexcept;
        TABLES_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
    };
    class COLUMNS : public xx::Object
    {
    public:
        xx::String_p TABLE_CATALOG;
        xx::String_p TABLE_SCHEMA;
        xx::String_p TABLE_NAME;
        xx::String_p COLUMN_NAME;
        int64_t ORDINAL_POSITION = 0;
        xx::String_p COLUMN_DEFAULT;
        xx::String_p IS_NULLABLE;
        xx::String_p DATA_TYPE;
        std::optional<int64_t> CHARACTER_MAXIMUM_LENGTH;
        std::optional<int64_t> CHARACTER_OCTET_LENGTH;
        std::optional<int64_t> NUMERIC_PRECISION;
        std::optional<int64_t> NUMERIC_SCALE;
        std::optional<int64_t> DATETIME_PRECISION;
        xx::String_p CHARACTER_SET_NAME;
        xx::String_p COLLATION_NAME;
        xx::String_p COLUMN_TYPE;
        xx::String_p COLUMN_KEY;
        xx::String_p EXTRA;
        xx::String_p PRIVILEGES;
        xx::String_p COLUMN_COMMENT;
        xx::String_p GENERATION_EXPRESSION;

        typedef COLUMNS ThisType;
        typedef xx::Object BaseType;
	    COLUMNS(xx::MemPool* const& mempool) noexcept;
	    COLUMNS(xx::BBuffer* const& bb);
		COLUMNS(COLUMNS const&) = delete;
		COLUMNS& operator=(COLUMNS const&) = delete;
        void ToString(xx::String& s) const noexcept override;
        void ToStringCore(xx::String& s) const noexcept override;
        void ToBBuffer(xx::BBuffer& bb) const noexcept override;
        int FromBBuffer(xx::BBuffer& bb) noexcept override;
        int FromBBufferCore(xx::BBuffer& bb) noexcept;
        void CopyTo(COLUMNS* const& o) const noexcept;
        COLUMNS* MakeCopy() const noexcept;
        COLUMNS_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
    };
}
    class DbTable : public MYSQLGEN::Tables::TABLES
    {
    public:
        xx::List_p<MYSQLGEN::DbColumn_p> childs;
        xx::String_p createScript;

        typedef DbTable ThisType;
        typedef MYSQLGEN::Tables::TABLES BaseType;
	    DbTable(xx::MemPool* const& mempool) noexcept;
	    DbTable(xx::BBuffer* const& bb);
		DbTable(DbTable const&) = delete;
		DbTable& operator=(DbTable const&) = delete;
        void ToString(xx::String& s) const noexcept override;
        void ToStringCore(xx::String& s) const noexcept override;
        void ToBBuffer(xx::BBuffer& bb) const noexcept override;
        int FromBBuffer(xx::BBuffer& bb) noexcept override;
        int FromBBufferCore(xx::BBuffer& bb) noexcept;
        void CopyTo(DbTable* const& o) const noexcept;
        DbTable* MakeCopy() const noexcept;
        DbTable_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
    };
    class DbColumn : public MYSQLGEN::Tables::COLUMNS
    {
    public:
        MYSQLGEN::DbTable_p parent;

        typedef DbColumn ThisType;
        typedef MYSQLGEN::Tables::COLUMNS BaseType;
	    DbColumn(xx::MemPool* const& mempool) noexcept;
	    DbColumn(xx::BBuffer* const& bb);
		DbColumn(DbColumn const&) = delete;
		DbColumn& operator=(DbColumn const&) = delete;
        void ToString(xx::String& s) const noexcept override;
        void ToStringCore(xx::String& s) const noexcept override;
        void ToBBuffer(xx::BBuffer& bb) const noexcept override;
        int FromBBuffer(xx::BBuffer& bb) noexcept override;
        int FromBBufferCore(xx::BBuffer& bb) noexcept;
        void CopyTo(DbColumn* const& o) const noexcept;
        DbColumn* MakeCopy() const noexcept;
        DbColumn_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
    };
namespace Tables
{
    class show_create_table : public xx::Object
    {
    public:
        xx::String_p Table;
        xx::String_p CreateTable;

        typedef show_create_table ThisType;
        typedef xx::Object BaseType;
	    show_create_table(xx::MemPool* const& mempool) noexcept;
	    show_create_table(xx::BBuffer* const& bb);
		show_create_table(show_create_table const&) = delete;
		show_create_table& operator=(show_create_table const&) = delete;
        void ToString(xx::String& s) const noexcept override;
        void ToStringCore(xx::String& s) const noexcept override;
        void ToBBuffer(xx::BBuffer& bb) const noexcept override;
        int FromBBuffer(xx::BBuffer& bb) noexcept override;
        int FromBBufferCore(xx::BBuffer& bb) noexcept;
        void CopyTo(show_create_table* const& o) const noexcept;
        show_create_table* MakeCopy() const noexcept;
        show_create_table_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
    };
}
}
namespace xx
{
	template<> struct TypeId<MYSQLGEN::DbTable> { static const uint16_t value = 3; };
	template<> struct TypeId<MYSQLGEN::Tables::TABLES> { static const uint16_t value = 4; };
	template<> struct TypeId<xx::List<MYSQLGEN::DbColumn_p>> { static const uint16_t value = 8; };
	template<> struct TypeId<MYSQLGEN::DbColumn> { static const uint16_t value = 6; };
	template<> struct TypeId<MYSQLGEN::Tables::COLUMNS> { static const uint16_t value = 7; };
	template<> struct TypeId<MYSQLGEN::Tables::show_create_table> { static const uint16_t value = 9; };
}
namespace MYSQLGEN
{
	inline DbTable::DbTable(xx::MemPool* const& mempool) noexcept
        : MYSQLGEN::Tables::TABLES(mempool)
	{
	}
	inline DbTable::DbTable(xx::BBuffer* const& bb)
        : MYSQLGEN::Tables::TABLES(bb)
	{
        if (int r = FromBBufferCore(*bb)) throw r;
	}
    inline void DbTable::ToBBuffer(xx::BBuffer& bb) const noexcept
    {
        this->BaseType::ToBBuffer(bb);
        bb.Write(this->childs);
        bb.Write(this->createScript);
    }
    inline int DbTable::FromBBuffer(xx::BBuffer& bb) noexcept
    {
        if (int r = this->BaseType::FromBBuffer(bb)) return r;
        return this->FromBBufferCore(bb);
    }
    inline int DbTable::FromBBufferCore(xx::BBuffer& bb) noexcept
    {
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->childs)) return r;
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->createScript)) return r;
        return 0;
    }

    inline void DbTable::ToString(xx::String& s) const noexcept
    {
        if (this->memHeader().flags)
        {
        	s.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else this->memHeader().flags = 1;

        s.Append("{ \"pkgTypeName\":\"DbTable\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(s);
        s.Append(" }");
        
        this->memHeader().flags = 0;
    }
    inline void DbTable::ToStringCore(xx::String& s) const noexcept
    {
        this->BaseType::ToStringCore(s);
        s.Append(", \"childs\":", this->childs);
        if (this->createScript) s.Append(", \"createScript\":\"", this->createScript, "\"");
        else s.Append(", \"createScript\":nil");
    }
    inline void DbTable::CopyTo(DbTable* const& o) const noexcept
    {
        this->BaseType::CopyTo(o);
        o->childs = this->childs;
        o->createScript = this->createScript;
    }
    inline DbTable* DbTable::MakeCopy() const noexcept
    {
        auto o = mempool->MPCreate<DbTable>();
        this->CopyTo(o);
        return o;
    }
    inline DbTable_p DbTable::MakePtrCopy() const noexcept
    {
        return DbTable_p(this->MakeCopy());
    }

	inline DbColumn::DbColumn(xx::MemPool* const& mempool) noexcept
        : MYSQLGEN::Tables::COLUMNS(mempool)
	{
	}
	inline DbColumn::DbColumn(xx::BBuffer* const& bb)
        : MYSQLGEN::Tables::COLUMNS(bb)
	{
        if (int r = FromBBufferCore(*bb)) throw r;
	}
    inline void DbColumn::ToBBuffer(xx::BBuffer& bb) const noexcept
    {
        this->BaseType::ToBBuffer(bb);
        bb.Write(this->parent);
    }
    inline int DbColumn::FromBBuffer(xx::BBuffer& bb) noexcept
    {
        if (int r = this->BaseType::FromBBuffer(bb)) return r;
        return this->FromBBufferCore(bb);
    }
    inline int DbColumn::FromBBufferCore(xx::BBuffer& bb) noexcept
    {
        if (int r = bb.Read(this->parent)) return r;
        return 0;
    }

    inline void DbColumn::ToString(xx::String& s) const noexcept
    {
        if (this->memHeader().flags)
        {
        	s.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else this->memHeader().flags = 1;

        s.Append("{ \"pkgTypeName\":\"DbColumn\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(s);
        s.Append(" }");
        
        this->memHeader().flags = 0;
    }
    inline void DbColumn::ToStringCore(xx::String& s) const noexcept
    {
        this->BaseType::ToStringCore(s);
        s.Append(", \"parent\":", this->parent);
    }
    inline void DbColumn::CopyTo(DbColumn* const& o) const noexcept
    {
        this->BaseType::CopyTo(o);
        o->parent = this->parent;
    }
    inline DbColumn* DbColumn::MakeCopy() const noexcept
    {
        auto o = mempool->MPCreate<DbColumn>();
        this->CopyTo(o);
        return o;
    }
    inline DbColumn_p DbColumn::MakePtrCopy() const noexcept
    {
        return DbColumn_p(this->MakeCopy());
    }

namespace Tables
{
	inline TABLES::TABLES(xx::MemPool* const& mempool) noexcept
        : xx::Object(mempool)
	{
	}
	inline TABLES::TABLES(xx::BBuffer* const& bb)
        : xx::Object(bb)
	{
        if (int r = FromBBufferCore(*bb)) throw r;
	}
    inline void TABLES::ToBBuffer(xx::BBuffer& bb) const noexcept
    {
        bb.Write(this->TABLE_CATALOG);
        bb.Write(this->TABLE_SCHEMA);
        bb.Write(this->TABLE_NAME);
        bb.Write(this->TABLE_TYPE);
        bb.Write(this->ENGINE);
        bb.Write(this->VERSION);
        bb.Write(this->ROW_FORMAT);
        bb.Write(this->TABLE_ROWS);
        bb.Write(this->AVG_ROW_LENGTH);
        bb.Write(this->DATA_LENGTH);
        bb.Write(this->MAX_DATA_LENGTH);
        bb.Write(this->INDEX_LENGTH);
        bb.Write(this->DATA_FREE);
        bb.Write(this->AUTO_INCREMENT);
        bb.Write(this->CREATE_TIME);
        bb.Write(this->UPDATE_TIME);
        bb.Write(this->CHECK_TIME);
        bb.Write(this->TABLE_COLLATION);
        bb.Write(this->CHECKSUM);
        bb.Write(this->CREATE_OPTIONS);
        bb.Write(this->TABLE_COMMENT);
    }
    inline int TABLES::FromBBuffer(xx::BBuffer& bb) noexcept
    {
        return this->FromBBufferCore(bb);
    }
    inline int TABLES::FromBBufferCore(xx::BBuffer& bb) noexcept
    {
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->TABLE_CATALOG)) return r;
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->TABLE_SCHEMA)) return r;
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->TABLE_NAME)) return r;
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->TABLE_TYPE)) return r;
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->ENGINE)) return r;
        if (int r = bb.Read(this->VERSION)) return r;
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->ROW_FORMAT)) return r;
        if (int r = bb.Read(this->TABLE_ROWS)) return r;
        if (int r = bb.Read(this->AVG_ROW_LENGTH)) return r;
        if (int r = bb.Read(this->DATA_LENGTH)) return r;
        if (int r = bb.Read(this->MAX_DATA_LENGTH)) return r;
        if (int r = bb.Read(this->INDEX_LENGTH)) return r;
        if (int r = bb.Read(this->DATA_FREE)) return r;
        if (int r = bb.Read(this->AUTO_INCREMENT)) return r;
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->CREATE_TIME)) return r;
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->UPDATE_TIME)) return r;
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->CHECK_TIME)) return r;
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->TABLE_COLLATION)) return r;
        if (int r = bb.Read(this->CHECKSUM)) return r;
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->CREATE_OPTIONS)) return r;
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->TABLE_COMMENT)) return r;
        return 0;
    }

    inline void TABLES::ToString(xx::String& s) const noexcept
    {
        if (this->memHeader().flags)
        {
        	s.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else this->memHeader().flags = 1;

        s.Append("{ \"pkgTypeName\":\"Tables.TABLES\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(s);
        s.Append(" }");
        
        this->memHeader().flags = 0;
    }
    inline void TABLES::ToStringCore(xx::String& s) const noexcept
    {
        this->BaseType::ToStringCore(s);
        if (this->TABLE_CATALOG) s.Append(", \"TABLE_CATALOG\":\"", this->TABLE_CATALOG, "\"");
        else s.Append(", \"TABLE_CATALOG\":nil");
        if (this->TABLE_SCHEMA) s.Append(", \"TABLE_SCHEMA\":\"", this->TABLE_SCHEMA, "\"");
        else s.Append(", \"TABLE_SCHEMA\":nil");
        if (this->TABLE_NAME) s.Append(", \"TABLE_NAME\":\"", this->TABLE_NAME, "\"");
        else s.Append(", \"TABLE_NAME\":nil");
        if (this->TABLE_TYPE) s.Append(", \"TABLE_TYPE\":\"", this->TABLE_TYPE, "\"");
        else s.Append(", \"TABLE_TYPE\":nil");
        if (this->ENGINE) s.Append(", \"ENGINE\":\"", this->ENGINE, "\"");
        else s.Append(", \"ENGINE\":nil");
        s.Append(", \"VERSION\":", this->VERSION);
        if (this->ROW_FORMAT) s.Append(", \"ROW_FORMAT\":\"", this->ROW_FORMAT, "\"");
        else s.Append(", \"ROW_FORMAT\":nil");
        s.Append(", \"TABLE_ROWS\":", this->TABLE_ROWS);
        s.Append(", \"AVG_ROW_LENGTH\":", this->AVG_ROW_LENGTH);
        s.Append(", \"DATA_LENGTH\":", this->DATA_LENGTH);
        s.Append(", \"MAX_DATA_LENGTH\":", this->MAX_DATA_LENGTH);
        s.Append(", \"INDEX_LENGTH\":", this->INDEX_LENGTH);
        s.Append(", \"DATA_FREE\":", this->DATA_FREE);
        s.Append(", \"AUTO_INCREMENT\":", this->AUTO_INCREMENT);
        if (this->CREATE_TIME) s.Append(", \"CREATE_TIME\":\"", this->CREATE_TIME, "\"");
        else s.Append(", \"CREATE_TIME\":nil");
        if (this->UPDATE_TIME) s.Append(", \"UPDATE_TIME\":\"", this->UPDATE_TIME, "\"");
        else s.Append(", \"UPDATE_TIME\":nil");
        if (this->CHECK_TIME) s.Append(", \"CHECK_TIME\":\"", this->CHECK_TIME, "\"");
        else s.Append(", \"CHECK_TIME\":nil");
        if (this->TABLE_COLLATION) s.Append(", \"TABLE_COLLATION\":\"", this->TABLE_COLLATION, "\"");
        else s.Append(", \"TABLE_COLLATION\":nil");
        s.Append(", \"CHECKSUM\":", this->CHECKSUM);
        if (this->CREATE_OPTIONS) s.Append(", \"CREATE_OPTIONS\":\"", this->CREATE_OPTIONS, "\"");
        else s.Append(", \"CREATE_OPTIONS\":nil");
        if (this->TABLE_COMMENT) s.Append(", \"TABLE_COMMENT\":\"", this->TABLE_COMMENT, "\"");
        else s.Append(", \"TABLE_COMMENT\":nil");
    }
    inline void TABLES::CopyTo(TABLES* const& o) const noexcept
    {
        o->TABLE_CATALOG = this->TABLE_CATALOG;
        o->TABLE_SCHEMA = this->TABLE_SCHEMA;
        o->TABLE_NAME = this->TABLE_NAME;
        o->TABLE_TYPE = this->TABLE_TYPE;
        o->ENGINE = this->ENGINE;
        o->VERSION = this->VERSION;
        o->ROW_FORMAT = this->ROW_FORMAT;
        o->TABLE_ROWS = this->TABLE_ROWS;
        o->AVG_ROW_LENGTH = this->AVG_ROW_LENGTH;
        o->DATA_LENGTH = this->DATA_LENGTH;
        o->MAX_DATA_LENGTH = this->MAX_DATA_LENGTH;
        o->INDEX_LENGTH = this->INDEX_LENGTH;
        o->DATA_FREE = this->DATA_FREE;
        o->AUTO_INCREMENT = this->AUTO_INCREMENT;
        o->CREATE_TIME = this->CREATE_TIME;
        o->UPDATE_TIME = this->UPDATE_TIME;
        o->CHECK_TIME = this->CHECK_TIME;
        o->TABLE_COLLATION = this->TABLE_COLLATION;
        o->CHECKSUM = this->CHECKSUM;
        o->CREATE_OPTIONS = this->CREATE_OPTIONS;
        o->TABLE_COMMENT = this->TABLE_COMMENT;
    }
    inline TABLES* TABLES::MakeCopy() const noexcept
    {
        auto o = mempool->MPCreate<TABLES>();
        this->CopyTo(o);
        return o;
    }
    inline TABLES_p TABLES::MakePtrCopy() const noexcept
    {
        return TABLES_p(this->MakeCopy());
    }

	inline COLUMNS::COLUMNS(xx::MemPool* const& mempool) noexcept
        : xx::Object(mempool)
	{
	}
	inline COLUMNS::COLUMNS(xx::BBuffer* const& bb)
        : xx::Object(bb)
	{
        if (int r = FromBBufferCore(*bb)) throw r;
	}
    inline void COLUMNS::ToBBuffer(xx::BBuffer& bb) const noexcept
    {
        bb.Write(this->TABLE_CATALOG);
        bb.Write(this->TABLE_SCHEMA);
        bb.Write(this->TABLE_NAME);
        bb.Write(this->COLUMN_NAME);
        bb.Write(this->ORDINAL_POSITION);
        bb.Write(this->COLUMN_DEFAULT);
        bb.Write(this->IS_NULLABLE);
        bb.Write(this->DATA_TYPE);
        bb.Write(this->CHARACTER_MAXIMUM_LENGTH);
        bb.Write(this->CHARACTER_OCTET_LENGTH);
        bb.Write(this->NUMERIC_PRECISION);
        bb.Write(this->NUMERIC_SCALE);
        bb.Write(this->DATETIME_PRECISION);
        bb.Write(this->CHARACTER_SET_NAME);
        bb.Write(this->COLLATION_NAME);
        bb.Write(this->COLUMN_TYPE);
        bb.Write(this->COLUMN_KEY);
        bb.Write(this->EXTRA);
        bb.Write(this->PRIVILEGES);
        bb.Write(this->COLUMN_COMMENT);
        bb.Write(this->GENERATION_EXPRESSION);
    }
    inline int COLUMNS::FromBBuffer(xx::BBuffer& bb) noexcept
    {
        return this->FromBBufferCore(bb);
    }
    inline int COLUMNS::FromBBufferCore(xx::BBuffer& bb) noexcept
    {
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->TABLE_CATALOG)) return r;
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->TABLE_SCHEMA)) return r;
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->TABLE_NAME)) return r;
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->COLUMN_NAME)) return r;
        if (int r = bb.Read(this->ORDINAL_POSITION)) return r;
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->COLUMN_DEFAULT)) return r;
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->IS_NULLABLE)) return r;
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->DATA_TYPE)) return r;
        if (int r = bb.Read(this->CHARACTER_MAXIMUM_LENGTH)) return r;
        if (int r = bb.Read(this->CHARACTER_OCTET_LENGTH)) return r;
        if (int r = bb.Read(this->NUMERIC_PRECISION)) return r;
        if (int r = bb.Read(this->NUMERIC_SCALE)) return r;
        if (int r = bb.Read(this->DATETIME_PRECISION)) return r;
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->CHARACTER_SET_NAME)) return r;
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->COLLATION_NAME)) return r;
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->COLUMN_TYPE)) return r;
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->COLUMN_KEY)) return r;
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->EXTRA)) return r;
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->PRIVILEGES)) return r;
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->COLUMN_COMMENT)) return r;
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->GENERATION_EXPRESSION)) return r;
        return 0;
    }

    inline void COLUMNS::ToString(xx::String& s) const noexcept
    {
        if (this->memHeader().flags)
        {
        	s.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else this->memHeader().flags = 1;

        s.Append("{ \"pkgTypeName\":\"Tables.COLUMNS\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(s);
        s.Append(" }");
        
        this->memHeader().flags = 0;
    }
    inline void COLUMNS::ToStringCore(xx::String& s) const noexcept
    {
        this->BaseType::ToStringCore(s);
        if (this->TABLE_CATALOG) s.Append(", \"TABLE_CATALOG\":\"", this->TABLE_CATALOG, "\"");
        else s.Append(", \"TABLE_CATALOG\":nil");
        if (this->TABLE_SCHEMA) s.Append(", \"TABLE_SCHEMA\":\"", this->TABLE_SCHEMA, "\"");
        else s.Append(", \"TABLE_SCHEMA\":nil");
        if (this->TABLE_NAME) s.Append(", \"TABLE_NAME\":\"", this->TABLE_NAME, "\"");
        else s.Append(", \"TABLE_NAME\":nil");
        if (this->COLUMN_NAME) s.Append(", \"COLUMN_NAME\":\"", this->COLUMN_NAME, "\"");
        else s.Append(", \"COLUMN_NAME\":nil");
        s.Append(", \"ORDINAL_POSITION\":", this->ORDINAL_POSITION);
        if (this->COLUMN_DEFAULT) s.Append(", \"COLUMN_DEFAULT\":\"", this->COLUMN_DEFAULT, "\"");
        else s.Append(", \"COLUMN_DEFAULT\":nil");
        if (this->IS_NULLABLE) s.Append(", \"IS_NULLABLE\":\"", this->IS_NULLABLE, "\"");
        else s.Append(", \"IS_NULLABLE\":nil");
        if (this->DATA_TYPE) s.Append(", \"DATA_TYPE\":\"", this->DATA_TYPE, "\"");
        else s.Append(", \"DATA_TYPE\":nil");
        s.Append(", \"CHARACTER_MAXIMUM_LENGTH\":", this->CHARACTER_MAXIMUM_LENGTH);
        s.Append(", \"CHARACTER_OCTET_LENGTH\":", this->CHARACTER_OCTET_LENGTH);
        s.Append(", \"NUMERIC_PRECISION\":", this->NUMERIC_PRECISION);
        s.Append(", \"NUMERIC_SCALE\":", this->NUMERIC_SCALE);
        s.Append(", \"DATETIME_PRECISION\":", this->DATETIME_PRECISION);
        if (this->CHARACTER_SET_NAME) s.Append(", \"CHARACTER_SET_NAME\":\"", this->CHARACTER_SET_NAME, "\"");
        else s.Append(", \"CHARACTER_SET_NAME\":nil");
        if (this->COLLATION_NAME) s.Append(", \"COLLATION_NAME\":\"", this->COLLATION_NAME, "\"");
        else s.Append(", \"COLLATION_NAME\":nil");
        if (this->COLUMN_TYPE) s.Append(", \"COLUMN_TYPE\":\"", this->COLUMN_TYPE, "\"");
        else s.Append(", \"COLUMN_TYPE\":nil");
        if (this->COLUMN_KEY) s.Append(", \"COLUMN_KEY\":\"", this->COLUMN_KEY, "\"");
        else s.Append(", \"COLUMN_KEY\":nil");
        if (this->EXTRA) s.Append(", \"EXTRA\":\"", this->EXTRA, "\"");
        else s.Append(", \"EXTRA\":nil");
        if (this->PRIVILEGES) s.Append(", \"PRIVILEGES\":\"", this->PRIVILEGES, "\"");
        else s.Append(", \"PRIVILEGES\":nil");
        if (this->COLUMN_COMMENT) s.Append(", \"COLUMN_COMMENT\":\"", this->COLUMN_COMMENT, "\"");
        else s.Append(", \"COLUMN_COMMENT\":nil");
        if (this->GENERATION_EXPRESSION) s.Append(", \"GENERATION_EXPRESSION\":\"", this->GENERATION_EXPRESSION, "\"");
        else s.Append(", \"GENERATION_EXPRESSION\":nil");
    }
    inline void COLUMNS::CopyTo(COLUMNS* const& o) const noexcept
    {
        o->TABLE_CATALOG = this->TABLE_CATALOG;
        o->TABLE_SCHEMA = this->TABLE_SCHEMA;
        o->TABLE_NAME = this->TABLE_NAME;
        o->COLUMN_NAME = this->COLUMN_NAME;
        o->ORDINAL_POSITION = this->ORDINAL_POSITION;
        o->COLUMN_DEFAULT = this->COLUMN_DEFAULT;
        o->IS_NULLABLE = this->IS_NULLABLE;
        o->DATA_TYPE = this->DATA_TYPE;
        o->CHARACTER_MAXIMUM_LENGTH = this->CHARACTER_MAXIMUM_LENGTH;
        o->CHARACTER_OCTET_LENGTH = this->CHARACTER_OCTET_LENGTH;
        o->NUMERIC_PRECISION = this->NUMERIC_PRECISION;
        o->NUMERIC_SCALE = this->NUMERIC_SCALE;
        o->DATETIME_PRECISION = this->DATETIME_PRECISION;
        o->CHARACTER_SET_NAME = this->CHARACTER_SET_NAME;
        o->COLLATION_NAME = this->COLLATION_NAME;
        o->COLUMN_TYPE = this->COLUMN_TYPE;
        o->COLUMN_KEY = this->COLUMN_KEY;
        o->EXTRA = this->EXTRA;
        o->PRIVILEGES = this->PRIVILEGES;
        o->COLUMN_COMMENT = this->COLUMN_COMMENT;
        o->GENERATION_EXPRESSION = this->GENERATION_EXPRESSION;
    }
    inline COLUMNS* COLUMNS::MakeCopy() const noexcept
    {
        auto o = mempool->MPCreate<COLUMNS>();
        this->CopyTo(o);
        return o;
    }
    inline COLUMNS_p COLUMNS::MakePtrCopy() const noexcept
    {
        return COLUMNS_p(this->MakeCopy());
    }

	inline show_create_table::show_create_table(xx::MemPool* const& mempool) noexcept
        : xx::Object(mempool)
	{
	}
	inline show_create_table::show_create_table(xx::BBuffer* const& bb)
        : xx::Object(bb)
	{
        if (int r = FromBBufferCore(*bb)) throw r;
	}
    inline void show_create_table::ToBBuffer(xx::BBuffer& bb) const noexcept
    {
        bb.Write(this->Table);
        bb.Write(this->CreateTable);
    }
    inline int show_create_table::FromBBuffer(xx::BBuffer& bb) noexcept
    {
        return this->FromBBufferCore(bb);
    }
    inline int show_create_table::FromBBufferCore(xx::BBuffer& bb) noexcept
    {
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->Table)) return r;
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->CreateTable)) return r;
        return 0;
    }

    inline void show_create_table::ToString(xx::String& s) const noexcept
    {
        if (this->memHeader().flags)
        {
        	s.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else this->memHeader().flags = 1;

        s.Append("{ \"pkgTypeName\":\"Tables.show_create_table\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(s);
        s.Append(" }");
        
        this->memHeader().flags = 0;
    }
    inline void show_create_table::ToStringCore(xx::String& s) const noexcept
    {
        this->BaseType::ToStringCore(s);
        if (this->Table) s.Append(", \"Table\":\"", this->Table, "\"");
        else s.Append(", \"Table\":nil");
        if (this->CreateTable) s.Append(", \"CreateTable\":\"", this->CreateTable, "\"");
        else s.Append(", \"CreateTable\":nil");
    }
    inline void show_create_table::CopyTo(show_create_table* const& o) const noexcept
    {
        o->Table = this->Table;
        o->CreateTable = this->CreateTable;
    }
    inline show_create_table* show_create_table::MakeCopy() const noexcept
    {
        auto o = mempool->MPCreate<show_create_table>();
        this->CopyTo(o);
        return o;
    }
    inline show_create_table_p show_create_table::MakePtrCopy() const noexcept
    {
        return show_create_table_p(this->MakeCopy());
    }

}
}
namespace MYSQLGEN
{
	inline void AllTypesRegister() noexcept
	{
        xx::MemPool::RegisterInternals();
	    xx::MemPool::Register<MYSQLGEN::DbTable, MYSQLGEN::Tables::TABLES>();
	    xx::MemPool::Register<MYSQLGEN::Tables::TABLES, xx::Object>();
	    xx::MemPool::Register<xx::List<MYSQLGEN::DbColumn_p>, xx::Object>();
	    xx::MemPool::Register<MYSQLGEN::DbColumn, MYSQLGEN::Tables::COLUMNS>();
	    xx::MemPool::Register<MYSQLGEN::Tables::COLUMNS, xx::Object>();
	    xx::MemPool::Register<MYSQLGEN::Tables::show_create_table, xx::Object>();
	}
}
