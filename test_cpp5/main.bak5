#include <windows.h>
#include <thread>
#include <iostream>

HWND hWnd = nullptr;
void Press(char key, int count = 1)
{
	for (int i = 0; i < count; ++i)
	{
		::PostMessage(hWnd, WM_KEYDOWN, key, 0);
		Sleep(50);
		::PostMessage(hWnd, WM_KEYUP, key, 0);
		std::cout << key;
	}
}
void TaskPress2()
{
	std::thread t([=]
	{
		while (true)
		{
			Press('1');
			Sleep(200);
			Press('2');
			Sleep(200);
			Press('3');
			Sleep(500);
			Press('2');
			Sleep(200);
			Press('3');
			Sleep(500);
			Press('2');
			Sleep(200);
			Press('3');
			Sleep(500);
			Press('2');
			Sleep(200);
			Press('3');
			Sleep(500);
			Press('2');
			Sleep(200);
			Press('3');
			Sleep(500);
			Press('2');
			Sleep(200);
			Press('3');
			Sleep(500);
			Press('2');
			Sleep(200);
			Press('3');
			Sleep(500);
		}
	});
	t.detach();
}
void TaskPress3()
{
	std::thread t([=]
	{
		while (true)
		{
			Press('A');			Sleep(50);
			Press('S');			Sleep(50);
			Press('D');			Sleep(50);
			Press('F');			Sleep(50);
			Press('G');			Sleep(50);
			Press('H');			Sleep(50);
			Press('J');			Sleep(50);
			Press('K');			Sleep(50);
			Press('L');			Sleep(50);
		}
	});
	t.detach();
}
void TaskPress(int preSleep, char key, int count = 1, int cd = 0)
{
	Sleep(preSleep);
	std::thread t([=]
	{
		while (true)
		{
			Press(key, count);
			if (cd)
			{
				Sleep(cd);
			}
			else
			{
				break;
			}
		}
	});
	t.detach();
}
int main()
{
	hWnd = FindWindow(nullptr, L"ClickerHeroes2");
	if (hWnd)
	{
		std::cout << "running...";

		//TaskPress(5, '1', 2, 6000);
		//TaskPress(15, '2', 2, 1000);
		//TaskPress(25, '3', 2, 700);
		//TaskPress(35, '4', 1, 4000);

		TaskPress2();
		TaskPress3();

		TaskPress(5, '5', 10, 70000);
		//TaskPress(55, '7', 10, 70000);
		//TaskPress(25, '8', 2, 0);

		//TaskPress(10, 'A', 1, 1000);
		//TaskPress(20, 'S', 1, 1000);
		//TaskPress(40, 'D', 1, 1000);
		//TaskPress(50, 'F', 1, 1000);
		//TaskPress(60, 'G', 1, 1000);
		//TaskPress(70, 'H', 1, 1000);
		//TaskPress(80, 'J', 1, 1000);
		//TaskPress(90, 'K', 1, 1000);
		//TaskPress(100, 'L', 1, 1000);

		std::cin.get();
	}
	else
	{

	}
}


//#include "xx_uv.h"
//
//// todo: xx_mysql.h
//
//// 实现一个基于 libuv + mariadb 异步接口的数据库模块, 提供类似 xx_sqlite 的用法
//// 具体为: 使用 mysql_get_socket 从 MYSQL* 拿到 socket 原始句柄, 再交由 libuv 的 pool_t 管理起来.
//// 以实现当 socket 状态变化时, 自动产生 libuv 的回调, 然后就可以执行 各种 mysql_xxxxxxxxxxx_cont 来检查正在执行的指令的执行情况.
//
//// 函数执行方式主要为回调模式, 执行期间无法继续下达其他指令. 执行期间整套类以状态机方式运作.
//// 不同于 sqlite, MYSQL* 上下文无法携带多个查询对象, 要同时做多个查询需要创建多个上下文. 即: 建立多个连接.
//
//
//namespace xx
//{
//	class MysqlManager;
//	class Mysql : public xx::Object
//	{
//		MysqlManager& mgr;
//		Mysql(MysqlManager& mgr);
//		// todo
//	};
//
//	// 根据连接串异步创建多个连接. 创建完毕后 cb
//	// 如果连接已经用完, 则不再继续创建, 而是将任务放入等待队列, 直到有连接还回
//	class MysqlManager : public xx::Object
//	{
//		MysqlManager(UvLoop& loop, String connStr, int ctxCount, std::function<void()> onComplete)
//			: xx::Object(loop.mempool)
//			, loop(loop)
//			, ctxPool(loop.mempool)
//			, tasks(loop.mempool)
//		{
//		}
//
//		// 用来管理 MYSQL* 中的 socket
//		UvLoop& loop;
//
//		// 预创建的连接池
//		List<Ptr<Mysql>> ctxPool;
//
//		// 无连接可用时的任务存放队列
//		Queue<std::function<void(Ptr<Mysql>)>> tasks;
//	};
//
//	inline Mysql::Mysql(MysqlManager& mgr)
//		: xx::Object(mgr.mempool)
//		, mgr(mgr)
//	{
//	}
//
//}
//
//#include <ctime>
//#include <iomanip>
//int main(int numArgs, char *args[])
//{
//	xx::MemPool mp;
//	xx::Object_p o = mp.MPCreatePtr<xx::String>();
//	o.As<xx::String>()->Append("xxx");
//	std::cout << o << std::endl;
//	std::cout << xx::GetDateTimeNowTicks() << std::endl;
//
//	std::time_t t = std::time(nullptr);
//	std::tm tm;
//	localtime_s(&tm, &t);
//	std::cout << std::put_time(&tm, "%Y-%m-%d %X") << std::endl;
//
//	auto tp = xx::DateTimeTicksToTimePoint(636650305817699126);
//	t = std::chrono::system_clock::to_time_t(tp);
//	localtime_s(&tm, &t);
//	std::cout << std::put_time(&tm, "%Y-%m-%d %X") << std::endl;
//	return 0;
//}


