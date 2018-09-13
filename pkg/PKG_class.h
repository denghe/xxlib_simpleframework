#pragma once
#include "xx.h"

namespace PKG
{
	struct PkgGenMd5
	{
		static constexpr char const* value = "dde27285e5784277142135589ddf9e51";
    };

    class Foo;
    using Foo_p = xx::Ptr<Foo>;
    using Foo_r = xx::Ref<Foo>;

    class FooEx;
    using FooEx_p = xx::Ptr<FooEx>;
    using FooEx_r = xx::Ref<FooEx>;

    class Node;
    using Node_p = xx::Ptr<Node>;
    using Node_r = xx::Ref<Node>;

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

namespace Tables
{
    // 玩家账号表( 基表 )
    class account;
    using account_p = xx::Ptr<account>;
    using account_r = xx::Ref<account>;

    // 在线聊天
    class chat;
    using chat_p = xx::Ptr<chat>;
    using chat_r = xx::Ref<chat>;

    // 通用配置表( 带版本号的键值对集合 )
    class config;
    using config_p = xx::Ptr<config>;
    using config_r = xx::Ref<config>;

    // 配置项所对应的值( 保留历史变化, 通常只有最新的那一个生效 )
    class config_value;
    using config_value_p = xx::Ptr<config_value>;
    using config_value_r = xx::Ref<config_value>;

    // 数据类型列表( 主要供 config 用 )
    class datatype;
    using datatype_p = xx::Ptr<datatype>;
    using datatype_r = xx::Ref<datatype>;

    // 鱼种类表
    class fish;
    using fish_p = xx::Ptr<fish>;
    using fish_r = xx::Ref<fish>;

    // 所有游戏列表( 基表 )
    class game;
    using game_p = xx::Ptr<game>;
    using game_r = xx::Ref<game>;

    // game 的 捕鱼游戏 派生表( 存放专属字段 )
    class game_catchfish;
    using game_catchfish_p = xx::Ptr<game_catchfish>;
    using game_catchfish_r = xx::Ref<game_catchfish>;

    // 玩家于捕鱼游戏的个人配置信息
    class game_catchfish_account;
    using game_catchfish_account_p = xx::Ptr<game_catchfish_account>;
    using game_catchfish_account_r = xx::Ref<game_catchfish_account>;

    // 捕鱼游戏内炮台skin与累计充值金额的对应表
    class game_catchfish_cannon;
    using game_catchfish_cannon_p = xx::Ptr<game_catchfish_cannon>;
    using game_catchfish_cannon_r = xx::Ref<game_catchfish_cannon>;

    // 捕鱼游戏的分级表( 1对多 )
    class game_catchfish_level;
    using game_catchfish_level_p = xx::Ptr<game_catchfish_level>;
    using game_catchfish_level_r = xx::Ref<game_catchfish_level>;

    // 捕鱼游戏的分级表 下属房间表( 1对多 )
    class game_catchfish_level_room;
    using game_catchfish_level_room_p = xx::Ptr<game_catchfish_level_room>;
    using game_catchfish_level_room_r = xx::Ref<game_catchfish_level_room>;

    // 头像变更历史表
    class log_account_avatar;
    using log_account_avatar_p = xx::Ptr<log_account_avatar>;
    using log_account_avatar_r = xx::Ref<log_account_avatar>;

    // 帐号启用标记变更历史表
    class log_account_enabled;
    using log_account_enabled_p = xx::Ptr<log_account_enabled>;
    using log_account_enabled_r = xx::Ref<log_account_enabled>;

    // 保险箱操作日志
    class log_account_money_safe;
    using log_account_money_safe_p = xx::Ptr<log_account_money_safe>;
    using log_account_money_safe_r = xx::Ref<log_account_money_safe>;

    // 昵称变更历史表
    class log_account_nickname;
    using log_account_nickname_p = xx::Ptr<log_account_nickname>;
    using log_account_nickname_r = xx::Ref<log_account_nickname>;

    // 密码变更历史表
    class log_account_password;
    using log_account_password_p = xx::Ptr<log_account_password>;
    using log_account_password_r = xx::Ref<log_account_password>;

    // 电话号码变更历史表
    class log_account_phone;
    using log_account_phone_p = xx::Ptr<log_account_phone>;
    using log_account_phone_r = xx::Ref<log_account_phone>;

    // 充值记录表
    class log_account_recharge;
    using log_account_recharge_p = xx::Ptr<log_account_recharge>;
    using log_account_recharge_r = xx::Ref<log_account_recharge>;

    // 玩家实时对局数据日志表(基表 )
    class log_game;
    using log_game_p = xx::Ptr<log_game>;
    using log_game_r = xx::Ref<log_game>;

    // 捕鱼房内玩家实时对局数据日志表( 数据于玩家进游戏时创建, 出游戏时将继续填充这个表剩余字段 )
    class log_game_catchfish;
    using log_game_catchfish_p = xx::Ptr<log_game_catchfish>;
    using log_game_catchfish_r = xx::Ref<log_game_catchfish>;

    // 捕鱼玩家事件日志基表
    class log_game_catchfish_event;
    using log_game_catchfish_event_p = xx::Ptr<log_game_catchfish_event>;
    using log_game_catchfish_event_r = xx::Ref<log_game_catchfish_event>;

    // 捕鱼玩家事件日志之子弹没打死鱼, 消耗掉了(bulletdie)
    class log_game_catchfish_event_bulletdie;
    using log_game_catchfish_event_bulletdie_p = xx::Ptr<log_game_catchfish_event_bulletdie>;
    using log_game_catchfish_event_bulletdie_r = xx::Ref<log_game_catchfish_event_bulletdie>;

    // 捕鱼玩家事件日志之子弹打空, 得返还(bulletmiss)
    class log_game_catchfish_event_bulletmiss;
    using log_game_catchfish_event_bulletmiss_p = xx::Ptr<log_game_catchfish_event_bulletmiss>;
    using log_game_catchfish_event_bulletmiss_r = xx::Ref<log_game_catchfish_event_bulletmiss>;

    // 捕鱼玩家事件日志之开火(fire) 注意: 非普通子弹属于服务器强制开火
    class log_game_catchfish_event_fire;
    using log_game_catchfish_event_fire_p = xx::Ptr<log_game_catchfish_event_fire>;
    using log_game_catchfish_event_fire_r = xx::Ref<log_game_catchfish_event_fire>;

    // 捕鱼玩家事件日志之打死鱼得到钱
    class log_game_catchfish_event_fishdie;
    using log_game_catchfish_event_fishdie_p = xx::Ptr<log_game_catchfish_event_fishdie>;
    using log_game_catchfish_event_fishdie_r = xx::Ref<log_game_catchfish_event_fishdie>;

    // 记录玩家上下线情况的日志. 分4种情况:   1. 上线     2. 断线     3. 重连.    4. 下线
    class log_login_out;
    using log_login_out_p = xx::Ptr<log_login_out>;
    using log_login_out_r = xx::Ref<log_login_out>;

    // 申请提现状态修改日志表
    class log_withdrawals;
    using log_withdrawals_p = xx::Ptr<log_withdrawals>;
    using log_withdrawals_r = xx::Ref<log_withdrawals>;

    // 邮箱表
    class mail;
    using mail_p = xx::Ptr<mail>;
    using mail_r = xx::Ref<mail>;

    // 跑马灯表
    class marquee;
    using marquee_p = xx::Ptr<marquee>;
    using marquee_r = xx::Ref<marquee>;

    // 公告表
    class notice;
    using notice_p = xx::Ptr<notice>;
    using notice_r = xx::Ref<notice>;

    // 公告读取表
    class notice_read;
    using notice_read_p = xx::Ptr<notice_read>;
    using notice_read_r = xx::Ref<notice_read>;

    // 公告类型表
    class notice_type;
    using notice_type_p = xx::Ptr<notice_type>;
    using notice_type_r = xx::Ref<notice_type>;

    // 短信校验状态数据( 数据有有效期. 数据服定时删掉超时数据 )
    class verify_code;
    using verify_code_p = xx::Ptr<verify_code>;
    using verify_code_r = xx::Ref<verify_code>;

    // VIP与累计充值金额的对应表
    class vip;
    using vip_p = xx::Ptr<vip>;
    using vip_r = xx::Ref<vip>;

    // 申请提现表
    class withdrawals;
    using withdrawals_p = xx::Ptr<withdrawals>;
    using withdrawals_r = xx::Ref<withdrawals>;

    // 提现状态表
    class withdrawals_state;
    using withdrawals_state_p = xx::Ptr<withdrawals_state>;
    using withdrawals_state_r = xx::Ref<withdrawals_state>;

    class node;
    using node_p = xx::Ptr<node>;
    using node_r = xx::Ref<node>;

}
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
        xx::Object_p o = nullptr;
        xx::List_p<xx::Object_p> os;

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
namespace Tables
{
    class node : public xx::Object
    {
    public:
        int32_t id = 0;
        std::optional<int32_t> pid;

        typedef node ThisType;
        typedef xx::Object BaseType;
	    node(xx::MemPool* const& mempool) noexcept;
	    node(xx::BBuffer* const& bb);
		node(node const&) = delete;
		node& operator=(node const&) = delete;
        void ToString(xx::String& s) const noexcept override;
        void ToStringCore(xx::String& s) const noexcept override;
        void ToBBuffer(xx::BBuffer& bb) const noexcept override;
        int FromBBuffer(xx::BBuffer& bb) noexcept override;
        int FromBBufferCore(xx::BBuffer& bb) noexcept;
        void CopyTo(node* const& o) const noexcept;
        node* MakeCopy() const noexcept;
        node_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
    };
}
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
namespace Tables
{
    // 电话号码变更历史表
    class log_account_phone : public xx::Object
    {
    public:
        int32_t id = 0;
        // 帐号id
        int32_t account_id = 0;
        // 电话号码
        xx::String_p phone;
        // 创建时间
        int64_t create_time = 0;

        typedef log_account_phone ThisType;
        typedef xx::Object BaseType;
	    log_account_phone(xx::MemPool* const& mempool) noexcept;
	    log_account_phone(xx::BBuffer* const& bb);
		log_account_phone(log_account_phone const&) = delete;
		log_account_phone& operator=(log_account_phone const&) = delete;
        void ToString(xx::String& s) const noexcept override;
        void ToStringCore(xx::String& s) const noexcept override;
        void ToBBuffer(xx::BBuffer& bb) const noexcept override;
        int FromBBuffer(xx::BBuffer& bb) noexcept override;
        int FromBBufferCore(xx::BBuffer& bb) noexcept;
        void CopyTo(log_account_phone* const& o) const noexcept;
        log_account_phone* MakeCopy() const noexcept;
        log_account_phone_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
    };
    // 充值记录表
    class log_account_recharge : public xx::Object
    {
    public:
        // 充值请求原始令牌
        xx::String_p token;
        // 帐号id
        int32_t account_id = 0;
        // 充值金额
        double money = 0;
        // 备注( 渠道, 操作人等 )
        xx::String_p memo;
        // 日志创建时间( 充值时间 )
        int64_t create_time = 0;

        typedef log_account_recharge ThisType;
        typedef xx::Object BaseType;
	    log_account_recharge(xx::MemPool* const& mempool) noexcept;
	    log_account_recharge(xx::BBuffer* const& bb);
		log_account_recharge(log_account_recharge const&) = delete;
		log_account_recharge& operator=(log_account_recharge const&) = delete;
        void ToString(xx::String& s) const noexcept override;
        void ToStringCore(xx::String& s) const noexcept override;
        void ToBBuffer(xx::BBuffer& bb) const noexcept override;
        int FromBBuffer(xx::BBuffer& bb) noexcept override;
        int FromBBufferCore(xx::BBuffer& bb) noexcept;
        void CopyTo(log_account_recharge* const& o) const noexcept;
        log_account_recharge* MakeCopy() const noexcept;
        log_account_recharge_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
    };
    // 玩家实时对局数据日志表(基表 )
    class log_game : public xx::Object
    {
    public:
        int32_t id = 0;
        // 游戏id(game表id)
        int32_t game_id = 0;

        typedef log_game ThisType;
        typedef xx::Object BaseType;
	    log_game(xx::MemPool* const& mempool) noexcept;
	    log_game(xx::BBuffer* const& bb);
		log_game(log_game const&) = delete;
		log_game& operator=(log_game const&) = delete;
        void ToString(xx::String& s) const noexcept override;
        void ToStringCore(xx::String& s) const noexcept override;
        void ToBBuffer(xx::BBuffer& bb) const noexcept override;
        int FromBBuffer(xx::BBuffer& bb) noexcept override;
        int FromBBufferCore(xx::BBuffer& bb) noexcept;
        void CopyTo(log_game* const& o) const noexcept;
        log_game* MakeCopy() const noexcept;
        log_game_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
    };
    // 捕鱼房内玩家实时对局数据日志表( 数据于玩家进游戏时创建, 出游戏时将继续填充这个表剩余字段 )
    class log_game_catchfish : public xx::Object
    {
    public:
        // 游戏日志id(log_game表id)
        int32_t id = 0;
        // 游戏级别id
        int32_t level_id = 0;
        // 房号
        int32_t room_id = 0;
        // 账号id
        int32_t account_id = 0;
        // 进房时间
        int64_t enter_time = 0;
        // 进房时的账户余额
        double enter_money = 0;
        // 离开时间( 日志记录创建时间 )
        int64_t leave_time = 0;
        // 离开时游戏内剩余金币换算成的金额
        double leave_money = 0;
        // 当次游戏内花掉的金币换算成的金额总量
        double consume = 0;

        typedef log_game_catchfish ThisType;
        typedef xx::Object BaseType;
	    log_game_catchfish(xx::MemPool* const& mempool) noexcept;
	    log_game_catchfish(xx::BBuffer* const& bb);
		log_game_catchfish(log_game_catchfish const&) = delete;
		log_game_catchfish& operator=(log_game_catchfish const&) = delete;
        void ToString(xx::String& s) const noexcept override;
        void ToStringCore(xx::String& s) const noexcept override;
        void ToBBuffer(xx::BBuffer& bb) const noexcept override;
        int FromBBuffer(xx::BBuffer& bb) noexcept override;
        int FromBBufferCore(xx::BBuffer& bb) noexcept;
        void CopyTo(log_game_catchfish* const& o) const noexcept;
        log_game_catchfish* MakeCopy() const noexcept;
        log_game_catchfish_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
    };
    // 捕鱼玩家事件日志基表
    class log_game_catchfish_event : public xx::Object
    {
    public:
        // 由游戏服自己生成的自增id. 用于派生表关联
        int32_t id = 0;
        // 所属对局记录id. 用于派生表关联
        int32_t log_game_catchfish_id = 0;
        // 事件类型编号. 1: fire   2: fishdie   3: bulletdie    4: bulletmiss
        int32_t event_type_id = 0;
        // 发生时的帧编号
        int32_t frame_number = 0;
        // 发生时间
        int64_t create_time = 0;

        typedef log_game_catchfish_event ThisType;
        typedef xx::Object BaseType;
	    log_game_catchfish_event(xx::MemPool* const& mempool) noexcept;
	    log_game_catchfish_event(xx::BBuffer* const& bb);
		log_game_catchfish_event(log_game_catchfish_event const&) = delete;
		log_game_catchfish_event& operator=(log_game_catchfish_event const&) = delete;
        void ToString(xx::String& s) const noexcept override;
        void ToStringCore(xx::String& s) const noexcept override;
        void ToBBuffer(xx::BBuffer& bb) const noexcept override;
        int FromBBuffer(xx::BBuffer& bb) noexcept override;
        int FromBBufferCore(xx::BBuffer& bb) noexcept;
        void CopyTo(log_game_catchfish_event* const& o) const noexcept;
        log_game_catchfish_event* MakeCopy() const noexcept;
        log_game_catchfish_event_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
    };
    // 捕鱼玩家事件日志之子弹没打死鱼, 消耗掉了(bulletdie)
    class log_game_catchfish_event_bulletdie : public xx::Object
    {
    public:
        // 关联到基表
        int32_t id = 0;
        // 关联到基表
        int32_t log_game_catchfish_id = 0;
        // 所属类型编号. 0: 普通子弹    1: 钻头蟹    2: 炸弹蟹    3: 闪电鲨
        int32_t type_id = 0;
        // 子弹序号
        int32_t bullet_id = 0;
        // 子弹金币数/倍率/强度
        int64_t coin = 0;
        // 折算成 account.money 相同货币单位的金额
        double money = 0;

        typedef log_game_catchfish_event_bulletdie ThisType;
        typedef xx::Object BaseType;
	    log_game_catchfish_event_bulletdie(xx::MemPool* const& mempool) noexcept;
	    log_game_catchfish_event_bulletdie(xx::BBuffer* const& bb);
		log_game_catchfish_event_bulletdie(log_game_catchfish_event_bulletdie const&) = delete;
		log_game_catchfish_event_bulletdie& operator=(log_game_catchfish_event_bulletdie const&) = delete;
        void ToString(xx::String& s) const noexcept override;
        void ToStringCore(xx::String& s) const noexcept override;
        void ToBBuffer(xx::BBuffer& bb) const noexcept override;
        int FromBBuffer(xx::BBuffer& bb) noexcept override;
        int FromBBufferCore(xx::BBuffer& bb) noexcept;
        void CopyTo(log_game_catchfish_event_bulletdie* const& o) const noexcept;
        log_game_catchfish_event_bulletdie* MakeCopy() const noexcept;
        log_game_catchfish_event_bulletdie_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
    };
    // 捕鱼玩家事件日志之子弹打空, 得返还(bulletmiss)
    class log_game_catchfish_event_bulletmiss : public xx::Object
    {
    public:
        // 关联到基表
        int32_t id = 0;
        // 关联到基表
        int32_t log_game_catchfish_id = 0;
        // 所属类型编号. 0: 普通子弹    1: 钻头蟹    2: 炸弹蟹    3: 闪电鲨
        int32_t type_id = 0;
        // 子弹序号
        int32_t bullet_id = 0;
        // 子弹金币数/倍率/强度
        int64_t coin = 0;
        // 折算成 account.money 相同货币单位的金额
        double money = 0;

        typedef log_game_catchfish_event_bulletmiss ThisType;
        typedef xx::Object BaseType;
	    log_game_catchfish_event_bulletmiss(xx::MemPool* const& mempool) noexcept;
	    log_game_catchfish_event_bulletmiss(xx::BBuffer* const& bb);
		log_game_catchfish_event_bulletmiss(log_game_catchfish_event_bulletmiss const&) = delete;
		log_game_catchfish_event_bulletmiss& operator=(log_game_catchfish_event_bulletmiss const&) = delete;
        void ToString(xx::String& s) const noexcept override;
        void ToStringCore(xx::String& s) const noexcept override;
        void ToBBuffer(xx::BBuffer& bb) const noexcept override;
        int FromBBuffer(xx::BBuffer& bb) noexcept override;
        int FromBBufferCore(xx::BBuffer& bb) noexcept;
        void CopyTo(log_game_catchfish_event_bulletmiss* const& o) const noexcept;
        log_game_catchfish_event_bulletmiss* MakeCopy() const noexcept;
        log_game_catchfish_event_bulletmiss_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
    };
    // 捕鱼玩家事件日志之开火(fire) 注意: 非普通子弹属于服务器强制开火
    class log_game_catchfish_event_fire : public xx::Object
    {
    public:
        // 关联到基表
        int32_t id = 0;
        // 关联到基表
        int32_t log_game_catchfish_id = 0;
        // 所属类型编号. 0: 普通子弹    1: 钻头蟹    2: 炸弹蟹    3: 闪电鲨
        int32_t type_id = 0;
        // 子弹序号
        int32_t bullet_id = 0;
        // 子弹金币数/倍率/强度( 爆炸等效果大批量剩余子弹可合并填写, miss 表也对应合并 )
        int64_t coin = 0;
        // 折算成 account.money 相同货币单位的金额
        double money = 0;

