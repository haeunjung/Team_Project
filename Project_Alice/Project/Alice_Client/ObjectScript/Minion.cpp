#include "Minion.h"
#include "Player.h"
#include "07.Component/Transform.h"
#include "../ClientMgr/MinionMgr.h"

void CMinion::SetRespawnPos(DxVector3 _vPos)
{
	m_vRespawnPos = _vPos;
}

void CMinion::SetPlayer(CPlayer * _pPlayer)
{
	SAFE_RELEASE(m_pPlayerScript);
	_pPlayer->AddRef();
	m_pPlayerScript = _pPlayer;
	//m_pPlayerScript->AddRef();

	SAFE_RELEASE(m_pPlayerTransform);
	m_pPlayerTransform = _pPlayer->GetTransform();
}

void CMinion::SetMonsterWorldPos(const DxVector3 _Pos)
{
	m_pTransform->SetWorldPos(_Pos);
}

float CMinion::DistCheckFromPlayer()
{
	return m_pTransform->GetWorldPos().Distance(m_pPlayerTransform->GetWorldPos());
	//return DxVector3();
}

void CMinion::TracePlayer()
{
	PLAYER_STATE PlayerState = m_pPlayerScript->GetPlayerState();
	if (PS_DEATH == PlayerState || PS_CLIMB == PlayerState || PS_CLIMBIDLE == PlayerState)
	{
		return;
	}

	m_fTime = 0.0f;
	m_eMonsterState = MS_SPOTTRACE;
}

MONSTER_STATE CMinion::GetState()
{
	return m_eMonsterState;
}

bool CMinion::RespawnUpdate(float _fTime)
{
	m_fRespawnTime += _fTime;

	if (10.0f <= m_fRespawnTime)
	{
		m_fTime = 0.0f;
		m_fRespawnTime = 0.0f;

		// 리스폰 위치 세팅
		SetRespawnPos();

		// 오브젝트 켜고
		m_pGameObject->SetIsEnable(true);

		// 리스폰 리스트에서 빼고
		GET_SINGLE(CMinionMgr)->EraseRespawnList(this);

		// 테스트용은 매니저에 추가할 필요 없고
		if (!m_bTest)
		{
			// 매니저에 추가하고
			GET_SINGLE(CMinionMgr)->PushMinion(this);
		}

		return true;
	}

	return false;
}

void CMinion::MinionAttackSound()
{
	m_pAttackSound->MyPlaySound("MinionAttack.wav");
}

bool CMinion::Init()
{		
	return true;
}

void CMinion::Update(float _fTime)
{
}

CMinion * CMinion::Clone()
{
	return new CMinion(*this);
}

void CMinion::OnCollisionEnter(CCollider * _pSrc, CCollider * _pDest, float _fTime)
{		
	
}

void CMinion::OnCollisionStay(CCollider * _pSrc, CCollider * _pDest, float _fTime)
{
	
}

bool CMinion::BackAttackCheck(const DxVector3 & _SrcForward, const DxVector3 & _DestForward)
{
	if (0.0f < _SrcForward.Dot(_DestForward))
	{
		return true;
	}
	return false;
}



void CMinion::SetRespawnPos()
{
	m_pTransform->SetWorldPos(m_vRespawnPos);
}

CMinion::CMinion() :
	m_pPlayerScript(NULL),
	m_pPlayerTransform(NULL),
	m_pAniController(NULL),
	m_fSpeed(2.0f),
	m_fTime(0.0f),
	m_fDist(0.0f),
	m_fRespawnTime(0.0f),
	m_pViewCol(NULL),
	m_pAttCol(NULL),
	m_pHitSound(NULL),
	m_pAttackSound(NULL),
	m_bTest(false)
{
	m_eMonsterState = MS_DEFAULT;
}

CMinion::CMinion(const CMinion & _Minion) :
	CScript(_Minion)
{
	m_pPlayerScript = NULL;
	m_fTime = _Minion.m_fTime;
}

CMinion::~CMinion()
{		
	SAFE_RELEASE(m_pHitSound);
	SAFE_RELEASE(m_pAttackSound);

	SAFE_RELEASE(m_pPlayerTransform);
	SAFE_RELEASE(m_pPlayerScript);
	SAFE_RELEASE(m_pAniController);
	SAFE_RELEASE(m_pViewCol);
	SAFE_RELEASE(m_pAttCol);
}
