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
	//pTransform->SetWorldPos(-32.0f, -1.0f, -32.0f);
	SAFE_RELEASE(pTransform);

	CTerrain*	pTerrain = pTerrainObject->AddComponent<CTerrain>("Terrain");
	pTerrain->CreateTerrain("Terrain", TERRAINSIZE + 5, TERRAINSIZE, 4, 4/*, "Terrain/Height1.bmp"*/);
	pTerrain->SetBaseTexture("TerrainDiffuse", L"Terrain/TexturesCom_PlywoodNew0050_1_seamless_S_COLOR.png");
	pTerrain->SetNormalTexture("TerrainNormal", L"Terrain/TexturesCom_PlywoodNew0050_1_seamless_S_NRM.png");
	pTerrain->SetSpecularTexture("TerrainSpc", L"Terrain/TexturesCom_PlywoodNew0050_1_seamless_S_SPEC.png");
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
	pCameraTransform->SetWorldPos(TERRAINSIZE * 0.5f, 10.0f, -10.0f);
	SAFE_RELEASE(pCameraTransform);

	CCameraFree*	pCameraFree = pCameraObject->AddComponent<CCameraFree>("CameraFree");
	SAFE_RELEASE(pCameraFree);
	
	//// 플레이어
	//CGameObject*		pPlayerObject = CGameObject::Create("PlayerObject");
	//pLayer->AddObject(pPlayerObject);
	SAFE_RELEASE(pLayer);

	////CCamera*	pCamera = m_pScene->GetMainCamera();	
	////pCamera->Attach(pPlayerObject, DxVector3(0.0f, 0.0f, -5.0f));	
	////SAFE_RELEASE(pCamera);
	SAFE_RELEASE(pCameraObject);

	//CTransform*		pTransform = pPlayerObject->GetTransform();	
	//pTransform->SetWorldPos(1.0f, 1.0f, 0.0f);
	//pTransform->SetWorldScale(0.01f, 0.01f, 0.01f);
	//SAFE_RELEASE(pTransform);

	//CRenderer* pPlayerRenderer = pPlayerObject->AddComponent<CRenderer>("Renderer");
	//pPlayerRenderer->SetMesh("PlayerMesh", L"Warehouse01.FBX");
	//pPlayerRenderer->SetShader(STANDARD_BUMP_SHADER);
	//pPlayerRenderer->SetInputLayout("BumpInputLayout");
	//pPlayerRenderer->SetRenderState(ALPHABLEND);
	//SAFE_RELEASE(pPlayerRenderer);	

	//CColliderSphere*	pSphere = pPlayerObject->AddComponent<CColliderSphere>("Collider");
	//pSphere->SetSphereInfo(Vec3Zero, 1.0f);
	//SAFE_RELEASE(pSphere);

	//pPlayerObject->AddRef();
	//GET_SINGLE(CToolValue)->pPlayerObj = pPlayerObject;
	//SAFE_RELEASE(pPlayerObject);

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
