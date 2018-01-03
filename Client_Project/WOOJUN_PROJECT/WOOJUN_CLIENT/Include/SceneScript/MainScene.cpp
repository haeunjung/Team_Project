#include "MainScene.h"
#include "../ObjectScript/Player.h"
#include "../ObjectScript/MonsterBullet.h"
#include "../ObjectScript/Minion.h"
#include "../ObjectScript/RotBullet.h"
#include "../ObjectScript/PlayerBullet.h"
#include "01.Core/Debug.h"
#include "01.Core/KeyMgr.h"
#include "05.Scene/Scene.h"
#include "05.Scene/Layer.h"
#include "07.Component/Renderer.h"
#include "07.Component/Renderer2D.h"
#include "07.Component/Transform.h"
#include "07.Component/Camera.h"
#include "07.Component/CameraArm.h"
#include "07.Component/Material.h"
#include "07.Component/Effect.h"
#include "07.Component/Animation2D.h"
#include "07.Component/ColliderRect.h"
#include "07.Component/UIButton.h"
#include "07.Component/CheckBox.h"
#include "07.Component/RadioButton.h"
#include "07.Component/RadioButtonMgr.h"
#include "07.Component/UIBack.h"
#include "07.Component/Terrain.h"
#include "../ObjectScript/Mouse.h"

void CMainScene::CreateProtoType()
{
	// 총알 프로토타입
	CGameObject*		pMonsterBulletObject = CGameObject::Create("MonsterBulletObject", true);
	// Create 함수 두번째 디폴트인자를 true로 주면 프로토타입으로 생성한다
	CMonsterBullet*		pMonsterBulletScript = pMonsterBulletObject->AddComponent<CMonsterBullet>("MonsterBulletScript");

	SAFE_RELEASE(pMonsterBulletScript);
	SAFE_RELEASE(pMonsterBulletObject);

	// 총알 프로토타입
	CGameObject*		pPlayerBulletObject = CGameObject::Create("PlayerBulletObject", true);
	// Create 함수 두번째 디폴트인자를 true로 주면 프로토타입으로 생성한다
	CPlayerBullet*		pPlayerBulletScript = pPlayerBulletObject->AddComponent<CPlayerBullet>("PlayerBulletScript");

	SAFE_RELEASE(pPlayerBulletScript);
	SAFE_RELEASE(pPlayerBulletObject);	
}

void CMainScene::CreateCheckBox()
{
	CLayer*		pUILayer = m_pScene->FindLayer("UILayer");

	CGameObject*	pCheckButtonObject = CGameObject::Create("CheckButtonObject");

	CTransform*	pTransform = pCheckButtonObject->GetTransform();
	DxVector3	vScale = { 128.0f, 128.0f, 1.0f };
	pTransform->SetWorldScale(vScale);
	pTransform->SetWorldPos(1100.0f, 500.0f, 0.0f);
	pTransform->SetPivot(0.0f, 0.0f, 0.0f);

	CColliderRect*	pColRect = pCheckButtonObject->AddComponent<CColliderRect>("ButtonCol");
	DxVector3	vPos = pTransform->GetWorldPos();
	pColRect->SetRectInfo(0.0f, 0.0f, vScale.x, vScale.y);

	SAFE_RELEASE(pColRect);
	SAFE_RELEASE(pTransform);

	CRenderer2D*	pRenderer = pCheckButtonObject->AddComponent<CRenderer2D>("CheckButtonRenderer2D");
	pRenderer->SetMesh("UIMesh");
	pRenderer->SetShader(UI_SHADER);
	pRenderer->SetInputLayout("TexInputLayout");
	pRenderer->SetRenderState(ALPHABLEND);

	CMaterial*	pMaterial = pRenderer->GetMaterial();
	pMaterial->SetDiffuseTexture("Linear", "CheckBox", L"free2you2.png");
	SAFE_RELEASE(pMaterial);
	SAFE_RELEASE(pRenderer);
			
	CCheckBox*	pCheckBox = pCheckButtonObject->AddComponent<CCheckBox>("CheckBox");
	pCheckBox->CheckBoxTrue();
	pCheckBox->SetButtonFunc(this, &CMainScene::CheckButton);
	SAFE_RELEASE(pCheckBox);

	pUILayer->AddObject(pCheckButtonObject);
	SAFE_RELEASE(pCheckButtonObject);

	SAFE_RELEASE(pUILayer);
}

