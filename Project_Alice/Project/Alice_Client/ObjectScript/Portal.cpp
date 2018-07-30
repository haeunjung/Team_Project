#include "Portal.h"
#include "05.Scene/SceneMgr.h"
#include "05.Scene/Scene.h"
#include "06.GameObject/GameObject.h"
#include "07.Component/Transform.h"
#include "07.Component/Renderer.h"
#include "07.Component/ColliderAABB.h"
#include "07.Component/ColliderSphere.h"
#include "../SceneScript/MainScene2.h"
#include "../ClientMgr/StageMgr.h"

CPortal::CPortal()
{
	SetTag("Portal");
	SetTypeName("CPortal");
	SetTypeID<CPortal>();
}

CPortal::~CPortal()
{
}

bool CPortal::Init()
{
	m_pTransform->SetWorldScale(0.02f, 0.02f, 0.02f);
	//m_pTransform->SetWorldPos(65.0f, 7.0f, 55.0f);

	CRenderer* pRenderer = m_pGameObject->AddComponent<CRenderer>("PortalRenderer");
	pRenderer->SetMesh("Portal", "Portal.FBX");
	pRenderer->SetShader(STANDARD_BUMP_SHADER);
	pRenderer->SetInputLayout("BumpInputLayout");
	SAFE_RELEASE(pRenderer);

	return true;
}

void CPortal::Update(float _fTime)
{
}

void CPortal::OnCollisionEnter(CCollider * _pSrc, CCollider * _pDest, float _fTime)
{
	if (CC_PLAYER_HIT == _pDest->GetColliderCheck()
		&& Vec3Zero != ((CColliderAABB*)_pSrc)->GetAABBInfo().vCenter)
	{
		StageChange();
	}
}

void CPortal::StageChange()
{
	GET_SINGLE(CStageMgr)->NextStage();
}

/*
스테이지 이동 시
유아이가 완전 초기화 되어야 하나??
UIMgr를 생각해 볼 필요가 있다.
*/
