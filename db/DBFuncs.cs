#pragma warning disable 0169, 0414
using TemplateLibrary;

[MySql]
partial interface MySqlFuncs
{
    [Desc(@"查 account 表所有数据")]
    [Sql(@"
select `id`, `username`, `nickname`, `avatar_id`, `phone`, `password`, `money`, `money_safe`, `total_recharge`, `total_consume`, `total_withdraw`, `create_time`, `enabled`, `enter_lobby_time`, `log_game_id`
  from `account`")]
    List<Tables.account> account__SelectAll();

    [Desc(@"往 account 表插入单条数据")]
    [Sql(@"
insert into `account` (`id`, `username`, `nickname`, `avatar_id`, `phone`, `password`, `money`, `money_safe`, `total_recharge`, `total_consume`, `total_withdraw`, `create_time`, `enabled`, `enter_lobby_time`, `log_game_id`)
values ({0}, {1}, {2}, {3}, {4}, {5}, {6}, {7}, {8}, {9}, {10}, {11}, {12}, {13}, {14})")]
    void account__Insert(int id, string username, string nickname, int avatar_id, string phone, string password, double money, double money_safe, double total_recharge, double total_consume, double total_withdraw, long create_time, int enabled, long? enter_lobby_time, int? log_game_id);

    [Desc(@"往 account 表插入单条数据对象")]
    [Sql(@"
insert into `account` (`id`, `username`, `nickname`, `avatar_id`, `phone`, `password`, `money`, `money_safe`, `total_recharge`, `total_consume`, `total_withdraw`, `create_time`, `enabled`, `enter_lobby_time`, `log_game_id`)
values {0}")]
    void account__Insert([SkipReadOnly] Tables.account row);

    [Desc(@"往 account 表插入多条数据对象")]
    [Sql(@"
insert into `account` (`id`, `username`, `nickname`, `avatar_id`, `phone`, `password`, `money`, `money_safe`, `total_recharge`, `total_consume`, `total_withdraw`, `create_time`, `enabled`, `enter_lobby_time`, `log_game_id`)
values {0}")]
    void account__Insert([SkipReadOnly] List<Tables.account> rows);

    [Desc(@"根据主键查 account 表指定数据行")]
    [Sql(@"
select `id`, `username`, `nickname`, `avatar_id`, `phone`, `password`, `money`, `money_safe`, `total_recharge`, `total_consume`, `total_withdraw`, `create_time`, `enabled`, `enter_lobby_time`, `log_game_id`
  from `account`
 where `id` = 0")]
    Tables.account account__SelectByPrimaryKey(int id);

    [Desc(@"查 account_copy 表所有数据")]
    [Sql(@"
select `id`, `username`, `nickname`, `avatar_id`, `phone`, `password`, `money`, `money_safe`, `total_recharge`, `total_consume`, `total_withdraw`, `create_time`, `enabled`, `enter_lobby_time`, `log_game_id`
  from `account_copy`")]
    List<Tables.account_copy> account_copy__SelectAll();

    [Desc(@"往 account_copy 表插入单条数据")]
    [Sql(@"
insert into `account_copy` (`id`, `username`, `nickname`, `avatar_id`, `phone`, `password`, `money`, `money_safe`, `total_recharge`, `total_consume`, `total_withdraw`, `create_time`, `enabled`, `enter_lobby_time`, `log_game_id`)
values ({0}, {1}, {2}, {3}, {4}, {5}, {6}, {7}, {8}, {9}, {10}, {11}, {12}, {13}, {14})")]
    void account_copy__Insert(int id, string username, string nickname, int avatar_id, string phone, string password, double money, double money_safe, double total_recharge, double total_consume, double total_withdraw, long create_time, int enabled, long? enter_lobby_time, int? log_game_id);

    [Desc(@"往 account_copy 表插入单条数据对象")]
    [Sql(@"
insert into `account_copy` (`id`, `username`, `nickname`, `avatar_id`, `phone`, `password`, `money`, `money_safe`, `total_recharge`, `total_consume`, `total_withdraw`, `create_time`, `enabled`, `enter_lobby_time`, `log_game_id`)
values {0}")]
    void account_copy__Insert([SkipReadOnly] Tables.account_copy row);

    [Desc(@"往 account_copy 表插入多条数据对象")]
    [Sql(@"
insert into `account_copy` (`id`, `username`, `nickname`, `avatar_id`, `phone`, `password`, `money`, `money_safe`, `total_recharge`, `total_consume`, `total_withdraw`, `create_time`, `enabled`, `enter_lobby_time`, `log_game_id`)
values {0}")]
    void account_copy__Insert([SkipReadOnly] List<Tables.account_copy> rows);

    [Desc(@"根据主键查 account_copy 表指定数据行")]
    [Sql(@"
select `id`, `username`, `nickname`, `avatar_id`, `phone`, `password`, `money`, `money_safe`, `total_recharge`, `total_consume`, `total_withdraw`, `create_time`, `enabled`, `enter_lobby_time`, `log_game_id`
  from `account_copy`
 where `id` = 0")]
    Tables.account_copy account_copy__SelectByPrimaryKey(int id);

    [Desc(@"查 account_copy1 表所有数据")]
    [Sql(@"
select `id`, `username`, `nickname`, `avatar_id`, `phone`, `password`, `money`, `money_safe`, `total_recharge`, `total_consume`, `total_withdraw`, `create_time`, `enabled`, `enter_lobby_time`, `log_game_id`
  from `account_copy1`")]
    List<Tables.account_copy1> account_copy1__SelectAll();

    [Desc(@"往 account_copy1 表插入单条数据")]
    [Sql(@"
insert into `account_copy1` (`id`, `username`, `nickname`, `avatar_id`, `phone`, `password`, `money`, `money_safe`, `total_recharge`, `total_consume`, `total_withdraw`, `create_time`, `enabled`, `enter_lobby_time`, `log_game_id`)
values ({0}, {1}, {2}, {3}, {4}, {5}, {6}, {7}, {8}, {9}, {10}, {11}, {12}, {13}, {14})")]
    void account_copy1__Insert(int id, string username, string nickname, int avatar_id, string phone, string password, double money, double money_safe, double total_recharge, double total_consume, double total_withdraw, long create_time, int enabled, long? enter_lobby_time, int? log_game_id);

    [Desc(@"往 account_copy1 表插入单条数据对象")]
    [Sql(@"
insert into `account_copy1` (`id`, `username`, `nickname`, `avatar_id`, `phone`, `password`, `money`, `money_safe`, `total_recharge`, `total_consume`, `total_withdraw`, `create_time`, `enabled`, `enter_lobby_time`, `log_game_id`)
values {0}")]
    void account_copy1__Insert([SkipReadOnly] Tables.account_copy1 row);

    [Desc(@"往 account_copy1 表插入多条数据对象")]
    [Sql(@"
insert into `account_copy1` (`id`, `username`, `nickname`, `avatar_id`, `phone`, `password`, `money`, `money_safe`, `total_recharge`, `total_consume`, `total_withdraw`, `create_time`, `enabled`, `enter_lobby_time`, `log_game_id`)
values {0}")]
    void account_copy1__Insert([SkipReadOnly] List<Tables.account_copy1> rows);

    [Desc(@"根据主键查 account_copy1 表指定数据行")]
    [Sql(@"
select `id`, `username`, `nickname`, `avatar_id`, `phone`, `password`, `money`, `money_safe`, `total_recharge`, `total_consume`, `total_withdraw`, `create_time`, `enabled`, `enter_lobby_time`, `log_game_id`
  from `account_copy1`
 where `id` = 0")]
    Tables.account_copy1 account_copy1__SelectByPrimaryKey(int id);

    [Desc(@"查 chat 表所有数据")]
    [Sql(@"
select `id`, `content`, `account_id`, `create_time`
  from `chat`")]
    List<Tables.chat> chat__SelectAll();

    [Desc(@"往 chat 表插入单条数据")]
    [Sql(@"
insert into `chat` (`content`, `account_id`, `create_time`)
values ({0}, {1}, {2})")]
    void chat__Insert(string content, int account_id, long create_time);

    [Desc(@"往 chat 表插入单条数据对象")]
    [Sql(@"
insert into `chat` (`content`, `account_id`, `create_time`)
values {0}")]
    void chat__Insert([SkipReadOnly] Tables.chat row);

    [Desc(@"往 chat 表插入多条数据对象")]
    [Sql(@"
insert into `chat` (`content`, `account_id`, `create_time`)
values {0}")]
    void chat__Insert([SkipReadOnly] List<Tables.chat> rows);

    [Desc(@"根据主键查 chat 表指定数据行")]
    [Sql(@"
select `content`, `account_id`, `create_time`
  from `chat`
 where `id` = 0")]
    Tables.chat chat__SelectByPrimaryKey(int id);

    [Desc(@"查 config 表所有数据")]
    [Sql(@"
select `key`, `datatype_name`, `title`, `desc`, `create_time`
  from `config`")]
    List<Tables.config> config__SelectAll();

    [Desc(@"往 config 表插入单条数据")]
    [Sql(@"
insert into `config` (`key`, `datatype_name`, `title`, `desc`, `create_time`)
values ({0}, {1}, {2}, {3}, {4})")]
    void config__Insert(string key, string datatype_name, string title, string desc, long create_time);

    [Desc(@"往 config 表插入单条数据对象")]
    [Sql(@"
insert into `config` (`key`, `datatype_name`, `title`, `desc`, `create_time`)
values {0}")]
    void config__Insert([SkipReadOnly] Tables.config row);

    [Desc(@"往 config 表插入多条数据对象")]
    [Sql(@"
insert into `config` (`key`, `datatype_name`, `title`, `desc`, `create_time`)
values {0}")]
    void config__Insert([SkipReadOnly] List<Tables.config> rows);

    [Desc(@"根据主键查 config 表指定数据行")]
    [Sql(@"
select `key`, `datatype_name`, `title`, `desc`, `create_time`
  from `config`
 where `key` = 0")]
    Tables.config config__SelectByPrimaryKey(string key);

    [Desc(@"查 config_value 表所有数据")]
    [Sql(@"
select `key`, `value_bigint`, `value_double`, `value_varchar`, `create_time`
  from `config_value`")]
    List<Tables.config_value> config_value__SelectAll();

    [Desc(@"往 config_value 表插入单条数据")]
    [Sql(@"
insert into `config_value` (`key`, `value_bigint`, `value_double`, `value_varchar`, `create_time`)
values ({0}, {1}, {2}, {3}, {4})")]
    void config_value__Insert(string key, long? value_bigint, double? value_double, string value_varchar, long create_time);

    [Desc(@"往 config_value 表插入单条数据对象")]
    [Sql(@"
insert into `config_value` (`key`, `value_bigint`, `value_double`, `value_varchar`, `create_time`)
values {0}")]
    void config_value__Insert([SkipReadOnly] Tables.config_value row);

    [Desc(@"往 config_value 表插入多条数据对象")]
    [Sql(@"
insert into `config_value` (`key`, `value_bigint`, `value_double`, `value_varchar`, `create_time`)
values {0}")]
    void config_value__Insert([SkipReadOnly] List<Tables.config_value> rows);

    [Desc(@"根据主键查 config_value 表指定数据行")]
    [Sql(@"
select `key`, `value_bigint`, `value_double`, `value_varchar`, `create_time`
  from `config_value`
 where `key` = 0 and `create_time` = 1")]
    Tables.config_value config_value__SelectByPrimaryKey(string key, long create_time);

    [Desc(@"查 datatype 表所有数据")]
    [Sql(@"
select `name`
  from `datatype`")]
    List<Tables.datatype> datatype__SelectAll();

    [Desc(@"往 datatype 表插入单条数据")]
    [Sql(@"
insert into `datatype` (`name`)
values ({0})")]
    void datatype__Insert(string name);

    [Desc(@"往 datatype 表插入单条数据对象")]
    [Sql(@"
insert into `datatype` (`name`)
values {0}")]
    void datatype__Insert([SkipReadOnly] Tables.datatype row);

    [Desc(@"往 datatype 表插入多条数据对象")]
    [Sql(@"
insert into `datatype` (`name`)
values {0}")]
    void datatype__Insert([SkipReadOnly] List<Tables.datatype> rows);

    [Desc(@"根据主键查 datatype 表指定数据行")]
    [Sql(@"
select `name`
  from `datatype`
 where `name` = 0")]
    Tables.datatype datatype__SelectByPrimaryKey(string name);

    [Desc(@"查 game 表所有数据")]
    [Sql(@"
select `id`, `name`, `type_name`, `enabled`
  from `game`")]
    List<Tables.game> game__SelectAll();

    [Desc(@"往 game 表插入单条数据")]
    [Sql(@"
insert into `game` (`id`, `name`, `type_name`, `enabled`)
values ({0}, {1}, {2}, {3})")]
    void game__Insert(int id, string name, string type_name, int enabled);

    [Desc(@"往 game 表插入单条数据对象")]
    [Sql(@"
insert into `game` (`id`, `name`, `type_name`, `enabled`)
values {0}")]
    void game__Insert([SkipReadOnly] Tables.game row);

    [Desc(@"往 game 表插入多条数据对象")]
    [Sql(@"
insert into `game` (`id`, `name`, `type_name`, `enabled`)
values {0}")]
    void game__Insert([SkipReadOnly] List<Tables.game> rows);

    [Desc(@"根据主键查 game 表指定数据行")]
    [Sql(@"
select `id`, `name`, `type_name`, `enabled`
  from `game`
 where `id` = 0")]
    Tables.game game__SelectByPrimaryKey(int id);

    [Desc(@"查 game_catchfish 表所有数据")]
    [Sql(@"
select `id`, `exchange_coin_ratio`
  from `game_catchfish`")]
    List<Tables.game_catchfish> game_catchfish__SelectAll();

    [Desc(@"往 game_catchfish 表插入单条数据")]
    [Sql(@"
insert into `game_catchfish` (`id`, `exchange_coin_ratio`)
values ({0}, {1})")]
    void game_catchfish__Insert(int id, int exchange_coin_ratio);

    [Desc(@"往 game_catchfish 表插入单条数据对象")]
    [Sql(@"
insert into `game_catchfish` (`id`, `exchange_coin_ratio`)
values {0}")]
    void game_catchfish__Insert([SkipReadOnly] Tables.game_catchfish row);

    [Desc(@"往 game_catchfish 表插入多条数据对象")]
    [Sql(@"
insert into `game_catchfish` (`id`, `exchange_coin_ratio`)
values {0}")]
    void game_catchfish__Insert([SkipReadOnly] List<Tables.game_catchfish> rows);

    [Desc(@"根据主键查 game_catchfish 表指定数据行")]
    [Sql(@"
select `id`, `exchange_coin_ratio`
  from `game_catchfish`
 where `id` = 0")]
    Tables.game_catchfish game_catchfish__SelectByPrimaryKey(int id);

    [Desc(@"查 game_catchfish_account 表所有数据")]
    [Sql(@"
select `account_id`, `game_id`, `level_id`, `last_bet`, `last_cannon_id`
  from `game_catchfish_account`")]
    List<Tables.game_catchfish_account> game_catchfish_account__SelectAll();

    [Desc(@"往 game_catchfish_account 表插入单条数据")]
    [Sql(@"
insert into `game_catchfish_account` (`account_id`, `game_id`, `level_id`, `last_bet`, `last_cannon_id`)
values ({0}, {1}, {2}, {3}, {4})")]
    void game_catchfish_account__Insert(int account_id, int game_id, int level_id, long last_bet, int last_cannon_id);

    [Desc(@"往 game_catchfish_account 表插入单条数据对象")]
    [Sql(@"
insert into `game_catchfish_account` (`account_id`, `game_id`, `level_id`, `last_bet`, `last_cannon_id`)
values {0}")]
    void game_catchfish_account__Insert([SkipReadOnly] Tables.game_catchfish_account row);

    [Desc(@"往 game_catchfish_account 表插入多条数据对象")]
    [Sql(@"
insert into `game_catchfish_account` (`account_id`, `game_id`, `level_id`, `last_bet`, `last_cannon_id`)
values {0}")]
    void game_catchfish_account__Insert([SkipReadOnly] List<Tables.game_catchfish_account> rows);

    [Desc(@"根据主键查 game_catchfish_account 表指定数据行")]
    [Sql(@"
select `account_id`, `game_id`, `level_id`, `last_bet`, `last_cannon_id`
  from `game_catchfish_account`
 where `account_id` = 0 and `game_id` = 1 and `level_id` = 2")]
    Tables.game_catchfish_account game_catchfish_account__SelectByPrimaryKey(int account_id, int game_id, int level_id);

    [Desc(@"查 game_catchfish_cannon 表所有数据")]
    [Sql(@"
select `id`, `vip_id`
  from `game_catchfish_cannon`")]
    List<Tables.game_catchfish_cannon> game_catchfish_cannon__SelectAll();

    [Desc(@"往 game_catchfish_cannon 表插入单条数据")]
    [Sql(@"
insert into `game_catchfish_cannon` (`id`, `vip_id`)
values ({0}, {1})")]
    void game_catchfish_cannon__Insert(int id, int vip_id);

    [Desc(@"往 game_catchfish_cannon 表插入单条数据对象")]
    [Sql(@"
insert into `game_catchfish_cannon` (`id`, `vip_id`)
values {0}")]
    void game_catchfish_cannon__Insert([SkipReadOnly] Tables.game_catchfish_cannon row);

    [Desc(@"往 game_catchfish_cannon 表插入多条数据对象")]
    [Sql(@"
insert into `game_catchfish_cannon` (`id`, `vip_id`)
values {0}")]
    void game_catchfish_cannon__Insert([SkipReadOnly] List<Tables.game_catchfish_cannon> rows);

    [Desc(@"根据主键查 game_catchfish_cannon 表指定数据行")]
    [Sql(@"
select `id`, `vip_id`
  from `game_catchfish_cannon`
 where `id` = 0")]
    Tables.game_catchfish_cannon game_catchfish_cannon__SelectByPrimaryKey(int id);

    [Desc(@"查 game_catchfish_fish 表所有数据")]
    [Sql(@"
select `id`, `name`, `minCoin`, `maxCoin`
  from `game_catchfish_fish`")]
    List<Tables.game_catchfish_fish> game_catchfish_fish__SelectAll();

    [Desc(@"往 game_catchfish_fish 表插入单条数据")]
    [Sql(@"
insert into `game_catchfish_fish` (`id`, `name`, `minCoin`, `maxCoin`)
values ({0}, {1}, {2}, {3})")]
    void game_catchfish_fish__Insert(int id, string name, int minCoin, int maxCoin);

    [Desc(@"往 game_catchfish_fish 表插入单条数据对象")]
    [Sql(@"
insert into `game_catchfish_fish` (`id`, `name`, `minCoin`, `maxCoin`)
values {0}")]
    void game_catchfish_fish__Insert([SkipReadOnly] Tables.game_catchfish_fish row);

    [Desc(@"往 game_catchfish_fish 表插入多条数据对象")]
    [Sql(@"
insert into `game_catchfish_fish` (`id`, `name`, `minCoin`, `maxCoin`)
values {0}")]
    void game_catchfish_fish__Insert([SkipReadOnly] List<Tables.game_catchfish_fish> rows);

    [Desc(@"根据主键查 game_catchfish_fish 表指定数据行")]
    [Sql(@"
select `id`, `name`, `minCoin`, `maxCoin`
  from `game_catchfish_fish`
 where `id` = 0")]
    Tables.game_catchfish_fish game_catchfish_fish__SelectByPrimaryKey(int id);

    [Desc(@"查 game_catchfish_level 表所有数据")]
    [Sql(@"
select `game_id`, `level_id`, `min_bet`, `max_bet`, `min_money`, `enabled`
  from `game_catchfish_level`")]
    List<Tables.game_catchfish_level> game_catchfish_level__SelectAll();

    [Desc(@"往 game_catchfish_level 表插入单条数据")]
    [Sql(@"
insert into `game_catchfish_level` (`game_id`, `level_id`, `min_bet`, `max_bet`, `min_money`, `enabled`)
values ({0}, {1}, {2}, {3}, {4}, {5})")]
    void game_catchfish_level__Insert(int game_id, int level_id, long min_bet, long max_bet, double min_money, int enabled);

    [Desc(@"往 game_catchfish_level 表插入单条数据对象")]
    [Sql(@"
insert into `game_catchfish_level` (`game_id`, `level_id`, `min_bet`, `max_bet`, `min_money`, `enabled`)
values {0}")]
    void game_catchfish_level__Insert([SkipReadOnly] Tables.game_catchfish_level row);

    [Desc(@"往 game_catchfish_level 表插入多条数据对象")]
    [Sql(@"
insert into `game_catchfish_level` (`game_id`, `level_id`, `min_bet`, `max_bet`, `min_money`, `enabled`)
values {0}")]
    void game_catchfish_level__Insert([SkipReadOnly] List<Tables.game_catchfish_level> rows);

    [Desc(@"根据主键查 game_catchfish_level 表指定数据行")]
    [Sql(@"
select `game_id`, `level_id`, `min_bet`, `max_bet`, `min_money`, `enabled`
  from `game_catchfish_level`
 where `game_id` = 0 and `level_id` = 1")]
    Tables.game_catchfish_level game_catchfish_level__SelectByPrimaryKey(int game_id, int level_id);

    [Desc(@"查 game_catchfish_level_room 表所有数据")]
    [Sql(@"
select `game_id`, `level_id`, `room_id`, `enabled`
  from `game_catchfish_level_room`")]
    List<Tables.game_catchfish_level_room> game_catchfish_level_room__SelectAll();

    [Desc(@"往 game_catchfish_level_room 表插入单条数据")]
    [Sql(@"
insert into `game_catchfish_level_room` (`game_id`, `level_id`, `room_id`, `enabled`)
values ({0}, {1}, {2}, {3})")]
    void game_catchfish_level_room__Insert(int game_id, int level_id, int room_id, int enabled);

    [Desc(@"往 game_catchfish_level_room 表插入单条数据对象")]
    [Sql(@"
insert into `game_catchfish_level_room` (`game_id`, `level_id`, `room_id`, `enabled`)
values {0}")]
    void game_catchfish_level_room__Insert([SkipReadOnly] Tables.game_catchfish_level_room row);

    [Desc(@"往 game_catchfish_level_room 表插入多条数据对象")]
    [Sql(@"
insert into `game_catchfish_level_room` (`game_id`, `level_id`, `room_id`, `enabled`)
values {0}")]
    void game_catchfish_level_room__Insert([SkipReadOnly] List<Tables.game_catchfish_level_room> rows);

    [Desc(@"根据主键查 game_catchfish_level_room 表指定数据行")]
    [Sql(@"
select `game_id`, `level_id`, `room_id`, `enabled`
  from `game_catchfish_level_room`
 where `game_id` = 0 and `level_id` = 1 and `room_id` = 2")]
    Tables.game_catchfish_level_room game_catchfish_level_room__SelectByPrimaryKey(int game_id, int level_id, int room_id);

    [Desc(@"查 log_account_avatar 表所有数据")]
    [Sql(@"
select `id`, `account_id`, `avatar_id`, `create_time`
  from `log_account_avatar`")]
    List<Tables.log_account_avatar> log_account_avatar__SelectAll();

    [Desc(@"往 log_account_avatar 表插入单条数据")]
    [Sql(@"
insert into `log_account_avatar` (`account_id`, `avatar_id`, `create_time`)
values ({0}, {1}, {2})")]
    void log_account_avatar__Insert(int account_id, int avatar_id, long create_time);

    [Desc(@"往 log_account_avatar 表插入单条数据对象")]
    [Sql(@"
insert into `log_account_avatar` (`account_id`, `avatar_id`, `create_time`)
values {0}")]
    void log_account_avatar__Insert([SkipReadOnly] Tables.log_account_avatar row);

    [Desc(@"往 log_account_avatar 表插入多条数据对象")]
    [Sql(@"
insert into `log_account_avatar` (`account_id`, `avatar_id`, `create_time`)
values {0}")]
    void log_account_avatar__Insert([SkipReadOnly] List<Tables.log_account_avatar> rows);

    [Desc(@"根据主键查 log_account_avatar 表指定数据行")]
    [Sql(@"
select `account_id`, `avatar_id`, `create_time`
  from `log_account_avatar`
 where `id` = 0")]
    Tables.log_account_avatar log_account_avatar__SelectByPrimaryKey(int id);

    [Desc(@"查 log_account_enabled 表所有数据")]
    [Sql(@"
select `id`, `account_id`, `enabled`, `memo`, `create_time`
  from `log_account_enabled`")]
    List<Tables.log_account_enabled> log_account_enabled__SelectAll();

    [Desc(@"往 log_account_enabled 表插入单条数据")]
    [Sql(@"
insert into `log_account_enabled` (`account_id`, `enabled`, `memo`, `create_time`)
values ({0}, {1}, {2}, {3})")]
    void log_account_enabled__Insert(int account_id, int enabled, string memo, long create_time);

    [Desc(@"往 log_account_enabled 表插入单条数据对象")]
    [Sql(@"
insert into `log_account_enabled` (`account_id`, `enabled`, `memo`, `create_time`)
values {0}")]
    void log_account_enabled__Insert([SkipReadOnly] Tables.log_account_enabled row);

    [Desc(@"往 log_account_enabled 表插入多条数据对象")]
    [Sql(@"
insert into `log_account_enabled` (`account_id`, `enabled`, `memo`, `create_time`)
values {0}")]
    void log_account_enabled__Insert([SkipReadOnly] List<Tables.log_account_enabled> rows);

    [Desc(@"根据主键查 log_account_enabled 表指定数据行")]
    [Sql(@"
select `account_id`, `enabled`, `memo`, `create_time`
  from `log_account_enabled`
 where `id` = 0")]
    Tables.log_account_enabled log_account_enabled__SelectByPrimaryKey(int id);

    [Desc(@"查 log_account_money_safe 表所有数据")]
    [Sql(@"
select `id`, `account_id`, `value`, `create_time`
  from `log_account_money_safe`")]
    List<Tables.log_account_money_safe> log_account_money_safe__SelectAll();

    [Desc(@"往 log_account_money_safe 表插入单条数据")]
    [Sql(@"
insert into `log_account_money_safe` (`account_id`, `value`, `create_time`)
values ({0}, {1}, {2})")]
    void log_account_money_safe__Insert(int account_id, double value, long create_time);

    [Desc(@"往 log_account_money_safe 表插入单条数据对象")]
    [Sql(@"
insert into `log_account_money_safe` (`account_id`, `value`, `create_time`)
values {0}")]
    void log_account_money_safe__Insert([SkipReadOnly] Tables.log_account_money_safe row);

    [Desc(@"往 log_account_money_safe 表插入多条数据对象")]
    [Sql(@"
insert into `log_account_money_safe` (`account_id`, `value`, `create_time`)
values {0}")]
    void log_account_money_safe__Insert([SkipReadOnly] List<Tables.log_account_money_safe> rows);

    [Desc(@"根据主键查 log_account_money_safe 表指定数据行")]
    [Sql(@"
select `account_id`, `value`, `create_time`
  from `log_account_money_safe`
 where `id` = 0")]
    Tables.log_account_money_safe log_account_money_safe__SelectByPrimaryKey(int id);

    [Desc(@"查 log_account_nickname 表所有数据")]
    [Sql(@"
select `id`, `account_id`, `nickname`, `create_time`
  from `log_account_nickname`")]
    List<Tables.log_account_nickname> log_account_nickname__SelectAll();

    [Desc(@"往 log_account_nickname 表插入单条数据")]
    [Sql(@"
insert into `log_account_nickname` (`account_id`, `nickname`, `create_time`)
values ({0}, {1}, {2})")]
    void log_account_nickname__Insert(int account_id, string nickname, long create_time);

    [Desc(@"往 log_account_nickname 表插入单条数据对象")]
    [Sql(@"
insert into `log_account_nickname` (`account_id`, `nickname`, `create_time`)
values {0}")]
    void log_account_nickname__Insert([SkipReadOnly] Tables.log_account_nickname row);

    [Desc(@"往 log_account_nickname 表插入多条数据对象")]
    [Sql(@"
insert into `log_account_nickname` (`account_id`, `nickname`, `create_time`)
values {0}")]
    void log_account_nickname__Insert([SkipReadOnly] List<Tables.log_account_nickname> rows);

    [Desc(@"根据主键查 log_account_nickname 表指定数据行")]
    [Sql(@"
select `account_id`, `nickname`, `create_time`
  from `log_account_nickname`
 where `id` = 0")]
    Tables.log_account_nickname log_account_nickname__SelectByPrimaryKey(int id);

    [Desc(@"查 log_account_password 表所有数据")]
    [Sql(@"
select `id`, `account_id`, `password`, `content`, `create_time`
  from `log_account_password`")]
    List<Tables.log_account_password> log_account_password__SelectAll();

    [Desc(@"往 log_account_password 表插入单条数据")]
    [Sql(@"
insert into `log_account_password` (`account_id`, `password`, `content`, `create_time`)
values ({0}, {1}, {2}, {3})")]
    void log_account_password__Insert(int account_id, string password, string content, long create_time);

    [Desc(@"往 log_account_password 表插入单条数据对象")]
    [Sql(@"
insert into `log_account_password` (`account_id`, `password`, `content`, `create_time`)
values {0}")]
    void log_account_password__Insert([SkipReadOnly] Tables.log_account_password row);

    [Desc(@"往 log_account_password 表插入多条数据对象")]
    [Sql(@"
insert into `log_account_password` (`account_id`, `password`, `content`, `create_time`)
values {0}")]
    void log_account_password__Insert([SkipReadOnly] List<Tables.log_account_password> rows);

    [Desc(@"根据主键查 log_account_password 表指定数据行")]
    [Sql(@"
select `account_id`, `password`, `content`, `create_time`
  from `log_account_password`
 where `id` = 0")]
    Tables.log_account_password log_account_password__SelectByPrimaryKey(int id);

    [Desc(@"查 log_account_phone 表所有数据")]
    [Sql(@"
select `id`, `account_id`, `phone`, `content`, `create_time`
  from `log_account_phone`")]
    List<Tables.log_account_phone> log_account_phone__SelectAll();

    [Desc(@"往 log_account_phone 表插入单条数据")]
    [Sql(@"
insert into `log_account_phone` (`account_id`, `phone`, `content`, `create_time`)
values ({0}, {1}, {2}, {3})")]
    void log_account_phone__Insert(int account_id, string phone, string content, long create_time);

    [Desc(@"往 log_account_phone 表插入单条数据对象")]
    [Sql(@"
insert into `log_account_phone` (`account_id`, `phone`, `content`, `create_time`)
values {0}")]
    void log_account_phone__Insert([SkipReadOnly] Tables.log_account_phone row);

    [Desc(@"往 log_account_phone 表插入多条数据对象")]
    [Sql(@"
insert into `log_account_phone` (`account_id`, `phone`, `content`, `create_time`)
values {0}")]
    void log_account_phone__Insert([SkipReadOnly] List<Tables.log_account_phone> rows);

    [Desc(@"根据主键查 log_account_phone 表指定数据行")]
    [Sql(@"
select `account_id`, `phone`, `content`, `create_time`
  from `log_account_phone`
 where `id` = 0")]
    Tables.log_account_phone log_account_phone__SelectByPrimaryKey(int id);

    [Desc(@"查 log_account_recharge 表所有数据")]
    [Sql(@"
select `token`, `account_id`, `money`, `memo`, `create_time`
  from `log_account_recharge`")]
    List<Tables.log_account_recharge> log_account_recharge__SelectAll();

    [Desc(@"往 log_account_recharge 表插入单条数据")]
    [Sql(@"
insert into `log_account_recharge` (`token`, `account_id`, `money`, `memo`, `create_time`)
values ({0}, {1}, {2}, {3}, {4})")]
    void log_account_recharge__Insert(string token, int account_id, double money, string memo, long create_time);

    [Desc(@"往 log_account_recharge 表插入单条数据对象")]
    [Sql(@"
insert into `log_account_recharge` (`token`, `account_id`, `money`, `memo`, `create_time`)
values {0}")]
    void log_account_recharge__Insert([SkipReadOnly] Tables.log_account_recharge row);

    [Desc(@"往 log_account_recharge 表插入多条数据对象")]
    [Sql(@"
insert into `log_account_recharge` (`token`, `account_id`, `money`, `memo`, `create_time`)
values {0}")]
    void log_account_recharge__Insert([SkipReadOnly] List<Tables.log_account_recharge> rows);

    [Desc(@"根据主键查 log_account_recharge 表指定数据行")]
    [Sql(@"
select `token`, `account_id`, `money`, `memo`, `create_time`
  from `log_account_recharge`
 where `token` = 0")]
    Tables.log_account_recharge log_account_recharge__SelectByPrimaryKey(string token);

    [Desc(@"查 log_game 表所有数据")]
    [Sql(@"
select `id`, `game_id`
  from `log_game`")]
    List<Tables.log_game> log_game__SelectAll();

    [Desc(@"往 log_game 表插入单条数据")]
    [Sql(@"
insert into `log_game` (`game_id`)
values ({0})")]
    void log_game__Insert(int game_id);

    [Desc(@"往 log_game 表插入单条数据对象")]
    [Sql(@"
insert into `log_game` (`game_id`)
values {0}")]
    void log_game__Insert([SkipReadOnly] Tables.log_game row);

    [Desc(@"往 log_game 表插入多条数据对象")]
    [Sql(@"
insert into `log_game` (`game_id`)
values {0}")]
    void log_game__Insert([SkipReadOnly] List<Tables.log_game> rows);

    [Desc(@"根据主键查 log_game 表指定数据行")]
    [Sql(@"
select `game_id`
  from `log_game`
 where `id` = 0")]
    Tables.log_game log_game__SelectByPrimaryKey(int id);

    [Desc(@"查 log_game_catchfish 表所有数据")]
    [Sql(@"
select `id`, `level_id`, `room_id`, `account_id`, `enter_time`, `enter_money`, `leave_time`, `leave_money`, `consume`
  from `log_game_catchfish`")]
    List<Tables.log_game_catchfish> log_game_catchfish__SelectAll();

    [Desc(@"往 log_game_catchfish 表插入单条数据")]
    [Sql(@"
insert into `log_game_catchfish` (`id`, `level_id`, `room_id`, `account_id`, `enter_time`, `enter_money`, `leave_time`, `leave_money`, `consume`)
values ({0}, {1}, {2}, {3}, {4}, {5}, {6}, {7}, {8})")]
    void log_game_catchfish__Insert(int id, int level_id, int room_id, int account_id, long enter_time, double enter_money, long leave_time, double leave_money, double consume);

    [Desc(@"往 log_game_catchfish 表插入单条数据对象")]
    [Sql(@"
insert into `log_game_catchfish` (`id`, `level_id`, `room_id`, `account_id`, `enter_time`, `enter_money`, `leave_time`, `leave_money`, `consume`)
values {0}")]
    void log_game_catchfish__Insert([SkipReadOnly] Tables.log_game_catchfish row);

    [Desc(@"往 log_game_catchfish 表插入多条数据对象")]
    [Sql(@"
insert into `log_game_catchfish` (`id`, `level_id`, `room_id`, `account_id`, `enter_time`, `enter_money`, `leave_time`, `leave_money`, `consume`)
values {0}")]
    void log_game_catchfish__Insert([SkipReadOnly] List<Tables.log_game_catchfish> rows);

    [Desc(@"根据主键查 log_game_catchfish 表指定数据行")]
    [Sql(@"
select `id`, `level_id`, `room_id`, `account_id`, `enter_time`, `enter_money`, `leave_time`, `leave_money`, `consume`
  from `log_game_catchfish`
 where `id` = 0")]
    Tables.log_game_catchfish log_game_catchfish__SelectByPrimaryKey(int id);

    [Desc(@"查 log_game_catchfish_event 表所有数据")]
    [Sql(@"
select `id`, `log_game_catchfish_id`, `event_type_id`, `frame_number`, `create_time`
  from `log_game_catchfish_event`")]
    List<Tables.log_game_catchfish_event> log_game_catchfish_event__SelectAll();

    [Desc(@"往 log_game_catchfish_event 表插入单条数据")]
    [Sql(@"
insert into `log_game_catchfish_event` (`id`, `log_game_catchfish_id`, `event_type_id`, `frame_number`, `create_time`)
values ({0}, {1}, {2}, {3}, {4})")]
    void log_game_catchfish_event__Insert(int id, int log_game_catchfish_id, int event_type_id, int frame_number, long create_time);

    [Desc(@"往 log_game_catchfish_event 表插入单条数据对象")]
    [Sql(@"
insert into `log_game_catchfish_event` (`id`, `log_game_catchfish_id`, `event_type_id`, `frame_number`, `create_time`)
values {0}")]
    void log_game_catchfish_event__Insert([SkipReadOnly] Tables.log_game_catchfish_event row);

    [Desc(@"往 log_game_catchfish_event 表插入多条数据对象")]
    [Sql(@"
insert into `log_game_catchfish_event` (`id`, `log_game_catchfish_id`, `event_type_id`, `frame_number`, `create_time`)
values {0}")]
    void log_game_catchfish_event__Insert([SkipReadOnly] List<Tables.log_game_catchfish_event> rows);

    [Desc(@"根据主键查 log_game_catchfish_event 表指定数据行")]
    [Sql(@"
select `id`, `log_game_catchfish_id`, `event_type_id`, `frame_number`, `create_time`
  from `log_game_catchfish_event`
 where `id` = 0 and `log_game_catchfish_id` = 1")]
    Tables.log_game_catchfish_event log_game_catchfish_event__SelectByPrimaryKey(int id, int log_game_catchfish_id);

    [Desc(@"查 log_game_catchfish_event_bulletdie 表所有数据")]
    [Sql(@"
select `id`, `log_game_catchfish_id`, `type_id`, `bullet_id`, `coin`, `money`, `fish_type_id`, `fish_id`, `fish_ratio`
  from `log_game_catchfish_event_bulletdie`")]
    List<Tables.log_game_catchfish_event_bulletdie> log_game_catchfish_event_bulletdie__SelectAll();

    [Desc(@"往 log_game_catchfish_event_bulletdie 表插入单条数据")]
    [Sql(@"
insert into `log_game_catchfish_event_bulletdie` (`id`, `log_game_catchfish_id`, `type_id`, `bullet_id`, `coin`, `money`, `fish_type_id`, `fish_id`, `fish_ratio`)
values ({0}, {1}, {2}, {3}, {4}, {5}, {6}, {7}, {8})")]
    void log_game_catchfish_event_bulletdie__Insert(int id, int log_game_catchfish_id, int type_id, int bullet_id, long coin, double money, int fish_type_id, int fish_id, long fish_ratio);

    [Desc(@"往 log_game_catchfish_event_bulletdie 表插入单条数据对象")]
    [Sql(@"
insert into `log_game_catchfish_event_bulletdie` (`id`, `log_game_catchfish_id`, `type_id`, `bullet_id`, `coin`, `money`, `fish_type_id`, `fish_id`, `fish_ratio`)
values {0}")]
    void log_game_catchfish_event_bulletdie__Insert([SkipReadOnly] Tables.log_game_catchfish_event_bulletdie row);

    [Desc(@"往 log_game_catchfish_event_bulletdie 表插入多条数据对象")]
    [Sql(@"
insert into `log_game_catchfish_event_bulletdie` (`id`, `log_game_catchfish_id`, `type_id`, `bullet_id`, `coin`, `money`, `fish_type_id`, `fish_id`, `fish_ratio`)
values {0}")]
    void log_game_catchfish_event_bulletdie__Insert([SkipReadOnly] List<Tables.log_game_catchfish_event_bulletdie> rows);

    [Desc(@"根据主键查 log_game_catchfish_event_bulletdie 表指定数据行")]
    [Sql(@"
select `id`, `log_game_catchfish_id`, `type_id`, `bullet_id`, `coin`, `money`, `fish_type_id`, `fish_id`, `fish_ratio`
  from `log_game_catchfish_event_bulletdie`
 where `id` = 0 and `log_game_catchfish_id` = 1")]
    Tables.log_game_catchfish_event_bulletdie log_game_catchfish_event_bulletdie__SelectByPrimaryKey(int id, int log_game_catchfish_id);

    [Desc(@"查 log_game_catchfish_event_bulletmiss 表所有数据")]
    [Sql(@"
select `id`, `log_game_catchfish_id`, `type_id`, `bullet_id`, `coin`, `money`
  from `log_game_catchfish_event_bulletmiss`")]
    List<Tables.log_game_catchfish_event_bulletmiss> log_game_catchfish_event_bulletmiss__SelectAll();

    [Desc(@"往 log_game_catchfish_event_bulletmiss 表插入单条数据")]
    [Sql(@"
insert into `log_game_catchfish_event_bulletmiss` (`id`, `log_game_catchfish_id`, `type_id`, `bullet_id`, `coin`, `money`)
values ({0}, {1}, {2}, {3}, {4}, {5})")]
    void log_game_catchfish_event_bulletmiss__Insert(int id, int log_game_catchfish_id, int type_id, int bullet_id, long coin, double money);

    [Desc(@"往 log_game_catchfish_event_bulletmiss 表插入单条数据对象")]
    [Sql(@"
insert into `log_game_catchfish_event_bulletmiss` (`id`, `log_game_catchfish_id`, `type_id`, `bullet_id`, `coin`, `money`)
values {0}")]
    void log_game_catchfish_event_bulletmiss__Insert([SkipReadOnly] Tables.log_game_catchfish_event_bulletmiss row);

    [Desc(@"往 log_game_catchfish_event_bulletmiss 表插入多条数据对象")]
    [Sql(@"
insert into `log_game_catchfish_event_bulletmiss` (`id`, `log_game_catchfish_id`, `type_id`, `bullet_id`, `coin`, `money`)
values {0}")]
    void log_game_catchfish_event_bulletmiss__Insert([SkipReadOnly] List<Tables.log_game_catchfish_event_bulletmiss> rows);

    [Desc(@"根据主键查 log_game_catchfish_event_bulletmiss 表指定数据行")]
    [Sql(@"
select `id`, `log_game_catchfish_id`, `type_id`, `bullet_id`, `coin`, `money`
  from `log_game_catchfish_event_bulletmiss`
 where `id` = 0 and `log_game_catchfish_id` = 1")]
    Tables.log_game_catchfish_event_bulletmiss log_game_catchfish_event_bulletmiss__SelectByPrimaryKey(int id, int log_game_catchfish_id);

    [Desc(@"查 log_game_catchfish_event_fire 表所有数据")]
    [Sql(@"
select `id`, `log_game_catchfish_id`, `type_id`, `bullet_id`, `coin`, `money`
  from `log_game_catchfish_event_fire`")]
    List<Tables.log_game_catchfish_event_fire> log_game_catchfish_event_fire__SelectAll();

    [Desc(@"往 log_game_catchfish_event_fire 表插入单条数据")]
    [Sql(@"
insert into `log_game_catchfish_event_fire` (`id`, `log_game_catchfish_id`, `type_id`, `bullet_id`, `coin`, `money`)
values ({0}, {1}, {2}, {3}, {4}, {5})")]
    void log_game_catchfish_event_fire__Insert(int id, int log_game_catchfish_id, int type_id, int bullet_id, long coin, double money);

    [Desc(@"往 log_game_catchfish_event_fire 表插入单条数据对象")]
    [Sql(@"
insert into `log_game_catchfish_event_fire` (`id`, `log_game_catchfish_id`, `type_id`, `bullet_id`, `coin`, `money`)
values {0}")]
    void log_game_catchfish_event_fire__Insert([SkipReadOnly] Tables.log_game_catchfish_event_fire row);

    [Desc(@"往 log_game_catchfish_event_fire 表插入多条数据对象")]
    [Sql(@"
insert into `log_game_catchfish_event_fire` (`id`, `log_game_catchfish_id`, `type_id`, `bullet_id`, `coin`, `money`)
values {0}")]
    void log_game_catchfish_event_fire__Insert([SkipReadOnly] List<Tables.log_game_catchfish_event_fire> rows);

    [Desc(@"根据主键查 log_game_catchfish_event_fire 表指定数据行")]
    [Sql(@"
select `id`, `log_game_catchfish_id`, `type_id`, `bullet_id`, `coin`, `money`
  from `log_game_catchfish_event_fire`
 where `id` = 0 and `log_game_catchfish_id` = 1")]
    Tables.log_game_catchfish_event_fire log_game_catchfish_event_fire__SelectByPrimaryKey(int id, int log_game_catchfish_id);

    [Desc(@"查 log_game_catchfish_event_fishdie 表所有数据")]
    [Sql(@"
select `id`, `log_game_catchfish_id`, `fish_type_id`, `fish_id`, `fish_ratio`, `bullet_coin`, `coin`, `money`
  from `log_game_catchfish_event_fishdie`")]
    List<Tables.log_game_catchfish_event_fishdie> log_game_catchfish_event_fishdie__SelectAll();

    [Desc(@"往 log_game_catchfish_event_fishdie 表插入单条数据")]
    [Sql(@"
insert into `log_game_catchfish_event_fishdie` (`id`, `log_game_catchfish_id`, `fish_type_id`, `fish_id`, `fish_ratio`, `bullet_coin`, `coin`, `money`)
values ({0}, {1}, {2}, {3}, {4}, {5}, {6}, {7})")]
    void log_game_catchfish_event_fishdie__Insert(int id, int log_game_catchfish_id, int fish_type_id, int fish_id, long fish_ratio, long bullet_coin, long coin, double money);

    [Desc(@"往 log_game_catchfish_event_fishdie 表插入单条数据对象")]
    [Sql(@"
insert into `log_game_catchfish_event_fishdie` (`id`, `log_game_catchfish_id`, `fish_type_id`, `fish_id`, `fish_ratio`, `bullet_coin`, `coin`, `money`)
values {0}")]
    void log_game_catchfish_event_fishdie__Insert([SkipReadOnly] Tables.log_game_catchfish_event_fishdie row);

    [Desc(@"往 log_game_catchfish_event_fishdie 表插入多条数据对象")]
    [Sql(@"
insert into `log_game_catchfish_event_fishdie` (`id`, `log_game_catchfish_id`, `fish_type_id`, `fish_id`, `fish_ratio`, `bullet_coin`, `coin`, `money`)
values {0}")]
    void log_game_catchfish_event_fishdie__Insert([SkipReadOnly] List<Tables.log_game_catchfish_event_fishdie> rows);

    [Desc(@"根据主键查 log_game_catchfish_event_fishdie 表指定数据行")]
    [Sql(@"
select `id`, `log_game_catchfish_id`, `fish_type_id`, `fish_id`, `fish_ratio`, `bullet_coin`, `coin`, `money`
  from `log_game_catchfish_event_fishdie`
 where `id` = 0 and `log_game_catchfish_id` = 1")]
    Tables.log_game_catchfish_event_fishdie log_game_catchfish_event_fishdie__SelectByPrimaryKey(int id, int log_game_catchfish_id);

    [Desc(@"查 log_login_out 表所有数据")]
    [Sql(@"
select `id`, `account_id`, `type_id`, `ip`, `create_time`
  from `log_login_out`")]
    List<Tables.log_login_out> log_login_out__SelectAll();

    [Desc(@"往 log_login_out 表插入单条数据")]
    [Sql(@"
insert into `log_login_out` (`account_id`, `type_id`, `ip`, `create_time`)
values ({0}, {1}, {2}, {3})")]
    void log_login_out__Insert(int account_id, int type_id, string ip, long create_time);

    [Desc(@"往 log_login_out 表插入单条数据对象")]
    [Sql(@"
insert into `log_login_out` (`account_id`, `type_id`, `ip`, `create_time`)
values {0}")]
    void log_login_out__Insert([SkipReadOnly] Tables.log_login_out row);

    [Desc(@"往 log_login_out 表插入多条数据对象")]
    [Sql(@"
insert into `log_login_out` (`account_id`, `type_id`, `ip`, `create_time`)
values {0}")]
    void log_login_out__Insert([SkipReadOnly] List<Tables.log_login_out> rows);

    [Desc(@"根据主键查 log_login_out 表指定数据行")]
    [Sql(@"
select `account_id`, `type_id`, `ip`, `create_time`
  from `log_login_out`
 where `id` = 0")]
    Tables.log_login_out log_login_out__SelectByPrimaryKey(int id);

    [Desc(@"查 log_withdrawals 表所有数据")]
    [Sql(@"
select `id`, `withdrawals_id`, `person_in_charge`, `state_id`, `description`, `create_time`
  from `log_withdrawals`")]
    List<Tables.log_withdrawals> log_withdrawals__SelectAll();

    [Desc(@"往 log_withdrawals 表插入单条数据")]
    [Sql(@"
insert into `log_withdrawals` (`withdrawals_id`, `person_in_charge`, `state_id`, `description`, `create_time`)
values ({0}, {1}, {2}, {3}, {4})")]
    void log_withdrawals__Insert(int withdrawals_id, string person_in_charge, int state_id, string description, long create_time);

    [Desc(@"往 log_withdrawals 表插入单条数据对象")]
    [Sql(@"
insert into `log_withdrawals` (`withdrawals_id`, `person_in_charge`, `state_id`, `description`, `create_time`)
values {0}")]
    void log_withdrawals__Insert([SkipReadOnly] Tables.log_withdrawals row);

    [Desc(@"往 log_withdrawals 表插入多条数据对象")]
    [Sql(@"
insert into `log_withdrawals` (`withdrawals_id`, `person_in_charge`, `state_id`, `description`, `create_time`)
values {0}")]
    void log_withdrawals__Insert([SkipReadOnly] List<Tables.log_withdrawals> rows);

    [Desc(@"根据主键查 log_withdrawals 表指定数据行")]
    [Sql(@"
select `withdrawals_id`, `person_in_charge`, `state_id`, `description`, `create_time`
  from `log_withdrawals`
 where `id` = 0")]
    Tables.log_withdrawals log_withdrawals__SelectByPrimaryKey(int id);

    [Desc(@"查 mail 表所有数据")]
    [Sql(@"
select `id`, `content`, `account_id`, `sender`, `create_time`, `is_read`
  from `mail`")]
    List<Tables.mail> mail__SelectAll();

    [Desc(@"往 mail 表插入单条数据")]
    [Sql(@"
insert into `mail` (`content`, `account_id`, `sender`, `create_time`, `is_read`)
values ({0}, {1}, {2}, {3}, {4})")]
    void mail__Insert(string content, int account_id, string sender, long create_time, int is_read);

    [Desc(@"往 mail 表插入单条数据对象")]
    [Sql(@"
insert into `mail` (`content`, `account_id`, `sender`, `create_time`, `is_read`)
values {0}")]
    void mail__Insert([SkipReadOnly] Tables.mail row);

    [Desc(@"往 mail 表插入多条数据对象")]
    [Sql(@"
insert into `mail` (`content`, `account_id`, `sender`, `create_time`, `is_read`)
values {0}")]
    void mail__Insert([SkipReadOnly] List<Tables.mail> rows);

    [Desc(@"根据主键查 mail 表指定数据行")]
    [Sql(@"
select `content`, `account_id`, `sender`, `create_time`, `is_read`
  from `mail`
 where `id` = 0")]
    Tables.mail mail__SelectByPrimaryKey(int id);

    [Desc(@"查 marquee 表所有数据")]
    [Sql(@"
select `id`, `content`, `frequency`, `update_time`, `create_time`, `enable`
  from `marquee`")]
    List<Tables.marquee> marquee__SelectAll();

    [Desc(@"往 marquee 表插入单条数据")]
    [Sql(@"
insert into `marquee` (`content`, `frequency`, `update_time`, `create_time`, `enable`)
values ({0}, {1}, {2}, {3}, {4})")]
    void marquee__Insert(string content, int frequency, long update_time, long create_time, int enable);

    [Desc(@"往 marquee 表插入单条数据对象")]
    [Sql(@"
insert into `marquee` (`content`, `frequency`, `update_time`, `create_time`, `enable`)
values {0}")]
    void marquee__Insert([SkipReadOnly] Tables.marquee row);

    [Desc(@"往 marquee 表插入多条数据对象")]
    [Sql(@"
insert into `marquee` (`content`, `frequency`, `update_time`, `create_time`, `enable`)
values {0}")]
    void marquee__Insert([SkipReadOnly] List<Tables.marquee> rows);

    [Desc(@"根据主键查 marquee 表指定数据行")]
    [Sql(@"
select `content`, `frequency`, `update_time`, `create_time`, `enable`
  from `marquee`
 where `id` = 0")]
    Tables.marquee marquee__SelectByPrimaryKey(int id);

    [Desc(@"查 notice 表所有数据")]
    [Sql(@"
select `id`, `content`, `notice_type_id`, `start_time`, `end_time`, `create_time`
  from `notice`")]
    List<Tables.notice> notice__SelectAll();

    [Desc(@"往 notice 表插入单条数据")]
    [Sql(@"
insert into `notice` (`content`, `notice_type_id`, `start_time`, `end_time`, `create_time`)
values ({0}, {1}, {2}, {3}, {4})")]
    void notice__Insert(string content, int notice_type_id, long? start_time, long? end_time, long create_time);

    [Desc(@"往 notice 表插入单条数据对象")]
    [Sql(@"
insert into `notice` (`content`, `notice_type_id`, `start_time`, `end_time`, `create_time`)
values {0}")]
    void notice__Insert([SkipReadOnly] Tables.notice row);

    [Desc(@"往 notice 表插入多条数据对象")]
    [Sql(@"
insert into `notice` (`content`, `notice_type_id`, `start_time`, `end_time`, `create_time`)
values {0}")]
    void notice__Insert([SkipReadOnly] List<Tables.notice> rows);

    [Desc(@"根据主键查 notice 表指定数据行")]
    [Sql(@"
select `content`, `notice_type_id`, `start_time`, `end_time`, `create_time`
  from `notice`
 where `id` = 0")]
    Tables.notice notice__SelectByPrimaryKey(int id);

    [Desc(@"查 notice_read 表所有数据")]
    [Sql(@"
select `notice_id`, `account_id`, `create_time`
  from `notice_read`")]
    List<Tables.notice_read> notice_read__SelectAll();

    [Desc(@"往 notice_read 表插入单条数据")]
    [Sql(@"
insert into `notice_read` (`notice_id`, `account_id`, `create_time`)
values ({0}, {1}, {2})")]
    void notice_read__Insert(int notice_id, int account_id, long create_time);

    [Desc(@"往 notice_read 表插入单条数据对象")]
    [Sql(@"
insert into `notice_read` (`notice_id`, `account_id`, `create_time`)
values {0}")]
    void notice_read__Insert([SkipReadOnly] Tables.notice_read row);

    [Desc(@"往 notice_read 表插入多条数据对象")]
    [Sql(@"
insert into `notice_read` (`notice_id`, `account_id`, `create_time`)
values {0}")]
    void notice_read__Insert([SkipReadOnly] List<Tables.notice_read> rows);

    [Desc(@"根据主键查 notice_read 表指定数据行")]
    [Sql(@"
select `notice_id`, `account_id`, `create_time`
  from `notice_read`
 where `notice_id` = 0 and `account_id` = 1")]
    Tables.notice_read notice_read__SelectByPrimaryKey(int notice_id, int account_id);

    [Desc(@"查 notice_type 表所有数据")]
    [Sql(@"
select `id`, `name`
  from `notice_type`")]
    List<Tables.notice_type> notice_type__SelectAll();

    [Desc(@"往 notice_type 表插入单条数据")]
    [Sql(@"
insert into `notice_type` (`id`, `name`)
values ({0}, {1})")]
    void notice_type__Insert(int id, string name);

    [Desc(@"往 notice_type 表插入单条数据对象")]
    [Sql(@"
insert into `notice_type` (`id`, `name`)
values {0}")]
    void notice_type__Insert([SkipReadOnly] Tables.notice_type row);

    [Desc(@"往 notice_type 表插入多条数据对象")]
    [Sql(@"
insert into `notice_type` (`id`, `name`)
values {0}")]
    void notice_type__Insert([SkipReadOnly] List<Tables.notice_type> rows);

    [Desc(@"根据主键查 notice_type 表指定数据行")]
    [Sql(@"
select `id`, `name`
  from `notice_type`
 where `id` = 0")]
    Tables.notice_type notice_type__SelectByPrimaryKey(int id);

    [Desc(@"查 verify_code 表所有数据")]
    [Sql(@"
select `phone`, `content`, `create_time`
  from `verify_code`")]
    List<Tables.verify_code> verify_code__SelectAll();

    [Desc(@"往 verify_code 表插入单条数据")]
    [Sql(@"
insert into `verify_code` (`phone`, `content`, `create_time`)
values ({0}, {1}, {2})")]
    void verify_code__Insert(string phone, string content, long create_time);

    [Desc(@"往 verify_code 表插入单条数据对象")]
    [Sql(@"
insert into `verify_code` (`phone`, `content`, `create_time`)
values {0}")]
    void verify_code__Insert([SkipReadOnly] Tables.verify_code row);

    [Desc(@"往 verify_code 表插入多条数据对象")]
    [Sql(@"
insert into `verify_code` (`phone`, `content`, `create_time`)
values {0}")]
    void verify_code__Insert([SkipReadOnly] List<Tables.verify_code> rows);

    [Desc(@"根据主键查 verify_code 表指定数据行")]
    [Sql(@"
select `phone`, `content`, `create_time`
  from `verify_code`
 where `phone` = 0")]
    Tables.verify_code verify_code__SelectByPrimaryKey(string phone);

    [Desc(@"查 vip 表所有数据")]
    [Sql(@"
select `id`, `total_recharge`
  from `vip`")]
    List<Tables.vip> vip__SelectAll();

    [Desc(@"往 vip 表插入单条数据")]
    [Sql(@"
insert into `vip` (`id`, `total_recharge`)
values ({0}, {1})")]
    void vip__Insert(int id, double total_recharge);

    [Desc(@"往 vip 表插入单条数据对象")]
    [Sql(@"
insert into `vip` (`id`, `total_recharge`)
values {0}")]
    void vip__Insert([SkipReadOnly] Tables.vip row);

    [Desc(@"往 vip 表插入多条数据对象")]
    [Sql(@"
insert into `vip` (`id`, `total_recharge`)
values {0}")]
    void vip__Insert([SkipReadOnly] List<Tables.vip> rows);

    [Desc(@"根据主键查 vip 表指定数据行")]
    [Sql(@"
select `id`, `total_recharge`
  from `vip`
 where `id` = 0")]
    Tables.vip vip__SelectByPrimaryKey(int id);

    [Desc(@"查 withdrawals 表所有数据")]
    [Sql(@"
select `withdrawals_id`, `account_id`, `money`, `requirement`, `state_id`, `description`, `create_time`, `last_create_time`
  from `withdrawals`")]
    List<Tables.withdrawals> withdrawals__SelectAll();

    [Desc(@"往 withdrawals 表插入单条数据")]
    [Sql(@"
insert into `withdrawals` (`withdrawals_id`, `account_id`, `money`, `requirement`, `state_id`, `description`, `create_time`, `last_create_time`)
values ({0}, {1}, {2}, {3}, {4}, {5}, {6}, {7})")]
    void withdrawals__Insert(int withdrawals_id, int account_id, double money, string requirement, int state_id, string description, long create_time, long last_create_time);

    [Desc(@"往 withdrawals 表插入单条数据对象")]
    [Sql(@"
insert into `withdrawals` (`withdrawals_id`, `account_id`, `money`, `requirement`, `state_id`, `description`, `create_time`, `last_create_time`)
values {0}")]
    void withdrawals__Insert([SkipReadOnly] Tables.withdrawals row);

    [Desc(@"往 withdrawals 表插入多条数据对象")]
    [Sql(@"
insert into `withdrawals` (`withdrawals_id`, `account_id`, `money`, `requirement`, `state_id`, `description`, `create_time`, `last_create_time`)
values {0}")]
    void withdrawals__Insert([SkipReadOnly] List<Tables.withdrawals> rows);

    [Desc(@"根据主键查 withdrawals 表指定数据行")]
    [Sql(@"
select `withdrawals_id`, `account_id`, `money`, `requirement`, `state_id`, `description`, `create_time`, `last_create_time`
  from `withdrawals`
 where `withdrawals_id` = 0")]
    Tables.withdrawals withdrawals__SelectByPrimaryKey(int withdrawals_id);

    [Desc(@"查 withdrawals_state 表所有数据")]
    [Sql(@"
select `id`, `name`
  from `withdrawals_state`")]
    List<Tables.withdrawals_state> withdrawals_state__SelectAll();

    [Desc(@"往 withdrawals_state 表插入单条数据")]
    [Sql(@"
insert into `withdrawals_state` (`id`, `name`)
values ({0}, {1})")]
    void withdrawals_state__Insert(int id, string name);

    [Desc(@"往 withdrawals_state 表插入单条数据对象")]
    [Sql(@"
insert into `withdrawals_state` (`id`, `name`)
values {0}")]
    void withdrawals_state__Insert([SkipReadOnly] Tables.withdrawals_state row);

    [Desc(@"往 withdrawals_state 表插入多条数据对象")]
    [Sql(@"
insert into `withdrawals_state` (`id`, `name`)
values {0}")]
    void withdrawals_state__Insert([SkipReadOnly] List<Tables.withdrawals_state> rows);

    [Desc(@"根据主键查 withdrawals_state 表指定数据行")]
    [Sql(@"
select `id`, `name`
  from `withdrawals_state`
 where `id` = 0")]
    Tables.withdrawals_state withdrawals_state__SelectByPrimaryKey(int id);

}
