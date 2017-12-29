#include "stdafx.h"
#include "ToolScene.h"

#include "05.Scene/Scene.h"
#include "05.Scene/Layer.h"
#include "06.GameObject/GameObject.h"
#include "07.Component/Transform.h"
#include "07.Component/Renderer.h"
#include "07.Component/Camera.h"
#include "07.Component/CameraArm.h"

#include "ToolValue.h"

bool CToolScene::Init()
{
	CLayer*		pLayer = m_pScene->FindLayer(DEFAULTLAYER);

	// 카메라 오브젝트 생성
	CGameObject*	pCameraObject = m_pScene->GetMainCameraObject();
	CTransform*		pCameraTransform = m_pScene->GetMainCameraTransform();
	pCameraTransform->SetWorldPos(0.0f, 0.0f, -10.0f);
	SAFE_RELEASE(pCameraTransform);

	/*CCameraArm*	pCameraArm = pCameraObject->AddComponent<CCameraArm>("CameraArm");
	SAFE_RELEASE(pCameraArm);*/

	// 플레이어
	CGameObject*		pPlayerObject = CGameObject::Create("PlayerObject");
	pLayer->AddObject(pPlayerObject);
	SAFE_RELEASE(pLayer);

	CCamera*	pCamera = m_pScene->GetMainCamera();	
	//pCamera->Attach(pPlayerObject, DxVector3(0.0f, 0.0f, -5.0f));	
	SAFE_RELEASE(pCamera);
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
