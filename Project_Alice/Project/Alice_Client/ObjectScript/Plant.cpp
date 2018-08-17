#include "Plant.h"
#include "Battery.h"
#include "Player.h"
#include "HitEffect.h"
#include "07.Component/Renderer.h"
#include "07.Component/Transform.h"
#include "05.Scene/Scene.h"
#include "07.Component/ColliderSphere.h"
#include "../ClientMgr/MinionMgr.h"

bool CPlant::Init()
{
	CTransform*		pTransform = m_pGameObject->GetTransform();
	//pTransform->SetWorldPos(40.0f, 0.0f, 20.0f);
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
	pSphere->SetSphereInfo(Vector3(0.0f, 0.0f, 0.0f), 0.7f);
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
	m_pAttCol->SetSphereInfo(Vector3(0.0f, 0.0f, 0.0f), 1.5f);
	m_pAttCol->SetBoneIndex(m_pAniController->FindBoneIndex("Bone16"));
	m_pAttCol->SetColCheck(CC_ATT);
	m_pAttCol->SetIsEnable(false);

	m_pHitSound = m_pGameObject->AddComponent<CSoundPlayer>("HitSound");
	m_pAttackSound = m_pGameObject->AddComponent<CSoundPlayer>("AttackSound");

	return true;
}

void CPlant::Update(float _fTime)
{
	switch (m_eMonsterState)
	{
	case MS_DEFAULT:
		PlantIdle(_fTime);
		break;
	case MS_WALK:
		PlantWalk(_fTime);
		break;
	case MS_TRACE:
		PlantTrace(_fTime);
		break;
	case MS_ATTACK:
		PlantAttack();
		break;
	case MS_DEATH:
		PlantDeath();
		break;
	case MS_SPOTTRACE:
		PlantSpotTrace(_fTime);
		break;
	}
}

CPlant * CPlant::Clone()
{
	return new CPlant(*this);
}

void CPlant::OnCollisionEnter(CCollider * _pSrc, CCollider * _pDest, float _fTime)
{
	if (true == m_bTest)
	{
		return;
	}

	if (MS_TRACE == m_eMonsterState)
	{
		return;
	}

	if (CC_VIEW == _pSrc->GetColliderCheck())
	{
		if (CC_PLAYER_HIT == _pDest->GetColliderCheck())
		{
			//SAFE_RELEASE(m_pPlayerTransform);
			//m_pPlayerTransform = _pDest->GetTransform();
			m_fTime = 0.0f;
			m_eMonsterState = MS_TRACE;
		}

		if (CC_OBJ == _pDest->GetColliderCheck())
		{
			/*m_pTransform->RotateY(PI);
			m_pViewCol->SetSphereInfo(m_pTransform->GetWorldPos() + m_pTransform->GetWorldAxis(AXIS_Z) * 2.5f, 2.5f);
			m_fTime = 0.0f;*/
			m_pTransform->RotateY(PI);
			m_pViewCol->SetSphereInfo(m_pTransform->GetWorldPos() + m_pTransform->GetWorldAxis(AXIS_Z) * 2.5f, 2.5f);
			m_eMonsterState = MS_DEFAULT;
		}
	}
}

void CPlant::OnCollisionStay(CCollider * _pSrc, CCollider * _pDest, float _fTime)
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

				/*m_pParticleObj = CGameObject::CreateClone("Particle");

				CTransform*	pParticleTr = m_pParticleObj->GetTransform();
				pParticleTr->SetWorldPos(vColPos);
				pParticleTr->Move(DxVector3(0.0f, 1.0f, 0.0f));
				SAFE_RELEASE(pParticleTr);

				CLayer* pLayer = m_pScene->FindLayer(DEFAULTLAYER);
				m_pLayer->AddObject(m_pParticleObj);
				SAFE_RELEASE(pLayer);*/
				//m_pParticleObj->AddRef();

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

void CPlant::PlantIdle(float _fTime)
{
	if (true == m_bTest)
	{
		return;
	}

	if (true != m_pAniController->CheckClipName("Idle"))
	{
		m_pAniController->ReturnToDefaultClip();
	}

	// 일정 시간이 되면 Walk로 가자
	m_fTime += _fTime;
	if (3.0f <= m_fTime)
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

void CPlant::PlantWalk(float _fTime)
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

void CPlant::PlantTrace(float _fTime)
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
		m_pTransform->Forward(m_fSpeed * 2.5f, _fTime);
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

void CPlant::PlantSpotTrace(float _fTime)
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
		m_pTransform->Forward(m_fSpeed * 2.5f, _fTime);
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
}

void CPlant::PlantAttack()
{
	PLAYER_STATE PlayerState = m_pPlayerScript->GetPlayerState();
	if (PS_DEATH == PlayerState || PS_CLIMB == PlayerState || PS_CLIMBIDLE == PlayerState)
	{
		m_eMonsterState = MS_DEFAULT;
	}

	if (true == m_pAniController->CheckClipName("Attack"))
	{
		if (40 <= m_pAniController->GetAnimationProgressFrame() &&
			41 >= m_pAniController->GetAnimationProgressFrame())
		{
			m_pAttackSound->MyPlaySound("MinionAttack.wav");
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

void CPlant::PlantDeath()
{
	if (true == m_pAniController->CheckClipName("Death"))
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

			//m_pParticleObj->Death();
			//SetIsEnable(false);
			m_pGameObject->SetIsEnable(false);
			m_eMonsterState = MS_DEFAULT;

			GET_SINGLE(CMinionMgr)->PushRespawnList(this);
			GET_SINGLE(CMinionMgr)->EraseMinion(this);
		}
	}
	else
	{
		m_pHitSound->MyPlaySound("MinionDeath.wav");
		m_pAniController->ChangeClip("Death");
	}
}

CPlant::CPlant()
{
	SetTypeID<CPlant>();
	SetTypeName("Plant");
}

CPlant::CPlant(const CPlant & _Plant)
{
}

CPlant::~CPlant()
{
}
