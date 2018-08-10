#include "Player.h"
#include "PlayerHitEffect.h"
#include "MessageBox.h"
#include "01.Core/Input.h"
#include "01.Core/Debug.h"
#include "05.Scene/Layer.h"
#include "05.Scene/Scene.h"
#include "03.Resource/Mesh.h"
#include "03.Resource/ResMgr.h"
#include "06.GameObject/GameObject.h"
#include "07.Component/Transform.h"
#include "07.Component/Renderer.h"
#include "07.Component/Camera.h"
#include "07.Component/Material.h"
#include "07.Component/Effect.h"
#include "07.Component/Animation2D.h"
#include "07.Component/Renderer2D.h"
#include "07.Component/PlayerLight.h"
#include "../ClientMgr/UIMgr.h"
#include "../ClientMgr/MinionMgr.h"

bool CPlayer::m_bCheat = false;

void CPlayer::CheatKey()
{
	m_bCheat = !m_bCheat;
}

void CPlayer::FirstStepSound(float _fTime)
{
	m_pHitSound->MyPlaySound("Step.wav");
}

void CPlayer::SecondStepSound(float _fTime)
{
	m_pAttackSound->MyPlaySound("Step.wav");
}

bool CPlayer::PlayerStateIsDeath()
{
	return m_bDeath;
}

const PLAYER_STATE & CPlayer::GetPlayerState() const
{
	return m_ePlayerState;
}

void CPlayer::SetPlayerDeath()
{
	if (!m_bCheat)
	{
		m_ePlayerState = PS_DEATH;
		m_pAniController->ChangeClip("Death");
	}	
}

void CPlayer::PlayAttackSound()
{
	m_pAttackSound->MyPlaySound("PlayerAttack.wav");
}

