#include "xx.h"
#include "RPC_class.h"
#include "xx_logger.h"

int main()
{
	//xx::MemPool::RegisterInternal();
	//RPC::AllTypesRegister();
	//xx::MemPool mp;

	//xx::BBuffer bb(&mp);
	//{
	//	auto pkg = mp.MPCreatePtr<RPC::Client_Login::Login>();

	//	mp.MPCreateTo(pkg->username);
	//	pkg->username->Assign("a");
	//	//pkg->password 保持空
	//	std::cout << pkg << std::endl;

	//	bb.WriteRoot(pkg);
	//	std::cout << bb << std::endl;
	//}
	//{
	//	xx::Object_p o;
	//	bb.Read(o);
	//	std::cout << o << std::endl;
	//}

	xx::Logger log("C:/Logs/xx.log.db");
	log.SetDefaultValue("machine name", "service type", 1);
	xx::Stopwatch sw2;
	log.OnRelease = [&]
	{
		std::cout << "sw2 =  " << sw2() << std::endl;
	};
	xx::Stopwatch sw;
	for (int j = 0; j < 10; ++j)
	{
		sw.Reset();
		size_t i = 0;
		for (; i < 1000000; i++)
		{
			//log.WriteAll(xx::LogLevel::Error, 0, "machine name", "service type", 1, "title", i, "desc.................................");
			log.Write(xx::LogLevel::Error, "title", i, "desc.................................");
		}
		std::cout << "inserted " << i << " logs. elapsed ms = " << sw() << std::endl;
		//Sleep(2000);
	}
	return 0;
}
