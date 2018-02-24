#include <xx_mempool.h>
#include <xx_bbuffer.h>

namespace RPC
{
	struct PkgGenMd5
	{
		static constexpr char const* value = "ef692c7f0e1cc9f6550064bb8e6e1767";

    };

namespace DB_Manage
{
    struct MsgResult;
    using MsgResult_p = xx::Ptr<MsgResult>;
    using MsgResult_v = xx::Dock<MsgResult>;

}
namespace Manage_DB
{
    struct Msg;
    using Msg_p = xx::Ptr<Msg>;
    using Msg_v = xx::Dock<Msg>;

}
namespace Login_Client
{
    struct LoginSuccess;
    using LoginSuccess_p = xx::Ptr<LoginSuccess>;
    using LoginSuccess_v = xx::Dock<LoginSuccess>;

}
namespace Client_Login
{
    struct Login;
    using Login_p = xx::Ptr<Login>;
    using Login_v = xx::Dock<Login>;

}
namespace DB_Login
{
    struct AuthSuccess;
    using AuthSuccess_p = xx::Ptr<AuthSuccess>;
    using AuthSuccess_v = xx::Dock<AuthSuccess>;

}
namespace Login_DB
{
    struct Auth;
    using Auth_p = xx::Ptr<Auth>;
    using Auth_v = xx::Dock<Auth>;

}
namespace Generic
{
    // 服务间创建连接, 刚连上时, 首包需要发这个来说明自己是谁
    struct ServiceInfo;
    using ServiceInfo_p = xx::Ptr<ServiceInfo>;
    using ServiceInfo_v = xx::Dock<ServiceInfo>;

    // 通用错误返回
    struct Error;
    using Error_p = xx::Ptr<Error>;
    using Error_v = xx::Dock<Error>;

    // 心跳保持兼延迟测试 -- 请求
    struct Ping;
    using Ping_p = xx::Ptr<Ping>;
    using Ping_v = xx::Dock<Ping>;

    // 心跳保持兼延迟测试 -- 回应
    struct Pong;
    using Pong_p = xx::Ptr<Pong>;
    using Pong_v = xx::Dock<Pong>;

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
    struct MsgResult : xx::Object
    {
        xx::String_p txt;

        typedef MsgResult ThisType;
        typedef xx::Object BaseType;
	    MsgResult();
	    MsgResult(xx::BBuffer *bb);
		MsgResult(MsgResult const&) = delete;
		MsgResult& operator=(MsgResult const&) = delete;
        virtual void ToString(xx::String &str) const override;
        virtual void ToStringCore(xx::String &str) const override;
        virtual void ToBBuffer(xx::BBuffer &bb) const override;
        virtual int FromBBuffer(xx::BBuffer &bb) override;
    };
}
namespace Manage_DB
{
    struct Msg : xx::Object
    {
        xx::String_p txt;

        typedef Msg ThisType;
        typedef xx::Object BaseType;
	    Msg();
	    Msg(xx::BBuffer *bb);
		Msg(Msg const&) = delete;
		Msg& operator=(Msg const&) = delete;
        virtual void ToString(xx::String &str) const override;
        virtual void ToStringCore(xx::String &str) const override;
        virtual void ToBBuffer(xx::BBuffer &bb) const override;
        virtual int FromBBuffer(xx::BBuffer &bb) override;
    };
}
namespace Login_Client
{
    struct LoginSuccess : xx::Object
    {
        int32_t id = 0;

        typedef LoginSuccess ThisType;
        typedef xx::Object BaseType;
	    LoginSuccess();
	    LoginSuccess(xx::BBuffer *bb);
		LoginSuccess(LoginSuccess const&) = delete;
		LoginSuccess& operator=(LoginSuccess const&) = delete;
        virtual void ToString(xx::String &str) const override;
        virtual void ToStringCore(xx::String &str) const override;
        virtual void ToBBuffer(xx::BBuffer &bb) const override;
        virtual int FromBBuffer(xx::BBuffer &bb) override;
    };
}
namespace Client_Login
{
    struct Login : xx::Object
    {
        xx::String_p username;
        xx::String_p password;

        typedef Login ThisType;
        typedef xx::Object BaseType;
	    Login();
	    Login(xx::BBuffer *bb);
		Login(Login const&) = delete;
		Login& operator=(Login const&) = delete;
        virtual void ToString(xx::String &str) const override;
        virtual void ToStringCore(xx::String &str) const override;
        virtual void ToBBuffer(xx::BBuffer &bb) const override;
        virtual int FromBBuffer(xx::BBuffer &bb) override;
    };
}
namespace DB_Login
{
    struct AuthSuccess : xx::Object
    {
        int32_t id = 0;

