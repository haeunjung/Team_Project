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
#include "07.Component/PointLight.h"
#include "07.Component/SpotParent.h"

void CToolScene::CreateTerrain()
{
	CLayer*	pMapLayer = m_pScene->FindLayer(MAPLAYER);

	CGameObject*	pTerrainObject = CGameObject::Create("TerrainObject");

	CTransform*		pTransform = pTerrainObject->GetTransform();
	//pTransform->SetWorldPos(-32.0f, -1.0f, -32.0f);
	SAFE_RELEASE(pTransform);

	CTerrain*	pTerrain = pTerrainObject->AddComponent<CTerrain>("Terrain");
	pTerrain->CreateTerrain("Terrain", 3, 2, 64, 64/*, "Terrain/Height1.bmp"*/);
	pTerrain->SetBaseTexture("TerrainDiffuse", L"Terrain/TexturesCom_WoodFine0032_2_seamless_S_COLOR.png");
	pTerrain->SetNormalTexture("TerrainNormal", L"Terrain/TexturesCom_WoodFine0032_2_seamless_S_NRM.png");
	pTerrain->SetSpecularTexture("TerrainSpc", L"Terrain/TexturesCom_WoodFine0032_2_seamless_S_SPEC.png");
	SAFE_RELEASE(pTerrain);

	pMapLayer->AddObject(pTerrainObject);
	SAFE_RELEASE(pTerrainObject);
	SAFE_RELEASE(pMapLayer);
}

