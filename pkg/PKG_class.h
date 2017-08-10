#include <xx_mempool.h>
#include <xx_bbuffer.h>

namespace PKG
{
    // 一个请求( 当前限定为 service 与 db 间 ), 通常携带一个流水号. 这是基类
    struct Request;
    // 一个回应( 当前限定为 service 与 db 间 ), 通常携带一个请求发过来的流水号. 这是基类
    struct Response;
    struct Property;
    struct Property_long;
    struct Property_double;
    struct Property_string;
    struct Properties;
    struct UserInfo;
namespace Server_Client
{
    // 进入成功, 返回用户信息
    struct JoinSuccess;
    // 进入失败, 返回错误信息
    struct JoinFail;
    // 推送文字消息
    struct PushJoin;
    // 推送文字消息
    struct PushMessage;
    // 推送退出消息
    struct PushLogout;
}
namespace Client_Server
{
    // 请求进入服务器
    struct Join;
    // 发消息
    struct Message;
    // 主动退出
    struct Logout;
}
    struct Property : xx::Object
    {
        xx::String* name = nullptr;

        typedef Property ThisType;
        typedef xx::Object BaseType;
	    Property();
	    Property(xx::BBuffer *bb);
	    ~Property();
        virtual void ToString(xx::String &str) const override;
        virtual void ToStringCore(xx::String &str) const override;
        virtual void ToBBuffer(xx::BBuffer &bb) const override;
        virtual int FromBBuffer(xx::BBuffer &bb) override;
    };
    // 一个回应( 当前限定为 service 与 db 间 ), 通常携带一个请求发过来的流水号. 这是基类
    struct Response : xx::Object
    {
        int32_t requestSerial = 0;

        typedef Response ThisType;
        typedef xx::Object BaseType;
	    Response();
	    Response(xx::BBuffer *bb);
	    ~Response();
        virtual void ToString(xx::String &str) const override;
        virtual void ToStringCore(xx::String &str) const override;
        virtual void ToBBuffer(xx::BBuffer &bb) const override;
        virtual int FromBBuffer(xx::BBuffer &bb) override;
    };
    // 一个请求( 当前限定为 service 与 db 间 ), 通常携带一个流水号. 这是基类
    struct Request : xx::Object
    {
        int32_t serial = 0;

        typedef Request ThisType;
        typedef xx::Object BaseType;
	    Request();
	    Request(xx::BBuffer *bb);
	    ~Request();
        virtual void ToString(xx::String &str) const override;
        virtual void ToStringCore(xx::String &str) const override;
        virtual void ToBBuffer(xx::BBuffer &bb) const override;
        virtual int FromBBuffer(xx::BBuffer &bb) override;
    };
    struct Property_long : PKG::Property
    {
        int64_t value = 0;

        typedef Property_long ThisType;
        typedef PKG::Property BaseType;
	    Property_long();
	    Property_long(xx::BBuffer *bb);
	    ~Property_long();
        virtual void ToString(xx::String &str) const override;
        virtual void ToStringCore(xx::String &str) const override;
        virtual void ToBBuffer(xx::BBuffer &bb) const override;
        virtual int FromBBuffer(xx::BBuffer &bb) override;
    };
    struct Property_double : PKG::Property
    {
        double value = 0;

        typedef Property_double ThisType;
        typedef PKG::Property BaseType;
	    Property_double();
	    Property_double(xx::BBuffer *bb);
	    ~Property_double();
        virtual void ToString(xx::String &str) const override;
        virtual void ToStringCore(xx::String &str) const override;
        virtual void ToBBuffer(xx::BBuffer &bb) const override;
        virtual int FromBBuffer(xx::BBuffer &bb) override;
    };
    struct Property_string : PKG::Property
    {
        xx::String* value = nullptr;

