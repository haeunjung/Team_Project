#include "MainScene.h"
#include "../ObjectScript/Player.h"
#include "../ObjectScript/MonsterBullet.h"
#include "../ObjectScript/Minion.h"
#include "../ObjectScript/RotBullet.h"
#include "../ObjectScript/PlayerBullet.h"
#include "01.Core/Debug.h"
#include "01.Core/Input.h"
#include "01.Core/PathMgr.h"
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
#include "07.Component/ColliderSphere.h"
#include "07.Component/ColliderAABB.h"
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
}

// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

void CMainScene::CreateObject()
{
	string	strPath = GET_SINGLE(CPathMgr)->FindPathToMultiByte(DATAPATH);
	strPath += "Stage1.Data";

	FILE*	pFile = NULL;
	fopen_s(&pFile, strPath.c_str(), "rb");

	if (!pFile)
	{
		return;
	}

	// Vector Size Load
	size_t Size = 0;
	fread(&Size, 4, 1, pFile);
	for (size_t i = 0; i < Size; ++i)
	{
		// 이름 로드
		int iLength = 0;
		fread(&iLength, 4, 1, pFile);
		char strKey[256] = {};
		fread(strKey, 1, iLength, pFile);

		// WorldPos 로드
		DxVector3	vPos;
		fread(&vPos, sizeof(DxVector3), 1, pFile);

		// WorldScale 로드
		DxVector3	vScale;
		fread(&vScale, sizeof(DxVector3), 1, pFile);

		// WorldRot 로드
		DxVector3	vRot;
		fread(&vRot, sizeof(DxVector3), 1, pFile);

		LoadObject(strKey, vPos, vScale, vRot);
	}

	fclose(pFile);
}

