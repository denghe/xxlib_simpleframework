#pragma once
#include "xx.h"

namespace WEB
{
	struct PkgGenMd5
	{
		static constexpr char const* value = "af917fae71ebd6f269da6026e8cf9da5";
    };

    // 管理人员
    class Manager;
    using Manager_p = xx::Ptr<Manager>;
    using Manager_r = xx::Ref<Manager>;

    // 身份
    class Role;
    using Role_p = xx::Ptr<Role>;
    using Role_r = xx::Ref<Role>;

    // 权限
    class Permission;
    using Permission_p = xx::Ptr<Permission>;
    using Permission_r = xx::Ref<Permission>;

    // 管理人员_身份_绑定
    class BindManagerRole;
    using BindManagerRole_p = xx::Ptr<BindManagerRole>;
    using BindManagerRole_r = xx::Ref<BindManagerRole>;

    // 身份_权限_绑定
    class BindRolePermission;
    using BindRolePermission_p = xx::Ptr<BindRolePermission>;
    using BindRolePermission_r = xx::Ref<BindRolePermission>;

namespace Generic
{
    // 通用错误返回
    class Error;
    using Error_p = xx::Ptr<Error>;
    using Error_r = xx::Ref<Error>;

    // 默认 rpc 成功返回
    class Success;
    using Success_p = xx::Ptr<Success>;
    using Success_r = xx::Ref<Success>;

    class SomeLists;
    using SomeLists_p = xx::Ptr<SomeLists>;
    using SomeLists_r = xx::Ref<SomeLists>;

}
namespace WEB_testcpp3
{
    // 指令基类. 路由用户上下文. 校验身份权限.
    class AuthInfo;
    using AuthInfo_p = xx::Ptr<AuthInfo>;
    using AuthInfo_r = xx::Ref<AuthInfo>;

    class Cmd1;
    using Cmd1_p = xx::Ptr<Cmd1>;
    using Cmd1_r = xx::Ref<Cmd1>;

    class Cmd2;
    using Cmd2_p = xx::Ptr<Cmd2>;
    using Cmd2_r = xx::Ref<Cmd2>;

}
namespace Tables
{
    // 管理人员表
    class manager;
    using manager_p = xx::Ptr<manager>;
    using manager_r = xx::Ref<manager>;

    // 管理人员_身份_绑定表
    class manager_role;
    using manager_role_p = xx::Ptr<manager_role>;
    using manager_role_r = xx::Ref<manager_role>;

    // 权限表
    class permission;
    using permission_p = xx::Ptr<permission>;
    using permission_r = xx::Ref<permission>;

    // 管理人员身份表
    class role;
    using role_p = xx::Ptr<role>;
    using role_r = xx::Ref<role>;

    // 身份_权限_绑定表
    class role_permission;
    using role_permission_p = xx::Ptr<role_permission>;
    using role_permission_r = xx::Ref<role_permission>;

}
namespace WEB_testcpp3
{
    // 指令基类. 路由用户上下文. 校验身份权限.
    class AuthInfo : public xx::Object
    {
    public:
        int32_t id = 0;

        typedef AuthInfo ThisType;
        typedef xx::Object BaseType;
	    AuthInfo(xx::MemPool* const& mempool) noexcept;
	    AuthInfo(xx::BBuffer* const& bb);
		AuthInfo(AuthInfo const&) = delete;
		AuthInfo& operator=(AuthInfo const&) = delete;
        void ToString(xx::String& s) const noexcept override;
        void ToStringCore(xx::String& s) const noexcept override;
        void ToBBuffer(xx::BBuffer& bb) const noexcept override;
        int FromBBuffer(xx::BBuffer& bb) noexcept override;
        int FromBBufferCore(xx::BBuffer& bb) noexcept;
        void CopyTo(AuthInfo* const& o) const noexcept;
        AuthInfo* MakeCopy() const noexcept;
        AuthInfo_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
    };
}
namespace Tables
{
    // 管理人员表
    class manager : public xx::Object
    {
    public:
        int32_t id = 0;
        xx::String_p username;
        xx::String_p password;

        typedef manager ThisType;
        typedef xx::Object BaseType;
	    manager(xx::MemPool* const& mempool) noexcept;
	    manager(xx::BBuffer* const& bb);
		manager(manager const&) = delete;
		manager& operator=(manager const&) = delete;
        void ToString(xx::String& s) const noexcept override;
        void ToStringCore(xx::String& s) const noexcept override;
        void ToBBuffer(xx::BBuffer& bb) const noexcept override;
        int FromBBuffer(xx::BBuffer& bb) noexcept override;
        int FromBBufferCore(xx::BBuffer& bb) noexcept;
        void CopyTo(manager* const& o) const noexcept;
        manager* MakeCopy() const noexcept;
        manager_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
    };
    // 管理人员_身份_绑定表
    class manager_role : public xx::Object
    {
    public:
        int32_t manager_id = 0;
        int32_t role_id = 0;

        typedef manager_role ThisType;
        typedef xx::Object BaseType;
	    manager_role(xx::MemPool* const& mempool) noexcept;
	    manager_role(xx::BBuffer* const& bb);
		manager_role(manager_role const&) = delete;
		manager_role& operator=(manager_role const&) = delete;
        void ToString(xx::String& s) const noexcept override;
        void ToStringCore(xx::String& s) const noexcept override;
        void ToBBuffer(xx::BBuffer& bb) const noexcept override;
        int FromBBuffer(xx::BBuffer& bb) noexcept override;
        int FromBBufferCore(xx::BBuffer& bb) noexcept;
        void CopyTo(manager_role* const& o) const noexcept;
        manager_role* MakeCopy() const noexcept;
        manager_role_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
    };
    // 权限表
    class permission : public xx::Object
    {
    public:
        int32_t id = 0;
        xx::String_p group;
        xx::String_p name;
        xx::String_p desc;

        typedef permission ThisType;
        typedef xx::Object BaseType;
	    permission(xx::MemPool* const& mempool) noexcept;
	    permission(xx::BBuffer* const& bb);
		permission(permission const&) = delete;
		permission& operator=(permission const&) = delete;
        void ToString(xx::String& s) const noexcept override;
        void ToStringCore(xx::String& s) const noexcept override;
        void ToBBuffer(xx::BBuffer& bb) const noexcept override;
        int FromBBuffer(xx::BBuffer& bb) noexcept override;
        int FromBBufferCore(xx::BBuffer& bb) noexcept;
        void CopyTo(permission* const& o) const noexcept;
        permission* MakeCopy() const noexcept;
        permission_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
    };
    // 管理人员身份表
    class role : public xx::Object
    {
    public:
        int32_t id = 0;
        xx::String_p name;
        xx::String_p desc;

