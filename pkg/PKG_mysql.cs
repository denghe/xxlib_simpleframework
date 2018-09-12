using System;
using xx;

namespace PKG
{
    public static class MySqlAppendExt
    {
        #region MySqlAppends
        #region Orginal
        public static void MySqlAppend<T>(this System.Text.StringBuilder sb, List<T> os, bool ignoreReadOnly = false) { if (os == null || os.dataLen == 0) throw new ArgumentNullException(); for (int i = 0; i < os.dataLen; ++i) { sb.MySqlAppend(os[i], ignoreReadOnly); sb.Append(", "); }; sb.Length -= 2; }

        public static void MySqlAppend<T>(this System.Text.StringBuilder sb, T v, bool ignoreReadOnly = false) { sb.Append(v); }
        public static void MySqlAppend<T>(this System.Text.StringBuilder sb, T? o, bool ignoreReadOnly = false) where T : struct { if (o.HasValue) { sb.MySqlAppend(o.Value); } else { sb.Append("null"); } }
        public static void MySqlAppend(this System.Text.StringBuilder sb, string v, bool ignoreReadOnly = false) { sb.Append("'" + v.Replace("'", "''") + "'"); }
        public static void MySqlAppend(this System.Text.StringBuilder sb, DateTime o, bool ignoreReadOnly = false) { sb.Append("'" + o.ToString("yyyy-MM-dd HH:mm:ss") + "'"); }
        public static void MySqlAppend(this System.Text.StringBuilder sb, List<DateTime> os, bool ignoreReadOnly = false) { if (os == null || os.dataLen == 0) throw new ArgumentNullException(); sb.Append("( "); for (int i = 0; i < os.dataLen; ++i) { sb.MySqlAppend(os[i]); sb.Append(", "); }; sb.Length -= 2; sb.Append(" )"); }
        public static void MySqlAppend(this System.Text.StringBuilder sb, List<byte> os, bool ignoreReadOnly = false) { if (os == null || os.dataLen == 0) throw new ArgumentNullException(); sb.Append("( "); for (int i = 0; i < os.dataLen; ++i) { sb.Append(os[i]); sb.Append(", "); }; sb.Length -= 2; sb.Append(" )"); }
        public static void MySqlAppend(this System.Text.StringBuilder sb, List<ushort> os, bool ignoreReadOnly = false) { if (os == null || os.dataLen == 0) throw new ArgumentNullException(); sb.Append("( "); for (int i = 0; i < os.dataLen; ++i) { sb.Append(os[i]); sb.Append(", "); }; sb.Length -= 2; sb.Append(" )"); }
        public static void MySqlAppend(this System.Text.StringBuilder sb, List<uint> os, bool ignoreReadOnly = false) { if (os == null || os.dataLen == 0) throw new ArgumentNullException(); sb.Append("( "); for (int i = 0; i < os.dataLen; ++i) { sb.Append(os[i]); sb.Append(", "); }; sb.Length -= 2; sb.Append(" )"); }
        public static void MySqlAppend(this System.Text.StringBuilder sb, List<ulong> os, bool ignoreReadOnly = false) { if (os == null || os.dataLen == 0) throw new ArgumentNullException(); sb.Append("( "); for (int i = 0; i < os.dataLen; ++i) { sb.Append(os[i]); sb.Append(", "); }; sb.Length -= 2; sb.Append(" )"); }
        public static void MySqlAppend(this System.Text.StringBuilder sb, List<sbyte> os, bool ignoreReadOnly = false) { if (os == null || os.dataLen == 0) throw new ArgumentNullException(); sb.Append("( "); for (int i = 0; i < os.dataLen; ++i) { sb.Append(os[i]); sb.Append(", "); }; sb.Length -= 2; sb.Append(" )"); }
        public static void MySqlAppend(this System.Text.StringBuilder sb, List<short> os, bool ignoreReadOnly = false) { if (os == null || os.dataLen == 0) throw new ArgumentNullException(); sb.Append("( "); for (int i = 0; i < os.dataLen; ++i) { sb.Append(os[i]); sb.Append(", "); }; sb.Length -= 2; sb.Append(" )"); }
        public static void MySqlAppend(this System.Text.StringBuilder sb, List<int> os, bool ignoreReadOnly = false) { if (os == null || os.dataLen == 0) throw new ArgumentNullException(); sb.Append("( "); for (int i = 0; i < os.dataLen; ++i) { sb.Append(os[i]); sb.Append(", "); }; sb.Length -= 2; sb.Append(" )"); }
        public static void MySqlAppend(this System.Text.StringBuilder sb, List<long> os, bool ignoreReadOnly = false) { if (os == null || os.dataLen == 0) throw new ArgumentNullException(); sb.Append("( "); for (int i = 0; i < os.dataLen; ++i) { sb.Append(os[i]); sb.Append(", "); }; sb.Length -= 2; sb.Append(" )"); }
        public static void MySqlAppend(this System.Text.StringBuilder sb, List<double> os, bool ignoreReadOnly = false) { if (os == null || os.dataLen == 0) throw new ArgumentNullException(); sb.Append("( "); for (int i = 0; i < os.dataLen; ++i) { sb.Append(os[i]); sb.Append(", "); }; sb.Length -= 2; sb.Append(" )"); }
        public static void MySqlAppend(this System.Text.StringBuilder sb, List<float> os, bool ignoreReadOnly = false) { if (os == null || os.dataLen == 0) throw new ArgumentNullException(); sb.Append("( "); for (int i = 0; i < os.dataLen; ++i) { sb.Append(os[i]); sb.Append(", "); }; sb.Length -= 2; sb.Append(" )"); }
        public static void MySqlAppend(this System.Text.StringBuilder sb, List<bool> os, bool ignoreReadOnly = false) { if (os == null || os.dataLen == 0) throw new ArgumentNullException(); sb.Append("( "); for (int i = 0; i < os.dataLen; ++i) { sb.Append(os[i]); sb.Append(", "); }; sb.Length -= 2; sb.Append(" )"); }
        #endregion

