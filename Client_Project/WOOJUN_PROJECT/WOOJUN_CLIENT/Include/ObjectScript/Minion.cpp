#include "Minion.h"
#include "07.Component/Renderer.h"
#include "07.Component/Transform.h"
#include "05.Scene/Layer.h"
#include "MonsterBullet.h"
#include "PlayerBullet.h"
#include "07.Component/ColliderSphere.h"

void CMinion::SetPlayer(CGameObject * _pPlayer)
{
	SAFE_RELEASE(m_pPlayer);
	_pPlayer->AddRef();
	m_pPlayer = _pPlayer;
}

void CMinion::BulletFire()
{
	// Bullet Object CreateClone
	CGameObject*	pBulletObject = CGameObject::CreateClone("MonsterBulletObject");
	// Bullet WorldPos Setting
	// GetTransform() : pBulletObject�� Transform�� �����ͼ�
	// �ٵ� �갡 AddRef�� �ϳ�
	// SetWorldPos() : pBulletObject�� WorldPos�� ����
	CTransform*		pTransform = pBulletObject->GetTransform();
	pTransform->SetWorldPos(m_pTransform->GetWorldPos());
	pTransform->SetWorldRot(m_pTransform->GetWorldRot());
	SAFE_RELEASE(pTransform);

	CMonsterBullet*	pBullet = pBulletObject->FindComponentFromTypeID<CMonsterBullet>();
	pBullet->SetPlayer(m_pPlayer);
	SAFE_RELEASE(pBullet);

	// ���� Layer�� �߰�
	m_pLayer->AddObject(pBulletObject);
	SAFE_RELEASE(pBulletObject);

	m_fTime = 0.0f;
}

bool CMinion::Init()
{	
	// ������ ����
	CRenderer*	pRenderer = m_pGameObject->AddComponent<CRenderer>("Renderer");
	pRenderer->SetMesh("ColorPyramid");
	pRenderer->SetShader("StandardColorShader");
	pRenderer->SetInputLayout("ColorInputLayout");
	SAFE_RELEASE(pRenderer);

	CColliderSphere*	pSphere = m_pGameObject->AddComponent<CColliderSphere>("Minion");
	pSphere->SetSphereInfo(Vec3Zero, 0.5f);
	SAFE_RELEASE(pSphere);

	return true;
}

void CMinion::Update(float _fTime)
{
	m_pTransform->LookAt(m_pPlayer, AXIS_Y);

	/*m_fTime += _fTime;
	if (3.0f <= m_fTime)
	{
		BulletFire();
	}*/
}

CMinion * CMinion::Clone()
{
	return new CMinion(*this);
}

void CMinion::OnCollisionEnter(CCollider * _pSrc, CCollider * _pDest, float _fTime)
{		
	if ("PlayerBullet" == _pDest->GetTagStr())
	{
		m_pGameObject->Death();		
	}
}

CMinion::CMinion() :
	m_pPlayer(NULL),
	m_fTime(0.0f)
{
	SetTypeID<CMinion>();
}

CMinion::CMinion(const CMinion & _Minion) :
	CScript(_Minion)
{
	m_pPlayer = NULL;
	m_fTime = _Minion.m_fTime;
}

CMinion::~CMinion()
{		
	SAFE_RELEASE(m_pPlayer);
}