        typedef log_game_catchfish_event_fire ThisType;
        typedef xx::Object BaseType;
	    log_game_catchfish_event_fire(xx::MemPool* const& mempool) noexcept;
	    log_game_catchfish_event_fire(xx::BBuffer* const& bb);
		log_game_catchfish_event_fire(log_game_catchfish_event_fire const&) = delete;
		log_game_catchfish_event_fire& operator=(log_game_catchfish_event_fire const&) = delete;
        void ToString(xx::String& s) const noexcept override;
        void ToStringCore(xx::String& s) const noexcept override;
        void ToBBuffer(xx::BBuffer& bb) const noexcept override;
        int FromBBuffer(xx::BBuffer& bb) noexcept override;
        int FromBBufferCore(xx::BBuffer& bb) noexcept;
        void CopyTo(log_game_catchfish_event_fire* const& o) const noexcept;
        log_game_catchfish_event_fire* MakeCopy() const noexcept;
        log_game_catchfish_event_fire_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
    };
    // 捕鱼玩家事件日志之打死鱼得到钱
    class log_game_catchfish_event_fishdie : public xx::Object
    {
    public:
        // 关联到基表
        int32_t id = 0;
        // 关联到基表
        int32_t log_game_catchfish_id = 0;
        // 鱼类型/配置编号. miss 情况下为 -1
        int32_t fish_type_id = 0;
        // 鱼内部编号. miss 情况下为 0
        int32_t fish_id = 0;
        // 鱼倍率. miss 情况下为 1
        int64_t fish_ratio = 0;
        // 子弹金币数
        int64_t bullet_coin = 0;
        // 最终玩家所得 = 子弹金币数 * 鱼倍率
        int64_t coin = 0;
        // 最终玩家所得 折算成 account.money 相同货币单位的金额
        double money = 0;

        typedef log_game_catchfish_event_fishdie ThisType;
        typedef xx::Object BaseType;
	    log_game_catchfish_event_fishdie(xx::MemPool* const& mempool) noexcept;
	    log_game_catchfish_event_fishdie(xx::BBuffer* const& bb);
		log_game_catchfish_event_fishdie(log_game_catchfish_event_fishdie const&) = delete;
		log_game_catchfish_event_fishdie& operator=(log_game_catchfish_event_fishdie const&) = delete;
        void ToString(xx::String& s) const noexcept override;
        void ToStringCore(xx::String& s) const noexcept override;
        void ToBBuffer(xx::BBuffer& bb) const noexcept override;
        int FromBBuffer(xx::BBuffer& bb) noexcept override;
        int FromBBufferCore(xx::BBuffer& bb) noexcept;
        void CopyTo(log_game_catchfish_event_fishdie* const& o) const noexcept;
        log_game_catchfish_event_fishdie* MakeCopy() const noexcept;
        log_game_catchfish_event_fishdie_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
    };
    // 记录玩家上下线情况的日志. 分4种情况:   1. 上线     2. 断线     3. 重连.    4. 下线
    class log_login_out : public xx::Object
    {
    public:
        int32_t id = 0;
        // 帐号id
        int32_t account_id = 0;
        // 1. 上线     2. 断线     3. 重连.    4. 下线
        int32_t type_id = 0;
        // 上线或重连时的IP地址
        xx::String_p ip;
        // 创建时间
        int64_t create_time = 0;

        typedef log_login_out ThisType;
        typedef xx::Object BaseType;
	    log_login_out(xx::MemPool* const& mempool) noexcept;
	    log_login_out(xx::BBuffer* const& bb);
		log_login_out(log_login_out const&) = delete;
		log_login_out& operator=(log_login_out const&) = delete;
        void ToString(xx::String& s) const noexcept override;
        void ToStringCore(xx::String& s) const noexcept override;
        void ToBBuffer(xx::BBuffer& bb) const noexcept override;
        int FromBBuffer(xx::BBuffer& bb) noexcept override;
        int FromBBufferCore(xx::BBuffer& bb) noexcept;
        void CopyTo(log_login_out* const& o) const noexcept;
        log_login_out* MakeCopy() const noexcept;
        log_login_out_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
    };
    // 申请提现状态修改日志表
    class log_withdrawals : public xx::Object
    {
    public:
        int32_t id = 0;
        // 申请提现表id
        int32_t withdrawals_id = 0;
        // 操作人(客服)
        xx::String_p person_in_charge;
        // 操作状态
        int32_t state_id = 0;
        // 操作描述
        xx::String_p description;
        // 创建时间
        int64_t create_time = 0;

        typedef log_withdrawals ThisType;
        typedef xx::Object BaseType;
	    log_withdrawals(xx::MemPool* const& mempool) noexcept;
	    log_withdrawals(xx::BBuffer* const& bb);
		log_withdrawals(log_withdrawals const&) = delete;
		log_withdrawals& operator=(log_withdrawals const&) = delete;
        void ToString(xx::String& s) const noexcept override;
        void ToStringCore(xx::String& s) const noexcept override;
        void ToBBuffer(xx::BBuffer& bb) const noexcept override;
        int FromBBuffer(xx::BBuffer& bb) noexcept override;
        int FromBBufferCore(xx::BBuffer& bb) noexcept;
        void CopyTo(log_withdrawals* const& o) const noexcept;
        log_withdrawals* MakeCopy() const noexcept;
        log_withdrawals_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
    };
    // 邮箱表
    class mail : public xx::Object
    {
    public:
        int32_t id = 0;
        // 内容
        xx::String_p content;
        // 收件人
        int32_t account_id = 0;
        // 发件人
        xx::String_p sender;
        // 创建时间
        int64_t create_time = 0;
        // 是否已读(0未读, 1已读)
        int32_t is_read = 0;

        typedef mail ThisType;
        typedef xx::Object BaseType;
	    mail(xx::MemPool* const& mempool) noexcept;
	    mail(xx::BBuffer* const& bb);
		mail(mail const&) = delete;
		mail& operator=(mail const&) = delete;
        void ToString(xx::String& s) const noexcept override;
        void ToStringCore(xx::String& s) const noexcept override;
        void ToBBuffer(xx::BBuffer& bb) const noexcept override;
        int FromBBuffer(xx::BBuffer& bb) noexcept override;
        int FromBBufferCore(xx::BBuffer& bb) noexcept;
        void CopyTo(mail* const& o) const noexcept;
        mail* MakeCopy() const noexcept;
        mail_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
    };
    // 跑马灯表
    class marquee : public xx::Object
    {
    public:
        // 自增id
        int32_t id = 0;
        // 内容
        xx::String_p content;
        // 喊话频率(多长时间循环执行一次，单位 秒)
        int32_t frequency = 0;
        // 更新时间
        int64_t update_time = 0;
        // 创建时间
        int64_t create_time = 0;
        // 是否有效(1有效)
        int32_t enable = 0;

        typedef marquee ThisType;
        typedef xx::Object BaseType;
	    marquee(xx::MemPool* const& mempool) noexcept;
	    marquee(xx::BBuffer* const& bb);
		marquee(marquee const&) = delete;
		marquee& operator=(marquee const&) = delete;
        void ToString(xx::String& s) const noexcept override;
        void ToStringCore(xx::String& s) const noexcept override;
        void ToBBuffer(xx::BBuffer& bb) const noexcept override;
        int FromBBuffer(xx::BBuffer& bb) noexcept override;
        int FromBBufferCore(xx::BBuffer& bb) noexcept;
        void CopyTo(marquee* const& o) const noexcept;
        marquee* MakeCopy() const noexcept;
        marquee_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
    };
    // 公告表
    class notice : public xx::Object
    {
    public:
        int32_t id = 0;
        // 公告内容
        xx::String_p content;
        // 公告类型
        int32_t notice_type_id = 0;
        // 有效起始时间
        std::optional<int64_t> start_time;
        // 有效结束时间
        std::optional<int64_t> end_time;
        // 创建时间
        int64_t create_time = 0;

        typedef notice ThisType;
        typedef xx::Object BaseType;
	    notice(xx::MemPool* const& mempool) noexcept;
	    notice(xx::BBuffer* const& bb);
		notice(notice const&) = delete;
		notice& operator=(notice const&) = delete;
        void ToString(xx::String& s) const noexcept override;
        void ToStringCore(xx::String& s) const noexcept override;
        void ToBBuffer(xx::BBuffer& bb) const noexcept override;
        int FromBBuffer(xx::BBuffer& bb) noexcept override;
        int FromBBufferCore(xx::BBuffer& bb) noexcept;
        void CopyTo(notice* const& o) const noexcept;
        notice* MakeCopy() const noexcept;
        notice_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
    };
    // 公告读取表
    class notice_read : public xx::Object
    {
    public:
        // 公告id
        int32_t notice_id = 0;
        // 玩家id
        int32_t account_id = 0;
        // 创建时间
        int64_t create_time = 0;

        typedef notice_read ThisType;
        typedef xx::Object BaseType;
	    notice_read(xx::MemPool* const& mempool) noexcept;
	    notice_read(xx::BBuffer* const& bb);
		notice_read(notice_read const&) = delete;
		notice_read& operator=(notice_read const&) = delete;
        void ToString(xx::String& s) const noexcept override;
        void ToStringCore(xx::String& s) const noexcept override;
        void ToBBuffer(xx::BBuffer& bb) const noexcept override;
        int FromBBuffer(xx::BBuffer& bb) noexcept override;
        int FromBBufferCore(xx::BBuffer& bb) noexcept;
        void CopyTo(notice_read* const& o) const noexcept;
        notice_read* MakeCopy() const noexcept;
        notice_read_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
    };
    // 公告类型表
    class notice_type : public xx::Object
    {
    public:
        int32_t id = 0;
        xx::String_p name;

        typedef notice_type ThisType;
        typedef xx::Object BaseType;
	    notice_type(xx::MemPool* const& mempool) noexcept;
	    notice_type(xx::BBuffer* const& bb);
		notice_type(notice_type const&) = delete;
		notice_type& operator=(notice_type const&) = delete;
        void ToString(xx::String& s) const noexcept override;
        void ToStringCore(xx::String& s) const noexcept override;
        void ToBBuffer(xx::BBuffer& bb) const noexcept override;
        int FromBBuffer(xx::BBuffer& bb) noexcept override;
        int FromBBufferCore(xx::BBuffer& bb) noexcept;
        void CopyTo(notice_type* const& o) const noexcept;
        notice_type* MakeCopy() const noexcept;
        notice_type_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
    };
    // 短信校验状态数据( 数据有有效期. 数据服定时删掉超时数据 )
    class verify_code : public xx::Object
    {
    public:
        // 收短信的电话号码
        xx::String_p phone;
        // 短信验证码内容
        xx::String_p content;
        // 下发时间( 用于判断数据是否过期, 过期就删除 )
        int64_t create_time = 0;

        typedef verify_code ThisType;
        typedef xx::Object BaseType;
	    verify_code(xx::MemPool* const& mempool) noexcept;
	    verify_code(xx::BBuffer* const& bb);
		verify_code(verify_code const&) = delete;
		verify_code& operator=(verify_code const&) = delete;
        void ToString(xx::String& s) const noexcept override;
        void ToStringCore(xx::String& s) const noexcept override;
        void ToBBuffer(xx::BBuffer& bb) const noexcept override;
        int FromBBuffer(xx::BBuffer& bb) noexcept override;
        int FromBBufferCore(xx::BBuffer& bb) noexcept;
        void CopyTo(verify_code* const& o) const noexcept;
        verify_code* MakeCopy() const noexcept;
        verify_code_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
    };
    // VIP与累计充值金额的对应表
    class vip : public xx::Object
    {
    public:
        // 唯一编号 & 级别
        int32_t id = 0;
        // 累计充值金额要求
        double total_recharge = 0;

        typedef vip ThisType;
        typedef xx::Object BaseType;
	    vip(xx::MemPool* const& mempool) noexcept;
	    vip(xx::BBuffer* const& bb);
		vip(vip const&) = delete;
		vip& operator=(vip const&) = delete;
        void ToString(xx::String& s) const noexcept override;
        void ToStringCore(xx::String& s) const noexcept override;
        void ToBBuffer(xx::BBuffer& bb) const noexcept override;
        int FromBBuffer(xx::BBuffer& bb) noexcept override;
        int FromBBufferCore(xx::BBuffer& bb) noexcept;
        void CopyTo(vip* const& o) const noexcept;
        vip* MakeCopy() const noexcept;
        vip_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
    };
    // 申请提现表
    class withdrawals : public xx::Object
    {
    public:
        // 提现订单号( 随机 9 位整数 )
        int32_t withdrawals_id = 0;
        // 帐号id
        int32_t account_id = 0;
        // 提现金额
        double money = 0;
        // 提现请求( 渠道, 账号等 )
        xx::String_p requirement;
        // 操作状态
        int32_t state_id = 0;
        // 操作描述
        xx::String_p description;
        // 提现申请时间
        int64_t create_time = 0;
        // 最后处理时间
        int64_t last_create_time = 0;

        typedef withdrawals ThisType;
        typedef xx::Object BaseType;
	    withdrawals(xx::MemPool* const& mempool) noexcept;
	    withdrawals(xx::BBuffer* const& bb);
		withdrawals(withdrawals const&) = delete;
		withdrawals& operator=(withdrawals const&) = delete;
        void ToString(xx::String& s) const noexcept override;
        void ToStringCore(xx::String& s) const noexcept override;
        void ToBBuffer(xx::BBuffer& bb) const noexcept override;
        int FromBBuffer(xx::BBuffer& bb) noexcept override;
        int FromBBufferCore(xx::BBuffer& bb) noexcept;
        void CopyTo(withdrawals* const& o) const noexcept;
        withdrawals* MakeCopy() const noexcept;
        withdrawals_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
    };
    // 密码变更历史表
    class log_account_password : public xx::Object
    {
    public:
        int32_t id = 0;
        // 帐号id
        int32_t account_id = 0;
        // 密码
        xx::String_p password;
        // 创建时间
        int64_t create_time = 0;

        typedef log_account_password ThisType;
        typedef xx::Object BaseType;
	    log_account_password(xx::MemPool* const& mempool) noexcept;
	    log_account_password(xx::BBuffer* const& bb);
		log_account_password(log_account_password const&) = delete;
		log_account_password& operator=(log_account_password const&) = delete;
        void ToString(xx::String& s) const noexcept override;
        void ToStringCore(xx::String& s) const noexcept override;
        void ToBBuffer(xx::BBuffer& bb) const noexcept override;
        int FromBBuffer(xx::BBuffer& bb) noexcept override;
        int FromBBufferCore(xx::BBuffer& bb) noexcept;
        void CopyTo(log_account_password* const& o) const noexcept;
        log_account_password* MakeCopy() const noexcept;
        log_account_password_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
    };
    // 昵称变更历史表
    class log_account_nickname : public xx::Object
    {
    public:
        int32_t id = 0;
        // 帐号id
        int32_t account_id = 0;
        // 昵称
        xx::String_p nickname;
        // 创建时间
        int64_t create_time = 0;

        typedef log_account_nickname ThisType;
        typedef xx::Object BaseType;
	    log_account_nickname(xx::MemPool* const& mempool) noexcept;
	    log_account_nickname(xx::BBuffer* const& bb);
		log_account_nickname(log_account_nickname const&) = delete;
		log_account_nickname& operator=(log_account_nickname const&) = delete;
        void ToString(xx::String& s) const noexcept override;
        void ToStringCore(xx::String& s) const noexcept override;
        void ToBBuffer(xx::BBuffer& bb) const noexcept override;
        int FromBBuffer(xx::BBuffer& bb) noexcept override;
        int FromBBufferCore(xx::BBuffer& bb) noexcept;
        void CopyTo(log_account_nickname* const& o) const noexcept;
        log_account_nickname* MakeCopy() const noexcept;
        log_account_nickname_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
    };
    // 保险箱操作日志
    class log_account_money_safe : public xx::Object
    {
    public:
        int32_t id = 0;
        // 帐号id
        int32_t account_id = 0;
        // 金额( 正数为存入保险箱, 负数为从保险箱提钱 )
        double value = 0;
        // 创建时间
        int64_t create_time = 0;

        typedef log_account_money_safe ThisType;
        typedef xx::Object BaseType;
	    log_account_money_safe(xx::MemPool* const& mempool) noexcept;
	    log_account_money_safe(xx::BBuffer* const& bb);
		log_account_money_safe(log_account_money_safe const&) = delete;
		log_account_money_safe& operator=(log_account_money_safe const&) = delete;
        void ToString(xx::String& s) const noexcept override;
        void ToStringCore(xx::String& s) const noexcept override;
        void ToBBuffer(xx::BBuffer& bb) const noexcept override;
        int FromBBuffer(xx::BBuffer& bb) noexcept override;
        int FromBBufferCore(xx::BBuffer& bb) noexcept;
        void CopyTo(log_account_money_safe* const& o) const noexcept;
        log_account_money_safe* MakeCopy() const noexcept;
        log_account_money_safe_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
    };
    // 提现状态表
    class withdrawals_state : public xx::Object
    {
    public:
        int32_t id = 0;
        xx::String_p name;

        typedef withdrawals_state ThisType;
        typedef xx::Object BaseType;
	    withdrawals_state(xx::MemPool* const& mempool) noexcept;
	    withdrawals_state(xx::BBuffer* const& bb);
		withdrawals_state(withdrawals_state const&) = delete;
		withdrawals_state& operator=(withdrawals_state const&) = delete;
        void ToString(xx::String& s) const noexcept override;
        void ToStringCore(xx::String& s) const noexcept override;
        void ToBBuffer(xx::BBuffer& bb) const noexcept override;
        int FromBBuffer(xx::BBuffer& bb) noexcept override;
        int FromBBufferCore(xx::BBuffer& bb) noexcept;
        void CopyTo(withdrawals_state* const& o) const noexcept;
        withdrawals_state* MakeCopy() const noexcept;
        withdrawals_state_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
    };
}
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
    class FooEx : public PKG::Foo
    {
    public:

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
namespace Tables
{
    // 玩家账号表( 基表 )
    class account : public xx::Object
    {
    public:
        // 用户Id ( 随机 8 位整数 )
        int32_t id = 0;
        // 原始用户名 唯一( GUID )
        xx::String_p username;
        // 昵称 唯一( 默认用某种规则生成 )
        xx::String_p nickname;
        // 头像
        int32_t avatar_id = 0;
        // 电话号码 唯一( 默认填充 username GUID )
        xx::String_p phone;
        // 密码( 默认为空 )
        xx::String_p password;
        // 账户余额( 保留4位小数位, 进部分游戏时会被清0, 结束时会兑换返还 )
        double money = 0;
        // 保险箱( 玩家可在账户余额间搬运数据 )
        double money_safe = 0;
        // 累计充值金额
        double total_recharge = 0;
        // 累计消费金额( 比如在鱼里一共打了多少钱的炮 )
        double total_consume = 0;
        // 累计提现金额
        double total_withdraw = 0;
        // 创建时间. epoch 10m 精度. 所有表的这个字段都是这个格式
        int64_t create_time = 0;
        // 启用标记( 1 : 启用 )
        int32_t enabled = 0;
        // 玩家进入大厅时间(为空 则离开大厅)
        std::optional<int64_t> enter_lobby_time;
        // 正在游戏中的玩家日志id(为空 则离开游戏)
        std::optional<int32_t> log_game_id;

        typedef account ThisType;
        typedef xx::Object BaseType;
	    account(xx::MemPool* const& mempool) noexcept;
	    account(xx::BBuffer* const& bb);
		account(account const&) = delete;
		account& operator=(account const&) = delete;
        void ToString(xx::String& s) const noexcept override;
        void ToStringCore(xx::String& s) const noexcept override;
        void ToBBuffer(xx::BBuffer& bb) const noexcept override;
        int FromBBuffer(xx::BBuffer& bb) noexcept override;
        int FromBBufferCore(xx::BBuffer& bb) noexcept;
        void CopyTo(account* const& o) const noexcept;
        account* MakeCopy() const noexcept;
        account_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
    };
    // 在线聊天
    class chat : public xx::Object
    {
    public:
        int32_t id = 0;
        // 内容
        xx::String_p content;
        // 聊天人id
        int32_t account_id = 0;
        // 创建时间
        int64_t create_time = 0;

