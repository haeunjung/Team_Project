#include "Plant_Range.h"
#include "Battery.h"
#include "Player.h"
#include "HitEffect.h"
#include "MonsterBullet.h"
#include "07.Component/Renderer.h"
#include "07.Component/Transform.h"
#include "05.Scene/Scene.h"
#include "07.Component/ColliderSphere.h"
#include "../ClientMgr/MinionMgr.h"

bool CPlantRange::Init()
{
	CTransform*		pTransform = m_pGameObject->GetTransform();
	pTransform->SetWorldScale(0.05f, 0.05f, 0.05f);
	pTransform->SetLocalRotY(-PI * 0.5f);
	pTransform->SetWorldRotY(PI);

	CRenderer* pPlayerRenderer = m_pGameObject->AddComponent<CRenderer>("Renderer");
	pPlayerRenderer->SetMesh("MinionMesh", L"SmallMonster.msh");
	pPlayerRenderer->SetShader(STANDARD_ANI_BUMP_SHADER);
	pPlayerRenderer->SetInputLayout("AniBumpInputLayout");
	SAFE_RELEASE(pPlayerRenderer);

	m_pAniController = (CAnimation3D*)m_pGameObject->FindComponentFromType(CT_ANIMATION3D);

	CColliderSphere*	pSphere = m_pGameObject->AddComponent<CColliderSphere>("MinionColSphere");
	pSphere->SetSphereInfo(Vector3(0.0f, 0.0f, 0.0f), 0.7f);
	pSphere->SetBoneIndex(m_pAniController->FindBoneIndex("Bone09"));
	pSphere->SetColCheck(CC_HIT);
	SAFE_RELEASE(pSphere);

	DxVector3 Pos = pTransform->GetWorldPos();
	DxVector3 Forward = pTransform->GetWorldAxis(AXIS_Z);
	SAFE_RELEASE(pTransform);

	m_pHitSound = m_pGameObject->AddComponent<CSoundPlayer>("HitSound");
	m_pAttackSound = m_pGameObject->AddComponent<CSoundPlayer>("AttackSound");

	return true;
}

void CPlantRange::Update(float _fTime)
{
	switch (m_eMonsterState)
	{
	case MS_DEFAULT:
		PlantRangeIdle(_fTime);
		break;
	case MS_ATTACK:
		PlantRangeAttack();
		break;
	case MS_DEATH:
		PlantRangeDeath();
		break;
	case MS_SPOTTRACE:
		PlantRangeSpotTrace(_fTime);
		break;
	}
}

CPlantRange * CPlantRange::Clone()
{
	return new CPlantRange(*this);
}

void CPlantRange::OnCollisionEnter(CCollider * _pSrc, CCollider * _pDest, float _fTime)
{
	
}

void CPlantRange::OnCollisionStay(CCollider * _pSrc, CCollider * _pDest, float _fTime)
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

void CPlantRange::PlantRangeIdle(float _fTime)
{
	
}

void CPlantRange::PlantRangeSpotTrace(float _fTime)
{
	m_eMonsterState = MS_ATTACK;
}

void CPlantRange::PlantRangeAttack()
{
	PLAYER_STATE PlayerState = m_pPlayerScript->GetPlayerState();
	if (PS_DEATH == PlayerState || PS_CLIMB == PlayerState || PS_CLIMBIDLE == PlayerState)
	{
		m_eMonsterState = MS_DEFAULT;
	}

	m_pTransform->LookAt(m_pPlayerTransform);
	if (true == m_pAniController->CheckClipName("Attack"))
	{
		if (true == m_pAniController->GetAnimationEnd())
		{
			CreateEnergyBall();
			m_pAniController->ReturnToDefaultClip();
			m_eMonsterState = MS_DEFAULT;
		}
	}
	else
	{
		m_pAniController->ChangeClip("Attack");
	}
}

void CPlantRange::PlantRangeDeath()
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

void CPlantRange::CreateEnergyBall()
{
	CGameObject* pEnergyBallObj = CGameObject::Create("EnergyBallObj");

	CTransform* pEnergyBallTransform = pEnergyBallObj->GetTransform();
	pEnergyBallTransform->SetWorldPos(m_pTransform->GetWorldPos());
	pEnergyBallTransform->SetWorldPosY(1.0f);
	SAFE_RELEASE(pEnergyBallTransform);

	CMonsterBullet* pEnergyBall = pEnergyBallObj->AddComponent<CMonsterBullet>("EnergyBall");

	CTransform* pPlayerChildTransform = m_pPlayerScript->GetChildTransform();
	pEnergyBall->SetPlayerTransform(pPlayerChildTransform);
	SAFE_RELEASE(pPlayerChildTransform);

	m_pLayer->AddObject(pEnergyBallObj);
	SAFE_RELEASE(pEnergyBall);

	SAFE_RELEASE(pEnergyBallObj);
}

CPlantRange::CPlantRange()
{
	SetTypeID<CPlantRange>();
	SetTag("CPlantRange");
	SetTypeName("PlantRange");
}

CPlantRange::CPlantRange(const CPlantRange & _PlantRange)
{
}

CPlantRange::~CPlantRange()
{
}