        public static void MySqlAppend(this System.Text.StringBuilder sb, Foo o, bool ignoreReadOnly = false)
        {
            sb.Append("(");
            sb.Append(o.id);
            sb.Append(", ");
            sb.MySqlAppend(o.age);
            sb.Append(", ");
            sb.MySqlAppend(o.info);
            sb.Append(", ");
            sb.Length -= 2;
            sb.Append(")");
        }
        public static void MySqlAppend(this System.Text.StringBuilder sb, Tables.account o, bool ignoreReadOnly = false)
        {
            sb.Append("(");
            sb.MySqlAppend(o.id);
            sb.Append(", ");
            sb.MySqlAppend(o.username);
            sb.Append(", ");
            sb.MySqlAppend(o.nickname);
            sb.Append(", ");
            sb.MySqlAppend(o.avatar_id);
            sb.Append(", ");
            sb.MySqlAppend(o.phone);
            sb.Append(", ");
            sb.MySqlAppend(o.password);
            sb.Append(", ");
            sb.MySqlAppend(o.money);
            sb.Append(", ");
            sb.MySqlAppend(o.money_safe);
            sb.Append(", ");
            sb.MySqlAppend(o.total_recharge);
            sb.Append(", ");
            sb.MySqlAppend(o.total_consume);
            sb.Append(", ");
            sb.MySqlAppend(o.total_withdraw);
            sb.Append(", ");
            sb.MySqlAppend(o.create_time);
            sb.Append(", ");
            sb.MySqlAppend(o.enabled);
            sb.Append(", ");
            sb.MySqlAppend(o.enter_lobby_time);
            sb.Append(", ");
            sb.MySqlAppend(o.log_game_id);
            sb.Append(", ");
            sb.Length -= 2;
            sb.Append(")");
        }
        public static void MySqlAppend(this System.Text.StringBuilder sb, Tables.chat o, bool ignoreReadOnly = false)
        {
            sb.Append("(");
            sb.MySqlAppend(o.id);
            sb.Append(", ");
            sb.MySqlAppend(o.content);
            sb.Append(", ");
            sb.MySqlAppend(o.account_id);
            sb.Append(", ");
            sb.MySqlAppend(o.create_time);
            sb.Append(", ");
            sb.Length -= 2;
            sb.Append(")");
        }
        public static void MySqlAppend(this System.Text.StringBuilder sb, Tables.config o, bool ignoreReadOnly = false)
        {
            sb.Append("(");
            sb.MySqlAppend(o.key);
            sb.Append(", ");
            sb.MySqlAppend(o.datatype_name);
            sb.Append(", ");
            sb.MySqlAppend(o.title);
            sb.Append(", ");
            sb.MySqlAppend(o.desc);
            sb.Append(", ");
            sb.MySqlAppend(o.create_time);
            sb.Append(", ");
            sb.Length -= 2;
            sb.Append(")");
        }
        public static void MySqlAppend(this System.Text.StringBuilder sb, Tables.config_value o, bool ignoreReadOnly = false)
        {
            sb.Append("(");
            sb.MySqlAppend(o.key);
            sb.Append(", ");
            sb.MySqlAppend(o.value_bigint);
            sb.Append(", ");
            sb.MySqlAppend(o.value_double);
            sb.Append(", ");
            sb.MySqlAppend(o.value_varchar);
            sb.Append(", ");
            sb.MySqlAppend(o.create_time);
            sb.Append(", ");
            sb.Length -= 2;
            sb.Append(")");
        }
        public static void MySqlAppend(this System.Text.StringBuilder sb, Tables.datatype o, bool ignoreReadOnly = false)
        {
            sb.Append("(");
            sb.MySqlAppend(o.name);
            sb.Append(", ");
            sb.Length -= 2;
            sb.Append(")");
        }
        public static void MySqlAppend(this System.Text.StringBuilder sb, Tables.fish o, bool ignoreReadOnly = false)
        {
            sb.Append("(");
            sb.MySqlAppend(o.id);
            sb.Append(", ");
            sb.MySqlAppend(o.name);
            sb.Append(", ");
            sb.MySqlAppend(o.minCoin);
            sb.Append(", ");
            sb.MySqlAppend(o.maxCoin);
            sb.Append(", ");
            sb.Length -= 2;
            sb.Append(")");
        }
        public static void MySqlAppend(this System.Text.StringBuilder sb, Tables.game o, bool ignoreReadOnly = false)
        {
            sb.Append("(");
            sb.MySqlAppend(o.id);
            sb.Append(", ");
            sb.MySqlAppend(o.name);
            sb.Append(", ");
            sb.MySqlAppend(o.type_name);
            sb.Append(", ");
            sb.MySqlAppend(o.enabled);
            sb.Append(", ");
            sb.Length -= 2;
            sb.Append(")");
        }
        public static void MySqlAppend(this System.Text.StringBuilder sb, Tables.game_catchfish o, bool ignoreReadOnly = false)
        {
            sb.Append("(");
            sb.MySqlAppend(o.id);
            sb.Append(", ");
            sb.MySqlAppend(o.exchange_coin_ratio);
            sb.Append(", ");
            sb.Length -= 2;
            sb.Append(")");
        }
        public static void MySqlAppend(this System.Text.StringBuilder sb, Tables.game_catchfish_account o, bool ignoreReadOnly = false)
        {
            sb.Append("(");
            sb.MySqlAppend(o.account_id);
            sb.Append(", ");
            sb.MySqlAppend(o.game_id);
            sb.Append(", ");
            sb.MySqlAppend(o.level_id);
            sb.Append(", ");
            sb.MySqlAppend(o.last_bet);
            sb.Append(", ");
            sb.MySqlAppend(o.last_cannon_id);
            sb.Append(", ");
            sb.Length -= 2;
            sb.Append(")");
        }
        public static void MySqlAppend(this System.Text.StringBuilder sb, Tables.game_catchfish_cannon o, bool ignoreReadOnly = false)
        {
            sb.Append("(");
            sb.MySqlAppend(o.id);
            sb.Append(", ");
            sb.MySqlAppend(o.vip_id);
            sb.Append(", ");
            sb.Length -= 2;
            sb.Append(")");
        }
        public static void MySqlAppend(this System.Text.StringBuilder sb, Tables.game_catchfish_level o, bool ignoreReadOnly = false)
        {
            sb.Append("(");
            sb.MySqlAppend(o.game_id);
            sb.Append(", ");
            sb.MySqlAppend(o.level_id);
            sb.Append(", ");
            sb.MySqlAppend(o.min_bet);
            sb.Append(", ");
            sb.MySqlAppend(o.max_bet);
            sb.Append(", ");
            sb.MySqlAppend(o.min_money);
            sb.Append(", ");
            sb.MySqlAppend(o.enabled);
            sb.Append(", ");
            sb.Length -= 2;
            sb.Append(")");
        }
        public static void MySqlAppend(this System.Text.StringBuilder sb, Tables.game_catchfish_level_room o, bool ignoreReadOnly = false)
        {
            sb.Append("(");
            sb.MySqlAppend(o.game_id);
            sb.Append(", ");
            sb.MySqlAppend(o.level_id);
            sb.Append(", ");
            sb.MySqlAppend(o.room_id);
            sb.Append(", ");
            sb.MySqlAppend(o.enabled);
            sb.Append(", ");
            sb.Length -= 2;
            sb.Append(")");
        }
        public static void MySqlAppend(this System.Text.StringBuilder sb, Tables.log_account_avatar o, bool ignoreReadOnly = false)
        {
            sb.Append("(");
            sb.MySqlAppend(o.id);
            sb.Append(", ");
            sb.MySqlAppend(o.account_id);
            sb.Append(", ");
            sb.MySqlAppend(o.avatar_id);
            sb.Append(", ");
            sb.MySqlAppend(o.create_time);
            sb.Append(", ");
            sb.Length -= 2;
            sb.Append(")");
        }
        public static void MySqlAppend(this System.Text.StringBuilder sb, Tables.log_account_enabled o, bool ignoreReadOnly = false)
        {
            sb.Append("(");
            sb.MySqlAppend(o.id);
            sb.Append(", ");
            sb.MySqlAppend(o.account_id);
            sb.Append(", ");
            sb.MySqlAppend(o.enabled);
            sb.Append(", ");
            sb.MySqlAppend(o.memo);
            sb.Append(", ");
            sb.MySqlAppend(o.create_time);
            sb.Append(", ");
            sb.Length -= 2;
            sb.Append(")");
        }
        public static void MySqlAppend(this System.Text.StringBuilder sb, Tables.log_account_money_safe o, bool ignoreReadOnly = false)
        {
            sb.Append("(");
            sb.MySqlAppend(o.id);
            sb.Append(", ");
            sb.MySqlAppend(o.account_id);
            sb.Append(", ");
            sb.MySqlAppend(o.value);
            sb.Append(", ");
            sb.MySqlAppend(o.create_time);
            sb.Append(", ");
            sb.Length -= 2;
            sb.Append(")");
        }
        public static void MySqlAppend(this System.Text.StringBuilder sb, Tables.log_account_nickname o, bool ignoreReadOnly = false)
        {
            sb.Append("(");
            sb.MySqlAppend(o.id);
            sb.Append(", ");
            sb.MySqlAppend(o.account_id);
            sb.Append(", ");
            sb.MySqlAppend(o.nickname);
            sb.Append(", ");
            sb.MySqlAppend(o.create_time);
            sb.Append(", ");
            sb.Length -= 2;
            sb.Append(")");
        }
        public static void MySqlAppend(this System.Text.StringBuilder sb, Tables.log_account_password o, bool ignoreReadOnly = false)
        {
            sb.Append("(");
            sb.MySqlAppend(o.id);
            sb.Append(", ");
            sb.MySqlAppend(o.account_id);
            sb.Append(", ");
            sb.MySqlAppend(o.password);
            sb.Append(", ");
            sb.MySqlAppend(o.create_time);
            sb.Append(", ");
            sb.Length -= 2;
            sb.Append(")");
        }
        public static void MySqlAppend(this System.Text.StringBuilder sb, Tables.log_account_phone o, bool ignoreReadOnly = false)
        {
            sb.Append("(");
            sb.MySqlAppend(o.id);
            sb.Append(", ");
            sb.MySqlAppend(o.account_id);
            sb.Append(", ");
            sb.MySqlAppend(o.phone);
            sb.Append(", ");
            sb.MySqlAppend(o.create_time);
            sb.Append(", ");
            sb.Length -= 2;
            sb.Append(")");
        }
        public static void MySqlAppend(this System.Text.StringBuilder sb, Tables.log_account_recharge o, bool ignoreReadOnly = false)
        {
            sb.Append("(");
            sb.MySqlAppend(o.token);
            sb.Append(", ");
            sb.MySqlAppend(o.account_id);
            sb.Append(", ");
            sb.MySqlAppend(o.money);
            sb.Append(", ");
            sb.MySqlAppend(o.memo);
            sb.Append(", ");
            sb.MySqlAppend(o.create_time);
            sb.Append(", ");
            sb.Length -= 2;
            sb.Append(")");
        }
        public static void MySqlAppend(this System.Text.StringBuilder sb, Tables.log_game o, bool ignoreReadOnly = false)
        {
            sb.Append("(");
            sb.MySqlAppend(o.id);
            sb.Append(", ");
            sb.MySqlAppend(o.game_id);
            sb.Append(", ");
            sb.Length -= 2;
            sb.Append(")");
        }
        public static void MySqlAppend(this System.Text.StringBuilder sb, Tables.log_game_catchfish o, bool ignoreReadOnly = false)
        {
            sb.Append("(");
            sb.MySqlAppend(o.id);
            sb.Append(", ");
            sb.MySqlAppend(o.level_id);
            sb.Append(", ");
            sb.MySqlAppend(o.room_id);
            sb.Append(", ");
            sb.MySqlAppend(o.account_id);
            sb.Append(", ");
            sb.MySqlAppend(o.enter_time);
            sb.Append(", ");
            sb.MySqlAppend(o.enter_money);
            sb.Append(", ");
            sb.MySqlAppend(o.leave_time);
            sb.Append(", ");
            sb.MySqlAppend(o.leave_money);
            sb.Append(", ");
            sb.MySqlAppend(o.consume);
            sb.Append(", ");
            sb.Length -= 2;
            sb.Append(")");
        }
        public static void MySqlAppend(this System.Text.StringBuilder sb, Tables.log_game_catchfish_event o, bool ignoreReadOnly = false)
        {
            sb.Append("(");
            sb.MySqlAppend(o.id);
            sb.Append(", ");
            sb.MySqlAppend(o.log_game_catchfish_id);
            sb.Append(", ");
            sb.MySqlAppend(o.event_type_id);
            sb.Append(", ");
            sb.MySqlAppend(o.frame_number);
            sb.Append(", ");
            sb.MySqlAppend(o.create_time);
            sb.Append(", ");
            sb.Length -= 2;
            sb.Append(")");
        }
        public static void MySqlAppend(this System.Text.StringBuilder sb, Tables.log_game_catchfish_event_bulletdie o, bool ignoreReadOnly = false)
        {
            sb.Append("(");
            sb.MySqlAppend(o.id);
            sb.Append(", ");
            sb.MySqlAppend(o.log_game_catchfish_id);
            sb.Append(", ");
            sb.MySqlAppend(o.type_id);
            sb.Append(", ");
            sb.MySqlAppend(o.bullet_id);
            sb.Append(", ");
            sb.MySqlAppend(o.coin);
            sb.Append(", ");
            sb.MySqlAppend(o.money);
            sb.Append(", ");
            sb.Length -= 2;
            sb.Append(")");
        }
        public static void MySqlAppend(this System.Text.StringBuilder sb, Tables.log_game_catchfish_event_bulletmiss o, bool ignoreReadOnly = false)
        {
            sb.Append("(");
            sb.MySqlAppend(o.id);
            sb.Append(", ");
            sb.MySqlAppend(o.log_game_catchfish_id);
            sb.Append(", ");
            sb.MySqlAppend(o.type_id);
            sb.Append(", ");
            sb.MySqlAppend(o.bullet_id);
            sb.Append(", ");
            sb.MySqlAppend(o.coin);
            sb.Append(", ");
            sb.MySqlAppend(o.money);
            sb.Append(", ");
            sb.Length -= 2;
            sb.Append(")");
        }
        public static void MySqlAppend(this System.Text.StringBuilder sb, Tables.log_game_catchfish_event_fire o, bool ignoreReadOnly = false)
        {
            sb.Append("(");
            sb.MySqlAppend(o.id);
            sb.Append(", ");
            sb.MySqlAppend(o.log_game_catchfish_id);
            sb.Append(", ");
            sb.MySqlAppend(o.type_id);
            sb.Append(", ");
            sb.MySqlAppend(o.bullet_id);
            sb.Append(", ");
            sb.MySqlAppend(o.coin);
            sb.Append(", ");
            sb.MySqlAppend(o.money);
            sb.Append(", ");
            sb.Length -= 2;
            sb.Append(")");
        }
        public static void MySqlAppend(this System.Text.StringBuilder sb, Tables.log_game_catchfish_event_fishdie o, bool ignoreReadOnly = false)
        {
            sb.Append("(");
            sb.MySqlAppend(o.id);
            sb.Append(", ");
            sb.MySqlAppend(o.log_game_catchfish_id);
            sb.Append(", ");
            sb.MySqlAppend(o.fish_type_id);
            sb.Append(", ");
            sb.MySqlAppend(o.fish_id);
            sb.Append(", ");
            sb.MySqlAppend(o.fish_ratio);
            sb.Append(", ");
            sb.MySqlAppend(o.bullet_coin);
            sb.Append(", ");
            sb.MySqlAppend(o.coin);
            sb.Append(", ");
            sb.MySqlAppend(o.money);
            sb.Append(", ");
            sb.Length -= 2;
            sb.Append(")");
        }
        public static void MySqlAppend(this System.Text.StringBuilder sb, Tables.log_login_out o, bool ignoreReadOnly = false)
        {
            sb.Append("(");
            sb.MySqlAppend(o.id);
            sb.Append(", ");
            sb.MySqlAppend(o.account_id);
            sb.Append(", ");
            sb.MySqlAppend(o.type_id);
            sb.Append(", ");
            sb.MySqlAppend(o.ip);
            sb.Append(", ");
            sb.MySqlAppend(o.create_time);
            sb.Append(", ");
            sb.Length -= 2;
            sb.Append(")");
        }
        public static void MySqlAppend(this System.Text.StringBuilder sb, Tables.log_withdrawals o, bool ignoreReadOnly = false)
        {
            sb.Append("(");
            sb.MySqlAppend(o.id);
            sb.Append(", ");
            sb.MySqlAppend(o.withdrawals_id);
            sb.Append(", ");
            sb.MySqlAppend(o.person_in_charge);
            sb.Append(", ");
            sb.MySqlAppend(o.state_id);
            sb.Append(", ");
            sb.MySqlAppend(o.description);
            sb.Append(", ");
            sb.MySqlAppend(o.create_time);
            sb.Append(", ");
            sb.Length -= 2;
            sb.Append(")");
        }
        public static void MySqlAppend(this System.Text.StringBuilder sb, Tables.mail o, bool ignoreReadOnly = false)
        {
            sb.Append("(");
            sb.MySqlAppend(o.id);
            sb.Append(", ");
            sb.MySqlAppend(o.content);
            sb.Append(", ");
            sb.MySqlAppend(o.account_id);
            sb.Append(", ");
            sb.MySqlAppend(o.sender);
            sb.Append(", ");
            sb.MySqlAppend(o.create_time);
            sb.Append(", ");
            sb.MySqlAppend(o.is_read);
            sb.Append(", ");
            sb.Length -= 2;
            sb.Append(")");
        }
        public static void MySqlAppend(this System.Text.StringBuilder sb, Tables.marquee o, bool ignoreReadOnly = false)
        {
            sb.Append("(");
            sb.MySqlAppend(o.id);
            sb.Append(", ");
            sb.MySqlAppend(o.content);
            sb.Append(", ");
            sb.MySqlAppend(o.frequency);
            sb.Append(", ");
            sb.MySqlAppend(o.update_time);
            sb.Append(", ");
            sb.MySqlAppend(o.create_time);
            sb.Append(", ");
            sb.MySqlAppend(o.enable);
            sb.Append(", ");
            sb.Length -= 2;
            sb.Append(")");
        }
        public static void MySqlAppend(this System.Text.StringBuilder sb, Tables.notice o, bool ignoreReadOnly = false)
        {
            sb.Append("(");
            sb.MySqlAppend(o.id);
            sb.Append(", ");
            sb.MySqlAppend(o.content);
            sb.Append(", ");
            sb.MySqlAppend(o.notice_type_id);
            sb.Append(", ");
            sb.MySqlAppend(o.start_time);
            sb.Append(", ");
            sb.MySqlAppend(o.end_time);
            sb.Append(", ");
            sb.MySqlAppend(o.create_time);
            sb.Append(", ");
            sb.Length -= 2;
            sb.Append(")");
        }
        public static void MySqlAppend(this System.Text.StringBuilder sb, Tables.notice_read o, bool ignoreReadOnly = false)
        {
            sb.Append("(");
            sb.MySqlAppend(o.notice_id);
            sb.Append(", ");
            sb.MySqlAppend(o.account_id);
            sb.Append(", ");
            sb.MySqlAppend(o.create_time);
            sb.Append(", ");
            sb.Length -= 2;
            sb.Append(")");
        }
        public static void MySqlAppend(this System.Text.StringBuilder sb, Tables.notice_type o, bool ignoreReadOnly = false)
        {
            sb.Append("(");
            sb.MySqlAppend(o.id);
            sb.Append(", ");
            sb.MySqlAppend(o.name);
            sb.Append(", ");
            sb.Length -= 2;
            sb.Append(")");
        }
        public static void MySqlAppend(this System.Text.StringBuilder sb, Tables.verify_code o, bool ignoreReadOnly = false)
        {
            sb.Append("(");
            sb.MySqlAppend(o.phone);
            sb.Append(", ");
            sb.MySqlAppend(o.content);
            sb.Append(", ");
            sb.MySqlAppend(o.create_time);
            sb.Append(", ");
            sb.Length -= 2;
            sb.Append(")");
        }
        public static void MySqlAppend(this System.Text.StringBuilder sb, Tables.vip o, bool ignoreReadOnly = false)
        {
            sb.Append("(");
            sb.MySqlAppend(o.id);
            sb.Append(", ");
            sb.MySqlAppend(o.total_recharge);
            sb.Append(", ");
            sb.Length -= 2;
            sb.Append(")");
        }
        public static void MySqlAppend(this System.Text.StringBuilder sb, Tables.withdrawals o, bool ignoreReadOnly = false)
        {
            sb.Append("(");
            sb.MySqlAppend(o.withdrawals_id);
            sb.Append(", ");
            sb.MySqlAppend(o.account_id);
            sb.Append(", ");
            sb.MySqlAppend(o.money);
            sb.Append(", ");
            sb.MySqlAppend(o.requirement);
            sb.Append(", ");
            sb.MySqlAppend(o.state_id);
            sb.Append(", ");
            sb.MySqlAppend(o.description);
            sb.Append(", ");
            sb.MySqlAppend(o.create_time);
            sb.Append(", ");
            sb.MySqlAppend(o.last_create_time);
            sb.Append(", ");
            sb.Length -= 2;
            sb.Append(")");
        }
        public static void MySqlAppend(this System.Text.StringBuilder sb, Tables.withdrawals_state o, bool ignoreReadOnly = false)
        {
            sb.Append("(");
            sb.MySqlAppend(o.id);
            sb.Append(", ");
            sb.MySqlAppend(o.name);
            sb.Append(", ");
            sb.Length -= 2;
            sb.Append(")");
        }
        public static void MySqlAppend(this System.Text.StringBuilder sb, Tables.node o, bool ignoreReadOnly = false)
        {
            sb.Append("(");
            sb.Append(o.id);
            sb.Append(", ");
            sb.MySqlAppend(o.pid);
            sb.Append(", ");
            sb.Length -= 2;
            sb.Append(")");
        }
        #endregion
    }

