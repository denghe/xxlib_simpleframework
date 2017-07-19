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
    struct Property : xx::MPObject
    {
        typedef Property ThisType;
        typedef xx::MPObject BaseType;
        xx::String* name = nullptr;
#pragma region ctor, interface impls

	    Property()
	    {
	    }
	    Property(xx::BBuffer *bb)
	    {
	        int rtv = 0;
            bb->readLengthLimit = 0;
            if (rtv = bb->Read(name)) throw rtv;
	    }
	    ~Property()
	    {
            mempool().SafeRelease(name);
	    }

        inline virtual void ToString(xx::String &str) const override
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
        inline virtual void ToStringCore(xx::String &str) const override
        {
            this->BaseType::ToStringCore(str);
            str.Append(", \"name\" : ", this->name);
        }


        inline virtual void ToBBuffer(xx::BBuffer &bb) const override
        {
            bb.Write(this->name);
        }

        inline virtual int FromBBuffer(xx::BBuffer &bb) override
        {
            int rtv = 0;
            bb.readLengthLimit = 0;
            if (rtv = bb.Read(this->name)) return rtv;
            return rtv;
        }

#pragma endregion
    };
    // 一个回应( 当前限定为 service 与 db 间 ), 通常携带一个请求发过来的流水号. 这是基类
    struct Response : xx::MPObject
    {
        typedef Response ThisType;
        typedef xx::MPObject BaseType;
        int32_t requestSerial = 0;
#pragma region ctor, interface impls

	    Response()
	    {
	    }
	    Response(xx::BBuffer *bb)
	    {
	        int rtv = 0;
            if (rtv = bb->Read(requestSerial)) throw rtv;
	    }
	    ~Response()
	    {
	    }

        inline virtual void ToString(xx::String &str) const override
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
        inline virtual void ToStringCore(xx::String &str) const override
        {
            this->BaseType::ToStringCore(str);
            str.Append(", \"requestSerial\" : ", this->requestSerial);
        }


        inline virtual void ToBBuffer(xx::BBuffer &bb) const override
        {
            bb.Write(this->requestSerial);
        }

        inline virtual int FromBBuffer(xx::BBuffer &bb) override
        {
            int rtv = 0;
            if (rtv = bb.Read(this->requestSerial)) return rtv;
            return rtv;
        }

#pragma endregion
    };
    // 一个请求( 当前限定为 service 与 db 间 ), 通常携带一个流水号. 这是基类
    struct Request : xx::MPObject
    {
        typedef Request ThisType;
        typedef xx::MPObject BaseType;
        int32_t serial = 0;
#pragma region ctor, interface impls

	    Request()
	    {
	    }
	    Request(xx::BBuffer *bb)
	    {
	        int rtv = 0;
            if (rtv = bb->Read(serial)) throw rtv;
	    }
	    ~Request()
	    {
	    }

        inline virtual void ToString(xx::String &str) const override
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
        inline virtual void ToStringCore(xx::String &str) const override
        {
            this->BaseType::ToStringCore(str);
            str.Append(", \"serial\" : ", this->serial);
        }


        inline virtual void ToBBuffer(xx::BBuffer &bb) const override
        {
            bb.Write(this->serial);
        }

        inline virtual int FromBBuffer(xx::BBuffer &bb) override
        {
            int rtv = 0;
            if (rtv = bb.Read(this->serial)) return rtv;
            return rtv;
        }

#pragma endregion
    };
    struct Property_long : PKG::Property
    {
        typedef Property_long ThisType;
        typedef PKG::Property BaseType;
        int64_t value = 0;
#pragma region ctor, interface impls

	    Property_long()
            : BaseType()
	    {
	    }
	    Property_long(xx::BBuffer *bb)
            : BaseType(bb)
	    {
	        int rtv = 0;
            if (rtv = bb->Read(value)) throw rtv;
	    }
	    ~Property_long()
	    {
	    }

        inline virtual void ToString(xx::String &str) const override
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
        inline virtual void ToStringCore(xx::String &str) const override
        {
            this->BaseType::ToStringCore(str);
            str.Append(", \"value\" : ", this->value);
        }


        inline virtual void ToBBuffer(xx::BBuffer &bb) const override
        {
            this->BaseType::ToBBuffer(bb);
            bb.Write(this->value);
        }

        inline virtual int FromBBuffer(xx::BBuffer &bb) override
        {
            int rtv = 0;
            if (rtv = this->BaseType::FromBBuffer(bb)) return rtv;
            if (rtv = bb.Read(this->value)) return rtv;
            return rtv;
        }

#pragma endregion
    };
    struct Property_double : PKG::Property
    {
        typedef Property_double ThisType;
        typedef PKG::Property BaseType;
        double value = 0;
#pragma region ctor, interface impls

	    Property_double()
            : BaseType()
	    {
	    }
	    Property_double(xx::BBuffer *bb)
            : BaseType(bb)
	    {
	        int rtv = 0;
            if (rtv = bb->Read(value)) throw rtv;
	    }
	    ~Property_double()
	    {
	    }

        inline virtual void ToString(xx::String &str) const override
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
        inline virtual void ToStringCore(xx::String &str) const override
        {
            this->BaseType::ToStringCore(str);
            str.Append(", \"value\" : ", this->value);
        }


        inline virtual void ToBBuffer(xx::BBuffer &bb) const override
        {
            this->BaseType::ToBBuffer(bb);
            bb.Write(this->value);
        }

        inline virtual int FromBBuffer(xx::BBuffer &bb) override
        {
            int rtv = 0;
            if (rtv = this->BaseType::FromBBuffer(bb)) return rtv;
            if (rtv = bb.Read(this->value)) return rtv;
            return rtv;
        }

#pragma endregion
    };
    struct Property_string : PKG::Property
    {
        typedef Property_string ThisType;
        typedef PKG::Property BaseType;
        xx::String* value = nullptr;
#pragma region ctor, interface impls

	    Property_string()
            : BaseType()
	    {
	    }
	    Property_string(xx::BBuffer *bb)
            : BaseType(bb)
	    {
	        int rtv = 0;
            bb->readLengthLimit = 0;
            if (rtv = bb->Read(value)) throw rtv;
	    }
	    ~Property_string()
	    {
            mempool().SafeRelease(value);
	    }

        inline virtual void ToString(xx::String &str) const override
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
        inline virtual void ToStringCore(xx::String &str) const override
        {
            this->BaseType::ToStringCore(str);
            str.Append(", \"value\" : ", this->value);
        }


        inline virtual void ToBBuffer(xx::BBuffer &bb) const override
        {
            this->BaseType::ToBBuffer(bb);
            bb.Write(this->value);
        }

        inline virtual int FromBBuffer(xx::BBuffer &bb) override
        {
            int rtv = 0;
            if (rtv = this->BaseType::FromBBuffer(bb)) return rtv;
            bb.readLengthLimit = 0;
            if (rtv = bb.Read(this->value)) return rtv;
            return rtv;
        }

#pragma endregion
    };
    struct Properties : PKG::Property
    {
        typedef Properties ThisType;
        typedef PKG::Property BaseType;
        xx::List<PKG::Property*>* value = nullptr;
#pragma region ctor, interface impls

	    Properties()
            : BaseType()
	    {
	    }
	    Properties(xx::BBuffer *bb)
            : BaseType(bb)
	    {
	        int rtv = 0;
            bb->readLengthLimit = 0;
            if (rtv = bb->Read(value)) throw rtv;
	    }
	    ~Properties()
	    {
            mempool().SafeRelease(value);
	    }

        inline virtual void ToString(xx::String &str) const override
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
        inline virtual void ToStringCore(xx::String &str) const override
        {
            this->BaseType::ToStringCore(str);
            str.Append(", \"value\" : ", this->value);
        }


        inline virtual void ToBBuffer(xx::BBuffer &bb) const override
        {
            this->BaseType::ToBBuffer(bb);
            bb.Write(this->value);
        }

        inline virtual int FromBBuffer(xx::BBuffer &bb) override
        {
            int rtv = 0;
            if (rtv = this->BaseType::FromBBuffer(bb)) return rtv;
            bb.readLengthLimit = 0;
            if (rtv = bb.Read(this->value)) return rtv;
            return rtv;
        }

#pragma endregion
    };
    struct UserInfo : xx::MPObject
    {
        typedef UserInfo ThisType;
        typedef xx::MPObject BaseType;
        int32_t id = 0;
        xx::List<PKG::Property*>* props = nullptr;
#pragma region ctor, interface impls

	    UserInfo()
	    {
	    }
	    UserInfo(xx::BBuffer *bb)
	    {
	        int rtv = 0;
            if (rtv = bb->Read(id)) throw rtv;
            bb->readLengthLimit = 0;
            if (rtv = bb->Read(props)) throw rtv;
	    }
	    ~UserInfo()
	    {
            mempool().SafeRelease(props);
	    }

        inline virtual void ToString(xx::String &str) const override
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
        inline virtual void ToStringCore(xx::String &str) const override
        {
            this->BaseType::ToStringCore(str);
            str.Append(", \"id\" : ", this->id);
            str.Append(", \"props\" : ", this->props);
        }


        inline virtual void ToBBuffer(xx::BBuffer &bb) const override
        {
            bb.Write(this->id);
            bb.Write(this->props);
        }

        inline virtual int FromBBuffer(xx::BBuffer &bb) override
        {
            int rtv = 0;
            if (rtv = bb.Read(this->id)) return rtv;
            bb.readLengthLimit = 0;
            if (rtv = bb.Read(this->props)) return rtv;
            return rtv;
        }

#pragma endregion
    };
