#include "Warrok.h"
#include "Battery.h"
#include "Player.h"
#include "HitEffect.h"
#include "01.Core/Debug.h"
#include "07.Component/Renderer.h"
#include "07.Component/Transform.h"
#include "05.Scene/Scene.h"
#include "07.Component/ColliderSphere.h"
#include "../ClientMgr/MinionMgr.h"

bool CWarrok::Init()
{
	// 로컬포스 계산할때 월드 스케일 값을 고려해야 한다
	m_pTransform->SetLocalPos(DxVector3(0.0f, 82.0f, 0.0f));
	m_pTransform->SetLocalRot(DxVector3(-PI * 0.5f, PI, 0.0f));
	m_pTransform->SetWorldScale(0.012f, 0.012f, 0.012f);

	CRenderer* pPlayerRenderer = m_pGameObject->AddComponent<CRenderer>("Renderer");
	pPlayerRenderer->SetMesh("Warrok", L"Warrok.msh");
	pPlayerRenderer->SetShader(STANDARD_ANI_BUMP_SHADER);
	pPlayerRenderer->SetInputLayout("AniBumpInputLayout");
	SAFE_RELEASE(pPlayerRenderer);

	m_pAniController = (CAnimation3D*)m_pGameObject->FindComponentFromType(CT_ANIMATION3D);

	CColliderSphere*	pSphere = m_pGameObject->AddComponent<CColliderSphere>("MinionColSphere");
	pSphere->SetSphereInfo(Vector3(0.0f, 0.0f, 0.0f), 0.7f);
	pSphere->SetBoneIndex(m_pAniController->FindBoneIndex("Spine1"));
	pSphere->SetColCheck(CC_HIT);
	SAFE_RELEASE(pSphere);

	DxVector3 Pos = m_pTransform->GetWorldPos();
	DxVector3 Forward = m_pTransform->GetWorldAxis(AXIS_Z);

	m_pViewCol = m_pGameObject->AddComponent<CColliderSphere>("MonsterViewCol");
	m_pViewCol->SetSphereInfo(Pos + Forward * 2.5f, 2.5f);
	m_pViewCol->SetColCheck(CC_VIEW);

	m_pAttCol = m_pGameObject->AddComponent<CColliderSphere>("MonsterAttCol");
	m_pAttCol->SetSphereInfo(Vector3(0.0f, 0.0f, 0.0f), 1.5f);
	m_pAttCol->SetBoneIndex(m_pAniController->FindBoneIndex("RightHand"));
	m_pAttCol->SetColCheck(CC_ATT);
	m_pAttCol->SetIsEnable(false);

	m_pHitSound = m_pGameObject->AddComponent<CSoundPlayer>("HitSound");
	m_pAttackSound = m_pGameObject->AddComponent<CSoundPlayer>("AttackSound");

	return true;
}

void CWarrok::Update(float _fTime)
{
	switch (m_eMonsterState)
	{
	case MS_DEFAULT:
		WarrokIdle(_fTime);
		break;
	case MS_WALK:
		WarrokWalk(_fTime);
		break;
	case MS_TRACE:
		WarrokTrace(_fTime);
		break;
	case MS_ATTACK:
		WarrokAttack();
		break;
	case MS_DEATH:
		WarrokDeath();
		break;
	case MS_SPOTTRACE:
		WarrokSpotTrace(_fTime);
		break;
	}
}

CWarrok * CWarrok::Clone()
{
	return new CWarrok(*this);
}

void CWarrok::OnCollisionEnter(CCollider * _pSrc, CCollider * _pDest, float _fTime)
{
	if (MS_TRACE == m_eMonsterState)
	{
		return;
	}

	if (CC_VIEW == _pSrc->GetColliderCheck())
	{
		if (CC_PLAYER_HIT == _pDest->GetColliderCheck())
		{
			m_fTime = 0.0f;
			m_eMonsterState = MS_TRACE;
		}

		if (CC_OBJ == _pDest->GetColliderCheck())
		{
			m_pTransform->RotateY(PI);
			m_pViewCol->SetSphereInfo(m_pTransform->GetWorldPos() + m_pTransform->GetWorldAxis(AXIS_Z) * 2.5f, 2.5f);
			m_eMonsterState = MS_DEFAULT;
		}
	}
}

