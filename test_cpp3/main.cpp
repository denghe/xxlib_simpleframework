#pragma execution_character_set("utf-8")
// 为配合 web 项目, 实现一个供其连接的服务端

#include "../pkg/WEB_class.h"
#include "xx_uv.h"

xx::MemPool mp;
xx::UvLoop uv(&mp);
xx::RtvChecker rc;

// 初始化一些包默认实例备用
void InitPkgDefaultInstances()
{
	WEB::Generic::Success::defaultInstance.MPCreate(&mp);

	WEB::Generic::Error::defaultInstance.MPCreate(&mp);
	WEB::Generic::Error::defaultInstance->errMsg.MPCreate(&mp);
}

int main(int argc, char* argv[])
{
	WEB::AllTypesRegister();
	InitPkgDefaultInstances();

	auto listener = uv.CreateTcpListener();
	rc = listener;
	rc = listener->Bind("0.0.0.0", 11111);
	rc = listener->Listen();

	listener->OnAccept = [](xx::UvTcpPeer* peer)
	{
		std::cout << peer->Ip() << " connected." << std::endl;

		// 转为弱引用在回调中方便 if (peer) 以判断是否已释放( 断开 ).
		peer->OnReceiveRequest = [peer = xx::UvTcpPeer_w(peer)](uint32_t serial, xx::BBuffer& bb)
		{
			xx::Object_p o_;

			if (bb.ReadRoot(o_) || !o_)
			{
				std::cout << "recv: " << o_ << std::endl;
				peer->Release();
				return;
			}
			std::cout << "recv: " << o_ << std::endl;

			switch (o_.GetTypeId())
			{
			case xx::TypeId_v<WEB::WEB_testcpp3::Cmd1>:
			{
				auto& o = o_.As<WEB::WEB_testcpp3::Cmd1>();

				// 返回操作成功
				auto& pkg = WEB::Generic::Success::defaultInstance;
				peer->SendResponse(serial, pkg);
				std::cout << "send: " << pkg << std::endl;
				break;
			}
			case xx::TypeId_v<WEB::WEB_testcpp3::Cmd2>:
			{
				auto& o = o_.As<WEB::WEB_testcpp3::Cmd2>();

				// 返回错误信息
				auto& pkg = WEB::Generic::Error::defaultInstance;
				pkg->errNum = -12345;
				pkg->errMsg->Clear();
				pkg->errMsg->Append("出错啦! id = ", o->id);
				peer->SendResponse(serial, pkg);
				std::cout << "send: " << pkg << std::endl;
				break;
			}
			default:
			{
				// 返回错误信息
				auto& pkg = WEB::Generic::Error::defaultInstance;
				pkg->errNum = -1;
				pkg->errMsg->Clear();
				pkg->errMsg->Append("test_cpp3 收到未处理包: ", o_);
				peer->SendResponse(serial, pkg);
				std::cout << "send: " << pkg << std::endl;
				return;
			}
			}
		};

		peer->OnDispose = [peer] 
		{
			std::cout << peer->Ip() << " disconnected." << std::endl;
		};
	};

	return uv.Run();
}
