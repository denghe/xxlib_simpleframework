#include <xx_mempool.h>
#include <xx_bbuffer.h>

namespace PKG
{
namespace Client_Lobby
{
    // 进入大厅
    struct Join;
    // 拉取房间列表
    struct PullRooms;
    // 申请进入房间
    struct JoinRoom;
    // 试修改金币( 模拟充值, 输/赢 )
    struct ChangeCoin;
}
namespace Game1_Client
{
    // 游戏1进入成功
    struct JoinSuccess;
}
namespace Client_Game1
{
    // 进入大厅
    struct Join;
    // 主动退出
    struct Quit;
}
namespace Lobby_Client
{
    // 大厅进入成功
    struct JoinSuccess;
    // 拉取房间列表成功: 内含具体的列表
    struct PullRoomsSuccess;
    // 拉取房间列表失败: 内含原因
    struct PullRoomsFail;
    // 申请进入房间成功: 内含连接信息
    struct JoinRoomSuccess;
    // 申请进入房间失败: 内含原因
    struct JoinRoomFail;
    // 金币变化之推送消息 向客户端转发( 不带账号id )
    struct SyncCoin;
    // 试修改金币 成功
    struct ChangeCoinSuccess;
    // 试修改金币 失败
    struct ChangeCoinFail;
}
namespace Game1_Lobby
{
    // 游戏1 返回给 大厅 给玩家分配的 token
    struct JoinSuccess;
    // 游戏1 返回给 大厅 进入失败( 满员?? 无资格?? 内部错误 )
    struct JoinFail;
    // client 并没有在规定时间拿着 token 来连, 或是玩家逻辑断线 / 一局完 / 强行退出
    struct Quit;
}
namespace Lobby_Game1
{
    // 大厅通知游戏有玩家要进来
    struct Join;
}
namespace Lobby_Login
{
    // 大厅 返回给 登录 给玩家分配的 token, 以及玩家接下来需要连的 服务类型 和 地址
    struct JoinSuccess;
    // 大厅 返回给 登录 进入失败( 满员?? )
    struct JoinFail;
}
namespace Login_Lobby
{
    // 登录通知大厅有玩家要进来
    struct Join;
}
namespace Login_Client
{
    // 登录成功, 返回导航地址和 token
    struct LoginSuccess;
    // 登录失败( 含原因 )
    struct LoginFail;
}
namespace DB_Login
{
    // DB 给 Login 返回 Auth 结果
    struct AuthResult;
}
namespace Login_DB
{
    // 登录服务器找 DB 做登录判断
    struct Auth;
}
namespace Client_Login
{
    // 客户端通过用户名和密码登录
    struct Login;
}
namespace Global
{
    // 服务地址信息( 用于各服务器下发导航信息
    struct IPAddress;
    // 一个请求( 当前限定为 service 与 db 间 ), 通常携带一个流水号. 这是基类
    struct Request;
    // 一个回应( 当前限定为 service 与 db 间 ), 通常携带一个请求发过来的流水号. 这是基类
    struct Response;
    // 当前用于 service 连接到 db 时简单描述自己是谁, 连接成功后的首包就需要发送.
    struct ServiceInfo;
    // 用户信息
    struct UserInfo;
    // 游戏小厅信息
    struct RoomInfo;
    // 游戏小厅信息派生: 扎金花
    struct RoomInfo_扎金花;
}
namespace Poker
{
    // 牌
    struct Card;
    // 某玩家手上的牌
    struct Suit;
}
namespace Lobby_DB
{
    // 试修改金币
    struct ChangeCoin;
    // 获取用户信息
    struct GetUserInfo;
}
namespace Game1
{
    // 参与游戏的玩家
    struct Player;
    // 基本单位: 桌
    struct Desk;
}
namespace DB_Lobby
{
    // 试修改金币 成功
    struct ChangeCoinSuccess;
    // 试修改金币 失败
    struct ChangeCoinFail;
    // 金币变化之消息推送( 发生于金币变化后 )
    struct SyncCoin;
    // 获取用户信息成功
    struct GetUserInfoSuccess;
    // 获取用户信息失败
    struct GetUserInfoFail;
}
namespace DB_Login
{
    enum class AuthResultTypes : int32_t
    {
        Success = 0,
        UsernameNotFound = 1,
        WrongPassword = 2,
        Disabled = 3,
    };
}
namespace Global
{
    enum class ServiceTypes : uint8_t
    {
        DB = 0,
        Login = 1,
        Lobby = 2,
        Game1 = 3,
    };
}
namespace Poker
{
    // 花色
    enum class Sets : uint8_t
    {
        // 黑桃
        Spades = 1,
        // 红心
        Hearts = 2,
        // 樱桃/梅花
        Clubs = 3,
        // 方片/方块
        Diamonds = 4,
    };
    // 点数
    enum class Points : uint8_t
    {
        Ace = 1,
        _2 = 2,
        _3 = 3,
        _4 = 4,
        _5 = 5,
        _6 = 6,
        _7 = 7,
        _8 = 8,
        _9 = 9,
        _10 = 10,
        Jack = 11,
        Queen = 12,
        King = 13,
        // 小王/小鬼
        Joker = 14,
        // 大王/大鬼
        JokerEx = 15,
    };
}
namespace Game1
{
    // 游戏阶段列表
    enum class Stages : uint8_t
    {
        // 等人点准备以开始游戏
        WaitReady = 0,
        // 洗牌
        Shuffle = 1,
        // 发牌
        Deal = 2,
        // 下底注
        BaseBet = 3,
        // 看牌
        Watch = 4,
        // 依次操作
        Foreach = 5,
        // 最终比牌
        Match = 6,
        // 局终报告
        Report = 7,
    };
}
namespace Poker
{
    // 牌
    struct Card
    {
        PKG::Poker::Sets set = (PKG::Poker::Sets)0;
        PKG::Poker::Points point = (PKG::Poker::Points)0;
    };
}
namespace Global
{
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
    // 游戏小厅信息
    struct RoomInfo : xx::MPObject
    {
        typedef RoomInfo ThisType;
        typedef xx::MPObject BaseType;
        int32_t id = 0;
#pragma region ctor, interface impls

	    RoomInfo()
	    {
	    }
	    RoomInfo(xx::BBuffer *bb)
	    {
	        int rtv = 0;
            if (rtv = bb->Read(id)) throw rtv;
	    }
	    ~RoomInfo()
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

            str.Append("{ \"type\" : \"RoomInfo\"");
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
}
namespace DB_Login
{
    // DB 给 Login 返回 Auth 结果
    struct AuthResult : PKG::Global::Response
    {
        typedef AuthResult ThisType;
        typedef PKG::Global::Response BaseType;
        PKG::DB_Login::AuthResultTypes type = (PKG::DB_Login::AuthResultTypes)0;
        int32_t accountId = 0;
#pragma region ctor, interface impls

	    AuthResult()
            : BaseType()
	    {
	    }
	    AuthResult(xx::BBuffer *bb)
            : BaseType(bb)
	    {
	        int rtv = 0;
            if (rtv = bb->Read(type)) throw rtv;
            if (rtv = bb->Read(accountId)) throw rtv;
	    }
	    ~AuthResult()
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

            str.Append("{ \"type\" : \"AuthResult\"");
            ToStringCore(str);
            str.Append(" }");
        
        	tsFlags() = 0;
        }
        inline virtual void ToStringCore(xx::String &str) const override
        {
            this->BaseType::ToStringCore(str);
            str.Append(", \"type\" : ", this->type);
            str.Append(", \"accountId\" : ", this->accountId);
        }


        inline virtual void ToBBuffer(xx::BBuffer &bb) const override
        {
            this->BaseType::ToBBuffer(bb);
            bb.Write(this->type);
            bb.Write(this->accountId);
        }

        inline virtual int FromBBuffer(xx::BBuffer &bb) override
        {
            int rtv = 0;
            if (rtv = this->BaseType::FromBBuffer(bb)) return rtv;
            if (rtv = bb.Read(this->type)) return rtv;
            if (rtv = bb.Read(this->accountId)) return rtv;
            return rtv;
        }

#pragma endregion
    };
}
namespace Login_DB
{
    // 登录服务器找 DB 做登录判断
    struct Auth : PKG::Global::Request
    {
        typedef Auth ThisType;
        typedef PKG::Global::Request BaseType;
        xx::String* username = nullptr;
        xx::String* password = nullptr;
#pragma region ctor, interface impls

	    Auth()
            : BaseType()
	    {
	    }
	    Auth(xx::BBuffer *bb)
            : BaseType(bb)
	    {
	        int rtv = 0;
            bb->readLengthLimit = 0;
            if (rtv = bb->Read(username)) throw rtv;
            bb->readLengthLimit = 0;
            if (rtv = bb->Read(password)) throw rtv;
	    }
	    ~Auth()
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

            str.Append("{ \"type\" : \"Auth\"");
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
            bb.readLengthLimit = 0;
            if (rtv = bb.Read(this->username)) return rtv;
            bb.readLengthLimit = 0;
            if (rtv = bb.Read(this->password)) return rtv;
            return rtv;
        }

#pragma endregion
    };
}
namespace Client_Login
{
    // 客户端通过用户名和密码登录
    struct Login : xx::MPObject
    {
        typedef Login ThisType;
        typedef xx::MPObject BaseType;
        xx::String* username = nullptr;
        xx::String* password = nullptr;
#pragma region ctor, interface impls

	    Login()
	    {
	    }
	    Login(xx::BBuffer *bb)
	    {
	        int rtv = 0;
            bb->readLengthLimit = 64;
            if (rtv = bb->Read(username)) throw rtv;
            bb->readLengthLimit = 64;
            if (rtv = bb->Read(password)) throw rtv;
	    }
	    ~Login()
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

