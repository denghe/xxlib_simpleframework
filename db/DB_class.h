#pragma once
#include "xx.h"
#include <optional>


namespace DB
{
namespace Game
{
    // 对应 game_account 账号表
    class Account;
    using Account_p = xx::Ptr<Account>;
	using Account_r = xx::Ref<Account>;

}
namespace Manage
{
    // 对应 manage_account 账号表
    class Account;
    using Account_p = xx::Ptr<Account>;
	using Account_r = xx::Ref<Account>;

    // 对应 manage_role 身份表
    class Role;
    using Role_p = xx::Ptr<Role>;
	using Role_r = xx::Ref<Role>;

    // 对应 manage_permission 权限表
    class Permission;
    using Permission_p = xx::Ptr<Permission>;
	using Permission_r = xx::Ref<Permission>;

    // 对应 manage_bind_account_role 管理人员 & 身份 绑定表
    class BindAccountRole;
    using BindAccountRole_p = xx::Ptr<BindAccountRole>;
	using BindAccountRole_r = xx::Ref<BindAccountRole>;

    // 对应 manage_bind_role_permission 身份 & 权限 绑定表
    class BindRolePermission;
    using BindRolePermission_p = xx::Ptr<BindRolePermission>;
	using BindRolePermission_r = xx::Ref<BindRolePermission>;

}
namespace Game
{
    // 对应 game_account 账号表
    class Account : public xx::Object
    {
    public:
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
        virtual void ToString(xx::String &s) const override;
        virtual void ToStringCore(xx::String &s) const override;
    };


}
namespace Manage
{
    // 对应 manage_account 账号表
    class Account : public xx::Object
    {
    public:
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
        virtual void ToString(xx::String &s) const override;
        virtual void ToStringCore(xx::String &s) const override;
    };


    // 对应 manage_role 身份表
    class Role : public xx::Object
    {
    public:
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
        virtual void ToString(xx::String &s) const override;
        virtual void ToStringCore(xx::String &s) const override;
    };


    // 对应 manage_permission 权限表
    class Permission : public xx::Object
    {
    public:
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
        virtual void ToString(xx::String &s) const override;
        virtual void ToStringCore(xx::String &s) const override;
    };


    // 对应 manage_bind_account_role 管理人员 & 身份 绑定表
    class BindAccountRole : public xx::Object
    {
    public:
        // 管理人员主键
        int32_t account_id = 0;
        // 身份主键
        int32_t role_id = 0;

        typedef BindAccountRole ThisType;
        typedef xx::Object BaseType;
	    BindAccountRole();
		BindAccountRole(BindAccountRole const&) = delete;
		BindAccountRole& operator=(BindAccountRole const&) = delete;
        virtual void ToString(xx::String &s) const override;
        virtual void ToStringCore(xx::String &s) const override;
    };


    // 对应 manage_bind_role_permission 身份 & 权限 绑定表
    class BindRolePermission : public xx::Object
    {
    public:
        // 身份主键
        int32_t role_id = 0;
        // 权限主键
        int32_t permission_id = 0;

        typedef BindRolePermission ThisType;
        typedef xx::Object BaseType;
	    BindRolePermission();
		BindRolePermission(BindRolePermission const&) = delete;
		BindRolePermission& operator=(BindRolePermission const&) = delete;
        virtual void ToString(xx::String &s) const override;
        virtual void ToStringCore(xx::String &s) const override;
    };


}
namespace Game
{
	inline Account::Account()
	{
	}

    inline void Account::ToString(xx::String &s) const
    {
        if (memHeader().flags)
        {
        	s.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else memHeader().flags = 1;

        s.Append("{ \"type\" : \"Account\"");
        ToStringCore(s);
        s.Append(" }");
        
        memHeader().flags = 0;
    }
    inline void Account::ToStringCore(xx::String &s) const
    {
        this->BaseType::ToStringCore(s);
        str.Append(", \"id\" : ", this->id);
        if (this->username) str.Append(", \"username\" : \"", this->username, "\"");
        else str.Append(", \"username\" : nil");
        if (this->password) str.Append(", \"password\" : \"", this->password, "\"");
        else str.Append(", \"password\" : nil");
    }
}
namespace Manage
{
	inline Account::Account()
	{
	}

