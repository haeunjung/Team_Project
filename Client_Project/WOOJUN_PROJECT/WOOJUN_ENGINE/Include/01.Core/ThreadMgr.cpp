#include "ThreadMgr.h"
#include "Thread.h"

WOOJUN_USING

DEFINITION_SINGLE(CThreadMgr)

CRITICAL_SECTION CThreadSync::m_CS;

CThreadMgr::CThreadMgr()
{
	InitializeCriticalSection(&CThreadSync::m_CS);
}

CThreadMgr::~CThreadMgr()
{
	unordered_map<string, CThread*>::iterator	iter;

	for (iter = m_mapThread.begin(); iter != m_mapThread.end(); ++iter)
	{
		SAFE_DELETE(iter->second);
	}

	m_mapThread.clear();

	DeleteCriticalSection(&CThreadSync::m_CS);
}
