#include "xx.h"
#include "RPC_class.h"
#include "xx_sqlite.h"

int main()
{
	xx::MemPool::RegisterInternal();
	RPC::AllTypesRegister();
	xx::MemPool mp;

	xx::BBuffer bb(&mp);
	{
		auto pkg = mp.MPCreatePtr<RPC::Client_Login::Login>();

		mp.MPCreateTo(pkg->username);
		pkg->username->Assign("a");
		//pkg->password 保持空
		std::cout << pkg << std::endl;

		bb.WriteRoot(pkg);
		std::cout << bb << std::endl;
	}
	{
		xx::Object_p o;
		bb.Read(o);
		std::cout << o << std::endl;
	}

	return 0;
}
