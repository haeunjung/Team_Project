#include "LoadingThread.h"
#include "05.Scene/SceneMgr.h"

#include "../SceneScript/MainScene.h"

bool CLoadingThread::GetLoadComplete() const
{
	return m_bLoadComplete;
}

void CLoadingThread::Change()
{
	GET_SINGLE(CSceneMgr)->ReplaceScene(m_pScene);
}

void CLoadingThread::Run()
{
	m_pScene = GET_SINGLE(CSceneMgr)->CreateScene("MainScene");

	CMainScene*	pMainScene = m_pScene->CreateScript<CMainScene>();

	m_bLoop = false;
	m_bLoadComplete = true;
}

CLoadingThread::CLoadingThread() :
	m_bLoadComplete(false)	
{
}

CLoadingThread::~CLoadingThread()
{
}