        typedef chat ThisType;
        typedef xx::Object BaseType;
	    chat(xx::MemPool* const& mempool) noexcept;
	    chat(xx::BBuffer* const& bb);
		chat(chat const&) = delete;
		chat& operator=(chat const&) = delete;
        void ToString(xx::String& s) const noexcept override;
        void ToStringCore(xx::String& s) const noexcept override;
        void ToBBuffer(xx::BBuffer& bb) const noexcept override;
        int FromBBuffer(xx::BBuffer& bb) noexcept override;
        int FromBBufferCore(xx::BBuffer& bb) noexcept;
        void CopyTo(chat* const& o) const noexcept;
        chat* MakeCopy() const noexcept;
        chat_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
    };
}
    class Node : public PKG::Tables::node
    {
    public:
        PKG::Node_p parent;
        xx::List_p<PKG::Node_p> childs;

        typedef Node ThisType;
        typedef PKG::Tables::node BaseType;
	    Node(xx::MemPool* const& mempool) noexcept;
	    Node(xx::BBuffer* const& bb);
		Node(Node const&) = delete;
		Node& operator=(Node const&) = delete;
        void ToString(xx::String& s) const noexcept override;
        void ToStringCore(xx::String& s) const noexcept override;
        void ToBBuffer(xx::BBuffer& bb) const noexcept override;
        int FromBBuffer(xx::BBuffer& bb) noexcept override;
        int FromBBufferCore(xx::BBuffer& bb) noexcept;
        void CopyTo(Node* const& o) const noexcept;
        Node* MakeCopy() const noexcept;
        Node_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
    };
namespace Tables
{
    // 通用配置表( 带版本号的键值对集合 )
    class config : public xx::Object
    {
    public:
        // 键名
        xx::String_p key;
        // 数据类型名
        xx::String_p datatype_name;
        // 显示名
        xx::String_p title;
        // 说明
        xx::String_p desc;
        // 创建时间
        int64_t create_time = 0;

        typedef config ThisType;
        typedef xx::Object BaseType;
	    config(xx::MemPool* const& mempool) noexcept;
	    config(xx::BBuffer* const& bb);
		config(config const&) = delete;
		config& operator=(config const&) = delete;
        void ToString(xx::String& s) const noexcept override;
        void ToStringCore(xx::String& s) const noexcept override;
        void ToBBuffer(xx::BBuffer& bb) const noexcept override;
        int FromBBuffer(xx::BBuffer& bb) noexcept override;
        int FromBBufferCore(xx::BBuffer& bb) noexcept;
        void CopyTo(config* const& o) const noexcept;
        config* MakeCopy() const noexcept;
        config_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
    };
    // 数据类型列表( 主要供 config 用 )
    class datatype : public xx::Object
    {
    public:
        xx::String_p name;

        typedef datatype ThisType;
        typedef xx::Object BaseType;
	    datatype(xx::MemPool* const& mempool) noexcept;
	    datatype(xx::BBuffer* const& bb);
		datatype(datatype const&) = delete;
		datatype& operator=(datatype const&) = delete;
        void ToString(xx::String& s) const noexcept override;
        void ToStringCore(xx::String& s) const noexcept override;
        void ToBBuffer(xx::BBuffer& bb) const noexcept override;
        int FromBBuffer(xx::BBuffer& bb) noexcept override;
        int FromBBufferCore(xx::BBuffer& bb) noexcept;
        void CopyTo(datatype* const& o) const noexcept;
        datatype* MakeCopy() const noexcept;
        datatype_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
    };
    // 鱼种类表
    class fish : public xx::Object
    {
    public:
        // 鱼配置id
        int32_t id = 0;
        // 名称
        xx::String_p name;
        // 打死鱼的金币所得最小基数( 也可理解成倍率 )
        int32_t minCoin = 0;
        //  打死鱼的金币所得最大基数( 也可理解成倍率 )
        int32_t maxCoin = 0;

        typedef fish ThisType;
        typedef xx::Object BaseType;
	    fish(xx::MemPool* const& mempool) noexcept;
	    fish(xx::BBuffer* const& bb);
		fish(fish const&) = delete;
		fish& operator=(fish const&) = delete;
        void ToString(xx::String& s) const noexcept override;
        void ToStringCore(xx::String& s) const noexcept override;
        void ToBBuffer(xx::BBuffer& bb) const noexcept override;
        int FromBBuffer(xx::BBuffer& bb) noexcept override;
        int FromBBufferCore(xx::BBuffer& bb) noexcept;
        void CopyTo(fish* const& o) const noexcept;
        fish* MakeCopy() const noexcept;
        fish_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
    };
    // 所有游戏列表( 基表 )
    class game : public xx::Object
    {
    public:
        // 游戏编号( 非自增, 直接填死 )
        int32_t id = 0;
        // 游戏名
        xx::String_p name;
        // 类型名( 派生表名 )
        xx::String_p type_name;
        // 是否启用 1: true
        int32_t enabled = 0;

        typedef game ThisType;
        typedef xx::Object BaseType;
	    game(xx::MemPool* const& mempool) noexcept;
	    game(xx::BBuffer* const& bb);
		game(game const&) = delete;
		game& operator=(game const&) = delete;
        void ToString(xx::String& s) const noexcept override;
        void ToStringCore(xx::String& s) const noexcept override;
        void ToBBuffer(xx::BBuffer& bb) const noexcept override;
        int FromBBuffer(xx::BBuffer& bb) noexcept override;
        int FromBBufferCore(xx::BBuffer& bb) noexcept;
        void CopyTo(game* const& o) const noexcept;
        game* MakeCopy() const noexcept;
        game_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
    };
    // game 的 捕鱼游戏 派生表( 存放专属字段 )
    class game_catchfish : public xx::Object
    {
    public:
        // 基表游戏编号
        int32_t id = 0;
        // 进出游戏时 money 自动兑换成 coin 要 乘除 的系数
        int32_t exchange_coin_ratio = 0;

        typedef game_catchfish ThisType;
        typedef xx::Object BaseType;
	    game_catchfish(xx::MemPool* const& mempool) noexcept;
	    game_catchfish(xx::BBuffer* const& bb);
		game_catchfish(game_catchfish const&) = delete;
		game_catchfish& operator=(game_catchfish const&) = delete;
        void ToString(xx::String& s) const noexcept override;
        void ToStringCore(xx::String& s) const noexcept override;
        void ToBBuffer(xx::BBuffer& bb) const noexcept override;
        int FromBBuffer(xx::BBuffer& bb) noexcept override;
        int FromBBufferCore(xx::BBuffer& bb) noexcept;
        void CopyTo(game_catchfish* const& o) const noexcept;
        game_catchfish* MakeCopy() const noexcept;
        game_catchfish_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
    };
    // 玩家于捕鱼游戏的个人配置信息
    class game_catchfish_account : public xx::Object
    {
    public:
        // 账号id
        int32_t account_id = 0;
        // 捕鱼游戏id
        int32_t game_id = 0;
        // 级别id
        int32_t level_id = 0;
        // 玩家上次游戏退出前用的子弹的倍率( 游戏内金币整数值 )
        int64_t last_bet = 0;
        // 玩家上次游戏退出前用的炮台id
        int32_t last_cannon_id = 0;

        typedef game_catchfish_account ThisType;
        typedef xx::Object BaseType;
	    game_catchfish_account(xx::MemPool* const& mempool) noexcept;
	    game_catchfish_account(xx::BBuffer* const& bb);
		game_catchfish_account(game_catchfish_account const&) = delete;
		game_catchfish_account& operator=(game_catchfish_account const&) = delete;
        void ToString(xx::String& s) const noexcept override;
        void ToStringCore(xx::String& s) const noexcept override;
        void ToBBuffer(xx::BBuffer& bb) const noexcept override;
        int FromBBuffer(xx::BBuffer& bb) noexcept override;
        int FromBBufferCore(xx::BBuffer& bb) noexcept;
        void CopyTo(game_catchfish_account* const& o) const noexcept;
        game_catchfish_account* MakeCopy() const noexcept;
        game_catchfish_account_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
    };
    // 捕鱼游戏内炮台skin与累计充值金额的对应表
    class game_catchfish_cannon : public xx::Object
    {
    public:
        // 唯一编号 & 炮台Skin
        int32_t id = 0;
        // VIP等级
        int32_t vip_id = 0;

        typedef game_catchfish_cannon ThisType;
        typedef xx::Object BaseType;
	    game_catchfish_cannon(xx::MemPool* const& mempool) noexcept;
	    game_catchfish_cannon(xx::BBuffer* const& bb);
		game_catchfish_cannon(game_catchfish_cannon const&) = delete;
		game_catchfish_cannon& operator=(game_catchfish_cannon const&) = delete;
        void ToString(xx::String& s) const noexcept override;
        void ToStringCore(xx::String& s) const noexcept override;
        void ToBBuffer(xx::BBuffer& bb) const noexcept override;
        int FromBBuffer(xx::BBuffer& bb) noexcept override;
        int FromBBufferCore(xx::BBuffer& bb) noexcept;
        void CopyTo(game_catchfish_cannon* const& o) const noexcept;
        game_catchfish_cannon* MakeCopy() const noexcept;
        game_catchfish_cannon_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
    };
    // 捕鱼游戏的分级表( 1对多 )
    class game_catchfish_level : public xx::Object
    {
    public:
        // 游戏id
        int32_t game_id = 0;
        // 游戏级别id. 0 1 2....: 新手厅  进阶厅  高手厅 ....
        int32_t level_id = 0;
        // 炮值(从)(游戏内金币)
        int64_t min_bet = 0;
        // 炮值(到)(游戏内金币)
        int64_t max_bet = 0;
        // 最低准入金额
        double min_money = 0;
        // 是否启用 1: true
        int32_t enabled = 0;

        typedef game_catchfish_level ThisType;
        typedef xx::Object BaseType;
	    game_catchfish_level(xx::MemPool* const& mempool) noexcept;
	    game_catchfish_level(xx::BBuffer* const& bb);
		game_catchfish_level(game_catchfish_level const&) = delete;
		game_catchfish_level& operator=(game_catchfish_level const&) = delete;
        void ToString(xx::String& s) const noexcept override;
        void ToStringCore(xx::String& s) const noexcept override;
        void ToBBuffer(xx::BBuffer& bb) const noexcept override;
        int FromBBuffer(xx::BBuffer& bb) noexcept override;
        int FromBBufferCore(xx::BBuffer& bb) noexcept;
        void CopyTo(game_catchfish_level* const& o) const noexcept;
        game_catchfish_level* MakeCopy() const noexcept;
        game_catchfish_level_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
    };
    // 捕鱼游戏的分级表 下属房间表( 1对多 )
    class game_catchfish_level_room : public xx::Object
    {
    public:
        // 游戏id
        int32_t game_id = 0;
        // 游戏级别id
        int32_t level_id = 0;
        // 房号( 从 1 开始填充, 新增取最大值+1 )
        int32_t room_id = 0;
        // 是否启用 1: true
        int32_t enabled = 0;

        typedef game_catchfish_level_room ThisType;
        typedef xx::Object BaseType;
	    game_catchfish_level_room(xx::MemPool* const& mempool) noexcept;
	    game_catchfish_level_room(xx::BBuffer* const& bb);
		game_catchfish_level_room(game_catchfish_level_room const&) = delete;
		game_catchfish_level_room& operator=(game_catchfish_level_room const&) = delete;
        void ToString(xx::String& s) const noexcept override;
        void ToStringCore(xx::String& s) const noexcept override;
        void ToBBuffer(xx::BBuffer& bb) const noexcept override;
        int FromBBuffer(xx::BBuffer& bb) noexcept override;
        int FromBBufferCore(xx::BBuffer& bb) noexcept;
        void CopyTo(game_catchfish_level_room* const& o) const noexcept;
        game_catchfish_level_room* MakeCopy() const noexcept;
        game_catchfish_level_room_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
    };
    // 头像变更历史表
    class log_account_avatar : public xx::Object
    {
    public:
        int32_t id = 0;
        // 帐号id
        int32_t account_id = 0;
        // 头像
        int32_t avatar_id = 0;
        // 创建时间
        int64_t create_time = 0;

        typedef log_account_avatar ThisType;
        typedef xx::Object BaseType;
	    log_account_avatar(xx::MemPool* const& mempool) noexcept;
	    log_account_avatar(xx::BBuffer* const& bb);
		log_account_avatar(log_account_avatar const&) = delete;
		log_account_avatar& operator=(log_account_avatar const&) = delete;
        void ToString(xx::String& s) const noexcept override;
        void ToStringCore(xx::String& s) const noexcept override;
        void ToBBuffer(xx::BBuffer& bb) const noexcept override;
        int FromBBuffer(xx::BBuffer& bb) noexcept override;
        int FromBBufferCore(xx::BBuffer& bb) noexcept;
        void CopyTo(log_account_avatar* const& o) const noexcept;
        log_account_avatar* MakeCopy() const noexcept;
        log_account_avatar_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
    };
    // 配置项所对应的值( 保留历史变化, 通常只有最新的那一个生效 )
    class config_value : public xx::Object
    {
    public:
        // 键名
        xx::String_p key;
        // 对应 bigint 数据类型的值( int64 )
        std::optional<int64_t> value_bigint;
        // 对应 double 数据类型的值
        std::optional<double> value_double;
        // 对应 varchar 数据类型的值( string )
        xx::String_p value_varchar;
        // 创建时间
        int64_t create_time = 0;

        typedef config_value ThisType;
        typedef xx::Object BaseType;
	    config_value(xx::MemPool* const& mempool) noexcept;
	    config_value(xx::BBuffer* const& bb);
		config_value(config_value const&) = delete;
		config_value& operator=(config_value const&) = delete;
        void ToString(xx::String& s) const noexcept override;
        void ToStringCore(xx::String& s) const noexcept override;
        void ToBBuffer(xx::BBuffer& bb) const noexcept override;
        int FromBBuffer(xx::BBuffer& bb) noexcept override;
        int FromBBufferCore(xx::BBuffer& bb) noexcept;
        void CopyTo(config_value* const& o) const noexcept;
        config_value* MakeCopy() const noexcept;
        config_value_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
    };
    // 帐号启用标记变更历史表
    class log_account_enabled : public xx::Object
    {
    public:
        int32_t id = 0;
        // 帐号id
        int32_t account_id = 0;
        // 启用标记
        int32_t enabled = 0;
        // 备注( 含修改人 & 原因 )
        xx::String_p memo;
        // 日志创建时间( 修改时间 )
        int64_t create_time = 0;