        typedef Property_string ThisType;
        typedef PKG::Property BaseType;
	    Property_string();
	    Property_string(xx::BBuffer *bb);
	    ~Property_string();
        virtual void ToString(xx::String &str) const override;
        virtual void ToStringCore(xx::String &str) const override;
        virtual void ToBBuffer(xx::BBuffer &bb) const override;
        virtual int FromBBuffer(xx::BBuffer &bb) override;
    };
    struct Properties : PKG::Property
    {
        xx::List<PKG::Property*>* value = nullptr;

        typedef Properties ThisType;
        typedef PKG::Property BaseType;
	    Properties();
	    Properties(xx::BBuffer *bb);
	    ~Properties();
        virtual void ToString(xx::String &str) const override;
        virtual void ToStringCore(xx::String &str) const override;
        virtual void ToBBuffer(xx::BBuffer &bb) const override;
        virtual int FromBBuffer(xx::BBuffer &bb) override;
    };
    struct UserInfo : xx::Object
    {
        int64_t id = 0;
        xx::List<PKG::Property*>* props = nullptr;

        typedef UserInfo ThisType;
        typedef xx::Object BaseType;
	    UserInfo();
	    UserInfo(xx::BBuffer *bb);
	    ~UserInfo();
        virtual void ToString(xx::String &str) const override;
        virtual void ToStringCore(xx::String &str) const override;
        virtual void ToBBuffer(xx::BBuffer &bb) const override;
        virtual int FromBBuffer(xx::BBuffer &bb) override;
    };
namespace Server_Client
{
    // 进入成功, 返回用户信息
    struct JoinSuccess : PKG::Response
    {
        PKG::UserInfo* self = nullptr;
        xx::List<PKG::UserInfo*>* users = nullptr;

        typedef JoinSuccess ThisType;
        typedef PKG::Response BaseType;
	    JoinSuccess();
	    JoinSuccess(xx::BBuffer *bb);
	    ~JoinSuccess();
        virtual void ToString(xx::String &str) const override;
        virtual void ToStringCore(xx::String &str) const override;
        virtual void ToBBuffer(xx::BBuffer &bb) const override;
        virtual int FromBBuffer(xx::BBuffer &bb) override;
    };
    // 进入失败, 返回错误信息
    struct JoinFail : PKG::Response
    {
        xx::String* reason = nullptr;

        typedef JoinFail ThisType;
        typedef PKG::Response BaseType;
	    JoinFail();
	    JoinFail(xx::BBuffer *bb);
	    ~JoinFail();
        virtual void ToString(xx::String &str) const override;
        virtual void ToStringCore(xx::String &str) const override;
        virtual void ToBBuffer(xx::BBuffer &bb) const override;
        virtual int FromBBuffer(xx::BBuffer &bb) override;
    };
    // 推送文字消息
    struct PushJoin : xx::Object
    {
        int64_t id = 0;

        typedef PushJoin ThisType;
        typedef xx::Object BaseType;
	    PushJoin();
	    PushJoin(xx::BBuffer *bb);
	    ~PushJoin();
        virtual void ToString(xx::String &str) const override;
        virtual void ToStringCore(xx::String &str) const override;
        virtual void ToBBuffer(xx::BBuffer &bb) const override;
        virtual int FromBBuffer(xx::BBuffer &bb) override;
    };
    // 推送文字消息
    struct PushMessage : xx::Object
    {
        int64_t id = 0;
        xx::String* text = nullptr;

        typedef PushMessage ThisType;
        typedef xx::Object BaseType;
	    PushMessage();
	    PushMessage(xx::BBuffer *bb);
	    ~PushMessage();
        virtual void ToString(xx::String &str) const override;
        virtual void ToStringCore(xx::String &str) const override;
        virtual void ToBBuffer(xx::BBuffer &bb) const override;
        virtual int FromBBuffer(xx::BBuffer &bb) override;
    };
    // 推送退出消息
    struct PushLogout : xx::Object
    {
        int64_t id = 0;
        // 退出原因( 主动? 掉线?
        xx::String* reason = nullptr;