    inline void Account::ToString(xx::String &s) const
    {
        if (memHeader().flags)
        {
        	s.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else memHeader().flags = 1;

        s.Append("{ \"type\" : \"Account\"");
        ToStringCore(s);
        s.Append(" }");
        
        memHeader().flags = 0;
    }
    inline void Account::ToStringCore(xx::String &s) const
    {
        this->BaseType::ToStringCore(s);
        str.Append(", \"id\" : ", this->id);
        if (this->username) str.Append(", \"username\" : \"", this->username, "\"");
        else str.Append(", \"username\" : nil");
        if (this->password) str.Append(", \"password\" : \"", this->password, "\"");
        else str.Append(", \"password\" : nil");
    }
	inline Role::Role()
	{
	}

    inline void Role::ToString(xx::String &s) const
    {
        if (memHeader().flags)
        {
        	s.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else memHeader().flags = 1;

        s.Append("{ \"type\" : \"Role\"");
        ToStringCore(s);
        s.Append(" }");
        
        memHeader().flags = 0;
    }
    inline void Role::ToStringCore(xx::String &s) const
    {
        this->BaseType::ToStringCore(s);
        str.Append(", \"id\" : ", this->id);
        if (this->name) str.Append(", \"name\" : \"", this->name, "\"");
        else str.Append(", \"name\" : nil");
        if (this->desc) str.Append(", \"desc\" : \"", this->desc, "\"");
        else str.Append(", \"desc\" : nil");
    }
	inline Permission::Permission()
	{
	}

    inline void Permission::ToString(xx::String &s) const
    {
        if (memHeader().flags)
        {
        	s.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else memHeader().flags = 1;

        s.Append("{ \"type\" : \"Permission\"");
        ToStringCore(s);
        s.Append(" }");
        
        memHeader().flags = 0;
    }
    inline void Permission::ToStringCore(xx::String &s) const
    {
        this->BaseType::ToStringCore(s);
        str.Append(", \"id\" : ", this->id);
        if (this->group) str.Append(", \"group\" : \"", this->group, "\"");
        else str.Append(", \"group\" : nil");
        if (this->name) str.Append(", \"name\" : \"", this->name, "\"");
        else str.Append(", \"name\" : nil");
        if (this->desc) str.Append(", \"desc\" : \"", this->desc, "\"");
        else str.Append(", \"desc\" : nil");
    }
	inline BindAccountRole::BindAccountRole()
	{
	}

    inline void BindAccountRole::ToString(xx::String &s) const
    {
        if (memHeader().flags)
        {
        	s.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else memHeader().flags = 1;

        s.Append("{ \"type\" : \"BindAccountRole\"");
        ToStringCore(s);
        s.Append(" }");
        
        memHeader().flags = 0;
    }
    inline void BindAccountRole::ToStringCore(xx::String &s) const
    {
        this->BaseType::ToStringCore(s);
        str.Append(", \"account_id\" : ", this->account_id);
        str.Append(", \"role_id\" : ", this->role_id);
    }
	inline BindRolePermission::BindRolePermission()
	{
	}

    inline void BindRolePermission::ToString(xx::String &s) const
    {
        if (memHeader().flags)
        {
        	s.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else memHeader().flags = 1;

        s.Append("{ \"type\" : \"BindRolePermission\"");
        ToStringCore(s);
        s.Append(" }");
        
        memHeader().flags = 0;
    }
    inline void BindRolePermission::ToStringCore(xx::String &s) const
    {
        this->BaseType::ToStringCore(s);
        str.Append(", \"role_id\" : ", this->role_id);
        str.Append(", \"permission_id\" : ", this->permission_id);
    }
}
}