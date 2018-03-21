
namespace RPC
{
	struct PkgGenMd5
	{
		static constexpr char const* value = "60e4855a3a77e7e016822f33b4d765ba";
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
	    MsgResult(xx::MemPool* mempool);
	    MsgResult(xx::BBuffer *bb);
		MsgResult(MsgResult const&) = delete;
		MsgResult& operator=(MsgResult const&) = delete;
        void ToString(xx::String &str) const override;
        void ToStringCore(xx::String &str) const override;
        void ToBBuffer(xx::BBuffer &bb) const override;
        int FromBBuffer(xx::BBuffer &bb) override;
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
	    Msg(xx::MemPool* mempool);
	    Msg(xx::BBuffer *bb);
		Msg(Msg const&) = delete;
		Msg& operator=(Msg const&) = delete;
        void ToString(xx::String &str) const override;
        void ToStringCore(xx::String &str) const override;
        void ToBBuffer(xx::BBuffer &bb) const override;
        int FromBBuffer(xx::BBuffer &bb) override;
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
	    LoginSuccess(xx::MemPool* mempool);
	    LoginSuccess(xx::BBuffer *bb);
		LoginSuccess(LoginSuccess const&) = delete;
		LoginSuccess& operator=(LoginSuccess const&) = delete;
        void ToString(xx::String &str) const override;
        void ToStringCore(xx::String &str) const override;
        void ToBBuffer(xx::BBuffer &bb) const override;
        int FromBBuffer(xx::BBuffer &bb) override;
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
	    Login(xx::MemPool* mempool);
	    Login(xx::BBuffer *bb);
		Login(Login const&) = delete;
		Login& operator=(Login const&) = delete;
        void ToString(xx::String &str) const override;
        void ToStringCore(xx::String &str) const override;
        void ToBBuffer(xx::BBuffer &bb) const override;
        int FromBBuffer(xx::BBuffer &bb) override;
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
	    AuthSuccess(xx::MemPool* mempool);
	    AuthSuccess(xx::BBuffer *bb);
		AuthSuccess(AuthSuccess const&) = delete;
		AuthSuccess& operator=(AuthSuccess const&) = delete;
        void ToString(xx::String &str) const override;
        void ToStringCore(xx::String &str) const override;
        void ToBBuffer(xx::BBuffer &bb) const override;
        int FromBBuffer(xx::BBuffer &bb) override;
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
	    Auth(xx::MemPool* mempool);
	    Auth(xx::BBuffer *bb);
		Auth(Auth const&) = delete;
		Auth& operator=(Auth const&) = delete;
        void ToString(xx::String &str) const override;
        void ToStringCore(xx::String &str) const override;
        void ToBBuffer(xx::BBuffer &bb) const override;
        int FromBBuffer(xx::BBuffer &bb) override;
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
	    ServiceInfo(xx::MemPool* mempool);
	    ServiceInfo(xx::BBuffer *bb);
		ServiceInfo(ServiceInfo const&) = delete;
		ServiceInfo& operator=(ServiceInfo const&) = delete;
        void ToString(xx::String &str) const override;
        void ToStringCore(xx::String &str) const override;
        void ToBBuffer(xx::BBuffer &bb) const override;
        int FromBBuffer(xx::BBuffer &bb) override;
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
	    Error(xx::MemPool* mempool);
	    Error(xx::BBuffer *bb);
		Error(Error const&) = delete;
		Error& operator=(Error const&) = delete;
        void ToString(xx::String &str) const override;
        void ToStringCore(xx::String &str) const override;
        void ToBBuffer(xx::BBuffer &bb) const override;
        int FromBBuffer(xx::BBuffer &bb) override;
    };
    // 心跳保持兼延迟测试 -- 请求
    class Ping : public xx::Object
    {
    public:
        int64_t ticks = 0;

        typedef Ping ThisType;
        typedef xx::Object BaseType;
	    Ping(xx::MemPool* mempool);
	    Ping(xx::BBuffer *bb);
		Ping(Ping const&) = delete;
		Ping& operator=(Ping const&) = delete;
        void ToString(xx::String &str) const override;
        void ToStringCore(xx::String &str) const override;
        void ToBBuffer(xx::BBuffer &bb) const override;
        int FromBBuffer(xx::BBuffer &bb) override;
    };
    // 心跳保持兼延迟测试 -- 回应
    class Pong : public xx::Object
    {
    public:
        int64_t ticks = 0;

        typedef Pong ThisType;
        typedef xx::Object BaseType;
	    Pong(xx::MemPool* mempool);
	    Pong(xx::BBuffer *bb);
		Pong(Pong const&) = delete;
		Pong& operator=(Pong const&) = delete;
        void ToString(xx::String &str) const override;
        void ToStringCore(xx::String &str) const override;
        void ToBBuffer(xx::BBuffer &bb) const override;
        int FromBBuffer(xx::BBuffer &bb) override;
    };
}
namespace DB_Manage
{
	inline MsgResult::MsgResult(xx::MemPool* mempool)
        : xx::Object(mempool)
	{
	}
	inline MsgResult::MsgResult(xx::BBuffer *bb)
        : xx::Object(bb)
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
        if (memHeader().flags)
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else memHeader().flags = 1;

        str.Append("{ \"type\" : \"MsgResult\"");
        ToStringCore(str);
        str.Append(" }");
        