void CMainScene::CreateRadioButton()
{
	CLayer*		pUILayer = m_pScene->FindLayer("UILayer");

	for (int i = 0; i < 3; ++i)
	{
		CGameObject*	pRadioButtonObject = CGameObject::Create("RadioButtonObject");

		CTransform*	pTransform = pRadioButtonObject->GetTransform();
		DxVector3	vScale = { 128.0f, 128.0f, 1.0f };
		pTransform->SetWorldScale(vScale);
		pTransform->SetWorldPos((150.0f * i) + 100.0f , 500.0f, 0.0f);
		pTransform->SetPivot(0.0f, 0.0f, 0.0f);

		CColliderRect*	pColRect = pRadioButtonObject->AddComponent<CColliderRect>("ButtonCol");
		DxVector3	vPos = pTransform->GetWorldPos();
		pColRect->SetRectInfo(0.0f, 0.0f, vScale.x, vScale.y);

		SAFE_RELEASE(pColRect);
		SAFE_RELEASE(pTransform);

		CRenderer2D*	pRenderer = pRadioButtonObject->AddComponent<CRenderer2D>("RadioButtonRenderer2D");
		pRenderer->SetMesh("UIMesh");
		pRenderer->SetShader(UI_SHADER);
		pRenderer->SetInputLayout("TexInputLayout");
		pRenderer->SetRenderState(ALPHABLEND);

		CMaterial*	pMaterial = pRenderer->GetMaterial();
		pMaterial->SetDiffuseTexture("Linear", "CheckBox", L"free2you2.png");
		SAFE_RELEASE(pMaterial);
		SAFE_RELEASE(pRenderer);

		CRadioButton*	pRadioButton = pRadioButtonObject->AddComponent<CRadioButton>("RadioButton");
		pRadioButton->SetButtonFunc(this, &CMainScene::RadioButton);
		GET_SINGLE(CRadioButtonMgr)->PushRadioButton("One", pRadioButton);
		SAFE_RELEASE(pRadioButton);

		pUILayer->AddObject(pRadioButtonObject);
		SAFE_RELEASE(pRadioButtonObject);
	}	

	SAFE_RELEASE(pUILayer);
}

void CMainScene::CreateMouse()
{
	CLayer*	pUILayer = m_pScene->FindLayer("UILayer");

	CGameObject*	pMouseObject = CGameObject::Create("MouseObject");

	CMouse*	pMouse = pMouseObject->AddComponent<CMouse>("Mouse");
	SAFE_RELEASE(pMouse);

	pUILayer->AddObject(pMouseObject);
	SAFE_RELEASE(pMouseObject);

	SAFE_RELEASE(pUILayer);
}

void CMainScene::CreateInventory()
{
	CLayer*		pUILayer = m_pScene->FindLayer("UILayer");

	CGameObject*	pCheckButtonObject = CGameObject::Create("CheckButtonObject");

	CTransform*	pTransform = pCheckButtonObject->GetTransform();
	DxVector3	vScale = { 400.0f, 250.0f, 1.0f };
	pTransform->SetWorldScale(vScale);
	pTransform->SetWorldPos(400.0f, 100.0f, 0.0f);
	pTransform->SetPivot(0.0f, 0.0f, 0.0f);

	CColliderRect*	pColRect = pCheckButtonObject->AddComponent<CColliderRect>("ButtonCol");
	DxVector3	vPos = pTransform->GetWorldPos();
	pColRect->SetRectInfo(0.0f, 0.0f, vScale.x, vScale.y);

	SAFE_RELEASE(pColRect);
	SAFE_RELEASE(pTransform);

	CRenderer2D*	pRenderer = pCheckButtonObject->AddComponent<CRenderer2D>("CheckButtonRenderer2D");
	pRenderer->SetMesh("UIMesh");
	pRenderer->SetShader(UI_SHADER);
	pRenderer->SetInputLayout("TexInputLayout");
	pRenderer->SetRenderState(ALPHABLEND);

	CMaterial*	pMaterial = pRenderer->GetMaterial();
	pMaterial->SetDiffuseTexture("Linear", "InvenBox", L"mjh.jpg");
	SAFE_RELEASE(pMaterial);
	SAFE_RELEASE(pRenderer);

	CUIBack*	pUIBack = pCheckButtonObject->AddComponent<CUIBack>("InvenBox");
	SAFE_RELEASE(pUIBack);

	pUILayer->AddObject(pCheckButtonObject);
	SAFE_RELEASE(pCheckButtonObject);

	SAFE_RELEASE(pUILayer);
}

