#include "Engine_Core.h"
#include "Client_Header.h"
#include "resource.h"
#include "05.Scene/SceneMgr.h"
#include "05.Scene/Scene.h"
#include "SceneScript/MainScene.h"
#include "SceneScript/StartScene.h"
#include "ClientMgr\StageMgr.h"

WOOJUN_USING

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	if (false == GET_SINGLE(CEngine_Core)->Init(hInstance, g_Resolution_Type[RT_HD].m_iWidth, g_Resolution_Type[RT_HD].m_iHeight,
		L"WoojunClient", L"WoojunClient", IDI_ICON1, IDI_ICON1, true))
	{
		DESTROY_SINGLE(CEngine_Core);
		return 0;
	}

	CScene*		pScene = GET_SINGLE(CSceneMgr)->GetCurScene();

	//pScene->CreateScript<CMainScene>();
	pScene->CreateScript<CStartScene>();

	int iReturnValue = GET_SINGLE(CEngine_Core)->Run();

	DESTROY_SINGLE(CEngine_Core);

	/*FILE* fp = NULL;
	fopen_s(&fp, "..\\..\\Res\\Data\\Save.Data", "wb");

	int One = 1;
	fwrite(&One, sizeof(int), 1, fp);

	fclose(fp);*/
	DESTROY_SINGLE(CStageMgr);

	return iReturnValue;
}