void CMainScene::LoadObject(const string & _strKey, const DxVector3 & _vPos, const DxVector3 & _vScale, const DxVector3 & _vRot)
 {
	CLayer*	pLayer = m_pScene->FindLayer(DEFAULTLAYER);
	
	CGameObject* pGameObject = CGameObject::Create(_strKey);
	
	CTransform* pTransform = pGameObject->GetTransform();
	pTransform->SetWorldPos(_vPos);
	pTransform->SetWorldScale(_vScale);
	pTransform->SetWorldRot(_vRot);

	//pTransform->SetLocalPos(-0.5f, 0.5f, -0.5f);
	SAFE_RELEASE(pTransform);
	
	/*CToolObject* pToolObject = pGameObject->AddComponent<CToolObject>(_strKey + "Object");
	SAFE_RELEASE(pToolObject);*/
	
	string	FileName = _strKey + ".FBX";
	
	CRenderer* pRenderer = pGameObject->AddComponent<CRenderer>(_strKey + "Renderer");
	pRenderer->SetMesh(_strKey, FileName.c_str());
	pRenderer->SetShader(STANDARD_BUMP_SHADER);
	pRenderer->SetInputLayout("BumpInputLayout");
	SAFE_RELEASE(pRenderer);
	
	//CColliderSphere* pColSphere = pGameObject->AddComponent<CColliderSphere>(_strKey + "ColSphere");
	//pColSphere->SetSphereInfo(_vPos, 1.0f);
	//SAFE_RELEASE(pColSphere);
	
	CColliderAABB* pColAABB = pGameObject->AddComponent<CColliderAABB>(_strKey + "ColAABB");
	pColAABB->SetColCheck(CC_OBJ);
	SAFE_RELEASE(pColAABB);

	pLayer->AddObject(pGameObject);
	
	SAFE_RELEASE(pGameObject);
	SAFE_RELEASE(pLayer);
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
	pTerrain->CreateTerrain("Terrain", TERRAINSIZE, TERRAINSIZE, 1, 1/*, "Terrain/Height1.bmp"*/);
	pTerrain->SetBaseTexture("TerrainDiffuse", L"Terrain/BD_Terrain_Cliff05.dds");
	pTerrain->SetNormalTexture("TerrainNormal", L"Terrain/BD_Terrain_Cliff05_NRM.bmp");
	pTerrain->SetSpecularTexture("TerrainSpc", L"Terrain/BD_Terrain_Cliff05_SPEC.bmp");
	/*pTerrain->SetBaseTexture("TerrainDiffuse", L"Terrain/BD_Terrain_Cave_01.dds");
	pTerrain->SetNormalTexture("TerrainNormal", L"Terrain/BD_Terrain_Cave_01_NRM.bmp");
	pTerrain->SetSpecularTexture("TerrainSpc", L"Terrain/BD_Terrain_Cave_01_SPC.bmp");*/

	vector<wstring>	vecMultiTex;

	vecMultiTex.push_back(L"Terrain/BD_Terrain_Cave_01.dds");
	//vecMultiTex.push_back(L"Terrain/Terrain_Cliff_11.dds");
	//vecMultiTex.push_back(L"Terrain/Terrain_Cliff_13.dds");
	//vecMultiTex.push_back(L"Terrain/Terrain_Cliff_15_Large.dds");

	pTerrain->SetSplatTexture("TerrainSplat", vecMultiTex);

	// Normal 세팅
	vecMultiTex.clear();

	vecMultiTex.push_back(L"Terrain/BD_Terrain_Cave_01_NRM.bmp");
	//vecMultiTex.push_back(L"Terrain/Terrain_Cliff_11_NRM.bmp");
	//vecMultiTex.push_back(L"Terrain/Terrain_Cliff_13_NRM.bmp");
	//vecMultiTex.push_back(L"Terrain/Terrain_Cliff_15_Large_NRM.bmp");

	pTerrain->SetSplatNormalTexture("TerrainSplatNormal", vecMultiTex);

	vecMultiTex.clear();

	vecMultiTex.push_back(L"Terrain/BD_Terrain_Cave_01_SPC.bmp");
	//vecMultiTex.push_back(L"Terrain/Terrain_Cliff_11_SPEC.bmp");
	//vecMultiTex.push_back(L"Terrain/Terrain_Cliff_13_SPEC.bmp");
	//vecMultiTex.push_back(L"Terrain/Terrain_Cliff_15_Large_SPEC.bmp");

	pTerrain->SetSplatSpecularTexture("TerrainSplatSpecular", vecMultiTex);

	vecMultiTex.clear();

	vecMultiTex.push_back(L"Terrain/RoadAlpha.bmp");
	//vecMultiTex.push_back(L"Terrain/SandBaseAlpha.bmp");
	//vecMultiTex.push_back(L"Terrain/StonAlpha.bmp");
	//vecMultiTex.push_back(L"Terrain/WaterBaseAlpha.bmp");

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

bool CMainScene::Init()
{
	CreateProtoType();
	//CreateObject();
	CreateTerrain();
	//CreateCheckBox();
	//CreateRadioButton();
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
	pCamera->Attach(pPlayerObject, DxVector3(0.0f, 0.0f, -2.5f));

	m_pPlayerObject = pPlayerObject;
	pPlayerObject->AddRef();
	
	CPlayer*	pPlayerScript = pPlayerObject->AddComponent<CPlayer>("PlayerScript");	
	SAFE_RELEASE(pPlayerScript);
	SAFE_RELEASE(pPlayerObject);			
	
	SAFE_RELEASE(pCamera);
	SAFE_RELEASE(pCameraObject);

	pCameraObject = m_pScene->CreateCamera("SubCamera"/*, DxVector3(0.0f, 0.0f, -5.0f)*/);
	SAFE_RELEASE(pCameraObject);

	/*CGameObject*	pMinionObj = CGameObject::CreateClone("MinionObject");

	CTransform*	pTransform = pMinionObj->GetTransform();
	pTransform->SetWorldPos(40.0f, 0.0f, 20.0f);
	SAFE_RELEASE(pTransform);

	CMinion*	pMinion = pMinionObj->FindComponentFromTypeID<CMinion>();
	pMinion->SetPlayer(m_pPlayerObject);
	SAFE_RELEASE(pMinion);*/

	CGameObject*	pMinionObj = CGameObject::Create("Minion");
	CMinion*	pMinion = pMinionObj->AddComponent<CMinion>("MinionScript");
	pMinion->SetMonsterWorldPos(DxVector3(40.0f, 0.0f, 20.0f));
	SAFE_RELEASE(pMinion);

	pLayer->AddObject(pMinionObj);
	SAFE_RELEASE(pMinionObj);

	CGameObject*	pMonsterObj = CGameObject::Create("Monster");
	CMinion*	pMonster = pMonsterObj->AddComponent<CMinion>("MinionScript");
	pMonster->SetMonsterWorldPos(DxVector3(30.0f, 0.0f, 30.0f));
	pMonster->SetIsTest(true);
	SAFE_RELEASE(pMonster);

	pLayer->AddObject(pMonsterObj);
	SAFE_RELEASE(pMonsterObj);

	SAFE_RELEASE(pLayer);

	// Test Box Mesh 생성
	//LoadObject("box", DxVector3(-1.0f, 0.0f, 1.0f), DxVector3(1.0f, 1.0f, 1.0f), DxVector3(PI, 0.0f, 0.0f));
	LoadObject("Box2", DxVector3(15.0f, 2.5f, 15.0f), DxVector3(5.0f, 5.0f, 5.0f), Vec3Zero);
	LoadObject("Box2", DxVector3(22.5f, 2.5f, 10.0f), DxVector3(10.0f, 5.0f, 15.0f), Vec3Zero);

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
	m_fRespawnLimitTime(5.0f)
{
	m_bCheck = true;
}

CMainScene::~CMainScene()
{
	SAFE_RELEASE(m_pPlayerObject);
}