        typedef AuthSuccess ThisType;
        typedef xx::Object BaseType;
	    AuthSuccess();
	    AuthSuccess(xx::BBuffer *bb);
		AuthSuccess(AuthSuccess const&) = delete;
		AuthSuccess& operator=(AuthSuccess const&) = delete;
        virtual void ToString(xx::String &str) const override;
        virtual void ToStringCore(xx::String &str) const override;
        virtual void ToBBuffer(xx::BBuffer &bb) const override;
        virtual int FromBBuffer(xx::BBuffer &bb) override;
    };
}
namespace Login_DB
{
    struct Auth : xx::Object
    {
        xx::String_p username;
        xx::String_p password;

        typedef Auth ThisType;
        typedef xx::Object BaseType;
	    Auth();
	    Auth(xx::BBuffer *bb);
		Auth(Auth const&) = delete;
		Auth& operator=(Auth const&) = delete;
        virtual void ToString(xx::String &str) const override;
        virtual void ToStringCore(xx::String &str) const override;
        virtual void ToBBuffer(xx::BBuffer &bb) const override;
        virtual int FromBBuffer(xx::BBuffer &bb) override;
    };
}
namespace Generic
{
    // 服务间创建连接, 刚连上时, 首包需要发这个来说明自己是谁
    struct ServiceInfo : xx::Object
    {
        // 服务类型
        RPC::Generic::ServiceTypes type = (RPC::Generic::ServiceTypes)0;

        typedef ServiceInfo ThisType;
        typedef xx::Object BaseType;
	    ServiceInfo();
	    ServiceInfo(xx::BBuffer *bb);
		ServiceInfo(ServiceInfo const&) = delete;
		ServiceInfo& operator=(ServiceInfo const&) = delete;
        virtual void ToString(xx::String &str) const override;
        virtual void ToStringCore(xx::String &str) const override;
        virtual void ToBBuffer(xx::BBuffer &bb) const override;
        virtual int FromBBuffer(xx::BBuffer &bb) override;
    };
    // 通用错误返回
    struct Error : xx::Object
    {
        // 错误码
        int32_t errNo = 0;
        // 错误文字
        xx::String_p errMsg;

        typedef Error ThisType;
        typedef xx::Object BaseType;
	    Error();
	    Error(xx::BBuffer *bb);
		Error(Error const&) = delete;
		Error& operator=(Error const&) = delete;
        virtual void ToString(xx::String &str) const override;
        virtual void ToStringCore(xx::String &str) const override;
        virtual void ToBBuffer(xx::BBuffer &bb) const override;
        virtual int FromBBuffer(xx::BBuffer &bb) override;
    };
    // 心跳保持兼延迟测试 -- 请求
    struct Ping : xx::Object
    {
        int64_t ticks = 0;

        typedef Ping ThisType;
        typedef xx::Object BaseType;
	    Ping();
	    Ping(xx::BBuffer *bb);
		Ping(Ping const&) = delete;
		Ping& operator=(Ping const&) = delete;
        virtual void ToString(xx::String &str) const override;
        virtual void ToStringCore(xx::String &str) const override;
        virtual void ToBBuffer(xx::BBuffer &bb) const override;
        virtual int FromBBuffer(xx::BBuffer &bb) override;
    };
    // 心跳保持兼延迟测试 -- 回应
    struct Pong : xx::Object
    {
        int64_t ticks = 0;

        typedef Pong ThisType;
        typedef xx::Object BaseType;
	    Pong();
	    Pong(xx::BBuffer *bb);
		Pong(Pong const&) = delete;
		Pong& operator=(Pong const&) = delete;
        virtual void ToString(xx::String &str) const override;
        virtual void ToStringCore(xx::String &str) const override;
        virtual void ToBBuffer(xx::BBuffer &bb) const override;
        virtual int FromBBuffer(xx::BBuffer &bb) override;
    };
}
namespace DB_Manage
{
	inline MsgResult::MsgResult()
	{
	}
	inline MsgResult::MsgResult(xx::BBuffer *bb)
	{
	    int rtv = 0;
        bb->readLengthLimit = 0;
        if (rtv = bb->Read(txt)) throw rtv;
	}
    inline void MsgResult::ToBBuffer(xx::BBuffer &bb) const
    {
        bb.Write(this->txt);
    }
    inline int MsgResult::FromBBuffer(xx::BBuffer &bb)
    {
        int rtv = 0;
        bb.readLengthLimit = 0;
        if (rtv = bb.Read(this->txt)) return rtv;
        return rtv;
    }