        typedef PushLogout ThisType;
        typedef xx::Object BaseType;
	    PushLogout();
	    PushLogout(xx::BBuffer *bb);
	    ~PushLogout();
        virtual void ToString(xx::String &str) const override;
        virtual void ToStringCore(xx::String &str) const override;
        virtual void ToBBuffer(xx::BBuffer &bb) const override;
        virtual int FromBBuffer(xx::BBuffer &bb) override;
    };
}
namespace Client_Server
{
    // 请求进入服务器
    struct Join : PKG::Request
    {
        xx::String* username = nullptr;
        xx::String* password = nullptr;

        typedef Join ThisType;
        typedef PKG::Request BaseType;
	    Join();
	    Join(xx::BBuffer *bb);
	    ~Join();
        virtual void ToString(xx::String &str) const override;
        virtual void ToStringCore(xx::String &str) const override;
        virtual void ToBBuffer(xx::BBuffer &bb) const override;
        virtual int FromBBuffer(xx::BBuffer &bb) override;
    };
    // 发消息
    struct Message : xx::Object
    {
        xx::String* text = nullptr;

        typedef Message ThisType;
        typedef xx::Object BaseType;
	    Message();
	    Message(xx::BBuffer *bb);
	    ~Message();
        virtual void ToString(xx::String &str) const override;
        virtual void ToStringCore(xx::String &str) const override;
        virtual void ToBBuffer(xx::BBuffer &bb) const override;
        virtual int FromBBuffer(xx::BBuffer &bb) override;
    };
    // 主动退出
    struct Logout : xx::Object
    {

        typedef Logout ThisType;
        typedef xx::Object BaseType;
	    Logout();
	    Logout(xx::BBuffer *bb);
	    ~Logout();
        virtual void ToString(xx::String &str) const override;
        virtual void ToStringCore(xx::String &str) const override;
        virtual void ToBBuffer(xx::BBuffer &bb) const override;
        virtual int FromBBuffer(xx::BBuffer &bb) override;
    };
}
	inline Request::Request()
	{
	}
	inline Request::Request(xx::BBuffer *bb)
	{
	    int rtv = 0;
        if (rtv = bb->Read(serial)) throw rtv;
	}
	inline Request::~Request()
	{
	}

