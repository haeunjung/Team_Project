#include "Player.h"
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

#include "RotBullet.h"
#include "PlayerBullet.h"
#include "../ClientMgr/UIMgr.h"

void CPlayer::AniCallback(float _fTime)
{
	int a = 0;
}

bool CPlayer::Init()
{		
	m_pTransform->SetWorldPos(30.0f, 0.0f, 15.0f);
	m_pTransform->SetWorldScale(0.05f, 0.05f, 0.05f);
	m_pTransform->SetLocalRotY(-PI * 0.5f);

	CRenderer* pPlayerRenderer = m_pGameObject->AddComponent<CRenderer>("Renderer");
	pPlayerRenderer->SetMesh("PlayerMesh", L"Elin.msh");
	pPlayerRenderer->SetShader(STANDARD_ANI_BUMP_SHADER);
	pPlayerRenderer->SetInputLayout("AniBumpInputLayout");	
	SAFE_RELEASE(pPlayerRenderer);

	m_pAniController = (CAnimation3D*)m_pGameObject->FindComponentFromType(CT_ANIMATION3D);
	//m_pAniController->AddClipCallback<CPlayer>("Run", 0.5f, this, &CPlayer::AniCallback);

	/*CColliderSphere*	pSphere = m_pGameObject->AddComponent<CColliderSphere>("PlayerAtt");
	pSphere->SetSphereInfo(Vector3(0.0f, 0.0f, 0.0f), 0.3f);
	pSphere->SetBoneIndex(m_pAniController->FindBoneIndex("HandR"));
	SAFE_RELEASE(pSphere);*/

	m_pAttCol = m_pGameObject->AddComponent<CColliderSphere>("PlayerAtt");
	m_pAttCol->SetSphereInfo(Vector3(0.0f, 0.0f, 0.0f), 0.5f);
	m_pAttCol->SetBoneIndex(m_pAniController->FindBoneIndex("HandR"));
	m_pAttCol->SetColCheck(CC_PLAYER_ATT);
	m_pAttCol->SetIsEnable(false);
	
	m_pHitCol = m_pGameObject->AddComponent<CColliderSphere>("PlayerHit");
	m_pHitCol->SetSphereInfo(0.0f, 0.0f, 0.0f, 0.5f);
	m_pHitCol->SetBoneIndex(m_pAniController->FindBoneIndex("Bip01-Spine1"));
	m_pHitCol->SetColCheck(CC_PLAYER_HIT);

	m_pFootCol = m_pGameObject->AddComponent<CColliderSphere>("PlayerFoot");
	m_pFootCol->SetSphereInfo(m_pTransform->GetWorldPos(), 0.5f);
	m_pFootCol->SetColCheck(CC_PLAYER_FOOT);

	CGameObject* pCameraObject = m_pScene->GetMainCameraObject();
	
	m_pCameraArm = (CCameraArm*)pCameraObject->FindComponentFromType(CT_CAMERAARM);	
	SAFE_RELEASE(pCameraObject);

	CreatePlayerLight();

	GET_SINGLE(CInput)->CreateKey("MoveForward", VK_UP);
	GET_SINGLE(CInput)->CreateKey("MoveBack", VK_DOWN);
	GET_SINGLE(CInput)->CreateKey("MoveLeft", VK_LEFT);
	GET_SINGLE(CInput)->CreateKey("MoveRight", VK_RIGHT);

	GET_SINGLE(CInput)->CreateKey("Init", VK_CONTROL, VK_MENU);	
	GET_SINGLE(CInput)->CreateKey("Attack", VK_CONTROL);
	GET_SINGLE(CInput)->CreateKey("Jump", VK_SPACE);
	GET_SINGLE(CInput)->CreateKey("ChangeCamera", 'C');
	GET_SINGLE(CInput)->CreateKey("HitPlayer", VK_F5);

	return true;
}

