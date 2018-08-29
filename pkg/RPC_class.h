#pragma once
#include "xx.h"

namespace RPC
{
	struct PkgGenMd5
	{
		static constexpr char const* value = "d922f9f2f344814099a4977383ca41a3";
    };

namespace DB_Manage
{
    class MsgResult;
    using MsgResult_p = xx::Ptr<MsgResult>;
    using MsgResult_r = xx::Ref<MsgResult>;

}
namespace Manage_DB
{
    class Msg;
    using Msg_p = xx::Ptr<Msg>;
    using Msg_r = xx::Ref<Msg>;

}
namespace Login_Client
{
    class LoginSuccess;
    using LoginSuccess_p = xx::Ptr<LoginSuccess>;
    using LoginSuccess_r = xx::Ref<LoginSuccess>;

}
namespace Client_Login
{
    class Login;
    using Login_p = xx::Ptr<Login>;
    using Login_r = xx::Ref<Login>;

}
namespace DB_Login
{
    class AuthSuccess;
    using AuthSuccess_p = xx::Ptr<AuthSuccess>;
    using AuthSuccess_r = xx::Ref<AuthSuccess>;

}
namespace Login_DB
{
    class Auth;
    using Auth_p = xx::Ptr<Auth>;
    using Auth_r = xx::Ref<Auth>;

}
namespace Generic
{
    // 服务间创建连接, 刚连上时, 首包需要发这个来说明自己是谁
    class ServiceInfo;
    using ServiceInfo_p = xx::Ptr<ServiceInfo>;
    using ServiceInfo_r = xx::Ref<ServiceInfo>;

    // 通用错误返回
    class Error;
    using Error_p = xx::Ptr<Error>;
    using Error_r = xx::Ref<Error>;

    // 心跳保持兼延迟测试 -- 请求
    class Ping;
    using Ping_p = xx::Ptr<Ping>;
    using Ping_r = xx::Ref<Ping>;

    // 心跳保持兼延迟测试 -- 回应
    class Pong;
    using Pong_p = xx::Ptr<Pong>;
    using Pong_r = xx::Ref<Pong>;

}
namespace Generic
{
    // 服务类型列表
    enum class ServiceTypes : int32_t
    {
        // 登录
        Login = 0,
        // 数据库
        DB = 1,
        // 管理端
        Manage = 2,
    };
}
namespace DB_Manage
{
    class MsgResult : public xx::Object
    {
    public:
        xx::String_p txt;

        typedef MsgResult ThisType;
        typedef xx::Object BaseType;
	    MsgResult(xx::MemPool* const& mempool) noexcept;
	    MsgResult(xx::BBuffer* const& bb);
		MsgResult(MsgResult const&) = delete;
		MsgResult& operator=(MsgResult const&) = delete;
        void ToString(xx::String& s) const noexcept override;
        void ToStringCore(xx::String& s) const noexcept override;
        void ToBBuffer(xx::BBuffer& bb) const noexcept override;
        int FromBBuffer(xx::BBuffer& bb) noexcept override;
        int FromBBufferCore(xx::BBuffer& bb) noexcept;
        void CopyTo(MsgResult* const& o) const noexcept;
        MsgResult* MakeCopy() const noexcept;
        MsgResult_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
    };
}
namespace Manage_DB
{
    class Msg : public xx::Object
    {
    public:
        xx::String_p txt;

        typedef Msg ThisType;
        typedef xx::Object BaseType;
	    Msg(xx::MemPool* const& mempool) noexcept;
	    Msg(xx::BBuffer* const& bb);
		Msg(Msg const&) = delete;
		Msg& operator=(Msg const&) = delete;
        void ToString(xx::String& s) const noexcept override;
        void ToStringCore(xx::String& s) const noexcept override;
        void ToBBuffer(xx::BBuffer& bb) const noexcept override;
        int FromBBuffer(xx::BBuffer& bb) noexcept override;
        int FromBBufferCore(xx::BBuffer& bb) noexcept;
        void CopyTo(Msg* const& o) const noexcept;
        Msg* MakeCopy() const noexcept;
        Msg_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
    };
}
namespace Login_Client
{
    class LoginSuccess : public xx::Object
    {
    public:
        int32_t id = 0;