namespace Server_Client
{
    // 进入成功, 返回用户信息
    struct JoinSuccess : PKG::Response
    {
        typedef JoinSuccess ThisType;
        typedef PKG::Response BaseType;
        PKG::UserInfo* self = nullptr;
        xx::List<PKG::UserInfo*>* users = nullptr;
#pragma region ctor, interface impls

	    JoinSuccess()
            : BaseType()
	    {
	    }
	    JoinSuccess(xx::BBuffer *bb)
            : BaseType(bb)
	    {
	        int rtv = 0;
            if (rtv = bb->Read(self)) throw rtv;
            bb->readLengthLimit = 0;
            if (rtv = bb->Read(users)) throw rtv;
	    }
	    ~JoinSuccess()
	    {
            mempool().SafeRelease(self);
            mempool().SafeRelease(users);
	    }

        inline virtual void ToString(xx::String &str) const override
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
        inline virtual void ToStringCore(xx::String &str) const override
        {
            this->BaseType::ToStringCore(str);
            str.Append(", \"self\" : ", this->self);
            str.Append(", \"users\" : ", this->users);
        }


        inline virtual void ToBBuffer(xx::BBuffer &bb) const override
        {
            this->BaseType::ToBBuffer(bb);
            bb.Write(this->self);
            bb.Write(this->users);
        }

