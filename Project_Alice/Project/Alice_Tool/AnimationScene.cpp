#include "stdafx.h"
#include "AnimationScene.h"
#include "05.Scene/Scene.h"
#include "06.GameObject/GameObject.h"
#include "07.Component/Transform.h"
#include "07.Component/CameraFree.h"

bool CAnimationScene::Init()
{
	// 카메라 오브젝트 생성
	CGameObject*	pCameraObject = m_pScene->GetMainCameraObject();

	CTransform*		pCameraTransform = m_pScene->GetMainCameraTransform();
	pCameraTransform->SetWorldPos(0.0f, 0.0f, -10.0f);
	SAFE_RELEASE(pCameraTransform);

	CCameraFree*	pCameraFree = pCameraObject->AddComponent<CCameraFree>("CameraFree");
	SAFE_RELEASE(pCameraFree);

	SAFE_RELEASE(pCameraObject);

	return true;
}

void CAnimationScene::Update(float _fTime)
{
}

CAnimationScene::CAnimationScene()
{
}

CAnimationScene::~CAnimationScene()
{
}