            str.Append("{ \"type\" : \"Login\"");
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
            bb.Write(this->username);
            bb.Write(this->password);
        }

        inline virtual int FromBBuffer(xx::BBuffer &bb) override
        {
            int rtv = 0;
            bb.readLengthLimit = 64;
            if (rtv = bb.Read(this->username)) return rtv;
            bb.readLengthLimit = 64;
            if (rtv = bb.Read(this->password)) return rtv;
            return rtv;
        }

#pragma endregion
    };
}
namespace Global
{
    // 服务地址信息( 用于各服务器下发导航信息
    struct IPAddress : xx::MPObject
    {
        typedef IPAddress ThisType;
        typedef xx::MPObject BaseType;
        // IPV4 或 V6 的地址
        xx::String* ip = nullptr;
        // 端口
        int32_t port = 0;
#pragma region ctor, interface impls

	    IPAddress()
	    {
	    }
	    IPAddress(xx::BBuffer *bb)
	    {
	        int rtv = 0;
            bb->readLengthLimit = 64;
            if (rtv = bb->Read(ip)) throw rtv;
            if (rtv = bb->Read(port)) throw rtv;
	    }
	    ~IPAddress()
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

            str.Append("{ \"type\" : \"IPAddress\"");
            ToStringCore(str);
            str.Append(" }");
        
        	tsFlags() = 0;
        }
        inline virtual void ToStringCore(xx::String &str) const override
        {
            this->BaseType::ToStringCore(str);
            str.Append(", \"ip\" : ", this->ip);
            str.Append(", \"port\" : ", this->port);
        }


        inline virtual void ToBBuffer(xx::BBuffer &bb) const override
        {
            bb.Write(this->ip);
            bb.Write(this->port);
        }

        inline virtual int FromBBuffer(xx::BBuffer &bb) override
        {
            int rtv = 0;
            bb.readLengthLimit = 64;
            if (rtv = bb.Read(this->ip)) return rtv;
            if (rtv = bb.Read(this->port)) return rtv;
            return rtv;
        }

#pragma endregion
    };
    // 当前用于 service 连接到 db 时简单描述自己是谁, 连接成功后的首包就需要发送.
    struct ServiceInfo : xx::MPObject
    {
        typedef ServiceInfo ThisType;
        typedef xx::MPObject BaseType;
        PKG::Global::ServiceTypes serviceType = (PKG::Global::ServiceTypes)0;
        // 实例id. 对同一种类型的多个服务实例有意义, 方便做映射
        int32_t instanceId = 0;
#pragma region ctor, interface impls

	    ServiceInfo()
	    {
	    }
	    ServiceInfo(xx::BBuffer *bb)
	    {
	        int rtv = 0;
            if (rtv = bb->Read(serviceType)) throw rtv;
            if (rtv = bb->Read(instanceId)) throw rtv;
	    }
	    ~ServiceInfo()
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

            str.Append("{ \"type\" : \"ServiceInfo\"");
            ToStringCore(str);
            str.Append(" }");
        
        	tsFlags() = 0;
        }
        inline virtual void ToStringCore(xx::String &str) const override
        {
            this->BaseType::ToStringCore(str);
            str.Append(", \"serviceType\" : ", this->serviceType);
            str.Append(", \"instanceId\" : ", this->instanceId);
        }


        inline virtual void ToBBuffer(xx::BBuffer &bb) const override
        {
            bb.Write(this->serviceType);
            bb.Write(this->instanceId);
        }

        inline virtual int FromBBuffer(xx::BBuffer &bb) override
        {
            int rtv = 0;
            if (rtv = bb.Read(this->serviceType)) return rtv;
            if (rtv = bb.Read(this->instanceId)) return rtv;
            return rtv;
        }

#pragma endregion
    };
    // 用户信息
    struct UserInfo : xx::MPObject
    {
        typedef UserInfo ThisType;
        typedef xx::MPObject BaseType;
        int64_t coin = 0;
#pragma region ctor, interface impls

	    UserInfo()
	    {
	    }
	    UserInfo(xx::BBuffer *bb)
	    {
	        int rtv = 0;
            if (rtv = bb->Read(coin)) throw rtv;
	    }
	    ~UserInfo()
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

            str.Append("{ \"type\" : \"UserInfo\"");
            ToStringCore(str);
            str.Append(" }");
        
        	tsFlags() = 0;
        }
        inline virtual void ToStringCore(xx::String &str) const override
        {
            this->BaseType::ToStringCore(str);
            str.Append(", \"coin\" : ", this->coin);
        }


        inline virtual void ToBBuffer(xx::BBuffer &bb) const override
        {
            bb.Write(this->coin);
        }

        inline virtual int FromBBuffer(xx::BBuffer &bb) override
        {
            int rtv = 0;
            if (rtv = bb.Read(this->coin)) return rtv;
            return rtv;
        }

#pragma endregion
    };
}
namespace Login_Client
{
    // 登录失败( 含原因 )
    struct LoginFail : xx::MPObject
    {
        typedef LoginFail ThisType;
        typedef xx::MPObject BaseType;
        int32_t errorCode = 0;
        xx::String* reason = nullptr;
#pragma region ctor, interface impls

	    LoginFail()
	    {
	    }
	    LoginFail(xx::BBuffer *bb)
	    {
	        int rtv = 0;
            if (rtv = bb->Read(errorCode)) throw rtv;
            bb->readLengthLimit = 0;
            if (rtv = bb->Read(reason)) throw rtv;
	    }
	    ~LoginFail()
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

            str.Append("{ \"type\" : \"LoginFail\"");
            ToStringCore(str);
            str.Append(" }");
        
        	tsFlags() = 0;
        }
        inline virtual void ToStringCore(xx::String &str) const override
        {
            this->BaseType::ToStringCore(str);
            str.Append(", \"errorCode\" : ", this->errorCode);
            str.Append(", \"reason\" : ", this->reason);
        }


        inline virtual void ToBBuffer(xx::BBuffer &bb) const override
        {
            bb.Write(this->errorCode);
            bb.Write(this->reason);
        }

        inline virtual int FromBBuffer(xx::BBuffer &bb) override
        {
            int rtv = 0;
            if (rtv = bb.Read(this->errorCode)) return rtv;
            bb.readLengthLimit = 0;
            if (rtv = bb.Read(this->reason)) return rtv;
            return rtv;
        }

#pragma endregion
    };
}
namespace Client_Lobby
{
    // 进入大厅
    struct Join : xx::MPObject
    {
        typedef Join ThisType;
        typedef xx::MPObject BaseType;
        // 账号id
        int32_t accountId = 0;
        // 临时票据
        xx::String* token = nullptr;
#pragma region ctor, interface impls

	    Join()
	    {
	    }
	    Join(xx::BBuffer *bb)
	    {
	        int rtv = 0;
            if (rtv = bb->Read(accountId)) throw rtv;
            bb->readLengthLimit = 64;
            if (rtv = bb->Read(token)) throw rtv;
	    }
	    ~Join()
	    {
            mempool().SafeRelease(token);
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
            str.Append(", \"accountId\" : ", this->accountId);
            str.Append(", \"token\" : ", this->token);
        }


        inline virtual void ToBBuffer(xx::BBuffer &bb) const override
        {
            bb.Write(this->accountId);
            bb.Write(this->token);
        }

        inline virtual int FromBBuffer(xx::BBuffer &bb) override
        {
            int rtv = 0;
            if (rtv = bb.Read(this->accountId)) return rtv;
            bb.readLengthLimit = 64;
            if (rtv = bb.Read(this->token)) return rtv;
            return rtv;
        }

#pragma endregion
    };
}
namespace Poker
{
    // 某玩家手上的牌
    struct Suit : xx::MPObject
    {
        typedef Suit ThisType;
        typedef xx::MPObject BaseType;
        int32_t accountId = 0;
        xx::List<PKG::Poker::Card>* cards = nullptr;
#pragma region ctor, interface impls

	    Suit()
	    {
	    }
	    Suit(xx::BBuffer *bb)
	    {
	        int rtv = 0;
            if (rtv = bb->Read(accountId)) throw rtv;
            bb->readLengthLimit = 0;
            if (rtv = bb->Read(cards)) throw rtv;
	    }
	    ~Suit()
	    {
            mempool().SafeRelease(cards);
	    }

        inline virtual void ToString(xx::String &str) const override
        {
        	if (tsFlags())
        	{
        		str.Append("[ \"***** recursived *****\" ]");
        		return;
        	}
        	else tsFlags() = 1;

            str.Append("{ \"type\" : \"Suit\"");
            ToStringCore(str);
            str.Append(" }");
        
        	tsFlags() = 0;
        }
        inline virtual void ToStringCore(xx::String &str) const override
        {
            this->BaseType::ToStringCore(str);
            str.Append(", \"accountId\" : ", this->accountId);
            str.Append(", \"cards\" : ", this->cards);
        }


        inline virtual void ToBBuffer(xx::BBuffer &bb) const override
        {
            bb.Write(this->accountId);
            bb.Write(this->cards);
        }

        inline virtual int FromBBuffer(xx::BBuffer &bb) override
        {
            int rtv = 0;
            if (rtv = bb.Read(this->accountId)) return rtv;
            bb.readLengthLimit = 0;
            if (rtv = bb.Read(this->cards)) return rtv;
            return rtv;
        }

#pragma endregion
    };
}
namespace Lobby_DB
{
    // 试修改金币
    struct ChangeCoin : PKG::Global::Request
    {
        typedef ChangeCoin ThisType;
        typedef PKG::Global::Request BaseType;
        // 账号id
        int32_t accountId = 0;
        // 原金币值
        int64_t original = 0;
        // 加减值
        int64_t val = 0;
        // 操作原因
        xx::String* reason = nullptr;
#pragma region ctor, interface impls