bool CPlayer::Init()
{	
	CGameObject* pChild = CGameObject::Create("Child");
	m_pGameObject->AddChild(pChild);
	m_pChildTransform = pChild->GetTransform();

	CCamera*	pCamera = m_pScene->GetMainCamera();
	pCamera->Attach(pChild, DxVector3(0.0f, 1.5f, -3.0f));
	SAFE_RELEASE(pCamera);
	SAFE_RELEASE(pChild);

	CRenderer* pPlayerRenderer = m_pGameObject->AddComponent<CRenderer>("Renderer");
	//pPlayerRenderer->SetMesh("PlayerMesh", L"Elin.msh");
	pPlayerRenderer->SetMesh("PlayerMesh", L"Player3.msh");	
	//pPlayerRenderer->SetMesh("PlayerMesh", L"New_Alice.msh");
	pPlayerRenderer->SetShader(STANDARD_ANI_BUMP_SHADER);
	pPlayerRenderer->SetInputLayout("AniBumpInputLayout");	
	m_fy = pPlayerRenderer->GetMeshSize().y;
	m_fz = pPlayerRenderer->GetMeshSize().z;
	SAFE_RELEASE(pPlayerRenderer);

	m_pTransform->SetLocalPos(0.0f, m_fy * 0.5f, 0.0f);
	m_pTransform->SetWorldPos(5.0f, 0.0f, 5.0f);
	m_pTransform->SetWorldScale(1.0f, 1.0f, 1.0f);
	//m_pTransform->SetWorldScale(0.5f, 0.5f, 0.5f);
	m_pTransform->SetLocalRot(-PI_HALF, PI, 0.0f);
	//m_pTransform->SetLocalRot(0.0f, PI, 0.0f);

	m_pAniController = (CAnimation3D*)m_pGameObject->FindComponentFromType(CT_ANIMATION3D);
	//m_pAniController->AddClipCallback<CPlayer>("Run", 0.2f, this, &CPlayer::FirstStepSound);
	//m_pAniController->AddClipCallback<CPlayer>("Run", 0.8f, this, &CPlayer::SecondStepSound);

	m_pAttCol = m_pGameObject->AddComponent<CColliderSphere>("PlayerAtt");
	m_pAttCol->SetSphereInfo(Vector3(0.0f, 0.0f, 0.0f), 0.3f);
	m_pAttCol->SetBoneIndex(m_pAniController->FindBoneIndex("mixamorig:RightHand"));
	m_pAttCol->SetColCheck(CC_PLAYER_ATT);
	m_pAttCol->SetIsEnable(false);
	
	m_pHitCol = m_pGameObject->AddComponent<CColliderSphere>("PlayerHit");
	m_pHitCol->SetSphereInfo(0.0f, 0.0f, 0.0f, 0.7f);
	m_pHitCol->SetBoneIndex(m_pAniController->FindBoneIndex("mixamorig:Spine"));
	m_pHitCol->SetColCheck(CC_PLAYER_HIT);

	m_pFootCol = m_pGameObject->AddComponent<CColliderSphere>("PlayerFoot");
	m_pFootCol->SetSphereInfo(m_pTransform->GetWorldPos(), 0.5f);
	m_pFootCol->SetColCheck(CC_PLAYER_FOOT);

	CGameObject* pCameraObject = m_pScene->GetMainCameraObject();
	
	m_pCameraArm = (CCameraArm*)pCameraObject->FindComponentFromType(CT_CAMERAARM);	
	SAFE_RELEASE(pCameraObject);

	m_pHitSound = m_pGameObject->AddComponent<CSoundPlayer>("HitSound");
	m_pAttackSound = m_pGameObject->AddComponent<CSoundPlayer>("AttackSound");
	m_pWarningSound = m_pGameObject->AddComponent<CSoundPlayer>("WarningSound");
	m_pGearSound = m_pGameObject->AddComponent<CSoundPlayer>("GearSound");

	CreatePlayerLight();

	GET_SINGLE(CInput)->CreateKey("MoveForward", VK_UP);
	GET_SINGLE(CInput)->CreateKey("MoveBack", VK_DOWN);
	GET_SINGLE(CInput)->CreateKey("MoveLeft", VK_LEFT);
	GET_SINGLE(CInput)->CreateKey("MoveRight", VK_RIGHT);

	GET_SINGLE(CInput)->CreateKey("Attack", VK_CONTROL);
	GET_SINGLE(CInput)->CreateKey("Jump", VK_SPACE);
	GET_SINGLE(CInput)->CreateKey("PosCheck", VK_RETURN);

	GET_SINGLE(CInput)->CreateKey("ChangeCamera", 'C');

	return true;
}

void CPlayer::Input(float _fTime)
{
	if (true == KEYPRESS("PosCheck"))
	{
		char strPos[256] = {};
		DxVector3 vPos = m_pTransform->GetWorldPos();
		sprintf_s(strPos, "[ X : %f, Y : %f, Z : %f]\n", vPos.x, vPos.y, vPos.z);
		CDebug::OutputConsole(strPos);
	}

	if (m_bAttack || m_bHeat || m_bClimbToTop || m_bDeath)
	{
		return;
	}

	PlayerMove(_fTime);
	
	if (m_bJump)
	{
		return;
	}

	if (true == KEYPRESS("Attack"))
	{
		m_ePlayerState = PS_ATTACK;
		return;
	}	

	if (KEYPRESS("Jump")
		&& KEYPUSH("MoveBack"))
	{
		return;
	}

	if (true == KEYPRESS("Jump"))
	{
		if (!m_bFrontCol && !m_bBackCol &&
			!m_bLeftCol && !m_bRightCol)
		{
			m_ePlayerState = PS_JUMP;
			return;
		}
	}	

	if (true == KEYPRESS("ChangeCamera"))
	{
		if (false == m_bChange)
		{
			// 카메라 체인지 하고
			m_pScene->ChangeCamera("SubCamera");
			// 카메라 컴포넌트 불러와서
			CCamera* pCamera = m_pScene->FindCamera("SubCamera");
			// 어태치 시키고
			pCamera->Attach(m_pGameObject, DxVector3(2.5f, 0.0f, 0.0f));
			// 릴리즈
			SAFE_RELEASE(pCamera);

			m_bChange = true;
		}
		else
		{
			// 카메라 체인지 하고
			m_pScene->ChangeCamera(MAINCAMERA);
			// 카메라 컴포넌트 불러와서
			CCamera* pCamera = m_pScene->GetMainCamera();
			// 어태치 시키고
			pCamera->Attach(m_pGameObject, DxVector3(0.0f, 0.0f, -2.5f));
			// 릴리즈
			SAFE_RELEASE(pCamera);

			m_bChange = false;
		}		
	}
}

