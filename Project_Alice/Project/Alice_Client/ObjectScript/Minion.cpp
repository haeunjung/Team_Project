#include "Minion.h"
#include "01.Core/Debug.h"
#include "07.Component/Renderer.h"
#include "07.Component/Transform.h"
#include "05.Scene/Layer.h"
#include "MonsterBullet.h"
#include "PlayerBullet.h"
#include "07.Component/ColliderSphere.h"
#include "07.Component/Collider.h"

void CMinion::SetPlayer(CGameObject * _pPlayer)
{
	SAFE_RELEASE(m_pPlayer);
	_pPlayer->AddRef();
	m_pPlayer = _pPlayer;
}

bool CMinion::Init()
{	
	CTransform*		pTransform = m_pGameObject->GetTransform();
	pTransform->SetWorldPos(40.0f, 0.0f, 20.0f);
	pTransform->SetWorldScale(0.05f, 0.05f, 0.05f);
	pTransform->SetLocalRotY(-PI * 0.5f);
	pTransform->SetWorldRotY(PI);

	CRenderer* pPlayerRenderer = m_pGameObject->AddComponent<CRenderer>("Renderer");
	pPlayerRenderer->SetMesh("MinionMesh", L"SmallMonster.msh");
	pPlayerRenderer->SetShader(STANDARD_ANI_BUMP_SHADER);
	pPlayerRenderer->SetInputLayout("AniBumpInputLayout");
	SAFE_RELEASE(pPlayerRenderer);

	m_pAniController = (CAnimation3D*)m_pGameObject->FindComponentFromType(CT_ANIMATION3D);
	//m_pAniController->AddClipCallback<CPlayer>("Run", 0.5f, this, &CPlayer::AniCallback);

	CColliderSphere*	pSphere = m_pGameObject->AddComponent<CColliderSphere>("MinionColSphere");
	pSphere->SetSphereInfo(Vector3(0.0f, 0.0f, 0.0f), 0.5f);
	pSphere->SetBoneIndex(m_pAniController->FindBoneIndex("Bone09"));
	pSphere->SetColCheck(CC_HIT);
	SAFE_RELEASE(pSphere);
	// Bone25
	// FxCenter

	DxVector3 Pos = pTransform->GetWorldPos();
	DxVector3 Forward = pTransform->GetWorldAxis(AXIS_Z);
	SAFE_RELEASE(pTransform);

	m_pViewCol = m_pGameObject->AddComponent<CColliderSphere>("MonsterViewCol");
	m_pViewCol->SetSphereInfo(Pos + Forward * 2.5f, 2.5f);
	m_pViewCol->SetColCheck(CC_VIEW);

	m_pAttCol = m_pGameObject->AddComponent<CColliderSphere>("MonsterAttCol");
	m_pAttCol->SetSphereInfo(Vector3(0.0f, 0.0f, 0.0f), 0.5f);
	m_pAttCol->SetBoneIndex(m_pAniController->FindBoneIndex("Bone16"));
	m_pAttCol->SetColCheck(CC_ATT);
	m_pAttCol->SetIsEnable(false);

	return true;
}

void CMinion::Update(float _fTime)
{
	switch (m_eMonsterState)
	{
	case MS_DEFAULT:
		MonsterIdle(_fTime);
		break;
	case MS_WALK:
		MonsterWalk(_fTime);
		break;
	case MS_TRACE:
		MonsterTrace(_fTime);
		break;
	case MS_ATTACK:
		MonsterAttack();
		break;
	case MS_DEATH:
		MonsterDeath();
		break;
	}

	//m_pTransform->LookAt(m_pPlayer, AXIS_Y);
}

CMinion * CMinion::Clone()
{
	return new CMinion(*this);
}

void CMinion::OnCollisionEnter(CCollider * _pSrc, CCollider * _pDest, float _fTime)
{		
	if (CC_VIEW == _pSrc->GetColliderCheck())
	{
		//if (CC_HIT == _pDest->GetColliderCheck())
		if("PlayerHit" == _pDest->GetTagStr())
		{
			SAFE_RELEASE(m_pPlayerTransform);
			m_pPlayerTransform = _pDest->GetTransform();
			m_fTime = 0.0f;
			m_eMonsterState = MS_TRACE;
		}
	}	
}