	    ChangeCoin()
            : BaseType()
	    {
	    }
	    ChangeCoin(xx::BBuffer *bb)
            : BaseType(bb)
	    {
	        int rtv = 0;
            if (rtv = bb->Read(accountId)) throw rtv;
            if (rtv = bb->Read(original)) throw rtv;
            if (rtv = bb->Read(val)) throw rtv;
            bb->readLengthLimit = 250;
            if (rtv = bb->Read(reason)) throw rtv;
	    }
	    ~ChangeCoin()
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

            str.Append("{ \"type\" : \"ChangeCoin\"");
            ToStringCore(str);
            str.Append(" }");
        
        	tsFlags() = 0;
        }
        inline virtual void ToStringCore(xx::String &str) const override
        {
            this->BaseType::ToStringCore(str);
            str.Append(", \"accountId\" : ", this->accountId);
            str.Append(", \"original\" : ", this->original);
            str.Append(", \"val\" : ", this->val);
            str.Append(", \"reason\" : ", this->reason);
        }


        inline virtual void ToBBuffer(xx::BBuffer &bb) const override
        {
            this->BaseType::ToBBuffer(bb);
            bb.Write(this->accountId);
            bb.Write(this->original);
            bb.Write(this->val);
            bb.Write(this->reason);
        }

        inline virtual int FromBBuffer(xx::BBuffer &bb) override
        {
            int rtv = 0;
            if (rtv = this->BaseType::FromBBuffer(bb)) return rtv;
            if (rtv = bb.Read(this->accountId)) return rtv;
            if (rtv = bb.Read(this->original)) return rtv;
            if (rtv = bb.Read(this->val)) return rtv;
            bb.readLengthLimit = 250;
            if (rtv = bb.Read(this->reason)) return rtv;
            return rtv;
        }

#pragma endregion
    };
    // 获取用户信息
    struct GetUserInfo : PKG::Global::Request
    {
        typedef GetUserInfo ThisType;
        typedef PKG::Global::Request BaseType;
        // 账号id
        int32_t accountId = 0;
#pragma region ctor, interface impls

	    GetUserInfo()
            : BaseType()
	    {
	    }
	    GetUserInfo(xx::BBuffer *bb)
            : BaseType(bb)
	    {
	        int rtv = 0;
            if (rtv = bb->Read(accountId)) throw rtv;
	    }
	    ~GetUserInfo()
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

            str.Append("{ \"type\" : \"GetUserInfo\"");
            ToStringCore(str);
            str.Append(" }");
        
        	tsFlags() = 0;
        }
        inline virtual void ToStringCore(xx::String &str) const override
        {
            this->BaseType::ToStringCore(str);
            str.Append(", \"accountId\" : ", this->accountId);
        }


        inline virtual void ToBBuffer(xx::BBuffer &bb) const override
        {
            this->BaseType::ToBBuffer(bb);
            bb.Write(this->accountId);
        }

        inline virtual int FromBBuffer(xx::BBuffer &bb) override
        {
            int rtv = 0;
            if (rtv = this->BaseType::FromBBuffer(bb)) return rtv;
            if (rtv = bb.Read(this->accountId)) return rtv;
            return rtv;
        }

#pragma endregion
    };
}
namespace Game1
{
    // 参与游戏的玩家
    struct Player : xx::MPObject
    {
        typedef Player ThisType;
        typedef xx::MPObject BaseType;
        // 账号id
        int32_t accountId = 0;
        // 临时票据
        xx::String* token = nullptr;
        // 当前金币数
        int64_t coin = 0;
        // 当前位于的桌
        PKG::Game1::Desk* desk = nullptr;
        // 自己位于容器中的下标
        int32_t desk_players_index = 0;
        // 本局游戏是否参与
        bool joined = false;
        // 手牌
        xx::List<PKG::Poker::Card>* cards = nullptr;
        // 是否看过
        bool shown = false;
        // 已下注金额
        int64_t stake = 0;
        // 开牌目标
        int32_t openTarget = 0;
        // 步骤自增编号
        int32_t stepId = 0;
#pragma region ctor, interface impls

	    Player()
	    {
	    }
	    Player(xx::BBuffer *bb)
	    {
	        int rtv = 0;
            if (rtv = bb->Read(accountId)) throw rtv;
            bb->readLengthLimit = 0;
            if (rtv = bb->Read(token)) throw rtv;
            if (rtv = bb->Read(coin)) throw rtv;
            if (rtv = bb->Read(desk)) throw rtv;
            if (rtv = bb->Read(desk_players_index)) throw rtv;
            if (rtv = bb->Read(joined)) throw rtv;
            bb->readLengthLimit = 0;
            if (rtv = bb->Read(cards)) throw rtv;
            if (rtv = bb->Read(shown)) throw rtv;
            if (rtv = bb->Read(stake)) throw rtv;
            if (rtv = bb->Read(openTarget)) throw rtv;
            if (rtv = bb->Read(stepId)) throw rtv;
	    }
	    ~Player()
	    {
            mempool().SafeRelease(token);
            mempool().SafeRelease(desk);
            mempool().SafeRelease(cards);
	    }

        inline virtual void ToString(xx::String &str) const override
        {
        	if (tsFlags())
        	{
        		str.Append("[ \"***** recursived *****\" ]");
        		return;
        	}
        	else tsFlags() = 1;

            str.Append("{ \"type\" : \"Player\"");
            ToStringCore(str);
            str.Append(" }");
        
        	tsFlags() = 0;
        }
        inline virtual void ToStringCore(xx::String &str) const override
        {
            this->BaseType::ToStringCore(str);
            str.Append(", \"accountId\" : ", this->accountId);
            str.Append(", \"token\" : ", this->token);
            str.Append(", \"coin\" : ", this->coin);
            str.Append(", \"desk\" : ", this->desk);
            str.Append(", \"desk_players_index\" : ", this->desk_players_index);
            str.Append(", \"joined\" : ", this->joined);
            str.Append(", \"cards\" : ", this->cards);
            str.Append(", \"shown\" : ", this->shown);
            str.Append(", \"stake\" : ", this->stake);
            str.Append(", \"openTarget\" : ", this->openTarget);
            str.Append(", \"stepId\" : ", this->stepId);
        }


        inline virtual void ToBBuffer(xx::BBuffer &bb) const override
        {
            bb.Write(this->accountId);
            bb.WriteDefaultValue<xx::String*>();
            bb.Write(this->coin);
            bb.Write(this->desk);
            bb.Write(this->desk_players_index);
            bb.Write(this->joined);
            bb.CustomWrite(bb, (void*)this, _offsetof(ThisType, cards));
            bb.Write(this->shown);
            bb.Write(this->stake);
            bb.Write(this->openTarget);
            bb.Write(this->stepId);
        }

        inline virtual int FromBBuffer(xx::BBuffer &bb) override
        {
            int rtv = 0;
            if (rtv = bb.Read(this->accountId)) return rtv;
            bb.readLengthLimit = 0;
            if (rtv = bb.Read(this->token)) return rtv;
            if (rtv = bb.Read(this->coin)) return rtv;
            if (rtv = bb.Read(this->desk)) return rtv;
            if (rtv = bb.Read(this->desk_players_index)) return rtv;
            if (rtv = bb.Read(this->joined)) return rtv;
            bb.readLengthLimit = 0;
            if (rtv = bb.Read(this->cards)) return rtv;
            if (rtv = bb.Read(this->shown)) return rtv;
            if (rtv = bb.Read(this->stake)) return rtv;
            if (rtv = bb.Read(this->openTarget)) return rtv;
            if (rtv = bb.Read(this->stepId)) return rtv;
            return rtv;
        }

#pragma endregion
    };
    // 基本单位: 桌
    struct Desk : xx::MPObject
    {
        typedef Desk ThisType;
        typedef xx::MPObject BaseType;
        // 本桌游戏的流水号()
        int32_t id = 0;
        // 在座的所有玩家( 包括参与者与观众 )
        xx::List<PKG::Game1::Player*>* players = nullptr;
        // 初始 52 张牌
        xx::List<PKG::Poker::Card>* cards = nullptr;
        // 已下注金额
        int64_t totalStake = 0;
#pragma region ctor, interface impls

	    Desk()
	    {
	    }
	    Desk(xx::BBuffer *bb)
	    {
	        int rtv = 0;
            if (rtv = bb->Read(id)) throw rtv;
            bb->readLengthLimit = 0;
            if (rtv = bb->Read(players)) throw rtv;
            bb->readLengthLimit = 0;
            if (rtv = bb->Read(cards)) throw rtv;
            if (rtv = bb->Read(totalStake)) throw rtv;
	    }
	    ~Desk()
	    {
            mempool().SafeRelease(players);
            mempool().SafeRelease(cards);
	    }

        inline virtual void ToString(xx::String &str) const override
        {
        	if (tsFlags())
        	{
        		str.Append("[ \"***** recursived *****\" ]");
        		return;
        	}
        	else tsFlags() = 1;

            str.Append("{ \"type\" : \"Desk\"");
            ToStringCore(str);
            str.Append(" }");
        
        	tsFlags() = 0;
        }
        inline virtual void ToStringCore(xx::String &str) const override
        {
            this->BaseType::ToStringCore(str);
            str.Append(", \"id\" : ", this->id);
            str.Append(", \"players\" : ", this->players);
            str.Append(", \"cards\" : ", this->cards);
            str.Append(", \"totalStake\" : ", this->totalStake);
        }


