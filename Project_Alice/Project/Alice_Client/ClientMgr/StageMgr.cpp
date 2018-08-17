#include "StageMgr.h"
#include "01.Core/PathMgr.h"
#include "05.Scene/SceneMgr.h"
#include "05.Scene/Scene.h"
#include "../SceneScript/MainScene.h"
#include "../SceneScript/MainScene2.h"
#include "../SceneScript/MainScene3.h"
#include "../SceneScript/LoadingScene.h"
#include "../SceneScript/EndingScene.h"

DEFINITION_SINGLE(CStageMgr)

CStageMgr::CStageMgr() :
	m_iCurStage(1)
{
}

CStageMgr::~CStageMgr()
{
}

void CStageMgr::SetCurStage(int _iStage)
{
	m_iCurStage = _iStage;
}

int CStageMgr::GetCurStage()
{
	return m_iCurStage;
}

void CStageMgr::StartStage()
{
	//string	strPath = GET_SINGLE(CPathMgr)->FindPathToMultiByte(DATAPATH);
	//strPath += "Save.Data";

	//FILE*	pFile = NULL;
	//fopen_s(&pFile, strPath.c_str(), "rb");

	//fread(&m_iCurStage, sizeof(int), 1, pFile);

	switch (m_iCurStage)
	{
	case 1:
	{
		CScene * pScene = GET_SINGLE(CSceneMgr)->CreateScene("MainScene");
		CMainScene* pMain = pScene->CreateScript<CMainScene>();
		GET_SINGLE(CSceneMgr)->ReplaceScene(pScene);
		break;
	}
	case 2:
	{
		CScene * pScene = GET_SINGLE(CSceneMgr)->CreateScene("MainScene2");
		CMainScene2* pMain2 = pScene->CreateScript<CMainScene2>();
		GET_SINGLE(CSceneMgr)->ReplaceScene(pScene);
		break;
	}
	case 3:
	{
		CScene * pScene = GET_SINGLE(CSceneMgr)->CreateScene("MainScene3");
		CMainScene3* pMain3 = pScene->CreateScript<CMainScene3>();
		GET_SINGLE(CSceneMgr)->ReplaceScene(pScene);
		break;
	}
	}
}

void CStageMgr::NextStage()
{
	++m_iCurStage;

	if (3 >= m_iCurStage)
	{
		CScene * pScene = GET_SINGLE(CSceneMgr)->CreateScene("LoadingScene");
		CLoadingScene* pLoading = pScene->CreateScript<CLoadingScene>();
		GET_SINGLE(CSceneMgr)->ReplaceScene(pScene);
	}
	else
	{
		CScene * pScene = GET_SINGLE(CSceneMgr)->CreateScene("EndingScene");
		CEndingScene* pEnding = pScene->CreateScript<CEndingScene>();
		GET_SINGLE(CSceneMgr)->ReplaceScene(pScene);
	}	
}


