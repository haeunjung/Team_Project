#include "PathMgr.h"

WOOJUN_USING

DEFINITION_SINGLE(CPathMgr)

bool CPathMgr::Init()
{
	WCHAR strPath[MAX_PATH] = {};
	GetModuleFileName(NULL, strPath, MAX_PATH);

	int Num = 0;

	for (int i = lstrlen(strPath) - 1; i >= 0; --i)
	{
		if ('\\' == strPath[i] || '/' == strPath[i])
		{
			++Num;

			if (3 == Num)
			{
				memset(&strPath[i + 1], 0, sizeof(WCHAR) * (MAX_PATH - (i + 1)));
				break;
			}
		}
	}

	WCHAR strResPath[MAX_PATH] = {};
	lstrcpy(strResPath, strPath);
	lstrcat(strResPath, L"Res\\");
	m_mapPath.insert(make_pair(RESOURCEPATH, strResPath));

	lstrcat(strPath, L"Project\\");	
	m_mapPath.insert(make_pair(BASEPATH, strPath));

	//C:\Users\wooju\Desktop\Team_Project\Project_Alice\

	AddPath(SHADERPATH, L"Fx\\");	
	AddPath(TEXTUREPATH, L"Texture\\", RESOURCEPATH);
	AddPath(MESHPATH, L"Mesh\\", RESOURCEPATH);
	AddPath(DATAPATH, L"Data\\", RESOURCEPATH);

	return true;
}

bool CPathMgr::AddPath(const string & _strKey, WCHAR * _pPath, const string & _strBaseKey /*= BASE_PATH*/)
{	
	if (NULL != FindPath(_strKey))
	{
		// _strKey로 찾았는데 이미 있어
		return false;
	}

	// _strBaseKey로 찾고
	const WCHAR* pBasePath = FindPath(_strBaseKey);

	wstring strPath;

	// 찾았으면
	if (NULL != pBasePath)
	{
		// strPath에 대입
		strPath = pBasePath;
	}
	// 못찾았으면이 가능한가??
	/*else
	{
		return false;
	}*/

	strPath += _pPath;

	m_mapPath.insert(make_pair(_strKey, strPath));

	return true;
}

const WCHAR * CPathMgr::FindPath(const string & _strKey)
{
	m_iter = m_mapPath.find(_strKey);

	if (m_mapPath.end() == m_iter)
	{
		return NULL;
	}

	return m_iter->second.c_str();
}

const CHAR * CPathMgr::FindPathToMultiByte(const string & _strKey)
{
	const WCHAR*	pPath = FindPath(_strKey);

	if (NULL == pPath)
	{
		return NULL;
	}

	CHAR	strPath[MAX_PATH] = {};
	WideCharToMultiByte(CP_ACP, 0, pPath, -1, strPath, lstrlen(pPath), NULL, NULL);

	m_strFindPath = strPath;

	return m_strFindPath.c_str();
}

CPathMgr::CPathMgr()
{
}

CPathMgr::~CPathMgr()
{
}