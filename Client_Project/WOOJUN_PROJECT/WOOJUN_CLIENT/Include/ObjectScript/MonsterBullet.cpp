#include "MonsterBullet.h"
#include "07.Component/Renderer.h"
#include "07.Component/Transform.h"
#include "07.Component/ColliderSphere.h"

void CMonsterBullet::SetPlayer(CGameObject * _pPlayer)
{
	SAFE_RELEASE(m_pPlayer);
	_pPlayer->AddRef();
	m_pPlayer = _pPlayer;
}

bool CMonsterBullet::Init()
{	
	m_pTransform->SetWorldScale(0.5f, 0.5f, 0.5f);

	CRenderer*	pBulletRenderer = m_pGameObject->AddComponent<CRenderer>("Renderer");
	pBulletRenderer->SetMesh("ColorPyramid");
	pBulletRenderer->SetShader("StandardColorShader");
	pBulletRenderer->SetInputLayout("ColorInputLayout");
	SAFE_RELEASE(pBulletRenderer);

	CColliderSphere*	pSphere = m_pGameObject->AddComponent<CColliderSphere>("MonsterBullet");
	pSphere->SetSphereInfo(Vec3Zero, 0.25f);
	SAFE_RELEASE(pSphere);

	return true;
}

void CMonsterBullet::Update(float _fTime)
{
	m_pTransform->LookAt(m_pPlayer, AXIS_Y);
	m_pTransform->Up(m_fSpeed, _fTime);
	
	CTransform*	pPlayerTransform = m_pPlayer->GetTransform();
	DxVector3	vPlayerPos = pPlayerTransform->GetWorldPos();

	float	fLength = m_pTransform->GetWorldPos().Distance(vPlayerPos);
	if (0.5f >= fLength)
	{
		m_pGameObject->Death();		
	}

	SAFE_RELEASE(pPlayerTransform);
}

CMonsterBullet * CMonsterBullet::Clone()
{
	return new CMonsterBullet(*this);
}

void CMonsterBullet::OnCollisionEnter(CCollider * _pSrc, CCollider * _pDest, float _fTime)
{
	if ("Player" == _pDest->GetTagStr())
	{
		m_pGameObject->Death();
	}
}

CMonsterBullet::CMonsterBullet() :
	m_pPlayer(NULL),
	m_fSpeed(5.0f),
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
	m_pPlayer = NULL;
	m_fSpeed = _MonsterBullet.m_fSpeed;
	m_fDist = _MonsterBullet.m_fDist;
	m_fTime = _MonsterBullet.m_fTime;
}

CMonsterBullet::~CMonsterBullet()
{		
	SAFE_RELEASE(m_pPlayer);
}
