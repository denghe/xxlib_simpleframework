#pragma once
#include <xx_bbuffer.h>
#include <optional>

namespace DB
{
namespace Game
{
    // 对应 game_account 账号表
    struct Account;
    using Account_p = xx::Ptr<Account>;
	using Account_v = xx::Dock<Account>;

}
namespace Manage
{
    // 对应 manage_account 账号表
    struct Account;
    using Account_p = xx::Ptr<Account>;
	using Account_v = xx::Dock<Account>;

    // 对应 manage_role 身份表
    struct Role;
    using Role_p = xx::Ptr<Role>;
	using Role_v = xx::Dock<Role>;

    // 对应 manage_permission 权限表
    struct Permission;
    using Permission_p = xx::Ptr<Permission>;
	using Permission_v = xx::Dock<Permission>;

    // 对应 manage_bind_account_role 管理人员 & 身份 绑定表
    struct BindAccountRole;
    using BindAccountRole_p = xx::Ptr<BindAccountRole>;
	using BindAccountRole_v = xx::Dock<BindAccountRole>;

    // 对应 manage_bind_role_permission 身份 & 权限 绑定表
    struct BindRolePermission;
    using BindRolePermission_p = xx::Ptr<BindRolePermission>;
	using BindRolePermission_v = xx::Dock<BindRolePermission>;

}
namespace Game
{
    // 对应 game_account 账号表
    struct Account : xx::Object
    {
        // 自增主键
        int64_t id = 0;
        // 用户名( 唯一索引 )
        xx::String_p username;
        // 密码( 无索引 )
        xx::String_p password;

        typedef Account ThisType;
        typedef xx::Object BaseType;
	    Account();
		Account(Account const&) = delete;
		Account& operator=(Account const&) = delete;
        virtual void ToString(xx::String &str) const override;
        virtual void ToStringCore(xx::String &str) const override;
    };


}
namespace Manage
{
    // 对应 manage_account 账号表
    struct Account : xx::Object
    {
        // 自增主键
        int64_t id = 0;
        // 用户名( 唯一索引 )
        xx::String_p username;
        // 密码( 无索引 )
        xx::String_p password;

        typedef Account ThisType;
        typedef xx::Object BaseType;
	    Account();
		Account(Account const&) = delete;
		Account& operator=(Account const&) = delete;
        virtual void ToString(xx::String &str) const override;
        virtual void ToStringCore(xx::String &str) const override;
    };


    // 对应 manage_role 身份表
    struct Role : xx::Object
    {
        // 主键
        int32_t id = 0;
        // 身份名
        xx::String_p name;
        // 备注
        xx::String_p desc;

        typedef Role ThisType;
        typedef xx::Object BaseType;
	    Role();
		Role(Role const&) = delete;
		Role& operator=(Role const&) = delete;
        virtual void ToString(xx::String &str) const override;
        virtual void ToStringCore(xx::String &str) const override;
    };


    // 对应 manage_permission 权限表
    struct Permission : xx::Object
    {
        // 主键
        int32_t id = 0;
        // 分组依据( 仅用于显示 )
        xx::String_p group;
        // 权限名
        xx::String_p name;
        // 备注
        xx::String_p desc;

        typedef Permission ThisType;
        typedef xx::Object BaseType;
	    Permission();
		Permission(Permission const&) = delete;
		Permission& operator=(Permission const&) = delete;
        virtual void ToString(xx::String &str) const override;
        virtual void ToStringCore(xx::String &str) const override;
    };


    // 对应 manage_bind_account_role 管理人员 & 身份 绑定表
    struct BindAccountRole : xx::Object
    {
        // 管理人员主键
        int32_t account_id = 0;
        // 身份主键
        int32_t role_id = 0;

        typedef BindAccountRole ThisType;
        typedef xx::Object BaseType;
	    BindAccountRole();
		BindAccountRole(BindAccountRole const&) = delete;
		BindAccountRole& operator=(BindAccountRole const&) = delete;
        virtual void ToString(xx::String &str) const override;
        virtual void ToStringCore(xx::String &str) const override;
    };


    // 对应 manage_bind_role_permission 身份 & 权限 绑定表
    struct BindRolePermission : xx::Object
    {
        // 身份主键
        int32_t role_id = 0;
        // 权限主键
        int32_t permission_id = 0;