void CPlayer::Update(float _fTime)
{	
	m_pChildTransform->SetWorldPos(m_pHitCol->GetSphereInfo().vCenter);

	m_pTransform->SetLocalPos(0.0f, m_fy * 0.5f, 0.0f);
	switch (m_ePlayerState)
	{
	case PS_DEFAULT:
		break;
	case PS_ATTACK:
		PlayerAttack();
		break;
	case PS_JUMP:
		PlayerJump(_fTime);
		break;
	case PS_MOVINGJUMP:
		PlayerJump(_fTime);
		//PlayerMovingJump(_fTime);
		break;
	case PS_CLIMBIDLE:
		PlayerClimbIdle();
		break;
	case PS_FALL:
		PlayerFall(_fTime);
		break;
	case PS_HEAT:
		PlayerHit(_fTime);
		break;
	case PS_CLIMBTOUP:
		PlayerClimbToTop();
		break;
	case PS_DEATH:
		PlayerDeath();
		break;
	}
}
 
void CPlayer::OnCollisionEnter(CCollider * _pSrc, CCollider * _pDest, float _fTime)
{
	if (PS_DEATH == m_ePlayerState)
	{
		return;
	}

	if (CC_PLAYER_HIT == _pSrc->GetColliderCheck() &&
		CC_SPOTLIGHT == _pDest->GetColliderCheck())
	{
		GET_SINGLE(CMinionMgr)->MinionListDistCheck();
		m_pWarningSound->MyPlaySound("Warning.wav");
	}

	if (m_iHp < MAXHP)
	{
		// 플레이어 HP가 최대보다 적을 때 Gear 획득 가능
		if (CC_PLAYER_HIT == _pSrc->GetColliderCheck() &&
			CC_GEAR == _pDest->GetColliderCheck())
		{
			m_pGearSound->MyPlaySound("GetBattery.mp3");

			++m_iHp;
			GET_SINGLE(CUIMgr)->SetHpOn(m_iHp);

			CGameObject* pGearObject = _pDest->GetGameObject();
			pGearObject->Death();
			SAFE_RELEASE(pGearObject);
		}
	}	
	
}

