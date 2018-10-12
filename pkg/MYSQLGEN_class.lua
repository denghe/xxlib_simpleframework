
MYSQLGEN_PkgGenMd5_Value = '91e88cb33203899cbfee1be05be3121c'

MYSQLGEN_DbTable = {
    typeName = "MYSQLGEN_DbTable",
    typeId = 3,
    Create = function()
        local o = {}
        o.__proto = MYSQLGEN_DbTable
        o.__index = o
        o.__newindex = o
		o.__isReleased = false
		o.Release = function()
			o.__isReleased = true
		end


        o.childs = null -- List_MYSQLGEN_DbColumn_
        o.createScript = null -- String
        setmetatable( o, MYSQLGEN_Tables_TABLES.Create() )
        return o
    end,
    FromBBuffer = function( bb, o )
        local p = getmetatable( o )
        p.__proto.FromBBuffer( bb, p )
        local ReadObject = bb.ReadObject
        o.childs = ReadObject( bb )
        o.createScript = ReadObject( bb )
    end,
    ToBBuffer = function( bb, o )
        local p = getmetatable( o )
        p.__proto.ToBBuffer( bb, p )
        local WriteObject = bb.WriteObject
        WriteObject( bb, o.childs )
        WriteObject( bb, o.createScript )
    end
}
BBuffer.Register( MYSQLGEN_DbTable )
MYSQLGEN_Tables_TABLES = {
    typeName = "MYSQLGEN_Tables_TABLES",
    typeId = 4,
    Create = function()
        local o = {}
        o.__proto = MYSQLGEN_Tables_TABLES
        o.__index = o
        o.__newindex = o
		o.__isReleased = false
		o.Release = function()
			o.__isReleased = true
		end


        o.TABLE_CATALOG = null -- String
        o.TABLE_SCHEMA = null -- String
        o.TABLE_NAME = null -- String
        o.TABLE_TYPE = null -- String
        o.ENGINE = null -- String
        o.VERSION = null -- NullableInt64
        o.ROW_FORMAT = null -- String
        o.TABLE_ROWS = null -- NullableInt64
        o.AVG_ROW_LENGTH = null -- NullableInt64
        o.DATA_LENGTH = null -- NullableInt64
        o.MAX_DATA_LENGTH = null -- NullableInt64
        o.INDEX_LENGTH = null -- NullableInt64
        o.DATA_FREE = null -- NullableInt64
        o.AUTO_INCREMENT = null -- NullableInt64
        o.CREATE_TIME = null -- String
        o.UPDATE_TIME = null -- String
        o.CHECK_TIME = null -- String
        o.TABLE_COLLATION = null -- String
        o.CHECKSUM = null -- NullableInt64
        o.CREATE_OPTIONS = null -- String
        o.TABLE_COMMENT = null -- String
        return o
    end,
    FromBBuffer = function( bb, o )
        local ReadObject = bb.ReadObject
        o.TABLE_CATALOG = ReadObject( bb )
        o.TABLE_SCHEMA = ReadObject( bb )
        o.TABLE_NAME = ReadObject( bb )
        o.TABLE_TYPE = ReadObject( bb )
        o.ENGINE = ReadObject( bb )
        o.VERSION = ReadObject( bb )
        o.ROW_FORMAT = ReadObject( bb )
        o.TABLE_ROWS = ReadObject( bb )
        o.AVG_ROW_LENGTH = ReadObject( bb )
        o.DATA_LENGTH = ReadObject( bb )
        o.MAX_DATA_LENGTH = ReadObject( bb )
        o.INDEX_LENGTH = ReadObject( bb )
        o.DATA_FREE = ReadObject( bb )
        o.AUTO_INCREMENT = ReadObject( bb )
        o.CREATE_TIME = ReadObject( bb )
        o.UPDATE_TIME = ReadObject( bb )
        o.CHECK_TIME = ReadObject( bb )
        o.TABLE_COLLATION = ReadObject( bb )
        o.CHECKSUM = ReadObject( bb )
        o.CREATE_OPTIONS = ReadObject( bb )
        o.TABLE_COMMENT = ReadObject( bb )
    end,
    ToBBuffer = function( bb, o )
        local WriteObject = bb.WriteObject
        WriteObject( bb, o.TABLE_CATALOG )
        WriteObject( bb, o.TABLE_SCHEMA )
        WriteObject( bb, o.TABLE_NAME )
        WriteObject( bb, o.TABLE_TYPE )
        WriteObject( bb, o.ENGINE )
        WriteObject( bb, o.VERSION )
        WriteObject( bb, o.ROW_FORMAT )
        WriteObject( bb, o.TABLE_ROWS )
        WriteObject( bb, o.AVG_ROW_LENGTH )
        WriteObject( bb, o.DATA_LENGTH )
        WriteObject( bb, o.MAX_DATA_LENGTH )
        WriteObject( bb, o.INDEX_LENGTH )
        WriteObject( bb, o.DATA_FREE )
        WriteObject( bb, o.AUTO_INCREMENT )
        WriteObject( bb, o.CREATE_TIME )
        WriteObject( bb, o.UPDATE_TIME )
        WriteObject( bb, o.CHECK_TIME )
        WriteObject( bb, o.TABLE_COLLATION )
        WriteObject( bb, o.CHECKSUM )
        WriteObject( bb, o.CREATE_OPTIONS )
        WriteObject( bb, o.TABLE_COMMENT )
    end
}
BBuffer.Register( MYSQLGEN_Tables_TABLES )
List_MYSQLGEN_DbColumn_ = {
    typeName = "List_MYSQLGEN_DbColumn_",
    typeId = 8,
    Create = function()
        local o = {}
        o.__proto = List_MYSQLGEN_DbColumn_
        o.__index = o
        o.__newindex = o
		o.__isReleased = false
		o.Release = function()
			o.__isReleased = true
		end

        return o
    end,
    FromBBuffer = function( bb, o )
		local len = bb:ReadUInt32()
        local f = BBuffer.ReadObject
		for i = 1, len do
			o[ i ] = f( bb )
		end
    end,
    ToBBuffer = function( bb, o )
        local len = #o
		bb:WriteUInt32( len )
        local f = BBuffer.WriteObject
        for i = 1, len do
			f( bb, o[ i ] )
		end
    end
}
BBuffer.Register( List_MYSQLGEN_DbColumn_ )
MYSQLGEN_DbColumn = {
    typeName = "MYSQLGEN_DbColumn",
    typeId = 6,
    Create = function()
        local o = {}
        o.__proto = MYSQLGEN_DbColumn
        o.__index = o
        o.__newindex = o
		o.__isReleased = false
		o.Release = function()
			o.__isReleased = true
		end


        o.parent = null -- MYSQLGEN_DbTable
        setmetatable( o, MYSQLGEN_Tables_COLUMNS.Create() )
        return o
    end,
    FromBBuffer = function( bb, o )
        local p = getmetatable( o )
        p.__proto.FromBBuffer( bb, p )
        o.parent = bb:ReadObject()
    end,
    ToBBuffer = function( bb, o )
        local p = getmetatable( o )
        p.__proto.ToBBuffer( bb, p )
        bb:WriteObject( o.parent )
    end
}
BBuffer.Register( MYSQLGEN_DbColumn )
MYSQLGEN_Tables_COLUMNS = {
    typeName = "MYSQLGEN_Tables_COLUMNS",
    typeId = 7,
    Create = function()
        local o = {}
        o.__proto = MYSQLGEN_Tables_COLUMNS
        o.__index = o
        o.__newindex = o
		o.__isReleased = false
		o.Release = function()
			o.__isReleased = true
		end


        o.TABLE_CATALOG = null -- String
        o.TABLE_SCHEMA = null -- String
        o.TABLE_NAME = null -- String
        o.COLUMN_NAME = null -- String
        o.ORDINAL_POSITION = 0 -- Int64
        o.COLUMN_DEFAULT = null -- String
        o.IS_NULLABLE = null -- String
        o.DATA_TYPE = null -- String
        o.CHARACTER_MAXIMUM_LENGTH = null -- NullableInt64
        o.CHARACTER_OCTET_LENGTH = null -- NullableInt64
        o.NUMERIC_PRECISION = null -- NullableInt64
        o.NUMERIC_SCALE = null -- NullableInt64
        o.DATETIME_PRECISION = null -- NullableInt64
        o.CHARACTER_SET_NAME = null -- String
        o.COLLATION_NAME = null -- String
        o.COLUMN_TYPE = null -- String
        o.COLUMN_KEY = null -- String
        o.EXTRA = null -- String
        o.PRIVILEGES = null -- String
        o.COLUMN_COMMENT = null -- String
        o.GENERATION_EXPRESSION = null -- String
        return o
    end,
    FromBBuffer = function( bb, o )
        local ReadObject = bb.ReadObject
        o.TABLE_CATALOG = ReadObject( bb )
        o.TABLE_SCHEMA = ReadObject( bb )
        o.TABLE_NAME = ReadObject( bb )
        o.COLUMN_NAME = ReadObject( bb )
        o.ORDINAL_POSITION = bb:ReadInt64()
        o.COLUMN_DEFAULT = ReadObject( bb )
        o.IS_NULLABLE = ReadObject( bb )
        o.DATA_TYPE = ReadObject( bb )
        o.CHARACTER_MAXIMUM_LENGTH = ReadObject( bb )
        o.CHARACTER_OCTET_LENGTH = ReadObject( bb )
        o.NUMERIC_PRECISION = ReadObject( bb )
        o.NUMERIC_SCALE = ReadObject( bb )
        o.DATETIME_PRECISION = ReadObject( bb )
        o.CHARACTER_SET_NAME = ReadObject( bb )
        o.COLLATION_NAME = ReadObject( bb )
        o.COLUMN_TYPE = ReadObject( bb )
        o.COLUMN_KEY = ReadObject( bb )
        o.EXTRA = ReadObject( bb )
        o.PRIVILEGES = ReadObject( bb )
        o.COLUMN_COMMENT = ReadObject( bb )
        o.GENERATION_EXPRESSION = ReadObject( bb )
    end,
    ToBBuffer = function( bb, o )
        local WriteObject = bb.WriteObject
        WriteObject( bb, o.TABLE_CATALOG )
        WriteObject( bb, o.TABLE_SCHEMA )
        WriteObject( bb, o.TABLE_NAME )
        WriteObject( bb, o.COLUMN_NAME )
        bb:WriteInt64( o.ORDINAL_POSITION )
        WriteObject( bb, o.COLUMN_DEFAULT )
        WriteObject( bb, o.IS_NULLABLE )
        WriteObject( bb, o.DATA_TYPE )
        WriteObject( bb, o.CHARACTER_MAXIMUM_LENGTH )
        WriteObject( bb, o.CHARACTER_OCTET_LENGTH )
        WriteObject( bb, o.NUMERIC_PRECISION )
        WriteObject( bb, o.NUMERIC_SCALE )
        WriteObject( bb, o.DATETIME_PRECISION )
        WriteObject( bb, o.CHARACTER_SET_NAME )
        WriteObject( bb, o.COLLATION_NAME )
        WriteObject( bb, o.COLUMN_TYPE )
        WriteObject( bb, o.COLUMN_KEY )
        WriteObject( bb, o.EXTRA )
        WriteObject( bb, o.PRIVILEGES )
        WriteObject( bb, o.COLUMN_COMMENT )
        WriteObject( bb, o.GENERATION_EXPRESSION )
    end
}
BBuffer.Register( MYSQLGEN_Tables_COLUMNS )
MYSQLGEN_Tables_show_create_table = {
    typeName = "MYSQLGEN_Tables_show_create_table",
    typeId = 9,
    Create = function()
        local o = {}
        o.__proto = MYSQLGEN_Tables_show_create_table
        o.__index = o
        o.__newindex = o
		o.__isReleased = false
		o.Release = function()
			o.__isReleased = true
		end


        o.Table = null -- String
        o.CreateTable = null -- String
        return o
    end,
    FromBBuffer = function( bb, o )
        local ReadObject = bb.ReadObject
        o.Table = ReadObject( bb )
        o.CreateTable = ReadObject( bb )
    end,
    ToBBuffer = function( bb, o )
        local WriteObject = bb.WriteObject
        WriteObject( bb, o.Table )
        WriteObject( bb, o.CreateTable )
    end
}
BBuffer.Register( MYSQLGEN_Tables_show_create_table )