        inline virtual int FromBBuffer(xx::BBuffer &bb) override
        {
            int rtv = 0;
            if (rtv = this->BaseType::FromBBuffer(bb)) return rtv;
            if (rtv = bb.Read(this->self)) return rtv;
            bb.readLengthLimit = 0;
            if (rtv = bb.Read(this->users)) return rtv;
            return rtv;
        }

#pragma endregion
    };
    // 进入失败, 返回错误信息
    struct JoinFail : PKG::Response
    {
        typedef JoinFail ThisType;
        typedef PKG::Response BaseType;
        xx::String* reason = nullptr;
#pragma region ctor, interface impls

	    JoinFail()
            : BaseType()
	    {
	    }
	    JoinFail(xx::BBuffer *bb)
            : BaseType(bb)
	    {
	        int rtv = 0;
            bb->readLengthLimit = 0;
            if (rtv = bb->Read(reason)) throw rtv;
	    }
	    ~JoinFail()
	    {
            mempool().SafeRelease(reason);
	    }

        inline virtual void ToString(xx::String &str) const override
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
        inline virtual void ToStringCore(xx::String &str) const override
        {
            this->BaseType::ToStringCore(str);
            str.Append(", \"reason\" : ", this->reason);
        }


        inline virtual void ToBBuffer(xx::BBuffer &bb) const override
        {
            this->BaseType::ToBBuffer(bb);
            bb.Write(this->reason);
        }

        inline virtual int FromBBuffer(xx::BBuffer &bb) override
        {
            int rtv = 0;
            if (rtv = this->BaseType::FromBBuffer(bb)) return rtv;
            bb.readLengthLimit = 0;
            if (rtv = bb.Read(this->reason)) return rtv;
            return rtv;
        }

#pragma endregion
    };
    // 推送文字消息
    struct PushJoin : xx::MPObject
    {
        typedef PushJoin ThisType;
        typedef xx::MPObject BaseType;
        int32_t id = 0;
#pragma region ctor, interface impls

	    PushJoin()
	    {
	    }
	    PushJoin(xx::BBuffer *bb)
	    {
	        int rtv = 0;
            if (rtv = bb->Read(id)) throw rtv;
	    }
	    ~PushJoin()
	    {
	    }

