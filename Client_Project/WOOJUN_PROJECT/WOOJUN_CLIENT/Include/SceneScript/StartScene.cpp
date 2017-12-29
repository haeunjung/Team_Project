#include "StartScene.h"
#include "Engine_Core.h"
#include "01.Core/KeyMgr.h"
#include "05.Scene/SceneMgr.h"
#include "05.Scene/Scene.h"
#include "05.Scene/Layer.h"
#include "07.Component/Transform.h"
#include "07.Component/Material.h"
#include "07.Component/Renderer2D.h"
#include "07.Component/UIButton.h"
#include "07.Component/ColliderRect.h"
#include "../ObjectScript/Mouse.h"

#include "MainScene.h"
#include "LoadingScene.h"

bool CStartScene::Init()
{
	CLayer*		pUILayer = m_pScene->FindLayer("UILayer");

	CGameObject*	pBGObject = CGameObject::Create("BGObject");

	CTransform*		pTransform = pBGObject->GetTransform();
	pTransform->SetWorldScale(1280.0f, 720.0f, 1.0f);
	SAFE_RELEASE(pTransform);

	CRenderer2D*	pRenderer = pBGObject->AddComponent<CRenderer2D>("BGRenderer2D");
	pRenderer->SetMesh("UIMesh");
	pRenderer->SetShader(UI_SHADER);
	pRenderer->SetInputLayout("TexInputLayout");
	pRenderer->SetRenderState(ALPHABLEND);

	CMaterial*	pMaterial = pRenderer->GetMaterial();
	pMaterial->SetDiffuseTexture("Linear", "StartScene", L"StartScene/Lineage1.bmp");
	SAFE_RELEASE(pMaterial);
	SAFE_RELEASE(pRenderer);

	pUILayer->AddObject(pBGObject);
	SAFE_RELEASE(pBGObject);	
		
	CGameObject*	pMouseObject = CGameObject::Create("MouseObject");
	
	CMouse*	pMouse = pMouseObject->AddComponent<CMouse>("Mouse");
	SAFE_RELEASE(pMouse);

	pUILayer->AddObject(pMouseObject);
	SAFE_RELEASE(pMouseObject);

	SAFE_RELEASE(pUILayer);		

	CreateStartButton();
	CreateEndButton();
	
	return true;
}

void CStartScene::Update(float _fTime)
{
}

void CStartScene::CreateStartButton()
{
	CLayer*		pUILayer = m_pScene->FindLayer("UILayer");

	CGameObject*	pStartButtonObject = CGameObject::Create("StartButtonObject");

	CTransform*	pTransform = pStartButtonObject->GetTransform();
	DxVector3	vScale = { 200.0f, 100.0f, 1.0f };
	pTransform->SetWorldScale(vScale);
	pTransform->SetWorldPos(340.0f, 400.0f, 0.0f);

	CColliderRect*	pColRect = pStartButtonObject->AddComponent<CColliderRect>("ButtonCol");
	DxVector3	vPos = pTransform->GetWorldPos();
	pColRect->SetRectInfo(0.0f, vScale.y, 0.0f, vScale.x, false);

	SAFE_RELEASE(pColRect);
	SAFE_RELEASE(pTransform);

	CRenderer2D*	pRenderer = pStartButtonObject->AddComponent<CRenderer2D>("StartButtonRenderer2D");
	pRenderer->SetMesh("UIMesh");
	pRenderer->SetShader(UI_SHADER);
	pRenderer->SetInputLayout("TexInputLayout");
	pRenderer->SetRenderState(ALPHABLEND);

	CMaterial*	pMaterial = pRenderer->GetMaterial();
	pMaterial->SetDiffuseTexture("Linear", "StartButton", L"StartScene/Menu_Start.png");
	SAFE_RELEASE(pMaterial);
	SAFE_RELEASE(pRenderer);
	
	CUIButton*	pButton = pStartButtonObject->AddComponent<CUIButton>("Button");	
	pButton->SetButtonFunc(this, &CStartScene::StartButton);
	SAFE_RELEASE(pButton);

	pUILayer->AddObject(pStartButtonObject);
	SAFE_RELEASE(pStartButtonObject);

	SAFE_RELEASE(pUILayer);
}

void CStartScene::CreateEndButton()
{
	CLayer*		pUILayer = m_pScene->FindLayer("UILayer");

	CGameObject*	pStartButtonObject = CGameObject::Create("StartButtonObject");

	CRenderer2D*	pRenderer = pStartButtonObject->AddComponent<CRenderer2D>("StartButtonRenderer2D");
	pRenderer->SetMesh("UIMesh");
	pRenderer->SetShader(UI_SHADER);
	pRenderer->SetInputLayout("TexInputLayout");
	pRenderer->SetRenderState(ALPHABLEND);

	CMaterial*	pMaterial = pRenderer->GetMaterial();
	pMaterial->SetDiffuseTexture("Linear", "EndButton", L"StartScene/Menu_End.png");
	SAFE_RELEASE(pMaterial);
	SAFE_RELEASE(pRenderer);

	CTransform*	pTransform = pStartButtonObject->GetTransform();
	DxVector3	vScale = { 200.0f, 100.0f, 1.0f };
	pTransform->SetWorldScale(vScale);
	pTransform->SetWorldPos(740.0f, 400.0f, 0.0f);

	CColliderRect*	pColRect = pStartButtonObject->AddComponent<CColliderRect>("ButtonCol");
	DxVector3	vPos = pTransform->GetWorldPos();
	pColRect->SetRectInfo(0.0f, vScale.y, 0.0f, vScale.x, false);	

	SAFE_RELEASE(pColRect);
	SAFE_RELEASE(pTransform);

	CUIButton*	pButton = pStartButtonObject->AddComponent<CUIButton>("Button");
	pButton->SetButtonFunc(this, &CStartScene::ExitButton);
	SAFE_RELEASE(pButton);

	pUILayer->AddObject(pStartButtonObject);
	SAFE_RELEASE(pStartButtonObject);

	SAFE_RELEASE(pUILayer);
}

void CStartScene::ExitButton(CGameObject* _pObj, float _fTime)
{		
	GET_SINGLE(CEngine_Core)->Exit();
}

void CStartScene::StartButton(CGameObject * _pObj, float _fTime)
{
	CScene* pScene = GET_SINGLE(CSceneMgr)->CreateScene("LoadingScene");

	CLoadingScene* pLoad = pScene->CreateScript<CLoadingScene>();

	GET_SINGLE(CSceneMgr)->ReplaceScene(pScene);
}

CStartScene::CStartScene()
{
}


CStartScene::~CStartScene()
{
}
