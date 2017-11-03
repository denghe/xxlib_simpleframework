#ifndef __CrossApp_CASyncQueue__
#define __CrossApp_CASyncQueue__

//#define _IS_USE_PTHREAD_ 
//#undef  _IS_USE_PTHREAD_     //非C++11的用户请屏蔽之

/************************************************************************/
/* 云娃提供                                                             */
/************************************************************************/
#include <queue>
#include <vector>

#ifndef _IS_USE_PTHREAD_
#include <mutex> 
#else
#include <pthread.h>
#endif

namespace YVSDK
{
#ifndef _IS_USE_PTHREAD_
	class  CYVLock
	{
	public:
		CYVLock(){  }
		~CYVLock(){ }

		void Lock(){m_cMutex.lock(); }
		void UnLock(){ m_cMutex.unlock(); }

	private:
		std::mutex m_cMutex;
	};
#else
	class  CYVLock
	{
	public:
		CYVLock(){ pthread_mutex_init(&m_cMutex, NULL); }
		~CYVLock(){ pthread_mutex_destroy(&m_cMutex); }

		void Lock(){ pthread_mutex_lock(&m_cMutex);  }
		void UnLock(){ pthread_mutex_unlock(&m_cMutex);  }
	private:
		pthread_mutex_t m_cMutex;
	};
#endif

	class CYVAutoLock
	{
	public:
		explicit CYVAutoLock(CYVLock& lock)
			: m_cLock(lock)
		{
			m_cLock.Lock();
		}
		~CYVAutoLock()
		{
			m_cLock.UnLock();
		}
	private:
		CYVLock& m_cLock;
	};

	template <typename T>
	class CYVSyncQueue
	{
	public:
		virtual ~CYVSyncQueue(void) {}

		void AddElement(T v)
		{
			CYVAutoLock lock(m_Cs);
			m_SyncQueue.push(v);
		}
		bool PopElement(T& v)
		{
			CYVAutoLock lock(m_Cs);
			if (!m_SyncQueue.empty())
			{
				v = m_SyncQueue.front();
				m_SyncQueue.pop();
				return true;
			}
			return false;
		}
		std::vector<T> GetQueueElements()
		{
			CYVAutoLock lock(m_Cs);

			std::vector<T> aList;
			while (!m_SyncQueue.empty())
			{
				aList.push_back(m_SyncQueue.front());
				m_SyncQueue.pop();
			}
			return aList;
		}
		int GetCount()
		{
			CYVAutoLock lock(m_Cs);
			return m_SyncQueue.size();
		}
		void Clear()
		{
			CYVAutoLock lock(m_Cs);
			while (!m_SyncQueue.empty())
				m_SyncQueue.pop();
		}

	private:
		CYVLock m_Cs;
		std::queue<T> m_SyncQueue;
	};
};
#endif