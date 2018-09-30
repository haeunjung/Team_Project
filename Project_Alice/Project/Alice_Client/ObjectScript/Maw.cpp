#include "Maw.h"
#include "Battery.h"
#include "Player.h"
#include "HitEffect.h"
#include "07.Component/Renderer.h"
#include "07.Component/Transform.h"
#include "05.Scene/Scene.h"
#include "07.Component/ColliderSphere.h"
#include "../ClientMgr/MinionMgr.h"

bool CMaw::Init()
{
	// �������� ����Ҷ� ���� ������ ���� ����ؾ� �Ѵ�
	m_pTransform->SetLocalPos(DxVector3(0.0f, 82.0f, 0.0f));
	m_pTransform->SetLocalRot(DxVector3(-PI * 0.5f, PI, 0.0f));
	m_pTransform->SetWorldScale(0.015f, 0.015f, 0.015f);

	CRenderer* pPlayerRenderer = m_pGameObject->AddComponent<CRenderer>("Renderer");
	pPlayerRenderer->SetMesh("Mutant", L"Mutant.msh");
	pPlayerRenderer->SetShader(STANDARD_ANI_BUMP_SHADER);
	pPlayerRenderer->SetInputLayout("AniBumpInputLayout");
	SAFE_RELEASE(pPlayerRenderer);

	m_pAniController = (CAnimation3D*)m_pGameObject->FindComponentFromType(CT_ANIMATION3D);

	CColliderSphere*	pSphere = m_pGameObject->AddComponent<CColliderSphere>("MinionColSphere");
	pSphere->SetSphereInfo(Vector3(0.0f, 0.0f, 0.0f), 0.7f);
	pSphere->SetBoneIndex(m_pAniController->FindBoneIndex("Mutant:Spine"));
	pSphere->SetColCheck(CC_HIT);
	SAFE_RELEASE(pSphere);

	DxVector3 Pos = m_pTransform->GetWorldPos();
	DxVector3 Forward = m_pTransform->GetWorldAxis(AXIS_Z);

	m_pViewCol = m_pGameObject->AddComponent<CColliderSphere>("MonsterViewCol");
	m_pViewCol->SetSphereInfo(Pos + Forward * 3.0f, 2.0f);
	m_pViewCol->SetColCheck(CC_VIEW);

	m_pAttCol = m_pGameObject->AddComponent<CColliderSphere>("MonsterAttCol");
	m_pAttCol->SetSphereInfo(Vector3(0.0f, 0.0f, 0.0f), 1.5f);
	m_pAttCol->SetBoneIndex(m_pAniController->FindBoneIndex("Mutant:LeftHand"));
	m_pAttCol->SetColCheck(CC_ATT);
	m_pAttCol->SetIsEnable(false);

	m_pHitSound = m_pGameObject->AddComponent<CSoundPlayer>("HitSound");
	m_pAttackSound = m_pGameObject->AddComponent<CSoundPlayer>("AttackSound");

	return true;
}

void CMaw::Update(float _fTime)
{
	switch (m_eMonsterState)
	{
	case MS_DEFAULT:
		MawIdle(_fTime);
		break;
	case MS_WALK:
		MawWalk(_fTime);
		break;
	case MS_TRACE:
		MawTrace(_fTime);
		break;
	case MS_ATTACK:
		MawAttack();
		break;
	case MS_DEATH:
		MawDeath();
		break;
	case MS_SPOTTRACE:
		MawSpotTrace(_fTime);
		break;
	}
}

CMaw * CMaw::Clone()
{
	return new CMaw(*this);
}

void CMaw::OnCollisionEnter(CCollider * _pSrc, CCollider * _pDest, float _fTime)
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

void CMaw::OnCollisionStay(CCollider * _pSrc, CCollider * _pDest, float _fTime)
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

void CMaw::MawIdle(float _fTime)
{
	// ���� �ð��� �Ǹ� Walk�� ����
	if (true != m_pAniController->CheckClipName("Idle"))
	{
		m_pAniController->ReturnToDefaultClip();
	}

	m_fTime += _fTime;
	if (9.0f <= m_fTime)
	{
		m_fTime = 0.0f;
		// - PI ~ PI
		float Rand = (rand() % 629 * 0.01f) - PI;
		m_pTransform->RotateY(Rand);

		// ViewCol ��ġ ����
		m_pViewCol->SetSphereInfo(m_pTransform->GetWorldPos() + m_pTransform->GetWorldAxis(AXIS_Z) * 2.5f, 2.5f);

		m_eMonsterState = MS_WALK;
	}
}

void CMaw::MawWalk(float _fTime)
{
	//m_pTransform->SetLocalPos(DxVector3(0.0f, 90.0f, 0.0f));
	// ���� �ð��� �Ǹ� �ٽ� Idle�� ����
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

void CMaw::MawTrace(float _fTime)
{
	if (0.0f < m_pPlayerTransform->GetWorldPos().y)
	{
		m_eMonsterState = MS_DEFAULT;
		return;
	}

	PLAYER_STATE PlayerState = m_pPlayerScript->GetPlayerState();
	if (PS_DEATH == PlayerState || PS_CLIMB == PlayerState || PS_CLIMBIDLE == PlayerState)
	{
		m_eMonsterState = MS_DEFAULT;
		return;
	}

	m_pTransform->LookAt(m_pPlayerTransform);
	m_pViewCol->SetSphereInfo(m_pTransform->GetWorldPos() + m_pTransform->GetWorldAxis(AXIS_Z) * 2.5f, 2.5f);

	if (true == m_pAniController->CheckClipName("Run"))
	{
		m_pTransform->Forward(m_fSpeed * 3.0f, _fTime);
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

void CMaw::MawSpotTrace(float _fTime)
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
		m_pTransform->Forward(m_fSpeed * 3.0f, _fTime);
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

void CMaw::MawAttack()
{
	PLAYER_STATE PlayerState = m_pPlayerScript->GetPlayerState();
	if (PS_DEATH == PlayerState || PS_CLIMB == PlayerState || PS_CLIMBIDLE == PlayerState)
	{
		m_eMonsterState = MS_DEFAULT;
	}

	if (true == m_pAniController->CheckClipName("Attack"))
	{
		if (389 <= m_pAniController->GetAnimationProgressFrame()
			&& 390 >= m_pAniController->GetAnimationProgressFrame())
		{
			m_pAttCol->SetIsEnable(true);
			m_pAttackSound->MyPlaySound("MutantAttack.wav");
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

void CMaw::MawDeath()
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
		m_pHitSound->MyPlaySound("MutantDeath.wav");
		m_pAniController->ChangeClip("Die");
	}
}

CMaw::CMaw()
{
	SetTypeID<CMaw>();
	SetTypeName("Maw");
}

CMaw::CMaw(const CMaw & _Mutant)
{
}

CMaw::~CMaw()
{
}
