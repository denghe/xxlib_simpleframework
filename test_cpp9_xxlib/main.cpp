#include "xx.h"
#include "xx_uv.h"
#include "RPC_class.h"

class Service1 : public xx::Object
{
public:
	xx::UvLoop& loop;
	xx::UvTcpListener tcpListener;
	xx::Ptr<Service1>* store = nullptr;
	Service1(xx::UvLoop& loop)
		: xx::Object(loop.mempool)
		, loop(loop)
		, tcpListener(loop)
	{
		tcpListener.Bind("0.0.0.0", 12345);
		tcpListener.Listen();
	}
	~Service1()
	{
		assert(store);
		store->Clear();
	}
};
using Service1_p = xx::Ptr<Service1>;


int main()
{
	xx::MemPool::RegisterInternal();
	RPC::AllTypesRegister();
	xx::MemPool mp;

	RPC::Client_Login::Login_p pkg;
	mp.MPCreateTo(pkg);
	mp.MPCreateTo(pkg->username);
	mp.MPCreateTo(pkg->password);
	pkg->username->Assign("a");
	pkg->password->Assign("b");
	std::cout << pkg << std::endl;

	xx::BBuffer bb(&mp);
	bb.WriteRoot(pkg);
	std::cout << bb << std::endl;

	xx::Object_p obj;
	auto r = bb.Read(obj);
	std::cout << r << ", " << obj << std::endl;

	return 0;


	xx::UvLoop loop(&mp);
	auto s1 = mp.CreatePtr<Service1>(loop);
	s1->store = &s1;
	loop.Run();
	return 0;
}
