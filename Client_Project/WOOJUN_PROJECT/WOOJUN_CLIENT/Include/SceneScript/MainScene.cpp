#include "MainScene.h"
#include "../ObjectScript/Player.h"
#include "../ObjectScript/MonsterBullet.h"
#include "../ObjectScript/Minion.h"
#include "../ObjectScript/RotBullet.h"
#include "../ObjectScript/PlayerBullet.h"
#include "01.Core/Debug.h"
#include "01.Core/Input.h"
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
	CGameObject*	pMonsterBulletObject = CGameObject::Create("MonsterBulletObject", true);
	// Create 함수 두번째 디폴트인자를 true로 주면 프로토타입으로 생성한다
	CMonsterBullet*	pMonsterBulletScript = pMonsterBulletObject->AddComponent<CMonsterBullet>("MonsterBulletScript");

	SAFE_RELEASE(pMonsterBulletScript);
	SAFE_RELEASE(pMonsterBulletObject);

	// 총알 프로토타입
	CGameObject*	pPlayerBulletObject = CGameObject::Create("PlayerBulletObject", true);
	// Create 함수 두번째 디폴트인자를 true로 주면 프로토타입으로 생성한다
	CPlayerBullet*	pPlayerBulletScript = pPlayerBulletObject->AddComponent<CPlayerBullet>("PlayerBulletScript");

	SAFE_RELEASE(pPlayerBulletScript);
	SAFE_RELEASE(pPlayerBulletObject);	

	// Monster
	CGameObject*	pMinionObj = CGameObject::Create("MinionObject", true);
	CMinion*	pMinion = pMinionObj->AddComponent<CMinion>("MinionScript");
	SAFE_RELEASE(pMinion);
	SAFE_RELEASE(pMinionObj);

	/*
	// WareHouse
	CGameObject*	pWareHouseObject = CGameObject::Create("WareHouseObject", true);
	CRenderer*		pRenderer = pWareHouseObject->AddComponent<CRenderer>("WareHouseRenderer");
	pRenderer->SetMesh("WareHouse", L"Warehouse01.FBX");
	pRenderer->SetShader(STANDARD_BUMP_SHADER);
	pRenderer->SetInputLayout("BumpInputLayout");
	pRenderer->SetRenderState(ALPHABLEND);
	SAFE_RELEASE(pWareHouseObject);
	SAFE_RELEASE(pRenderer);

	// BurnHouse1
	CGameObject*	pBurnHouse1 = CGameObject::Create("BurnHouse1Object", true);
	pRenderer = pBurnHouse1->AddComponent<CRenderer>("BurnHouse1Renderer");
	pRenderer->SetMesh("BurnHouse1", L"BurnHouse1.FBX");
	pRenderer->SetShader(STANDARD_BUMP_SHADER);
	pRenderer->SetInputLayout("BumpInputLayout");
	pRenderer->SetRenderState(ALPHABLEND);
	SAFE_RELEASE(pBurnHouse1);
	SAFE_RELEASE(pRenderer);

	// BurnHouse2
	CGameObject*	pBurnHouse2 = CGameObject::Create("BurnHouse2Object", true);
	pRenderer = pBurnHouse2->AddComponent<CRenderer>("BurnHouse2Renderer");
	pRenderer->SetMesh("BurnHouse2", L"BurnHouse2.FBX");
	pRenderer->SetShader(STANDARD_BUMP_SHADER);
	pRenderer->SetInputLayout("BumpInputLayout");
	pRenderer->SetRenderState(ALPHABLEND);
	SAFE_RELEASE(pBurnHouse2);
	SAFE_RELEASE(pRenderer);

	// BurnHouse3
	CGameObject*	pBurnHouse3 = CGameObject::Create("BurnHouse3Object", true);
	pRenderer = pBurnHouse3->AddComponent<CRenderer>("BurnHouse3Renderer");
	pRenderer->SetMesh("BurnHouse3", L"BurnHouse3.FBX");
	pRenderer->SetShader(STANDARD_BUMP_SHADER);
	pRenderer->SetInputLayout("BumpInputLayout");
	pRenderer->SetRenderState(ALPHABLEND);
	SAFE_RELEASE(pBurnHouse3);
	SAFE_RELEASE(pRenderer);

	// BurnHouse4
	CGameObject*	pBurnHouse4 = CGameObject::Create("BurnHouse4Object", true);
	pRenderer = pBurnHouse4->AddComponent<CRenderer>("BurnHouse4Renderer");
	pRenderer->SetMesh("BurnHouse4", L"BurnHouse4.FBX");
	pRenderer->SetShader(STANDARD_BUMP_SHADER);
	pRenderer->SetInputLayout("BumpInputLayout");
	pRenderer->SetRenderState(ALPHABLEND);
	SAFE_RELEASE(pBurnHouse4);
	SAFE_RELEASE(pRenderer);

	// BurnHouse5
	CGameObject*	pBurnHouse5 = CGameObject::Create("BurnHouse5Object", true);
	pRenderer = pBurnHouse5->AddComponent<CRenderer>("BurnHouse5Renderer");
	pRenderer->SetMesh("BurnHouse5", L"BurnHouse5.FBX");
	pRenderer->SetShader(STANDARD_BUMP_SHADER);
	pRenderer->SetInputLayout("BumpInputLayout");
	pRenderer->SetRenderState(ALPHABLEND);
	SAFE_RELEASE(pBurnHouse5);
	SAFE_RELEASE(pRenderer);

	// Sakura
	CGameObject*	pSakura = CGameObject::Create("SakuraObject", true);
	pRenderer = pSakura->AddComponent<CRenderer>("SakuraRenderer");
	pRenderer->SetMesh("Sakura", L"Sakura_00.FBX");
	pRenderer->SetShader(STANDARD_BUMP_SHADER);
	pRenderer->SetInputLayout("BumpInputLayout");
	pRenderer->SetRenderState(ALPHABLEND);
	SAFE_RELEASE(pSakura);
	SAFE_RELEASE(pRenderer);

	// Insam
	CGameObject*	pInsam = CGameObject::Create("InsamObject", true);
	pRenderer = pInsam->AddComponent<CRenderer>("InsamRenderer");
	pRenderer->SetMesh("Insam", L"InSam_00.FBX");
	pRenderer->SetShader(STANDARD_BUMP_SHADER);
	pRenderer->SetInputLayout("BumpInputLayout");
	pRenderer->SetRenderState(ALPHABLEND);
	SAFE_RELEASE(pInsam);
	SAFE_RELEASE(pRenderer);

	// ReturnPoint
	CGameObject*	pReturnPoint = CGameObject::Create("ReturnPointObject", true);
	pRenderer = pReturnPoint->AddComponent<CRenderer>("ReturnPointRenderer");
	pRenderer->SetMesh("ReturnPoint", L"ReturnPoint.FBX");
	pRenderer->SetShader(STANDARD_BUMP_SHADER);
	pRenderer->SetInputLayout("BumpInputLayout");
	pRenderer->SetRenderState(ALPHABLEND);
	SAFE_RELEASE(pReturnPoint);
	SAFE_RELEASE(pRenderer);
	*/
}