void CMainScene::CreateHpBar()
{
	CLayer*		pUILayer = m_pScene->FindLayer("UILayer");

	CGameObject*	pHpBarObejct = CGameObject::Create("HpBarObejct");

	CTransform*	pTransform = pHpBarObejct->GetTransform();
	DxVector3	vScale = { 400.0f, 50.0f, 1.0f };
	pTransform->SetWorldScale(vScale);
	pTransform->SetWorldPos(10.0f, 10.0f, 0.0f);	
	pTransform->SetPivot(0.0f, 0.0f, 0.0f);
	SAFE_RELEASE(pTransform);

	CRenderer2D*	pRenderer = pHpBarObejct->AddComponent<CRenderer2D>("HpBarRenderer2D");
	pRenderer->SetMesh("UIMesh");
	pRenderer->SetShader(UI_SHADER);
	pRenderer->SetInputLayout("TexInputLayout");
	pRenderer->SetRenderState(ALPHABLEND);

	CMaterial*	pMaterial = pRenderer->GetMaterial();
	pMaterial->SetDiffuseTexture("Linear", "HpBar", L"SmallHpBar.png");
	SAFE_RELEASE(pMaterial);
	SAFE_RELEASE(pRenderer);
	
	m_pHpBar = pHpBarObejct->AddComponent<CUIBar>("HpBar");
	m_pHpBar->SetMinMax(0.0f, 1000.0f);
	m_pHpBar->SetBarDir(BD_LEFT);	

	pUILayer->AddObject(pHpBarObejct);
	SAFE_RELEASE(pHpBarObejct);

	SAFE_RELEASE(pUILayer);
}

bool CMainScene::Init()
{
	CreateProtoType();
	CreateCheckBox();
	CreateRadioButton();

	//CreateInventory();
	CreateHpBar();
	//CreateMouse();

	CLayer*		pLayer = m_pScene->FindLayer(DEFAULTLAYER);
	
	// 카메라 오브젝트 생성
	CGameObject*	pCameraObject = m_pScene->GetMainCameraObject();

	CCameraArm*	pCameraArm = pCameraObject->AddComponent<CCameraArm>("CameraArm");
	SAFE_RELEASE(pCameraArm);	
	
	// 플레이어
	CGameObject*		pPlayerObject = CGameObject::Create("PlayerObject");	
	pLayer->AddObject(pPlayerObject);

	CCamera*	pCamera = m_pScene->GetMainCamera();
	pCamera->Attach(pPlayerObject, DxVector3(0.0f, 0.0f, -5.0f));

	pPlayerObject->AddRef();
	m_pPlayerObject = pPlayerObject;
	
	CPlayer*	pPlayerScript = pPlayerObject->AddComponent<CPlayer>("PlayerScript");
	
	SAFE_RELEASE(pPlayerScript);
	SAFE_RELEASE(pPlayerObject);		
	
	CGameObject*	pMinionObj = CGameObject::Create("MinionObject", true);
	CMinion*	pMinion = pMinionObj->AddComponent<CMinion>("MinionScript");
	SAFE_RELEASE(pMinion);
	SAFE_RELEASE(pMinionObj);
	
	SAFE_RELEASE(pCamera);
	SAFE_RELEASE(pCameraObject);

	int num = 10;
	while (0 <= num)
	{
		m_fRespawnTime -= m_fRespawnLimitTime;

		DxVector3	vPos = DxVector3(rand() % 6 - 2.5f, rand() % 4 - 1.5f, rand() % 8 + 3.f);

		CGameObject*	pMinionObj = CGameObject::CreateClone("MinionObject");

		CTransform*	pTransform = pMinionObj->GetTransform();
		pTransform->SetWorldPos(vPos);
		SAFE_RELEASE(pTransform);

		CMinion*	pMinion = pMinionObj->FindComponentFromTypeID<CMinion>();
		pMinion->SetPlayer(m_pPlayerObject);
		SAFE_RELEASE(pMinion);

		CLayer*	pLayer = m_pScene->FindLayer(DEFAULTLAYER);
		pLayer->AddObject(pMinionObj);
		SAFE_RELEASE(pLayer);

		SAFE_RELEASE(pMinionObj);

		--num;
	}
	
	SAFE_RELEASE(pLayer);

	CLayer*	pMapLayer = m_pScene->FindLayer(MAPLAYER);

	CGameObject*	pTerrainObject = CGameObject::Create("TerrainObject");

	CTerrain*	pTerrain = pTerrainObject->AddComponent<CTerrain>("Terrain");
	pTerrain->CreateTerrain("Terrain", 65, 65, 1, 1);
	/*pTerrain->SetBaseTexture("TerrainDiffuse", L"Terrain/BD_Terrain_Cliff05.dds");
	pTerrain->SetNormalTexture("TerrainDiffuse", L"Terrain/BD_Terrain_Cliff05_NRM.bmp");
	pTerrain->SetSpecularTexture("TerrainDiffuse", L"Terrain/BD_Terrain_Cliff05_SPEC.bmp");*/
	pTerrain->SetBaseTexture("TerrainDiffuse", L"Terrain/BD_Terrain_Cave_01.dds");
	pTerrain->SetNormalTexture("TerrainNormal", L"Terrain/BD_Terrain_Cave_01_NRM.bmp");
	pTerrain->SetSpecularTexture("TerrainSpc", L"Terrain/BD_Terrain_Cave_01_SPC.bmp");

	SAFE_RELEASE(pTerrain);

	pMapLayer->AddObject(pTerrainObject);
	SAFE_RELEASE(pTerrainObject);
	SAFE_RELEASE(pMapLayer);

	pCameraObject = m_pScene->CreateCamera("BulletCamera", DxVector3(0.0f, 0.0f, -5.0f));
	SAFE_RELEASE(pCameraObject);

	return true;
}

