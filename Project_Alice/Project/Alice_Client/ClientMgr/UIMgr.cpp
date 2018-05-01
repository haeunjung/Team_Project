#include "UIMgr.h"
#include "05.Scene/Scene.h"
#include "06.GameObject/GameObject.h"
#include "07.Component/Renderer2D.h"
#include "07.Component/Renderer.h"
#include "07.Component/Material.h"
#include "07.Component/UIBack.h"
#include "07.Component/Animation2D.h"
#include "07.Component/Effect.h"
#include "../ObjectScript/BatteryCount.h"
#include "../ObjectScript/TimeBar.h"

DEFINITION_SINGLE(CUIMgr)

CUIMgr::CUIMgr() :
	m_pBatteryCount(NULL),
	m_pTimeBar(NULL),
	m_pSpringTransform(NULL)
{
}

CUIMgr::~CUIMgr()
{	
	SAFE_RELEASE(m_pSpringTransform);
	SAFE_RELEASE(m_pTimeBar);
	SAFE_RELEASE(m_pBatteryCount);
}

void CUIMgr::GetBattery()
{
	m_pBatteryCount->PlusCount();
}

void CUIMgr::UseBattery()
{
	m_pTimeBar->GetTime();
}

bool CUIMgr::Init(CScene* _pScene)
{
	CLayer*		pUILayer = _pScene->FindLayer("UILayer");

	CreateBattery2D(pUILayer);
	CreateBatteryCount(pUILayer);
	//CreateHpIcon(_pScene);
	CreateTimeBar(pUILayer);
	CreateSpring(pUILayer);

	SAFE_RELEASE(pUILayer);

	return true;
}

void CUIMgr::Update(float _fTime)
{
	//m_pSpringTransform->RotateZ(-1.57f, _fTime);
}

void CUIMgr::CreateBattery2D(CLayer* _pLayer)
{
	CGameObject*	pBatteryObject = CGameObject::Create("BatteryObject");

	CTransform*	pTransform = pBatteryObject->GetTransform();
	DxVector3	vScale = { 100.0f, 100.0f, 1.0f };
	pTransform->SetWorldScale(vScale);
	pTransform->SetWorldPos(200.0f, 20.0f, 0.0f);
	pTransform->SetPivot(0.0f, 0.0f, 0.0f);
	SAFE_RELEASE(pTransform);

	CRenderer2D*	pRenderer = pBatteryObject->AddComponent<CRenderer2D>("BatteryRenderer2D");
	pRenderer->SetMesh("UIMesh");
	pRenderer->SetShader(UI_SHADER);
	pRenderer->SetInputLayout("TexInputLayout");
	pRenderer->SetRenderState(ALPHABLEND);

	CMaterial*	pMaterial = pRenderer->GetMaterial();
	pMaterial->SetDiffuseTexture("Linear", "Battery2D", L"Battery2D.png");
	SAFE_RELEASE(pMaterial);
	SAFE_RELEASE(pRenderer);

	CUIBack*	pUIBack = pBatteryObject->AddComponent<CUIBack>("Battery2DBack");
	SAFE_RELEASE(pUIBack);

	_pLayer->AddObject(pBatteryObject);
	SAFE_RELEASE(pBatteryObject);
}

void CUIMgr::CreateBatteryCount(CLayer* _pLayer)
{
	CGameObject*	pBatteryCountObject = CGameObject::Create("BatteryCountObject");
	m_pBatteryCount = pBatteryCountObject->AddComponent<CBatteryCount>("BatteryCount");
	
	_pLayer->AddObject(pBatteryCountObject);
	SAFE_RELEASE(pBatteryCountObject);
}