        typedef log_account_enabled ThisType;
        typedef xx::Object BaseType;
	    log_account_enabled(xx::MemPool* const& mempool) noexcept;
	    log_account_enabled(xx::BBuffer* const& bb);
		log_account_enabled(log_account_enabled const&) = delete;
		log_account_enabled& operator=(log_account_enabled const&) = delete;
        void ToString(xx::String& s) const noexcept override;
        void ToStringCore(xx::String& s) const noexcept override;
        void ToBBuffer(xx::BBuffer& bb) const noexcept override;
        int FromBBuffer(xx::BBuffer& bb) noexcept override;
        int FromBBufferCore(xx::BBuffer& bb) noexcept;
        void CopyTo(log_account_enabled* const& o) const noexcept;
        log_account_enabled* MakeCopy() const noexcept;
        log_account_enabled_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
    };
}
}
namespace xx
{
	template<> struct TypeId<PKG::Foo> { static const uint16_t value = 3; };
	template<> struct TypeId<xx::List<PKG::Foo_p>> { static const uint16_t value = 4; };
	template<> struct TypeId<xx::List<xx::Object_p>> { static const uint16_t value = 5; };
	template<> struct TypeId<PKG::FooEx> { static const uint16_t value = 6; };
	template<> struct TypeId<PKG::Node> { static const uint16_t value = 7; };
	template<> struct TypeId<PKG::Tables::node> { static const uint16_t value = 8; };
	template<> struct TypeId<xx::List<PKG::Node_p>> { static const uint16_t value = 9; };
	template<> struct TypeId<PKG::DataSet> { static const uint16_t value = 10; };
	template<> struct TypeId<xx::List<PKG::Table_p>> { static const uint16_t value = 11; };
	template<> struct TypeId<PKG::Table> { static const uint16_t value = 12; };
	template<> struct TypeId<xx::List<PKG::TableColumn_p>> { static const uint16_t value = 13; };
	template<> struct TypeId<PKG::TableColumn> { static const uint16_t value = 14; };
	template<> struct TypeId<xx::List<PKG::TableRow_p>> { static const uint16_t value = 15; };
	template<> struct TypeId<PKG::TableRow> { static const uint16_t value = 16; };
	template<> struct TypeId<xx::List<PKG::TableRowValue_p>> { static const uint16_t value = 17; };
	template<> struct TypeId<PKG::TableRowValue> { static const uint16_t value = 18; };
	template<> struct TypeId<PKG::TableRowValue_Int> { static const uint16_t value = 19; };
	template<> struct TypeId<PKG::TableRowValue_NullableInt> { static const uint16_t value = 20; };
	template<> struct TypeId<PKG::TableRowValue_String> { static const uint16_t value = 21; };
	template<> struct TypeId<PKG::Tables::account> { static const uint16_t value = 22; };
	template<> struct TypeId<PKG::Tables::chat> { static const uint16_t value = 23; };
	template<> struct TypeId<PKG::Tables::config> { static const uint16_t value = 24; };
	template<> struct TypeId<PKG::Tables::config_value> { static const uint16_t value = 25; };
	template<> struct TypeId<PKG::Tables::datatype> { static const uint16_t value = 26; };
	template<> struct TypeId<PKG::Tables::fish> { static const uint16_t value = 27; };
	template<> struct TypeId<PKG::Tables::game> { static const uint16_t value = 28; };
	template<> struct TypeId<PKG::Tables::game_catchfish> { static const uint16_t value = 29; };
	template<> struct TypeId<PKG::Tables::game_catchfish_account> { static const uint16_t value = 30; };
	template<> struct TypeId<PKG::Tables::game_catchfish_cannon> { static const uint16_t value = 31; };
	template<> struct TypeId<PKG::Tables::game_catchfish_level> { static const uint16_t value = 32; };
	template<> struct TypeId<PKG::Tables::game_catchfish_level_room> { static const uint16_t value = 33; };
	template<> struct TypeId<PKG::Tables::log_account_avatar> { static const uint16_t value = 34; };
	template<> struct TypeId<PKG::Tables::log_account_enabled> { static const uint16_t value = 35; };
	template<> struct TypeId<PKG::Tables::log_account_money_safe> { static const uint16_t value = 36; };
	template<> struct TypeId<PKG::Tables::log_account_nickname> { static const uint16_t value = 37; };
	template<> struct TypeId<PKG::Tables::log_account_password> { static const uint16_t value = 38; };
	template<> struct TypeId<PKG::Tables::log_account_phone> { static const uint16_t value = 39; };
	template<> struct TypeId<PKG::Tables::log_account_recharge> { static const uint16_t value = 40; };
	template<> struct TypeId<PKG::Tables::log_game> { static const uint16_t value = 41; };
	template<> struct TypeId<PKG::Tables::log_game_catchfish> { static const uint16_t value = 42; };
	template<> struct TypeId<PKG::Tables::log_game_catchfish_event> { static const uint16_t value = 43; };
	template<> struct TypeId<PKG::Tables::log_game_catchfish_event_bulletdie> { static const uint16_t value = 44; };
	template<> struct TypeId<PKG::Tables::log_game_catchfish_event_bulletmiss> { static const uint16_t value = 45; };
	template<> struct TypeId<PKG::Tables::log_game_catchfish_event_fire> { static const uint16_t value = 46; };
	template<> struct TypeId<PKG::Tables::log_game_catchfish_event_fishdie> { static const uint16_t value = 47; };
	template<> struct TypeId<PKG::Tables::log_login_out> { static const uint16_t value = 48; };
	template<> struct TypeId<PKG::Tables::log_withdrawals> { static const uint16_t value = 49; };
	template<> struct TypeId<PKG::Tables::mail> { static const uint16_t value = 50; };
	template<> struct TypeId<PKG::Tables::marquee> { static const uint16_t value = 51; };
	template<> struct TypeId<PKG::Tables::notice> { static const uint16_t value = 52; };
	template<> struct TypeId<PKG::Tables::notice_read> { static const uint16_t value = 53; };
	template<> struct TypeId<PKG::Tables::notice_type> { static const uint16_t value = 54; };
	template<> struct TypeId<PKG::Tables::verify_code> { static const uint16_t value = 55; };
	template<> struct TypeId<PKG::Tables::vip> { static const uint16_t value = 56; };
	template<> struct TypeId<PKG::Tables::withdrawals> { static const uint16_t value = 57; };
	template<> struct TypeId<PKG::Tables::withdrawals_state> { static const uint16_t value = 58; };
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
        bb.Write(this->o);
        bb.Write(this->os);
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
        if (int r = bb.Read(this->o)) return r;
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->os)) return r;
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
        s.Append(", \"o\":", this->o);
        s.Append(", \"os\":", this->os);
    }
    inline void Foo::CopyTo(Foo* const& o) const noexcept
    {
        o->id = this->id;
        o->age = this->age;
        o->info = this->info;
        o->childs = this->childs;
        o->o = this->o;
        o->os = this->os;
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
    }
    inline int FooEx::FromBBuffer(xx::BBuffer& bb) noexcept
    {
        if (int r = this->BaseType::FromBBuffer(bb)) return r;
        return this->FromBBufferCore(bb);
    }
    inline int FooEx::FromBBufferCore(xx::BBuffer& bb) noexcept
    {
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
    }
    inline void FooEx::CopyTo(FooEx* const& o) const noexcept
    {
        this->BaseType::CopyTo(o);
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

	inline Node::Node(xx::MemPool* const& mempool) noexcept
        : PKG::Tables::node(mempool)
	{
	}
	inline Node::Node(xx::BBuffer* const& bb)
        : PKG::Tables::node(bb)
	{
        if (int r = FromBBufferCore(*bb)) throw r;
	}
    inline void Node::ToBBuffer(xx::BBuffer& bb) const noexcept
    {
        this->BaseType::ToBBuffer(bb);
        bb.Write(this->parent);
        bb.Write(this->childs);
    }
    inline int Node::FromBBuffer(xx::BBuffer& bb) noexcept
    {
        if (int r = this->BaseType::FromBBuffer(bb)) return r;
        return this->FromBBufferCore(bb);
    }
    inline int Node::FromBBufferCore(xx::BBuffer& bb) noexcept
    {
        if (int r = bb.Read(this->parent)) return r;
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->childs)) return r;
        return 0;
    }

    inline void Node::ToString(xx::String& s) const noexcept
    {
        if (this->memHeader().flags)
        {
        	s.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else this->memHeader().flags = 1;

        s.Append("{ \"pkgTypeName\":\"Node\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(s);
        s.Append(" }");
        
        this->memHeader().flags = 0;
    }
    inline void Node::ToStringCore(xx::String& s) const noexcept
    {
        this->BaseType::ToStringCore(s);
        s.Append(", \"parent\":", this->parent);
        s.Append(", \"childs\":", this->childs);
    }
    inline void Node::CopyTo(Node* const& o) const noexcept
    {
        this->BaseType::CopyTo(o);
        o->parent = this->parent;
        o->childs = this->childs;
    }
    inline Node* Node::MakeCopy() const noexcept
    {
        auto o = mempool->MPCreate<Node>();
        this->CopyTo(o);
        return o;
    }
    inline Node_p Node::MakePtrCopy() const noexcept
    {
        return Node_p(this->MakeCopy());
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

namespace Tables
{
	inline account::account(xx::MemPool* const& mempool) noexcept
        : xx::Object(mempool)
	{
	}
	inline account::account(xx::BBuffer* const& bb)
        : xx::Object(bb)
	{
        if (int r = FromBBufferCore(*bb)) throw r;
	}
    inline void account::ToBBuffer(xx::BBuffer& bb) const noexcept
    {
        bb.Write(this->id);
        bb.Write(this->username);
        bb.Write(this->nickname);
        bb.Write(this->avatar_id);
        bb.Write(this->phone);
        bb.Write(this->password);
        bb.Write(this->money);
        bb.Write(this->money_safe);
        bb.Write(this->total_recharge);
        bb.Write(this->total_consume);
        bb.Write(this->total_withdraw);
        bb.Write(this->create_time);
        bb.Write(this->enabled);
        bb.Write(this->enter_lobby_time);
        bb.Write(this->log_game_id);
    }
    inline int account::FromBBuffer(xx::BBuffer& bb) noexcept
    {
        return this->FromBBufferCore(bb);
    }
    inline int account::FromBBufferCore(xx::BBuffer& bb) noexcept
    {
        if (int r = bb.Read(this->id)) return r;
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->username)) return r;
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->nickname)) return r;
        if (int r = bb.Read(this->avatar_id)) return r;
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->phone)) return r;
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->password)) return r;
        if (int r = bb.Read(this->money)) return r;
        if (int r = bb.Read(this->money_safe)) return r;
        if (int r = bb.Read(this->total_recharge)) return r;
        if (int r = bb.Read(this->total_consume)) return r;
        if (int r = bb.Read(this->total_withdraw)) return r;
        if (int r = bb.Read(this->create_time)) return r;
        if (int r = bb.Read(this->enabled)) return r;
        if (int r = bb.Read(this->enter_lobby_time)) return r;
        if (int r = bb.Read(this->log_game_id)) return r;
        return 0;
    }

    inline void account::ToString(xx::String& s) const noexcept
    {
        if (this->memHeader().flags)
        {
        	s.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else this->memHeader().flags = 1;

        s.Append("{ \"pkgTypeName\":\"Tables.account\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(s);
        s.Append(" }");
        
        this->memHeader().flags = 0;
    }
    inline void account::ToStringCore(xx::String& s) const noexcept
    {
        this->BaseType::ToStringCore(s);
        s.Append(", \"id\":", this->id);
        if (this->username) s.Append(", \"username\":\"", this->username, "\"");
        else s.Append(", \"username\":nil");
        if (this->nickname) s.Append(", \"nickname\":\"", this->nickname, "\"");
        else s.Append(", \"nickname\":nil");
        s.Append(", \"avatar_id\":", this->avatar_id);
        if (this->phone) s.Append(", \"phone\":\"", this->phone, "\"");
        else s.Append(", \"phone\":nil");
        if (this->password) s.Append(", \"password\":\"", this->password, "\"");
        else s.Append(", \"password\":nil");
        s.Append(", \"money\":", this->money);
        s.Append(", \"money_safe\":", this->money_safe);
        s.Append(", \"total_recharge\":", this->total_recharge);
        s.Append(", \"total_consume\":", this->total_consume);
        s.Append(", \"total_withdraw\":", this->total_withdraw);
        s.Append(", \"create_time\":", this->create_time);
        s.Append(", \"enabled\":", this->enabled);
        s.Append(", \"enter_lobby_time\":", this->enter_lobby_time);
        s.Append(", \"log_game_id\":", this->log_game_id);
    }
    inline void account::CopyTo(account* const& o) const noexcept
    {
        o->id = this->id;
        o->username = this->username;
        o->nickname = this->nickname;
        o->avatar_id = this->avatar_id;
        o->phone = this->phone;
        o->password = this->password;
        o->money = this->money;
        o->money_safe = this->money_safe;
        o->total_recharge = this->total_recharge;
        o->total_consume = this->total_consume;
        o->total_withdraw = this->total_withdraw;
        o->create_time = this->create_time;
        o->enabled = this->enabled;
        o->enter_lobby_time = this->enter_lobby_time;
        o->log_game_id = this->log_game_id;
    }
    inline account* account::MakeCopy() const noexcept
    {
        auto o = mempool->MPCreate<account>();
        this->CopyTo(o);
        return o;
    }
    inline account_p account::MakePtrCopy() const noexcept
    {
        return account_p(this->MakeCopy());
    }

	inline chat::chat(xx::MemPool* const& mempool) noexcept
        : xx::Object(mempool)
	{
	}
	inline chat::chat(xx::BBuffer* const& bb)
        : xx::Object(bb)
	{
        if (int r = FromBBufferCore(*bb)) throw r;
	}
    inline void chat::ToBBuffer(xx::BBuffer& bb) const noexcept
    {
        bb.Write(this->id);
        bb.Write(this->content);
        bb.Write(this->account_id);
        bb.Write(this->create_time);
    }
    inline int chat::FromBBuffer(xx::BBuffer& bb) noexcept
    {
        return this->FromBBufferCore(bb);
    }
    inline int chat::FromBBufferCore(xx::BBuffer& bb) noexcept
    {
        if (int r = bb.Read(this->id)) return r;
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->content)) return r;
        if (int r = bb.Read(this->account_id)) return r;
        if (int r = bb.Read(this->create_time)) return r;
        return 0;
    }

    inline void chat::ToString(xx::String& s) const noexcept
    {
        if (this->memHeader().flags)
        {
        	s.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else this->memHeader().flags = 1;

        s.Append("{ \"pkgTypeName\":\"Tables.chat\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(s);
        s.Append(" }");
        
        this->memHeader().flags = 0;
    }
    inline void chat::ToStringCore(xx::String& s) const noexcept
    {
        this->BaseType::ToStringCore(s);
        s.Append(", \"id\":", this->id);
        if (this->content) s.Append(", \"content\":\"", this->content, "\"");
        else s.Append(", \"content\":nil");
        s.Append(", \"account_id\":", this->account_id);
        s.Append(", \"create_time\":", this->create_time);
    }
    inline void chat::CopyTo(chat* const& o) const noexcept
    {
        o->id = this->id;
        o->content = this->content;
        o->account_id = this->account_id;
        o->create_time = this->create_time;
    }
    inline chat* chat::MakeCopy() const noexcept
    {
        auto o = mempool->MPCreate<chat>();
        this->CopyTo(o);
        return o;
    }
    inline chat_p chat::MakePtrCopy() const noexcept
    {
        return chat_p(this->MakeCopy());
    }

	inline config::config(xx::MemPool* const& mempool) noexcept
        : xx::Object(mempool)
	{
	}
	inline config::config(xx::BBuffer* const& bb)
        : xx::Object(bb)
	{
        if (int r = FromBBufferCore(*bb)) throw r;
	}
    inline void config::ToBBuffer(xx::BBuffer& bb) const noexcept
    {
        bb.Write(this->key);
        bb.Write(this->datatype_name);
        bb.Write(this->title);
        bb.Write(this->desc);
        bb.Write(this->create_time);
    }
    inline int config::FromBBuffer(xx::BBuffer& bb) noexcept
    {
        return this->FromBBufferCore(bb);
    }
    inline int config::FromBBufferCore(xx::BBuffer& bb) noexcept
    {
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->key)) return r;
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->datatype_name)) return r;
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->title)) return r;
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->desc)) return r;
        if (int r = bb.Read(this->create_time)) return r;
        return 0;
    }

    inline void config::ToString(xx::String& s) const noexcept
    {
        if (this->memHeader().flags)
        {
        	s.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else this->memHeader().flags = 1;

        s.Append("{ \"pkgTypeName\":\"Tables.config\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(s);
        s.Append(" }");
        
        this->memHeader().flags = 0;
    }
    inline void config::ToStringCore(xx::String& s) const noexcept
    {
        this->BaseType::ToStringCore(s);
        if (this->key) s.Append(", \"key\":\"", this->key, "\"");
        else s.Append(", \"key\":nil");
        if (this->datatype_name) s.Append(", \"datatype_name\":\"", this->datatype_name, "\"");
        else s.Append(", \"datatype_name\":nil");
        if (this->title) s.Append(", \"title\":\"", this->title, "\"");
        else s.Append(", \"title\":nil");
        if (this->desc) s.Append(", \"desc\":\"", this->desc, "\"");
        else s.Append(", \"desc\":nil");
        s.Append(", \"create_time\":", this->create_time);
    }
    inline void config::CopyTo(config* const& o) const noexcept
    {
        o->key = this->key;
        o->datatype_name = this->datatype_name;
        o->title = this->title;
        o->desc = this->desc;
        o->create_time = this->create_time;
    }
    inline config* config::MakeCopy() const noexcept
    {
        auto o = mempool->MPCreate<config>();
        this->CopyTo(o);
        return o;
    }
    inline config_p config::MakePtrCopy() const noexcept
    {
        return config_p(this->MakeCopy());
    }

	inline config_value::config_value(xx::MemPool* const& mempool) noexcept
        : xx::Object(mempool)
	{
	}
	inline config_value::config_value(xx::BBuffer* const& bb)
        : xx::Object(bb)
	{
        if (int r = FromBBufferCore(*bb)) throw r;
	}
    inline void config_value::ToBBuffer(xx::BBuffer& bb) const noexcept
    {
        bb.Write(this->key);
        bb.Write(this->value_bigint);
        bb.Write(this->value_double);
        bb.Write(this->value_varchar);
        bb.Write(this->create_time);
    }
    inline int config_value::FromBBuffer(xx::BBuffer& bb) noexcept
    {
        return this->FromBBufferCore(bb);
    }
    inline int config_value::FromBBufferCore(xx::BBuffer& bb) noexcept
    {
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->key)) return r;
        if (int r = bb.Read(this->value_bigint)) return r;
        if (int r = bb.Read(this->value_double)) return r;
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->value_varchar)) return r;
        if (int r = bb.Read(this->create_time)) return r;
        return 0;
    }

    inline void config_value::ToString(xx::String& s) const noexcept
    {
        if (this->memHeader().flags)
        {
        	s.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else this->memHeader().flags = 1;

        s.Append("{ \"pkgTypeName\":\"Tables.config_value\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(s);
        s.Append(" }");
        
        this->memHeader().flags = 0;
    }
    inline void config_value::ToStringCore(xx::String& s) const noexcept
    {
        this->BaseType::ToStringCore(s);
        if (this->key) s.Append(", \"key\":\"", this->key, "\"");
        else s.Append(", \"key\":nil");
        s.Append(", \"value_bigint\":", this->value_bigint);
        s.Append(", \"value_double\":", this->value_double);
        if (this->value_varchar) s.Append(", \"value_varchar\":\"", this->value_varchar, "\"");
        else s.Append(", \"value_varchar\":nil");
        s.Append(", \"create_time\":", this->create_time);
    }
    inline void config_value::CopyTo(config_value* const& o) const noexcept
    {
        o->key = this->key;
        o->value_bigint = this->value_bigint;
        o->value_double = this->value_double;
        o->value_varchar = this->value_varchar;
        o->create_time = this->create_time;
    }
    inline config_value* config_value::MakeCopy() const noexcept
    {
        auto o = mempool->MPCreate<config_value>();
        this->CopyTo(o);
        return o;
    }
    inline config_value_p config_value::MakePtrCopy() const noexcept
    {
        return config_value_p(this->MakeCopy());
    }

	inline datatype::datatype(xx::MemPool* const& mempool) noexcept
        : xx::Object(mempool)
	{
	}
	inline datatype::datatype(xx::BBuffer* const& bb)
        : xx::Object(bb)
	{
        if (int r = FromBBufferCore(*bb)) throw r;
	}
    inline void datatype::ToBBuffer(xx::BBuffer& bb) const noexcept
    {
        bb.Write(this->name);
    }
    inline int datatype::FromBBuffer(xx::BBuffer& bb) noexcept
    {
        return this->FromBBufferCore(bb);
    }
    inline int datatype::FromBBufferCore(xx::BBuffer& bb) noexcept
    {
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->name)) return r;
        return 0;
    }

    inline void datatype::ToString(xx::String& s) const noexcept
    {
        if (this->memHeader().flags)
        {
        	s.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else this->memHeader().flags = 1;

        s.Append("{ \"pkgTypeName\":\"Tables.datatype\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(s);
        s.Append(" }");
        
        this->memHeader().flags = 0;
    }
    inline void datatype::ToStringCore(xx::String& s) const noexcept
    {
        this->BaseType::ToStringCore(s);
        if (this->name) s.Append(", \"name\":\"", this->name, "\"");
        else s.Append(", \"name\":nil");
    }
    inline void datatype::CopyTo(datatype* const& o) const noexcept
    {
        o->name = this->name;
    }
    inline datatype* datatype::MakeCopy() const noexcept
    {
        auto o = mempool->MPCreate<datatype>();
        this->CopyTo(o);
        return o;
    }
    inline datatype_p datatype::MakePtrCopy() const noexcept
    {
        return datatype_p(this->MakeCopy());
    }

	inline fish::fish(xx::MemPool* const& mempool) noexcept
        : xx::Object(mempool)
	{
	}
	inline fish::fish(xx::BBuffer* const& bb)
        : xx::Object(bb)
	{
        if (int r = FromBBufferCore(*bb)) throw r;
	}
    inline void fish::ToBBuffer(xx::BBuffer& bb) const noexcept
    {
        bb.Write(this->id);
        bb.Write(this->name);
        bb.Write(this->minCoin);
        bb.Write(this->maxCoin);
    }
    inline int fish::FromBBuffer(xx::BBuffer& bb) noexcept
    {
        return this->FromBBufferCore(bb);
    }
    inline int fish::FromBBufferCore(xx::BBuffer& bb) noexcept
    {
        if (int r = bb.Read(this->id)) return r;
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->name)) return r;
        if (int r = bb.Read(this->minCoin)) return r;
        if (int r = bb.Read(this->maxCoin)) return r;
        return 0;
    }

    inline void fish::ToString(xx::String& s) const noexcept
    {
        if (this->memHeader().flags)
        {
        	s.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else this->memHeader().flags = 1;

        s.Append("{ \"pkgTypeName\":\"Tables.fish\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(s);
        s.Append(" }");
        
        this->memHeader().flags = 0;
    }
    inline void fish::ToStringCore(xx::String& s) const noexcept
    {
        this->BaseType::ToStringCore(s);
        s.Append(", \"id\":", this->id);
        if (this->name) s.Append(", \"name\":\"", this->name, "\"");
        else s.Append(", \"name\":nil");
        s.Append(", \"minCoin\":", this->minCoin);
        s.Append(", \"maxCoin\":", this->maxCoin);
    }
    inline void fish::CopyTo(fish* const& o) const noexcept
    {
        o->id = this->id;
        o->name = this->name;
        o->minCoin = this->minCoin;
        o->maxCoin = this->maxCoin;
    }
    inline fish* fish::MakeCopy() const noexcept
    {
        auto o = mempool->MPCreate<fish>();
        this->CopyTo(o);
        return o;
    }
    inline fish_p fish::MakePtrCopy() const noexcept
    {
        return fish_p(this->MakeCopy());
    }

	inline game::game(xx::MemPool* const& mempool) noexcept
        : xx::Object(mempool)
	{
	}
	inline game::game(xx::BBuffer* const& bb)
        : xx::Object(bb)
	{
        if (int r = FromBBufferCore(*bb)) throw r;
	}
    inline void game::ToBBuffer(xx::BBuffer& bb) const noexcept
    {
        bb.Write(this->id);
        bb.Write(this->name);
        bb.Write(this->type_name);
        bb.Write(this->enabled);
    }
    inline int game::FromBBuffer(xx::BBuffer& bb) noexcept
    {
        return this->FromBBufferCore(bb);
    }
    inline int game::FromBBufferCore(xx::BBuffer& bb) noexcept
    {
        if (int r = bb.Read(this->id)) return r;
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->name)) return r;
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->type_name)) return r;
        if (int r = bb.Read(this->enabled)) return r;
        return 0;
    }

    inline void game::ToString(xx::String& s) const noexcept
    {
        if (this->memHeader().flags)
        {
        	s.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else this->memHeader().flags = 1;

        s.Append("{ \"pkgTypeName\":\"Tables.game\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(s);
        s.Append(" }");
        
        this->memHeader().flags = 0;
    }
    inline void game::ToStringCore(xx::String& s) const noexcept
    {
        this->BaseType::ToStringCore(s);
        s.Append(", \"id\":", this->id);
        if (this->name) s.Append(", \"name\":\"", this->name, "\"");
        else s.Append(", \"name\":nil");
        if (this->type_name) s.Append(", \"type_name\":\"", this->type_name, "\"");
        else s.Append(", \"type_name\":nil");
        s.Append(", \"enabled\":", this->enabled);
    }
    inline void game::CopyTo(game* const& o) const noexcept
    {
        o->id = this->id;
        o->name = this->name;
        o->type_name = this->type_name;
        o->enabled = this->enabled;
    }
    inline game* game::MakeCopy() const noexcept
    {
        auto o = mempool->MPCreate<game>();
        this->CopyTo(o);
        return o;
    }
    inline game_p game::MakePtrCopy() const noexcept
    {
        return game_p(this->MakeCopy());
    }

	inline game_catchfish::game_catchfish(xx::MemPool* const& mempool) noexcept
        : xx::Object(mempool)
	{
	}
	inline game_catchfish::game_catchfish(xx::BBuffer* const& bb)
        : xx::Object(bb)
	{
        if (int r = FromBBufferCore(*bb)) throw r;
	}
    inline void game_catchfish::ToBBuffer(xx::BBuffer& bb) const noexcept
    {
        bb.Write(this->id);
        bb.Write(this->exchange_coin_ratio);
    }
    inline int game_catchfish::FromBBuffer(xx::BBuffer& bb) noexcept
    {
        return this->FromBBufferCore(bb);
    }
    inline int game_catchfish::FromBBufferCore(xx::BBuffer& bb) noexcept
    {
        if (int r = bb.Read(this->id)) return r;
        if (int r = bb.Read(this->exchange_coin_ratio)) return r;
        return 0;
    }

    inline void game_catchfish::ToString(xx::String& s) const noexcept
    {
        if (this->memHeader().flags)
        {
        	s.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else this->memHeader().flags = 1;

        s.Append("{ \"pkgTypeName\":\"Tables.game_catchfish\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(s);
        s.Append(" }");
        
        this->memHeader().flags = 0;
    }
    inline void game_catchfish::ToStringCore(xx::String& s) const noexcept
    {
        this->BaseType::ToStringCore(s);
        s.Append(", \"id\":", this->id);
        s.Append(", \"exchange_coin_ratio\":", this->exchange_coin_ratio);
    }
    inline void game_catchfish::CopyTo(game_catchfish* const& o) const noexcept
    {
        o->id = this->id;
        o->exchange_coin_ratio = this->exchange_coin_ratio;
    }
    inline game_catchfish* game_catchfish::MakeCopy() const noexcept
    {
        auto o = mempool->MPCreate<game_catchfish>();
        this->CopyTo(o);
        return o;
    }
    inline game_catchfish_p game_catchfish::MakePtrCopy() const noexcept
    {
        return game_catchfish_p(this->MakeCopy());
    }

	inline game_catchfish_account::game_catchfish_account(xx::MemPool* const& mempool) noexcept
        : xx::Object(mempool)
	{
	}
	inline game_catchfish_account::game_catchfish_account(xx::BBuffer* const& bb)
        : xx::Object(bb)
	{
        if (int r = FromBBufferCore(*bb)) throw r;
	}
    inline void game_catchfish_account::ToBBuffer(xx::BBuffer& bb) const noexcept
    {
        bb.Write(this->account_id);
        bb.Write(this->game_id);
        bb.Write(this->level_id);
        bb.Write(this->last_bet);
        bb.Write(this->last_cannon_id);
    }
    inline int game_catchfish_account::FromBBuffer(xx::BBuffer& bb) noexcept
    {
        return this->FromBBufferCore(bb);
    }
    inline int game_catchfish_account::FromBBufferCore(xx::BBuffer& bb) noexcept
    {
        if (int r = bb.Read(this->account_id)) return r;
        if (int r = bb.Read(this->game_id)) return r;
        if (int r = bb.Read(this->level_id)) return r;
        if (int r = bb.Read(this->last_bet)) return r;
        if (int r = bb.Read(this->last_cannon_id)) return r;
        return 0;
    }

    inline void game_catchfish_account::ToString(xx::String& s) const noexcept
    {
        if (this->memHeader().flags)
        {
        	s.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else this->memHeader().flags = 1;

        s.Append("{ \"pkgTypeName\":\"Tables.game_catchfish_account\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(s);
        s.Append(" }");
        
        this->memHeader().flags = 0;
    }
    inline void game_catchfish_account::ToStringCore(xx::String& s) const noexcept
    {
        this->BaseType::ToStringCore(s);
        s.Append(", \"account_id\":", this->account_id);
        s.Append(", \"game_id\":", this->game_id);
        s.Append(", \"level_id\":", this->level_id);
        s.Append(", \"last_bet\":", this->last_bet);
        s.Append(", \"last_cannon_id\":", this->last_cannon_id);
    }
    inline void game_catchfish_account::CopyTo(game_catchfish_account* const& o) const noexcept
    {
        o->account_id = this->account_id;
        o->game_id = this->game_id;
        o->level_id = this->level_id;
        o->last_bet = this->last_bet;
        o->last_cannon_id = this->last_cannon_id;
    }
    inline game_catchfish_account* game_catchfish_account::MakeCopy() const noexcept
    {
        auto o = mempool->MPCreate<game_catchfish_account>();
        this->CopyTo(o);
        return o;
    }
    inline game_catchfish_account_p game_catchfish_account::MakePtrCopy() const noexcept
    {
        return game_catchfish_account_p(this->MakeCopy());
    }

	inline game_catchfish_cannon::game_catchfish_cannon(xx::MemPool* const& mempool) noexcept
        : xx::Object(mempool)
	{
	}
	inline game_catchfish_cannon::game_catchfish_cannon(xx::BBuffer* const& bb)
        : xx::Object(bb)
	{
        if (int r = FromBBufferCore(*bb)) throw r;
	}
    inline void game_catchfish_cannon::ToBBuffer(xx::BBuffer& bb) const noexcept
    {
        bb.Write(this->id);
        bb.Write(this->vip_id);
    }
    inline int game_catchfish_cannon::FromBBuffer(xx::BBuffer& bb) noexcept
    {
        return this->FromBBufferCore(bb);
    }
    inline int game_catchfish_cannon::FromBBufferCore(xx::BBuffer& bb) noexcept
    {
        if (int r = bb.Read(this->id)) return r;
        if (int r = bb.Read(this->vip_id)) return r;
        return 0;
    }

    inline void game_catchfish_cannon::ToString(xx::String& s) const noexcept
    {
        if (this->memHeader().flags)
        {
        	s.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else this->memHeader().flags = 1;

        s.Append("{ \"pkgTypeName\":\"Tables.game_catchfish_cannon\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(s);
        s.Append(" }");
        
        this->memHeader().flags = 0;
    }
    inline void game_catchfish_cannon::ToStringCore(xx::String& s) const noexcept
    {
        this->BaseType::ToStringCore(s);
        s.Append(", \"id\":", this->id);
        s.Append(", \"vip_id\":", this->vip_id);
    }
    inline void game_catchfish_cannon::CopyTo(game_catchfish_cannon* const& o) const noexcept
    {
        o->id = this->id;
        o->vip_id = this->vip_id;
    }
    inline game_catchfish_cannon* game_catchfish_cannon::MakeCopy() const noexcept
    {
        auto o = mempool->MPCreate<game_catchfish_cannon>();
        this->CopyTo(o);
        return o;
    }
    inline game_catchfish_cannon_p game_catchfish_cannon::MakePtrCopy() const noexcept
    {
        return game_catchfish_cannon_p(this->MakeCopy());
    }

	inline game_catchfish_level::game_catchfish_level(xx::MemPool* const& mempool) noexcept
        : xx::Object(mempool)
	{
	}
	inline game_catchfish_level::game_catchfish_level(xx::BBuffer* const& bb)
        : xx::Object(bb)
	{
        if (int r = FromBBufferCore(*bb)) throw r;
	}
    inline void game_catchfish_level::ToBBuffer(xx::BBuffer& bb) const noexcept
    {
        bb.Write(this->game_id);
        bb.Write(this->level_id);
        bb.Write(this->min_bet);
        bb.Write(this->max_bet);
        bb.Write(this->min_money);
        bb.Write(this->enabled);
    }
    inline int game_catchfish_level::FromBBuffer(xx::BBuffer& bb) noexcept
    {
        return this->FromBBufferCore(bb);
    }
    inline int game_catchfish_level::FromBBufferCore(xx::BBuffer& bb) noexcept
    {
        if (int r = bb.Read(this->game_id)) return r;
        if (int r = bb.Read(this->level_id)) return r;
        if (int r = bb.Read(this->min_bet)) return r;
        if (int r = bb.Read(this->max_bet)) return r;
        if (int r = bb.Read(this->min_money)) return r;
        if (int r = bb.Read(this->enabled)) return r;
        return 0;
    }

    inline void game_catchfish_level::ToString(xx::String& s) const noexcept
    {
        if (this->memHeader().flags)
        {
        	s.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else this->memHeader().flags = 1;

        s.Append("{ \"pkgTypeName\":\"Tables.game_catchfish_level\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(s);
        s.Append(" }");
        
        this->memHeader().flags = 0;
    }
    inline void game_catchfish_level::ToStringCore(xx::String& s) const noexcept
    {
        this->BaseType::ToStringCore(s);
        s.Append(", \"game_id\":", this->game_id);
        s.Append(", \"level_id\":", this->level_id);
        s.Append(", \"min_bet\":", this->min_bet);
        s.Append(", \"max_bet\":", this->max_bet);
        s.Append(", \"min_money\":", this->min_money);
        s.Append(", \"enabled\":", this->enabled);
    }
    inline void game_catchfish_level::CopyTo(game_catchfish_level* const& o) const noexcept
    {
        o->game_id = this->game_id;
        o->level_id = this->level_id;
        o->min_bet = this->min_bet;
        o->max_bet = this->max_bet;
        o->min_money = this->min_money;
        o->enabled = this->enabled;
    }
    inline game_catchfish_level* game_catchfish_level::MakeCopy() const noexcept
    {
        auto o = mempool->MPCreate<game_catchfish_level>();
        this->CopyTo(o);
        return o;
    }
    inline game_catchfish_level_p game_catchfish_level::MakePtrCopy() const noexcept
    {
        return game_catchfish_level_p(this->MakeCopy());
    }

	inline game_catchfish_level_room::game_catchfish_level_room(xx::MemPool* const& mempool) noexcept
        : xx::Object(mempool)
	{
	}
	inline game_catchfish_level_room::game_catchfish_level_room(xx::BBuffer* const& bb)
        : xx::Object(bb)
	{
        if (int r = FromBBufferCore(*bb)) throw r;
	}
    inline void game_catchfish_level_room::ToBBuffer(xx::BBuffer& bb) const noexcept
    {
        bb.Write(this->game_id);
        bb.Write(this->level_id);
        bb.Write(this->room_id);
        bb.Write(this->enabled);
    }
    inline int game_catchfish_level_room::FromBBuffer(xx::BBuffer& bb) noexcept
    {
        return this->FromBBufferCore(bb);
    }
    inline int game_catchfish_level_room::FromBBufferCore(xx::BBuffer& bb) noexcept
    {
        if (int r = bb.Read(this->game_id)) return r;
        if (int r = bb.Read(this->level_id)) return r;
        if (int r = bb.Read(this->room_id)) return r;
        if (int r = bb.Read(this->enabled)) return r;
        return 0;
    }

    inline void game_catchfish_level_room::ToString(xx::String& s) const noexcept
    {
        if (this->memHeader().flags)
        {
        	s.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else this->memHeader().flags = 1;

        s.Append("{ \"pkgTypeName\":\"Tables.game_catchfish_level_room\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(s);
        s.Append(" }");
        
        this->memHeader().flags = 0;
    }
    inline void game_catchfish_level_room::ToStringCore(xx::String& s) const noexcept
    {
        this->BaseType::ToStringCore(s);
        s.Append(", \"game_id\":", this->game_id);
        s.Append(", \"level_id\":", this->level_id);
        s.Append(", \"room_id\":", this->room_id);
        s.Append(", \"enabled\":", this->enabled);
    }
    inline void game_catchfish_level_room::CopyTo(game_catchfish_level_room* const& o) const noexcept
    {
        o->game_id = this->game_id;
        o->level_id = this->level_id;
        o->room_id = this->room_id;
        o->enabled = this->enabled;
    }
    inline game_catchfish_level_room* game_catchfish_level_room::MakeCopy() const noexcept
    {
        auto o = mempool->MPCreate<game_catchfish_level_room>();
        this->CopyTo(o);
        return o;
    }
    inline game_catchfish_level_room_p game_catchfish_level_room::MakePtrCopy() const noexcept
    {
        return game_catchfish_level_room_p(this->MakeCopy());
    }

	inline log_account_avatar::log_account_avatar(xx::MemPool* const& mempool) noexcept
        : xx::Object(mempool)
	{
	}
	inline log_account_avatar::log_account_avatar(xx::BBuffer* const& bb)
        : xx::Object(bb)
	{
        if (int r = FromBBufferCore(*bb)) throw r;
	}
    inline void log_account_avatar::ToBBuffer(xx::BBuffer& bb) const noexcept
    {
        bb.Write(this->id);
        bb.Write(this->account_id);
        bb.Write(this->avatar_id);
        bb.Write(this->create_time);
    }
    inline int log_account_avatar::FromBBuffer(xx::BBuffer& bb) noexcept
    {
        return this->FromBBufferCore(bb);
    }
    inline int log_account_avatar::FromBBufferCore(xx::BBuffer& bb) noexcept
    {
        if (int r = bb.Read(this->id)) return r;
        if (int r = bb.Read(this->account_id)) return r;
        if (int r = bb.Read(this->avatar_id)) return r;
        if (int r = bb.Read(this->create_time)) return r;
        return 0;
    }

    inline void log_account_avatar::ToString(xx::String& s) const noexcept
    {
        if (this->memHeader().flags)
        {
        	s.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else this->memHeader().flags = 1;

        s.Append("{ \"pkgTypeName\":\"Tables.log_account_avatar\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(s);
        s.Append(" }");
        
        this->memHeader().flags = 0;
    }
    inline void log_account_avatar::ToStringCore(xx::String& s) const noexcept
    {
        this->BaseType::ToStringCore(s);
        s.Append(", \"id\":", this->id);
        s.Append(", \"account_id\":", this->account_id);
        s.Append(", \"avatar_id\":", this->avatar_id);
        s.Append(", \"create_time\":", this->create_time);
    }
    inline void log_account_avatar::CopyTo(log_account_avatar* const& o) const noexcept
    {
        o->id = this->id;
        o->account_id = this->account_id;
        o->avatar_id = this->avatar_id;
        o->create_time = this->create_time;
    }
    inline log_account_avatar* log_account_avatar::MakeCopy() const noexcept
    {
        auto o = mempool->MPCreate<log_account_avatar>();
        this->CopyTo(o);
        return o;
    }
    inline log_account_avatar_p log_account_avatar::MakePtrCopy() const noexcept
    {
        return log_account_avatar_p(this->MakeCopy());
    }

	inline log_account_enabled::log_account_enabled(xx::MemPool* const& mempool) noexcept
        : xx::Object(mempool)
	{
	}
	inline log_account_enabled::log_account_enabled(xx::BBuffer* const& bb)
        : xx::Object(bb)
	{
        if (int r = FromBBufferCore(*bb)) throw r;
	}
    inline void log_account_enabled::ToBBuffer(xx::BBuffer& bb) const noexcept
    {
        bb.Write(this->id);
        bb.Write(this->account_id);
        bb.Write(this->enabled);
        bb.Write(this->memo);
        bb.Write(this->create_time);
    }
    inline int log_account_enabled::FromBBuffer(xx::BBuffer& bb) noexcept
    {
        return this->FromBBufferCore(bb);
    }
    inline int log_account_enabled::FromBBufferCore(xx::BBuffer& bb) noexcept
    {
        if (int r = bb.Read(this->id)) return r;
        if (int r = bb.Read(this->account_id)) return r;
        if (int r = bb.Read(this->enabled)) return r;
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->memo)) return r;
        if (int r = bb.Read(this->create_time)) return r;
        return 0;
    }

    inline void log_account_enabled::ToString(xx::String& s) const noexcept
    {
        if (this->memHeader().flags)
        {
        	s.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else this->memHeader().flags = 1;

        s.Append("{ \"pkgTypeName\":\"Tables.log_account_enabled\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(s);
        s.Append(" }");
        
        this->memHeader().flags = 0;
    }
    inline void log_account_enabled::ToStringCore(xx::String& s) const noexcept
    {
        this->BaseType::ToStringCore(s);
        s.Append(", \"id\":", this->id);
        s.Append(", \"account_id\":", this->account_id);
        s.Append(", \"enabled\":", this->enabled);
        if (this->memo) s.Append(", \"memo\":\"", this->memo, "\"");
        else s.Append(", \"memo\":nil");
        s.Append(", \"create_time\":", this->create_time);
    }
    inline void log_account_enabled::CopyTo(log_account_enabled* const& o) const noexcept
    {
        o->id = this->id;
        o->account_id = this->account_id;
        o->enabled = this->enabled;
        o->memo = this->memo;
        o->create_time = this->create_time;
    }
    inline log_account_enabled* log_account_enabled::MakeCopy() const noexcept
    {
        auto o = mempool->MPCreate<log_account_enabled>();
        this->CopyTo(o);
        return o;
    }
    inline log_account_enabled_p log_account_enabled::MakePtrCopy() const noexcept
    {
        return log_account_enabled_p(this->MakeCopy());
    }

	inline log_account_money_safe::log_account_money_safe(xx::MemPool* const& mempool) noexcept
        : xx::Object(mempool)
	{
	}
	inline log_account_money_safe::log_account_money_safe(xx::BBuffer* const& bb)
        : xx::Object(bb)
	{
        if (int r = FromBBufferCore(*bb)) throw r;
	}
    inline void log_account_money_safe::ToBBuffer(xx::BBuffer& bb) const noexcept
    {
        bb.Write(this->id);
        bb.Write(this->account_id);
        bb.Write(this->value);
        bb.Write(this->create_time);
    }
    inline int log_account_money_safe::FromBBuffer(xx::BBuffer& bb) noexcept
    {
        return this->FromBBufferCore(bb);
    }
    inline int log_account_money_safe::FromBBufferCore(xx::BBuffer& bb) noexcept
    {
        if (int r = bb.Read(this->id)) return r;
        if (int r = bb.Read(this->account_id)) return r;
        if (int r = bb.Read(this->value)) return r;
        if (int r = bb.Read(this->create_time)) return r;
        return 0;
    }

    inline void log_account_money_safe::ToString(xx::String& s) const noexcept
    {
        if (this->memHeader().flags)
        {
        	s.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else this->memHeader().flags = 1;

        s.Append("{ \"pkgTypeName\":\"Tables.log_account_money_safe\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(s);
        s.Append(" }");
        
        this->memHeader().flags = 0;
    }
    inline void log_account_money_safe::ToStringCore(xx::String& s) const noexcept
    {
        this->BaseType::ToStringCore(s);
        s.Append(", \"id\":", this->id);
        s.Append(", \"account_id\":", this->account_id);
        s.Append(", \"value\":", this->value);
        s.Append(", \"create_time\":", this->create_time);
    }
    inline void log_account_money_safe::CopyTo(log_account_money_safe* const& o) const noexcept
    {
        o->id = this->id;
        o->account_id = this->account_id;
        o->value = this->value;
        o->create_time = this->create_time;
    }
    inline log_account_money_safe* log_account_money_safe::MakeCopy() const noexcept
    {
        auto o = mempool->MPCreate<log_account_money_safe>();
        this->CopyTo(o);
        return o;
    }
    inline log_account_money_safe_p log_account_money_safe::MakePtrCopy() const noexcept
    {
        return log_account_money_safe_p(this->MakeCopy());
    }

	inline log_account_nickname::log_account_nickname(xx::MemPool* const& mempool) noexcept
        : xx::Object(mempool)
	{
	}
	inline log_account_nickname::log_account_nickname(xx::BBuffer* const& bb)
        : xx::Object(bb)
	{
        if (int r = FromBBufferCore(*bb)) throw r;
	}
    inline void log_account_nickname::ToBBuffer(xx::BBuffer& bb) const noexcept
    {
        bb.Write(this->id);
        bb.Write(this->account_id);
        bb.Write(this->nickname);
        bb.Write(this->create_time);
    }
    inline int log_account_nickname::FromBBuffer(xx::BBuffer& bb) noexcept
    {
        return this->FromBBufferCore(bb);
    }
    inline int log_account_nickname::FromBBufferCore(xx::BBuffer& bb) noexcept
    {
        if (int r = bb.Read(this->id)) return r;
        if (int r = bb.Read(this->account_id)) return r;
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->nickname)) return r;
        if (int r = bb.Read(this->create_time)) return r;
        return 0;
    }

    inline void log_account_nickname::ToString(xx::String& s) const noexcept
    {
        if (this->memHeader().flags)
        {
        	s.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else this->memHeader().flags = 1;

        s.Append("{ \"pkgTypeName\":\"Tables.log_account_nickname\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(s);
        s.Append(" }");
        
        this->memHeader().flags = 0;
    }
    inline void log_account_nickname::ToStringCore(xx::String& s) const noexcept
    {
        this->BaseType::ToStringCore(s);
        s.Append(", \"id\":", this->id);
        s.Append(", \"account_id\":", this->account_id);
        if (this->nickname) s.Append(", \"nickname\":\"", this->nickname, "\"");
        else s.Append(", \"nickname\":nil");
        s.Append(", \"create_time\":", this->create_time);
    }
    inline void log_account_nickname::CopyTo(log_account_nickname* const& o) const noexcept
    {
        o->id = this->id;
        o->account_id = this->account_id;
        o->nickname = this->nickname;
        o->create_time = this->create_time;
    }
    inline log_account_nickname* log_account_nickname::MakeCopy() const noexcept
    {
        auto o = mempool->MPCreate<log_account_nickname>();
        this->CopyTo(o);
        return o;
    }
    inline log_account_nickname_p log_account_nickname::MakePtrCopy() const noexcept
    {
        return log_account_nickname_p(this->MakeCopy());
    }

	inline log_account_password::log_account_password(xx::MemPool* const& mempool) noexcept
        : xx::Object(mempool)
	{
	}
	inline log_account_password::log_account_password(xx::BBuffer* const& bb)
        : xx::Object(bb)
	{
        if (int r = FromBBufferCore(*bb)) throw r;
	}
    inline void log_account_password::ToBBuffer(xx::BBuffer& bb) const noexcept
    {
        bb.Write(this->id);
        bb.Write(this->account_id);
        bb.Write(this->password);
        bb.Write(this->create_time);
    }
    inline int log_account_password::FromBBuffer(xx::BBuffer& bb) noexcept
    {
        return this->FromBBufferCore(bb);
    }
    inline int log_account_password::FromBBufferCore(xx::BBuffer& bb) noexcept
    {
        if (int r = bb.Read(this->id)) return r;
        if (int r = bb.Read(this->account_id)) return r;
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->password)) return r;
        if (int r = bb.Read(this->create_time)) return r;
        return 0;
    }

    inline void log_account_password::ToString(xx::String& s) const noexcept
    {
        if (this->memHeader().flags)
        {
        	s.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else this->memHeader().flags = 1;

        s.Append("{ \"pkgTypeName\":\"Tables.log_account_password\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(s);
        s.Append(" }");
        
        this->memHeader().flags = 0;
    }
    inline void log_account_password::ToStringCore(xx::String& s) const noexcept
    {
        this->BaseType::ToStringCore(s);
        s.Append(", \"id\":", this->id);
        s.Append(", \"account_id\":", this->account_id);
        if (this->password) s.Append(", \"password\":\"", this->password, "\"");
        else s.Append(", \"password\":nil");
        s.Append(", \"create_time\":", this->create_time);
    }
    inline void log_account_password::CopyTo(log_account_password* const& o) const noexcept
    {
        o->id = this->id;
        o->account_id = this->account_id;
        o->password = this->password;
        o->create_time = this->create_time;
    }
    inline log_account_password* log_account_password::MakeCopy() const noexcept
    {
        auto o = mempool->MPCreate<log_account_password>();
        this->CopyTo(o);
        return o;
    }
    inline log_account_password_p log_account_password::MakePtrCopy() const noexcept
    {
        return log_account_password_p(this->MakeCopy());
    }

	inline log_account_phone::log_account_phone(xx::MemPool* const& mempool) noexcept
        : xx::Object(mempool)
	{
	}
	inline log_account_phone::log_account_phone(xx::BBuffer* const& bb)
        : xx::Object(bb)
	{
        if (int r = FromBBufferCore(*bb)) throw r;
	}
    inline void log_account_phone::ToBBuffer(xx::BBuffer& bb) const noexcept
    {
        bb.Write(this->id);
        bb.Write(this->account_id);
        bb.Write(this->phone);
        bb.Write(this->create_time);
    }
    inline int log_account_phone::FromBBuffer(xx::BBuffer& bb) noexcept
    {
        return this->FromBBufferCore(bb);
    }
    inline int log_account_phone::FromBBufferCore(xx::BBuffer& bb) noexcept
    {
        if (int r = bb.Read(this->id)) return r;
        if (int r = bb.Read(this->account_id)) return r;
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->phone)) return r;
        if (int r = bb.Read(this->create_time)) return r;
        return 0;
    }

    inline void log_account_phone::ToString(xx::String& s) const noexcept
    {
        if (this->memHeader().flags)
        {
        	s.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else this->memHeader().flags = 1;

        s.Append("{ \"pkgTypeName\":\"Tables.log_account_phone\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(s);
        s.Append(" }");
        
        this->memHeader().flags = 0;
    }
    inline void log_account_phone::ToStringCore(xx::String& s) const noexcept
    {
        this->BaseType::ToStringCore(s);
        s.Append(", \"id\":", this->id);
        s.Append(", \"account_id\":", this->account_id);
        if (this->phone) s.Append(", \"phone\":\"", this->phone, "\"");
        else s.Append(", \"phone\":nil");
        s.Append(", \"create_time\":", this->create_time);
    }
    inline void log_account_phone::CopyTo(log_account_phone* const& o) const noexcept
    {
        o->id = this->id;
        o->account_id = this->account_id;
        o->phone = this->phone;
        o->create_time = this->create_time;
    }
    inline log_account_phone* log_account_phone::MakeCopy() const noexcept
    {
        auto o = mempool->MPCreate<log_account_phone>();
        this->CopyTo(o);
        return o;
    }
    inline log_account_phone_p log_account_phone::MakePtrCopy() const noexcept
    {
        return log_account_phone_p(this->MakeCopy());
    }

	inline log_account_recharge::log_account_recharge(xx::MemPool* const& mempool) noexcept
        : xx::Object(mempool)
	{
	}
	inline log_account_recharge::log_account_recharge(xx::BBuffer* const& bb)
        : xx::Object(bb)
	{
        if (int r = FromBBufferCore(*bb)) throw r;
	}
    inline void log_account_recharge::ToBBuffer(xx::BBuffer& bb) const noexcept
    {
        bb.Write(this->token);
        bb.Write(this->account_id);
        bb.Write(this->money);
        bb.Write(this->memo);
        bb.Write(this->create_time);
    }
    inline int log_account_recharge::FromBBuffer(xx::BBuffer& bb) noexcept
    {
        return this->FromBBufferCore(bb);
    }
    inline int log_account_recharge::FromBBufferCore(xx::BBuffer& bb) noexcept
    {
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->token)) return r;
        if (int r = bb.Read(this->account_id)) return r;
        if (int r = bb.Read(this->money)) return r;
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->memo)) return r;
        if (int r = bb.Read(this->create_time)) return r;
        return 0;
    }

    inline void log_account_recharge::ToString(xx::String& s) const noexcept
    {
        if (this->memHeader().flags)
        {
        	s.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else this->memHeader().flags = 1;

        s.Append("{ \"pkgTypeName\":\"Tables.log_account_recharge\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(s);
        s.Append(" }");
        
        this->memHeader().flags = 0;
    }
    inline void log_account_recharge::ToStringCore(xx::String& s) const noexcept
    {
        this->BaseType::ToStringCore(s);
        if (this->token) s.Append(", \"token\":\"", this->token, "\"");
        else s.Append(", \"token\":nil");
        s.Append(", \"account_id\":", this->account_id);
        s.Append(", \"money\":", this->money);
        if (this->memo) s.Append(", \"memo\":\"", this->memo, "\"");
        else s.Append(", \"memo\":nil");
        s.Append(", \"create_time\":", this->create_time);
    }
    inline void log_account_recharge::CopyTo(log_account_recharge* const& o) const noexcept
    {
        o->token = this->token;
        o->account_id = this->account_id;
        o->money = this->money;
        o->memo = this->memo;
        o->create_time = this->create_time;
    }
    inline log_account_recharge* log_account_recharge::MakeCopy() const noexcept
    {
        auto o = mempool->MPCreate<log_account_recharge>();
        this->CopyTo(o);
        return o;
    }
    inline log_account_recharge_p log_account_recharge::MakePtrCopy() const noexcept
    {
        return log_account_recharge_p(this->MakeCopy());
    }

	inline log_game::log_game(xx::MemPool* const& mempool) noexcept
        : xx::Object(mempool)
	{
	}
	inline log_game::log_game(xx::BBuffer* const& bb)
        : xx::Object(bb)
	{
        if (int r = FromBBufferCore(*bb)) throw r;
	}
    inline void log_game::ToBBuffer(xx::BBuffer& bb) const noexcept
    {
        bb.Write(this->id);
        bb.Write(this->game_id);
    }
    inline int log_game::FromBBuffer(xx::BBuffer& bb) noexcept
    {
        return this->FromBBufferCore(bb);
    }
    inline int log_game::FromBBufferCore(xx::BBuffer& bb) noexcept
    {
        if (int r = bb.Read(this->id)) return r;
        if (int r = bb.Read(this->game_id)) return r;
        return 0;
    }

    inline void log_game::ToString(xx::String& s) const noexcept
    {
        if (this->memHeader().flags)
        {
        	s.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else this->memHeader().flags = 1;

        s.Append("{ \"pkgTypeName\":\"Tables.log_game\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(s);
        s.Append(" }");
        
        this->memHeader().flags = 0;
    }
    inline void log_game::ToStringCore(xx::String& s) const noexcept
    {
        this->BaseType::ToStringCore(s);
        s.Append(", \"id\":", this->id);
        s.Append(", \"game_id\":", this->game_id);
    }
    inline void log_game::CopyTo(log_game* const& o) const noexcept
    {
        o->id = this->id;
        o->game_id = this->game_id;
    }
    inline log_game* log_game::MakeCopy() const noexcept
    {
        auto o = mempool->MPCreate<log_game>();
        this->CopyTo(o);
        return o;
    }
    inline log_game_p log_game::MakePtrCopy() const noexcept
    {
        return log_game_p(this->MakeCopy());
    }

	inline log_game_catchfish::log_game_catchfish(xx::MemPool* const& mempool) noexcept
        : xx::Object(mempool)
	{
	}
	inline log_game_catchfish::log_game_catchfish(xx::BBuffer* const& bb)
        : xx::Object(bb)
	{
        if (int r = FromBBufferCore(*bb)) throw r;
	}
    inline void log_game_catchfish::ToBBuffer(xx::BBuffer& bb) const noexcept
    {
        bb.Write(this->id);
        bb.Write(this->level_id);
        bb.Write(this->room_id);
        bb.Write(this->account_id);
        bb.Write(this->enter_time);
        bb.Write(this->enter_money);
        bb.Write(this->leave_time);
        bb.Write(this->leave_money);
        bb.Write(this->consume);
    }
    inline int log_game_catchfish::FromBBuffer(xx::BBuffer& bb) noexcept
    {
        return this->FromBBufferCore(bb);
    }
    inline int log_game_catchfish::FromBBufferCore(xx::BBuffer& bb) noexcept
    {
        if (int r = bb.Read(this->id)) return r;
        if (int r = bb.Read(this->level_id)) return r;
        if (int r = bb.Read(this->room_id)) return r;
        if (int r = bb.Read(this->account_id)) return r;
        if (int r = bb.Read(this->enter_time)) return r;
        if (int r = bb.Read(this->enter_money)) return r;
        if (int r = bb.Read(this->leave_time)) return r;
        if (int r = bb.Read(this->leave_money)) return r;
        if (int r = bb.Read(this->consume)) return r;
        return 0;
    }

    inline void log_game_catchfish::ToString(xx::String& s) const noexcept
    {
        if (this->memHeader().flags)
        {
        	s.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else this->memHeader().flags = 1;

        s.Append("{ \"pkgTypeName\":\"Tables.log_game_catchfish\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(s);
        s.Append(" }");
        
        this->memHeader().flags = 0;
    }
    inline void log_game_catchfish::ToStringCore(xx::String& s) const noexcept
    {
        this->BaseType::ToStringCore(s);
        s.Append(", \"id\":", this->id);
        s.Append(", \"level_id\":", this->level_id);
        s.Append(", \"room_id\":", this->room_id);
        s.Append(", \"account_id\":", this->account_id);
        s.Append(", \"enter_time\":", this->enter_time);
        s.Append(", \"enter_money\":", this->enter_money);
        s.Append(", \"leave_time\":", this->leave_time);
        s.Append(", \"leave_money\":", this->leave_money);
        s.Append(", \"consume\":", this->consume);
    }
    inline void log_game_catchfish::CopyTo(log_game_catchfish* const& o) const noexcept
    {
        o->id = this->id;
        o->level_id = this->level_id;
        o->room_id = this->room_id;
        o->account_id = this->account_id;
        o->enter_time = this->enter_time;
        o->enter_money = this->enter_money;
        o->leave_time = this->leave_time;
        o->leave_money = this->leave_money;
        o->consume = this->consume;
    }
    inline log_game_catchfish* log_game_catchfish::MakeCopy() const noexcept
    {
        auto o = mempool->MPCreate<log_game_catchfish>();
        this->CopyTo(o);
        return o;
    }
    inline log_game_catchfish_p log_game_catchfish::MakePtrCopy() const noexcept
    {
        return log_game_catchfish_p(this->MakeCopy());
    }

	inline log_game_catchfish_event::log_game_catchfish_event(xx::MemPool* const& mempool) noexcept
        : xx::Object(mempool)
	{
	}
	inline log_game_catchfish_event::log_game_catchfish_event(xx::BBuffer* const& bb)
        : xx::Object(bb)
	{
        if (int r = FromBBufferCore(*bb)) throw r;
	}
    inline void log_game_catchfish_event::ToBBuffer(xx::BBuffer& bb) const noexcept
    {
        bb.Write(this->id);
        bb.Write(this->log_game_catchfish_id);
        bb.Write(this->event_type_id);
        bb.Write(this->frame_number);
        bb.Write(this->create_time);
    }
    inline int log_game_catchfish_event::FromBBuffer(xx::BBuffer& bb) noexcept
    {
        return this->FromBBufferCore(bb);
    }
    inline int log_game_catchfish_event::FromBBufferCore(xx::BBuffer& bb) noexcept
    {
        if (int r = bb.Read(this->id)) return r;
        if (int r = bb.Read(this->log_game_catchfish_id)) return r;
        if (int r = bb.Read(this->event_type_id)) return r;
        if (int r = bb.Read(this->frame_number)) return r;
        if (int r = bb.Read(this->create_time)) return r;
        return 0;
    }

    inline void log_game_catchfish_event::ToString(xx::String& s) const noexcept
    {
        if (this->memHeader().flags)
        {
        	s.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else this->memHeader().flags = 1;

        s.Append("{ \"pkgTypeName\":\"Tables.log_game_catchfish_event\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(s);
        s.Append(" }");
        
        this->memHeader().flags = 0;
    }
    inline void log_game_catchfish_event::ToStringCore(xx::String& s) const noexcept
    {
        this->BaseType::ToStringCore(s);
        s.Append(", \"id\":", this->id);
        s.Append(", \"log_game_catchfish_id\":", this->log_game_catchfish_id);
        s.Append(", \"event_type_id\":", this->event_type_id);
        s.Append(", \"frame_number\":", this->frame_number);
        s.Append(", \"create_time\":", this->create_time);
    }
    inline void log_game_catchfish_event::CopyTo(log_game_catchfish_event* const& o) const noexcept
    {
        o->id = this->id;
        o->log_game_catchfish_id = this->log_game_catchfish_id;
        o->event_type_id = this->event_type_id;
        o->frame_number = this->frame_number;
        o->create_time = this->create_time;
    }
    inline log_game_catchfish_event* log_game_catchfish_event::MakeCopy() const noexcept
    {
        auto o = mempool->MPCreate<log_game_catchfish_event>();
        this->CopyTo(o);
        return o;
    }
    inline log_game_catchfish_event_p log_game_catchfish_event::MakePtrCopy() const noexcept
    {
        return log_game_catchfish_event_p(this->MakeCopy());
    }

	inline log_game_catchfish_event_bulletdie::log_game_catchfish_event_bulletdie(xx::MemPool* const& mempool) noexcept
        : xx::Object(mempool)
	{
	}
	inline log_game_catchfish_event_bulletdie::log_game_catchfish_event_bulletdie(xx::BBuffer* const& bb)
        : xx::Object(bb)
	{
        if (int r = FromBBufferCore(*bb)) throw r;
	}
    inline void log_game_catchfish_event_bulletdie::ToBBuffer(xx::BBuffer& bb) const noexcept
    {
        bb.Write(this->id);
        bb.Write(this->log_game_catchfish_id);
        bb.Write(this->type_id);
        bb.Write(this->bullet_id);
        bb.Write(this->coin);
        bb.Write(this->money);
    }
    inline int log_game_catchfish_event_bulletdie::FromBBuffer(xx::BBuffer& bb) noexcept
    {
        return this->FromBBufferCore(bb);
    }
    inline int log_game_catchfish_event_bulletdie::FromBBufferCore(xx::BBuffer& bb) noexcept
    {
        if (int r = bb.Read(this->id)) return r;
        if (int r = bb.Read(this->log_game_catchfish_id)) return r;
        if (int r = bb.Read(this->type_id)) return r;
        if (int r = bb.Read(this->bullet_id)) return r;
        if (int r = bb.Read(this->coin)) return r;
        if (int r = bb.Read(this->money)) return r;
        return 0;
    }

    inline void log_game_catchfish_event_bulletdie::ToString(xx::String& s) const noexcept
    {
        if (this->memHeader().flags)
        {
        	s.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else this->memHeader().flags = 1;

        s.Append("{ \"pkgTypeName\":\"Tables.log_game_catchfish_event_bulletdie\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(s);
        s.Append(" }");
        
        this->memHeader().flags = 0;
    }
    inline void log_game_catchfish_event_bulletdie::ToStringCore(xx::String& s) const noexcept
    {
        this->BaseType::ToStringCore(s);
        s.Append(", \"id\":", this->id);
        s.Append(", \"log_game_catchfish_id\":", this->log_game_catchfish_id);
        s.Append(", \"type_id\":", this->type_id);
        s.Append(", \"bullet_id\":", this->bullet_id);
        s.Append(", \"coin\":", this->coin);
        s.Append(", \"money\":", this->money);
    }
    inline void log_game_catchfish_event_bulletdie::CopyTo(log_game_catchfish_event_bulletdie* const& o) const noexcept
    {
        o->id = this->id;
        o->log_game_catchfish_id = this->log_game_catchfish_id;
        o->type_id = this->type_id;
        o->bullet_id = this->bullet_id;
        o->coin = this->coin;
        o->money = this->money;
    }
    inline log_game_catchfish_event_bulletdie* log_game_catchfish_event_bulletdie::MakeCopy() const noexcept
    {
        auto o = mempool->MPCreate<log_game_catchfish_event_bulletdie>();
        this->CopyTo(o);
        return o;
    }
    inline log_game_catchfish_event_bulletdie_p log_game_catchfish_event_bulletdie::MakePtrCopy() const noexcept
    {
        return log_game_catchfish_event_bulletdie_p(this->MakeCopy());
    }

	inline log_game_catchfish_event_bulletmiss::log_game_catchfish_event_bulletmiss(xx::MemPool* const& mempool) noexcept
        : xx::Object(mempool)
	{
	}
	inline log_game_catchfish_event_bulletmiss::log_game_catchfish_event_bulletmiss(xx::BBuffer* const& bb)
        : xx::Object(bb)
	{
        if (int r = FromBBufferCore(*bb)) throw r;
	}
    inline void log_game_catchfish_event_bulletmiss::ToBBuffer(xx::BBuffer& bb) const noexcept
    {
        bb.Write(this->id);
        bb.Write(this->log_game_catchfish_id);
        bb.Write(this->type_id);
        bb.Write(this->bullet_id);
        bb.Write(this->coin);
        bb.Write(this->money);
    }
    inline int log_game_catchfish_event_bulletmiss::FromBBuffer(xx::BBuffer& bb) noexcept
    {
        return this->FromBBufferCore(bb);
    }
    inline int log_game_catchfish_event_bulletmiss::FromBBufferCore(xx::BBuffer& bb) noexcept
    {
        if (int r = bb.Read(this->id)) return r;
        if (int r = bb.Read(this->log_game_catchfish_id)) return r;
        if (int r = bb.Read(this->type_id)) return r;
        if (int r = bb.Read(this->bullet_id)) return r;
        if (int r = bb.Read(this->coin)) return r;
        if (int r = bb.Read(this->money)) return r;
        return 0;
    }

    inline void log_game_catchfish_event_bulletmiss::ToString(xx::String& s) const noexcept
    {
        if (this->memHeader().flags)
        {
        	s.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else this->memHeader().flags = 1;

        s.Append("{ \"pkgTypeName\":\"Tables.log_game_catchfish_event_bulletmiss\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(s);
        s.Append(" }");
        
        this->memHeader().flags = 0;
    }
    inline void log_game_catchfish_event_bulletmiss::ToStringCore(xx::String& s) const noexcept
    {
        this->BaseType::ToStringCore(s);
        s.Append(", \"id\":", this->id);
        s.Append(", \"log_game_catchfish_id\":", this->log_game_catchfish_id);
        s.Append(", \"type_id\":", this->type_id);
        s.Append(", \"bullet_id\":", this->bullet_id);
        s.Append(", \"coin\":", this->coin);
        s.Append(", \"money\":", this->money);
    }
    inline void log_game_catchfish_event_bulletmiss::CopyTo(log_game_catchfish_event_bulletmiss* const& o) const noexcept
    {
        o->id = this->id;
        o->log_game_catchfish_id = this->log_game_catchfish_id;
        o->type_id = this->type_id;
        o->bullet_id = this->bullet_id;
        o->coin = this->coin;
        o->money = this->money;
    }
    inline log_game_catchfish_event_bulletmiss* log_game_catchfish_event_bulletmiss::MakeCopy() const noexcept
    {
        auto o = mempool->MPCreate<log_game_catchfish_event_bulletmiss>();
        this->CopyTo(o);
        return o;
    }
    inline log_game_catchfish_event_bulletmiss_p log_game_catchfish_event_bulletmiss::MakePtrCopy() const noexcept
    {
        return log_game_catchfish_event_bulletmiss_p(this->MakeCopy());
    }

	inline log_game_catchfish_event_fire::log_game_catchfish_event_fire(xx::MemPool* const& mempool) noexcept
        : xx::Object(mempool)
	{
	}
	inline log_game_catchfish_event_fire::log_game_catchfish_event_fire(xx::BBuffer* const& bb)
        : xx::Object(bb)
	{
        if (int r = FromBBufferCore(*bb)) throw r;
	}
    inline void log_game_catchfish_event_fire::ToBBuffer(xx::BBuffer& bb) const noexcept
    {
        bb.Write(this->id);
        bb.Write(this->log_game_catchfish_id);
        bb.Write(this->type_id);
        bb.Write(this->bullet_id);
        bb.Write(this->coin);
        bb.Write(this->money);
    }
    inline int log_game_catchfish_event_fire::FromBBuffer(xx::BBuffer& bb) noexcept
    {
        return this->FromBBufferCore(bb);
    }
    inline int log_game_catchfish_event_fire::FromBBufferCore(xx::BBuffer& bb) noexcept
    {
        if (int r = bb.Read(this->id)) return r;
        if (int r = bb.Read(this->log_game_catchfish_id)) return r;
        if (int r = bb.Read(this->type_id)) return r;
        if (int r = bb.Read(this->bullet_id)) return r;
        if (int r = bb.Read(this->coin)) return r;
        if (int r = bb.Read(this->money)) return r;
        return 0;
    }

    inline void log_game_catchfish_event_fire::ToString(xx::String& s) const noexcept
    {
        if (this->memHeader().flags)
        {
        	s.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else this->memHeader().flags = 1;

        s.Append("{ \"pkgTypeName\":\"Tables.log_game_catchfish_event_fire\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(s);
        s.Append(" }");
        
        this->memHeader().flags = 0;
    }
    inline void log_game_catchfish_event_fire::ToStringCore(xx::String& s) const noexcept
    {
        this->BaseType::ToStringCore(s);
        s.Append(", \"id\":", this->id);
        s.Append(", \"log_game_catchfish_id\":", this->log_game_catchfish_id);
        s.Append(", \"type_id\":", this->type_id);
        s.Append(", \"bullet_id\":", this->bullet_id);
        s.Append(", \"coin\":", this->coin);
        s.Append(", \"money\":", this->money);
    }
    inline void log_game_catchfish_event_fire::CopyTo(log_game_catchfish_event_fire* const& o) const noexcept
    {
        o->id = this->id;
        o->log_game_catchfish_id = this->log_game_catchfish_id;
        o->type_id = this->type_id;
        o->bullet_id = this->bullet_id;
        o->coin = this->coin;
        o->money = this->money;
    }
    inline log_game_catchfish_event_fire* log_game_catchfish_event_fire::MakeCopy() const noexcept
    {
        auto o = mempool->MPCreate<log_game_catchfish_event_fire>();
        this->CopyTo(o);
        return o;
    }
    inline log_game_catchfish_event_fire_p log_game_catchfish_event_fire::MakePtrCopy() const noexcept
    {
        return log_game_catchfish_event_fire_p(this->MakeCopy());
    }

	inline log_game_catchfish_event_fishdie::log_game_catchfish_event_fishdie(xx::MemPool* const& mempool) noexcept
        : xx::Object(mempool)
	{
	}
	inline log_game_catchfish_event_fishdie::log_game_catchfish_event_fishdie(xx::BBuffer* const& bb)
        : xx::Object(bb)
	{
        if (int r = FromBBufferCore(*bb)) throw r;
	}
    inline void log_game_catchfish_event_fishdie::ToBBuffer(xx::BBuffer& bb) const noexcept
    {
        bb.Write(this->id);
        bb.Write(this->log_game_catchfish_id);
        bb.Write(this->fish_type_id);
        bb.Write(this->fish_id);
        bb.Write(this->fish_ratio);
        bb.Write(this->bullet_coin);
        bb.Write(this->coin);
        bb.Write(this->money);
    }
    inline int log_game_catchfish_event_fishdie::FromBBuffer(xx::BBuffer& bb) noexcept
    {
        return this->FromBBufferCore(bb);
    }
    inline int log_game_catchfish_event_fishdie::FromBBufferCore(xx::BBuffer& bb) noexcept
    {
        if (int r = bb.Read(this->id)) return r;
        if (int r = bb.Read(this->log_game_catchfish_id)) return r;
        if (int r = bb.Read(this->fish_type_id)) return r;
        if (int r = bb.Read(this->fish_id)) return r;
        if (int r = bb.Read(this->fish_ratio)) return r;
        if (int r = bb.Read(this->bullet_coin)) return r;
        if (int r = bb.Read(this->coin)) return r;
        if (int r = bb.Read(this->money)) return r;
        return 0;
    }

    inline void log_game_catchfish_event_fishdie::ToString(xx::String& s) const noexcept
    {
        if (this->memHeader().flags)
        {
        	s.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else this->memHeader().flags = 1;

        s.Append("{ \"pkgTypeName\":\"Tables.log_game_catchfish_event_fishdie\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(s);
        s.Append(" }");
        
        this->memHeader().flags = 0;
    }
    inline void log_game_catchfish_event_fishdie::ToStringCore(xx::String& s) const noexcept
    {
        this->BaseType::ToStringCore(s);
        s.Append(", \"id\":", this->id);
        s.Append(", \"log_game_catchfish_id\":", this->log_game_catchfish_id);
        s.Append(", \"fish_type_id\":", this->fish_type_id);
        s.Append(", \"fish_id\":", this->fish_id);
        s.Append(", \"fish_ratio\":", this->fish_ratio);
        s.Append(", \"bullet_coin\":", this->bullet_coin);
        s.Append(", \"coin\":", this->coin);
        s.Append(", \"money\":", this->money);
    }
    inline void log_game_catchfish_event_fishdie::CopyTo(log_game_catchfish_event_fishdie* const& o) const noexcept
    {
        o->id = this->id;
        o->log_game_catchfish_id = this->log_game_catchfish_id;
        o->fish_type_id = this->fish_type_id;
        o->fish_id = this->fish_id;
        o->fish_ratio = this->fish_ratio;
        o->bullet_coin = this->bullet_coin;
        o->coin = this->coin;
        o->money = this->money;
    }
    inline log_game_catchfish_event_fishdie* log_game_catchfish_event_fishdie::MakeCopy() const noexcept
    {
        auto o = mempool->MPCreate<log_game_catchfish_event_fishdie>();
        this->CopyTo(o);
        return o;
    }
    inline log_game_catchfish_event_fishdie_p log_game_catchfish_event_fishdie::MakePtrCopy() const noexcept
    {
        return log_game_catchfish_event_fishdie_p(this->MakeCopy());
    }

	inline log_login_out::log_login_out(xx::MemPool* const& mempool) noexcept
        : xx::Object(mempool)
	{
	}
	inline log_login_out::log_login_out(xx::BBuffer* const& bb)
        : xx::Object(bb)
	{
        if (int r = FromBBufferCore(*bb)) throw r;
	}
    inline void log_login_out::ToBBuffer(xx::BBuffer& bb) const noexcept
    {
        bb.Write(this->id);
        bb.Write(this->account_id);
        bb.Write(this->type_id);
        bb.Write(this->ip);
        bb.Write(this->create_time);
    }
    inline int log_login_out::FromBBuffer(xx::BBuffer& bb) noexcept
    {
        return this->FromBBufferCore(bb);
    }
    inline int log_login_out::FromBBufferCore(xx::BBuffer& bb) noexcept
    {
        if (int r = bb.Read(this->id)) return r;
        if (int r = bb.Read(this->account_id)) return r;
        if (int r = bb.Read(this->type_id)) return r;
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->ip)) return r;
        if (int r = bb.Read(this->create_time)) return r;
        return 0;
    }

    inline void log_login_out::ToString(xx::String& s) const noexcept
    {
        if (this->memHeader().flags)
        {
        	s.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else this->memHeader().flags = 1;

        s.Append("{ \"pkgTypeName\":\"Tables.log_login_out\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(s);
        s.Append(" }");
        
        this->memHeader().flags = 0;
    }
    inline void log_login_out::ToStringCore(xx::String& s) const noexcept
    {
        this->BaseType::ToStringCore(s);
        s.Append(", \"id\":", this->id);
        s.Append(", \"account_id\":", this->account_id);
        s.Append(", \"type_id\":", this->type_id);
        if (this->ip) s.Append(", \"ip\":\"", this->ip, "\"");
        else s.Append(", \"ip\":nil");
        s.Append(", \"create_time\":", this->create_time);
    }
    inline void log_login_out::CopyTo(log_login_out* const& o) const noexcept
    {
        o->id = this->id;
        o->account_id = this->account_id;
        o->type_id = this->type_id;
        o->ip = this->ip;
        o->create_time = this->create_time;
    }
    inline log_login_out* log_login_out::MakeCopy() const noexcept
    {
        auto o = mempool->MPCreate<log_login_out>();
        this->CopyTo(o);
        return o;
    }
    inline log_login_out_p log_login_out::MakePtrCopy() const noexcept
    {
        return log_login_out_p(this->MakeCopy());
    }

	inline log_withdrawals::log_withdrawals(xx::MemPool* const& mempool) noexcept
        : xx::Object(mempool)
	{
	}
	inline log_withdrawals::log_withdrawals(xx::BBuffer* const& bb)
        : xx::Object(bb)
	{
        if (int r = FromBBufferCore(*bb)) throw r;
	}
    inline void log_withdrawals::ToBBuffer(xx::BBuffer& bb) const noexcept
    {
        bb.Write(this->id);
        bb.Write(this->withdrawals_id);
        bb.Write(this->person_in_charge);
        bb.Write(this->state_id);
        bb.Write(this->description);
        bb.Write(this->create_time);
    }
    inline int log_withdrawals::FromBBuffer(xx::BBuffer& bb) noexcept
    {
        return this->FromBBufferCore(bb);
    }
    inline int log_withdrawals::FromBBufferCore(xx::BBuffer& bb) noexcept
    {
        if (int r = bb.Read(this->id)) return r;
        if (int r = bb.Read(this->withdrawals_id)) return r;
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->person_in_charge)) return r;
        if (int r = bb.Read(this->state_id)) return r;
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->description)) return r;
        if (int r = bb.Read(this->create_time)) return r;
        return 0;
    }

    inline void log_withdrawals::ToString(xx::String& s) const noexcept
    {
        if (this->memHeader().flags)
        {
        	s.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else this->memHeader().flags = 1;

        s.Append("{ \"pkgTypeName\":\"Tables.log_withdrawals\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(s);
        s.Append(" }");
        
        this->memHeader().flags = 0;
    }
    inline void log_withdrawals::ToStringCore(xx::String& s) const noexcept
    {
        this->BaseType::ToStringCore(s);
        s.Append(", \"id\":", this->id);
        s.Append(", \"withdrawals_id\":", this->withdrawals_id);
        if (this->person_in_charge) s.Append(", \"person_in_charge\":\"", this->person_in_charge, "\"");
        else s.Append(", \"person_in_charge\":nil");
        s.Append(", \"state_id\":", this->state_id);
        if (this->description) s.Append(", \"description\":\"", this->description, "\"");
        else s.Append(", \"description\":nil");
        s.Append(", \"create_time\":", this->create_time);
    }
    inline void log_withdrawals::CopyTo(log_withdrawals* const& o) const noexcept
    {
        o->id = this->id;
        o->withdrawals_id = this->withdrawals_id;
        o->person_in_charge = this->person_in_charge;
        o->state_id = this->state_id;
        o->description = this->description;
        o->create_time = this->create_time;
    }
    inline log_withdrawals* log_withdrawals::MakeCopy() const noexcept
    {
        auto o = mempool->MPCreate<log_withdrawals>();
        this->CopyTo(o);
        return o;
    }
    inline log_withdrawals_p log_withdrawals::MakePtrCopy() const noexcept
    {
        return log_withdrawals_p(this->MakeCopy());
    }

	inline mail::mail(xx::MemPool* const& mempool) noexcept
        : xx::Object(mempool)
	{
	}
	inline mail::mail(xx::BBuffer* const& bb)
        : xx::Object(bb)
	{
        if (int r = FromBBufferCore(*bb)) throw r;
	}
    inline void mail::ToBBuffer(xx::BBuffer& bb) const noexcept
    {
        bb.Write(this->id);
        bb.Write(this->content);
        bb.Write(this->account_id);
        bb.Write(this->sender);
        bb.Write(this->create_time);
        bb.Write(this->is_read);
    }
    inline int mail::FromBBuffer(xx::BBuffer& bb) noexcept
    {
        return this->FromBBufferCore(bb);
    }
    inline int mail::FromBBufferCore(xx::BBuffer& bb) noexcept
    {
        if (int r = bb.Read(this->id)) return r;
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->content)) return r;
        if (int r = bb.Read(this->account_id)) return r;
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->sender)) return r;
        if (int r = bb.Read(this->create_time)) return r;
        if (int r = bb.Read(this->is_read)) return r;
        return 0;
    }

    inline void mail::ToString(xx::String& s) const noexcept
    {
        if (this->memHeader().flags)
        {
        	s.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else this->memHeader().flags = 1;

        s.Append("{ \"pkgTypeName\":\"Tables.mail\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(s);
        s.Append(" }");
        
        this->memHeader().flags = 0;
    }
    inline void mail::ToStringCore(xx::String& s) const noexcept
    {
        this->BaseType::ToStringCore(s);
        s.Append(", \"id\":", this->id);
        if (this->content) s.Append(", \"content\":\"", this->content, "\"");
        else s.Append(", \"content\":nil");
        s.Append(", \"account_id\":", this->account_id);
        if (this->sender) s.Append(", \"sender\":\"", this->sender, "\"");
        else s.Append(", \"sender\":nil");
        s.Append(", \"create_time\":", this->create_time);
        s.Append(", \"is_read\":", this->is_read);
    }
    inline void mail::CopyTo(mail* const& o) const noexcept
    {
        o->id = this->id;
        o->content = this->content;
        o->account_id = this->account_id;
        o->sender = this->sender;
        o->create_time = this->create_time;
        o->is_read = this->is_read;
    }
    inline mail* mail::MakeCopy() const noexcept
    {
        auto o = mempool->MPCreate<mail>();
        this->CopyTo(o);
        return o;
    }
    inline mail_p mail::MakePtrCopy() const noexcept
    {
        return mail_p(this->MakeCopy());
    }

	inline marquee::marquee(xx::MemPool* const& mempool) noexcept
        : xx::Object(mempool)
	{
	}
	inline marquee::marquee(xx::BBuffer* const& bb)
        : xx::Object(bb)
	{
        if (int r = FromBBufferCore(*bb)) throw r;
	}
    inline void marquee::ToBBuffer(xx::BBuffer& bb) const noexcept
    {
        bb.Write(this->id);
        bb.Write(this->content);
        bb.Write(this->frequency);
        bb.Write(this->update_time);
        bb.Write(this->create_time);
        bb.Write(this->enable);
    }
    inline int marquee::FromBBuffer(xx::BBuffer& bb) noexcept
    {
        return this->FromBBufferCore(bb);
    }
    inline int marquee::FromBBufferCore(xx::BBuffer& bb) noexcept
    {
        if (int r = bb.Read(this->id)) return r;
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->content)) return r;
        if (int r = bb.Read(this->frequency)) return r;
        if (int r = bb.Read(this->update_time)) return r;
        if (int r = bb.Read(this->create_time)) return r;
        if (int r = bb.Read(this->enable)) return r;
        return 0;
    }

    inline void marquee::ToString(xx::String& s) const noexcept
    {
        if (this->memHeader().flags)
        {
        	s.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else this->memHeader().flags = 1;

        s.Append("{ \"pkgTypeName\":\"Tables.marquee\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(s);
        s.Append(" }");
        
        this->memHeader().flags = 0;
    }
    inline void marquee::ToStringCore(xx::String& s) const noexcept
    {
        this->BaseType::ToStringCore(s);
        s.Append(", \"id\":", this->id);
        if (this->content) s.Append(", \"content\":\"", this->content, "\"");
        else s.Append(", \"content\":nil");
        s.Append(", \"frequency\":", this->frequency);
        s.Append(", \"update_time\":", this->update_time);
        s.Append(", \"create_time\":", this->create_time);
        s.Append(", \"enable\":", this->enable);
    }
    inline void marquee::CopyTo(marquee* const& o) const noexcept
    {
        o->id = this->id;
        o->content = this->content;
        o->frequency = this->frequency;
        o->update_time = this->update_time;
        o->create_time = this->create_time;
        o->enable = this->enable;
    }
    inline marquee* marquee::MakeCopy() const noexcept
    {
        auto o = mempool->MPCreate<marquee>();
        this->CopyTo(o);
        return o;
    }
    inline marquee_p marquee::MakePtrCopy() const noexcept
    {
        return marquee_p(this->MakeCopy());
    }

	inline notice::notice(xx::MemPool* const& mempool) noexcept
        : xx::Object(mempool)
	{
	}
	inline notice::notice(xx::BBuffer* const& bb)
        : xx::Object(bb)
	{
        if (int r = FromBBufferCore(*bb)) throw r;
	}
    inline void notice::ToBBuffer(xx::BBuffer& bb) const noexcept
    {
        bb.Write(this->id);
        bb.Write(this->content);
        bb.Write(this->notice_type_id);
        bb.Write(this->start_time);
        bb.Write(this->end_time);
        bb.Write(this->create_time);
    }
    inline int notice::FromBBuffer(xx::BBuffer& bb) noexcept
    {
        return this->FromBBufferCore(bb);
    }
    inline int notice::FromBBufferCore(xx::BBuffer& bb) noexcept
    {
        if (int r = bb.Read(this->id)) return r;
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->content)) return r;
        if (int r = bb.Read(this->notice_type_id)) return r;
        if (int r = bb.Read(this->start_time)) return r;
        if (int r = bb.Read(this->end_time)) return r;
        if (int r = bb.Read(this->create_time)) return r;
        return 0;
    }

    inline void notice::ToString(xx::String& s) const noexcept
    {
        if (this->memHeader().flags)
        {
        	s.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else this->memHeader().flags = 1;

        s.Append("{ \"pkgTypeName\":\"Tables.notice\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(s);
        s.Append(" }");
        
        this->memHeader().flags = 0;
    }
    inline void notice::ToStringCore(xx::String& s) const noexcept
    {
        this->BaseType::ToStringCore(s);
        s.Append(", \"id\":", this->id);
        if (this->content) s.Append(", \"content\":\"", this->content, "\"");
        else s.Append(", \"content\":nil");
        s.Append(", \"notice_type_id\":", this->notice_type_id);
        s.Append(", \"start_time\":", this->start_time);
        s.Append(", \"end_time\":", this->end_time);
        s.Append(", \"create_time\":", this->create_time);
    }
    inline void notice::CopyTo(notice* const& o) const noexcept
    {
        o->id = this->id;
        o->content = this->content;
        o->notice_type_id = this->notice_type_id;
        o->start_time = this->start_time;
        o->end_time = this->end_time;
        o->create_time = this->create_time;
    }
    inline notice* notice::MakeCopy() const noexcept
    {
        auto o = mempool->MPCreate<notice>();
        this->CopyTo(o);
        return o;
    }
    inline notice_p notice::MakePtrCopy() const noexcept
    {
        return notice_p(this->MakeCopy());
    }

	inline notice_read::notice_read(xx::MemPool* const& mempool) noexcept
        : xx::Object(mempool)
	{
	}
	inline notice_read::notice_read(xx::BBuffer* const& bb)
        : xx::Object(bb)
	{
        if (int r = FromBBufferCore(*bb)) throw r;
	}
    inline void notice_read::ToBBuffer(xx::BBuffer& bb) const noexcept
    {
        bb.Write(this->notice_id);
        bb.Write(this->account_id);
        bb.Write(this->create_time);
    }
    inline int notice_read::FromBBuffer(xx::BBuffer& bb) noexcept
    {
        return this->FromBBufferCore(bb);
    }
    inline int notice_read::FromBBufferCore(xx::BBuffer& bb) noexcept
    {
        if (int r = bb.Read(this->notice_id)) return r;
        if (int r = bb.Read(this->account_id)) return r;
        if (int r = bb.Read(this->create_time)) return r;
        return 0;
    }

    inline void notice_read::ToString(xx::String& s) const noexcept
    {
        if (this->memHeader().flags)
        {
        	s.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else this->memHeader().flags = 1;

        s.Append("{ \"pkgTypeName\":\"Tables.notice_read\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(s);
        s.Append(" }");
        
        this->memHeader().flags = 0;
    }
    inline void notice_read::ToStringCore(xx::String& s) const noexcept
    {
        this->BaseType::ToStringCore(s);
        s.Append(", \"notice_id\":", this->notice_id);
        s.Append(", \"account_id\":", this->account_id);
        s.Append(", \"create_time\":", this->create_time);
    }
    inline void notice_read::CopyTo(notice_read* const& o) const noexcept
    {
        o->notice_id = this->notice_id;
        o->account_id = this->account_id;
        o->create_time = this->create_time;
    }
    inline notice_read* notice_read::MakeCopy() const noexcept
    {
        auto o = mempool->MPCreate<notice_read>();
        this->CopyTo(o);
        return o;
    }
    inline notice_read_p notice_read::MakePtrCopy() const noexcept
    {
        return notice_read_p(this->MakeCopy());
    }

	inline notice_type::notice_type(xx::MemPool* const& mempool) noexcept
        : xx::Object(mempool)
	{
	}
	inline notice_type::notice_type(xx::BBuffer* const& bb)
        : xx::Object(bb)
	{
        if (int r = FromBBufferCore(*bb)) throw r;
	}
    inline void notice_type::ToBBuffer(xx::BBuffer& bb) const noexcept
    {
        bb.Write(this->id);
        bb.Write(this->name);
    }
    inline int notice_type::FromBBuffer(xx::BBuffer& bb) noexcept
    {
        return this->FromBBufferCore(bb);
    }
    inline int notice_type::FromBBufferCore(xx::BBuffer& bb) noexcept
    {
        if (int r = bb.Read(this->id)) return r;
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->name)) return r;
        return 0;
    }

    inline void notice_type::ToString(xx::String& s) const noexcept
    {
        if (this->memHeader().flags)
        {
        	s.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else this->memHeader().flags = 1;

        s.Append("{ \"pkgTypeName\":\"Tables.notice_type\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(s);
        s.Append(" }");
        
        this->memHeader().flags = 0;
    }
    inline void notice_type::ToStringCore(xx::String& s) const noexcept
    {
        this->BaseType::ToStringCore(s);
        s.Append(", \"id\":", this->id);
        if (this->name) s.Append(", \"name\":\"", this->name, "\"");
        else s.Append(", \"name\":nil");
    }
    inline void notice_type::CopyTo(notice_type* const& o) const noexcept
    {
        o->id = this->id;
        o->name = this->name;
    }
    inline notice_type* notice_type::MakeCopy() const noexcept
    {
        auto o = mempool->MPCreate<notice_type>();
        this->CopyTo(o);
        return o;
    }
    inline notice_type_p notice_type::MakePtrCopy() const noexcept
    {
        return notice_type_p(this->MakeCopy());
    }

	inline verify_code::verify_code(xx::MemPool* const& mempool) noexcept
        : xx::Object(mempool)
	{
	}
	inline verify_code::verify_code(xx::BBuffer* const& bb)
        : xx::Object(bb)
	{
        if (int r = FromBBufferCore(*bb)) throw r;
	}
    inline void verify_code::ToBBuffer(xx::BBuffer& bb) const noexcept
    {
        bb.Write(this->phone);
        bb.Write(this->content);
        bb.Write(this->create_time);
    }
    inline int verify_code::FromBBuffer(xx::BBuffer& bb) noexcept
    {
        return this->FromBBufferCore(bb);
    }
    inline int verify_code::FromBBufferCore(xx::BBuffer& bb) noexcept
    {
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->phone)) return r;
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->content)) return r;
        if (int r = bb.Read(this->create_time)) return r;
        return 0;
    }

    inline void verify_code::ToString(xx::String& s) const noexcept
    {
        if (this->memHeader().flags)
        {
        	s.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else this->memHeader().flags = 1;

        s.Append("{ \"pkgTypeName\":\"Tables.verify_code\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(s);
        s.Append(" }");
        
        this->memHeader().flags = 0;
    }
    inline void verify_code::ToStringCore(xx::String& s) const noexcept
    {
        this->BaseType::ToStringCore(s);
        if (this->phone) s.Append(", \"phone\":\"", this->phone, "\"");
        else s.Append(", \"phone\":nil");
        if (this->content) s.Append(", \"content\":\"", this->content, "\"");
        else s.Append(", \"content\":nil");
        s.Append(", \"create_time\":", this->create_time);
    }
    inline void verify_code::CopyTo(verify_code* const& o) const noexcept
    {
        o->phone = this->phone;
        o->content = this->content;
        o->create_time = this->create_time;
    }
    inline verify_code* verify_code::MakeCopy() const noexcept
    {
        auto o = mempool->MPCreate<verify_code>();
        this->CopyTo(o);
        return o;
    }
    inline verify_code_p verify_code::MakePtrCopy() const noexcept
    {
        return verify_code_p(this->MakeCopy());
    }

	inline vip::vip(xx::MemPool* const& mempool) noexcept
        : xx::Object(mempool)
	{
	}
	inline vip::vip(xx::BBuffer* const& bb)
        : xx::Object(bb)
	{
        if (int r = FromBBufferCore(*bb)) throw r;
	}
    inline void vip::ToBBuffer(xx::BBuffer& bb) const noexcept
    {
        bb.Write(this->id);
        bb.Write(this->total_recharge);
    }
    inline int vip::FromBBuffer(xx::BBuffer& bb) noexcept
    {
        return this->FromBBufferCore(bb);
    }
    inline int vip::FromBBufferCore(xx::BBuffer& bb) noexcept
    {
        if (int r = bb.Read(this->id)) return r;
        if (int r = bb.Read(this->total_recharge)) return r;
        return 0;
    }

    inline void vip::ToString(xx::String& s) const noexcept
    {
        if (this->memHeader().flags)
        {
        	s.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else this->memHeader().flags = 1;

        s.Append("{ \"pkgTypeName\":\"Tables.vip\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(s);
        s.Append(" }");
        
        this->memHeader().flags = 0;
    }
    inline void vip::ToStringCore(xx::String& s) const noexcept
    {
        this->BaseType::ToStringCore(s);
        s.Append(", \"id\":", this->id);
        s.Append(", \"total_recharge\":", this->total_recharge);
    }
    inline void vip::CopyTo(vip* const& o) const noexcept
    {
        o->id = this->id;
        o->total_recharge = this->total_recharge;
    }
    inline vip* vip::MakeCopy() const noexcept
    {
        auto o = mempool->MPCreate<vip>();
        this->CopyTo(o);
        return o;
    }
    inline vip_p vip::MakePtrCopy() const noexcept
    {
        return vip_p(this->MakeCopy());
    }

	inline withdrawals::withdrawals(xx::MemPool* const& mempool) noexcept
        : xx::Object(mempool)
	{
	}
	inline withdrawals::withdrawals(xx::BBuffer* const& bb)
        : xx::Object(bb)
	{
        if (int r = FromBBufferCore(*bb)) throw r;
	}
    inline void withdrawals::ToBBuffer(xx::BBuffer& bb) const noexcept
    {
        bb.Write(this->withdrawals_id);
        bb.Write(this->account_id);
        bb.Write(this->money);
        bb.Write(this->requirement);
        bb.Write(this->state_id);
        bb.Write(this->description);
        bb.Write(this->create_time);
        bb.Write(this->last_create_time);
    }
    inline int withdrawals::FromBBuffer(xx::BBuffer& bb) noexcept
    {
        return this->FromBBufferCore(bb);
    }
    inline int withdrawals::FromBBufferCore(xx::BBuffer& bb) noexcept
    {
        if (int r = bb.Read(this->withdrawals_id)) return r;
        if (int r = bb.Read(this->account_id)) return r;
        if (int r = bb.Read(this->money)) return r;
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->requirement)) return r;
        if (int r = bb.Read(this->state_id)) return r;
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->description)) return r;
        if (int r = bb.Read(this->create_time)) return r;
        if (int r = bb.Read(this->last_create_time)) return r;
        return 0;
    }

    inline void withdrawals::ToString(xx::String& s) const noexcept
    {
        if (this->memHeader().flags)
        {
        	s.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else this->memHeader().flags = 1;

        s.Append("{ \"pkgTypeName\":\"Tables.withdrawals\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(s);
        s.Append(" }");
        
        this->memHeader().flags = 0;
    }
    inline void withdrawals::ToStringCore(xx::String& s) const noexcept
    {
        this->BaseType::ToStringCore(s);
        s.Append(", \"withdrawals_id\":", this->withdrawals_id);
        s.Append(", \"account_id\":", this->account_id);
        s.Append(", \"money\":", this->money);
        if (this->requirement) s.Append(", \"requirement\":\"", this->requirement, "\"");
        else s.Append(", \"requirement\":nil");
        s.Append(", \"state_id\":", this->state_id);
        if (this->description) s.Append(", \"description\":\"", this->description, "\"");
        else s.Append(", \"description\":nil");
        s.Append(", \"create_time\":", this->create_time);
        s.Append(", \"last_create_time\":", this->last_create_time);
    }
    inline void withdrawals::CopyTo(withdrawals* const& o) const noexcept
    {
        o->withdrawals_id = this->withdrawals_id;
        o->account_id = this->account_id;
        o->money = this->money;
        o->requirement = this->requirement;
        o->state_id = this->state_id;
        o->description = this->description;
        o->create_time = this->create_time;
        o->last_create_time = this->last_create_time;
    }
    inline withdrawals* withdrawals::MakeCopy() const noexcept
    {
        auto o = mempool->MPCreate<withdrawals>();
        this->CopyTo(o);
        return o;
    }
    inline withdrawals_p withdrawals::MakePtrCopy() const noexcept
    {
        return withdrawals_p(this->MakeCopy());
    }

	inline withdrawals_state::withdrawals_state(xx::MemPool* const& mempool) noexcept
        : xx::Object(mempool)
	{
	}
	inline withdrawals_state::withdrawals_state(xx::BBuffer* const& bb)
        : xx::Object(bb)
	{
        if (int r = FromBBufferCore(*bb)) throw r;
	}
    inline void withdrawals_state::ToBBuffer(xx::BBuffer& bb) const noexcept
    {
        bb.Write(this->id);
        bb.Write(this->name);
    }
    inline int withdrawals_state::FromBBuffer(xx::BBuffer& bb) noexcept
    {
        return this->FromBBufferCore(bb);
    }
    inline int withdrawals_state::FromBBufferCore(xx::BBuffer& bb) noexcept
    {
        if (int r = bb.Read(this->id)) return r;
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->name)) return r;
        return 0;
    }

    inline void withdrawals_state::ToString(xx::String& s) const noexcept
    {
        if (this->memHeader().flags)
        {
        	s.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else this->memHeader().flags = 1;

        s.Append("{ \"pkgTypeName\":\"Tables.withdrawals_state\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(s);
        s.Append(" }");
        
        this->memHeader().flags = 0;
    }
    inline void withdrawals_state::ToStringCore(xx::String& s) const noexcept
    {
        this->BaseType::ToStringCore(s);
        s.Append(", \"id\":", this->id);
        if (this->name) s.Append(", \"name\":\"", this->name, "\"");
        else s.Append(", \"name\":nil");
    }
    inline void withdrawals_state::CopyTo(withdrawals_state* const& o) const noexcept
    {
        o->id = this->id;
        o->name = this->name;
    }
    inline withdrawals_state* withdrawals_state::MakeCopy() const noexcept
    {
        auto o = mempool->MPCreate<withdrawals_state>();
        this->CopyTo(o);
        return o;
    }
    inline withdrawals_state_p withdrawals_state::MakePtrCopy() const noexcept
    {
        return withdrawals_state_p(this->MakeCopy());
    }

	inline node::node(xx::MemPool* const& mempool) noexcept
        : xx::Object(mempool)
	{
	}
	inline node::node(xx::BBuffer* const& bb)
        : xx::Object(bb)
	{
        if (int r = FromBBufferCore(*bb)) throw r;
	}
    inline void node::ToBBuffer(xx::BBuffer& bb) const noexcept
    {
        bb.Write(this->id);
        bb.Write(this->pid);
    }
    inline int node::FromBBuffer(xx::BBuffer& bb) noexcept
    {
        return this->FromBBufferCore(bb);
    }
    inline int node::FromBBufferCore(xx::BBuffer& bb) noexcept
    {
        if (int r = bb.Read(this->id)) return r;
        if (int r = bb.Read(this->pid)) return r;
        return 0;
    }

    inline void node::ToString(xx::String& s) const noexcept
    {
        if (this->memHeader().flags)
        {
        	s.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else this->memHeader().flags = 1;

        s.Append("{ \"pkgTypeName\":\"Tables.node\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(s);
        s.Append(" }");
        
        this->memHeader().flags = 0;
    }
    inline void node::ToStringCore(xx::String& s) const noexcept
    {
        this->BaseType::ToStringCore(s);
        s.Append(", \"id\":", this->id);
        s.Append(", \"pid\":", this->pid);
    }
    inline void node::CopyTo(node* const& o) const noexcept
    {
        o->id = this->id;
        o->pid = this->pid;
    }
    inline node* node::MakeCopy() const noexcept
    {
        auto o = mempool->MPCreate<node>();
        this->CopyTo(o);
        return o;
    }
    inline node_p node::MakePtrCopy() const noexcept
    {
        return node_p(this->MakeCopy());
    }

}
}
namespace PKG
{
	inline void AllTypesRegister() noexcept
	{
        xx::MemPool::RegisterInternals();
	    xx::MemPool::Register<PKG::Foo, xx::Object>();
	    xx::MemPool::Register<xx::List<PKG::Foo_p>, xx::Object>();
	    xx::MemPool::Register<xx::List<xx::Object_p>, xx::Object>();
	    xx::MemPool::Register<PKG::FooEx, PKG::Foo>();
	    xx::MemPool::Register<PKG::Node, PKG::Tables::node>();
	    xx::MemPool::Register<PKG::Tables::node, xx::Object>();
	    xx::MemPool::Register<xx::List<PKG::Node_p>, xx::Object>();
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
	    xx::MemPool::Register<PKG::Tables::account, xx::Object>();
	    xx::MemPool::Register<PKG::Tables::chat, xx::Object>();
	    xx::MemPool::Register<PKG::Tables::config, xx::Object>();
	    xx::MemPool::Register<PKG::Tables::config_value, xx::Object>();
	    xx::MemPool::Register<PKG::Tables::datatype, xx::Object>();
	    xx::MemPool::Register<PKG::Tables::fish, xx::Object>();
	    xx::MemPool::Register<PKG::Tables::game, xx::Object>();
	    xx::MemPool::Register<PKG::Tables::game_catchfish, xx::Object>();
	    xx::MemPool::Register<PKG::Tables::game_catchfish_account, xx::Object>();
	    xx::MemPool::Register<PKG::Tables::game_catchfish_cannon, xx::Object>();
	    xx::MemPool::Register<PKG::Tables::game_catchfish_level, xx::Object>();
	    xx::MemPool::Register<PKG::Tables::game_catchfish_level_room, xx::Object>();
	    xx::MemPool::Register<PKG::Tables::log_account_avatar, xx::Object>();
	    xx::MemPool::Register<PKG::Tables::log_account_enabled, xx::Object>();
	    xx::MemPool::Register<PKG::Tables::log_account_money_safe, xx::Object>();
	    xx::MemPool::Register<PKG::Tables::log_account_nickname, xx::Object>();
	    xx::MemPool::Register<PKG::Tables::log_account_password, xx::Object>();
	    xx::MemPool::Register<PKG::Tables::log_account_phone, xx::Object>();
	    xx::MemPool::Register<PKG::Tables::log_account_recharge, xx::Object>();
	    xx::MemPool::Register<PKG::Tables::log_game, xx::Object>();
	    xx::MemPool::Register<PKG::Tables::log_game_catchfish, xx::Object>();
	    xx::MemPool::Register<PKG::Tables::log_game_catchfish_event, xx::Object>();
	    xx::MemPool::Register<PKG::Tables::log_game_catchfish_event_bulletdie, xx::Object>();
	    xx::MemPool::Register<PKG::Tables::log_game_catchfish_event_bulletmiss, xx::Object>();
	    xx::MemPool::Register<PKG::Tables::log_game_catchfish_event_fire, xx::Object>();
	    xx::MemPool::Register<PKG::Tables::log_game_catchfish_event_fishdie, xx::Object>();
	    xx::MemPool::Register<PKG::Tables::log_login_out, xx::Object>();
	    xx::MemPool::Register<PKG::Tables::log_withdrawals, xx::Object>();
	    xx::MemPool::Register<PKG::Tables::mail, xx::Object>();
	    xx::MemPool::Register<PKG::Tables::marquee, xx::Object>();
	    xx::MemPool::Register<PKG::Tables::notice, xx::Object>();
	    xx::MemPool::Register<PKG::Tables::notice_read, xx::Object>();
	    xx::MemPool::Register<PKG::Tables::notice_type, xx::Object>();
	    xx::MemPool::Register<PKG::Tables::verify_code, xx::Object>();
	    xx::MemPool::Register<PKG::Tables::vip, xx::Object>();
	    xx::MemPool::Register<PKG::Tables::withdrawals, xx::Object>();
	    xx::MemPool::Register<PKG::Tables::withdrawals_state, xx::Object>();
	}
}