        typedef LoginSuccess ThisType;
        typedef xx::Object BaseType;
	    LoginSuccess(xx::MemPool* const& mempool) noexcept;
	    LoginSuccess(xx::BBuffer* const& bb);
		LoginSuccess(LoginSuccess const&) = delete;
		LoginSuccess& operator=(LoginSuccess const&) = delete;
        void ToString(xx::String& s) const noexcept override;
        void ToStringCore(xx::String& s) const noexcept override;
        void ToBBuffer(xx::BBuffer& bb) const noexcept override;
        int FromBBuffer(xx::BBuffer& bb) noexcept override;
        int FromBBufferCore(xx::BBuffer& bb) noexcept;
        void CopyTo(LoginSuccess* const& o) const noexcept;
        LoginSuccess* MakeCopy() const noexcept;
        LoginSuccess_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
    };
}
namespace Client_Login
{
    class Login : public xx::Object
    {
    public:
        xx::String_p username;
        xx::String_p password;

        typedef Login ThisType;
        typedef xx::Object BaseType;
	    Login(xx::MemPool* const& mempool) noexcept;
	    Login(xx::BBuffer* const& bb);
		Login(Login const&) = delete;
		Login& operator=(Login const&) = delete;
        void ToString(xx::String& s) const noexcept override;
        void ToStringCore(xx::String& s) const noexcept override;
        void ToBBuffer(xx::BBuffer& bb) const noexcept override;
        int FromBBuffer(xx::BBuffer& bb) noexcept override;
        int FromBBufferCore(xx::BBuffer& bb) noexcept;
        void CopyTo(Login* const& o) const noexcept;
        Login* MakeCopy() const noexcept;
        Login_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
    };
}
namespace DB_Login
{
    class AuthSuccess : public xx::Object
    {
    public:
        int32_t id = 0;

        typedef AuthSuccess ThisType;
        typedef xx::Object BaseType;
	    AuthSuccess(xx::MemPool* const& mempool) noexcept;
	    AuthSuccess(xx::BBuffer* const& bb);
		AuthSuccess(AuthSuccess const&) = delete;
		AuthSuccess& operator=(AuthSuccess const&) = delete;
        void ToString(xx::String& s) const noexcept override;
        void ToStringCore(xx::String& s) const noexcept override;
        void ToBBuffer(xx::BBuffer& bb) const noexcept override;
        int FromBBuffer(xx::BBuffer& bb) noexcept override;
        int FromBBufferCore(xx::BBuffer& bb) noexcept;
        void CopyTo(AuthSuccess* const& o) const noexcept;
        AuthSuccess* MakeCopy() const noexcept;
        AuthSuccess_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
    };
}
namespace Login_DB
{
    class Auth : public xx::Object
    {
    public:
        xx::String_p username;
        xx::String_p password;

        typedef Auth ThisType;
        typedef xx::Object BaseType;
	    Auth(xx::MemPool* const& mempool) noexcept;
	    Auth(xx::BBuffer* const& bb);
		Auth(Auth const&) = delete;
		Auth& operator=(Auth const&) = delete;
        void ToString(xx::String& s) const noexcept override;
        void ToStringCore(xx::String& s) const noexcept override;
        void ToBBuffer(xx::BBuffer& bb) const noexcept override;
        int FromBBuffer(xx::BBuffer& bb) noexcept override;
        int FromBBufferCore(xx::BBuffer& bb) noexcept;
        void CopyTo(Auth* const& o) const noexcept;
        Auth* MakeCopy() const noexcept;
        Auth_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
    };
}
namespace Generic
{
    // 服务间创建连接, 刚连上时, 首包需要发这个来说明自己是谁
    class ServiceInfo : public xx::Object
    {
    public:
        // 服务类型
        RPC::Generic::ServiceTypes type = (RPC::Generic::ServiceTypes)0;