void CUIMgr::CreateHpIcon(CLayer* _pLayer)
{	
	CGameObject*	pHpObject = CGameObject::Create("HpObject");

	CTransform*	pTransform = pHpObject->GetTransform();
	DxVector3	vScale = { 128.0f, 128.0f, 1.0f };
	pTransform->SetWorldScale(vScale);
	pTransform->SetWorldPos(500.0f, 30.0f, 0.0f);
	pTransform->SetPivot(0.0f, 0.0f, 0.0f);
	SAFE_RELEASE(pTransform);
		
	CRenderer2D*	pRenderer = pHpObject->AddComponent<CRenderer2D>("MouseRenderer");
	pRenderer->SetMesh("UIMesh");
	pRenderer->SetShader(UI_SHADER);
	pRenderer->SetInputLayout("TexInputLayout");
	pRenderer->SetRenderState(ALPHABLEND);

	vector<wstring>	vecMouse;
	for (int i = 0; i <= 9; ++i)
	{
		TCHAR	strMouse[MAX_PATH] = {};
		wsprintf(strMouse, L"%d.png", i);

		vecMouse.push_back(strMouse);
	}

	CAnimation2D*	pAnimation2D = pHpObject->AddComponent<CAnimation2D>("HeartAnimation2D");
	pAnimation2D->AddAnimation2DClip("Mouse", A2D_FRAME, AO_LOOP, vecMouse.size(), 1, 1.0f, 0, 0.0f, "Mouse", 0, &vecMouse);
	pAnimation2D->Start(true);

	CMaterial*	pMaterial = pRenderer->GetMaterial();
	pMaterial->SetDiffuseTexture("Linear", "Battery2D", L"Battery2D.png");	
	SAFE_RELEASE(pMaterial);
	SAFE_RELEASE(pRenderer);

	CUIBack*	pUIBack = pHpObject->AddComponent<CUIBack>("Battery2DBack");
	SAFE_RELEASE(pUIBack);

	_pLayer->AddObject(pHpObject);
	SAFE_RELEASE(pHpObject);
}

void CUIMgr::CreateTimeBar(CLayer* _pLayer)
{
	CGameObject*	pTimeBarObject = CGameObject::Create("TimeBarObject");
	m_pTimeBar = pTimeBarObject->AddComponent<CTimeBar>("TimeBar");

	_pLayer->AddObject(pTimeBarObject);
	SAFE_RELEASE(pTimeBarObject);
}

void CUIMgr::CreateSpring(CLayer * _pLayer)
{
	CGameObject*	pSpringObject = CGameObject::Create("SpringObject");

	m_pSpringTransform = pSpringObject->GetTransform();
	DxVector3	vScale = { 100.0f, 100.0f, 0.0f };
	m_pSpringTransform->SetWorldScale(vScale);
	m_pSpringTransform->SetWorldPos(100.0f, 625.0f, 0.0f);	

	CUIBack*	pUI = pSpringObject->AddComponent<CUIBack>("SpringRendererUI");
	SAFE_RELEASE(pUI);

	CRenderer2D*	pSpringRenderer = pSpringObject->AddComponent<CRenderer2D>("SpringRenderer");
	pSpringRenderer->SetMesh("UIMesh");
	pSpringRenderer->SetShader(UI_SHADER);
	pSpringRenderer->SetInputLayout("TexInputLayout");
	pSpringRenderer->SetRenderState(ALPHABLEND);

	CMaterial*	pMaterial = pSpringRenderer->GetMaterial();
	pMaterial->SetDiffuseTexture("Linear", "Ribon", L"Ribon.png");
	SAFE_RELEASE(pMaterial);
	SAFE_RELEASE(pSpringRenderer);

	_pLayer->AddObject(pSpringObject);
	SAFE_RELEASE(pSpringObject);

	CGameObject* pTimeBar = CGameObject::Create("HpBar");

	vScale = DxVector3(1000.0f, 50.0f, 1.0f);
	DxVector3 vPos = { 135.0f, 610.0f, 500.0f };

	CTransform* pTransform = pTimeBar->GetTransform();
	pTransform->SetWorldScale(vScale);
	pTransform->SetWorldPos(vPos);
	pTransform->SetPivot(0.0f, 0.0f, 0.0f);
	SAFE_RELEASE(pTransform);

	CRenderer2D*	pTimeBarRenderer = pTimeBar->AddComponent<CRenderer2D>("TimeBarMaxRenderer");
	pTimeBarRenderer->SetMesh("UIMesh");
	pTimeBarRenderer->SetShader(UI_SHADER);
	pTimeBarRenderer->SetInputLayout("TexInputLayout");
	pTimeBarRenderer->SetRenderState(ALPHABLEND);

	pMaterial = pTimeBarRenderer->GetMaterial();
	pMaterial->SetDiffuseTexture("Linear", "TimeBar1", L"TimeBar1.png");
	//pMaterial->SetDiffuseColor(DxVector4(0.5f, 0.5f, 0.5f, 1.0f));
	SAFE_RELEASE(pMaterial);
	SAFE_RELEASE(pTimeBarRenderer);

	pUI = pTimeBar->AddComponent<CUIBack>("TimeBarRenderer");
	SAFE_RELEASE(pUI);

	_pLayer->AddObject(pTimeBar);
	SAFE_RELEASE(pTimeBar);
}