    public class MySqlFuncs
    {
        #region conn, cmd, sb, recordsAffecteds
        public MySql.Data.MySqlClient.MySqlConnection conn;
        public MySql.Data.MySqlClient.MySqlCommand cmd;
        public System.Text.StringBuilder sb = new System.Text.StringBuilder();
        private List<int> recordsAffecteds = new List<int>();

        public List<int> RecordsAffecteds
        {
            get { return recordsAffecteds; }
        }

        public int RecordsAffected
        {
            get { return recordsAffecteds[0]; }
        }

        public MySqlFuncs(MySql.Data.MySqlClient.MySqlConnection conn)
        {
            this.conn = conn;
            this.cmd = conn.CreateCommand();
        }
        #endregion


        /// <summary>
        /// 模拟从表查数据填充到 Foo. 顺序和数量一致应该与 Foo 中标有 Column 的类成员一致
        /// </summary>
        public T SelectFoo<T>
        (
            int id,
            int? age,
            string info
            , MySql.Data.MySqlClient.MySqlTransaction tran_ = null) where T : Foo, new()
        {
            cmd.Transaction = tran_;
            recordsAffecteds.Clear();
            sb.Clear();
            sb.Append(@"select ");
            sb.Append(id);
            sb.Append(@", ");
            sb.MySqlAppend(age);
            sb.Append(@", ");
            sb.MySqlAppend(info);
            cmd.CommandText = sb.ToString();

            using (var r = cmd.ExecuteReader())
            {
                recordsAffecteds.Add(r.RecordsAffected);
                if (!r.Read())
                {
                    return null;
                }
                return new T
                {
                    id = r.GetInt32(0), 
                    age = r.IsDBNull(1) ? null : (int?)r.GetInt32(1), 
                    info = r.IsDBNull(2) ? null : (string)r.GetString(2)
                };
            }
        }