        inline virtual void ToBBuffer(xx::BBuffer &bb) const override
        {
            bb.Write(this->id);
            bb.Write(this->players);
            bb.WriteDefaultValue<xx::List<PKG::Poker::Card>*>();
            bb.Write(this->totalStake);
        }

        inline virtual int FromBBuffer(xx::BBuffer &bb) override
        {
            int rtv = 0;
            if (rtv = bb.Read(this->id)) return rtv;
            bb.readLengthLimit = 0;
            if (rtv = bb.Read(this->players)) return rtv;
            bb.readLengthLimit = 0;
            if (rtv = bb.Read(this->cards)) return rtv;
            if (rtv = bb.Read(this->totalStake)) return rtv;
            return rtv;
        }

#pragma endregion
    };
}
namespace DB_Lobby
{
    // 试修改金币 成功
    struct ChangeCoinSuccess : PKG::Global::Response
    {
        typedef ChangeCoinSuccess ThisType;
        typedef PKG::Global::Response BaseType;
#pragma region ctor, interface impls

	    ChangeCoinSuccess()
            : BaseType()
	    {
	    }
	    ChangeCoinSuccess(xx::BBuffer *bb)
            : BaseType(bb)
	    {
	    }
	    ~ChangeCoinSuccess()
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

            str.Append("{ \"type\" : \"ChangeCoinSuccess\"");
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
            this->BaseType::ToBBuffer(bb);
        }

        inline virtual int FromBBuffer(xx::BBuffer &bb) override
        {
            int rtv = 0;
            if (rtv = this->BaseType::FromBBuffer(bb)) return rtv;
            return rtv;
        }

#pragma endregion
    };
    // 试修改金币 失败
    struct ChangeCoinFail : PKG::Global::Response
    {
        typedef ChangeCoinFail ThisType;
        typedef PKG::Global::Response BaseType;
        // 失败原因
        xx::String* reason = nullptr;
#pragma region ctor, interface impls

	    ChangeCoinFail()
            : BaseType()
	    {
	    }
	    ChangeCoinFail(xx::BBuffer *bb)
            : BaseType(bb)
	    {
	        int rtv = 0;
            bb->readLengthLimit = 0;
            if (rtv = bb->Read(reason)) throw rtv;
	    }
	    ~ChangeCoinFail()
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

            str.Append("{ \"type\" : \"ChangeCoinFail\"");
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
    // 金币变化之消息推送( 发生于金币变化后 )
    struct SyncCoin : xx::MPObject
    {
        typedef SyncCoin ThisType;
        typedef xx::MPObject BaseType;
        // 版本号( 如果从多个渠道收到通知则保留该值最大的 )
        int32_t versionNumber = 0;
        // 账号id
        int32_t accountId = 0;
        // 原金币值
        int64_t original = 0;
        // 加减值
        int64_t val = 0;
        // 当前金币值
        int64_t curr = 0;
        // 操作原因
        xx::String* reason = nullptr;
#pragma region ctor, interface impls

	    SyncCoin()
	    {
	    }
	    SyncCoin(xx::BBuffer *bb)
	    {
	        int rtv = 0;
            if (rtv = bb->Read(versionNumber)) throw rtv;
            if (rtv = bb->Read(accountId)) throw rtv;
            if (rtv = bb->Read(original)) throw rtv;
            if (rtv = bb->Read(val)) throw rtv;
            if (rtv = bb->Read(curr)) throw rtv;
            bb->readLengthLimit = 0;
            if (rtv = bb->Read(reason)) throw rtv;
	    }
	    ~SyncCoin()
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

            str.Append("{ \"type\" : \"SyncCoin\"");
            ToStringCore(str);
            str.Append(" }");
        
        	tsFlags() = 0;
        }
        inline virtual void ToStringCore(xx::String &str) const override
        {
            this->BaseType::ToStringCore(str);
            str.Append(", \"versionNumber\" : ", this->versionNumber);
            str.Append(", \"accountId\" : ", this->accountId);
            str.Append(", \"original\" : ", this->original);
            str.Append(", \"val\" : ", this->val);
            str.Append(", \"curr\" : ", this->curr);
            str.Append(", \"reason\" : ", this->reason);
        }


        inline virtual void ToBBuffer(xx::BBuffer &bb) const override
        {
            bb.Write(this->versionNumber);
            bb.Write(this->accountId);
            bb.Write(this->original);
            bb.Write(this->val);
            bb.Write(this->curr);
            bb.Write(this->reason);
        }

        inline virtual int FromBBuffer(xx::BBuffer &bb) override
        {
            int rtv = 0;
            if (rtv = bb.Read(this->versionNumber)) return rtv;
            if (rtv = bb.Read(this->accountId)) return rtv;
            if (rtv = bb.Read(this->original)) return rtv;
            if (rtv = bb.Read(this->val)) return rtv;
            if (rtv = bb.Read(this->curr)) return rtv;
            bb.readLengthLimit = 0;
            if (rtv = bb.Read(this->reason)) return rtv;
            return rtv;
        }

#pragma endregion
    };
}
namespace Global
{
    // 游戏小厅信息派生: 扎金花
    struct RoomInfo_扎金花 : PKG::Global::RoomInfo
    {
        typedef RoomInfo_扎金花 ThisType;
        typedef PKG::Global::RoomInfo BaseType;
        int64_t 进场基础金币要求 = 0;
        int64_t 底注多少金币 = 0;
        int64_t 封顶加注额度 = 0;
        int32_t 几轮后可直接比牌 = 0;
        int32_t 最多加注几轮 = 0;
#pragma region ctor, interface impls

	    RoomInfo_扎金花()
            : BaseType()
	    {
	    }
	    RoomInfo_扎金花(xx::BBuffer *bb)
            : BaseType(bb)
	    {
	        int rtv = 0;
            if (rtv = bb->Read(进场基础金币要求)) throw rtv;
            if (rtv = bb->Read(底注多少金币)) throw rtv;
            if (rtv = bb->Read(封顶加注额度)) throw rtv;
            if (rtv = bb->Read(几轮后可直接比牌)) throw rtv;
            if (rtv = bb->Read(最多加注几轮)) throw rtv;
	    }
	    ~RoomInfo_扎金花()
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

            str.Append("{ \"type\" : \"RoomInfo_扎金花\"");
            ToStringCore(str);
            str.Append(" }");
        
        	tsFlags() = 0;
        }
        inline virtual void ToStringCore(xx::String &str) const override
        {
            this->BaseType::ToStringCore(str);
            str.Append(", \"进场基础金币要求\" : ", this->进场基础金币要求);
            str.Append(", \"底注多少金币\" : ", this->底注多少金币);
            str.Append(", \"封顶加注额度\" : ", this->封顶加注额度);
            str.Append(", \"几轮后可直接比牌\" : ", this->几轮后可直接比牌);
            str.Append(", \"最多加注几轮\" : ", this->最多加注几轮);
        }


        inline virtual void ToBBuffer(xx::BBuffer &bb) const override
        {
            this->BaseType::ToBBuffer(bb);
            bb.Write(this->进场基础金币要求);
            bb.Write(this->底注多少金币);
            bb.Write(this->封顶加注额度);
            bb.Write(this->几轮后可直接比牌);
            bb.Write(this->最多加注几轮);
        }

        inline virtual int FromBBuffer(xx::BBuffer &bb) override
        {
            int rtv = 0;
            if (rtv = this->BaseType::FromBBuffer(bb)) return rtv;
            if (rtv = bb.Read(this->进场基础金币要求)) return rtv;
            if (rtv = bb.Read(this->底注多少金币)) return rtv;
            if (rtv = bb.Read(this->封顶加注额度)) return rtv;
            if (rtv = bb.Read(this->几轮后可直接比牌)) return rtv;
            if (rtv = bb.Read(this->最多加注几轮)) return rtv;
            return rtv;
        }

#pragma endregion
    };
}
namespace Login_Client
{
    // 登录成功, 返回导航地址和 token
    struct LoginSuccess : xx::MPObject
    {
        typedef LoginSuccess ThisType;
        typedef xx::MPObject BaseType;
        int32_t accountId = 0;
        xx::String* token = nullptr;
        PKG::Global::IPAddress* serviceAddress = nullptr;
        // 目标地址可能是大厅也可能是游戏
        PKG::Global::ServiceTypes serviceType = (PKG::Global::ServiceTypes)0;
#pragma region ctor, interface impls

	    LoginSuccess()
	    {
	    }
	    LoginSuccess(xx::BBuffer *bb)
	    {
	        int rtv = 0;
            if (rtv = bb->Read(accountId)) throw rtv;
            bb->readLengthLimit = 0;
            if (rtv = bb->Read(token)) throw rtv;
            if (rtv = bb->Read(serviceAddress)) throw rtv;
            if (rtv = bb->Read(serviceType)) throw rtv;
	    }
	    ~LoginSuccess()
	    {
            mempool().SafeRelease(token);
            mempool().SafeRelease(serviceAddress);
	    }

        inline virtual void ToString(xx::String &str) const override
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
        inline virtual void ToStringCore(xx::String &str) const override
        {
            this->BaseType::ToStringCore(str);
            str.Append(", \"accountId\" : ", this->accountId);
            str.Append(", \"token\" : ", this->token);
            str.Append(", \"serviceAddress\" : ", this->serviceAddress);
            str.Append(", \"serviceType\" : ", this->serviceType);
        }


        inline virtual void ToBBuffer(xx::BBuffer &bb) const override
        {
            bb.Write(this->accountId);
            bb.Write(this->token);
            bb.Write(this->serviceAddress);
            bb.Write(this->serviceType);
        }