void CMainScene::Update(float _fTime)
{
	/*m_fRespawnTime += _fTime;

	while (m_fRespawnTime >= m_fRespawnLimitTime)
	{
		m_fRespawnTime -= m_fRespawnLimitTime;

		DxVector3	vPos = DxVector3(rand() % 6 - 2.5f, rand() % 4 - 1.5f, rand() % 8 + 3.f);

		CGameObject*	pMinionObj = CGameObject::CreateClone("MinionObject");

		CTransform*	pTransform = pMinionObj->GetTransform();
		pTransform->SetWorldPos(vPos);
		SAFE_RELEASE(pTransform);

		CMinion*	pMinion = pMinionObj->FindComponentFromTypeID<CMinion>();
		pMinion->SetPlayer(m_pPlayerObject);
		SAFE_RELEASE(pMinion);

		CLayer*	pLayer = m_pScene->FindLayer(DEFAULTLAYER);
		pLayer->AddObject(pMinionObj);
		SAFE_RELEASE(pLayer);

		SAFE_RELEASE(pMinionObj);
	}*/

	//static bool	bEnable = true;
	//static bool	bPush = false;

	//if (GetAsyncKeyState(VK_RETURN) & 0x8000)
	//{
	//	bPush = true;
	//}
	//else if (bPush)
	//{
	//	bPush = false;
	//	bEnable = !bEnable;
	//	CLayer*	pLayer = m_pScene->FindLayer(DEFAULTLAYER);

	//	pLayer->SetIsEnable(bEnable);

	//	SAFE_RELEASE(pLayer);	
	//}

	if (true == KEYPRESS("F1"))
	{
		m_pHpBar->AddValue(-100.0f * _fTime);
	}

	if (true == KEYPRESS("F2"))
	{
		m_pHpBar->AddValue(100.0f * _fTime);
	}

	CLayer*	pLayer = m_pScene->FindLayer(DEFAULTLAYER);
	pLayer->SetIsEnable(m_bCheck);
	SAFE_RELEASE(pLayer);
}

void CMainScene::CheckButton(CGameObject * _pObj, float _fTime)
{
	m_bCheck = !m_bCheck;
}

void CMainScene::RadioButton(CGameObject * _pObj, float _fTime)
{
	int CurRadioNum = GET_SINGLE(CRadioButtonMgr)->GetRadioButtonMgrIndex("One");

	char strNum[256] = {};
	if (0 != CurRadioNum)
	{
		sprintf_s(strNum, "CurRadioNum : %d\n", CurRadioNum);
	}
	else
	{
		sprintf_s(strNum, "All Radio Button Off\n");
	}
	
	CDebug::OutputConsole(strNum);
}

CMainScene::CMainScene() :
	m_pPlayerObject(NULL),
	m_fRespawnTime(0.0f),
	m_fRespawnLimitTime(5.0f),
	m_pHpBar(NULL)
{
	m_bCheck = true;
}

CMainScene::~CMainScene()
{
	SAFE_RELEASE(m_pPlayerObject);

	SAFE_RELEASE(m_pHpBar);
}
