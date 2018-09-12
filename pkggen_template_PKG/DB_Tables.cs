
#pragma warning disable 0169, 0414
using TemplateLibrary;

namespace Tables
{
    /*
CREATE TABLE `account` (
  `id` int(10) NOT NULL COMMENT '用户Id ( 随机 8 位整数 )',
  `username` varchar(50) NOT NULL COMMENT '原始用户名 唯一( GUID )',
  `nickname` varchar(50) NOT NULL COMMENT '昵称 唯一( 默认用某种规则生成 )',
  `avatar_id` int(11) NOT NULL COMMENT '头像',
  `phone` varchar(50) NOT NULL COMMENT '电话号码 唯一( 默认填充 username GUID )',
  `password` varchar(50) NOT NULL COMMENT '密码( 默认为空 )',
  `money` double NOT NULL COMMENT '账户余额( 保留4位小数位, 进部分游戏时会被清0, 结束时会兑换返还 )',
  `money_safe` double NOT NULL COMMENT '保险箱( 玩家可在账户余额间搬运数据 )',
  `total_recharge` double NOT NULL COMMENT '累计充值金额',
  `total_consume` double NOT NULL COMMENT '累计消费金额( 比如在鱼里一共打了多少钱的炮 )',
  `total_withdraw` double NOT NULL COMMENT '累计提现金额',
  `create_time` bigint(20) NOT NULL COMMENT '创建时间. epoch 10m 精度. 所有表的这个字段都是这个格式',
  `enabled` int(11) NOT NULL COMMENT '启用标记( 1 : 启用 )',
  `enter_lobby_time` bigint(20) DEFAULT NULL COMMENT '玩家进入大厅时间(为空 则离开大厅)',
  `log_game_id` int(11) DEFAULT NULL COMMENT '正在游戏中的玩家日志id(为空 则离开游戏)',
  PRIMARY KEY (`id`),
  UNIQUE KEY `phone` (`phone`),
  UNIQUE KEY `nickname` (`nickname`),
  UNIQUE KEY `username` (`username`),
  KEY `FK_account_log_game_id` (`log_game_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 PACK_KEYS=0 COMMENT='玩家账号表( 基表 )'
    */
    class account
    {
        [Column] int id;
        [Column] string username;
        [Column] string nickname;
        [Column] int avatar_id;
        [Column] string phone;
        [Column] string password;
        [Column] double money;
        [Column] double money_safe;
        [Column] double total_recharge;
        [Column] double total_consume;
        [Column] double total_withdraw;
        [Column] long create_time;
        [Column] int enabled;
        [Column] long enter_lobby_time;
        [Column] int log_game_id;
    }
    /*
CREATE TABLE `chat` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `content` varchar(3000) NOT NULL COMMENT '内容',
  `account_id` int(11) NOT NULL COMMENT '聊天人id',
  `create_time` bigint(20) NOT NULL COMMENT '创建时间',
  PRIMARY KEY (`id`),
  KEY `FK_chat_account_id` (`account_id`),
  CONSTRAINT `FK_chat_account_id` FOREIGN KEY (`account_id`) REFERENCES `account` (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=21 DEFAULT CHARSET=utf8 COMMENT='在线聊天'
    */
    class chat
    {
        [Column] int id;
        [Column] string content;
        [Column] int account_id;
        [Column] long create_time;
    }
    /*
CREATE TABLE `config` (
  `key` varchar(50) NOT NULL COMMENT '键名',
  `datatype_name` varchar(50) NOT NULL COMMENT '数据类型名',
  `title` varchar(50) NOT NULL COMMENT '显示名',
  `desc` varchar(250) NOT NULL COMMENT '说明',
  `create_time` bigint(20) NOT NULL COMMENT '创建时间',
  PRIMARY KEY (`key`),
  KEY `FK_config_datatype` (`datatype_name`),
  CONSTRAINT `FK_config_datatype` FOREIGN KEY (`datatype_name`) REFERENCES `datatype` (`name`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='通用配置表( 带版本号的键值对集合 )'
    */
    class config
    {
        [Column] string key;
        [Column] string datatype_name;
        [Column] string title;
        [Column] string desc;
        [Column] long create_time;
    }
    /*
CREATE TABLE `config_value` (
  `key` varchar(50) NOT NULL COMMENT '键名',
  `value_bigint` bigint(20) DEFAULT NULL COMMENT '对应 bigint 数据类型的值( int64 )',
  `value_double` double DEFAULT NULL COMMENT '对应 double 数据类型的值',
  `value_varchar` varchar(250) DEFAULT NULL COMMENT '对应 varchar 数据类型的值( string )',
  `create_time` bigint(20) NOT NULL COMMENT '创建时间',
  PRIMARY KEY (`key`,`create_time`),
  KEY `create_time` (`create_time`),
  KEY `key` (`key`),
  CONSTRAINT `FK_config_value_config` FOREIGN KEY (`key`) REFERENCES `config` (`key`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='配置项所对应的值( 保留历史变化, 通常只有最新的那一个生效 )'
    */
    class config_value
    {
        [Column] string key;
        [Column] long value_bigint;
        [Column] double value_double;
        [Column] string value_varchar;
        [Column] long create_time;
    }
    /*
CREATE TABLE `datatype` (
  `name` varchar(50) NOT NULL,
  PRIMARY KEY (`name`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='数据类型列表( 主要供 config 用 )'
    */
    class datatype
    {
        [Column] string name;
    }
    /*
CREATE TABLE `fish` (
  `id` int(11) NOT NULL COMMENT '鱼配置id',
  `name` varchar(50) NOT NULL COMMENT '名称',
  `minCoin` int(11) NOT NULL COMMENT '打死鱼的金币所得最小基数( 也可理解成倍率 )',
  `maxCoin` int(11) NOT NULL COMMENT ' 打死鱼的金币所得最大基数( 也可理解成倍率 )',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='鱼种类表'
    */
    class fish
    {
        [Column] int id;
        [Column] string name;
        [Column] int minCoin;
        [Column] int maxCoin;
    }
    /*
CREATE TABLE `game` (
  `id` int(11) NOT NULL COMMENT '游戏编号( 非自增, 直接填死 )',
  `name` varchar(50) NOT NULL COMMENT '游戏名',
  `type_name` varchar(50) NOT NULL COMMENT '类型名( 派生表名 )',
  `enabled` int(11) NOT NULL COMMENT '是否启用 1: true',
  PRIMARY KEY (`id`),
  UNIQUE KEY `name` (`name`),
  UNIQUE KEY `type_name` (`type_name`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 PACK_KEYS=0 COMMENT='所有游戏列表( 基表 )'
    */
    class game
    {
        [Column] int id;
        [Column] string name;
        [Column] string type_name;
        [Column] int enabled;
    }
    /*
CREATE TABLE `game_catchfish` (
  `id` int(11) NOT NULL COMMENT '基表游戏编号',
  `exchange_coin_ratio` int(11) NOT NULL COMMENT '进出游戏时 money 自动兑换成 coin 要 乘除 的系数',
  PRIMARY KEY (`id`),
  CONSTRAINT `fk_game_catchfish_id__game_id` FOREIGN KEY (`id`) REFERENCES `game` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='game 的 捕鱼游戏 派生表( 存放专属字段 )'
    */
    class game_catchfish
    {
        [Column] int id;
        [Column] int exchange_coin_ratio;
    }
    /*
CREATE TABLE `game_catchfish_account` (
  `account_id` int(11) NOT NULL COMMENT '账号id',
  `game_id` int(11) NOT NULL COMMENT '捕鱼游戏id',
  `level_id` int(11) NOT NULL COMMENT '级别id',
  `last_bet` bigint(20) NOT NULL COMMENT '玩家上次游戏退出前用的子弹的倍率( 游戏内金币整数值 )',
  `last_cannon_id` int(11) NOT NULL COMMENT '玩家上次游戏退出前用的炮台id',
  PRIMARY KEY (`account_id`,`game_id`,`level_id`),
  KEY `FK_game_catchfish_account_game_catchfish_cannon` (`last_cannon_id`),
  KEY `FK_game_catchfish_account_game_catchfish_level` (`game_id`,`level_id`),
  CONSTRAINT `FK_game_catchfish_account_account` FOREIGN KEY (`account_id`) REFERENCES `account` (`id`),
  CONSTRAINT `FK_game_catchfish_account_game_catchfish_cannon` FOREIGN KEY (`last_cannon_id`) REFERENCES `game_catchfish_cannon` (`id`),
  CONSTRAINT `FK_game_catchfish_account_game_catchfish_level` FOREIGN KEY (`game_id`, `level_id`) REFERENCES `game_catchfish_level` (`game_id`, `level_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='玩家于捕鱼游戏的个人配置信息'
    */
    class game_catchfish_account
    {
        [Column] int account_id;
        [Column] int game_id;
        [Column] int level_id;
        [Column] long last_bet;
        [Column] int last_cannon_id;
    }
    /*
CREATE TABLE `game_catchfish_cannon` (
  `id` int(11) NOT NULL COMMENT '唯一编号 & 炮台Skin',
  `vip_id` int(11) NOT NULL COMMENT 'VIP等级',
  PRIMARY KEY (`id`),
  KEY `FK_game_catchfish_cannon_vip` (`vip_id`),
  CONSTRAINT `FK_game_catchfish_cannon_vip` FOREIGN KEY (`vip_id`) REFERENCES `vip` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='捕鱼游戏内炮台skin与累计充值金额的对应表'
    */
    class game_catchfish_cannon
    {
        [Column] int id;
        [Column] int vip_id;
    }
    /*
CREATE TABLE `game_catchfish_level` (
  `game_id` int(11) NOT NULL COMMENT '游戏id',
  `level_id` int(11) NOT NULL COMMENT '游戏级别id. 0 1 2....: 新手厅  进阶厅  高手厅 ....',
  `min_bet` bigint(20) NOT NULL COMMENT '炮值(从)(游戏内金币)',
  `max_bet` bigint(20) NOT NULL COMMENT '炮值(到)(游戏内金币)',
  `min_money` double NOT NULL COMMENT '最低准入金额',
  `enabled` int(11) NOT NULL COMMENT '是否启用 1: true',
  PRIMARY KEY (`game_id`,`level_id`),
  CONSTRAINT `fk_game_catchfish_level_id__game_catchfish_id` FOREIGN KEY (`game_id`) REFERENCES `game_catchfish` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 PACK_KEYS=0 COMMENT='捕鱼游戏的分级表( 1对多 )'
    */
    class game_catchfish_level
    {
        [Column] int game_id;
        [Column] int level_id;
        [Column] long min_bet;
        [Column] long max_bet;
        [Column] double min_money;
        [Column] int enabled;
    }
    /*
CREATE TABLE `game_catchfish_level_room` (
  `game_id` int(11) NOT NULL AUTO_INCREMENT COMMENT '游戏id',
  `level_id` int(11) NOT NULL COMMENT '游戏级别id',
  `room_id` int(11) NOT NULL COMMENT '房号( 从 1 开始填充, 新增取最大值+1 )',
  `enabled` int(11) NOT NULL COMMENT '是否启用 1: true',
  PRIMARY KEY (`game_id`,`level_id`,`room_id`),
  CONSTRAINT `FK_game_catchfish_level_room_game_catchfish_level` FOREIGN KEY (`game_id`, `level_id`) REFERENCES `game_catchfish_level` (`game_id`, `level_id`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8 COMMENT='捕鱼游戏的分级表 下属房间表( 1对多 )'
    */
    class game_catchfish_level_room
    {
        [Column] int game_id;
        [Column] int level_id;
        [Column] int room_id;
        [Column] int enabled;
    }
    /*
CREATE TABLE `log_account_avatar` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `account_id` int(11) NOT NULL COMMENT '帐号id',
  `avatar_id` int(11) NOT NULL COMMENT '头像',
  `create_time` bigint(20) NOT NULL COMMENT '创建时间',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=6 DEFAULT CHARSET=utf8 COMMENT='头像变更历史表'
    */
    class log_account_avatar
    {
        [Column] int id;
        [Column] int account_id;
        [Column] int avatar_id;
        [Column] long create_time;
    }
    /*
CREATE TABLE `log_account_enabled` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `account_id` int(11) NOT NULL COMMENT '帐号id',
  `enabled` int(11) NOT NULL COMMENT '启用标记',
  `memo` varchar(250) NOT NULL COMMENT '备注( 含修改人 & 原因 )',
  `create_time` bigint(20) NOT NULL COMMENT '日志创建时间( 修改时间 )',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=6 DEFAULT CHARSET=utf8 COMMENT='帐号启用标记变更历史表'
    */
    class log_account_enabled
    {
        [Column] int id;
        [Column] int account_id;
        [Column] int enabled;
        [Column] string memo;
        [Column] long create_time;
    }
    /*
CREATE TABLE `log_account_money_safe` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `account_id` int(11) NOT NULL COMMENT '帐号id',
  `value` double NOT NULL COMMENT '金额( 正数为存入保险箱, 负数为从保险箱提钱 )',
  `create_time` bigint(20) NOT NULL COMMENT '创建时间',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=54 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='保险箱操作日志'
    */
    class log_account_money_safe
    {
        [Column] int id;
        [Column] int account_id;
        [Column] double value;
        [Column] long create_time;
    }
    /*
CREATE TABLE `log_account_nickname` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `account_id` int(11) NOT NULL COMMENT '帐号id',
  `nickname` varchar(50) NOT NULL COMMENT '昵称',
  `create_time` bigint(20) NOT NULL COMMENT '创建时间',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=32 DEFAULT CHARSET=utf8 COMMENT='昵称变更历史表'
    */
    class log_account_nickname
    {
        [Column] int id;
        [Column] int account_id;
        [Column] string nickname;
        [Column] long create_time;
    }
    /*
CREATE TABLE `log_account_password` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `account_id` int(11) NOT NULL COMMENT '帐号id',
  `password` varchar(50) NOT NULL COMMENT '密码',
  `create_time` bigint(20) NOT NULL COMMENT '创建时间',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=25 DEFAULT CHARSET=utf8 COMMENT='密码变更历史表'
    */
    class log_account_password
    {
        [Column] int id;
        [Column] int account_id;
        [Column] string password;
        [Column] long create_time;
    }
    /*
CREATE TABLE `log_account_phone` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `account_id` int(11) NOT NULL COMMENT '帐号id',
  `phone` varchar(50) NOT NULL COMMENT '电话号码',
  `create_time` bigint(20) NOT NULL COMMENT '创建时间',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=utf8 COMMENT='电话号码变更历史表'
    */
    class log_account_phone
    {
        [Column] int id;
        [Column] int account_id;
        [Column] string phone;
        [Column] long create_time;
    }
    /*
CREATE TABLE `log_account_recharge` (
  `token` varchar(50) NOT NULL COMMENT '充值请求原始令牌',
  `account_id` int(11) NOT NULL COMMENT '帐号id',
  `money` double NOT NULL COMMENT '充值金额',
  `memo` varchar(250) NOT NULL COMMENT '备注( 渠道, 操作人等 )',
  `create_time` bigint(20) NOT NULL COMMENT '日志创建时间( 充值时间 )',
  PRIMARY KEY (`token`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='充值记录表'
    */
    class log_account_recharge
    {
        [Column] string token;
        [Column] int account_id;
        [Column] double money;
        [Column] string memo;
        [Column] long create_time;
    }
    /*
CREATE TABLE `log_game` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `game_id` int(11) NOT NULL COMMENT '游戏id(game表id)',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=43 DEFAULT CHARSET=utf8 COMMENT='玩家实时对局数据日志表(基表 )'
    */
    class log_game
    {
        [Column] int id;
        [Column] int game_id;
    }
    /*
CREATE TABLE `log_game_catchfish` (
  `id` int(11) NOT NULL COMMENT '游戏日志id(log_game表id)',
  `level_id` int(11) NOT NULL COMMENT '游戏级别id',
  `room_id` int(11) NOT NULL COMMENT '房号',
  `account_id` int(11) NOT NULL COMMENT '账号id',
  `enter_time` bigint(20) NOT NULL COMMENT '进房时间',
  `enter_money` double NOT NULL COMMENT '进房时的账户余额',
  `leave_time` bigint(20) NOT NULL COMMENT '离开时间( 日志记录创建时间 )',
  `leave_money` double NOT NULL COMMENT '离开时游戏内剩余金币换算成的金额',
  `consume` double NOT NULL COMMENT '当次游戏内花掉的金币换算成的金额总量',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='捕鱼房内玩家实时对局数据日志表( 数据于玩家进游戏时创建, 出游戏时将继续填充这个表剩余字段 )'
    */
    class log_game_catchfish
    {
        [Column] int id;
        [Column] int level_id;
        [Column] int room_id;
        [Column] int account_id;
        [Column] long enter_time;
        [Column] double enter_money;
        [Column] long leave_time;
        [Column] double leave_money;
        [Column] double consume;
    }
    /*
CREATE TABLE `log_game_catchfish_event` (
  `id` int(11) NOT NULL COMMENT '由游戏服自己生成的自增id. 用于派生表关联',
  `log_game_catchfish_id` int(11) NOT NULL COMMENT '所属对局记录id. 用于派生表关联',
  `event_type_id` int(11) NOT NULL COMMENT '事件类型编号. 1: fire   2: fishdie   3: bulletdie    4: bulletmiss',
  `frame_number` int(11) NOT NULL COMMENT '发生时的帧编号',
  `create_time` bigint(20) NOT NULL COMMENT '发生时间',
  PRIMARY KEY (`id`,`log_game_catchfish_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='捕鱼玩家事件日志基表'
    */
    class log_game_catchfish_event
    {
        [Column] int id;
        [Column] int log_game_catchfish_id;
        [Column] int event_type_id;
        [Column] int frame_number;
        [Column] long create_time;
    }
    /*
CREATE TABLE `log_game_catchfish_event_bulletdie` (
  `id` int(11) NOT NULL COMMENT '关联到基表',
  `log_game_catchfish_id` int(11) NOT NULL COMMENT '关联到基表',
  `type_id` int(11) NOT NULL COMMENT '所属类型编号. 0: 普通子弹    1: 钻头蟹    2: 炸弹蟹    3: 闪电鲨',
  `bullet_id` int(11) NOT NULL COMMENT '子弹序号',
  `coin` bigint(20) NOT NULL COMMENT '子弹金币数/倍率/强度',
  `money` double NOT NULL COMMENT '折算成 account.money 相同货币单位的金额',
  PRIMARY KEY (`id`,`log_game_catchfish_id`),
  CONSTRAINT `log_game_catchfish_event_bulletdie_ibfk_1` FOREIGN KEY (`id`, `log_game_catchfish_id`) REFERENCES `log_game_catchfish_event` (`id`, `log_game_catchfish_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='捕鱼玩家事件日志之子弹没打死鱼, 消耗掉了(bulletdie)'
    */
    class log_game_catchfish_event_bulletdie
    {
        [Column] int id;
        [Column] int log_game_catchfish_id;
        [Column] int type_id;
        [Column] int bullet_id;
        [Column] long coin;
        [Column] double money;
    }
    /*
CREATE TABLE `log_game_catchfish_event_bulletmiss` (
  `id` int(11) NOT NULL COMMENT '关联到基表',
  `log_game_catchfish_id` int(11) NOT NULL COMMENT '关联到基表',
  `type_id` int(11) NOT NULL COMMENT '所属类型编号. 0: 普通子弹    1: 钻头蟹    2: 炸弹蟹    3: 闪电鲨',
  `bullet_id` int(11) NOT NULL COMMENT '子弹序号',
  `coin` bigint(20) NOT NULL COMMENT '子弹金币数/倍率/强度',
  `money` double NOT NULL COMMENT '折算成 account.money 相同货币单位的金额',
  PRIMARY KEY (`id`,`log_game_catchfish_id`),
  CONSTRAINT `log_game_catchfish_event_bulletmiss_ibfk_1` FOREIGN KEY (`id`, `log_game_catchfish_id`) REFERENCES `log_game_catchfish_event` (`id`, `log_game_catchfish_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='捕鱼玩家事件日志之子弹打空, 得返还(bulletmiss)'
    */
    class log_game_catchfish_event_bulletmiss
    {
        [Column] int id;
        [Column] int log_game_catchfish_id;
        [Column] int type_id;
        [Column] int bullet_id;
        [Column] long coin;
        [Column] double money;
    }
    /*
CREATE TABLE `log_game_catchfish_event_fire` (
  `id` int(11) NOT NULL COMMENT '关联到基表',
  `log_game_catchfish_id` int(11) NOT NULL COMMENT '关联到基表',
  `type_id` int(11) NOT NULL COMMENT '所属类型编号. 0: 普通子弹    1: 钻头蟹    2: 炸弹蟹    3: 闪电鲨',
  `bullet_id` int(11) NOT NULL COMMENT '子弹序号',
  `coin` bigint(20) NOT NULL COMMENT '子弹金币数/倍率/强度( 爆炸等效果大批量剩余子弹可合并填写, miss 表也对应合并 )',
  `money` double NOT NULL COMMENT '折算成 account.money 相同货币单位的金额',
  PRIMARY KEY (`id`,`log_game_catchfish_id`),
  CONSTRAINT `log_game_catchfish_event_fire_ibfk_1` FOREIGN KEY (`id`, `log_game_catchfish_id`) REFERENCES `log_game_catchfish_event` (`id`, `log_game_catchfish_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='捕鱼玩家事件日志之开火(fire) 注意: 非普通子弹属于服务器强制开火'
    */
    class log_game_catchfish_event_fire
    {
        [Column] int id;
        [Column] int log_game_catchfish_id;
        [Column] int type_id;
        [Column] int bullet_id;
        [Column] long coin;
        [Column] double money;
    }
    /*
CREATE TABLE `log_game_catchfish_event_fishdie` (
  `id` int(11) NOT NULL COMMENT '关联到基表',
  `log_game_catchfish_id` int(11) NOT NULL COMMENT '关联到基表',
  `fish_type_id` int(11) NOT NULL COMMENT '鱼类型/配置编号. miss 情况下为 -1',
  `fish_id` int(11) NOT NULL COMMENT '鱼内部编号. miss 情况下为 0',
  `fish_ratio` bigint(20) NOT NULL COMMENT '鱼倍率. miss 情况下为 1',
  `bullet_coin` bigint(20) NOT NULL COMMENT '子弹金币数',
  `coin` bigint(20) NOT NULL COMMENT '最终玩家所得 = 子弹金币数 * 鱼倍率',
  `money` double NOT NULL COMMENT '最终玩家所得 折算成 account.money 相同货币单位的金额',
  PRIMARY KEY (`id`,`log_game_catchfish_id`),
  CONSTRAINT `log_game_catchfish_event_fishdie_ibfk_1` FOREIGN KEY (`id`, `log_game_catchfish_id`) REFERENCES `log_game_catchfish_event` (`id`, `log_game_catchfish_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='捕鱼玩家事件日志之打死鱼得到钱'
    */
    class log_game_catchfish_event_fishdie
    {
        [Column] int id;
        [Column] int log_game_catchfish_id;
        [Column] int fish_type_id;
        [Column] int fish_id;
        [Column] long fish_ratio;
        [Column] long bullet_coin;
        [Column] long coin;
        [Column] double money;
    }
    /*
CREATE TABLE `log_login_out` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `account_id` int(11) NOT NULL COMMENT '帐号id',
  `type_id` int(11) NOT NULL COMMENT '1. 上线     2. 断线     3. 重连.    4. 下线',
  `ip` varchar(50) NOT NULL COMMENT '上线或重连时的IP地址',
  `create_time` bigint(20) NOT NULL COMMENT '创建时间',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=503 DEFAULT CHARSET=utf8 COMMENT='记录玩家上下线情况的日志. 分4种情况:   1. 上线     2. 断线     3. 重连.    4. 下线'
    */
    class log_login_out
    {
        [Column] int id;
        [Column] int account_id;
        [Column] int type_id;
        [Column] string ip;
        [Column] long create_time;
    }
    /*
CREATE TABLE `log_withdrawals` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `withdrawals_id` int(11) NOT NULL COMMENT '申请提现表id',
  `person_in_charge` varchar(100) NOT NULL COMMENT '操作人(客服)',
  `state_id` int(11) NOT NULL COMMENT '操作状态',
  `description` varchar(500) NOT NULL COMMENT '操作描述',
  `create_time` bigint(20) NOT NULL COMMENT '创建时间',
  PRIMARY KEY (`id`),
  KEY `FK_log_withdrawals_state_id` (`state_id`),
  CONSTRAINT `FK_log_withdrawals_state_id` FOREIGN KEY (`state_id`) REFERENCES `withdrawals_state` (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=70 DEFAULT CHARSET=utf8 COMMENT='申请提现状态修改日志表'
    */
    class log_withdrawals
    {
        [Column] int id;
        [Column] int withdrawals_id;
        [Column] string person_in_charge;
        [Column] int state_id;
        [Column] string description;
        [Column] long create_time;
    }
    /*
CREATE TABLE `mail` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `content` varchar(3000) NOT NULL COMMENT '内容',
  `account_id` int(11) NOT NULL COMMENT '收件人',
  `sender` varchar(100) NOT NULL COMMENT '发件人',
  `create_time` bigint(20) NOT NULL COMMENT '创建时间',
  `is_read` int(11) NOT NULL COMMENT '是否已读(0未读, 1已读)',
  PRIMARY KEY (`id`),
  KEY `FK_emlbox_account_id` (`account_id`),
  CONSTRAINT `FK_emlbox_account_id` FOREIGN KEY (`account_id`) REFERENCES `account` (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=27 DEFAULT CHARSET=utf8 COMMENT='邮箱表'
    */
    class mail
    {
        [Column] int id;
        [Column] string content;
        [Column] int account_id;
        [Column] string sender;
        [Column] long create_time;
        [Column] int is_read;
    }
    /*
CREATE TABLE `marquee` (
  `id` int(11) NOT NULL AUTO_INCREMENT COMMENT '自增id',
  `content` varchar(3000) NOT NULL DEFAULT '0' COMMENT '内容',
  `frequency` int(11) NOT NULL DEFAULT '0' COMMENT '喊话频率(多长时间循环执行一次，单位 秒)',
  `update_time` bigint(20) NOT NULL DEFAULT '0' COMMENT '更新时间',
  `create_time` bigint(20) NOT NULL DEFAULT '0' COMMENT '创建时间',
  `enable` int(11) NOT NULL DEFAULT '0' COMMENT '是否有效(1有效)',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8 COMMENT='跑马灯表'
    */
    class marquee
    {
        [Column] int id;
        [Column] string content;
        [Column] int frequency;
        [Column] long update_time;
        [Column] long create_time;
        [Column] int enable;
    }
    /*
CREATE TABLE `notice` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `content` varchar(3000) NOT NULL COMMENT '公告内容',
  `notice_type_id` int(11) NOT NULL COMMENT '公告类型',
  `start_time` bigint(20) DEFAULT NULL COMMENT '有效起始时间',
  `end_time` bigint(20) DEFAULT NULL COMMENT '有效结束时间',
  `create_time` bigint(20) NOT NULL COMMENT '创建时间',
  PRIMARY KEY (`id`),
  KEY `FK_notice_type_id` (`notice_type_id`),
  CONSTRAINT `FK_notice_type_id` FOREIGN KEY (`notice_type_id`) REFERENCES `notice_type` (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=16 DEFAULT CHARSET=utf8 COMMENT='公告表'
    */
    class notice
    {
        [Column] int id;
        [Column] string content;
        [Column] int notice_type_id;
        [Column] long start_time;
        [Column] long end_time;
        [Column] long create_time;
    }
    /*
CREATE TABLE `notice_read` (
  `notice_id` int(11) NOT NULL COMMENT '公告id',
  `account_id` int(11) NOT NULL COMMENT '玩家id',
  `create_time` bigint(20) NOT NULL COMMENT '创建时间',
  UNIQUE KEY `index_notice_account_id` (`notice_id`,`account_id`),
  KEY `FK_notice_read_notice_id` (`notice_id`),
  KEY `FK_notice_read_account_id` (`account_id`),
  CONSTRAINT `FK_notice_read_account_id` FOREIGN KEY (`account_id`) REFERENCES `account` (`id`),
  CONSTRAINT `FK_notice_read_notice_id` FOREIGN KEY (`notice_id`) REFERENCES `notice` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='公告读取表'
    */
    class notice_read
    {
        [Column] int notice_id;
        [Column] int account_id;
        [Column] long create_time;
    }
    /*
CREATE TABLE `notice_type` (
  `id` int(11) NOT NULL,
  `name` varchar(50) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='公告类型表'
    */
    class notice_type
    {
        [Column] int id;
        [Column] string name;
    }
    /*
CREATE TABLE `verify_code` (
  `phone` varchar(50) NOT NULL COMMENT '收短信的电话号码',
  `content` varchar(50) NOT NULL COMMENT '短信验证码内容',
  `create_time` bigint(20) NOT NULL COMMENT '下发时间( 用于判断数据是否过期, 过期就删除 )',
  PRIMARY KEY (`phone`),
  KEY `Index 2` (`create_time`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='短信校验状态数据( 数据有有效期. 数据服定时删掉超时数据 )'
    */
    class verify_code
    {
        [Column] string phone;
        [Column] string content;
        [Column] long create_time;
    }
    /*
CREATE TABLE `vip` (
  `id` int(11) NOT NULL COMMENT '唯一编号 & 级别',
  `total_recharge` double NOT NULL COMMENT '累计充值金额要求',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='VIP与累计充值金额的对应表'
    */
    class vip
    {
        [Column] int id;
        [Column] double total_recharge;
    }
    /*
CREATE TABLE `withdrawals` (
  `withdrawals_id` int(10) NOT NULL COMMENT '提现订单号( 随机 9 位整数 )',
  `account_id` int(11) NOT NULL COMMENT '帐号id',
  `money` double NOT NULL COMMENT '提现金额',
  `requirement` varchar(250) NOT NULL COMMENT '提现请求( 渠道, 账号等 )',
  `state_id` int(11) NOT NULL COMMENT '操作状态',
  `description` varchar(500) NOT NULL COMMENT '操作描述',
  `create_time` bigint(20) NOT NULL COMMENT '提现申请时间',
  `last_create_time` bigint(20) NOT NULL COMMENT '最后处理时间',
  PRIMARY KEY (`withdrawals_id`),
  KEY `FK_withdrawals_state_id` (`state_id`),
  CONSTRAINT `FK_withdrawals_state_id` FOREIGN KEY (`state_id`) REFERENCES `withdrawals_state` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='申请提现表'
    */
    class withdrawals
    {
        [Column] int withdrawals_id;
        [Column] int account_id;
        [Column] double money;
        [Column] string requirement;
        [Column] int state_id;
        [Column] string description;
        [Column] long create_time;
        [Column] long last_create_time;
    }
    /*
CREATE TABLE `withdrawals_state` (
  `id` int(11) NOT NULL,
  `name` varchar(50) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='提现状态表'
    */
    class withdrawals_state
    {
        [Column] int id;
        [Column] string name;
    }
}