        memHeader().flags = 0;
    }
    inline void MsgResult::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        if (this->txt) str.Append(", \"txt\" : \"", this->txt, "\"");
        else str.Append(", \"txt\" : nil");
    }


}
namespace Manage_DB
{
	inline Msg::Msg(xx::MemPool* mempool)
        : xx::Object(mempool)
	{
	}
	inline Msg::Msg(xx::BBuffer *bb)
        : xx::Object(bb)
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
        if (memHeader().flags)
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else memHeader().flags = 1;

        str.Append("{ \"type\" : \"Msg\"");
        ToStringCore(str);
        str.Append(" }");
        
        memHeader().flags = 0;
    }
    inline void Msg::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        if (this->txt) str.Append(", \"txt\" : \"", this->txt, "\"");
        else str.Append(", \"txt\" : nil");
    }


}
namespace Login_Client
{
	inline LoginSuccess::LoginSuccess(xx::MemPool* mempool)
        : xx::Object(mempool)
	{
	}
	inline LoginSuccess::LoginSuccess(xx::BBuffer *bb)
        : xx::Object(bb)
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
        if (memHeader().flags)
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else memHeader().flags = 1;

        str.Append("{ \"type\" : \"LoginSuccess\"");
        ToStringCore(str);
        str.Append(" }");
        
        memHeader().flags = 0;
    }
    inline void LoginSuccess::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"id\" : ", this->id);
    }


}
namespace Client_Login
{
	inline Login::Login(xx::MemPool* mempool)
        : xx::Object(mempool)
	{
	}
	inline Login::Login(xx::BBuffer *bb)
        : xx::Object(bb)
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
        if (memHeader().flags)
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else memHeader().flags = 1;

        str.Append("{ \"type\" : \"Login\"");
        ToStringCore(str);
        str.Append(" }");
        
        memHeader().flags = 0;
    }
    inline void Login::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        if (this->username) str.Append(", \"username\" : \"", this->username, "\"");
        else str.Append(", \"username\" : nil");
        if (this->password) str.Append(", \"password\" : \"", this->password, "\"");
        else str.Append(", \"password\" : nil");
    }


}
namespace DB_Login
{
	inline AuthSuccess::AuthSuccess(xx::MemPool* mempool)
        : xx::Object(mempool)
	{
	}
	inline AuthSuccess::AuthSuccess(xx::BBuffer *bb)
        : xx::Object(bb)
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
        if (memHeader().flags)
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else memHeader().flags = 1;

        str.Append("{ \"type\" : \"AuthSuccess\"");
        ToStringCore(str);
        str.Append(" }");
        
        memHeader().flags = 0;
    }
    inline void AuthSuccess::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"id\" : ", this->id);
    }


}
namespace Login_DB
{
	inline Auth::Auth(xx::MemPool* mempool)
        : xx::Object(mempool)
	{
	}
	inline Auth::Auth(xx::BBuffer *bb)
        : xx::Object(bb)
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
        if (memHeader().flags)
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else memHeader().flags = 1;

        str.Append("{ \"type\" : \"Auth\"");
        ToStringCore(str);
        str.Append(" }");
        
        memHeader().flags = 0;
    }
    inline void Auth::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        if (this->username) str.Append(", \"username\" : \"", this->username, "\"");
        else str.Append(", \"username\" : nil");
        if (this->password) str.Append(", \"password\" : \"", this->password, "\"");
        else str.Append(", \"password\" : nil");
    }


}
namespace Generic
{
	inline ServiceInfo::ServiceInfo(xx::MemPool* mempool)
        : xx::Object(mempool)
	{
	}
	inline ServiceInfo::ServiceInfo(xx::BBuffer *bb)
        : xx::Object(bb)
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
        if (memHeader().flags)
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else memHeader().flags = 1;

        str.Append("{ \"type\" : \"ServiceInfo\"");
        ToStringCore(str);
        str.Append(" }");
        
        memHeader().flags = 0;
    }
    inline void ServiceInfo::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"type\" : ", this->type);
    }


	inline Error::Error(xx::MemPool* mempool)
        : xx::Object(mempool)
	{
	}
	inline Error::Error(xx::BBuffer *bb)
        : xx::Object(bb)
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
        if (memHeader().flags)
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else memHeader().flags = 1;

        str.Append("{ \"type\" : \"Error\"");
        ToStringCore(str);
        str.Append(" }");
        
        memHeader().flags = 0;
    }
    inline void Error::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"errNo\" : ", this->errNo);
        if (this->errMsg) str.Append(", \"errMsg\" : \"", this->errMsg, "\"");
        else str.Append(", \"errMsg\" : nil");
    }


	inline Ping::Ping(xx::MemPool* mempool)
        : xx::Object(mempool)
	{
	}
	inline Ping::Ping(xx::BBuffer *bb)
        : xx::Object(bb)
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
        if (memHeader().flags)
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else memHeader().flags = 1;

        str.Append("{ \"type\" : \"Ping\"");
        ToStringCore(str);
        str.Append(" }");
        
        memHeader().flags = 0;
    }
    inline void Ping::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"ticks\" : ", this->ticks);
    }


	inline Pong::Pong(xx::MemPool* mempool)
        : xx::Object(mempool)
	{
	}
	inline Pong::Pong(xx::BBuffer *bb)
        : xx::Object(bb)
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
        if (memHeader().flags)
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else memHeader().flags = 1;

        str.Append("{ \"type\" : \"Pong\"");
        ToStringCore(str);
        str.Append(" }");
        
        memHeader().flags = 0;
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
