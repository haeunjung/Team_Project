#pragma once

#include "..\Engine_Header.h"

WOOJUN_BEGIN

class DLL CPathMgr
{
private:
	unordered_map<string, wstring>	m_mapPath;
	unordered_map<string, wstring>::iterator m_iter;

	string		m_strFindPath;
public:
	bool Init();
	bool AddPath(const string& _strKey, WCHAR* _pPath, const string& _strBaseKey = BASEPATH);
	const WCHAR* FindPath(const string& _strKey);
	const CHAR* FindPathToMultiByte(const string& _strKey);

	DECLARE_SINGLE(CPathMgr)
};

WOOJUN_END