    inline void MsgResult::ToString(xx::String &str) const
    {
        if (tsFlags())
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else tsFlags() = 1;

        str.Append("{ \"type\" : \"MsgResult\"");
        ToStringCore(str);
        str.Append(" }");
        
        tsFlags() = 0;
    }
    inline void MsgResult::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"txt\" : ", this->txt);
    }


}
namespace Manage_DB
{
	inline Msg::Msg()
	{
	}
	inline Msg::Msg(xx::BBuffer *bb)
	{
	    int rtv = 0;
        bb->readLengthLimit = 200;
        if (rtv = bb->Read(txt)) throw rtv;
	}
    inline void Msg::ToBBuffer(xx::BBuffer &bb) const
    {
        bb.Write(this->txt);
    }
    inline int Msg::FromBBuffer(xx::BBuffer &bb)
    {
        int rtv = 0;
        bb.readLengthLimit = 200;
        if (rtv = bb.Read(this->txt)) return rtv;
        return rtv;
    }

    inline void Msg::ToString(xx::String &str) const
    {
        if (tsFlags())
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else tsFlags() = 1;

        str.Append("{ \"type\" : \"Msg\"");
        ToStringCore(str);
        str.Append(" }");
        
        tsFlags() = 0;
    }
    inline void Msg::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"txt\" : ", this->txt);
    }


}
namespace Login_Client
{
	inline LoginSuccess::LoginSuccess()
	{
	}
	inline LoginSuccess::LoginSuccess(xx::BBuffer *bb)
	{
	    int rtv = 0;
        if (rtv = bb->Read(id)) throw rtv;
	}
    inline void LoginSuccess::ToBBuffer(xx::BBuffer &bb) const
    {
        bb.Write(this->id);
    }
    inline int LoginSuccess::FromBBuffer(xx::BBuffer &bb)
    {
        int rtv = 0;
        if (rtv = bb.Read(this->id)) return rtv;
        return rtv;
    }

    inline void LoginSuccess::ToString(xx::String &str) const
    {
        if (tsFlags())
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else tsFlags() = 1;

        str.Append("{ \"type\" : \"LoginSuccess\"");
        ToStringCore(str);
        str.Append(" }");
        
        tsFlags() = 0;
    }
    inline void LoginSuccess::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"id\" : ", this->id);
    }


}
namespace Client_Login
{
	inline Login::Login()
	{
	}
	inline Login::Login(xx::BBuffer *bb)
	{
	    int rtv = 0;
        bb->readLengthLimit = 50;
        if (rtv = bb->Read(username)) throw rtv;
        bb->readLengthLimit = 50;
        if (rtv = bb->Read(password)) throw rtv;
	}
    inline void Login::ToBBuffer(xx::BBuffer &bb) const
    {
        bb.Write(this->username);
        bb.Write(this->password);
    }
    inline int Login::FromBBuffer(xx::BBuffer &bb)
    {
        int rtv = 0;
        bb.readLengthLimit = 50;
        if (rtv = bb.Read(this->username)) return rtv;
        bb.readLengthLimit = 50;
        if (rtv = bb.Read(this->password)) return rtv;
        return rtv;
    }

    inline void Login::ToString(xx::String &str) const
    {
        if (tsFlags())
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else tsFlags() = 1;

        str.Append("{ \"type\" : \"Login\"");
        ToStringCore(str);
        str.Append(" }");
        
        tsFlags() = 0;
    }
    inline void Login::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"username\" : ", this->username);
        str.Append(", \"password\" : ", this->password);
    }


}
namespace DB_Login
{
	inline AuthSuccess::AuthSuccess()
	{
	}
	inline AuthSuccess::AuthSuccess(xx::BBuffer *bb)
	{
	    int rtv = 0;
        if (rtv = bb->Read(id)) throw rtv;
	}
    inline void AuthSuccess::ToBBuffer(xx::BBuffer &bb) const
    {
        bb.Write(this->id);
    }
    inline int AuthSuccess::FromBBuffer(xx::BBuffer &bb)
    {
        int rtv = 0;
        if (rtv = bb.Read(this->id)) return rtv;
        return rtv;
    }

    inline void AuthSuccess::ToString(xx::String &str) const
    {
        if (tsFlags())
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else tsFlags() = 1;

        str.Append("{ \"type\" : \"AuthSuccess\"");
        ToStringCore(str);
        str.Append(" }");
        
        tsFlags() = 0;
    }
    inline void AuthSuccess::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"id\" : ", this->id);
    }


}
namespace Login_DB
{
	inline Auth::Auth()
	{
	}
	inline Auth::Auth(xx::BBuffer *bb)
	{
	    int rtv = 0;
        bb->readLengthLimit = 0;
        if (rtv = bb->Read(username)) throw rtv;
        bb->readLengthLimit = 0;
        if (rtv = bb->Read(password)) throw rtv;
	}
    inline void Auth::ToBBuffer(xx::BBuffer &bb) const
    {
        bb.Write(this->username);
        bb.Write(this->password);
    }
    inline int Auth::FromBBuffer(xx::BBuffer &bb)
    {
        int rtv = 0;
        bb.readLengthLimit = 0;
        if (rtv = bb.Read(this->username)) return rtv;
        bb.readLengthLimit = 0;
        if (rtv = bb.Read(this->password)) return rtv;
        return rtv;
    }

