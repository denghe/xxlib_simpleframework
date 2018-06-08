#include "xx_uv.h"

// todo: xx_mysql.h

// 实现一个基于 libuv + mariadb 异步接口的数据库模块, 提供类似 xx_sqlite 的用法
// 具体为: 使用 mysql_get_socket 从 MYSQL* 拿到 socket 原始句柄, 再交由 libuv 的 pool_t 管理起来.
// 以实现当 socket 状态变化时, 自动产生 libuv 的回调, 然后就可以执行 各种 mysql_xxxxxxxxxxx_cont 来检查正在执行的指令的执行情况.

// 函数执行方式主要为回调模式, 执行期间无法继续下达其他指令. 执行期间整套类以状态机方式运作.
// 不同于 sqlite, MYSQL* 上下文无法携带多个查询对象, 要同时做多个查询需要创建多个上下文. 即: 建立多个连接.


namespace xx
{
	class MysqlManager;
	class Mysql : public xx::Object
	{
		MysqlManager& mgr;
		Mysql(MysqlManager& mgr);
		// todo
	};

	// 根据连接串异步创建多个连接. 创建完毕后 cb
	// 如果连接已经用完, 则不再继续创建, 而是将任务放入等待队列, 直到有连接还回
	class MysqlManager : public xx::Object
	{
		MysqlManager(UvLoop& loop, String connStr, int ctxCount, std::function<void()> onComplete)
			: xx::Object(loop.mempool)
			, loop(loop)
			, ctxPool(loop.mempool)
			, tasks(loop.mempool)
		{
		}

		// 用来管理 MYSQL* 中的 socket
		UvLoop& loop;

		// 预创建的连接池
		List<Ptr<Mysql>> ctxPool;

		// 无连接可用时的任务存放队列
		Queue<std::function<void(Ptr<Mysql>)>> tasks;
	};

	inline Mysql::Mysql(MysqlManager& mgr)
		: xx::Object(mgr.mempool)
		, mgr(mgr)
	{
	}

}


int main(int numArgs, char *args[])
{
	xx::MemPool mp;
	xx::Object_p o = mp.MPCreatePtr<xx::String>();
	o.As<xx::String>()->Append("xxx");
	std::cout << o << std::endl;

	return 0;
}
