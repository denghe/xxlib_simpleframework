﻿#include "xx_uv.h"

namespace xx
{
	// 用来在 CatchFish 中存坐标
	struct Pos
	{
		float x = 0, y = 0;
	};

	// 适配 Pos 之 ToString
	template<>
	struct StrFunc<Pos, void>
	{
		static inline void WriteTo(String& s, Pos const &in)
		{
			s.Reserve(s.dataLen + 40);
			s.Append("{ \"x\":", in.x, ", \"y\":", in.y, " }");
		}
	};

	// 适配 Pos 之 序列化 & 反序列化
	template<>
	struct BytesFunc<Pos, void>
	{
		static inline void WriteTo(BBuffer& bb, Pos const &in)
		{
			bb.Reserve(bb.dataLen + sizeof(Pos));
			memcpy(bb.buf + bb.dataLen, &in, sizeof(Pos));
			bb.dataLen += sizeof(Pos);
		}
		static inline int ReadFrom(BBuffer& bb, Pos &out)
		{
			if (bb.offset + sizeof(Pos) > bb.dataLen) return -1;
			memcpy(&out, bb.buf + bb.offset, sizeof(Pos));
			bb.offset += sizeof(Pos);
			return 0;
		}
	};
}

#include "../pkg/PKG_class.h"

// 从生成物场景派生( 不增加成员变量, 以便于序列化 )
class Scene : public PKG::CatchFish::Scene
{
public:
	Scene(xx::MemPool* mp)
		: PKG::CatchFish::Scene(mp)
	{
		rnd.MPCreate(mp, 123);
		players.MPCreate(mp);
		fishs.MPCreate(mp);
		bullets.MPCreate(mp);
	}

	bool Update()
	{
		std::cout << "frameNumber = " << frameNumber << std::endl;
		std::cout << "rnd = " << rnd->Next(0, 2) << std::endl;

		++frameNumber;
		return true;
	}

};
// 使用相同的 typeId 映射
template<> struct xx::TypeId<Scene> { static const uint16_t value = xx::TypeId_v<PKG::CatchFish::Scene>; };


#include <chrono>
#include <thread>

int main()
{
	// typeid 映射
	xx::MemPool::RegisterInternal();
	PKG::AllTypesRegister();

	// 创建基础内存池
	xx::MemPool mp;

	// 创建派生场景
	xx::Ptr<Scene> scene;
	scene.MPCreate(&mp);

	// 试跑几下
	for (int i = 0; i < 5; ++i)
	{
		scene->Update();
	}

	// 创建一个序列化用的 bb 并序列化场景
	xx::BBuffer bb(&mp);
	bb.WriteRoot(scene);

	// 回收老场景内存
	scene.Clear();

	// 从 bb 反序列化场景出来
	xx::Ptr<Scene> scene2;
	auto r = bb.ReadRoot(scene2);
	assert(!r);

	// 用反序列化出来的场景接着跑几下
	for (int i = 0; i < 5; ++i)
	{
		scene2->Update();
	}
	//std::this_thread::sleep_for(std::chrono::milliseconds(1000/60));
	return 0;
}