void CPlayer::OnCollisionStay(CCollider * _pSrc, CCollider * _pDest, float _fTime)
{
	if (CC_PLAYER_HIT == _pSrc->GetColliderCheck())
	{
		if ("MonsterAttCol" == _pDest->GetTagStr() &&
			true == _pDest->GetIsEnable())
		{
			m_pHitSound->MyPlaySound("PlayerHit.mp3");

			PlayerHitEffect();

			--m_iHp;
			GET_SINGLE(CUIMgr)->SetHpOff(m_iHp);

			CCamera* pCamera = m_pScene->GetMainCamera();
			pCamera->ActionCameraOn();
			SAFE_RELEASE(pCamera);

			_pDest->SetIsEnable(false);
			//_pDest->ColOn();

			if (0 >= m_iHp && !m_bCheat)
			{
				SetPlayerDeath();
			}
			else
			{
				m_ePlayerState = PS_HEAT;
			}
		}
	}

	if (CC_PLAYER_HIT == _pSrc->GetColliderCheck() &&
		CC_OBJ == _pDest->GetColliderCheck())
	{		
		if (true == KEYPRESS("Jump"))
		{
			if (!m_bClimb)
			{
				m_ePlayerState = PS_CLIMB;
				m_pFootCol->SetIsEnable(false);

				SAFE_RELEASE(m_pOtherCol);
				m_pOtherCol = _pDest;
				m_pOtherCol->AddRef();
			}
			else
			{
				m_ePlayerState = PS_DEFAULT;							
			}

			m_bClimb = !m_bClimb;
		}

		if (CAP_LEFT == ((CColliderAABB*)_pDest)->GetAABBInfo().eColAABB)
		{
			m_bLeftCol = true;
		}
		else if(CAP_RIGHT == ((CColliderAABB*)_pDest)->GetAABBInfo().eColAABB)
		{			
			m_bRightCol = true;
		}
		else if (CAP_FRONT == ((CColliderAABB*)_pDest)->GetAABBInfo().eColAABB)
		{
			m_bFrontCol = true;
		}
		else if(CAP_BACK == ((CColliderAABB*)_pDest)->GetAABBInfo().eColAABB)
		{
			m_bBackCol = true;
		}

		for (int i = 0; i < AXIS_MAX; ++i)
		{
			m_vColAxis[i] = _pDest->GetTransformWorldAxis((AXIS)i);
		}
	}

	if (CC_PLAYER_FOOT == _pSrc->GetColliderCheck() &&
		CC_OBJ == _pDest->GetColliderCheck())
	{
		if (m_bFall)
		{	
			float DestY = ((CColliderAABB*)_pDest)->GetTop();
			if (m_pTransform->GetWorldPos().y + 1.0f > DestY)
			{
  				SAFE_RELEASE(m_pOtherCol);
				m_pOtherCol = _pDest;
				m_pOtherCol->AddRef();

				m_pTransform->SetWorldPosY(DestY);
				m_ePlayerState = PS_DEFAULT;

				m_bJump = false;
				m_bFall = false;
				m_fJumpTime = 0.0f;
			}
		}
		
		/*if (CAP_UP == ((CColliderAABB*)_pDest)->GetAABBInfo().eColAABB)
		{
			

			float DestY = ((CColliderAABB*)_pDest)->GetTop();
			m_pTransform->SetWorldPosY(DestY);
			m_ePlayerState = PS_DEFAULT;
		}		*/
	}
}

void CPlayer::OnCollisionLeave(CCollider * _pSrc, CCollider * _pDest, float _fTime)
{
	if (CC_PLAYER_HIT == _pSrc->GetColliderCheck() &&
		CC_OBJ == _pDest->GetColliderCheck())
	{
		m_bFrontCol = false ;
		m_bBackCol = false; 
		m_bLeftCol = false;
		m_bRightCol = false;

		if (PS_CLIMBTOUP != m_ePlayerState)
		{
			m_pFootCol->SetIsEnable(true);
		}		
	}

	if (CC_PLAYER_FOOT == _pSrc->GetColliderCheck() &&
		CC_OBJ == _pDest->GetColliderCheck())
	{
		if (m_ePlayerState != PS_JUMP && m_ePlayerState != PS_MOVINGJUMP)
		{
			m_bFall = true;
			m_ePlayerState = PS_FALL;
			m_fJumpTime = 1.0f;
		}		
	}
}

