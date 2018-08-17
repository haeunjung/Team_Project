#include "MonsterBullet.h"
#include "06.GameObject/GameObject.h"
#include "07.Component/Renderer.h"
#include "07.Component/Transform.h"
#include "07.Component/ColliderSphere.h"
#include "07.Component/Effect.h"
#include "07.Component/Animation2D.h"

void CMonsterBullet::SetPlayerTransform(CTransform* _pPlayerTransform)
{
	SAFE_RELEASE(m_pPlayerTransform);
	_pPlayerTransform->AddRef();
	m_pPlayerTransform = _pPlayerTransform;
}

bool CMonsterBullet::Init()
{	
	CRenderer* pRenderer = m_pGameObject->AddComponent<CRenderer>("EffectRenderer");
	pRenderer->SetMesh("PosMesh");
	pRenderer->SetShader(EFFECT_SHADER);
	pRenderer->SetInputLayout("PosInputLayout");
	pRenderer->SetRenderState(ALPHABLEND);
	SAFE_RELEASE(pRenderer);

	CEffect*	pEffect = m_pGameObject->AddComponent<CEffect>("Effect");
	SAFE_RELEASE(pEffect);

	CAnimation2D*	pAnimation2D = m_pGameObject->AddComponent<CAnimation2D>("EffectAnimation");
	pAnimation2D->AddAnimation2DClip("EnergyBall", A2D_ATLAS, AO_LOOP,
		8, 1, 1.0f, 0, 0.f, "EnergyBall", 0, L"Effect/EnergyBall.png");
	pAnimation2D->Start();
	SAFE_RELEASE(pAnimation2D);

	m_pTransform->SetWorldScale(2.5f, 2.5f, 2.5f);

	CColliderSphere*	pSphere = m_pGameObject->AddComponent<CColliderSphere>("MonsterBullet");
	pSphere->SetColCheck(CC_BULLET);
	pSphere->SetSphereInfo(m_pTransform->GetWorldPos(), 0.25f);
	SAFE_RELEASE(pSphere);

	return true;
}

void CMonsterBullet::Update(float _fTime)
{
	m_pTransform->LookAt(m_pPlayerTransform, AXIS_Y);
	m_pTransform->Up(m_fSpeed, _fTime);
}

CMonsterBullet * CMonsterBullet::Clone()
{
	return new CMonsterBullet(*this);
}

void CMonsterBullet::OnCollisionEnter(CCollider * _pSrc, CCollider * _pDest, float _fTime)
{
}

CMonsterBullet::CMonsterBullet() :
	m_pPlayerTransform(NULL),
	m_fSpeed(10.0f),
	m_fDist(3.0f),
	m_fTime(0.0f)
{
	SetTypeID<CMonsterBullet>();
	SetTypeName("CMonsterBullet");
	SetTag("MonsterBullet");
}

CMonsterBullet::CMonsterBullet(const CMonsterBullet & _MonsterBullet) :
	CScript(_MonsterBullet)
{	
	m_pPlayerTransform = NULL;
	m_fSpeed = _MonsterBullet.m_fSpeed;
	m_fDist = _MonsterBullet.m_fDist;
	m_fTime = _MonsterBullet.m_fTime;
}

CMonsterBullet::~CMonsterBullet()
{		
	SAFE_RELEASE(m_pPlayerTransform);
}