        inline virtual int FromBBuffer(xx::BBuffer &bb) override
        {
            int rtv = 0;
            if (rtv = bb.Read(this->accountId)) return rtv;
            bb.readLengthLimit = 0;
            if (rtv = bb.Read(this->token)) return rtv;
            if (rtv = bb.Read(this->serviceAddress)) return rtv;
            if (rtv = bb.Read(this->serviceType)) return rtv;
            return rtv;
        }

#pragma endregion
    };
}
namespace Login_Lobby
{
    // 登录通知大厅有玩家要进来
    struct Join : PKG::Global::Request
    {
        typedef Join ThisType;
        typedef PKG::Global::Request BaseType;
        int32_t accountId = 0;
#pragma region ctor, interface impls

	    Join()
            : BaseType()
	    {
	    }
	    Join(xx::BBuffer *bb)
            : BaseType(bb)
	    {
	        int rtv = 0;
            if (rtv = bb->Read(accountId)) throw rtv;
	    }
	    ~Join()
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

            str.Append("{ \"type\" : \"Join\"");
            ToStringCore(str);
            str.Append(" }");
        
        	tsFlags() = 0;
        }
        inline virtual void ToStringCore(xx::String &str) const override
        {
            this->BaseType::ToStringCore(str);
            str.Append(", \"accountId\" : ", this->accountId);
        }


        inline virtual void ToBBuffer(xx::BBuffer &bb) const override
        {
            this->BaseType::ToBBuffer(bb);
            bb.Write(this->accountId);
        }

        inline virtual int FromBBuffer(xx::BBuffer &bb) override
        {
            int rtv = 0;
            if (rtv = this->BaseType::FromBBuffer(bb)) return rtv;
            if (rtv = bb.Read(this->accountId)) return rtv;
            return rtv;
        }

#pragma endregion
    };
}
namespace Lobby_Login
{
    // 大厅 返回给 登录 进入失败( 满员?? )
    struct JoinFail : PKG::Global::Response
    {
        typedef JoinFail ThisType;
        typedef PKG::Global::Response BaseType;
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
}
namespace Client_Lobby
{
    // 拉取房间列表
    struct PullRooms : xx::MPObject
    {
        typedef PullRooms ThisType;
        typedef xx::MPObject BaseType;
#pragma region ctor, interface impls

	    PullRooms()
	    {
	    }
	    PullRooms(xx::BBuffer *bb)
	    {
	    }
	    ~PullRooms()
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

            str.Append("{ \"type\" : \"PullRooms\"");
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
    // 申请进入房间
    struct JoinRoom : xx::MPObject
    {
        typedef JoinRoom ThisType;
        typedef xx::MPObject BaseType;
        int32_t roomId = 0;
#pragma region ctor, interface impls

	    JoinRoom()
	    {
	    }
	    JoinRoom(xx::BBuffer *bb)
	    {
	        int rtv = 0;
            if (rtv = bb->Read(roomId)) throw rtv;
	    }
	    ~JoinRoom()
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

            str.Append("{ \"type\" : \"JoinRoom\"");
            ToStringCore(str);
            str.Append(" }");
        
        	tsFlags() = 0;
        }
        inline virtual void ToStringCore(xx::String &str) const override
        {
            this->BaseType::ToStringCore(str);
            str.Append(", \"roomId\" : ", this->roomId);
        }


        inline virtual void ToBBuffer(xx::BBuffer &bb) const override
        {
            bb.Write(this->roomId);
        }

        inline virtual int FromBBuffer(xx::BBuffer &bb) override
        {
            int rtv = 0;
            if (rtv = bb.Read(this->roomId)) return rtv;
            return rtv;
        }

#pragma endregion
    };
    // 试修改金币( 模拟充值, 输/赢 )
    struct ChangeCoin : xx::MPObject
    {
        typedef ChangeCoin ThisType;
        typedef xx::MPObject BaseType;
        // 原金币值
        int64_t original = 0;
        // 加减值
        int64_t val = 0;
        // 操作原因
        xx::String* reason = nullptr;
#pragma region ctor, interface impls

	    ChangeCoin()
	    {
	    }
	    ChangeCoin(xx::BBuffer *bb)
	    {
	        int rtv = 0;
            if (rtv = bb->Read(original)) throw rtv;
            if (rtv = bb->Read(val)) throw rtv;
            bb->readLengthLimit = 250;
            if (rtv = bb->Read(reason)) throw rtv;
	    }
	    ~ChangeCoin()
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

            str.Append("{ \"type\" : \"ChangeCoin\"");
            ToStringCore(str);
            str.Append(" }");
        
        	tsFlags() = 0;
        }
        inline virtual void ToStringCore(xx::String &str) const override
        {
            this->BaseType::ToStringCore(str);
            str.Append(", \"original\" : ", this->original);
            str.Append(", \"val\" : ", this->val);
            str.Append(", \"reason\" : ", this->reason);
        }


        inline virtual void ToBBuffer(xx::BBuffer &bb) const override
        {
            bb.Write(this->original);
            bb.Write(this->val);
            bb.Write(this->reason);
        }

        inline virtual int FromBBuffer(xx::BBuffer &bb) override
        {
            int rtv = 0;
            if (rtv = bb.Read(this->original)) return rtv;
            if (rtv = bb.Read(this->val)) return rtv;
            bb.readLengthLimit = 250;
            if (rtv = bb.Read(this->reason)) return rtv;
            return rtv;
        }

#pragma endregion
    };
}
namespace Game1_Client
{
    // 游戏1进入成功
    struct JoinSuccess : xx::MPObject
    {
        typedef JoinSuccess ThisType;
        typedef xx::MPObject BaseType;
#pragma region ctor, interface impls

	    JoinSuccess()
	    {
	    }
	    JoinSuccess(xx::BBuffer *bb)
	    {
	    }
	    ~JoinSuccess()
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

            str.Append("{ \"type\" : \"JoinSuccess\"");
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
namespace Client_Game1
{
    // 进入大厅
    struct Join : xx::MPObject
    {
        typedef Join ThisType;
        typedef xx::MPObject BaseType;
        // 账号id
        int32_t accountId = 0;
        // 临时票据
        xx::String* token = nullptr;
#pragma region ctor, interface impls

	    Join()
	    {
	    }
	    Join(xx::BBuffer *bb)
	    {
	        int rtv = 0;
            if (rtv = bb->Read(accountId)) throw rtv;
            bb->readLengthLimit = 64;
            if (rtv = bb->Read(token)) throw rtv;
	    }
	    ~Join()
	    {
            mempool().SafeRelease(token);
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
            str.Append(", \"accountId\" : ", this->accountId);
            str.Append(", \"token\" : ", this->token);
        }


        inline virtual void ToBBuffer(xx::BBuffer &bb) const override
        {
            bb.Write(this->accountId);
            bb.Write(this->token);
        }

        inline virtual int FromBBuffer(xx::BBuffer &bb) override
        {
            int rtv = 0;
            if (rtv = bb.Read(this->accountId)) return rtv;
            bb.readLengthLimit = 64;
            if (rtv = bb.Read(this->token)) return rtv;
            return rtv;
        }

#pragma endregion
    };
    // 主动退出
    struct Quit : xx::MPObject
    {
        typedef Quit ThisType;
        typedef xx::MPObject BaseType;
#pragma region ctor, interface impls

	    Quit()
	    {
	    }
	    Quit(xx::BBuffer *bb)
	    {
	    }
	    ~Quit()
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

            str.Append("{ \"type\" : \"Quit\"");
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
namespace Lobby_Client
{
    // 大厅进入成功
    struct JoinSuccess : xx::MPObject
    {
        typedef JoinSuccess ThisType;
        typedef xx::MPObject BaseType;
        // 当前金币数
        int64_t coin = 0;
#pragma region ctor, interface impls

	    JoinSuccess()
	    {
	    }
	    JoinSuccess(xx::BBuffer *bb)
	    {
	        int rtv = 0;
            if (rtv = bb->Read(coin)) throw rtv;
	    }
	    ~JoinSuccess()
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

            str.Append("{ \"type\" : \"JoinSuccess\"");
            ToStringCore(str);
            str.Append(" }");
        
        	tsFlags() = 0;
        }
        inline virtual void ToStringCore(xx::String &str) const override
        {
            this->BaseType::ToStringCore(str);
            str.Append(", \"coin\" : ", this->coin);
        }


        inline virtual void ToBBuffer(xx::BBuffer &bb) const override
        {
            bb.Write(this->coin);
        }

        inline virtual int FromBBuffer(xx::BBuffer &bb) override
        {
            int rtv = 0;
            if (rtv = bb.Read(this->coin)) return rtv;
            return rtv;
        }

#pragma endregion
    };
    // 拉取房间列表成功: 内含具体的列表
    struct PullRoomsSuccess : xx::MPObject
    {
        typedef PullRoomsSuccess ThisType;
        typedef xx::MPObject BaseType;
        xx::List<PKG::Global::RoomInfo*>* rooms = nullptr;
#pragma region ctor, interface impls

	    PullRoomsSuccess()
	    {
	    }
	    PullRoomsSuccess(xx::BBuffer *bb)
	    {
	        int rtv = 0;
            bb->readLengthLimit = 0;
            if (rtv = bb->Read(rooms)) throw rtv;
	    }
	    ~PullRoomsSuccess()
	    {
            mempool().SafeRelease(rooms);
	    }

        inline virtual void ToString(xx::String &str) const override
        {
        	if (tsFlags())
        	{
        		str.Append("[ \"***** recursived *****\" ]");
        		return;
        	}
        	else tsFlags() = 1;

            str.Append("{ \"type\" : \"PullRoomsSuccess\"");
            ToStringCore(str);
            str.Append(" }");
        
        	tsFlags() = 0;
        }
        inline virtual void ToStringCore(xx::String &str) const override
        {
            this->BaseType::ToStringCore(str);
            str.Append(", \"rooms\" : ", this->rooms);
        }