void CPlayer::PlayerMove(float _fTime)
{
	if (true == KEYPUSH("MoveForward"))
	{
		if (PS_CLIMB == m_ePlayerState || PS_CLIMBIDLE == m_ePlayerState)
		{
			MoveUp(_fTime);
		}
		else
		{
			MoveForward(_fTime);
		}
		
	}
	else if (true == KEYUP("MoveForward"))
	{
		if (PS_CLIMB == m_ePlayerState)
		{
			m_ePlayerState = PS_CLIMBIDLE;
		}
		else
		{
			m_pAniController->ReturnToDefaultClip();
		}		
	}

	if (true == KEYPUSH("MoveBack"))
	{
		if (PS_CLIMB == m_ePlayerState || PS_CLIMBIDLE == m_ePlayerState)
		{
			MoveDown(_fTime);
		}
		else
		{
			MoveBack(_fTime);
		}
		
	}
	else if (true == KEYUP("MoveBack"))
	{
		if (PS_CLIMB == m_ePlayerState)
		{
			m_ePlayerState = PS_CLIMBIDLE;
		}
		else
		{
			m_pAniController->ReturnToDefaultClip();
		}
	}

	if (true == KEYPUSH("MoveLeft"))
	{
		if (PS_CLIMB == m_ePlayerState || PS_CLIMBIDLE == m_ePlayerState)
		{
			return;
		}

		/*if (m_pCameraArm->RotateY(-PI_HALF, _fTime))
		{
			m_pTransform->RotateY(-PI_HALF, _fTime);
		}*/		
		m_pCameraArm->RotateY(-PI_HALF, _fTime);		
		m_pTransform->RotateY(-PI_HALF, _fTime);
		
	}
	if (true == KEYPUSH("MoveRight"))
	{
		if (PS_CLIMB == m_ePlayerState || PS_CLIMBIDLE == m_ePlayerState)
		{
			return;
		}

		/*if (m_pCameraArm->RotateY(PI_HALF, _fTime))
		{
			m_pTransform->RotateY(PI_HALF, _fTime);
		}*/
		m_pCameraArm->RotateY(PI_HALF, _fTime);
		m_pTransform->RotateY(PI_HALF, _fTime);
	}
}

void CPlayer::MoveForward(float _fTime)
{
	m_vPrePos = m_pTransform->GetWorldPos();
	m_pTransform->Forward(m_fSpeed, _fTime);
	
	if (m_bFrontCol)
	{
		if (0.0f > m_vColAxis[AXIS_Z].Dot(m_pTransform->GetWorldAxis(AXIS_Z)))
		{
			m_pTransform->SetWorldPosZ(m_vPrePos.z);
		}
	}
	else if (m_bBackCol)
	{
		if (0.0f < m_vColAxis[AXIS_Z].Dot(m_pTransform->GetWorldAxis(AXIS_Z)))
		{
			m_pTransform->SetWorldPosZ(m_vPrePos.z);
		}
	}

	if (m_bLeftCol)
	{
		if (0.0f < m_vColAxis[AXIS_X].Dot(m_pTransform->GetWorldAxis(AXIS_Z)))
		{
			m_pTransform->SetWorldPosX(m_vPrePos.x);
		}
	}
	else if (m_bRightCol)
	{
		if (0.0f > m_vColAxis[AXIS_X].Dot(m_pTransform->GetWorldAxis(AXIS_Z)))
		{
			m_pTransform->SetWorldPosX(m_vPrePos.x);
		}
	}

	if (m_bJump)
	{
		return;
	}

	m_pAniController->ChangeClip("Run");
}

void CPlayer::MoveBack(float _fTime)
{
	m_vPrePos = m_pTransform->GetWorldPos();
	m_pTransform->Forward(-m_fSpeed * 0.5f, _fTime);

	if (m_bFrontCol)
	{
		if (0.0f < m_vColAxis[AXIS_Z].Dot(m_pTransform->GetWorldAxis(AXIS_Z)))
		{
			m_pTransform->SetWorldPosZ(m_vPrePos.z);
		}
	}
	else if (m_bBackCol)
	{
		if (0.0f > m_vColAxis[AXIS_Z].Dot(m_pTransform->GetWorldAxis(AXIS_Z)))
		{
			m_pTransform->SetWorldPosZ(m_vPrePos.z);
		}
	}

	if (m_bLeftCol)
	{
		if (0.0f > m_vColAxis[AXIS_X].Dot(m_pTransform->GetWorldAxis(AXIS_Z)))
		{
			m_pTransform->SetWorldPosX(m_vPrePos.x);
		}
	}
	else if (m_bRightCol)
	{
		if (0.0f < m_vColAxis[AXIS_X].Dot(m_pTransform->GetWorldAxis(AXIS_Z)))
		{
			m_pTransform->SetWorldPosX(m_vPrePos.x);
		}
	}

	m_pAniController->ChangeClip("Back");
}

