#pragma warning disable 0169, 0414
using TemplateLibrary;

// 这个命名空间下的命名和库结构完全一致
namespace Tables
{
    /*
    CREATE TABLE `TABLES` (
    `TABLE_CATALOG` VARCHAR(512) NOT NULL DEFAULT '',
    `TABLE_SCHEMA` VARCHAR(64) NOT NULL DEFAULT '',
    `TABLE_NAME` VARCHAR(64) NOT NULL DEFAULT '',
    `TABLE_TYPE` VARCHAR(64) NOT NULL DEFAULT '',
    `ENGINE` VARCHAR(64) NULL DEFAULT NULL,
    `VERSION` BIGINT(21) UNSIGNED NULL DEFAULT NULL,
    `ROW_FORMAT` VARCHAR(10) NULL DEFAULT NULL,
    `TABLE_ROWS` BIGINT(21) UNSIGNED NULL DEFAULT NULL,
    `AVG_ROW_LENGTH` BIGINT(21) UNSIGNED NULL DEFAULT NULL,
    `DATA_LENGTH` BIGINT(21) UNSIGNED NULL DEFAULT NULL,
    `MAX_DATA_LENGTH` BIGINT(21) UNSIGNED NULL DEFAULT NULL,
    `INDEX_LENGTH` BIGINT(21) UNSIGNED NULL DEFAULT NULL,
    `DATA_FREE` BIGINT(21) UNSIGNED NULL DEFAULT NULL,
    `AUTO_INCREMENT` BIGINT(21) UNSIGNED NULL DEFAULT NULL,
    `CREATE_TIME` DATETIME NULL DEFAULT NULL,
    `UPDATE_TIME` DATETIME NULL DEFAULT NULL,
    `CHECK_TIME` DATETIME NULL DEFAULT NULL,
    `TABLE_COLLATION` VARCHAR(32) NULL DEFAULT NULL,
    `CHECKSUM` BIGINT(21) UNSIGNED NULL DEFAULT NULL,
    `CREATE_OPTIONS` VARCHAR(255) NULL DEFAULT NULL,
    `TABLE_COMMENT` VARCHAR(2048) NOT NULL DEFAULT ''
    )
    COLLATE='utf8_general_ci'
    ENGINE=MEMORY;
     */
    class TABLES
    {
        [Column] string TABLE_CATALOG;
        [Column] string TABLE_SCHEMA;
        [Column] string TABLE_NAME;
        [Column] string TABLE_TYPE;
        [Column] string ENGINE;
        [Column] long? VERSION;
        [Column] string ROW_FORMAT;
        [Column] long? TABLE_ROWS;
        [Column] long? AVG_ROW_LENGTH;
        [Column] long? DATA_LENGTH;
        [Column] long? MAX_DATA_LENGTH;
        [Column] long? INDEX_LENGTH;
        [Column] long? DATA_FREE;
        [Column] long? AUTO_INCREMENT;
        [Column] string CREATE_TIME;
        [Column] string UPDATE_TIME;
        [Column] string CHECK_TIME;
        [Column] string TABLE_COLLATION;
        [Column] long? CHECKSUM;
        [Column] string CREATE_OPTIONS;
        [Column] string TABLE_COMMENT;
    }


    /*
    CREATE TABLE `COLUMNS` (
    `TABLE_CATALOG` VARCHAR(512) NOT NULL DEFAULT '',
    `TABLE_SCHEMA` VARCHAR(64) NOT NULL DEFAULT '',
    `TABLE_NAME` VARCHAR(64) NOT NULL DEFAULT '',
    `COLUMN_NAME` VARCHAR(64) NOT NULL DEFAULT '',
    `ORDINAL_POSITION` BIGINT(21) UNSIGNED NOT NULL DEFAULT '0',
    `COLUMN_DEFAULT` LONGTEXT NULL,
    `IS_NULLABLE` VARCHAR(3) NOT NULL DEFAULT '',
    `DATA_TYPE` VARCHAR(64) NOT NULL DEFAULT '',
    `CHARACTER_MAXIMUM_LENGTH` BIGINT(21) UNSIGNED NULL DEFAULT NULL,
    `CHARACTER_OCTET_LENGTH` BIGINT(21) UNSIGNED NULL DEFAULT NULL,
    `NUMERIC_PRECISION` BIGINT(21) UNSIGNED NULL DEFAULT NULL,
    `NUMERIC_SCALE` BIGINT(21) UNSIGNED NULL DEFAULT NULL,
    `DATETIME_PRECISION` BIGINT(21) UNSIGNED NULL DEFAULT NULL,
    `CHARACTER_SET_NAME` VARCHAR(32) NULL DEFAULT NULL,
    `COLLATION_NAME` VARCHAR(32) NULL DEFAULT NULL,
    `COLUMN_TYPE` LONGTEXT NOT NULL,
    `COLUMN_KEY` VARCHAR(3) NOT NULL DEFAULT '',
    `EXTRA` VARCHAR(30) NOT NULL DEFAULT '',
    `PRIVILEGES` VARCHAR(80) NOT NULL DEFAULT '',
    `COLUMN_COMMENT` VARCHAR(1024) NOT NULL DEFAULT '',
    `GENERATION_EXPRESSION` LONGTEXT NOT NULL
    )
    COLLATE='utf8_general_ci'
    ENGINE=InnoDB;
     */
    class COLUMNS
    {
        [Column] string TABLE_CATALOG;
        [Column] string TABLE_SCHEMA;
        [Column] string TABLE_NAME;
        [Column] string COLUMN_NAME;
        [Column] long ORDINAL_POSITION;
        [Column] string COLUMN_DEFAULT;
        [Column] string IS_NULLABLE;
        [Column] string DATA_TYPE;
        [Column] long? CHARACTER_MAXIMUM_LENGTH;
        [Column] long? CHARACTER_OCTET_LENGTH;
        [Column] long? NUMERIC_PRECISION;
        [Column] long? NUMERIC_SCALE;
        [Column] long? DATETIME_PRECISION;
        [Column] string CHARACTER_SET_NAME;
        [Column] string COLLATION_NAME;
        [Column] string COLUMN_TYPE;
        [Column] string COLUMN_KEY;
        [Column] string EXTRA;
        [Column] string PRIVILEGES;
        [Column] string COLUMN_COMMENT;
        [Column] string GENERATION_EXPRESSION;
    }

    // 这个表名来自 show create table 指令串连
    class show_create_table
    {
        [Column] string Table;
        [Column] string CreateTable;
    }
}

class DbTable : Tables.TABLES
{
    List<DbColumn> childs;
    string createScript;
}

class DbColumn : Tables.COLUMNS
{
    DbTable parent;
}

[MySql]
partial interface MySqlFuncs
{
    [Desc("查出库中有哪些表")]
    [Sql(@"
select * from information_schema.TABLES
 where TABLE_SCHEMA = {0};
")]
    List<Tables.TABLES> GetTables(string dbDatabase);


    [Desc("查出表的字段列表")]
    [Sql(@"
select * from information_schema.COLUMNS
 where TABLE_SCHEMA = {0}
   and TABLE_NAME = {1};
")]
    List<Tables.COLUMNS> GetColumns(string dbDatabase, string tableName);


    [Desc("查出表建表脚本")]
    [Sql(@"
SHOW CREATE TABLE `{0}`.`{1}`;
")]
    Tables.show_create_table GetCreateScript([Literal]string dbDatabase, [Literal]string tableName);
}