void CMainScene::CreateCheckBox()
{
	CLayer*		pUILayer = m_pScene->FindLayer("UILayer");

	CGameObject*	pCheckButtonObject = CGameObject::Create("CheckButtonObject");

	CTransform*	pTransform = pCheckButtonObject->GetTransform();
	DxVector3	vScale = { 128.0f, 128.0f, 1.0f };
	pTransform->SetWorldScale(vScale);
	pTransform->SetWorldPos(1100.0f, 550.0f, 0.0f);
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
		pTransform->SetWorldPos((150.0f * i) + 50.0f , 550.0f, 0.0f);
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

void CMainScene::CreateTerrain()
{
	CLayer*	pMapLayer = m_pScene->FindLayer(MAPLAYER);

	CGameObject*	pTerrainObject = CGameObject::Create("TerrainObject");

	CTransform*		pTransform = pTerrainObject->GetTransform();
	//pTransform->SetWorldPos(-32.0f, -1.0f, -32.0f);
	SAFE_RELEASE(pTransform);

	CTerrain*	pTerrain = pTerrainObject->AddComponent<CTerrain>("Terrain");
	pTerrain->CreateTerrain("Terrain", TERRAINSIZE, TERRAINSIZE, 1, 1, "Terrain/Height1.bmp");
	pTerrain->SetBaseTexture("TerrainDiffuse", L"Terrain/BD_Terrain_Cliff05.dds");
	pTerrain->SetNormalTexture("TerrainNormal", L"Terrain/BD_Terrain_Cliff05_NRM.bmp");
	pTerrain->SetSpecularTexture("TerrainSpc", L"Terrain/BD_Terrain_Cliff05_SPEC.bmp");
	/*pTerrain->SetBaseTexture("TerrainDiffuse", L"Terrain/BD_Terrain_Cave_01.dds");
	pTerrain->SetNormalTexture("TerrainNormal", L"Terrain/BD_Terrain_Cave_01_NRM.bmp");
	pTerrain->SetSpecularTexture("TerrainSpc", L"Terrain/BD_Terrain_Cave_01_SPC.bmp");*/

	vector<wstring>	vecMultiTex;

	vecMultiTex.push_back(L"Terrain/BD_Terrain_Cave_01.dds");
	vecMultiTex.push_back(L"Terrain/Terrain_Cliff_11.dds");
	vecMultiTex.push_back(L"Terrain/Terrain_Cliff_13.dds");
	vecMultiTex.push_back(L"Terrain/Terrain_Cliff_15_Large.dds");

	pTerrain->SetSplatTexture("TerrainSplat", vecMultiTex);

	// Normal 세팅
	vecMultiTex.clear();

	vecMultiTex.push_back(L"Terrain/BD_Terrain_Cave_01_NRM.bmp");
	vecMultiTex.push_back(L"Terrain/Terrain_Cliff_11_NRM.bmp");
	vecMultiTex.push_back(L"Terrain/Terrain_Cliff_13_NRM.bmp");
	vecMultiTex.push_back(L"Terrain/Terrain_Cliff_15_Large_NRM.bmp");

	pTerrain->SetSplatNormalTexture("TerrainSplatNormal", vecMultiTex);

	vecMultiTex.clear();

	vecMultiTex.push_back(L"Terrain/BD_Terrain_Cave_01_SPC.bmp");
	vecMultiTex.push_back(L"Terrain/Terrain_Cliff_11_SPEC.bmp");
	vecMultiTex.push_back(L"Terrain/Terrain_Cliff_13_SPEC.bmp");
	vecMultiTex.push_back(L"Terrain/Terrain_Cliff_15_Large_SPEC.bmp");

	pTerrain->SetSplatSpecularTexture("TerrainSplatSpecular", vecMultiTex);

	vecMultiTex.clear();

	vecMultiTex.push_back(L"Terrain/RoadAlpha.bmp");
	vecMultiTex.push_back(L"Terrain/SandBaseAlpha.bmp");
	vecMultiTex.push_back(L"Terrain/StonAlpha.bmp");
	vecMultiTex.push_back(L"Terrain/WaterBaseAlpha.bmp");

	pTerrain->SetSplatAlphaTexture("TerrainSplatAlpha", vecMultiTex);
	pTerrain->SetSplatCount(vecMultiTex.size());
	vecMultiTex.clear();

	SAFE_RELEASE(pTerrain);

	pMapLayer->AddObject(pTerrainObject);
	SAFE_RELEASE(pTerrainObject);
	SAFE_RELEASE(pMapLayer);
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
	CreateTerrain();
	CreateCheckBox();
	CreateRadioButton();
	CreateHpBar();
	//CreateInventory();	

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
	
	SAFE_RELEASE(pCamera);
	SAFE_RELEASE(pCameraObject);

	//int num = 10;
	//while (0 <= num)
	//{
	//	m_fRespawnTime -= m_fRespawnLimitTime;

	//	DxVector3	vPos = DxVector3(rand() % 6 - 2.5f, rand() % 4 - 1.5f, rand() % 8 + 3.f);

	//	CGameObject*	pMinionObj = CGameObject::CreateClone("MinionObject");

	//	CTransform*	pTransform = pMinionObj->GetTransform();
	//	pTransform->SetWorldPos(vPos);
	//	SAFE_RELEASE(pTransform);

	//	CMinion*	pMinion = pMinionObj->FindComponentFromTypeID<CMinion>();
	//	pMinion->SetPlayer(m_pPlayerObject);
	//	SAFE_RELEASE(pMinion);

	//	CLayer*	pLayer = m_pScene->FindLayer(DEFAULTLAYER);
	//	pLayer->AddObject(pMinionObj);
	//	SAFE_RELEASE(pLayer);

	//	SAFE_RELEASE(pMinionObj);

	//	--num;
	//}
	/*CGameObject*	pObejct = CGameObject::CreateClone("WareHouseObject");
	pLayer->AddObject(pObejct);
	CTransform*		pTransform = pObejct->GetTransform();
	pTransform->SetWorldScale(0.01f, 0.01f, 0.01f);
	pTransform->SetWorldPos(5.0f, 0.0f, 5.0f);
	SAFE_RELEASE(pTransform);	
	SAFE_RELEASE(pObejct);

	pObejct = CGameObject::CreateClone("BurnHouse1Object");
	pLayer->AddObject(pObejct);
	pTransform = pObejct->GetTransform();
	pTransform->SetWorldScale(0.01f, 0.01f, 0.01f);
	pTransform->SetWorldPos(-5.0f, 0.0f, 5.0f);
	SAFE_RELEASE(pTransform);
	SAFE_RELEASE(pObejct);

	pObejct = CGameObject::CreateClone("BurnHouse2Object");
	pLayer->AddObject(pObejct);
	pTransform = pObejct->GetTransform();
	pTransform->SetWorldScale(0.01f, 0.01f, 0.01f);
	pTransform->SetWorldPos(0.0f, 0.0f, 20.0f);
	SAFE_RELEASE(pTransform);
	SAFE_RELEASE(pObejct);

	pObejct = CGameObject::CreateClone("BurnHouse3Object");
	pLayer->AddObject(pObejct);
	pTransform = pObejct->GetTransform();
	pTransform->SetWorldScale(0.01f, 0.01f, 0.01f);
	pTransform->SetWorldPos(-10.0f, 0.0f, 10.0f);
	SAFE_RELEASE(pTransform);
	SAFE_RELEASE(pObejct);

	pObejct = CGameObject::CreateClone("BurnHouse4Object");
	pLayer->AddObject(pObejct);
	pTransform = pObejct->GetTransform();
	pTransform->SetWorldScale(0.01f, 0.01f, 0.01f);
	pTransform->SetWorldPos(15.0f, 0.0f, 15.0f);
	SAFE_RELEASE(pTransform);
	SAFE_RELEASE(pObejct);

	pObejct = CGameObject::CreateClone("BurnHouse5Object");
	pLayer->AddObject(pObejct);
	pTransform = pObejct->GetTransform();
	pTransform->SetWorldScale(0.01f, 0.01f, 0.01f);
	pTransform->SetWorldPos(-15.0f, 0.0f, 15.0f);
	SAFE_RELEASE(pTransform);
	SAFE_RELEASE(pObejct);

	pObejct = CGameObject::CreateClone("SakuraObject");
	pLayer->AddObject(pObejct);
	pTransform = pObejct->GetTransform();
	pTransform->SetWorldScale(0.1f, 0.1f, 0.1f);
	pTransform->SetWorldPos(-20.0f, 0.0f, 0.0f);
	SAFE_RELEASE(pTransform);
	SAFE_RELEASE(pObejct);

	pObejct = CGameObject::CreateClone("InsamObject");
	pLayer->AddObject(pObejct);
	pTransform = pObejct->GetTransform();
	pTransform->SetWorldScale(0.1f, 0.1f, 0.1f);
	pTransform->SetWorldPos(20.0f, 5.0f, 0.0f);
	SAFE_RELEASE(pTransform);
	SAFE_RELEASE(pObejct);

	pObejct = CGameObject::CreateClone("ReturnPointObject");
	pLayer->AddObject(pObejct);
	pTransform = pObejct->GetTransform();
	pTransform->SetWorldScale(0.1f, 0.1f, 0.1f);
	pTransform->SetWorldPos(-30.0f, 0.0f, 30.0f);
	SAFE_RELEASE(pTransform);
	SAFE_RELEASE(pObejct);*/

	SAFE_RELEASE(pLayer);

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
