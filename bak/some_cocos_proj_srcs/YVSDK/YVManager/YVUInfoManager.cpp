#include "YVUInfoManager.h"
using namespace  YVSDK;

YVUInfoPtr YVUInfoManager::getUInfoById(uint32 id)
{
	UInfoMap::iterator it  =  m_uinfos.find(id);
	if (it != m_uinfos.end())
	{
		return it->second;
	}
	return NULL;
}

void YVUInfoManager::updateUInfo(YVUInfoPtr info)
{
	UInfoMap::iterator it = m_uinfos.find(info->userid);
	if (it == m_uinfos.end())
	{
		m_uinfos.insert(std::make_pair(info->userid, info));
	}
	else
	{
		it->second = info;
	}
}