        inline virtual void ToBBuffer(xx::BBuffer &bb) const override
        {
            bb.Write(this->rooms);
        }

        inline virtual int FromBBuffer(xx::BBuffer &bb) override
        {
            int rtv = 0;
            bb.readLengthLimit = 0;
            if (rtv = bb.Read(this->rooms)) return rtv;
            return rtv;
        }

#pragma endregion
    };
    // 拉取房间列表失败: 内含原因
    struct PullRoomsFail : xx::MPObject
    {
        typedef PullRoomsFail ThisType;
        typedef xx::MPObject BaseType;
        xx::String* reason = nullptr;
#pragma region ctor, interface impls

	    PullRoomsFail()
	    {
	    }
	    PullRoomsFail(xx::BBuffer *bb)
	    {
	        int rtv = 0;
            bb->readLengthLimit = 0;
            if (rtv = bb->Read(reason)) throw rtv;
	    }
	    ~PullRoomsFail()
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

            str.Append("{ \"type\" : \"PullRoomsFail\"");
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
            bb.Write(this->reason);
        }

        inline virtual int FromBBuffer(xx::BBuffer &bb) override
        {
            int rtv = 0;
            bb.readLengthLimit = 0;
            if (rtv = bb.Read(this->reason)) return rtv;
            return rtv;
        }

#pragma endregion
    };
    // 申请进入房间成功: 内含连接信息
    struct JoinRoomSuccess : xx::MPObject
    {
        typedef JoinRoomSuccess ThisType;
        typedef xx::MPObject BaseType;
        xx::String* token = nullptr;
        PKG::Global::IPAddress* serviceAddress = nullptr;
#pragma region ctor, interface impls

	    JoinRoomSuccess()
	    {
	    }
	    JoinRoomSuccess(xx::BBuffer *bb)
	    {
	        int rtv = 0;
            bb->readLengthLimit = 0;
            if (rtv = bb->Read(token)) throw rtv;
            if (rtv = bb->Read(serviceAddress)) throw rtv;
	    }
	    ~JoinRoomSuccess()
	    {
            mempool().SafeRelease(token);
            mempool().SafeRelease(serviceAddress);
	    }

        inline virtual void ToString(xx::String &str) const override
        {
        	if (tsFlags())
        	{
        		str.Append("[ \"***** recursived *****\" ]");
        		return;
        	}
        	else tsFlags() = 1;

            str.Append("{ \"type\" : \"JoinRoomSuccess\"");
            ToStringCore(str);
            str.Append(" }");
        
        	tsFlags() = 0;
        }
        inline virtual void ToStringCore(xx::String &str) const override
        {
            this->BaseType::ToStringCore(str);
            str.Append(", \"token\" : ", this->token);
            str.Append(", \"serviceAddress\" : ", this->serviceAddress);
        }


        inline virtual void ToBBuffer(xx::BBuffer &bb) const override
        {
            bb.Write(this->token);
            bb.Write(this->serviceAddress);
        }

        inline virtual int FromBBuffer(xx::BBuffer &bb) override
        {
            int rtv = 0;
            bb.readLengthLimit = 0;
            if (rtv = bb.Read(this->token)) return rtv;
            if (rtv = bb.Read(this->serviceAddress)) return rtv;
            return rtv;
        }

#pragma endregion
    };
    // 申请进入房间失败: 内含原因
    struct JoinRoomFail : xx::MPObject
    {
        typedef JoinRoomFail ThisType;
        typedef xx::MPObject BaseType;
        xx::String* reason = nullptr;
#pragma region ctor, interface impls

	    JoinRoomFail()
	    {
	    }
	    JoinRoomFail(xx::BBuffer *bb)
	    {
	        int rtv = 0;
            bb->readLengthLimit = 0;
            if (rtv = bb->Read(reason)) throw rtv;
	    }
	    ~JoinRoomFail()
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

            str.Append("{ \"type\" : \"JoinRoomFail\"");
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
            bb.Write(this->reason);
        }

        inline virtual int FromBBuffer(xx::BBuffer &bb) override
        {
            int rtv = 0;
            bb.readLengthLimit = 0;
            if (rtv = bb.Read(this->reason)) return rtv;
            return rtv;
        }

#pragma endregion
    };
    // 金币变化之推送消息 向客户端转发( 不带账号id )
    struct SyncCoin : xx::MPObject
    {
        typedef SyncCoin ThisType;
        typedef xx::MPObject BaseType;
        // 版本号( 如果从多个渠道收到通知则保留该值最大的 )
        int32_t versionNumber = 0;
        // 原金币值
        int64_t original = 0;
        // 加减值
        int64_t val = 0;
        // 当前金币值
        int64_t curr = 0;
        // 操作原因
        xx::String* reason = nullptr;
#pragma region ctor, interface impls

	    SyncCoin()
	    {
	    }
	    SyncCoin(xx::BBuffer *bb)
	    {
	        int rtv = 0;
            if (rtv = bb->Read(versionNumber)) throw rtv;
            if (rtv = bb->Read(original)) throw rtv;
            if (rtv = bb->Read(val)) throw rtv;
            if (rtv = bb->Read(curr)) throw rtv;
            bb->readLengthLimit = 0;
            if (rtv = bb->Read(reason)) throw rtv;
	    }
	    ~SyncCoin()
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

            str.Append("{ \"type\" : \"SyncCoin\"");
            ToStringCore(str);
            str.Append(" }");
        
        	tsFlags() = 0;
        }
        inline virtual void ToStringCore(xx::String &str) const override
        {
            this->BaseType::ToStringCore(str);
            str.Append(", \"versionNumber\" : ", this->versionNumber);
            str.Append(", \"original\" : ", this->original);
            str.Append(", \"val\" : ", this->val);
            str.Append(", \"curr\" : ", this->curr);
            str.Append(", \"reason\" : ", this->reason);
        }


        inline virtual void ToBBuffer(xx::BBuffer &bb) const override
        {
            bb.Write(this->versionNumber);
            bb.Write(this->original);
            bb.Write(this->val);
            bb.Write(this->curr);
            bb.Write(this->reason);
        }

        inline virtual int FromBBuffer(xx::BBuffer &bb) override
        {
            int rtv = 0;
            if (rtv = bb.Read(this->versionNumber)) return rtv;
            if (rtv = bb.Read(this->original)) return rtv;
            if (rtv = bb.Read(this->val)) return rtv;
            if (rtv = bb.Read(this->curr)) return rtv;
            bb.readLengthLimit = 0;
            if (rtv = bb.Read(this->reason)) return rtv;
            return rtv;
        }

#pragma endregion
    };
    // 试修改金币 成功
    struct ChangeCoinSuccess : xx::MPObject
    {
        typedef ChangeCoinSuccess ThisType;
        typedef xx::MPObject BaseType;
#pragma region ctor, interface impls

	    ChangeCoinSuccess()
	    {
	    }
	    ChangeCoinSuccess(xx::BBuffer *bb)
	    {
	    }
	    ~ChangeCoinSuccess()
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

            str.Append("{ \"type\" : \"ChangeCoinSuccess\"");
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
    // 试修改金币 失败
    struct ChangeCoinFail : xx::MPObject
    {
        typedef ChangeCoinFail ThisType;
        typedef xx::MPObject BaseType;
        xx::String* reason = nullptr;
#pragma region ctor, interface impls

	    ChangeCoinFail()
	    {
	    }
	    ChangeCoinFail(xx::BBuffer *bb)
	    {
	        int rtv = 0;
            bb->readLengthLimit = 0;
            if (rtv = bb->Read(reason)) throw rtv;
	    }
	    ~ChangeCoinFail()
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

            str.Append("{ \"type\" : \"ChangeCoinFail\"");
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
            bb.Write(this->reason);
        }

        inline virtual int FromBBuffer(xx::BBuffer &bb) override
        {
            int rtv = 0;
            bb.readLengthLimit = 0;
            if (rtv = bb.Read(this->reason)) return rtv;
            return rtv;
        }

#pragma endregion
    };
}
namespace Game1_Lobby
{
    // 游戏1 返回给 大厅 给玩家分配的 token
    struct JoinSuccess : PKG::Global::Response
    {
        typedef JoinSuccess ThisType;
        typedef PKG::Global::Response BaseType;
        // 该 token 由 Game1 生成, 下发给 client, 并带超时时间. 在规定时间内, 如果 client 没拿着 token 连上来, 则过期, 重新通知大厅 缺人, 等大厅裁决
        xx::String* token = nullptr;
#pragma region ctor, interface impls

	    JoinSuccess()
            : BaseType()
	    {
	    }
	    JoinSuccess(xx::BBuffer *bb)
            : BaseType(bb)
	    {
	        int rtv = 0;
            bb->readLengthLimit = 0;
            if (rtv = bb->Read(token)) throw rtv;
	    }
	    ~JoinSuccess()
	    {
            mempool().SafeRelease(token);
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
            str.Append(", \"token\" : ", this->token);
        }


        inline virtual void ToBBuffer(xx::BBuffer &bb) const override
        {
            this->BaseType::ToBBuffer(bb);
            bb.Write(this->token);
        }