void CPlayer::Input(float _fTime)
{
	if (m_bAttack || m_bJump || m_bHeat)
	{
		return;
	}

	if (true == KEYPRESS("Attack"))
	{
		m_ePlayerState = PS_ATTACK;
		return;
	}

	PlayerMove(_fTime);

	if (KEYPRESS("Jump")
		&& KEYPUSH("MoveForward"))
	{
		if (!m_bFrontCol && !m_bBackCol &&
			!m_bLeftCol && !m_bRightCol)
		{
			m_ePlayerState = PS_MOVINGJUMP;
			return;
		}		
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
	switch (m_ePlayerState)
	{
	case PS_DEFAULT:
		break;
	case PS_ATTACK:
		PlayerAttack();
		break;
	case PS_JUMP:
		PlayerJump();
		break;
	case PS_MOVINGJUMP:
		PlayerMovingJump(_fTime);
		break;
	case PS_CLIMB:
		PlayerClimb();
		break;
	case PS_FALL:
		PlayerFall(_fTime);
		break;
	case PS_HEAT:
		PlayerHeat(_fTime);
		break;
	}
}

void CPlayer::OnCollisionStay(CCollider * _pSrc, CCollider * _pDest, float _fTime)
{
	if (CC_PLAYER_HIT == _pSrc->GetColliderCheck())
	{
		if ("MonsterAttCol" == _pDest->GetTagStr() &&
			true == _pDest->GetIsEnable())
		{
			m_iHp -= 1;
			GET_SINGLE(CUIMgr)->SetHp(m_iHp);

			CCamera* pCamera = m_pScene->GetMainCamera();
			pCamera->ActionCameraOn();
			SAFE_RELEASE(pCamera);

			m_ePlayerState = PS_HEAT;

			_pDest->SetIsEnable(false);
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
		m_bUpCol = true;
	}
}

void CPlayer::OnCollisionLeave(CCollider * _pSrc, CCollider * _pDest, float _fTime)
{
	if (CC_PLAYER_HIT == _pSrc->GetColliderCheck() &&
		CC_OBJ == _pDest->GetColliderCheck())
	{
		m_bFrontCol = false;
		m_bBackCol = false; 
		m_bLeftCol = false;
		m_bRightCol = false;

		m_pFootCol->SetIsEnable(true);
	}

	if (CC_PLAYER_FOOT == _pSrc->GetColliderCheck() &&
		CC_OBJ == _pDest->GetColliderCheck())
	{
		m_bUpCol = false;
		m_ePlayerState = PS_FALL;
	}
}

void CPlayer::PlayerMove(float _fTime)
{
	if (true == KEYPUSH("MoveForward"))
	{
		if (PS_CLIMB == m_ePlayerState)
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

		}
		else
		{
			m_pAniController->ReturnToDefaultClip();
		}		
	}

	if (true == KEYPUSH("MoveBack"))
	{
		if (PS_CLIMB == m_ePlayerState)
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

		}
		else
		{
			m_pAniController->ReturnToDefaultClip();
		}
	}

	if (true == KEYPUSH("MoveLeft"))
	{
		if (m_pCameraArm->RotateY(-PI_HALF, _fTime))
		{
			m_pTransform->RotateY(-PI_HALF, _fTime);
		}		
	}
	if (true == KEYPUSH("MoveRight"))
	{
		if (m_pCameraArm->RotateY(PI_HALF, _fTime))
		{
			m_pTransform->RotateY(PI_HALF, _fTime);
		}
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

	m_pAniController->ChangeClip("Run");
}

void CPlayer::MoveBack(float _fTime)
{
	m_vPrePos = m_pTransform->GetWorldPos();
	m_pTransform->Forward(-m_fSpeed, _fTime);

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

	m_pAniController->ChangeClip("Run");
}

void CPlayer::MoveUp(float _fTime)
{
	m_pTransform->Up(m_fSpeed, _fTime);
	m_pAniController->ReturnToDefaultClip();

	float SrcY = m_pHitCol->GetSphereTop();
	//float DestY = m_pOtherCol.vCenter.y + m_pOtherCol.vScale.y;
	float DestY = ((CColliderAABB*)m_pOtherCol)->GetTop();

	if (SrcY > DestY)
	{
		// 여기서부터
		// 마지막 올라가는 상태로 바꿔주고
		// 그 상태 애니메이션이 끝나면
		// 오브젝트에 올라타서 지형처럼 행동 가능하게
		int a = 0;

		m_pTransform->SetWorldPosY(DestY);
		m_ePlayerState = PS_DEFAULT;
	}
}

void CPlayer::MoveDown(float _fTime)
{
	if (0.0f < m_pTransform->GetWorldPos().y)
	{
		m_pTransform->Up(-m_fSpeed, _fTime);
	}
	else
	{
		m_pTransform->SetWorldPosY(0.0f);
	}
	
	m_pAniController->ReturnToDefaultClip();
}

void CPlayer::PlayerAttack()
{
	if (true == m_pAniController->CheckClipName("Attack01"))
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
		m_pAniController->ChangeClip("Attack01");
		m_bAttack = true;
	}
}

void CPlayer::PlayerJump()
{
	if (true == m_pAniController->CheckClipName("Jump"))
	{		
		if (true == m_pAniController->GetAnimationEnd())
		{
			m_pAniController->ReturnToDefaultClip();
			m_bJump = false;
			m_ePlayerState = PS_DEFAULT;
		}
	}
	else
	{
		m_pAniController->ChangeClip("Jump");
		m_bJump = true;
	}
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

void CPlayer::PlayerClimb()
{
	int a = 0;
}

void CPlayer::PlayerFall(float _fTime)
{
	if (true == m_bUpCol)
	{
		return;
	}

	if (0.0f < m_pTransform->GetWorldPos().y)
	{
		m_pTransform->Up(-m_fSpeed, _fTime);
	}
	else
	{
		m_pTransform->SetWorldPosY(0.0f);
	}
}

void CPlayer::PlayerHeat(float _fTime)
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

CPlayer::CPlayer() :
	m_fSpeed(5.0f),
	m_fHeatTime(0.0f),
	m_iHp(MAXHP),
	m_ePlayerState(PS_DEFAULT),
	m_bAttack(false),
	m_bJump(false),
	m_bHeat(false),
	m_bChange(false),
	m_bFrontCol(false),
	m_bBackCol(false),
	m_bLeftCol(false),
	m_bRightCol(false),
	m_bUpCol(false),
	m_pAniController(NULL),
	m_pAttCol(NULL),
	m_pHitCol(NULL),
	m_pFootCol(NULL),
	m_pHpBar(NULL),
	m_pCameraArm(NULL),
	m_pOtherCol(NULL),
	m_bClimb(false)
{
	SetTypeID<CPlayer>();
	SetTypeName("CPlayer");
}

CPlayer::~CPlayer()
{		
	SAFE_RELEASE(m_pOtherCol);

	SAFE_RELEASE(m_pCameraArm);
	SAFE_RELEASE(m_pHitCol);
	SAFE_RELEASE(m_pAttCol);
	SAFE_RELEASE(m_pFootCol);
	SAFE_RELEASE(m_pAniController);
	SAFE_RELEASE(m_pHpBar);
}