        typedef BindRolePermission ThisType;
        typedef xx::Object BaseType;
	    BindRolePermission();
		BindRolePermission(BindRolePermission const&) = delete;
		BindRolePermission& operator=(BindRolePermission const&) = delete;
        virtual void ToString(xx::String &str) const override;
        virtual void ToStringCore(xx::String &str) const override;
    };


}
namespace Game
{
	inline Account::Account()
	{
	}

    inline void Account::ToString(xx::String &str) const
    {
        if (tsFlags())
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else tsFlags() = 1;

        str.Append("{ \"type\" : \"Account\"");
        ToStringCore(str);
        str.Append(" }");
        
        tsFlags() = 0;
    }
    inline void Account::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"id\" : ", this->id);
        str.Append(", \"username\" : ", this->username);
        str.Append(", \"password\" : ", this->password);
    }
}
namespace Manage
{
	inline Account::Account()
	{
	}

    inline void Account::ToString(xx::String &str) const
    {
        if (tsFlags())
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else tsFlags() = 1;

        str.Append("{ \"type\" : \"Account\"");
        ToStringCore(str);
        str.Append(" }");
        
        tsFlags() = 0;
    }
    inline void Account::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"id\" : ", this->id);
        str.Append(", \"username\" : ", this->username);
        str.Append(", \"password\" : ", this->password);
    }
	inline Role::Role()
	{
	}

    inline void Role::ToString(xx::String &str) const
    {
        if (tsFlags())
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else tsFlags() = 1;

        str.Append("{ \"type\" : \"Role\"");
        ToStringCore(str);
        str.Append(" }");
        
        tsFlags() = 0;
    }
    inline void Role::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"id\" : ", this->id);
        str.Append(", \"name\" : ", this->name);
        str.Append(", \"desc\" : ", this->desc);
    }
	inline Permission::Permission()
	{
	}

    inline void Permission::ToString(xx::String &str) const
    {
        if (tsFlags())
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else tsFlags() = 1;

        str.Append("{ \"type\" : \"Permission\"");
        ToStringCore(str);
        str.Append(" }");
        
        tsFlags() = 0;
    }
    inline void Permission::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"id\" : ", this->id);
        str.Append(", \"group\" : ", this->group);
        str.Append(", \"name\" : ", this->name);
        str.Append(", \"desc\" : ", this->desc);
    }
	inline BindAccountRole::BindAccountRole()
	{
	}

    inline void BindAccountRole::ToString(xx::String &str) const
    {
        if (tsFlags())
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else tsFlags() = 1;

        str.Append("{ \"type\" : \"BindAccountRole\"");
        ToStringCore(str);
        str.Append(" }");
        
        tsFlags() = 0;
    }
    inline void BindAccountRole::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"account_id\" : ", this->account_id);
        str.Append(", \"role_id\" : ", this->role_id);
    }
	inline BindRolePermission::BindRolePermission()
	{
	}

    inline void BindRolePermission::ToString(xx::String &str) const
    {
        if (tsFlags())
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else tsFlags() = 1;

        str.Append("{ \"type\" : \"BindRolePermission\"");
        ToStringCore(str);
        str.Append(" }");
        
        tsFlags() = 0;
    }
    inline void BindRolePermission::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"role_id\" : ", this->role_id);
        str.Append(", \"permission_id\" : ", this->permission_id);
    }
}
}
namespace xx
{
	template<>
	struct MemmoveSupport<DB::Game::Account_v>
	{
		static const bool value = true;
    };
	template<>
	struct MemmoveSupport<DB::Manage::Account_v>
	{
		static const bool value = true;
    };
	template<>
	struct MemmoveSupport<DB::Manage::Role_v>
	{
		static const bool value = true;
    };
	template<>
	struct MemmoveSupport<DB::Manage::Permission_v>
	{
		static const bool value = true;
    };
	template<>
	struct MemmoveSupport<DB::Manage::BindAccountRole_v>
	{
		static const bool value = true;
    };
	template<>
	struct MemmoveSupport<DB::Manage::BindRolePermission_v>
	{
		static const bool value = true;
    };
}

namespace xx
{
}