    inline void Request::ToString(xx::String &str) const
    {
        if (tsFlags())
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else tsFlags() = 1;

        str.Append("{ \"type\" : \"Request\"");
        ToStringCore(str);
        str.Append(" }");
        
        tsFlags() = 0;
    }
    inline void Request::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"serial\" : ", this->serial);
    }


    inline void Request::ToBBuffer(xx::BBuffer &bb) const
    {
        bb.Write(this->serial);
    }

    inline int Request::FromBBuffer(xx::BBuffer &bb)
    {
        int rtv = 0;
        if (rtv = bb.Read(this->serial)) return rtv;
        return rtv;
    }

	inline Response::Response()
	{
	}
	inline Response::Response(xx::BBuffer *bb)
	{
	    int rtv = 0;
        if (rtv = bb->Read(requestSerial)) throw rtv;
	}
	inline Response::~Response()
	{
	}

    inline void Response::ToString(xx::String &str) const
    {
        if (tsFlags())
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else tsFlags() = 1;

        str.Append("{ \"type\" : \"Response\"");
        ToStringCore(str);
        str.Append(" }");
        
        tsFlags() = 0;
    }
    inline void Response::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"requestSerial\" : ", this->requestSerial);
    }


    inline void Response::ToBBuffer(xx::BBuffer &bb) const
    {
        bb.Write(this->requestSerial);
    }

    inline int Response::FromBBuffer(xx::BBuffer &bb)
    {
        int rtv = 0;
        if (rtv = bb.Read(this->requestSerial)) return rtv;
        return rtv;
    }

	inline Property::Property()
	{
	}
	inline Property::Property(xx::BBuffer *bb)
	{
	    int rtv = 0;
        bb->readLengthLimit = 0;
        if (rtv = bb->Read(name)) throw rtv;
	}
	inline Property::~Property()
	{
        mempool().SafeRelease(name);
	}

    inline void Property::ToString(xx::String &str) const
    {
        if (tsFlags())
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else tsFlags() = 1;

        str.Append("{ \"type\" : \"Property\"");
        ToStringCore(str);
        str.Append(" }");
        
        tsFlags() = 0;
    }
    inline void Property::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"name\" : ", this->name);
    }


    inline void Property::ToBBuffer(xx::BBuffer &bb) const
    {
        bb.Write(this->name);
    }

    inline int Property::FromBBuffer(xx::BBuffer &bb)
    {
        int rtv = 0;
        bb.readLengthLimit = 0;
        if (rtv = bb.Read(this->name)) return rtv;
        return rtv;
    }

	inline Property_long::Property_long()
        : PKG::Property()
	{
	}
	inline Property_long::Property_long(xx::BBuffer *bb)
        : PKG::Property(bb)
	{
	    int rtv = 0;
        if (rtv = bb->Read(value)) throw rtv;
	}
	inline Property_long::~Property_long()
	{
	}

    inline void Property_long::ToString(xx::String &str) const
    {
        if (tsFlags())
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else tsFlags() = 1;

        str.Append("{ \"type\" : \"Property_long\"");
        ToStringCore(str);
        str.Append(" }");
        
        tsFlags() = 0;
    }
    inline void Property_long::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"value\" : ", this->value);
    }


    inline void Property_long::ToBBuffer(xx::BBuffer &bb) const
    {
        this->BaseType::ToBBuffer(bb);
        bb.Write(this->value);
    }

    inline int Property_long::FromBBuffer(xx::BBuffer &bb)
    {
        int rtv = 0;
        if (rtv = this->BaseType::FromBBuffer(bb)) return rtv;
        if (rtv = bb.Read(this->value)) return rtv;
        return rtv;
    }

	inline Property_double::Property_double()
        : PKG::Property()
	{
	}
	inline Property_double::Property_double(xx::BBuffer *bb)
        : PKG::Property(bb)
	{
	    int rtv = 0;
        if (rtv = bb->Read(value)) throw rtv;
	}
	inline Property_double::~Property_double()
	{
	}

    inline void Property_double::ToString(xx::String &str) const
    {
        if (tsFlags())
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else tsFlags() = 1;

        str.Append("{ \"type\" : \"Property_double\"");
        ToStringCore(str);
        str.Append(" }");
        
        tsFlags() = 0;
    }
    inline void Property_double::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"value\" : ", this->value);
    }


    inline void Property_double::ToBBuffer(xx::BBuffer &bb) const
    {
        this->BaseType::ToBBuffer(bb);
        bb.Write(this->value);
    }

    inline int Property_double::FromBBuffer(xx::BBuffer &bb)
    {
        int rtv = 0;
        if (rtv = this->BaseType::FromBBuffer(bb)) return rtv;
        if (rtv = bb.Read(this->value)) return rtv;
        return rtv;
    }

	inline Property_string::Property_string()
        : PKG::Property()
	{
	}
	inline Property_string::Property_string(xx::BBuffer *bb)
        : PKG::Property(bb)
	{
	    int rtv = 0;
        bb->readLengthLimit = 0;
        if (rtv = bb->Read(value)) throw rtv;
	}
	inline Property_string::~Property_string()
	{
        mempool().SafeRelease(value);
	}

    inline void Property_string::ToString(xx::String &str) const
    {
        if (tsFlags())
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else tsFlags() = 1;

        str.Append("{ \"type\" : \"Property_string\"");
        ToStringCore(str);
        str.Append(" }");
        
        tsFlags() = 0;
    }
    inline void Property_string::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"value\" : ", this->value);
    }


    inline void Property_string::ToBBuffer(xx::BBuffer &bb) const
    {
        this->BaseType::ToBBuffer(bb);
        bb.Write(this->value);
    }

    inline int Property_string::FromBBuffer(xx::BBuffer &bb)
    {
        int rtv = 0;
        if (rtv = this->BaseType::FromBBuffer(bb)) return rtv;
        bb.readLengthLimit = 0;
        if (rtv = bb.Read(this->value)) return rtv;
        return rtv;
    }

	inline Properties::Properties()
        : PKG::Property()
	{
	}
	inline Properties::Properties(xx::BBuffer *bb)
        : PKG::Property(bb)
	{
	    int rtv = 0;
        bb->readLengthLimit = 0;
        if (rtv = bb->Read(value)) throw rtv;
	}
	inline Properties::~Properties()
	{
        mempool().SafeRelease(value);
	}

    inline void Properties::ToString(xx::String &str) const
    {
        if (tsFlags())
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else tsFlags() = 1;

        str.Append("{ \"type\" : \"Properties\"");
        ToStringCore(str);
        str.Append(" }");
        
        tsFlags() = 0;
    }
    inline void Properties::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"value\" : ", this->value);
    }


    inline void Properties::ToBBuffer(xx::BBuffer &bb) const
    {
        this->BaseType::ToBBuffer(bb);
        bb.Write(this->value);
    }

    inline int Properties::FromBBuffer(xx::BBuffer &bb)
    {
        int rtv = 0;
        if (rtv = this->BaseType::FromBBuffer(bb)) return rtv;
        bb.readLengthLimit = 0;
        if (rtv = bb.Read(this->value)) return rtv;
        return rtv;
    }

	inline UserInfo::UserInfo()
	{
	}
	inline UserInfo::UserInfo(xx::BBuffer *bb)
	{
	    int rtv = 0;
        if (rtv = bb->Read(id)) throw rtv;
        bb->readLengthLimit = 0;
        if (rtv = bb->Read(props)) throw rtv;
	}
	inline UserInfo::~UserInfo()
	{
        mempool().SafeRelease(props);
	}

    inline void UserInfo::ToString(xx::String &str) const
    {
        if (tsFlags())
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else tsFlags() = 1;

        str.Append("{ \"type\" : \"UserInfo\"");
        ToStringCore(str);
        str.Append(" }");
        
        tsFlags() = 0;
    }
    inline void UserInfo::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"id\" : ", this->id);
        str.Append(", \"props\" : ", this->props);
    }


    inline void UserInfo::ToBBuffer(xx::BBuffer &bb) const
    {
        bb.Write(this->id);
        bb.Write(this->props);
    }

    inline int UserInfo::FromBBuffer(xx::BBuffer &bb)
    {
        int rtv = 0;
        if (rtv = bb.Read(this->id)) return rtv;
        bb.readLengthLimit = 0;
        if (rtv = bb.Read(this->props)) return rtv;
        return rtv;
    }

