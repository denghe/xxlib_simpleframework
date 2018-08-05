#define _CRT_SECURE_NO_WARNINGS


// todo: 发现 Ref 无法放入字典, 需要补齐结构


#include "xx_uv.h"
#include "../pkg/RPC_class.h"
// 超时值容器
struct TimeoutData : public xx::UvTimeouterBase
{
	TimeoutData(xx::MemPool* mp) : xx::UvTimeouterBase(mp) {}
	int keyIndex = -1;
	xx::Object_p value;		// todo: 已序列化结果缓存?
};

void f1()
{
	xx::MemPool mp;
	xx::UvLoop uvLoop(&mp);
	xx::UvTimeoutManager uvTM(uvLoop, 1000, 21, 20);
	xx::Dict<xx::String_p, TimeoutData*> cache(&mp);
	// 模拟放入一个值
	{
		// 创建一个超时值容器
		auto d = mp.MPCreate<TimeoutData>();

		// 设超时回调: 从字典移除后自杀
		d->OnTimeout = [d, &cache]
		{
			cache.RemoveAt(d->keyIndex);
			std::cout << "timeout. release: " << d->value << std::endl;
			d->Release();
		};
		d->timeouter = &uvTM;
		uvTM.Add(d);			// 加入超时管理

		// 模拟填值
		d->value = mp.MPCreatePtr<xx::String>("asdfqwerzxcv");

		// 放入字典, 存下标
		auto r = cache.Add(mp.MPCreatePtr<xx::String>("a"), d);
		d->keyIndex = r.index;
	}
	// 模拟读取一个值
	{
		auto key = mp.MPCreatePtr<xx::String>("a");
		auto idx = cache.Find(key);
		if (idx != -1)
		{
			auto d = cache.ValueAt(idx);
			uvTM.AddOrUpdate(d);			// 刷新超时

			std::cout << d->value << std::endl;
		}
	}

	//xx::UvTcpListener uvListener(uvLoop);
	//uvListener.Bind("0.0.0.0", 12345);

	uvLoop.Run();
}

void f2()
{

}

int main()
{
	f1();

	return 0;
}