        typedef ServiceInfo ThisType;
        typedef xx::Object BaseType;
	    ServiceInfo(xx::MemPool* const& mempool) noexcept;
	    ServiceInfo(xx::BBuffer* const& bb);
		ServiceInfo(ServiceInfo const&) = delete;
		ServiceInfo& operator=(ServiceInfo const&) = delete;
        void ToString(xx::String& s) const noexcept override;
        void ToStringCore(xx::String& s) const noexcept override;
        void ToBBuffer(xx::BBuffer& bb) const noexcept override;
        int FromBBuffer(xx::BBuffer& bb) noexcept override;
        int FromBBufferCore(xx::BBuffer& bb) noexcept;
        void CopyTo(ServiceInfo* const& o) const noexcept;
        ServiceInfo* MakeCopy() const noexcept;
        ServiceInfo_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
    };
    // 通用错误返回
    class Error : public xx::Object
    {
    public:
        // 错误码
        int32_t errNo = 0;
        // 错误文字
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
    // 心跳保持兼延迟测试 -- 请求
    class Ping : public xx::Object
    {
    public:
        double ticks = 0;

        typedef Ping ThisType;
        typedef xx::Object BaseType;
	    Ping(xx::MemPool* const& mempool) noexcept;
	    Ping(xx::BBuffer* const& bb);
		Ping(Ping const&) = delete;
		Ping& operator=(Ping const&) = delete;
        void ToString(xx::String& s) const noexcept override;
        void ToStringCore(xx::String& s) const noexcept override;
        void ToBBuffer(xx::BBuffer& bb) const noexcept override;
        int FromBBuffer(xx::BBuffer& bb) noexcept override;
        int FromBBufferCore(xx::BBuffer& bb) noexcept;
        void CopyTo(Ping* const& o) const noexcept;
        Ping* MakeCopy() const noexcept;
        Ping_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
    };
    // 心跳保持兼延迟测试 -- 回应
    class Pong : public xx::Object
    {
    public:
        double ticks = 0;

        typedef Pong ThisType;
        typedef xx::Object BaseType;
	    Pong(xx::MemPool* const& mempool) noexcept;
	    Pong(xx::BBuffer* const& bb);
		Pong(Pong const&) = delete;
		Pong& operator=(Pong const&) = delete;
        void ToString(xx::String& s) const noexcept override;
        void ToStringCore(xx::String& s) const noexcept override;
        void ToBBuffer(xx::BBuffer& bb) const noexcept override;
        int FromBBuffer(xx::BBuffer& bb) noexcept override;
        int FromBBufferCore(xx::BBuffer& bb) noexcept;
        void CopyTo(Pong* const& o) const noexcept;
        Pong* MakeCopy() const noexcept;
        Pong_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
    };
}
}
namespace xx
{
	template<> struct TypeId<RPC::DB_Manage::MsgResult> { static const uint16_t value = 11; };
	template<> struct TypeId<RPC::Manage_DB::Msg> { static const uint16_t value = 12; };
	template<> struct TypeId<RPC::Login_Client::LoginSuccess> { static const uint16_t value = 4; };
	template<> struct TypeId<RPC::Client_Login::Login> { static const uint16_t value = 5; };
	template<> struct TypeId<RPC::DB_Login::AuthSuccess> { static const uint16_t value = 6; };
	template<> struct TypeId<RPC::Login_DB::Auth> { static const uint16_t value = 7; };
	template<> struct TypeId<RPC::Generic::ServiceInfo> { static const uint16_t value = 3; };
	template<> struct TypeId<RPC::Generic::Error> { static const uint16_t value = 8; };
	template<> struct TypeId<RPC::Generic::Ping> { static const uint16_t value = 9; };
	template<> struct TypeId<RPC::Generic::Pong> { static const uint16_t value = 10; };
}
namespace RPC
{
namespace DB_Manage
{
	inline MsgResult::MsgResult(xx::MemPool* const& mempool) noexcept
        : xx::Object(mempool)
	{
	}
	inline MsgResult::MsgResult(xx::BBuffer* const& bb)
        : xx::Object(bb)
	{
        if (int r = FromBBufferCore(*bb)) throw r;
	}
    inline void MsgResult::ToBBuffer(xx::BBuffer& bb) const noexcept
    {
        bb.Write(this->txt);
    }
    inline int MsgResult::FromBBuffer(xx::BBuffer& bb) noexcept
    {
        return this->FromBBufferCore(bb);
    }
    inline int MsgResult::FromBBufferCore(xx::BBuffer& bb) noexcept
    {
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->txt)) return r;
        return 0;
    }