namespace Server_Client
{
	inline JoinSuccess::JoinSuccess()
        : PKG::Response()
	{
	}
	inline JoinSuccess::JoinSuccess(xx::BBuffer *bb)
        : PKG::Response(bb)
	{
	    int rtv = 0;
        if (rtv = bb->Read(self)) throw rtv;
        bb->readLengthLimit = 0;
        if (rtv = bb->Read(users)) throw rtv;
	}
	inline JoinSuccess::~JoinSuccess()
	{
        mempool().SafeRelease(self);
        mempool().SafeRelease(users);
	}

    inline void JoinSuccess::ToString(xx::String &str) const
    {
        if (tsFlags())
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else tsFlags() = 1;

        str.Append("{ \"type\" : \"JoinSuccess\"");
        ToStringCore(str);
        str.Append(" }");
        
        tsFlags() = 0;
    }
    inline void JoinSuccess::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"self\" : ", this->self);
        str.Append(", \"users\" : ", this->users);
    }


    inline void JoinSuccess::ToBBuffer(xx::BBuffer &bb) const
    {
        this->BaseType::ToBBuffer(bb);
        bb.Write(this->self);
        bb.Write(this->users);
    }

    inline int JoinSuccess::FromBBuffer(xx::BBuffer &bb)
    {
        int rtv = 0;
        if (rtv = this->BaseType::FromBBuffer(bb)) return rtv;
        if (rtv = bb.Read(this->self)) return rtv;
        bb.readLengthLimit = 0;
        if (rtv = bb.Read(this->users)) return rtv;
        return rtv;
    }

	inline JoinFail::JoinFail()
        : PKG::Response()
	{
	}
	inline JoinFail::JoinFail(xx::BBuffer *bb)
        : PKG::Response(bb)
	{
	    int rtv = 0;
        bb->readLengthLimit = 0;
        if (rtv = bb->Read(reason)) throw rtv;
	}
	inline JoinFail::~JoinFail()
	{
        mempool().SafeRelease(reason);
	}

    inline void JoinFail::ToString(xx::String &str) const
    {
        if (tsFlags())
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else tsFlags() = 1;

        str.Append("{ \"type\" : \"JoinFail\"");
        ToStringCore(str);
        str.Append(" }");
        
        tsFlags() = 0;
    }
    inline void JoinFail::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"reason\" : ", this->reason);
    }


    inline void JoinFail::ToBBuffer(xx::BBuffer &bb) const
    {
        this->BaseType::ToBBuffer(bb);
        bb.Write(this->reason);
    }

    inline int JoinFail::FromBBuffer(xx::BBuffer &bb)
    {
        int rtv = 0;
        if (rtv = this->BaseType::FromBBuffer(bb)) return rtv;
        bb.readLengthLimit = 0;
        if (rtv = bb.Read(this->reason)) return rtv;
        return rtv;
    }

	inline PushJoin::PushJoin()
	{
	}
	inline PushJoin::PushJoin(xx::BBuffer *bb)
	{
	    int rtv = 0;
        if (rtv = bb->Read(id)) throw rtv;
	}
	inline PushJoin::~PushJoin()
	{
	}

    inline void PushJoin::ToString(xx::String &str) const
    {
        if (tsFlags())
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else tsFlags() = 1;

        str.Append("{ \"type\" : \"PushJoin\"");
        ToStringCore(str);
        str.Append(" }");
        
        tsFlags() = 0;
    }
    inline void PushJoin::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"id\" : ", this->id);
    }


    inline void PushJoin::ToBBuffer(xx::BBuffer &bb) const
    {
        bb.Write(this->id);
    }

    inline int PushJoin::FromBBuffer(xx::BBuffer &bb)
    {
        int rtv = 0;
        if (rtv = bb.Read(this->id)) return rtv;
        return rtv;
    }

	inline PushMessage::PushMessage()
	{
	}
	inline PushMessage::PushMessage(xx::BBuffer *bb)
	{
	    int rtv = 0;
        if (rtv = bb->Read(id)) throw rtv;
        bb->readLengthLimit = 0;
        if (rtv = bb->Read(text)) throw rtv;
	}
	inline PushMessage::~PushMessage()
	{
        mempool().SafeRelease(text);
	}

    inline void PushMessage::ToString(xx::String &str) const
    {
        if (tsFlags())
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else tsFlags() = 1;

        str.Append("{ \"type\" : \"PushMessage\"");
        ToStringCore(str);
        str.Append(" }");
        
        tsFlags() = 0;
    }
    inline void PushMessage::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"id\" : ", this->id);
        str.Append(", \"text\" : ", this->text);
    }


    inline void PushMessage::ToBBuffer(xx::BBuffer &bb) const
    {
        bb.Write(this->id);
        bb.Write(this->text);
    }

    inline int PushMessage::FromBBuffer(xx::BBuffer &bb)
    {
        int rtv = 0;
        if (rtv = bb.Read(this->id)) return rtv;
        bb.readLengthLimit = 0;
        if (rtv = bb.Read(this->text)) return rtv;
        return rtv;
    }

	inline PushLogout::PushLogout()
	{
	}
	inline PushLogout::PushLogout(xx::BBuffer *bb)
	{
	    int rtv = 0;
        if (rtv = bb->Read(id)) throw rtv;
        bb->readLengthLimit = 0;
        if (rtv = bb->Read(reason)) throw rtv;
	}
	inline PushLogout::~PushLogout()
	{
        mempool().SafeRelease(reason);
	}

    inline void PushLogout::ToString(xx::String &str) const
    {
        if (tsFlags())
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else tsFlags() = 1;

        str.Append("{ \"type\" : \"PushLogout\"");
        ToStringCore(str);
        str.Append(" }");
        
        tsFlags() = 0;
    }
    inline void PushLogout::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"id\" : ", this->id);
        str.Append(", \"reason\" : ", this->reason);
    }


    inline void PushLogout::ToBBuffer(xx::BBuffer &bb) const
    {
        bb.Write(this->id);
        bb.Write(this->reason);
    }

    inline int PushLogout::FromBBuffer(xx::BBuffer &bb)
    {
        int rtv = 0;
        if (rtv = bb.Read(this->id)) return rtv;
        bb.readLengthLimit = 0;
        if (rtv = bb.Read(this->reason)) return rtv;
        return rtv;
    }

}
namespace Client_Server
{
	inline Join::Join()
        : PKG::Request()
	{
	}
	inline Join::Join(xx::BBuffer *bb)
        : PKG::Request(bb)
	{
	    int rtv = 0;
        bb->readLengthLimit = 16;
        if (rtv = bb->Read(username)) throw rtv;
        bb->readLengthLimit = 16;
        if (rtv = bb->Read(password)) throw rtv;
	}
	inline Join::~Join()
	{
        mempool().SafeRelease(username);
        mempool().SafeRelease(password);
	}

