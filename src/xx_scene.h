#pragma once
#include "xx_list.h"

// todo: 改为 .h + .hpp, 去掉命名空间. 作为纯包含文件 / sample 来用. 从 xx 中移除.
// .hpp 被包含时需要满足具备名为 MP 的内存池 type
// CreateToWithoutTypeId(fsmStack) 换为带 id 的版本

namespace xx
{
	// 带 Update 虚函数的类的基类
	struct UpdateBase : MPObject
	{
		// 返回非 0 表示自杀. 正数通常是正常结束生命, 负数通常表示出错
		virtual int Update() = 0;
	};

	struct SceneBase;
	struct SceneObjBase;

	// 状态机基类. 只能用 SceneObjBase 的函数来创建和杀掉. 不可以直接 Release
	struct FSMBase : UpdateBase
	{
		SceneObjBase* owner;
	};

	// 场景的子的基类( 场景对象 ). 只能用 SceneBase 的函数来创建和杀掉. 不可以直接 Release
	struct SceneObjBase : UpdateBase
	{
		// 指向场景容器
		SceneBase* sceneBase;

		// 位于场景怪容器中的下标( for 交换式快删 )
		uint32_t sceneObjsIndex;

		// todo: 提供一个容器用来存上个 FSM 的执行结果( 正常结束, 满足退出条件, 打断事件来临之类 )
		List<FSMBase*>* fsmStack;
		FSMBase* currFSM = nullptr;
		FSMBase* deadFSM = nullptr;		// 延迟杀掉

		SceneObjBase()
		{
			mempoolbase().CreateToWithoutTypeId(fsmStack);
		}

		template<typename MP, typename T, typename ...Args>
		T* CreateFSM(Args&&...args)
		{
			static_assert(std::is_base_of<FSMBase, T>::value, "the T must be inherit of FSMBase.");
			auto p = mempool<MP>().Create<T>(std::forward<Args>(args)...);
			p->owner = this;
			return p;
		}
		inline void SetFSM(FSMBase* fsm)
		{
			assert(fsm);
			assert(!deadFSM);
			deadFSM = currFSM;
			currFSM = fsm;
		}
		inline void PushFSM(FSMBase* fsm)
		{
			fsmStack->Add(currFSM);
			currFSM = fsm;
		}
		inline void PopFSM()
		{
			assert(!deadFSM);
			deadFSM = currFSM;
			currFSM = fsmStack->Top();
			fsmStack->Pop();
		}
		inline virtual int Update() override
		{
			auto rtv = currFSM->Update();
			assert(currFSM);				// 不允许自杀
			if (deadFSM)
			{
				deadFSM->Release();
				deadFSM = nullptr;
			}
			return rtv;
		}
		~SceneObjBase()
		{
			if (deadFSM)
			{
				deadFSM->Release();
				deadFSM = nullptr;
			}
			if (currFSM)
			{
				currFSM->Release();
				currFSM = nullptr;
			}
			while (fsmStack->dataLen)
			{
				fsmStack->Top()->Release();
				fsmStack->Pop();
			}
			fsmStack->Release();
		}
	};

	// 场景基类( 整合了内存池, 对象容器, LUA State )
	struct SceneBase : UpdateBase
	{
		// 所有 SceneObjBase* 的唯一持有容器. 只能用 SceneBase 的 Create, Release 来操作
		List<SceneObjBase*, true>* objs;

		SceneBase()
		{
			mempoolbase().CreateToWithoutTypeId(objs);
		}
		~SceneBase()
		{
			if (objs)
			{
				objs->Release();
				objs = nullptr;
			}
		}

		template<typename MP, typename T, typename...Args>
		MPtr<T> Create(Args&&...args)
		{
			auto p = mempool<MP>().Create<T>(std::forward<Args>(args)...);
			p->sceneBase = this;
			p->sceneObjsIndex = (uint32_t)objs->dataLen;
			objs->AddDirect(p);
			return p;
		}

		template<typename MP, typename T, typename...Args>
		void CreateTo(MPtr<T>& tar, Args&&...args)
		{
			tar = Create<MP, T>(std::forward<Args>(args)...);
		}

		// 调目标析构并入池, 从 objs 移除. 使用交换移除法, 返回是否产生过交换行为
		inline bool Release(SceneObjBase* p)
		{
			auto idx = p->sceneObjsIndex;
			if (objs->SwapRemoveAt(idx))
			{
				objs->At(idx)->sceneObjsIndex = idx;
				return true;
			}
			return false;
		}
	};

}