        public Tuple <T0, int, List<T2>, int?> SelectFooFoo<T0, T2>
        (
            int id,
            int? age,
            string info
            , MySql.Data.MySqlClient.MySqlTransaction tran_ = null) where T0 : Foo, new() where T2 : Foo, new()
        {
            cmd.Transaction = tran_;
            recordsAffecteds.Clear();
            sb.Clear();
            sb.Append(@"
select ");
            sb.Append(id);
            sb.Append(@", ");
            sb.MySqlAppend(age);
            sb.Append(@", ");
            sb.MySqlAppend(info);
            sb.Append(@";
select ");
            sb.Append(id);
            sb.Append(@";
select ");
            sb.Append(id);
            sb.Append(@", ");
            sb.MySqlAppend(age);
            sb.Append(@", ");
            sb.MySqlAppend(info);
            sb.Append(@";
select ");
            sb.MySqlAppend(age);
            sb.Append(@";
");
            cmd.CommandText = sb.ToString();

            using (var r = cmd.ExecuteReader())
            {
                recordsAffecteds.Add(r.RecordsAffected);
                recordsAffecteds.Add(r.RecordsAffected);
                T0 rtv1 = null;
                if (r.Read())
                {
                    rtv1 = new T0
                    {
                    id = r.GetInt32(0), 
                    age = r.IsDBNull(1) ? null : (int?)r.GetInt32(1), 
                    info = r.IsDBNull(2) ? null : (string)r.GetString(2)
                    };
                }
                r.NextResult();
                recordsAffecteds.Add(r.RecordsAffected);
                var rtv2 = default(int);
                if (r.Read())
                {
                    rtv2 = r.GetInt32(0);
                }
                r.NextResult();
                recordsAffecteds.Add(r.RecordsAffected);
                var rtv3 = new List<T2>();
                while (r.Read())
                {
                    rtv3.Add(new T2
                    {
                        id = r.GetInt32(0),
                        age = r.IsDBNull(1) ? null : (int?)r.GetInt32(1),
                        info = r.IsDBNull(2) ? null : (string)r.GetString(2)
                    });
                }
                r.NextResult();
                recordsAffecteds.Add(r.RecordsAffected);
                var rtv4 = default(int?);
                if (r.Read())
                {
                    rtv4 = r.IsDBNull(0) ? null : (int?)r.GetInt32(0);
                }
                return new Tuple <T0, int, List<T2>, int?>(rtv1, rtv2, rtv3, rtv4);
            }
        }

        public List<T> SelectNodes<T>(MySql.Data.MySqlClient.MySqlTransaction tran_ = null) where T : Tables.node, new()
        {
            cmd.Transaction = tran_;
            cmd.CommandText = @"
select 1, null
union all
select 2, 1
union all
select 3, 1
union all
select 4, 2;
";
            var rtv = new List<T>();
            using (var r = cmd.ExecuteReader())
            {
                recordsAffecteds.Add(r.RecordsAffected);
                while (r.Read())
                {
                    rtv.Add(new T
                    {
                        id = r.GetInt32(0),
                        pid = r.IsDBNull(1) ? null : (int?)r.GetInt32(1)
                    });
                }
            }
            return rtv;
        }
    }
}