void CPlayer::MoveUp(float _fTime)
{
	m_ePlayerState = PS_CLIMB;
	m_pTransform->Up(m_fSpeed * 0.3f, _fTime);
	//m_pAniController->ReturnToDefaultClip();

	float SrcY = m_pHitCol->GetSphereTop();
	//float DestY = m_pOtherCol.vCenter.y + m_pOtherCol.vScale.y;
	float DestY = ((CColliderAABB*)m_pOtherCol)->GetTop();

	if (SrcY + 0.3f> DestY)
	{
		// 여기서부터
		// 마지막 올라가는 상태로 바꿔주고
		// 그 상태 애니메이션이 끝나면
		// 오브젝트에 올라타서 지형처럼 행동 가능하게

		/*m_pTransform->SetWorldPosY(DestY);		
		m_ePlayerState = PS_DEFAULT;*/
		m_ePlayerState = PS_CLIMBTOUP;
		m_pAniController->ChangeClip("ClimbToTop");
		return;
	}

	m_pAniController->ChangeClip("ClimbUp");
}

void CPlayer::MoveDown(float _fTime)
{
	m_ePlayerState = PS_CLIMB;
	if (0.0f < m_pTransform->GetWorldPos().y)
	{
		m_pTransform->Up(-m_fSpeed * 0.3f, _fTime);
	}
	else
	{
		m_pTransform->SetWorldPosY(0.0f);
		m_pAniController->ChangeClip("Hanging2");
		return;
	}
	
	m_pAniController->ChangeClip("ClimbDown");
}

void CPlayer::PlayerAttack()
{
	if (true == m_pAniController->CheckClipName("Attack"))
	{
		if (20 <= m_pAniController->GetAnimationProgressFrame())
		{
			m_pAttCol->SetIsEnable(true);
		}

		if (true == m_pAniController->GetAnimationEnd())
		{
			m_pAniController->ReturnToDefaultClip();
			m_pAttCol->SetIsEnable(false);
			m_bAttack = false;
			m_ePlayerState = PS_DEFAULT;
		}
	}
	else
	{
		m_pAniController->ChangeClip("Attack");
		m_bAttack = true;
	}
}

void CPlayer::PlayerJump(float _fTime)
{
	if (false == m_pAniController->CheckClipName("Jump"))
	{
		m_bJump = true;
		m_pAniController->ChangeClip("Jump");
	}

	//y=-a*x+b에서 (a: 중력가속도, b: 초기 점프속도)
	//적분하여 y = (-a/2)*x*x + (b*x) 공식을 얻는다.(x: 점프시간, y: 오브젝트의 높이)
	//변화된 높이 height를 기존 높이 _posY에 더한다.
	float height = (m_fJumpTime * m_fJumpTime * (-m_fGravity) / 2) + (m_fJumpTime * m_fJumpPower);
	if (0.0f > height)
	{
 		m_bFall = true;
	}
	//_transform.position = new Vector3(_transform.position.x, _posY + height, _transform.position.z);
	m_pTransform->Up(height);
	//m_pTransform->SetLocalPosY(0.0f);
	//점프시간을 증가시킨다.
	m_fJumpTime += _fTime;

	//처음의 높이 보다 더 내려 갔을때 => 점프전 상태로 복귀한다.
	if (0.0f > m_pTransform->GetWorldPos().y)
	{
		m_ePlayerState = PS_DEFAULT;
		m_fJumpTime = 0.0f;
		m_pTransform->SetWorldPosY(0.0f);
		//m_pTransform->SetLocalPosY(m_fy * 0.5f);

		m_bJump = false;
		m_bFall = false;
		m_pAniController->ReturnToDefaultClip();

		//m_pLandingSound->MyPlaySound("Landing.aiff");
	}

	m_pFootCol->SetSphereInfo(m_pTransform->GetWorldPos(), 0.5f);
}