void CWarrok::OnCollisionStay(CCollider * _pSrc, CCollider * _pDest, float _fTime)
{
	if (MS_DEATH == m_eMonsterState)
	{
		return;
	}

	if (CC_HIT == _pSrc->GetColliderCheck())
	{
		if (CC_PLAYER_ATT == _pDest->GetColliderCheck() &&
			true == _pDest->GetIsEnable())
		{
			if (true == BackAttackCheck(_pSrc->GetTransformWorldAxis(AXIS_Z), _pDest->GetTransformWorldAxis(AXIS_Z)))
			{
				m_pPlayerScript->PlayAttackSound();

				m_fTime = 0.0f;
				_pDest->SetIsEnable(false);
				m_eMonsterState = MS_DEATH;

				DxVector3 vColPos = ((CColliderSphere*)_pDest)->GetSphereInfo().vCenter;

				CLayer* pLayer = m_pScene->FindLayer("UILayer");

				CGameObject* pEffect = CGameObject::CreateClone("HitEffect");

				CTransform* pTransform = pEffect->GetTransform();
				pTransform->SetWorldPos(vColPos);
				pTransform->Move(DxVector3(-1.0f, 1.0f, 0.0f));
				SAFE_RELEASE(pTransform);

				pLayer->AddObject(pEffect);
				SAFE_RELEASE(pEffect);

				pEffect = CGameObject::CreateClone("HitEffect");

				pTransform = pEffect->GetTransform();
				pTransform->SetWorldPos(vColPos);
				pTransform->Move(DxVector3(1.0f, 0.0f, 0.0f));
				SAFE_RELEASE(pTransform);

				pLayer->AddObject(pEffect);
				SAFE_RELEASE(pEffect);

				pEffect = CGameObject::CreateClone("HitEffect");

				pTransform = pEffect->GetTransform();
				pTransform->SetWorldPos(vColPos);
				pTransform->Move(DxVector3(0.0f, 2.0f, 0.0f));
				SAFE_RELEASE(pTransform);

				pLayer->AddObject(pEffect);
				SAFE_RELEASE(pEffect);

				SAFE_RELEASE(pLayer);
			}
		}
	}
}

void CWarrok::WarrokIdle(float _fTime)
{
	// 일정 시간이 되면 Walk로 가자
	if (true != m_pAniController->CheckClipName("Idle"))
	{
		m_pAniController->ReturnToDefaultClip();
	}

	m_fTime += _fTime;
	if (6.0f <= m_fTime)
	{
		m_fTime = 0.0f;
		// - PI ~ PI
		float Rand = (rand() % 629 * 0.01f) - PI;
		m_pTransform->RotateY(Rand);

		// ViewCol 위치 갱신
		m_pViewCol->SetSphereInfo(m_pTransform->GetWorldPos() + m_pTransform->GetWorldAxis(AXIS_Z) * 2.5f, 2.5f);

		m_eMonsterState = MS_WALK;
	}
}

void CWarrok::WarrokWalk(float _fTime)
{
	//m_pTransform->SetLocalPos(DxVector3(0.0f, 90.0f, 0.0f));
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

void CWarrok::WarrokTrace(float _fTime)
{
	PLAYER_STATE PlayerState = m_pPlayerScript->GetPlayerState();
	if (PS_DEATH == PlayerState || PS_CLIMB == PlayerState || PS_CLIMBIDLE == PlayerState)
	{
		m_eMonsterState = MS_DEFAULT;
	}

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
	if (1.2f >= m_fDist)
	{
		m_eMonsterState = MS_ATTACK;
	}
	else if (12.0f <= m_fDist)
	{
		m_eMonsterState = MS_DEFAULT;
	}
}

void CWarrok::WarrokSpotTrace(float _fTime)
{
	PLAYER_STATE PlayerState = m_pPlayerScript->GetPlayerState();
	if (PS_DEATH == PlayerState || PS_CLIMB == PlayerState || PS_CLIMBIDLE == PlayerState)
	{
		m_eMonsterState = MS_DEFAULT;
	}

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
	if (1.2f >= m_fDist)
	{
		m_eMonsterState = MS_ATTACK;
	}
}

void CWarrok::WarrokAttack()
{
	PLAYER_STATE PlayerState = m_pPlayerScript->GetPlayerState();
	if (PS_DEATH == PlayerState || PS_CLIMB == PlayerState || PS_CLIMBIDLE == PlayerState)
	{
		m_eMonsterState = MS_DEFAULT;
	}

	if (true == m_pAniController->CheckClipName("Attack"))
	{
		//char strFrame[256] = {};
		//int iFrame = m_pAniController->GetAnimationProgressFrame();
		//sprintf_s(strFrame, "[ %d Frame ]\n", iFrame);
		//CDebug::OutputConsole(strFrame);

		if (220 == m_pAniController->GetAnimationProgressFrame())
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
	if (1.2f < m_fDist)
	{
		m_eMonsterState = MS_TRACE;
	}
}

void CWarrok::WarrokDeath()
{
	if (true == m_pAniController->CheckClipName("Die"))
	{
		if (true == m_pAniController->GetAnimationEnd())
		{
			CGameObject* pBattery = CGameObject::CreateClone("BatteryObject");
			CTransform* pTransform = pBattery->GetTransform();
			pTransform->SetWorldPos(m_pTransform->GetWorldPos());
			pTransform->SetWorldPosY(1.0f);
			SAFE_RELEASE(pTransform);

			m_pLayer->AddObject(pBattery);
			SAFE_RELEASE(pBattery);

			m_pGameObject->SetIsEnable(false);
			m_eMonsterState = MS_DEFAULT;

			GET_SINGLE(CMinionMgr)->PushRespawnList(this);
			GET_SINGLE(CMinionMgr)->EraseMinion(this);
		}
	}
	else
	{
		m_pHitSound->MyPlaySound("MinionDeath.wav");
		m_pAniController->ChangeClip("Die");
	}
}

CWarrok::CWarrok()
{
	SetTypeID<CWarrok>();
	SetTypeName("Warrok");
}

CWarrok::CWarrok(const CWarrok & _Warrok)
{
}

CWarrok::~CWarrok()
{
}