        typedef role ThisType;
        typedef xx::Object BaseType;
	    role(xx::MemPool* const& mempool) noexcept;
	    role(xx::BBuffer* const& bb);
		role(role const&) = delete;
		role& operator=(role const&) = delete;
        void ToString(xx::String& s) const noexcept override;
        void ToStringCore(xx::String& s) const noexcept override;
        void ToBBuffer(xx::BBuffer& bb) const noexcept override;
        int FromBBuffer(xx::BBuffer& bb) noexcept override;
        int FromBBufferCore(xx::BBuffer& bb) noexcept;
        void CopyTo(role* const& o) const noexcept;
        role* MakeCopy() const noexcept;
        role_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
    };
    // 身份_权限_绑定表
    class role_permission : public xx::Object
    {
    public:
        int32_t role_id = 0;
        int32_t permission_id = 0;

        typedef role_permission ThisType;
        typedef xx::Object BaseType;
	    role_permission(xx::MemPool* const& mempool) noexcept;
	    role_permission(xx::BBuffer* const& bb);
		role_permission(role_permission const&) = delete;
		role_permission& operator=(role_permission const&) = delete;
        void ToString(xx::String& s) const noexcept override;
        void ToStringCore(xx::String& s) const noexcept override;
        void ToBBuffer(xx::BBuffer& bb) const noexcept override;
        int FromBBuffer(xx::BBuffer& bb) noexcept override;
        int FromBBufferCore(xx::BBuffer& bb) noexcept;
        void CopyTo(role_permission* const& o) const noexcept;
        role_permission* MakeCopy() const noexcept;
        role_permission_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
    };
}
    // 管理人员
    class Manager : public WEB::Tables::manager
    {
    public:
        // 当前令牌( 页面每次 Load 时与存放于 Session 中的值做对比用 )
        xx::String_p token;
        // 最后访问的时间点( 页面每次 Load 时更新该值, 用于超时判断 )
        int64_t lastVisitTime = 0;

        typedef Manager ThisType;
        typedef WEB::Tables::manager BaseType;
	    Manager(xx::MemPool* const& mempool) noexcept;
	    Manager(xx::BBuffer* const& bb);
		Manager(Manager const&) = delete;
		Manager& operator=(Manager const&) = delete;
        void ToString(xx::String& s) const noexcept override;
        void ToStringCore(xx::String& s) const noexcept override;
        void ToBBuffer(xx::BBuffer& bb) const noexcept override;
        int FromBBuffer(xx::BBuffer& bb) noexcept override;
        int FromBBufferCore(xx::BBuffer& bb) noexcept;
        void CopyTo(Manager* const& o) const noexcept;
        Manager* MakeCopy() const noexcept;
        Manager_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
    };
    // 身份
    class Role : public WEB::Tables::role
    {
    public:

        typedef Role ThisType;
        typedef WEB::Tables::role BaseType;
	    Role(xx::MemPool* const& mempool) noexcept;
	    Role(xx::BBuffer* const& bb);
		Role(Role const&) = delete;
		Role& operator=(Role const&) = delete;
        void ToString(xx::String& s) const noexcept override;
        void ToStringCore(xx::String& s) const noexcept override;
        void ToBBuffer(xx::BBuffer& bb) const noexcept override;
        int FromBBuffer(xx::BBuffer& bb) noexcept override;
        int FromBBufferCore(xx::BBuffer& bb) noexcept;
        void CopyTo(Role* const& o) const noexcept;
        Role* MakeCopy() const noexcept;
        Role_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
    };
    // 权限
    class Permission : public WEB::Tables::permission
    {
    public:

        typedef Permission ThisType;
        typedef WEB::Tables::permission BaseType;
	    Permission(xx::MemPool* const& mempool) noexcept;
	    Permission(xx::BBuffer* const& bb);
		Permission(Permission const&) = delete;
		Permission& operator=(Permission const&) = delete;
        void ToString(xx::String& s) const noexcept override;
        void ToStringCore(xx::String& s) const noexcept override;
        void ToBBuffer(xx::BBuffer& bb) const noexcept override;
        int FromBBuffer(xx::BBuffer& bb) noexcept override;
        int FromBBufferCore(xx::BBuffer& bb) noexcept;
        void CopyTo(Permission* const& o) const noexcept;
        Permission* MakeCopy() const noexcept;
        Permission_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
    };
    // 管理人员_身份_绑定
    class BindManagerRole : public WEB::Tables::manager_role
    {
    public:

        typedef BindManagerRole ThisType;
        typedef WEB::Tables::manager_role BaseType;
	    BindManagerRole(xx::MemPool* const& mempool) noexcept;
	    BindManagerRole(xx::BBuffer* const& bb);
		BindManagerRole(BindManagerRole const&) = delete;
		BindManagerRole& operator=(BindManagerRole const&) = delete;
        void ToString(xx::String& s) const noexcept override;
        void ToStringCore(xx::String& s) const noexcept override;
        void ToBBuffer(xx::BBuffer& bb) const noexcept override;
        int FromBBuffer(xx::BBuffer& bb) noexcept override;
        int FromBBufferCore(xx::BBuffer& bb) noexcept;
        void CopyTo(BindManagerRole* const& o) const noexcept;
        BindManagerRole* MakeCopy() const noexcept;
        BindManagerRole_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
    };
    // 身份_权限_绑定
    class BindRolePermission : public WEB::Tables::role_permission
    {
    public:

        typedef BindRolePermission ThisType;
        typedef WEB::Tables::role_permission BaseType;
	    BindRolePermission(xx::MemPool* const& mempool) noexcept;
	    BindRolePermission(xx::BBuffer* const& bb);
		BindRolePermission(BindRolePermission const&) = delete;
		BindRolePermission& operator=(BindRolePermission const&) = delete;
        void ToString(xx::String& s) const noexcept override;
        void ToStringCore(xx::String& s) const noexcept override;
        void ToBBuffer(xx::BBuffer& bb) const noexcept override;
        int FromBBuffer(xx::BBuffer& bb) noexcept override;
        int FromBBufferCore(xx::BBuffer& bb) noexcept;
        void CopyTo(BindRolePermission* const& o) const noexcept;
        BindRolePermission* MakeCopy() const noexcept;
        BindRolePermission_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
    };
namespace Generic
{
    // 通用错误返回
    class Error : public xx::Object
    {
    public:
        int32_t errNum = 0;
        xx::String_p errMsg;