        inline virtual void ToString(xx::String &str) const override
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
        inline virtual void ToStringCore(xx::String &str) const override
        {
            this->BaseType::ToStringCore(str);
            str.Append(", \"id\" : ", this->id);
        }


        inline virtual void ToBBuffer(xx::BBuffer &bb) const override
        {
            bb.Write(this->id);
        }

        inline virtual int FromBBuffer(xx::BBuffer &bb) override
        {
            int rtv = 0;
            if (rtv = bb.Read(this->id)) return rtv;
            return rtv;
        }

#pragma endregion
    };
    // 推送文字消息
    struct PushMessage : xx::MPObject
    {
        typedef PushMessage ThisType;
        typedef xx::MPObject BaseType;
        int32_t id = 0;
        xx::String* text = nullptr;
#pragma region ctor, interface impls

	    PushMessage()
	    {
	    }
	    PushMessage(xx::BBuffer *bb)
	    {
	        int rtv = 0;
            if (rtv = bb->Read(id)) throw rtv;
            bb->readLengthLimit = 0;
            if (rtv = bb->Read(text)) throw rtv;
	    }
	    ~PushMessage()
	    {
            mempool().SafeRelease(text);
	    }

        inline virtual void ToString(xx::String &str) const override
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
        inline virtual void ToStringCore(xx::String &str) const override
        {
            this->BaseType::ToStringCore(str);
            str.Append(", \"id\" : ", this->id);
            str.Append(", \"text\" : ", this->text);
        }


        inline virtual void ToBBuffer(xx::BBuffer &bb) const override
        {
            bb.Write(this->id);
            bb.Write(this->text);
        }

        inline virtual int FromBBuffer(xx::BBuffer &bb) override
        {
            int rtv = 0;
            if (rtv = bb.Read(this->id)) return rtv;
            bb.readLengthLimit = 0;
            if (rtv = bb.Read(this->text)) return rtv;
            return rtv;
        }

#pragma endregion
    };
    // 推送退出消息
    struct PushLogout : xx::MPObject
    {
        typedef PushLogout ThisType;
        typedef xx::MPObject BaseType;
        int32_t id = 0;
        // 退出原因( 主动? 掉线?
        xx::String* reason = nullptr;
#pragma region ctor, interface impls

	    PushLogout()
	    {
	    }
	    PushLogout(xx::BBuffer *bb)
	    {
	        int rtv = 0;
            if (rtv = bb->Read(id)) throw rtv;
            bb->readLengthLimit = 0;
            if (rtv = bb->Read(reason)) throw rtv;
	    }
	    ~PushLogout()
	    {
            mempool().SafeRelease(reason);
	    }

        inline virtual void ToString(xx::String &str) const override
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
        inline virtual void ToStringCore(xx::String &str) const override
        {
            this->BaseType::ToStringCore(str);
            str.Append(", \"id\" : ", this->id);
            str.Append(", \"reason\" : ", this->reason);
        }


        inline virtual void ToBBuffer(xx::BBuffer &bb) const override
        {
            bb.Write(this->id);
            bb.Write(this->reason);
        }

        inline virtual int FromBBuffer(xx::BBuffer &bb) override
        {
            int rtv = 0;
            if (rtv = bb.Read(this->id)) return rtv;
            bb.readLengthLimit = 0;
            if (rtv = bb.Read(this->reason)) return rtv;
            return rtv;
        }

#pragma endregion
    };
}
namespace Client_Server
{
    // 请求进入服务器
    struct Join : PKG::Request
    {
        typedef Join ThisType;
        typedef PKG::Request BaseType;
        xx::String* username = nullptr;
        xx::String* password = nullptr;
#pragma region ctor, interface impls

	    Join()
            : BaseType()
	    {
	    }
	    Join(xx::BBuffer *bb)
            : BaseType(bb)
	    {
	        int rtv = 0;
            bb->readLengthLimit = 16;
            if (rtv = bb->Read(username)) throw rtv;
            bb->readLengthLimit = 16;
            if (rtv = bb->Read(password)) throw rtv;
	    }
	    ~Join()
	    {
            mempool().SafeRelease(username);
            mempool().SafeRelease(password);
	    }