        inline virtual int FromBBuffer(xx::BBuffer &bb) override
        {
            int rtv = 0;
            if (rtv = this->BaseType::FromBBuffer(bb)) return rtv;
            bb.readLengthLimit = 0;
            if (rtv = bb.Read(this->token)) return rtv;
            return rtv;
        }

#pragma endregion
    };
    // 游戏1 返回给 大厅 进入失败( 满员?? 无资格?? 内部错误 )
    struct JoinFail : PKG::Global::Response
    {
        typedef JoinFail ThisType;
        typedef PKG::Global::Response BaseType;
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
    // client 并没有在规定时间拿着 token 来连, 或是玩家逻辑断线 / 一局完 / 强行退出
    struct Quit : xx::MPObject
    {
        typedef Quit ThisType;
        typedef xx::MPObject BaseType;
        int32_t accountId = 0;
        xx::String* reason = nullptr;
#pragma region ctor, interface impls

	    Quit()
	    {
	    }
	    Quit(xx::BBuffer *bb)
	    {
	        int rtv = 0;
            if (rtv = bb->Read(accountId)) throw rtv;
            bb->readLengthLimit = 0;
            if (rtv = bb->Read(reason)) throw rtv;
	    }
	    ~Quit()
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

            str.Append("{ \"type\" : \"Quit\"");
            ToStringCore(str);
            str.Append(" }");
        
        	tsFlags() = 0;
        }
        inline virtual void ToStringCore(xx::String &str) const override
        {
            this->BaseType::ToStringCore(str);
            str.Append(", \"accountId\" : ", this->accountId);
            str.Append(", \"reason\" : ", this->reason);
        }


        inline virtual void ToBBuffer(xx::BBuffer &bb) const override
        {
            bb.Write(this->accountId);
            bb.Write(this->reason);
        }

        inline virtual int FromBBuffer(xx::BBuffer &bb) override
        {
            int rtv = 0;
            if (rtv = bb.Read(this->accountId)) return rtv;
            bb.readLengthLimit = 0;
            if (rtv = bb.Read(this->reason)) return rtv;
            return rtv;
        }

#pragma endregion
    };
}
namespace Lobby_Game1
{
    // 大厅通知游戏有玩家要进来
    struct Join : PKG::Global::Request
    {
        typedef Join ThisType;
        typedef PKG::Global::Request BaseType;
        int32_t accountId = 0;
#pragma region ctor, interface impls

	    Join()
            : BaseType()
	    {
	    }
	    Join(xx::BBuffer *bb)
            : BaseType(bb)
	    {
	        int rtv = 0;
            if (rtv = bb->Read(accountId)) throw rtv;
	    }
	    ~Join()
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

            str.Append("{ \"type\" : \"Join\"");
            ToStringCore(str);
            str.Append(" }");
        
        	tsFlags() = 0;
        }
        inline virtual void ToStringCore(xx::String &str) const override
        {
            this->BaseType::ToStringCore(str);
            str.Append(", \"accountId\" : ", this->accountId);
        }


        inline virtual void ToBBuffer(xx::BBuffer &bb) const override
        {
            this->BaseType::ToBBuffer(bb);
            bb.Write(this->accountId);
        }

        inline virtual int FromBBuffer(xx::BBuffer &bb) override
        {
            int rtv = 0;
            if (rtv = this->BaseType::FromBBuffer(bb)) return rtv;
            if (rtv = bb.Read(this->accountId)) return rtv;
            return rtv;
        }

#pragma endregion
    };
}
namespace Lobby_Login
{
    // 大厅 返回给 登录 给玩家分配的 token, 以及玩家接下来需要连的 服务类型 和 地址
    struct JoinSuccess : PKG::Global::Response
    {
        typedef JoinSuccess ThisType;
        typedef PKG::Global::Response BaseType;
        int32_t accountId = 0;
        xx::String* token = nullptr;
        // 如果是断线重连, 则返回玩家当前所在服务的类型. 否则返回 Lobby
        PKG::Global::ServiceTypes serviceType = (PKG::Global::ServiceTypes)0;
        PKG::Global::IPAddress* serviceAddress = nullptr;
#pragma region ctor, interface impls

	    JoinSuccess()
            : BaseType()
	    {
	    }
	    JoinSuccess(xx::BBuffer *bb)
            : BaseType(bb)
	    {
	        int rtv = 0;
            if (rtv = bb->Read(accountId)) throw rtv;
            bb->readLengthLimit = 0;
            if (rtv = bb->Read(token)) throw rtv;
            if (rtv = bb->Read(serviceType)) throw rtv;
            if (rtv = bb->Read(serviceAddress)) throw rtv;
	    }
	    ~JoinSuccess()
	    {
            mempool().SafeRelease(token);
            mempool().SafeRelease(serviceAddress);
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
            str.Append(", \"accountId\" : ", this->accountId);
            str.Append(", \"token\" : ", this->token);
            str.Append(", \"serviceType\" : ", this->serviceType);
            str.Append(", \"serviceAddress\" : ", this->serviceAddress);
        }


        inline virtual void ToBBuffer(xx::BBuffer &bb) const override
        {
            this->BaseType::ToBBuffer(bb);
            bb.Write(this->accountId);
            bb.Write(this->token);
            bb.Write(this->serviceType);
            bb.Write(this->serviceAddress);
        }

        inline virtual int FromBBuffer(xx::BBuffer &bb) override
        {
            int rtv = 0;
            if (rtv = this->BaseType::FromBBuffer(bb)) return rtv;
            if (rtv = bb.Read(this->accountId)) return rtv;
            bb.readLengthLimit = 0;
            if (rtv = bb.Read(this->token)) return rtv;
            if (rtv = bb.Read(this->serviceType)) return rtv;
            if (rtv = bb.Read(this->serviceAddress)) return rtv;
            return rtv;
        }

#pragma endregion
    };
}
namespace DB_Lobby
{
    // 获取用户信息成功
    struct GetUserInfoSuccess : PKG::Global::Response
    {
        typedef GetUserInfoSuccess ThisType;
        typedef PKG::Global::Response BaseType;
        PKG::Global::UserInfo* userInfo = nullptr;
#pragma region ctor, interface impls

	    GetUserInfoSuccess()
            : BaseType()
	    {
	    }
	    GetUserInfoSuccess(xx::BBuffer *bb)
            : BaseType(bb)
	    {
	        int rtv = 0;
            if (rtv = bb->Read(userInfo)) throw rtv;
	    }
	    ~GetUserInfoSuccess()
	    {
            mempool().SafeRelease(userInfo);
	    }

        inline virtual void ToString(xx::String &str) const override
        {
        	if (tsFlags())
        	{
        		str.Append("[ \"***** recursived *****\" ]");
        		return;
        	}
        	else tsFlags() = 1;

            str.Append("{ \"type\" : \"GetUserInfoSuccess\"");
            ToStringCore(str);
            str.Append(" }");
        
        	tsFlags() = 0;
        }
        inline virtual void ToStringCore(xx::String &str) const override
        {
            this->BaseType::ToStringCore(str);
            str.Append(", \"userInfo\" : ", this->userInfo);
        }


        inline virtual void ToBBuffer(xx::BBuffer &bb) const override
        {
            this->BaseType::ToBBuffer(bb);
            bb.Write(this->userInfo);
        }

        inline virtual int FromBBuffer(xx::BBuffer &bb) override
        {
            int rtv = 0;
            if (rtv = this->BaseType::FromBBuffer(bb)) return rtv;
            if (rtv = bb.Read(this->userInfo)) return rtv;
            return rtv;
        }

#pragma endregion
    };
    // 获取用户信息失败
    struct GetUserInfoFail : PKG::Global::Response
    {
        typedef GetUserInfoFail ThisType;
        typedef PKG::Global::Response BaseType;
        // 失败原因
        xx::String* reason = nullptr;
#pragma region ctor, interface impls

	    GetUserInfoFail()
            : BaseType()
	    {
	    }
	    GetUserInfoFail(xx::BBuffer *bb)
            : BaseType(bb)
	    {
	        int rtv = 0;
            bb->readLengthLimit = 0;
            if (rtv = bb->Read(reason)) throw rtv;
	    }
	    ~GetUserInfoFail()
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