        typedef Error ThisType;
        typedef xx::Object BaseType;
	    Error(xx::MemPool* const& mempool) noexcept;
	    Error(xx::BBuffer* const& bb);
		Error(Error const&) = delete;
		Error& operator=(Error const&) = delete;
        void ToString(xx::String& s) const noexcept override;
        void ToStringCore(xx::String& s) const noexcept override;
        void ToBBuffer(xx::BBuffer& bb) const noexcept override;
        int FromBBuffer(xx::BBuffer& bb) noexcept override;
        int FromBBufferCore(xx::BBuffer& bb) noexcept;
        void CopyTo(Error* const& o) const noexcept;
        Error* MakeCopy() const noexcept;
        Error_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
    };
    // 默认 rpc 成功返回
    class Success : public xx::Object
    {
    public:

        typedef Success ThisType;
        typedef xx::Object BaseType;
	    Success(xx::MemPool* const& mempool) noexcept;
	    Success(xx::BBuffer* const& bb);
		Success(Success const&) = delete;
		Success& operator=(Success const&) = delete;
        void ToString(xx::String& s) const noexcept override;
        void ToStringCore(xx::String& s) const noexcept override;
        void ToBBuffer(xx::BBuffer& bb) const noexcept override;
        int FromBBuffer(xx::BBuffer& bb) noexcept override;
        int FromBBufferCore(xx::BBuffer& bb) noexcept;
        void CopyTo(Success* const& o) const noexcept;
        Success* MakeCopy() const noexcept;
        Success_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
    };
    class SomeLists : public xx::Object
    {
    public:
        xx::List_p<int32_t> ints;
        xx::List_p<int64_t> longs;
        xx::List_p<xx::String_p> strings;
        xx::List_p<WEB::Manager_p> Managers;
        xx::List_p<WEB::Role_p> Roles;
        xx::List_p<WEB::Permission_p> Permissions;
        xx::List_p<WEB::BindManagerRole_p> BindManagerRoles;
        xx::List_p<WEB::BindRolePermission_p> BindRolePermissions;

        typedef SomeLists ThisType;
        typedef xx::Object BaseType;
	    SomeLists(xx::MemPool* const& mempool) noexcept;
	    SomeLists(xx::BBuffer* const& bb);
		SomeLists(SomeLists const&) = delete;
		SomeLists& operator=(SomeLists const&) = delete;
        void ToString(xx::String& s) const noexcept override;
        void ToStringCore(xx::String& s) const noexcept override;
        void ToBBuffer(xx::BBuffer& bb) const noexcept override;
        int FromBBuffer(xx::BBuffer& bb) noexcept override;
        int FromBBufferCore(xx::BBuffer& bb) noexcept;
        void CopyTo(SomeLists* const& o) const noexcept;
        SomeLists* MakeCopy() const noexcept;
        SomeLists_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
    };
}
namespace WEB_testcpp3
{
    class Cmd1 : public WEB::WEB_testcpp3::AuthInfo
    {
    public:

        typedef Cmd1 ThisType;
        typedef WEB::WEB_testcpp3::AuthInfo BaseType;
	    Cmd1(xx::MemPool* const& mempool) noexcept;
	    Cmd1(xx::BBuffer* const& bb);
		Cmd1(Cmd1 const&) = delete;
		Cmd1& operator=(Cmd1 const&) = delete;
        void ToString(xx::String& s) const noexcept override;
        void ToStringCore(xx::String& s) const noexcept override;
        void ToBBuffer(xx::BBuffer& bb) const noexcept override;
        int FromBBuffer(xx::BBuffer& bb) noexcept override;
        int FromBBufferCore(xx::BBuffer& bb) noexcept;
        void CopyTo(Cmd1* const& o) const noexcept;
        Cmd1* MakeCopy() const noexcept;
        Cmd1_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
    };
    class Cmd2 : public WEB::WEB_testcpp3::AuthInfo
    {
    public:

        typedef Cmd2 ThisType;
        typedef WEB::WEB_testcpp3::AuthInfo BaseType;
	    Cmd2(xx::MemPool* const& mempool) noexcept;
	    Cmd2(xx::BBuffer* const& bb);
		Cmd2(Cmd2 const&) = delete;
		Cmd2& operator=(Cmd2 const&) = delete;
        void ToString(xx::String& s) const noexcept override;
        void ToStringCore(xx::String& s) const noexcept override;
        void ToBBuffer(xx::BBuffer& bb) const noexcept override;
        int FromBBuffer(xx::BBuffer& bb) noexcept override;
        int FromBBufferCore(xx::BBuffer& bb) noexcept;
        void CopyTo(Cmd2* const& o) const noexcept;
        Cmd2* MakeCopy() const noexcept;
        Cmd2_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
    };
}
}
namespace xx
{
	template<> struct TypeId<WEB::Manager> { static const uint16_t value = 3; };
	template<> struct TypeId<WEB::Tables::manager> { static const uint16_t value = 4; };
	template<> struct TypeId<WEB::Role> { static const uint16_t value = 5; };
	template<> struct TypeId<WEB::Tables::role> { static const uint16_t value = 11; };
	template<> struct TypeId<WEB::Permission> { static const uint16_t value = 6; };
	template<> struct TypeId<WEB::Tables::permission> { static const uint16_t value = 10; };
	template<> struct TypeId<WEB::BindManagerRole> { static const uint16_t value = 7; };
	template<> struct TypeId<WEB::Tables::manager_role> { static const uint16_t value = 9; };
	template<> struct TypeId<WEB::BindRolePermission> { static const uint16_t value = 8; };
	template<> struct TypeId<WEB::Tables::role_permission> { static const uint16_t value = 12; };
	template<> struct TypeId<WEB::Generic::Error> { static const uint16_t value = 22; };
	template<> struct TypeId<WEB::Generic::Success> { static const uint16_t value = 23; };
	template<> struct TypeId<WEB::Generic::SomeLists> { static const uint16_t value = 13; };
	template<> struct TypeId<xx::List<int32_t>> { static const uint16_t value = 14; };
	template<> struct TypeId<xx::List<int64_t>> { static const uint16_t value = 15; };
	template<> struct TypeId<xx::List<xx::String_p>> { static const uint16_t value = 16; };
	template<> struct TypeId<xx::List<WEB::Manager_p>> { static const uint16_t value = 17; };
	template<> struct TypeId<xx::List<WEB::Role_p>> { static const uint16_t value = 18; };
	template<> struct TypeId<xx::List<WEB::Permission_p>> { static const uint16_t value = 19; };
	template<> struct TypeId<xx::List<WEB::BindManagerRole_p>> { static const uint16_t value = 20; };
	template<> struct TypeId<xx::List<WEB::BindRolePermission_p>> { static const uint16_t value = 21; };
	template<> struct TypeId<WEB::WEB_testcpp3::AuthInfo> { static const uint16_t value = 24; };
	template<> struct TypeId<WEB::WEB_testcpp3::Cmd1> { static const uint16_t value = 25; };
	template<> struct TypeId<WEB::WEB_testcpp3::Cmd2> { static const uint16_t value = 26; };
}
namespace WEB
{
	inline Manager::Manager(xx::MemPool* const& mempool) noexcept
        : WEB::Tables::manager(mempool)
	{
	}
	inline Manager::Manager(xx::BBuffer* const& bb)
        : WEB::Tables::manager(bb)
	{
        if (int r = FromBBufferCore(*bb)) throw r;
	}
    inline void Manager::ToBBuffer(xx::BBuffer& bb) const noexcept
    {
        this->BaseType::ToBBuffer(bb);
        bb.Write(this->token);
        bb.Write(this->lastVisitTime);
    }
    inline int Manager::FromBBuffer(xx::BBuffer& bb) noexcept
    {
        if (int r = this->BaseType::FromBBuffer(bb)) return r;
        return this->FromBBufferCore(bb);
    }
    inline int Manager::FromBBufferCore(xx::BBuffer& bb) noexcept
    {
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->token)) return r;
        if (int r = bb.Read(this->lastVisitTime)) return r;
        return 0;
    }

    inline void Manager::ToString(xx::String& s) const noexcept
    {
        if (this->memHeader().flags)
        {
        	s.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else this->memHeader().flags = 1;

        s.Append("{ \"pkgTypeName\":\"Manager\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(s);
        s.Append(" }");
        
        this->memHeader().flags = 0;
    }
    inline void Manager::ToStringCore(xx::String& s) const noexcept
    {
        this->BaseType::ToStringCore(s);
        if (this->token) s.Append(", \"token\":\"", this->token, "\"");
        else s.Append(", \"token\":nil");
        s.Append(", \"lastVisitTime\":", this->lastVisitTime);
    }
    inline void Manager::CopyTo(Manager* const& o) const noexcept
    {
        this->BaseType::CopyTo(o);
        o->token = this->token;
        o->lastVisitTime = this->lastVisitTime;
    }
    inline Manager* Manager::MakeCopy() const noexcept
    {
        auto o = mempool->MPCreate<Manager>();
        this->CopyTo(o);
        return o;
    }
    inline Manager_p Manager::MakePtrCopy() const noexcept
    {
        return Manager_p(this->MakeCopy());
    }

	inline Role::Role(xx::MemPool* const& mempool) noexcept
        : WEB::Tables::role(mempool)
	{
	}
	inline Role::Role(xx::BBuffer* const& bb)
        : WEB::Tables::role(bb)
	{
        if (int r = FromBBufferCore(*bb)) throw r;
	}
    inline void Role::ToBBuffer(xx::BBuffer& bb) const noexcept
    {
        this->BaseType::ToBBuffer(bb);
    }
    inline int Role::FromBBuffer(xx::BBuffer& bb) noexcept
    {
        if (int r = this->BaseType::FromBBuffer(bb)) return r;
        return this->FromBBufferCore(bb);
    }
    inline int Role::FromBBufferCore(xx::BBuffer& bb) noexcept
    {
        return 0;
    }

    inline void Role::ToString(xx::String& s) const noexcept
    {
        if (this->memHeader().flags)
        {
        	s.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else this->memHeader().flags = 1;

        s.Append("{ \"pkgTypeName\":\"Role\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(s);
        s.Append(" }");
        
        this->memHeader().flags = 0;
    }
    inline void Role::ToStringCore(xx::String& s) const noexcept
    {
        this->BaseType::ToStringCore(s);
    }
    inline void Role::CopyTo(Role* const& o) const noexcept
    {
        this->BaseType::CopyTo(o);
    }
    inline Role* Role::MakeCopy() const noexcept
    {
        auto o = mempool->MPCreate<Role>();
        this->CopyTo(o);
        return o;
    }
    inline Role_p Role::MakePtrCopy() const noexcept
    {
        return Role_p(this->MakeCopy());
    }

	inline Permission::Permission(xx::MemPool* const& mempool) noexcept
        : WEB::Tables::permission(mempool)
	{
	}
	inline Permission::Permission(xx::BBuffer* const& bb)
        : WEB::Tables::permission(bb)
	{
        if (int r = FromBBufferCore(*bb)) throw r;
	}
    inline void Permission::ToBBuffer(xx::BBuffer& bb) const noexcept
    {
        this->BaseType::ToBBuffer(bb);
    }
    inline int Permission::FromBBuffer(xx::BBuffer& bb) noexcept
    {
        if (int r = this->BaseType::FromBBuffer(bb)) return r;
        return this->FromBBufferCore(bb);
    }
    inline int Permission::FromBBufferCore(xx::BBuffer& bb) noexcept
    {
        return 0;
    }

    inline void Permission::ToString(xx::String& s) const noexcept
    {
        if (this->memHeader().flags)
        {
        	s.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else this->memHeader().flags = 1;

        s.Append("{ \"pkgTypeName\":\"Permission\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(s);
        s.Append(" }");
        
        this->memHeader().flags = 0;
    }
    inline void Permission::ToStringCore(xx::String& s) const noexcept
    {
        this->BaseType::ToStringCore(s);
    }
    inline void Permission::CopyTo(Permission* const& o) const noexcept
    {
        this->BaseType::CopyTo(o);
    }
    inline Permission* Permission::MakeCopy() const noexcept
    {
        auto o = mempool->MPCreate<Permission>();
        this->CopyTo(o);
        return o;
    }
    inline Permission_p Permission::MakePtrCopy() const noexcept
    {
        return Permission_p(this->MakeCopy());
    }

	inline BindManagerRole::BindManagerRole(xx::MemPool* const& mempool) noexcept
        : WEB::Tables::manager_role(mempool)
	{
	}
	inline BindManagerRole::BindManagerRole(xx::BBuffer* const& bb)
        : WEB::Tables::manager_role(bb)
	{
        if (int r = FromBBufferCore(*bb)) throw r;
	}
    inline void BindManagerRole::ToBBuffer(xx::BBuffer& bb) const noexcept
    {
        this->BaseType::ToBBuffer(bb);
    }
    inline int BindManagerRole::FromBBuffer(xx::BBuffer& bb) noexcept
    {
        if (int r = this->BaseType::FromBBuffer(bb)) return r;
        return this->FromBBufferCore(bb);
    }
    inline int BindManagerRole::FromBBufferCore(xx::BBuffer& bb) noexcept
    {
        return 0;
    }

    inline void BindManagerRole::ToString(xx::String& s) const noexcept
    {
        if (this->memHeader().flags)
        {
        	s.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else this->memHeader().flags = 1;

        s.Append("{ \"pkgTypeName\":\"BindManagerRole\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(s);
        s.Append(" }");
        
        this->memHeader().flags = 0;
    }
    inline void BindManagerRole::ToStringCore(xx::String& s) const noexcept
    {
        this->BaseType::ToStringCore(s);
    }
    inline void BindManagerRole::CopyTo(BindManagerRole* const& o) const noexcept
    {
        this->BaseType::CopyTo(o);
    }
    inline BindManagerRole* BindManagerRole::MakeCopy() const noexcept
    {
        auto o = mempool->MPCreate<BindManagerRole>();
        this->CopyTo(o);
        return o;
    }
    inline BindManagerRole_p BindManagerRole::MakePtrCopy() const noexcept
    {
        return BindManagerRole_p(this->MakeCopy());
    }

	inline BindRolePermission::BindRolePermission(xx::MemPool* const& mempool) noexcept
        : WEB::Tables::role_permission(mempool)
	{
	}
	inline BindRolePermission::BindRolePermission(xx::BBuffer* const& bb)
        : WEB::Tables::role_permission(bb)
	{
        if (int r = FromBBufferCore(*bb)) throw r;
	}
    inline void BindRolePermission::ToBBuffer(xx::BBuffer& bb) const noexcept
    {
        this->BaseType::ToBBuffer(bb);
    }
    inline int BindRolePermission::FromBBuffer(xx::BBuffer& bb) noexcept
    {
        if (int r = this->BaseType::FromBBuffer(bb)) return r;
        return this->FromBBufferCore(bb);
    }
    inline int BindRolePermission::FromBBufferCore(xx::BBuffer& bb) noexcept
    {
        return 0;
    }

    inline void BindRolePermission::ToString(xx::String& s) const noexcept
    {
        if (this->memHeader().flags)
        {
        	s.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else this->memHeader().flags = 1;

        s.Append("{ \"pkgTypeName\":\"BindRolePermission\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(s);
        s.Append(" }");
        
        this->memHeader().flags = 0;
    }
    inline void BindRolePermission::ToStringCore(xx::String& s) const noexcept
    {
        this->BaseType::ToStringCore(s);
    }
    inline void BindRolePermission::CopyTo(BindRolePermission* const& o) const noexcept
    {
        this->BaseType::CopyTo(o);
    }
    inline BindRolePermission* BindRolePermission::MakeCopy() const noexcept
    {
        auto o = mempool->MPCreate<BindRolePermission>();
        this->CopyTo(o);
        return o;
    }
    inline BindRolePermission_p BindRolePermission::MakePtrCopy() const noexcept
    {
        return BindRolePermission_p(this->MakeCopy());
    }

namespace Generic
{
	inline Error::Error(xx::MemPool* const& mempool) noexcept
        : xx::Object(mempool)
	{
	}
	inline Error::Error(xx::BBuffer* const& bb)
        : xx::Object(bb)
	{
        if (int r = FromBBufferCore(*bb)) throw r;
	}
    inline void Error::ToBBuffer(xx::BBuffer& bb) const noexcept
    {
        bb.Write(this->errNum);
        bb.Write(this->errMsg);
    }
    inline int Error::FromBBuffer(xx::BBuffer& bb) noexcept
    {
        return this->FromBBufferCore(bb);
    }
    inline int Error::FromBBufferCore(xx::BBuffer& bb) noexcept
    {
        if (int r = bb.Read(this->errNum)) return r;
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->errMsg)) return r;
        return 0;
    }

    inline void Error::ToString(xx::String& s) const noexcept
    {
        if (this->memHeader().flags)
        {
        	s.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else this->memHeader().flags = 1;

        s.Append("{ \"pkgTypeName\":\"Generic.Error\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(s);
        s.Append(" }");
        
        this->memHeader().flags = 0;
    }
    inline void Error::ToStringCore(xx::String& s) const noexcept
    {
        this->BaseType::ToStringCore(s);
        s.Append(", \"errNum\":", this->errNum);
        if (this->errMsg) s.Append(", \"errMsg\":\"", this->errMsg, "\"");
        else s.Append(", \"errMsg\":nil");
    }
    inline void Error::CopyTo(Error* const& o) const noexcept
    {
        o->errNum = this->errNum;
        o->errMsg = this->errMsg;
    }
    inline Error* Error::MakeCopy() const noexcept
    {
        auto o = mempool->MPCreate<Error>();
        this->CopyTo(o);
        return o;
    }
    inline Error_p Error::MakePtrCopy() const noexcept
    {
        return Error_p(this->MakeCopy());
    }

	inline Success::Success(xx::MemPool* const& mempool) noexcept
        : xx::Object(mempool)
	{
	}
	inline Success::Success(xx::BBuffer* const& bb)
        : xx::Object(bb)
	{
        if (int r = FromBBufferCore(*bb)) throw r;
	}
    inline void Success::ToBBuffer(xx::BBuffer& bb) const noexcept
    {
    }
    inline int Success::FromBBuffer(xx::BBuffer& bb) noexcept
    {
        return this->FromBBufferCore(bb);
    }
    inline int Success::FromBBufferCore(xx::BBuffer& bb) noexcept
    {
        return 0;
    }

    inline void Success::ToString(xx::String& s) const noexcept
    {
        if (this->memHeader().flags)
        {
        	s.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else this->memHeader().flags = 1;

        s.Append("{ \"pkgTypeName\":\"Generic.Success\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(s);
        s.Append(" }");
        
        this->memHeader().flags = 0;
    }
    inline void Success::ToStringCore(xx::String& s) const noexcept
    {
        this->BaseType::ToStringCore(s);
    }
    inline void Success::CopyTo(Success* const& o) const noexcept
    {
    }
    inline Success* Success::MakeCopy() const noexcept
    {
        auto o = mempool->MPCreate<Success>();
        this->CopyTo(o);
        return o;
    }
    inline Success_p Success::MakePtrCopy() const noexcept
    {
        return Success_p(this->MakeCopy());
    }

	inline SomeLists::SomeLists(xx::MemPool* const& mempool) noexcept
        : xx::Object(mempool)
	{
	}
	inline SomeLists::SomeLists(xx::BBuffer* const& bb)
        : xx::Object(bb)
	{
        if (int r = FromBBufferCore(*bb)) throw r;
	}
    inline void SomeLists::ToBBuffer(xx::BBuffer& bb) const noexcept
    {
        bb.Write(this->ints);
        bb.Write(this->longs);
        bb.Write(this->strings);
        bb.Write(this->Managers);
        bb.Write(this->Roles);
        bb.Write(this->Permissions);
        bb.Write(this->BindManagerRoles);
        bb.Write(this->BindRolePermissions);
    }
    inline int SomeLists::FromBBuffer(xx::BBuffer& bb) noexcept
    {
        return this->FromBBufferCore(bb);
    }
    inline int SomeLists::FromBBufferCore(xx::BBuffer& bb) noexcept
    {
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->ints)) return r;
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->longs)) return r;
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->strings)) return r;
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->Managers)) return r;
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->Roles)) return r;
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->Permissions)) return r;
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->BindManagerRoles)) return r;
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->BindRolePermissions)) return r;
        return 0;
    }

    inline void SomeLists::ToString(xx::String& s) const noexcept
    {
        if (this->memHeader().flags)
        {
        	s.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else this->memHeader().flags = 1;

        s.Append("{ \"pkgTypeName\":\"Generic.SomeLists\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(s);
        s.Append(" }");
        
        this->memHeader().flags = 0;
    }
    inline void SomeLists::ToStringCore(xx::String& s) const noexcept
    {
        this->BaseType::ToStringCore(s);
        s.Append(", \"ints\":", this->ints);
        s.Append(", \"longs\":", this->longs);
        s.Append(", \"strings\":", this->strings);
        s.Append(", \"Managers\":", this->Managers);
        s.Append(", \"Roles\":", this->Roles);
        s.Append(", \"Permissions\":", this->Permissions);
        s.Append(", \"BindManagerRoles\":", this->BindManagerRoles);
        s.Append(", \"BindRolePermissions\":", this->BindRolePermissions);
    }
    inline void SomeLists::CopyTo(SomeLists* const& o) const noexcept
    {
        o->ints = this->ints;
        o->longs = this->longs;
        o->strings = this->strings;
        o->Managers = this->Managers;
        o->Roles = this->Roles;
        o->Permissions = this->Permissions;
        o->BindManagerRoles = this->BindManagerRoles;
        o->BindRolePermissions = this->BindRolePermissions;
    }
    inline SomeLists* SomeLists::MakeCopy() const noexcept
    {
        auto o = mempool->MPCreate<SomeLists>();
        this->CopyTo(o);
        return o;
    }
    inline SomeLists_p SomeLists::MakePtrCopy() const noexcept
    {
        return SomeLists_p(this->MakeCopy());
    }

}
namespace WEB_testcpp3
{
	inline AuthInfo::AuthInfo(xx::MemPool* const& mempool) noexcept
        : xx::Object(mempool)
	{
	}
	inline AuthInfo::AuthInfo(xx::BBuffer* const& bb)
        : xx::Object(bb)
	{
        if (int r = FromBBufferCore(*bb)) throw r;
	}
    inline void AuthInfo::ToBBuffer(xx::BBuffer& bb) const noexcept
    {
        bb.Write(this->id);
    }
    inline int AuthInfo::FromBBuffer(xx::BBuffer& bb) noexcept
    {
        return this->FromBBufferCore(bb);
    }
    inline int AuthInfo::FromBBufferCore(xx::BBuffer& bb) noexcept
    {
        if (int r = bb.Read(this->id)) return r;
        return 0;
    }

    inline void AuthInfo::ToString(xx::String& s) const noexcept
    {
        if (this->memHeader().flags)
        {
        	s.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else this->memHeader().flags = 1;

        s.Append("{ \"pkgTypeName\":\"WEB_testcpp3.AuthInfo\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(s);
        s.Append(" }");
        
        this->memHeader().flags = 0;
    }
    inline void AuthInfo::ToStringCore(xx::String& s) const noexcept
    {
        this->BaseType::ToStringCore(s);
        s.Append(", \"id\":", this->id);
    }
    inline void AuthInfo::CopyTo(AuthInfo* const& o) const noexcept
    {
        o->id = this->id;
    }
    inline AuthInfo* AuthInfo::MakeCopy() const noexcept
    {
        auto o = mempool->MPCreate<AuthInfo>();
        this->CopyTo(o);
        return o;
    }
    inline AuthInfo_p AuthInfo::MakePtrCopy() const noexcept
    {
        return AuthInfo_p(this->MakeCopy());
    }

	inline Cmd1::Cmd1(xx::MemPool* const& mempool) noexcept
        : WEB::WEB_testcpp3::AuthInfo(mempool)
	{
	}
	inline Cmd1::Cmd1(xx::BBuffer* const& bb)
        : WEB::WEB_testcpp3::AuthInfo(bb)
	{
        if (int r = FromBBufferCore(*bb)) throw r;
	}
    inline void Cmd1::ToBBuffer(xx::BBuffer& bb) const noexcept
    {
        this->BaseType::ToBBuffer(bb);
    }
    inline int Cmd1::FromBBuffer(xx::BBuffer& bb) noexcept
    {
        if (int r = this->BaseType::FromBBuffer(bb)) return r;
        return this->FromBBufferCore(bb);
    }
    inline int Cmd1::FromBBufferCore(xx::BBuffer& bb) noexcept
    {
        return 0;
    }

    inline void Cmd1::ToString(xx::String& s) const noexcept
    {
        if (this->memHeader().flags)
        {
        	s.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else this->memHeader().flags = 1;

        s.Append("{ \"pkgTypeName\":\"WEB_testcpp3.Cmd1\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(s);
        s.Append(" }");
        
        this->memHeader().flags = 0;
    }
    inline void Cmd1::ToStringCore(xx::String& s) const noexcept
    {
        this->BaseType::ToStringCore(s);
    }
    inline void Cmd1::CopyTo(Cmd1* const& o) const noexcept
    {
        this->BaseType::CopyTo(o);
    }
    inline Cmd1* Cmd1::MakeCopy() const noexcept
    {
        auto o = mempool->MPCreate<Cmd1>();
        this->CopyTo(o);
        return o;
    }
    inline Cmd1_p Cmd1::MakePtrCopy() const noexcept
    {
        return Cmd1_p(this->MakeCopy());
    }

	inline Cmd2::Cmd2(xx::MemPool* const& mempool) noexcept
        : WEB::WEB_testcpp3::AuthInfo(mempool)
	{
	}
	inline Cmd2::Cmd2(xx::BBuffer* const& bb)
        : WEB::WEB_testcpp3::AuthInfo(bb)
	{
        if (int r = FromBBufferCore(*bb)) throw r;
	}
    inline void Cmd2::ToBBuffer(xx::BBuffer& bb) const noexcept
    {
        this->BaseType::ToBBuffer(bb);
    }
    inline int Cmd2::FromBBuffer(xx::BBuffer& bb) noexcept
    {
        if (int r = this->BaseType::FromBBuffer(bb)) return r;
        return this->FromBBufferCore(bb);
    }
    inline int Cmd2::FromBBufferCore(xx::BBuffer& bb) noexcept
    {
        return 0;
    }

    inline void Cmd2::ToString(xx::String& s) const noexcept
    {
        if (this->memHeader().flags)
        {
        	s.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else this->memHeader().flags = 1;

        s.Append("{ \"pkgTypeName\":\"WEB_testcpp3.Cmd2\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(s);
        s.Append(" }");
        
        this->memHeader().flags = 0;
    }
    inline void Cmd2::ToStringCore(xx::String& s) const noexcept
    {
        this->BaseType::ToStringCore(s);
    }
    inline void Cmd2::CopyTo(Cmd2* const& o) const noexcept
    {
        this->BaseType::CopyTo(o);
    }
    inline Cmd2* Cmd2::MakeCopy() const noexcept
    {
        auto o = mempool->MPCreate<Cmd2>();
        this->CopyTo(o);
        return o;
    }
    inline Cmd2_p Cmd2::MakePtrCopy() const noexcept
    {
        return Cmd2_p(this->MakeCopy());
    }

}
namespace Tables
{
	inline manager::manager(xx::MemPool* const& mempool) noexcept
        : xx::Object(mempool)
	{
	}
	inline manager::manager(xx::BBuffer* const& bb)
        : xx::Object(bb)
	{
        if (int r = FromBBufferCore(*bb)) throw r;
	}
    inline void manager::ToBBuffer(xx::BBuffer& bb) const noexcept
    {
        bb.Write(this->id);
        bb.Write(this->username);
        bb.Write(this->password);
    }
    inline int manager::FromBBuffer(xx::BBuffer& bb) noexcept
    {
        return this->FromBBufferCore(bb);
    }
    inline int manager::FromBBufferCore(xx::BBuffer& bb) noexcept
    {
        if (int r = bb.Read(this->id)) return r;
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->username)) return r;
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->password)) return r;
        return 0;
    }

    inline void manager::ToString(xx::String& s) const noexcept
    {
        if (this->memHeader().flags)
        {
        	s.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else this->memHeader().flags = 1;

        s.Append("{ \"pkgTypeName\":\"Tables.manager\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(s);
        s.Append(" }");
        
        this->memHeader().flags = 0;
    }
    inline void manager::ToStringCore(xx::String& s) const noexcept
    {
        this->BaseType::ToStringCore(s);
        s.Append(", \"id\":", this->id);
        if (this->username) s.Append(", \"username\":\"", this->username, "\"");
        else s.Append(", \"username\":nil");
        if (this->password) s.Append(", \"password\":\"", this->password, "\"");
        else s.Append(", \"password\":nil");
    }
    inline void manager::CopyTo(manager* const& o) const noexcept
    {
        o->id = this->id;
        o->username = this->username;
        o->password = this->password;
    }
    inline manager* manager::MakeCopy() const noexcept
    {
        auto o = mempool->MPCreate<manager>();
        this->CopyTo(o);
        return o;
    }
    inline manager_p manager::MakePtrCopy() const noexcept
    {
        return manager_p(this->MakeCopy());
    }

	inline manager_role::manager_role(xx::MemPool* const& mempool) noexcept
        : xx::Object(mempool)
	{
	}
	inline manager_role::manager_role(xx::BBuffer* const& bb)
        : xx::Object(bb)
	{
        if (int r = FromBBufferCore(*bb)) throw r;
	}
    inline void manager_role::ToBBuffer(xx::BBuffer& bb) const noexcept
    {
        bb.Write(this->manager_id);
        bb.Write(this->role_id);
    }
    inline int manager_role::FromBBuffer(xx::BBuffer& bb) noexcept
    {
        return this->FromBBufferCore(bb);
    }
    inline int manager_role::FromBBufferCore(xx::BBuffer& bb) noexcept
    {
        if (int r = bb.Read(this->manager_id)) return r;
        if (int r = bb.Read(this->role_id)) return r;
        return 0;
    }

    inline void manager_role::ToString(xx::String& s) const noexcept
    {
        if (this->memHeader().flags)
        {
        	s.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else this->memHeader().flags = 1;

        s.Append("{ \"pkgTypeName\":\"Tables.manager_role\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(s);
        s.Append(" }");
        
        this->memHeader().flags = 0;
    }
    inline void manager_role::ToStringCore(xx::String& s) const noexcept
    {
        this->BaseType::ToStringCore(s);
        s.Append(", \"manager_id\":", this->manager_id);
        s.Append(", \"role_id\":", this->role_id);
    }
    inline void manager_role::CopyTo(manager_role* const& o) const noexcept
    {
        o->manager_id = this->manager_id;
        o->role_id = this->role_id;
    }
    inline manager_role* manager_role::MakeCopy() const noexcept
    {
        auto o = mempool->MPCreate<manager_role>();
        this->CopyTo(o);
        return o;
    }
    inline manager_role_p manager_role::MakePtrCopy() const noexcept
    {
        return manager_role_p(this->MakeCopy());
    }

	inline permission::permission(xx::MemPool* const& mempool) noexcept
        : xx::Object(mempool)
	{
	}
	inline permission::permission(xx::BBuffer* const& bb)
        : xx::Object(bb)
	{
        if (int r = FromBBufferCore(*bb)) throw r;
	}
    inline void permission::ToBBuffer(xx::BBuffer& bb) const noexcept
    {
        bb.Write(this->id);
        bb.Write(this->group);
        bb.Write(this->name);
        bb.Write(this->desc);
    }
    inline int permission::FromBBuffer(xx::BBuffer& bb) noexcept
    {
        return this->FromBBufferCore(bb);
    }
    inline int permission::FromBBufferCore(xx::BBuffer& bb) noexcept
    {
        if (int r = bb.Read(this->id)) return r;
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->group)) return r;
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->name)) return r;
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->desc)) return r;
        return 0;
    }

    inline void permission::ToString(xx::String& s) const noexcept
    {
        if (this->memHeader().flags)
        {
        	s.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else this->memHeader().flags = 1;

        s.Append("{ \"pkgTypeName\":\"Tables.permission\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(s);
        s.Append(" }");
        
        this->memHeader().flags = 0;
    }
    inline void permission::ToStringCore(xx::String& s) const noexcept
    {
        this->BaseType::ToStringCore(s);
        s.Append(", \"id\":", this->id);
        if (this->group) s.Append(", \"group\":\"", this->group, "\"");
        else s.Append(", \"group\":nil");
        if (this->name) s.Append(", \"name\":\"", this->name, "\"");
        else s.Append(", \"name\":nil");
        if (this->desc) s.Append(", \"desc\":\"", this->desc, "\"");
        else s.Append(", \"desc\":nil");
    }
    inline void permission::CopyTo(permission* const& o) const noexcept
    {
        o->id = this->id;
        o->group = this->group;
        o->name = this->name;
        o->desc = this->desc;
    }
    inline permission* permission::MakeCopy() const noexcept
    {
        auto o = mempool->MPCreate<permission>();
        this->CopyTo(o);
        return o;
    }
    inline permission_p permission::MakePtrCopy() const noexcept
    {
        return permission_p(this->MakeCopy());
    }

	inline role::role(xx::MemPool* const& mempool) noexcept
        : xx::Object(mempool)
	{
	}
	inline role::role(xx::BBuffer* const& bb)
        : xx::Object(bb)
	{
        if (int r = FromBBufferCore(*bb)) throw r;
	}
    inline void role::ToBBuffer(xx::BBuffer& bb) const noexcept
    {
        bb.Write(this->id);
        bb.Write(this->name);
        bb.Write(this->desc);
    }
    inline int role::FromBBuffer(xx::BBuffer& bb) noexcept
    {
        return this->FromBBufferCore(bb);
    }
    inline int role::FromBBufferCore(xx::BBuffer& bb) noexcept
    {
        if (int r = bb.Read(this->id)) return r;
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->name)) return r;
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->desc)) return r;
        return 0;
    }

    inline void role::ToString(xx::String& s) const noexcept
    {
        if (this->memHeader().flags)
        {
        	s.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else this->memHeader().flags = 1;

        s.Append("{ \"pkgTypeName\":\"Tables.role\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(s);
        s.Append(" }");
        
        this->memHeader().flags = 0;
    }
    inline void role::ToStringCore(xx::String& s) const noexcept
    {
        this->BaseType::ToStringCore(s);
        s.Append(", \"id\":", this->id);
        if (this->name) s.Append(", \"name\":\"", this->name, "\"");
        else s.Append(", \"name\":nil");
        if (this->desc) s.Append(", \"desc\":\"", this->desc, "\"");
        else s.Append(", \"desc\":nil");
    }
    inline void role::CopyTo(role* const& o) const noexcept
    {
        o->id = this->id;
        o->name = this->name;
        o->desc = this->desc;
    }
    inline role* role::MakeCopy() const noexcept
    {
        auto o = mempool->MPCreate<role>();
        this->CopyTo(o);
        return o;
    }
    inline role_p role::MakePtrCopy() const noexcept
    {
        return role_p(this->MakeCopy());
    }

	inline role_permission::role_permission(xx::MemPool* const& mempool) noexcept
        : xx::Object(mempool)
	{
	}
	inline role_permission::role_permission(xx::BBuffer* const& bb)
        : xx::Object(bb)
	{
        if (int r = FromBBufferCore(*bb)) throw r;
	}
    inline void role_permission::ToBBuffer(xx::BBuffer& bb) const noexcept
    {
        bb.Write(this->role_id);
        bb.Write(this->permission_id);
    }
    inline int role_permission::FromBBuffer(xx::BBuffer& bb) noexcept
    {
        return this->FromBBufferCore(bb);
    }
    inline int role_permission::FromBBufferCore(xx::BBuffer& bb) noexcept
    {
        if (int r = bb.Read(this->role_id)) return r;
        if (int r = bb.Read(this->permission_id)) return r;
        return 0;
    }

    inline void role_permission::ToString(xx::String& s) const noexcept
    {
        if (this->memHeader().flags)
        {
        	s.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else this->memHeader().flags = 1;

        s.Append("{ \"pkgTypeName\":\"Tables.role_permission\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(s);
        s.Append(" }");
        
        this->memHeader().flags = 0;
    }
    inline void role_permission::ToStringCore(xx::String& s) const noexcept
    {
        this->BaseType::ToStringCore(s);
        s.Append(", \"role_id\":", this->role_id);
        s.Append(", \"permission_id\":", this->permission_id);
    }
    inline void role_permission::CopyTo(role_permission* const& o) const noexcept
    {
        o->role_id = this->role_id;
        o->permission_id = this->permission_id;
    }
    inline role_permission* role_permission::MakeCopy() const noexcept
    {
        auto o = mempool->MPCreate<role_permission>();
        this->CopyTo(o);
        return o;
    }
    inline role_permission_p role_permission::MakePtrCopy() const noexcept
    {
        return role_permission_p(this->MakeCopy());
    }

}
}
namespace WEB
{
	inline void AllTypesRegister() noexcept
	{
        xx::MemPool::RegisterInternals();
	    xx::MemPool::Register<WEB::Manager, WEB::Tables::manager>();
	    xx::MemPool::Register<WEB::Tables::manager, xx::Object>();
	    xx::MemPool::Register<WEB::Role, WEB::Tables::role>();
	    xx::MemPool::Register<WEB::Tables::role, xx::Object>();
	    xx::MemPool::Register<WEB::Permission, WEB::Tables::permission>();
	    xx::MemPool::Register<WEB::Tables::permission, xx::Object>();
	    xx::MemPool::Register<WEB::BindManagerRole, WEB::Tables::manager_role>();
	    xx::MemPool::Register<WEB::Tables::manager_role, xx::Object>();
	    xx::MemPool::Register<WEB::BindRolePermission, WEB::Tables::role_permission>();
	    xx::MemPool::Register<WEB::Tables::role_permission, xx::Object>();
	    xx::MemPool::Register<WEB::Generic::Error, xx::Object>();
	    xx::MemPool::Register<WEB::Generic::Success, xx::Object>();
	    xx::MemPool::Register<WEB::Generic::SomeLists, xx::Object>();
	    xx::MemPool::Register<xx::List<int32_t>, xx::Object>();
	    xx::MemPool::Register<xx::List<int64_t>, xx::Object>();
	    xx::MemPool::Register<xx::List<xx::String_p>, xx::Object>();
	    xx::MemPool::Register<xx::List<WEB::Manager_p>, xx::Object>();
	    xx::MemPool::Register<xx::List<WEB::Role_p>, xx::Object>();
	    xx::MemPool::Register<xx::List<WEB::Permission_p>, xx::Object>();
	    xx::MemPool::Register<xx::List<WEB::BindManagerRole_p>, xx::Object>();
	    xx::MemPool::Register<xx::List<WEB::BindRolePermission_p>, xx::Object>();
	    xx::MemPool::Register<WEB::WEB_testcpp3::AuthInfo, xx::Object>();
	    xx::MemPool::Register<WEB::WEB_testcpp3::Cmd1, WEB::WEB_testcpp3::AuthInfo>();
	    xx::MemPool::Register<WEB::WEB_testcpp3::Cmd2, WEB::WEB_testcpp3::AuthInfo>();
	}
}