void CPlayer::PlayerMovingJump(float _fTime)
{
	if (true == m_pAniController->CheckClipName("Jump"))
	{
		if (true == m_pAniController->GetAnimationEnd())
		{
			m_pAniController->ReturnToDefaultClip();
			m_bJump = false;
			m_ePlayerState = PS_DEFAULT;
		}

		m_pTransform->Forward(m_fSpeed, _fTime);
	}
	else
	{
		m_pAniController->ChangeClip("Jump");
		m_bJump = true;
	}
}

void CPlayer::PlayerClimbIdle()
{
	m_pTransform->SetLocalPos(0.0f, m_fy * 0.5f - 0.25f, -0.5f);
	m_pAniController->ChangeClip("Hanging2");
}

void CPlayer::PlayerFall(float _fTime)
{
	if (!m_bFall)
	{
		return;
	}

	if (0.0f < m_pTransform->GetWorldPos().y)
	{
		float height = (m_fJumpTime * m_fJumpTime * (-m_fGravity * 0.5f) / 2) + (m_fJumpTime * m_fJumpPower);
		//_transform.position = new Vector3(_transform.position.x, _posY + height, _transform.position.z);
		m_pTransform->Up(height);
		//점프시간을 증가시킨다.
		m_fJumpTime += _fTime;
	}
	else
	{
		m_ePlayerState = PS_DEFAULT;
		m_fJumpTime = 0.0f;
		m_pTransform->SetWorldPosY(0.0f);
		m_bFall = false;
	}

	m_pFootCol->SetSphereInfo(m_pTransform->GetWorldPos(), 0.5f);
	//float height = (m_fJumpTime * m_fJumpTime * (-m_fGravity) / 2) + (m_fJumpTime * m_fJumpPower);
	////_transform.position = new Vector3(_transform.position.x, _posY + height, _transform.position.z);
	//m_pTransform->Up(height);
	////점프시간을 증가시킨다.
	//m_fJumpTime += _fTime;


	//if (0.0f > m_pTransform->GetWorldPos().y)
	//{
	//	m_ePlayerState = PS_DEFAULT;
	//	m_fJumpTime = 0.0f;
	//	m_pTransform->SetWorldPosY(0.0f);
	//}
}

void CPlayer::PlayerHit(float _fTime)
{
	m_bHeat = true;
	m_fHeatTime += _fTime;

	if (0.5f <= m_fHeatTime)
	{
		m_bHeat = false;
		m_fHeatTime = 0.0f;
		m_ePlayerState = PS_DEFAULT;
	}
}

void CPlayer::PlayerClimbToTop()
{
	m_pTransform->SetLocalPosZ(-0.5f);
	m_bClimbToTop = true;

	if (m_pAniController->GetAnimationEnd())
	{
		float DestY = ((CColliderAABB*)m_pOtherCol)->GetTop();
		m_pTransform->SetWorldPos(m_pHitCol->GetSphereInfo().vCenter.x,	DestY,
			m_pHitCol->GetSphereInfo().vCenter.z);
		m_pTransform->Forward(1.5f);
		m_bClimbToTop = false;
		m_ePlayerState = PS_DEFAULT;

		m_pFootCol->SetIsEnable(true);
	}
}

void CPlayer::PlayerDeath()
{
	m_bDeath = true;
	if (m_pAniController->CheckClipName("DeathLoop"))
	{		
		return;
	}

	if (0.9f <= m_pAniController->GetAnimationProgress())
	{
		// Message Box
		CGameObject* pMsgObj = CGameObject::Create("MsgObj");

		CLayer* pUILayer = m_pScene->FindLayer("UILayer");
		pUILayer->AddObject(pMsgObj);

		CMessageBox* pMsgScript = pMsgObj->AddComponent<CMessageBox>("MsgScript");
		SAFE_RELEASE(pMsgScript);

		SAFE_RELEASE(pUILayer);
		SAFE_RELEASE(pMsgObj);

		m_pAniController->ChangeClip("DeathLoop");		
	}
}

