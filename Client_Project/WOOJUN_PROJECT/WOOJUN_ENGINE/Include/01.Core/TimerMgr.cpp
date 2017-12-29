#include "TimerMgr.h"
#include "Timer.h"

WOOJUN_USING

DEFINITION_SINGLE(CTimerMgr)

CTimerMgr::CTimerMgr()
{
}

CTimerMgr::~CTimerMgr()
{
	Safe_Release_Map(m_mapTimer);
}

bool CTimerMgr::Init()
{
	CTimer* pTimer = CreateTimer(MAINTIMER);

	SAFE_RELEASE(pTimer);

	return true;
}

CTimer * CTimerMgr::CreateTimer(const string & _strKey)
{
	CTimer* pTimer = FindTimer(_strKey);

	if (NULL != pTimer)
	{
		return pTimer;
	}

	pTimer = new CTimer();

	if (false == pTimer->Init())
	{
		SAFE_RELEASE(pTimer);
		return NULL;
	}

	pTimer->AddRef();

	m_mapTimer.insert(make_pair(_strKey, pTimer));

	return pTimer;

}

CTimer * CTimerMgr::FindTimer(const string & _strKey)
{
	unordered_map<string, class CTimer*>::iterator	iter = m_mapTimer.find(_strKey);

	if (m_mapTimer.end() == iter)
	{
		return NULL;	
	}

	iter->second->AddRef();

	return iter->second;
}