void CToolScene::CreateLignt()
{
	CGameObject* pLightObject = m_pScene->CreateLight("PointLight1", LT_POINT);

	CTransform* pTransform = pLightObject->GetTransform();
	pTransform->SetWorldPos(15.f, 7.0f, 15.f);
	SAFE_RELEASE(pTransform);

	CPointLight* pPointLight = (CPointLight*)pLightObject->FindComponentFromType(CT_LIGHT);

	LIGHTINFO	tLightInfo = {};
	tLightInfo.eType = LT_POINT;
	tLightInfo.vDiffuse = { 1.0f, 1.0f, 1.0f, 1.f };
	tLightInfo.vAmbient = { 0.2f, 0.2f, 0.2f, 1.f };
	tLightInfo.vSpecular = { 0.2f, 0.2f, 0.2f, 1.f };
	tLightInfo.vAttenuation = DxVector3(0.0f, 0.2f, 0.0f);

	pPointLight->SetLightInfo(tLightInfo);
	SAFE_RELEASE(pPointLight);
	SAFE_RELEASE(pLightObject);

	pLightObject = m_pScene->CreateLight("PointLigh2", LT_POINT);

	pTransform = pLightObject->GetTransform();
	pTransform->SetWorldPos(15.f, 7.0f, 50.f);
	SAFE_RELEASE(pTransform);

	pPointLight = (CPointLight*)pLightObject->FindComponentFromType(CT_LIGHT);
	pPointLight->SetLightInfo(tLightInfo);
	SAFE_RELEASE(pPointLight);
	SAFE_RELEASE(pLightObject);

	pLightObject = m_pScene->CreateLight("PointLigh3", LT_POINT);

	pTransform = pLightObject->GetTransform();
	pTransform->SetWorldPos(45.f, 10.0f, 27.f);
	SAFE_RELEASE(pTransform);

	pPointLight = (CPointLight*)pLightObject->FindComponentFromType(CT_LIGHT);
	pPointLight->SetLightInfo(tLightInfo);
	SAFE_RELEASE(pPointLight);
	SAFE_RELEASE(pLightObject);

	pLightObject = m_pScene->CreateLight("PointLigh4", LT_POINT);

	pTransform = pLightObject->GetTransform();
	pTransform->SetWorldPos(75.0f, 7.0f, 50.f);
	SAFE_RELEASE(pTransform);

	pPointLight = (CPointLight*)pLightObject->FindComponentFromType(CT_LIGHT);
	pPointLight->SetLightInfo(tLightInfo);
	SAFE_RELEASE(pPointLight);
	SAFE_RELEASE(pLightObject);

	pLightObject = m_pScene->CreateLight("PointLig5", LT_POINT);

	pTransform = pLightObject->GetTransform();
	pTransform->SetWorldPos(70.0f, 7.0f, 5.f);
	SAFE_RELEASE(pTransform);

	pPointLight = (CPointLight*)pLightObject->FindComponentFromType(CT_LIGHT);
	pPointLight->SetLightInfo(tLightInfo);
	SAFE_RELEASE(pPointLight);
	SAFE_RELEASE(pLightObject);

	tLightInfo.eType = LT_SPOTPARENT;
	tLightInfo.vDiffuse = { 0.0f, 0.0f, 0.0f, 1.f };
	tLightInfo.vAmbient = { 1.0f, 0.5f, 0.0f, 1.f };
	tLightInfo.vSpecular = { 1.0f, 0.5f, 0.0f, 1.f };
	tLightInfo.vAttenuation = DxVector3(1.0f, 0.0f, 0.0f);

	CLayer* pLayer = m_pScene->FindLayer(DEFAULTLAYER);

	pLightObject = m_pScene->CreateLight("SpotLight1", LT_SPOT);

	pTransform = pLightObject->GetTransform();
	pTransform->SetWorldPos(40.0f, 10.0f, 15.0f);
	SAFE_RELEASE(pTransform);

	CSpotParent* pSpotLight = (CSpotParent*)pLightObject->FindComponentFromType(CT_LIGHT);
	pSpotLight->SetLightInfo(tLightInfo);
	pSpotLight->InitChildSpotLight();
	SAFE_RELEASE(pSpotLight);

	pLayer->AddObject(pLightObject);
	SAFE_RELEASE(pLightObject);

	pLightObject = m_pScene->CreateLight("SpotLight2", LT_SPOT);

	pTransform = pLightObject->GetTransform();
	pTransform->SetWorldPos(50.0f, 10.0f, 40.0f);
	SAFE_RELEASE(pTransform);

	pSpotLight = (CSpotParent*)pLightObject->FindComponentFromType(CT_LIGHT);
	pSpotLight->SetLightInfo(tLightInfo);
	pSpotLight->InitChildSpotLight();
	SAFE_RELEASE(pSpotLight);

	pLayer->AddObject(pLightObject);
	SAFE_RELEASE(pLightObject);

	pLightObject = m_pScene->CreateLight("SpotLight3", LT_SPOT);

	pTransform = pLightObject->GetTransform();
	pTransform->SetWorldPos(10.0f, 10.0f, 20.0f);
	SAFE_RELEASE(pTransform);

	pSpotLight = (CSpotParent*)pLightObject->FindComponentFromType(CT_LIGHT);
	pSpotLight->SetLightInfo(tLightInfo);
	pSpotLight->InitChildSpotLight();
	SAFE_RELEASE(pSpotLight);

	pLayer->AddObject(pLightObject);
	SAFE_RELEASE(pLightObject);

	pLightObject = m_pScene->CreateLight("SpotLight4", LT_SPOT);

	pTransform = pLightObject->GetTransform();
	pTransform->SetWorldPos(77.0f, 10.0f, 20.0f);
	SAFE_RELEASE(pTransform);

	pSpotLight = (CSpotParent*)pLightObject->FindComponentFromType(CT_LIGHT);
	pSpotLight->SetLightInfo(tLightInfo);
	pSpotLight->InitChildSpotLight();
	SAFE_RELEASE(pSpotLight);

	pLayer->AddObject(pLightObject);
	SAFE_RELEASE(pLightObject);

	SAFE_RELEASE(pLayer);
}

bool CToolScene::Init()
{
	CreateTerrain();
	//CreateLignt();

	CLayer*		pLayer = m_pScene->FindLayer(DEFAULTLAYER);

	// 카메라 오브젝트 생성
	CGameObject*	pCameraObject = m_pScene->GetMainCameraObject();
	CTransform*		pCameraTransform = m_pScene->GetMainCameraTransform();
	pCameraTransform->SetWorldPos(TERRAINSIZE * 0.5f, 10.0f, -10.0f);
	SAFE_RELEASE(pCameraTransform);

	CCameraFree*	pCameraFree = pCameraObject->AddComponent<CCameraFree>("CameraFree");
	SAFE_RELEASE(pCameraFree);

	SAFE_RELEASE(pLayer);

	SAFE_RELEASE(pCameraObject);

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
