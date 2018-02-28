#pragma once
#include "../Engine_Header.h"

WOOJUN_BEGIN

class CThread;
class DLL CThreadMgr
{
private:
	unordered_map<string, CThread*>		m_mapThread;
public:
	template <typename T>
	T* CreateThread(const string& _strKey)
	{
		T*	pThread = new T();

		if (false == pThread->Init())
		{
			SAFE_DELETE(pThread);
			return NULL;
		}

		m_mapThread.insert(make_pair(_strKey, pThread));

		return pThread;
	}

	DECLARE_SINGLE(CThreadMgr)
};

WOOJUN_END