            str.Append("{ \"type\" : \"GetUserInfoFail\"");
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
}
}
namespace xx
{
	template<>
	struct BytesFunc<PKG::Poker::Card, void>
	{
		static inline uint32_t Calc(PKG::Poker::Card const &in)
		{
			return BBCalc(in.set, in.point);
		}
		static inline uint32_t WriteTo(char *dstBuf, PKG::Poker::Card const &in)
		{
			return BBWriteTo(dstBuf, in.set, in.point);
		}
		static inline int ReadFrom(char const *srcBuf, uint32_t const &dataLen, uint32_t &offset, PKG::Poker::Card &out)
		{
			return BBReadFrom(srcBuf, dataLen, offset, out.set, out.point);
		}
	};
	template<>
	struct StrFunc<PKG::Poker::Card, void>
	{
		static inline uint32_t Calc(PKG::Poker::Card const &in)
		{
			return StrCalc(in.set, in.point) + 20;
		}
		static inline uint32_t WriteTo(char *dstBuf, PKG::Poker::Card const &in)
		{
			return StrWriteTo(dstBuf, "{ \"type\" : \"Card\"", ", \"set\" : ", in.set, ", \"point\" : ", in.point, " }");
        }
    };
	template<> struct TypeId<xx::String> { static const uint16_t value = 1; };
	template<> struct TypeId<xx::BBuffer> { static const uint16_t value = 2; };
	template<> struct TypeId<PKG::Client_Lobby::Join> { static const uint16_t value = 3; };
	template<> struct TypeId<PKG::Client_Lobby::PullRooms> { static const uint16_t value = 4; };
	template<> struct TypeId<PKG::Client_Lobby::JoinRoom> { static const uint16_t value = 5; };
	template<> struct TypeId<PKG::Client_Lobby::ChangeCoin> { static const uint16_t value = 6; };
	template<> struct TypeId<PKG::Game1_Client::JoinSuccess> { static const uint16_t value = 7; };
	template<> struct TypeId<PKG::Client_Game1::Join> { static const uint16_t value = 8; };
	template<> struct TypeId<PKG::Client_Game1::Quit> { static const uint16_t value = 9; };
	template<> struct TypeId<PKG::Lobby_Client::JoinSuccess> { static const uint16_t value = 10; };
	template<> struct TypeId<PKG::Lobby_Client::PullRoomsSuccess> { static const uint16_t value = 11; };
	template<> struct TypeId<xx::List<PKG::Global::RoomInfo*>> { static const uint16_t value = 12; };
	template<> struct TypeId<PKG::Lobby_Client::PullRoomsFail> { static const uint16_t value = 13; };
	template<> struct TypeId<PKG::Lobby_Client::JoinRoomSuccess> { static const uint16_t value = 14; };
	template<> struct TypeId<PKG::Global::IPAddress> { static const uint16_t value = 15; };
	template<> struct TypeId<PKG::Lobby_Client::JoinRoomFail> { static const uint16_t value = 16; };
	template<> struct TypeId<PKG::Lobby_Client::SyncCoin> { static const uint16_t value = 17; };
	template<> struct TypeId<PKG::Lobby_Client::ChangeCoinSuccess> { static const uint16_t value = 18; };
	template<> struct TypeId<PKG::Lobby_Client::ChangeCoinFail> { static const uint16_t value = 19; };
	template<> struct TypeId<PKG::Game1_Lobby::JoinSuccess> { static const uint16_t value = 20; };
	template<> struct TypeId<PKG::Global::Response> { static const uint16_t value = 21; };
	template<> struct TypeId<PKG::Game1_Lobby::JoinFail> { static const uint16_t value = 22; };
	template<> struct TypeId<PKG::Game1_Lobby::Quit> { static const uint16_t value = 23; };
	template<> struct TypeId<PKG::Lobby_Game1::Join> { static const uint16_t value = 24; };
	template<> struct TypeId<PKG::Global::Request> { static const uint16_t value = 25; };
	template<> struct TypeId<PKG::Lobby_Login::JoinSuccess> { static const uint16_t value = 26; };
	template<> struct TypeId<PKG::Lobby_Login::JoinFail> { static const uint16_t value = 27; };
	template<> struct TypeId<PKG::Login_Lobby::Join> { static const uint16_t value = 28; };
	template<> struct TypeId<PKG::Login_Client::LoginSuccess> { static const uint16_t value = 29; };
	template<> struct TypeId<PKG::Login_Client::LoginFail> { static const uint16_t value = 30; };
	template<> struct TypeId<PKG::DB_Login::AuthResult> { static const uint16_t value = 31; };
	template<> struct TypeId<PKG::Login_DB::Auth> { static const uint16_t value = 32; };
	template<> struct TypeId<PKG::Client_Login::Login> { static const uint16_t value = 33; };
	template<> struct TypeId<PKG::Global::ServiceInfo> { static const uint16_t value = 34; };
	template<> struct TypeId<PKG::Global::UserInfo> { static const uint16_t value = 35; };
	template<> struct TypeId<PKG::Global::RoomInfo> { static const uint16_t value = 36; };
	template<> struct TypeId<PKG::Global::RoomInfo_扎金花> { static const uint16_t value = 37; };
	template<> struct TypeId<PKG::Poker::Suit> { static const uint16_t value = 38; };
	template<> struct TypeId<xx::List<PKG::Poker::Card>> { static const uint16_t value = 39; };
	template<> struct TypeId<PKG::Lobby_DB::ChangeCoin> { static const uint16_t value = 40; };
	template<> struct TypeId<PKG::Lobby_DB::GetUserInfo> { static const uint16_t value = 41; };
	template<> struct TypeId<PKG::Game1::Player> { static const uint16_t value = 42; };
	template<> struct TypeId<PKG::Game1::Desk> { static const uint16_t value = 43; };
	template<> struct TypeId<xx::List<PKG::Game1::Player*>> { static const uint16_t value = 44; };
	template<> struct TypeId<PKG::DB_Lobby::ChangeCoinSuccess> { static const uint16_t value = 45; };
	template<> struct TypeId<PKG::DB_Lobby::ChangeCoinFail> { static const uint16_t value = 46; };
	template<> struct TypeId<PKG::DB_Lobby::SyncCoin> { static const uint16_t value = 47; };
	template<> struct TypeId<PKG::DB_Lobby::GetUserInfoSuccess> { static const uint16_t value = 48; };
	template<> struct TypeId<PKG::DB_Lobby::GetUserInfoFail> { static const uint16_t value = 49; };
}
namespace PKG
{
	inline void AllTypesRegister()
	{
	    xx::MemPool::Register<xx::String, xx::MPObject>();
	    xx::MemPool::Register<xx::BBuffer, xx::MPObject>();
	    xx::MemPool::Register<PKG::Client_Lobby::Join, xx::MPObject>();
	    xx::MemPool::Register<PKG::Client_Lobby::PullRooms, xx::MPObject>();
	    xx::MemPool::Register<PKG::Client_Lobby::JoinRoom, xx::MPObject>();
	    xx::MemPool::Register<PKG::Client_Lobby::ChangeCoin, xx::MPObject>();
	    xx::MemPool::Register<PKG::Game1_Client::JoinSuccess, xx::MPObject>();
	    xx::MemPool::Register<PKG::Client_Game1::Join, xx::MPObject>();
	    xx::MemPool::Register<PKG::Client_Game1::Quit, xx::MPObject>();
	    xx::MemPool::Register<PKG::Lobby_Client::JoinSuccess, xx::MPObject>();
	    xx::MemPool::Register<PKG::Lobby_Client::PullRoomsSuccess, xx::MPObject>();
	    xx::MemPool::Register<xx::List<PKG::Global::RoomInfo*>, xx::MPObject>();
	    xx::MemPool::Register<PKG::Lobby_Client::PullRoomsFail, xx::MPObject>();
	    xx::MemPool::Register<PKG::Lobby_Client::JoinRoomSuccess, xx::MPObject>();
	    xx::MemPool::Register<PKG::Global::IPAddress, xx::MPObject>();
	    xx::MemPool::Register<PKG::Lobby_Client::JoinRoomFail, xx::MPObject>();
	    xx::MemPool::Register<PKG::Lobby_Client::SyncCoin, xx::MPObject>();
	    xx::MemPool::Register<PKG::Lobby_Client::ChangeCoinSuccess, xx::MPObject>();
	    xx::MemPool::Register<PKG::Lobby_Client::ChangeCoinFail, xx::MPObject>();
	    xx::MemPool::Register<PKG::Game1_Lobby::JoinSuccess, PKG::Global::Response>();
	    xx::MemPool::Register<PKG::Global::Response, xx::MPObject>();
	    xx::MemPool::Register<PKG::Game1_Lobby::JoinFail, PKG::Global::Response>();
	    xx::MemPool::Register<PKG::Game1_Lobby::Quit, xx::MPObject>();
	    xx::MemPool::Register<PKG::Lobby_Game1::Join, PKG::Global::Request>();
	    xx::MemPool::Register<PKG::Global::Request, xx::MPObject>();
	    xx::MemPool::Register<PKG::Lobby_Login::JoinSuccess, PKG::Global::Response>();
	    xx::MemPool::Register<PKG::Lobby_Login::JoinFail, PKG::Global::Response>();
	    xx::MemPool::Register<PKG::Login_Lobby::Join, PKG::Global::Request>();
	    xx::MemPool::Register<PKG::Login_Client::LoginSuccess, xx::MPObject>();
	    xx::MemPool::Register<PKG::Login_Client::LoginFail, xx::MPObject>();
	    xx::MemPool::Register<PKG::DB_Login::AuthResult, PKG::Global::Response>();
	    xx::MemPool::Register<PKG::Login_DB::Auth, PKG::Global::Request>();
	    xx::MemPool::Register<PKG::Client_Login::Login, xx::MPObject>();
	    xx::MemPool::Register<PKG::Global::ServiceInfo, xx::MPObject>();
	    xx::MemPool::Register<PKG::Global::UserInfo, xx::MPObject>();
	    xx::MemPool::Register<PKG::Global::RoomInfo, xx::MPObject>();
	    xx::MemPool::Register<PKG::Global::RoomInfo_扎金花, PKG::Global::RoomInfo>();
	    xx::MemPool::Register<PKG::Poker::Suit, xx::MPObject>();
	    xx::MemPool::Register<xx::List<PKG::Poker::Card>, xx::MPObject>();
	    xx::MemPool::Register<PKG::Lobby_DB::ChangeCoin, PKG::Global::Request>();
	    xx::MemPool::Register<PKG::Lobby_DB::GetUserInfo, PKG::Global::Request>();
	    xx::MemPool::Register<PKG::Game1::Player, xx::MPObject>();
	    xx::MemPool::Register<PKG::Game1::Desk, xx::MPObject>();
	    xx::MemPool::Register<xx::List<PKG::Game1::Player*>, xx::MPObject>();
	    xx::MemPool::Register<PKG::DB_Lobby::ChangeCoinSuccess, PKG::Global::Response>();
	    xx::MemPool::Register<PKG::DB_Lobby::ChangeCoinFail, PKG::Global::Response>();
	    xx::MemPool::Register<PKG::DB_Lobby::SyncCoin, xx::MPObject>();
	    xx::MemPool::Register<PKG::DB_Lobby::GetUserInfoSuccess, PKG::Global::Response>();
	    xx::MemPool::Register<PKG::DB_Lobby::GetUserInfoFail, PKG::Global::Response>();
	}
}