    inline void Join::ToString(xx::String &str) const
    {
        if (tsFlags())
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else tsFlags() = 1;

        str.Append("{ \"type\" : \"Join\"");
        ToStringCore(str);
        str.Append(" }");
        
        tsFlags() = 0;
    }
    inline void Join::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"username\" : ", this->username);
        str.Append(", \"password\" : ", this->password);
    }


    inline void Join::ToBBuffer(xx::BBuffer &bb) const
    {
        this->BaseType::ToBBuffer(bb);
        bb.Write(this->username);
        bb.Write(this->password);
    }

    inline int Join::FromBBuffer(xx::BBuffer &bb)
    {
        int rtv = 0;
        if (rtv = this->BaseType::FromBBuffer(bb)) return rtv;
        bb.readLengthLimit = 16;
        if (rtv = bb.Read(this->username)) return rtv;
        bb.readLengthLimit = 16;
        if (rtv = bb.Read(this->password)) return rtv;
        return rtv;
    }

	inline Message::Message()
	{
	}
	inline Message::Message(xx::BBuffer *bb)
	{
	    int rtv = 0;
        bb->readLengthLimit = 256;
        if (rtv = bb->Read(text)) throw rtv;
	}
	inline Message::~Message()
	{
        mempool().SafeRelease(text);
	}

    inline void Message::ToString(xx::String &str) const
    {
        if (tsFlags())
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else tsFlags() = 1;

        str.Append("{ \"type\" : \"Message\"");
        ToStringCore(str);
        str.Append(" }");
        
        tsFlags() = 0;
    }
    inline void Message::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"text\" : ", this->text);
    }


    inline void Message::ToBBuffer(xx::BBuffer &bb) const
    {
        bb.Write(this->text);
    }

    inline int Message::FromBBuffer(xx::BBuffer &bb)
    {
        int rtv = 0;
        bb.readLengthLimit = 256;
        if (rtv = bb.Read(this->text)) return rtv;
        return rtv;
    }

	inline Logout::Logout()
	{
	}
	inline Logout::Logout(xx::BBuffer *bb)
	{
	}
	inline Logout::~Logout()
	{
	}

    inline void Logout::ToString(xx::String &str) const
    {
        if (tsFlags())
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else tsFlags() = 1;

        str.Append("{ \"type\" : \"Logout\"");
        ToStringCore(str);
        str.Append(" }");
        
        tsFlags() = 0;
    }
    inline void Logout::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
    }


    inline void Logout::ToBBuffer(xx::BBuffer &bb) const
    {
    }

    inline int Logout::FromBBuffer(xx::BBuffer &bb)
    {
        int rtv = 0;
        return rtv;
    }

}
}
namespace xx
{
	template<> struct TypeId<xx::String> { static const uint16_t value = 1; };
	template<> struct TypeId<xx::BBuffer> { static const uint16_t value = 2; };
	template<> struct TypeId<PKG::Request> { static const uint16_t value = 3; };
	template<> struct TypeId<PKG::Response> { static const uint16_t value = 4; };
	template<> struct TypeId<PKG::Property> { static const uint16_t value = 5; };
	template<> struct TypeId<PKG::Property_long> { static const uint16_t value = 6; };
	template<> struct TypeId<PKG::Property_double> { static const uint16_t value = 7; };
	template<> struct TypeId<PKG::Property_string> { static const uint16_t value = 8; };
	template<> struct TypeId<PKG::Properties> { static const uint16_t value = 9; };
	template<> struct TypeId<xx::List<PKG::Property*>> { static const uint16_t value = 10; };
	template<> struct TypeId<PKG::UserInfo> { static const uint16_t value = 11; };
	template<> struct TypeId<PKG::Server_Client::JoinSuccess> { static const uint16_t value = 12; };
	template<> struct TypeId<xx::List<PKG::UserInfo*>> { static const uint16_t value = 13; };
	template<> struct TypeId<PKG::Server_Client::JoinFail> { static const uint16_t value = 14; };
	template<> struct TypeId<PKG::Server_Client::PushJoin> { static const uint16_t value = 15; };
	template<> struct TypeId<PKG::Server_Client::PushMessage> { static const uint16_t value = 16; };
	template<> struct TypeId<PKG::Server_Client::PushLogout> { static const uint16_t value = 17; };
	template<> struct TypeId<PKG::Client_Server::Join> { static const uint16_t value = 18; };
	template<> struct TypeId<PKG::Client_Server::Message> { static const uint16_t value = 19; };
	template<> struct TypeId<PKG::Client_Server::Logout> { static const uint16_t value = 20; };
}
namespace PKG
{
	inline void AllTypesRegister()
	{
	    xx::MemPool::Register<xx::String, xx::Object>();
	    xx::MemPool::Register<xx::BBuffer, xx::Object>();
	    xx::MemPool::Register<PKG::Request, xx::Object>();
	    xx::MemPool::Register<PKG::Response, xx::Object>();
	    xx::MemPool::Register<PKG::Property, xx::Object>();
	    xx::MemPool::Register<PKG::Property_long, PKG::Property>();
	    xx::MemPool::Register<PKG::Property_double, PKG::Property>();
	    xx::MemPool::Register<PKG::Property_string, PKG::Property>();
	    xx::MemPool::Register<PKG::Properties, PKG::Property>();
	    xx::MemPool::Register<xx::List<PKG::Property*>, xx::Object>();
	    xx::MemPool::Register<PKG::UserInfo, xx::Object>();
	    xx::MemPool::Register<PKG::Server_Client::JoinSuccess, PKG::Response>();
	    xx::MemPool::Register<xx::List<PKG::UserInfo*>, xx::Object>();
	    xx::MemPool::Register<PKG::Server_Client::JoinFail, PKG::Response>();
	    xx::MemPool::Register<PKG::Server_Client::PushJoin, xx::Object>();
	    xx::MemPool::Register<PKG::Server_Client::PushMessage, xx::Object>();
	    xx::MemPool::Register<PKG::Server_Client::PushLogout, xx::Object>();
	    xx::MemPool::Register<PKG::Client_Server::Join, PKG::Request>();
	    xx::MemPool::Register<PKG::Client_Server::Message, xx::Object>();
	    xx::MemPool::Register<PKG::Client_Server::Logout, xx::Object>();
	}
}