    inline void MsgResult::ToString(xx::String& s) const noexcept
    {
        if (this->memHeader().flags)
        {
        	s.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else this->memHeader().flags = 1;

        s.Append("{ \"pkgTypeName\":\"DB_Manage.MsgResult\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(s);
        s.Append(" }");
        
        this->memHeader().flags = 0;
    }
    inline void MsgResult::ToStringCore(xx::String& s) const noexcept
    {
        this->BaseType::ToStringCore(s);
        if (this->txt) s.Append(", \"txt\":\"", this->txt, "\"");
        else s.Append(", \"txt\":nil");
    }
    inline void MsgResult::CopyTo(MsgResult* const& o) const noexcept
    {
        o->txt = this->txt;
    }
    inline MsgResult* MsgResult::MakeCopy() const noexcept
    {
        auto o = mempool->MPCreate<MsgResult>();
        this->CopyTo(o);
        return o;
    }
    inline MsgResult_p MsgResult::MakePtrCopy() const noexcept
    {
        return MsgResult_p(this->MakeCopy());
    }

}
namespace Manage_DB
{
	inline Msg::Msg(xx::MemPool* const& mempool) noexcept
        : xx::Object(mempool)
	{
	}
	inline Msg::Msg(xx::BBuffer* const& bb)
        : xx::Object(bb)
	{
        if (int r = FromBBufferCore(*bb)) throw r;
	}
    inline void Msg::ToBBuffer(xx::BBuffer& bb) const noexcept
    {
        bb.Write(this->txt);
    }
    inline int Msg::FromBBuffer(xx::BBuffer& bb) noexcept
    {
        return this->FromBBufferCore(bb);
    }
    inline int Msg::FromBBufferCore(xx::BBuffer& bb) noexcept
    {
        bb.readLengthLimit = 200;
        if (int r = bb.Read(this->txt)) return r;
        return 0;
    }

    inline void Msg::ToString(xx::String& s) const noexcept
    {
        if (this->memHeader().flags)
        {
        	s.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else this->memHeader().flags = 1;

        s.Append("{ \"pkgTypeName\":\"Manage_DB.Msg\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(s);
        s.Append(" }");
        
        this->memHeader().flags = 0;
    }
    inline void Msg::ToStringCore(xx::String& s) const noexcept
    {
        this->BaseType::ToStringCore(s);
        if (this->txt) s.Append(", \"txt\":\"", this->txt, "\"");
        else s.Append(", \"txt\":nil");
    }
    inline void Msg::CopyTo(Msg* const& o) const noexcept
    {
        o->txt = this->txt;
    }
    inline Msg* Msg::MakeCopy() const noexcept
    {
        auto o = mempool->MPCreate<Msg>();
        this->CopyTo(o);
        return o;
    }
    inline Msg_p Msg::MakePtrCopy() const noexcept
    {
        return Msg_p(this->MakeCopy());
    }

}
namespace Login_Client
{
	inline LoginSuccess::LoginSuccess(xx::MemPool* const& mempool) noexcept
        : xx::Object(mempool)
	{
	}
	inline LoginSuccess::LoginSuccess(xx::BBuffer* const& bb)
        : xx::Object(bb)
	{
        if (int r = FromBBufferCore(*bb)) throw r;
	}
    inline void LoginSuccess::ToBBuffer(xx::BBuffer& bb) const noexcept
    {
        bb.Write(this->id);
    }
    inline int LoginSuccess::FromBBuffer(xx::BBuffer& bb) noexcept
    {
        return this->FromBBufferCore(bb);
    }
    inline int LoginSuccess::FromBBufferCore(xx::BBuffer& bb) noexcept
    {
        if (int r = bb.Read(this->id)) return r;
        return 0;
    }

    inline void LoginSuccess::ToString(xx::String& s) const noexcept
    {
        if (this->memHeader().flags)
        {
        	s.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else this->memHeader().flags = 1;

        s.Append("{ \"pkgTypeName\":\"Login_Client.LoginSuccess\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(s);
        s.Append(" }");
        
        this->memHeader().flags = 0;
    }
    inline void LoginSuccess::ToStringCore(xx::String& s) const noexcept
    {
        this->BaseType::ToStringCore(s);
        s.Append(", \"id\":", this->id);
    }
    inline void LoginSuccess::CopyTo(LoginSuccess* const& o) const noexcept
    {
        o->id = this->id;
    }
    inline LoginSuccess* LoginSuccess::MakeCopy() const noexcept
    {
        auto o = mempool->MPCreate<LoginSuccess>();
        this->CopyTo(o);
        return o;
    }
    inline LoginSuccess_p LoginSuccess::MakePtrCopy() const noexcept
    {
        return LoginSuccess_p(this->MakeCopy());
    }

}
namespace Client_Login
{
	inline Login::Login(xx::MemPool* const& mempool) noexcept
        : xx::Object(mempool)
	{
	}
	inline Login::Login(xx::BBuffer* const& bb)
        : xx::Object(bb)
	{
        if (int r = FromBBufferCore(*bb)) throw r;
	}
    inline void Login::ToBBuffer(xx::BBuffer& bb) const noexcept
    {
        bb.Write(this->username);
        bb.Write(this->password);
    }
    inline int Login::FromBBuffer(xx::BBuffer& bb) noexcept
    {
        return this->FromBBufferCore(bb);
    }
    inline int Login::FromBBufferCore(xx::BBuffer& bb) noexcept
    {
        bb.readLengthLimit = 50;
        if (int r = bb.Read(this->username)) return r;
        bb.readLengthLimit = 50;
        if (int r = bb.Read(this->password)) return r;
        return 0;
    }

    inline void Login::ToString(xx::String& s) const noexcept
    {
        if (this->memHeader().flags)
        {
        	s.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else this->memHeader().flags = 1;

        s.Append("{ \"pkgTypeName\":\"Client_Login.Login\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(s);
        s.Append(" }");
        
        this->memHeader().flags = 0;
    }
    inline void Login::ToStringCore(xx::String& s) const noexcept
    {
        this->BaseType::ToStringCore(s);
        if (this->username) s.Append(", \"username\":\"", this->username, "\"");
        else s.Append(", \"username\":nil");
        if (this->password) s.Append(", \"password\":\"", this->password, "\"");
        else s.Append(", \"password\":nil");
    }
    inline void Login::CopyTo(Login* const& o) const noexcept
    {
        o->username = this->username;
        o->password = this->password;
    }
    inline Login* Login::MakeCopy() const noexcept
    {
        auto o = mempool->MPCreate<Login>();
        this->CopyTo(o);
        return o;
    }
    inline Login_p Login::MakePtrCopy() const noexcept
    {
        return Login_p(this->MakeCopy());
    }

}
namespace DB_Login
{
	inline AuthSuccess::AuthSuccess(xx::MemPool* const& mempool) noexcept
        : xx::Object(mempool)
	{
	}
	inline AuthSuccess::AuthSuccess(xx::BBuffer* const& bb)
        : xx::Object(bb)
	{
        if (int r = FromBBufferCore(*bb)) throw r;
	}
    inline void AuthSuccess::ToBBuffer(xx::BBuffer& bb) const noexcept
    {
        bb.Write(this->id);
    }
    inline int AuthSuccess::FromBBuffer(xx::BBuffer& bb) noexcept
    {
        return this->FromBBufferCore(bb);
    }
    inline int AuthSuccess::FromBBufferCore(xx::BBuffer& bb) noexcept
    {
        if (int r = bb.Read(this->id)) return r;
        return 0;
    }

    inline void AuthSuccess::ToString(xx::String& s) const noexcept
    {
        if (this->memHeader().flags)
        {
        	s.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else this->memHeader().flags = 1;

        s.Append("{ \"pkgTypeName\":\"DB_Login.AuthSuccess\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(s);
        s.Append(" }");
        
        this->memHeader().flags = 0;
    }
    inline void AuthSuccess::ToStringCore(xx::String& s) const noexcept
    {
        this->BaseType::ToStringCore(s);
        s.Append(", \"id\":", this->id);
    }
    inline void AuthSuccess::CopyTo(AuthSuccess* const& o) const noexcept
    {
        o->id = this->id;
    }
    inline AuthSuccess* AuthSuccess::MakeCopy() const noexcept
    {
        auto o = mempool->MPCreate<AuthSuccess>();
        this->CopyTo(o);
        return o;
    }
    inline AuthSuccess_p AuthSuccess::MakePtrCopy() const noexcept
    {
        return AuthSuccess_p(this->MakeCopy());
    }

}
namespace Login_DB
{
	inline Auth::Auth(xx::MemPool* const& mempool) noexcept
        : xx::Object(mempool)
	{
	}
	inline Auth::Auth(xx::BBuffer* const& bb)
        : xx::Object(bb)
	{
        if (int r = FromBBufferCore(*bb)) throw r;
	}
    inline void Auth::ToBBuffer(xx::BBuffer& bb) const noexcept
    {
        bb.Write(this->username);
        bb.Write(this->password);
    }
    inline int Auth::FromBBuffer(xx::BBuffer& bb) noexcept
    {
        return this->FromBBufferCore(bb);
    }
    inline int Auth::FromBBufferCore(xx::BBuffer& bb) noexcept
    {
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->username)) return r;
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->password)) return r;
        return 0;
    }

    inline void Auth::ToString(xx::String& s) const noexcept
    {
        if (this->memHeader().flags)
        {
        	s.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else this->memHeader().flags = 1;

        s.Append("{ \"pkgTypeName\":\"Login_DB.Auth\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(s);
        s.Append(" }");
        
        this->memHeader().flags = 0;
    }
    inline void Auth::ToStringCore(xx::String& s) const noexcept
    {
        this->BaseType::ToStringCore(s);
        if (this->username) s.Append(", \"username\":\"", this->username, "\"");
        else s.Append(", \"username\":nil");
        if (this->password) s.Append(", \"password\":\"", this->password, "\"");
        else s.Append(", \"password\":nil");
    }
    inline void Auth::CopyTo(Auth* const& o) const noexcept
    {
        o->username = this->username;
        o->password = this->password;
    }
    inline Auth* Auth::MakeCopy() const noexcept
    {
        auto o = mempool->MPCreate<Auth>();
        this->CopyTo(o);
        return o;
    }
    inline Auth_p Auth::MakePtrCopy() const noexcept
    {
        return Auth_p(this->MakeCopy());
    }

}
namespace Generic
{
	inline ServiceInfo::ServiceInfo(xx::MemPool* const& mempool) noexcept
        : xx::Object(mempool)
	{
	}
	inline ServiceInfo::ServiceInfo(xx::BBuffer* const& bb)
        : xx::Object(bb)
	{
        if (int r = FromBBufferCore(*bb)) throw r;
	}
    inline void ServiceInfo::ToBBuffer(xx::BBuffer& bb) const noexcept
    {
        bb.Write(this->type);
    }
    inline int ServiceInfo::FromBBuffer(xx::BBuffer& bb) noexcept
    {
        return this->FromBBufferCore(bb);
    }
    inline int ServiceInfo::FromBBufferCore(xx::BBuffer& bb) noexcept
    {
        if (int r = bb.Read(this->type)) return r;
        return 0;
    }

    inline void ServiceInfo::ToString(xx::String& s) const noexcept
    {
        if (this->memHeader().flags)
        {
        	s.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else this->memHeader().flags = 1;

        s.Append("{ \"pkgTypeName\":\"Generic.ServiceInfo\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(s);
        s.Append(" }");
        
        this->memHeader().flags = 0;
    }
    inline void ServiceInfo::ToStringCore(xx::String& s) const noexcept
    {
        this->BaseType::ToStringCore(s);
        s.Append(", \"type\":", this->type);
    }
    inline void ServiceInfo::CopyTo(ServiceInfo* const& o) const noexcept
    {
        o->type = this->type;
    }
    inline ServiceInfo* ServiceInfo::MakeCopy() const noexcept
    {
        auto o = mempool->MPCreate<ServiceInfo>();
        this->CopyTo(o);
        return o;
    }
    inline ServiceInfo_p ServiceInfo::MakePtrCopy() const noexcept
    {
        return ServiceInfo_p(this->MakeCopy());
    }

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
        bb.Write(this->errNo);
        bb.Write(this->errMsg);
    }
    inline int Error::FromBBuffer(xx::BBuffer& bb) noexcept
    {
        return this->FromBBufferCore(bb);
    }
    inline int Error::FromBBufferCore(xx::BBuffer& bb) noexcept
    {
        if (int r = bb.Read(this->errNo)) return r;
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
        s.Append(", \"errNo\":", this->errNo);
        if (this->errMsg) s.Append(", \"errMsg\":\"", this->errMsg, "\"");
        else s.Append(", \"errMsg\":nil");
    }
    inline void Error::CopyTo(Error* const& o) const noexcept
    {
        o->errNo = this->errNo;
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

	inline Ping::Ping(xx::MemPool* const& mempool) noexcept
        : xx::Object(mempool)
	{
	}
	inline Ping::Ping(xx::BBuffer* const& bb)
        : xx::Object(bb)
	{
        if (int r = FromBBufferCore(*bb)) throw r;
	}
    inline void Ping::ToBBuffer(xx::BBuffer& bb) const noexcept
    {
        bb.Write(this->ticks);
    }
    inline int Ping::FromBBuffer(xx::BBuffer& bb) noexcept
    {
        return this->FromBBufferCore(bb);
    }
    inline int Ping::FromBBufferCore(xx::BBuffer& bb) noexcept
    {
        if (int r = bb.Read(this->ticks)) return r;
        return 0;
    }

    inline void Ping::ToString(xx::String& s) const noexcept
    {
        if (this->memHeader().flags)
        {
        	s.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else this->memHeader().flags = 1;

        s.Append("{ \"pkgTypeName\":\"Generic.Ping\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(s);
        s.Append(" }");
        
        this->memHeader().flags = 0;
    }
    inline void Ping::ToStringCore(xx::String& s) const noexcept
    {
        this->BaseType::ToStringCore(s);
        s.Append(", \"ticks\":", this->ticks);
    }
    inline void Ping::CopyTo(Ping* const& o) const noexcept
    {
        o->ticks = this->ticks;
    }
    inline Ping* Ping::MakeCopy() const noexcept
    {
        auto o = mempool->MPCreate<Ping>();
        this->CopyTo(o);
        return o;
    }
    inline Ping_p Ping::MakePtrCopy() const noexcept
    {
        return Ping_p(this->MakeCopy());
    }

	inline Pong::Pong(xx::MemPool* const& mempool) noexcept
        : xx::Object(mempool)
	{
	}
	inline Pong::Pong(xx::BBuffer* const& bb)
        : xx::Object(bb)
	{
        if (int r = FromBBufferCore(*bb)) throw r;
	}
    inline void Pong::ToBBuffer(xx::BBuffer& bb) const noexcept
    {
        bb.Write(this->ticks);
    }
    inline int Pong::FromBBuffer(xx::BBuffer& bb) noexcept
    {
        return this->FromBBufferCore(bb);
    }
    inline int Pong::FromBBufferCore(xx::BBuffer& bb) noexcept
    {
        if (int r = bb.Read(this->ticks)) return r;
        return 0;
    }

    inline void Pong::ToString(xx::String& s) const noexcept
    {
        if (this->memHeader().flags)
        {
        	s.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else this->memHeader().flags = 1;

        s.Append("{ \"pkgTypeName\":\"Generic.Pong\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(s);
        s.Append(" }");
        
        this->memHeader().flags = 0;
    }
    inline void Pong::ToStringCore(xx::String& s) const noexcept
    {
        this->BaseType::ToStringCore(s);
        s.Append(", \"ticks\":", this->ticks);
    }
    inline void Pong::CopyTo(Pong* const& o) const noexcept
    {
        o->ticks = this->ticks;
    }
    inline Pong* Pong::MakeCopy() const noexcept
    {
        auto o = mempool->MPCreate<Pong>();
        this->CopyTo(o);
        return o;
    }
    inline Pong_p Pong::MakePtrCopy() const noexcept
    {
        return Pong_p(this->MakeCopy());
    }

}
}
namespace RPC
{
	inline void AllTypesRegister() noexcept
	{
	    xx::MemPool::Register<RPC::DB_Manage::MsgResult, xx::Object>();
	    xx::MemPool::Register<RPC::Manage_DB::Msg, xx::Object>();
	    xx::MemPool::Register<RPC::Login_Client::LoginSuccess, xx::Object>();
	    xx::MemPool::Register<RPC::Client_Login::Login, xx::Object>();
	    xx::MemPool::Register<RPC::DB_Login::AuthSuccess, xx::Object>();
	    xx::MemPool::Register<RPC::Login_DB::Auth, xx::Object>();
	    xx::MemPool::Register<RPC::Generic::ServiceInfo, xx::Object>();
	    xx::MemPool::Register<RPC::Generic::Error, xx::Object>();
	    xx::MemPool::Register<RPC::Generic::Ping, xx::Object>();
	    xx::MemPool::Register<RPC::Generic::Pong, xx::Object>();
	}
}