    inline void Auth::ToString(xx::String &str) const
    {
        if (tsFlags())
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else tsFlags() = 1;

        str.Append("{ \"type\" : \"Auth\"");
        ToStringCore(str);
        str.Append(" }");
        
        tsFlags() = 0;
    }
    inline void Auth::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"username\" : ", this->username);
        str.Append(", \"password\" : ", this->password);
    }


}
namespace Generic
{
	inline ServiceInfo::ServiceInfo()
	{
	}
	inline ServiceInfo::ServiceInfo(xx::BBuffer *bb)
	{
	    int rtv = 0;
        if (rtv = bb->Read(type)) throw rtv;
	}
    inline void ServiceInfo::ToBBuffer(xx::BBuffer &bb) const
    {
        bb.Write(this->type);
    }
    inline int ServiceInfo::FromBBuffer(xx::BBuffer &bb)
    {
        int rtv = 0;
        if (rtv = bb.Read(this->type)) return rtv;
        return rtv;
    }

    inline void ServiceInfo::ToString(xx::String &str) const
    {
        if (tsFlags())
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else tsFlags() = 1;

        str.Append("{ \"type\" : \"ServiceInfo\"");
        ToStringCore(str);
        str.Append(" }");
        
        tsFlags() = 0;
    }
    inline void ServiceInfo::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"type\" : ", this->type);
    }


	inline Error::Error()
	{
	}
	inline Error::Error(xx::BBuffer *bb)
	{
	    int rtv = 0;
        if (rtv = bb->Read(errNo)) throw rtv;
        bb->readLengthLimit = 0;
        if (rtv = bb->Read(errMsg)) throw rtv;
	}
    inline void Error::ToBBuffer(xx::BBuffer &bb) const
    {
        bb.Write(this->errNo);
        bb.Write(this->errMsg);
    }
    inline int Error::FromBBuffer(xx::BBuffer &bb)
    {
        int rtv = 0;
        if (rtv = bb.Read(this->errNo)) return rtv;
        bb.readLengthLimit = 0;
        if (rtv = bb.Read(this->errMsg)) return rtv;
        return rtv;
    }

    inline void Error::ToString(xx::String &str) const
    {
        if (tsFlags())
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else tsFlags() = 1;

        str.Append("{ \"type\" : \"Error\"");
        ToStringCore(str);
        str.Append(" }");
        
        tsFlags() = 0;
    }
    inline void Error::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"errNo\" : ", this->errNo);
        str.Append(", \"errMsg\" : ", this->errMsg);
    }


	inline Ping::Ping()
	{
	}
	inline Ping::Ping(xx::BBuffer *bb)
	{
	    int rtv = 0;
        if (rtv = bb->Read(ticks)) throw rtv;
	}
    inline void Ping::ToBBuffer(xx::BBuffer &bb) const
    {
        bb.Write(this->ticks);
    }
    inline int Ping::FromBBuffer(xx::BBuffer &bb)
    {
        int rtv = 0;
        if (rtv = bb.Read(this->ticks)) return rtv;
        return rtv;
    }

    inline void Ping::ToString(xx::String &str) const
    {
        if (tsFlags())
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else tsFlags() = 1;

        str.Append("{ \"type\" : \"Ping\"");
        ToStringCore(str);
        str.Append(" }");
        
        tsFlags() = 0;
    }
    inline void Ping::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"ticks\" : ", this->ticks);
    }


	inline Pong::Pong()
	{
	}
	inline Pong::Pong(xx::BBuffer *bb)
	{
	    int rtv = 0;
        if (rtv = bb->Read(ticks)) throw rtv;
	}
    inline void Pong::ToBBuffer(xx::BBuffer &bb) const
    {
        bb.Write(this->ticks);
    }
    inline int Pong::FromBBuffer(xx::BBuffer &bb)
    {
        int rtv = 0;
        if (rtv = bb.Read(this->ticks)) return rtv;
        return rtv;
    }

    inline void Pong::ToString(xx::String &str) const
    {
        if (tsFlags())
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else tsFlags() = 1;

        str.Append("{ \"type\" : \"Pong\"");
        ToStringCore(str);
        str.Append(" }");
        
        tsFlags() = 0;
    }
    inline void Pong::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"ticks\" : ", this->ticks);
    }


}
}
namespace xx
{
	template<> struct TypeId<xx::String> { static const uint16_t value = 1; };
	template<> struct TypeId<xx::BBuffer> { static const uint16_t value = 2; };
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
	inline void AllTypesRegister()
	{
	    xx::MemPool::Register<xx::String, xx::Object>();
	    xx::MemPool::Register<xx::BBuffer, xx::Object>();
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
