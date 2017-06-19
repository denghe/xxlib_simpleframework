#include <xx_mempool.h>
#include <xx_bbuffer.h>

namespace PKG
{
namespace Discovery
{
    // 服务启动后用该指令告知 Discovery
    struct Register;
    // 注册结果--成功
    struct RegisterResult_Success;
    // 注册结果--失败: 重复的实例id
    struct RegisterResult_DupeInstanceId;
}
namespace Global
{
    // 服务信息描述
    struct Service;
    // 暂定用于包之附加描述
    struct UserToken;
}
    enum class ServiceTypes : int32_t
    {
        Loader = 0,
        Discovery = 1,
        Navgate = 2,
        Proxy = 3,
        Login = 4,
        Lobby = 5,
        Room = 6,
        DB = 7,
        DBCommit = 8,
    };
namespace Discovery
{
    // 服务启动后用该指令告知 Discovery
    struct Register : xx::MPObject
    {
        typedef xx::MPObject BaseType;
        // 实例id
        uint16_t instanceId = 0;
        // 端口号
        int32_t port = 0;
#pragma region ctor, interface impls

	    Register()
	    {
	    }
	    Register(xx::BBuffer *bb)
	    {
	    }
	    ~Register()
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

            str.Append("{ \"type\" : \"Register\"");
            ToStringCore(str);
            str.Append(" }");
        
        	tsFlags() = 0;
        }
        inline virtual void ToStringCore(xx::String &str) const override
        {
            this->BaseType::ToStringCore(str);
            str.Append(", \"instanceId\" : ", this->instanceId);
            str.Append(", \"port\" : ", this->port);
        }


        inline virtual void ToBBuffer(xx::BBuffer &bb) const override
        {
            bb.Write(this->instanceId);
            bb.Write(this->port);
        }

        inline virtual int FromBBuffer(xx::BBuffer &bb) override
        {
            bb.Read(this->instanceId);
            bb.Read(this->port);
            return 0;
        }

#pragma endregion
    };
    // 注册结果--成功
    struct RegisterResult_Success : xx::MPObject
    {
        typedef xx::MPObject BaseType;
        // 现有服务实例列表
        xx::List<PKG::Global::Service*>* services = nullptr;
#pragma region ctor, interface impls

	    RegisterResult_Success()
	    {
	    }
	    RegisterResult_Success(xx::BBuffer *bb)
	    {
            if (auto rtv = bb->Read(services)) throw rtv;
	    }
	    ~RegisterResult_Success()
	    {
            mempool().SafeRelease(services);
	    }

        inline virtual void ToString(xx::String &str) const override
        {
        	if (tsFlags())
        	{
        		str.Append("[ \"***** recursived *****\" ]");
        		return;
        	}
        	else tsFlags() = 1;

            str.Append("{ \"type\" : \"RegisterResult_Success\"");
            ToStringCore(str);
            str.Append(" }");
        
        	tsFlags() = 0;
        }
        inline virtual void ToStringCore(xx::String &str) const override
        {
            this->BaseType::ToStringCore(str);
            str.Append(", \"services\" : ", this->services);
        }


        inline virtual void ToBBuffer(xx::BBuffer &bb) const override
        {
            bb.Write(this->services);
        }

        inline virtual int FromBBuffer(xx::BBuffer &bb) override
        {
            bb.Read(this->services);
            return 0;
        }

#pragma endregion
    };
    // 注册结果--失败: 重复的实例id
    struct RegisterResult_DupeInstanceId : xx::MPObject
    {
        typedef xx::MPObject BaseType;
#pragma region ctor, interface impls

	    RegisterResult_DupeInstanceId()
	    {
	    }
	    RegisterResult_DupeInstanceId(xx::BBuffer *bb)
	    {
	    }
	    ~RegisterResult_DupeInstanceId()
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

