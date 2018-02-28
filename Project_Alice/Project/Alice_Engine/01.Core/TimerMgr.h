#pragma once
#include "../Engine_Header.h"

WOOJUN_BEGIN

class CTimer;
class DLL CTimerMgr
{
private:
	unordered_map<string, CTimer*> m_mapTimer;
public:
	bool Init();
	CTimer* CreateTimer(const string& _strKey);
	CTimer* FindTimer(const string& _strKey);

	DECLARE_SINGLE(CTimerMgr)
};

WOOJUN_END