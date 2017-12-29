#include "PathMgr.h"

WOOJUN_USING

DEFINITION_SINGLE(CPathMgr)

bool CPathMgr::Init()
{
	WCHAR strPath[MAX_PATH] = {};	
	GetModuleFileName(NULL, strPath, MAX_PATH);

	for (int i = lstrlen(strPath) - 1; i >= 0; --i)
	{
		if ('\\' == strPath[i] || '/' == strPath[i])
		{			
			memset(&strPath[i + 1], 0, sizeof(WCHAR) * (MAX_PATH - (i + 1)));
			break;
		}
	}

	m_mapPath.insert(make_pair(BASEPATH, strPath));

	AddPath(SHADERPATH, L"Shader\\");	
	AddPath(TEXTUREPATH, L"Texture\\");
	AddPath(MESHPATH, L"Mesh\\");

	return true;
}

bool CPathMgr::AddPath(const string & _strKey, WCHAR * _pPath, const string & _strBaseKey /*= BASE_PATH*/)
{	
	if (NULL != FindPath(_strKey))
	{
		// _strKey�� ã�Ҵµ� �̹� �־�
		return false;
	}

	// _strBaseKey�� ã��
	const WCHAR* pBasePath = FindPath(_strBaseKey);

	wstring strPath;

	// ã������
	if (NULL != pBasePath)
	{
		// strPath�� ����
		strPath = pBasePath;
	}
	// ��ã�������� �����Ѱ�??
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