            str.Append("{ \"type\" : \"RegisterResult_DupeInstanceId\"");
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
            return 0;
        }

#pragma endregion
    };
}
namespace Global
{
    // 服务信息描述
    struct Service : xx::MPObject
    {
        typedef xx::MPObject BaseType;
        PKG::ServiceTypes type = (PKG::ServiceTypes)0;
        uint16_t instanceId = 0;
        xx::String* ip = nullptr;
        int32_t port = 0;
#pragma region ctor, interface impls

	    Service()
	    {
	    }
	    Service(xx::BBuffer *bb)
	    {
            if (auto rtv = bb->Read(ip)) throw rtv;
	    }
	    ~Service()
	    {
            mempool().SafeRelease(ip);
	    }

        inline virtual void ToString(xx::String &str) const override
        {
        	if (tsFlags())
        	{
        		str.Append("[ \"***** recursived *****\" ]");
        		return;
        	}
        	else tsFlags() = 1;

            str.Append("{ \"type\" : \"Service\"");
            ToStringCore(str);
            str.Append(" }");
        
        	tsFlags() = 0;
        }
        inline virtual void ToStringCore(xx::String &str) const override
        {
            this->BaseType::ToStringCore(str);
            str.Append(", \"type\" : ", this->type);
            str.Append(", \"instanceId\" : ", this->instanceId);
            str.Append(", \"ip\" : ", this->ip);
            str.Append(", \"port\" : ", this->port);
        }


        inline virtual void ToBBuffer(xx::BBuffer &bb) const override
        {
            bb.Write(this->type);
            bb.Write(this->instanceId);
            bb.Write(this->ip);
            bb.Write(this->port);
        }

        inline virtual int FromBBuffer(xx::BBuffer &bb) override
        {
            bb.Read(this->type);
            bb.Read(this->instanceId);
            bb.Read(this->ip);
            bb.Read(this->port);
            return 0;
        }

#pragma endregion
    };
    // 暂定用于包之附加描述
    struct UserToken : xx::MPObject
    {
        typedef xx::MPObject BaseType;
        uint64_t token = 0;
#pragma region ctor, interface impls

	    UserToken()
	    {
	    }
	    UserToken(xx::BBuffer *bb)
	    {
	    }
	    ~UserToken()
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

            str.Append("{ \"type\" : \"UserToken\"");
            ToStringCore(str);
            str.Append(" }");
        
        	tsFlags() = 0;
        }
        inline virtual void ToStringCore(xx::String &str) const override
        {
            this->BaseType::ToStringCore(str);
            str.Append(", \"token\" : ", this->token);
        }


        inline virtual void ToBBuffer(xx::BBuffer &bb) const override
        {
            bb.Write(this->token);
        }

        inline virtual int FromBBuffer(xx::BBuffer &bb) override
        {
            bb.Read(this->token);
            return 0;
        }

#pragma endregion
    };
}
}
namespace xx
{
	template<> struct TypeId<PKG::Discovery::Register> { static const uint16_t value = 2; };
	template<> struct TypeId<PKG::Discovery::RegisterResult_Success> { static const uint16_t value = 3; };
	template<> struct TypeId<xx::List<PKG::Global::Service*>> { static const uint16_t value = 4; };
	template<> struct TypeId<PKG::Discovery::RegisterResult_DupeInstanceId> { static const uint16_t value = 5; };
	template<> struct TypeId<PKG::Global::Service> { static const uint16_t value = 6; };
	template<> struct TypeId<PKG::Global::UserToken> { static const uint16_t value = 7; };
}
namespace PKG
{
	inline void RegisterTypes()
	{
	    xx::MemPool::Register<PKG::Discovery::Register, xx::MPObject>();
	    xx::MemPool::Register<PKG::Discovery::RegisterResult_Success, xx::MPObject>();
	    xx::MemPool::Register<xx::List<PKG::Global::Service*>, xx::MPObject>();
	    xx::MemPool::Register<PKG::Discovery::RegisterResult_DupeInstanceId, xx::MPObject>();
	    xx::MemPool::Register<PKG::Global::Service, xx::MPObject>();
	    xx::MemPool::Register<PKG::Global::UserToken, xx::MPObject>();
	}
}