void CMinion::OnCollisionStay(CCollider * _pSrc, CCollider * _pDest, float _fTime)
{
	if (CC_HIT == _pSrc->GetColliderCheck())
	{
		//if (CC_ATT == _pDest->GetColliderCheck())
		if ("PlayerAtt" == _pDest->GetTagStr() &&
			true == _pDest->GetIsEnable())
		{
			if (true == BackAttackCheck(_pSrc->GetTransformWorldAxis(AXIS_Z), _pDest->GetTransformWorldAxis(AXIS_Z)))
			{
				m_fTime = 0.0f;
				_pDest->SetIsEnable(false);
				m_eMonsterState = MS_DEATH;
			}
		}
	}
}

bool CMinion::BackAttackCheck(const DxVector3 & _SrcForward, const DxVector3 & _DestForward)
{
	if (0.0f < _SrcForward.Dot(_DestForward))
	{
		return true;
	}
	return false;
}

void CMinion::MonsterIdle(float _fTime)
{
	// 일정 시간이 되면 Walk로 가자
	m_fTime += _fTime;

	if (3.0f <= m_fTime)
	{
		m_fTime = 0.0f;
		m_pTransform->RotateY(PI);

		// ViewCol 위치 갱신
		m_pViewCol->SetSphereInfo(m_pTransform->GetWorldPos() + m_pTransform->GetWorldAxis(AXIS_Z) * 2.5f, 2.5f);

		m_eMonsterState = MS_WALK;
	}
}

void CMinion::MonsterWalk(float _fTime)
{
	// 일정 시간이 되면 다시 Idle로 가자
	m_fTime += _fTime;

	if (true == m_pAniController->CheckClipName("Walk"))
	{
		if (3.0f <= m_fTime)
		{
			m_fTime = 0.0f;
			m_eMonsterState = MS_DEFAULT;;
		}

		m_pTransform->Forward(m_fSpeed, _fTime);
	}
	else
	{
		m_pAniController->ChangeClip("Walk");
	}
}

void CMinion::MonsterTrace(float _fTime)
{
	m_pTransform->LookAt(m_pPlayerTransform);
	m_pViewCol->SetSphereInfo(m_pTransform->GetWorldPos() + m_pTransform->GetWorldAxis(AXIS_Z) * 2.5f, 2.5f);

	if (true == m_pAniController->CheckClipName("Run"))
	{
		m_pTransform->Forward(m_fSpeed * 1.5f, _fTime);
	}
	else
	{
		m_pAniController->ChangeClip("Run");
	}

	m_fDist = m_pPlayerTransform->GetWorldPos().Distance(m_pTransform->GetWorldPos());
	if (1.0f >= m_fDist)
	{
		m_eMonsterState = MS_ATTACK;
	}
	else if (10.0f <= m_fDist)
	{
		m_eMonsterState = MS_DEFAULT;
	}
}

void CMinion::MonsterAttack()
{
	if (true == m_pAniController->CheckClipName("Attack"))
	{		
		if (40 == m_pAniController->GetAnimationProgressFrame())
		{
			m_pAttCol->SetIsEnable(true);
		}

		if (true == m_pAniController->GetAnimationEnd())
		{
			m_pAniController->ReturnToDefaultClip();
			m_pAttCol->SetIsEnable(false);
		}
	}
	else
	{
		m_pAniController->ChangeClip("Attack");
	}

	m_fDist = m_pPlayerTransform->GetWorldPos().Distance(m_pTransform->GetWorldPos());
	if (1.0f <= m_fDist)
	{
		m_eMonsterState = MS_TRACE;
	}
}

void CMinion::MonsterDeath()
{
	if (true == m_pAniController->CheckClipName("Death"))
	{
		if (true == m_pAniController->GetAnimationEnd())
		{
			m_pGameObject->Death();
		}
	}
	else
	{
		m_pAniController->ChangeClip("Death");
	}
}

CMinion::CMinion() :
	m_pPlayer(NULL),
	m_pPlayerTransform(NULL),
	m_pAniController(NULL),
	m_fSpeed(2.0f),
	m_fTime(0.0f),
	m_fDist(0.0f),
	m_pViewCol(NULL),
	m_pAttCol(NULL)
{
	SetTypeID<CMinion>();
	SetTypeName("CMinion");
}

CMinion::CMinion(const CMinion & _Minion) :
	CScript(_Minion)
{
	//m_pAniController
	m_pPlayer = NULL;
	m_fTime = _Minion.m_fTime;
}

CMinion::~CMinion()
{		
	SAFE_RELEASE(m_pPlayerTransform);
	SAFE_RELEASE(m_pPlayer);
	SAFE_RELEASE(m_pAniController);
	SAFE_RELEASE(m_pViewCol);
	SAFE_RELEASE(m_pAttCol);
}