void CPlayer::CreatePlayerLight()
{
	CGameObject* pLightObject = m_pScene->CreateLight("SpotLight", LT_PLAYERLIGHT);

	CTransform* pLightTransform = pLightObject->GetTransform();
	pLightTransform->SetWorldPos(30.0f, 10.0f, 15.0f);
	SAFE_RELEASE(pLightTransform);

	CPlayerLight* pPlayerLight = (CPlayerLight*)pLightObject->FindComponentFromType(CT_LIGHT);

	LIGHTINFO	tLightInfo = {};
	tLightInfo.eType = LT_PLAYERLIGHT;
	tLightInfo.vDiffuse = { 0.7f, 0.7f, 0.7f, 1.f };
	tLightInfo.vAmbient = { 0.7f, 0.7f, 0.7f, 1.f };
	tLightInfo.vSpecular = { 0.7f, 0.7f, 0.7f, 1.f };
	tLightInfo.vAttenuation = DxVector3(1.2f, 0.0f, 0.0f);

	pPlayerLight->SetLightInfo(tLightInfo);
	pPlayerLight->SetAttachObject(m_pTransform);
	SAFE_RELEASE(pPlayerLight);

	//CLayer*		pUILayer = m_pScene->FindLayer("UILayer");
	//pUILayer->AddObject(pLightObject);
	SAFE_RELEASE(pLightObject);
	//SAFE_RELEASE(pUILayer);
}

void CPlayer::PlayerHitEffect()
{
	CLayer* pLayer = m_pScene->FindLayer("UILayer");

	CGameObject* pEffect = CGameObject::CreateClone("PlayerHitEffect");

	CTransform* pTransform = pEffect->GetTransform();
	pTransform->SetWorldPos(m_pTransform->GetWorldPos());
	pTransform->Move(DxVector3(0.0f, 1.5f, 0.0f));
	SAFE_RELEASE(pTransform);

	pLayer->AddObject(pEffect);
	SAFE_RELEASE(pEffect);
	SAFE_RELEASE(pLayer);
}

CPlayer::CPlayer() :
	m_pChildTransform(NULL),
	m_fSpeed(5.0f),
	m_fHeatTime(0.0f),
	m_iHp(MAXHP),
	m_ePlayerState(PS_DEFAULT),
	m_bDeath(false),
	m_bAttack(false),
	m_bJump(false),
	m_bHeat(false),
	m_bChange(false),
	m_bFrontCol(false),
	m_bBackCol(false),
	m_bLeftCol(false),
	m_bRightCol(false),
	m_bFall(false),
	m_pAniController(NULL),
	m_pAttCol(NULL),
	m_pHitCol(NULL),
	m_pFootCol(NULL),
	m_pHpBar(NULL),
	m_pCameraArm(NULL),
	m_pOtherCol(NULL),
	m_pHitSound(NULL),
	m_pAttackSound(NULL),
	m_pWarningSound(NULL),
	m_pGearSound(NULL),
	m_bClimb(false),
	m_bClimbToTop(false)
{
	SetTypeID<CPlayer>();
	SetTypeName("CPlayer");

	// Jump
	m_fJumpTime = 0.0f;
	m_fJumpPower = 2.0f;
	m_fGravity = 9.8f;
}

CPlayer::~CPlayer()
{		
	SAFE_RELEASE(m_pGearSound);
	SAFE_RELEASE(m_pWarningSound);
	SAFE_RELEASE(m_pAttackSound);
	SAFE_RELEASE(m_pHitSound);

	SAFE_RELEASE(m_pChildTransform);
	SAFE_RELEASE(m_pOtherCol);

	SAFE_RELEASE(m_pCameraArm);
	SAFE_RELEASE(m_pHitCol);
	SAFE_RELEASE(m_pAttCol);
	SAFE_RELEASE(m_pFootCol);
	SAFE_RELEASE(m_pAniController);
	SAFE_RELEASE(m_pHpBar);
}