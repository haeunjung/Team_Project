#include "stdafx.h"
#include "ToolScene.h"
#include "ToolValue.h"
#include "05.Scene/Scene.h"
#include "05.Scene/Layer.h"
#include "06.GameObject/GameObject.h"
#include "07.Component/Transform.h"
#include "07.Component/Renderer.h"
#include "07.Component/Camera.h"
#include "07.Component/CameraArm.h"
#include "07.Component/Terrain.h"
#include "07.Component/CameraFree.h"
#include "07.Component/ColliderSphere.h"

void CToolScene::CreateTerrain()
{
	CLayer*	pMapLayer = m_pScene->FindLayer(MAPLAYER);

	CGameObject*	pTerrainObject = CGameObject::Create("TerrainObject");

	CTransform*		pTransform = pTerrainObject->GetTransform();	
	SAFE_RELEASE(pTransform);

	CTerrain*	pTerrain = pTerrainObject->AddComponent<CTerrain>("Terrain");
	pTerrain->CreateTerrain("Terrain", 129, 129, 1, 1, "Terrain/Height1.bmp");
	pTerrain->SetBaseTexture("TerrainDiffuse", L"Terrain/Terrain_Pebbles_01.dds");
	pTerrain->SetNormalTexture("TerrainNormal", L"Terrain/Terrain_Pebbles_01_NRM.bmp");
	pTerrain->SetSpecularTexture("TerrainSpc", L"Terrain/Terrain_Pebbles_01_SPEC.bmp");	

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

bool CToolScene::Init()
{
	CreateTerrain();

	CLayer*		pLayer = m_pScene->FindLayer(DEFAULTLAYER);

	// 카메라 오브젝트 생성
	CGameObject*	pCameraObject = m_pScene->GetMainCameraObject();
	CTransform*		pCameraTransform = m_pScene->GetMainCameraTransform();
	pCameraTransform->SetWorldPos(0.0f, 0.0f, -10.0f);
	SAFE_RELEASE(pCameraTransform);

	CCameraFree*	pCameraFree = pCameraObject->AddComponent<CCameraFree>("CameraFree");
	SAFE_RELEASE(pCameraFree);

	// 플레이어
	CGameObject*		pPlayerObject = CGameObject::Create("PlayerObject");
	pLayer->AddObject(pPlayerObject);
	SAFE_RELEASE(pLayer);

	//CCamera*	pCamera = m_pScene->GetMainCamera();	
	//pCamera->Attach(pPlayerObject, DxVector3(0.0f, 0.0f, -5.0f));	
	//SAFE_RELEASE(pCamera);
	SAFE_RELEASE(pCameraObject);

	CTransform*		pTransform = pPlayerObject->GetTransform();	
	pTransform->SetWorldPos(1.0f, 1.0f, 0.0f);
	pTransform->SetWorldScale(0.01f, 0.01f, 0.01f);
	SAFE_RELEASE(pTransform);

	CRenderer* pPlayerRenderer = pPlayerObject->AddComponent<CRenderer>("Renderer");
	pPlayerRenderer->SetMesh("PlayerMesh", L"Warehouse01.FBX");
	pPlayerRenderer->SetShader(STANDARD_BUMP_SHADER);
	pPlayerRenderer->SetInputLayout("BumpInputLayout");
	pPlayerRenderer->SetRenderState(ALPHABLEND);
	SAFE_RELEASE(pPlayerRenderer);	

	CColliderSphere*	pSphere = pPlayerObject->AddComponent<CColliderSphere>("Collider");
	pSphere->SetSphereInfo(Vec3Zero, 1.0f);
	SAFE_RELEASE(pSphere);

	pPlayerObject->AddRef();
	GET_SINGLE(CToolValue)->pPlayerObj = pPlayerObject;
	SAFE_RELEASE(pPlayerObject);

	return true;
}

void CToolScene::Update(float _fTime)
{
	int a = 0;
}

CToolScene::CToolScene()
{
}


CToolScene::~CToolScene()
{
}