        inline virtual void ToString(xx::String &str) const override
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
        inline virtual void ToStringCore(xx::String &str) const override
        {
            this->BaseType::ToStringCore(str);
            str.Append(", \"username\" : ", this->username);
            str.Append(", \"password\" : ", this->password);
        }


        inline virtual void ToBBuffer(xx::BBuffer &bb) const override
        {
            this->BaseType::ToBBuffer(bb);
            bb.Write(this->username);
            bb.Write(this->password);
        }

        inline virtual int FromBBuffer(xx::BBuffer &bb) override
        {
            int rtv = 0;
            if (rtv = this->BaseType::FromBBuffer(bb)) return rtv;
            bb.readLengthLimit = 16;
            if (rtv = bb.Read(this->username)) return rtv;
            bb.readLengthLimit = 16;
            if (rtv = bb.Read(this->password)) return rtv;
            return rtv;
        }

#pragma endregion
    };
    // 发消息
    struct Message : xx::MPObject
    {
        typedef Message ThisType;
        typedef xx::MPObject BaseType;
        xx::String* text = nullptr;
#pragma region ctor, interface impls

	    Message()
	    {
	    }
	    Message(xx::BBuffer *bb)
	    {
	        int rtv = 0;
            bb->readLengthLimit = 256;
            if (rtv = bb->Read(text)) throw rtv;
	    }
	    ~Message()
	    {
            mempool().SafeRelease(text);
	    }

        inline virtual void ToString(xx::String &str) const override
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
        inline virtual void ToStringCore(xx::String &str) const override
        {
            this->BaseType::ToStringCore(str);
            str.Append(", \"text\" : ", this->text);
        }


        inline virtual void ToBBuffer(xx::BBuffer &bb) const override
        {
            bb.Write(this->text);
        }

        inline virtual int FromBBuffer(xx::BBuffer &bb) override
        {
            int rtv = 0;
            bb.readLengthLimit = 256;
            if (rtv = bb.Read(this->text)) return rtv;
            return rtv;
        }

#pragma endregion
    };
    // 主动退出
    struct Logout : xx::MPObject
    {
        typedef Logout ThisType;
        typedef xx::MPObject BaseType;
#pragma region ctor, interface impls

	    Logout()
	    {
	    }
	    Logout(xx::BBuffer *bb)
	    {
	    }
	    ~Logout()
	    {
	    }

        inline virtual void ToString(xx::String &str) const override
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
        inline virtual void ToStringCore(xx::String &str) const override
        {
            this->BaseType::ToStringCore(str);
        }


        inline virtual void ToBBuffer(xx::BBuffer &bb) const override
        {
        }

        inline virtual int FromBBuffer(xx::BBuffer &bb) override
        {
            int rtv = 0;
            return rtv;
        }

#pragma endregion
    };
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
	    xx::MemPool::Register<xx::String, xx::MPObject>();
	    xx::MemPool::Register<xx::BBuffer, xx::MPObject>();
	    xx::MemPool::Register<PKG::Request, xx::MPObject>();
	    xx::MemPool::Register<PKG::Response, xx::MPObject>();
	    xx::MemPool::Register<PKG::Property, xx::MPObject>();
	    xx::MemPool::Register<PKG::Property_long, PKG::Property>();
	    xx::MemPool::Register<PKG::Property_double, PKG::Property>();
	    xx::MemPool::Register<PKG::Property_string, PKG::Property>();
	    xx::MemPool::Register<PKG::Properties, PKG::Property>();
	    xx::MemPool::Register<xx::List<PKG::Property*>, xx::MPObject>();
	    xx::MemPool::Register<PKG::UserInfo, xx::MPObject>();
	    xx::MemPool::Register<PKG::Server_Client::JoinSuccess, PKG::Response>();
	    xx::MemPool::Register<xx::List<PKG::UserInfo*>, xx::MPObject>();
	    xx::MemPool::Register<PKG::Server_Client::JoinFail, PKG::Response>();
	    xx::MemPool::Register<PKG::Server_Client::PushJoin, xx::MPObject>();
	    xx::MemPool::Register<PKG::Server_Client::PushMessage, xx::MPObject>();
	    xx::MemPool::Register<PKG::Server_Client::PushLogout, xx::MPObject>();
	    xx::MemPool::Register<PKG::Client_Server::Join, PKG::Request>();
	    xx::MemPool::Register<PKG::Client_Server::Message, xx::MPObject>();
	    xx::MemPool::Register<PKG::Client_Server::Logout, xx::MPObject>();
	}
}
