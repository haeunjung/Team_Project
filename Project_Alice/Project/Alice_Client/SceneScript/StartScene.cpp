#include "StartScene.h"
#include "Engine_Core.h"
#include "01.Core/Input.h"
#include "05.Scene/SceneMgr.h"
#include "05.Scene/Scene.h"
#include "05.Scene/Layer.h"
#include "07.Component/Transform.h"
#include "07.Component/Renderer2D.h"
#include "07.Component/UIButton.h"
#include "07.Component/ColliderRect.h"
#include "../ObjectScript/Mouse.h"
#include "../ClientMgr/StageMgr.h"
#include "MainScene.h"
#include "LoadingScene.h"

bool CStartScene::Init()
{
	CLayer*		pUILayer = m_pScene->FindLayer("UILayer");

	// BGM
	CGameObject* pBGM = CGameObject::Create("BGM");
	m_pSoundPlayer = pBGM->AddComponent<CSoundPlayer>("BGMPlayer");
	m_pSoundPlayer->MyPlaySound("Title.mp3", FMOD_LOOP_NORMAL);

	SAFE_RELEASE(pBGM);

	CGameObject*	pBGObject = CGameObject::Create("BGObject");

	CTransform*		pTransform = pBGObject->GetTransform();
	pTransform->SetWorldPos(640.0f, 360.0f, 0.0f);
	pTransform->SetWorldScale(1280.0f, 720.0f, 1.0f);
	SAFE_RELEASE(pTransform);

	CRenderer2D*	pRenderer = pBGObject->AddComponent<CRenderer2D>("BGRenderer2D");
	pRenderer->SetMesh("UIMesh");
	pRenderer->SetShader(UI_SHADER);
	pRenderer->SetInputLayout("TexInputLayout");
	pRenderer->SetRenderState(ALPHABLEND);

	m_pMaterial = pRenderer->GetMaterial();
	//m_pMaterial->SetDiffuseTexture("Linear", "StartScene", L"StartScene/Title2.png");
	//SAFE_RELEASE(pMaterial);
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
	fTime += _fTime;
	if (1.0f <= fTime)
	{
		++Idx;
		fTime = 0.0f;
	}

	if (3 <= Idx)
	{
		Idx = 0;
	}

	switch (Idx)
	{
	case 0:
		m_pMaterial->SetDiffuseTexture("Linear", "Title1", L"StartScene/Title1-1.png");
		break;
	case 1:
		m_pMaterial->SetDiffuseTexture("Linear", "Title2", L"StartScene/Title1-2.png");
		break;
	case 2:
		m_pMaterial->SetDiffuseTexture("Linear", "Title3", L"StartScene/Title1-3.png");
		break;
	}
}

void CStartScene::CreateStartButton()
{
	CLayer*		pUILayer = m_pScene->FindLayer("UILayer");

	CGameObject*	pStartButtonObject = CGameObject::Create("StartButtonObject");

	CTransform*	pTransform = pStartButtonObject->GetTransform();
	DxVector3	vScale = { 200.0f, 100.0f, 1.0f };
	pTransform->SetWorldScale(vScale);
	pTransform->SetWorldPos(440.0f, 550.0f, 0.0f);

	CColliderRect*	pColRect = pStartButtonObject->AddComponent<CColliderRect>("ButtonCol");
	DxVector3	vPos = pTransform->GetWorldPos();
	DxVector3	vPivot = pTransform->GetPivot();
	DxVector3	vLeft = vPivot * vScale * -1.f;
	pColRect->SetRectInfo(vLeft.x, vLeft.y, vLeft.x + vScale.x, vLeft.y + vScale.y);

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
	pTransform->SetWorldPos(840.0f, 550.0f, 0.0f);

	CColliderRect*	pColRect = pStartButtonObject->AddComponent<CColliderRect>("ButtonCol");
	DxVector3	vPos = pTransform->GetWorldPos();
	DxVector3	vPivot = pTransform->GetPivot();
	DxVector3	vLeft = vPivot * vScale * -1.f;
	pColRect->SetRectInfo(vLeft.x, vLeft.y, vLeft.x + vScale.x, vLeft.y + vScale.y);

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
	//GET_SINGLE(CStageMgr)->SetCurStage(2);
	GET_SINGLE(CStageMgr)->StartStage();
}

CStartScene::CStartScene() :
	m_pSoundPlayer(NULL),
	m_pMaterial(NULL)
{
	fTime = 0.0f;
	Idx = 0;
}


CStartScene::~CStartScene()
{
	SAFE_RELEASE(m_pMaterial);
	SAFE_RELEASE(m_pSoundPlayer);
}
