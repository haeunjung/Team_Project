#include "SceneMgr.h"
#include "Scene.h"
#include "../01.Core/CollisionMgr.h"

WOOJUN_USING

DEFINITION_SINGLE(CSceneMgr)

CScene * CSceneMgr::CreateScene(const string & _strTag)
{
	CScene* pScene = new CScene();

	if (false == pScene->Init())
	{
		SAFE_DELETE(pScene);
		return NULL;
	}

	pScene->SetTag(_strTag);

	return pScene;
}

int CSceneMgr::ChangeScene()
{
	if (NULL != m_pNextScene)
	{
		if (NULL != m_pPrevScene)
		{
			m_pCurScene = m_pNextScene;
		}
		else
		{
			SAFE_DELETE(m_pCurScene);
			m_pCurScene = m_pNextScene;
		}

		m_pNextScene = NULL;

		return SC_NEXT;
	}

	return SC_NONE;
}

void CSceneMgr::ReplaceScene(CScene * _pScene)
{
	// ���� Scene ����� NextScene�� ����
	m_pNextScene = _pScene;
}

void CSceneMgr::PushScene(CScene * _pScene)
{
	// ���� Scene�� PrevScene
	m_pPrevScene = m_pCurScene;

	// NextScene ����
	m_pNextScene = _pScene;
}

void CSceneMgr::PrevScene()
{
	// ���� Scene���� ����
	SAFE_DELETE(m_pCurScene);
	m_pCurScene = m_pPrevScene;
	m_pPrevScene = NULL;
}

bool CSceneMgr::Init()
{
	// Default Scene Create
	m_pCurScene = CreateScene(DEFAULTSCENE);

	if (NULL == m_pCurScene)
	{
		return false;
	}	

	return true;
}

int CSceneMgr::Input(float _fTime)
{
	m_pCurScene->Input(_fTime);

	return ChangeScene();
}

int CSceneMgr::Update(float _fTime)
{
	m_pCurScene->Update(_fTime);

	return ChangeScene();
}

int CSceneMgr::LateUpdate(float _fTime)
{
	m_pCurScene->LateUpdate(_fTime);

	return ChangeScene();
}

void CSceneMgr::Collision(float _fTime)
{
	m_pCurScene->Collision(_fTime);
}

void CSceneMgr::Render(float _fTime)
{
	m_pCurScene->Render(_fTime);
}

bool CSceneMgr::InsertScene(CScene* _pScene)
{		
	CScene*	pScene = FindScene(_pScene->m_strTag);

	if (NULL == pScene && _pScene == m_pCurScene)
	{
		return false;
	}

	// �μ�Ʈ �Ϸ��� ���� ���� ���̸�
	// �μ�Ʈ ���� ������

	m_mapScene.insert(make_pair(_pScene->m_strTag, _pScene));

	return true;
}

CScene * CSceneMgr::FindScene(const string & _strKey)
{
	unordered_map<string, CScene*>::iterator	iter = m_mapScene.find(_strKey);

	if (iter == m_mapScene.end())
	{
		return NULL;
	}

	return iter->second;
}

bool CSceneMgr::ChangeScene(const string & _strKey)
{
	CScene*	pScene = FindScene(_strKey);

	if (NULL == pScene)
	{
		return false;
	}
	
	// ������� �߰��ǰ�
	m_mapScene.insert(make_pair(m_pCurScene->m_strTag, m_pCurScene));

	// �ʿ��� �ٲܾ��� ��������
	m_mapScene.erase(_strKey);

	// �ٲٷ��� ���� ���� ���� �Ǿ���ϰ�
	m_pCurScene = pScene;

	return true;
}

CSceneMgr::CSceneMgr() : 
	m_pCurScene(NULL),
	m_pNextScene(NULL),
	m_pPrevScene(NULL)
{
}

CSceneMgr::~CSceneMgr()
{
	SAFE_DELETE(m_pPrevScene);
	SAFE_DELETE(m_pNextScene);
	SAFE_DELETE(m_pCurScene);

	unordered_map<string, CScene*>::iterator	iter;
	unordered_map<string, CScene*>::iterator	iterEnd = m_mapScene.end();

	for (iter = m_mapScene.begin(); iter != iterEnd; ++iter)
	{
		SAFE_DELETE(iter->second);
	}
}