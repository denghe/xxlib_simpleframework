#ifndef _YAYA_NetManager_H_
#define _YAYA_NetManager_H_

#include <map>
#include <vector>


namespace YVSDK
{
	class YVPlatform;
	struct YaYaRequestBase;
	struct YaYaRespondBase;

//----------------------------------------------------------------------------------------------	
	struct YAYACallBackObj
	{
		virtual ~YAYACallBackObj(){}
		virtual long getObjAddr() = 0;
		virtual void call(YaYaRespondBase* request) = 0;
	};

	template<typename T>
	struct YAYACallBackSlot :
		public YAYACallBackObj
	{
		T* t;
		typedef void (T::*SEL_DataEvent)(YaYaRespondBase* request);
		SEL_DataEvent f;

		YAYACallBackSlot(T* obj, SEL_DataEvent func)
		{
			t = obj;
			f = func;
		}

		long getObjAddr()
		{
			return (*reinterpret_cast<long*>(t));
		}

		void call(YaYaRespondBase* request)
		{
			(t->*f)(request);
		}
	};

	typedef std::vector<YAYACallBackObj*> YaYaMsgCallFuncList;
	typedef std::map<int, YaYaMsgCallFuncList> YaYaMsgMap;
//----------------------------------------------------------------------------------------------	

	class YVMsgDispatcher
	{
		friend YVPlatform;
	public:
		template<typename T>
		void registerMsg(int cmdid, T* obj, void (T::*func)(YaYaRespondBase*))
		{
			YAYACallBackSlot<T>* callObj = new YAYACallBackSlot<T>(obj, func);

			YaYaMsgMap::iterator it = m_msgCallBackMap->find(cmdid);
			if (it != m_msgCallBackMap->end())
			{
				it->second.push_back(callObj);
			}
			else
			{
				YaYaMsgCallFuncList callFuncList;
				callFuncList.push_back(callObj);
				m_msgCallBackMap->insert(make_pair(cmdid, callFuncList));
			}
		}

		template<typename T>
		void unregisterMsg(int cmdid, T* obj)
		{
			YaYaMsgMap::iterator it = m_msgCallBackMap->find(cmdid);
			if (it == m_msgCallBackMap->end())
			{
				return;
			}

			YaYaMsgCallFuncList& funcList = it->second;
			long objAddr = *reinterpret_cast<long *>(obj);

			for (YaYaMsgCallFuncList::iterator its = funcList.begin();
				its != funcList.end(); ++its)
			{
				if ((*its)->getObjAddr() == objAddr)
				{
					delete *its;
					funcList.erase(its);
					break;
				}
			}

			if (funcList.size() == 0)
			{
				m_msgCallBackMap->erase(it);
			}
		}

        //: Shrimps, comment unused code
		//static YVMsgDispatcher* getSingletonPtr()
		//{
		//	if (s_msgDispatcher == NULL)
		//	{
		//		s_msgDispatcher = new YVMsgDispatcher();
		//	}
		//	return s_msgDispatcher;
		//}
        //.

	public:
		YVMsgDispatcher();
		~YVMsgDispatcher();
		void dispatchMsg(float dt);
		bool send(YaYaRequestBase*);
		static YaYaMsgMap* m_msgCallBackMap;

		void releseSDK();
	private:
		bool initSDK();
		static YVMsgDispatcher* s_msgDispatcher;
		bool _isSDKInit;
	};

//----------------------------------------------------------------------------------------------	
};
#endif