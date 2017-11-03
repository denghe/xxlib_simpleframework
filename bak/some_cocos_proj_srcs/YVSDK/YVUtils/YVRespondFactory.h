#ifndef _YAYA_RESPOND_H_
#define _YAYA_RESPOND_H_
#include <map>

namespace YVSDK
{
	struct YaYaRespondBase;
	typedef YaYaRespondBase* (*RespondCreator)();

#define AutoRegisterRespond(cmd, T)  \
	YaYaRespondBase* T##Creator(){ return new T(); } \
	struct T##_S  \
	{ \
	T##_S() \
	{ \
	RespondFactory::getSingletonPtr()->addCreator(cmd, T##Creator); \
	} \
	}; \
	T##_S T##r;

	class RespondFactory
	{
	public:
		~RespondFactory()
		{
			m_respondFactoryPtr = NULL;
			m_creatorList.clear();
		}

		static RespondFactory* getSingletonPtr()
		{
			if (m_respondFactoryPtr == NULL)
			{
				m_respondFactoryPtr = new RespondFactory();
			}
			return m_respondFactoryPtr;
		}

		void addCreator(unsigned int cmd, RespondCreator creator)
		{
			std::map<unsigned int, RespondCreator>::iterator it = m_creatorList.find(cmd);
			if (it == m_creatorList.end())
			{
				m_creatorList.insert(std::make_pair(cmd, creator));
			}
		}

		YaYaRespondBase* getRepsond(unsigned int cmd)
		{
			std::map<unsigned int, RespondCreator>::iterator it = m_creatorList.find(cmd);
			if (it == m_creatorList.end())
			{
				return NULL;
			}
			return (*(it->second))();
		}

	private:
		static RespondFactory* m_respondFactoryPtr;
		std::map<unsigned int, RespondCreator> m_creatorList;
	};

};
#endif