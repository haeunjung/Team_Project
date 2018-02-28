#include "PlayerBullet.h"
#include "06.GameObject/GameObject.h"
#include "07.Component/Renderer.h"
#include "07.Component/Transform.h"
#include "07.Component/ColliderSphere.h"

bool CPlayerBullet::Init()
{
	m_pTransform->SetWorldScale(0.5f, 0.5f, 0.5f);

	CRenderer*	pRenderer = m_pGameObject->AddComponent<CRenderer>("Renderer");
	pRenderer->SetMesh("ColorPyramid");
	pRenderer->SetShader("StandardColorShader");
	pRenderer->SetInputLayout("ColorInputLayout");
	//pRenderer->SetRenderState(WIRE_FRAME);
	SAFE_RELEASE(pRenderer);	

	CColliderSphere*	pSphere = m_pGameObject->AddComponent<CColliderSphere>("PlayerBullet");
	pSphere->SetSphereInfo(Vec3Zero, 0.25f);	
	SAFE_RELEASE(pSphere);

	return true;
}

void CPlayerBullet::Update(float _fTime)
{
	m_pTransform->Up(m_fSpeed, _fTime);
	
	m_fDist -= m_fSpeed * _fTime;
	if (0.0f >= m_fDist)
	{
		m_pGameObject->Death();
	}
}

CPlayerBullet * CPlayerBullet::Clone()
{
	return new CPlayerBullet(*this);
}

void CPlayerBullet::OnCollisionEnter(CCollider * _pSrc, CCollider * _pDest, float _fTime)
{
	//MessageBox(NULL, L"ÃÑ¾ËÃæµ¹", L"ÃÑ¾ËÃæµ¹", MB_OK);
	if ("Minion" == _pDest->GetTagStr())
	{
		m_pGameObject->Death();
	}
}

CPlayerBullet::CPlayerBullet() :
	m_fSpeed(5.0f),
	m_fDist(20.0f)	
{
	SetTypeID<CPlayerBullet>();
	SetTypeName("CPlayerBullet");
	SetTag("PlayerBullet");
}

CPlayerBullet::CPlayerBullet(const CPlayerBullet & _PlayerBullet) :
	CScript(_PlayerBullet)
{	
	m_fSpeed = _PlayerBullet.m_fSpeed;
	m_fDist = _PlayerBullet.m_fDist;
}

CPlayerBullet::~CPlayerBullet()
{			
}
