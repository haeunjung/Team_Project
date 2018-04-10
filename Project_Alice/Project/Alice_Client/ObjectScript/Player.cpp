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
#include "07.Component/ColliderAABB.h"
#include "RotBullet.h"
#include "PlayerBullet.h"

void CPlayer::AniCallback(float _fTime)
{
	int a = 0;
}

bool CPlayer::Init()
{		
	CreateHpBar();

	CTransform*		pTransform = m_pGameObject->GetTransform();
	pTransform->SetWorldPos(30.0f, 0.0f, 20.0f);
	pTransform->SetWorldScale(0.05f, 0.05f, 0.05f);
	pTransform->SetLocalRotY(-PI * 0.5f);
	SAFE_RELEASE(pTransform);

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
	m_pAttCol->SetColCheck(CC_ATT);
	m_pAttCol->SetIsEnable(false);

	m_pHitCol = m_pGameObject->AddComponent<CColliderSphere>("PlayerHit");
	m_pHitCol->SetSphereInfo(0.0f, 0.0f, 0.0f, 0.5f);
	m_pHitCol->SetBoneIndex(m_pAniController->FindBoneIndex("Bip01-Spine1"));
	m_pHitCol->SetColCheck(CC_HIT);

	CGameObject* pCameraObject = m_pScene->GetMainCameraObject();
	
	m_pCameraArm = (CCameraArm*)pCameraObject->FindComponentFromType(CT_CAMERAARM);
	
	SAFE_RELEASE(pCameraObject);

	GET_SINGLE(CInput)->CreateKey("MoveForward", VK_UP);
	GET_SINGLE(CInput)->CreateKey("MoveBack", VK_DOWN);
	GET_SINGLE(CInput)->CreateKey("MoveLeft", VK_LEFT);
	GET_SINGLE(CInput)->CreateKey("MoveRight", VK_RIGHT);

	GET_SINGLE(CInput)->CreateKey("Init", VK_CONTROL, VK_MENU);	
	GET_SINGLE(CInput)->CreateKey("Attack", VK_CONTROL);
	GET_SINGLE(CInput)->CreateKey("Jump", 'Z');
	GET_SINGLE(CInput)->CreateKey("ChangeCamera", 'C');
	GET_SINGLE(CInput)->CreateKey("HitPlayer", VK_F5);

	return true;
}

void CPlayer::Input(float _fTime)
{
	if (m_bAttack || m_bJump)
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
		m_ePlayerState = PS_MOVINGJUMP;
		return;
	}

	if (true == KEYPRESS("Jump"))
	{
		m_ePlayerState = PS_JUMP;
		return;
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

	if (true == KEYPRESS("F1"))
	{
		if (0 < m_iHp)
		{
			m_iHp -= 10;
			m_pHpBar->SetCurValue(m_iHp);
		}
	}
	else if (true == KEYPRESS("F2"))
	{
		if (m_iHpMax > m_iHp)
		{
			m_iHp += 10;
			m_pHpBar->SetCurValue(m_iHp);
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
		Attack();
		break;
	case PS_JUMP:
		Jump();
		break;
	case PS_MOVINGJUMP:
		MovingJump(_fTime);
		break;
	}
}

void CPlayer::OnCollisionStay(CCollider * _pSrc, CCollider * _pDest, float _fTime)
{
	if (CC_HIT == _pSrc->GetColliderCheck())
	{
		if ("MonsterAttCol" == _pDest->GetTagStr() &&
			true == _pDest->GetIsEnable())
		{
			m_iHp -= 10;
			m_pHpBar->SetCurValue(m_iHp);
			_pDest->SetIsEnable(false);
		}
	}
	
	if (CC_HIT == _pSrc->GetColliderCheck() &&
		CC_OBJ == _pDest->GetColliderCheck())
	{		
		COL_AABB_POS Col = ((CColliderAABB*)_pDest)->GetAABBInfo().eColAABB;
		if (CAP_LEFT == Col)
		{
			m_bLeftCol = true;
		}
		else if(CAP_RIGHT == Col)
		{			
			m_bRightCol = true;
		}
		else if (CAP_FRONT == Col)
		{
			m_bFrontCol = true;
		}
		else if(CAP_BACK == Col)
		{
			m_bBackCol = true;
		}

		for (int i = 0; i < AXIS_MAX; ++i)
		{
			m_vColAxis[i] = _pDest->GetTransformWorldAxis((AXIS)i);
		}
	}
}

void CPlayer::OnCollisionLeave(CCollider * _pSrc, CCollider * _pDest, float _fTime)
{
	if (CC_HIT == _pSrc->GetColliderCheck() &&
		CC_OBJ == _pDest->GetColliderCheck())
	{
		m_bFrontCol = false;
		m_bBackCol = false; 
		m_bLeftCol = false;
		m_bRightCol = false;
	}
}

void CPlayer::PlayerMove(float _fTime)
{
	if (true == KEYPUSH("MoveForward"))
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
	else if (true == KEYUP("MoveForward"))
	{
		m_pAniController->ReturnToDefaultClip();
	}

	if (true == KEYPUSH("MoveBack"))
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
	else if (true == KEYUP("MoveBack"))
	{
		m_pAniController->ReturnToDefaultClip();
	}

	if (true == KEYPUSH("MoveLeft"))
	{
		m_pTransform->RotateY(-PI_HALF, _fTime);
		m_pCameraArm->RotateY(-PI_HALF, _fTime);
	}
	if (true == KEYPUSH("MoveRight"))
	{
		m_pTransform->RotateY(PI_HALF, _fTime);
		m_pCameraArm->RotateY(PI_HALF, _fTime);
	}
}

void CPlayer::CreateHpBar()
{
	CLayer*		pUILayer = m_pScene->FindLayer("UILayer");

	CGameObject*	pHpBarObejct = CGameObject::Create("HpBarObejct");

	CTransform*	pTransform = pHpBarObejct->GetTransform();
	DxVector3	vScale = { 400.0f, 50.0f, 1.0f };
	pTransform->SetWorldScale(vScale);
	pTransform->SetWorldPos(850.0f, 25.0f, 0.0f);
	pTransform->SetPivot(0.0f, 0.0f, 0.0f);
	SAFE_RELEASE(pTransform);

	CRenderer2D*	pRenderer = pHpBarObejct->AddComponent<CRenderer2D>("HpBarRenderer2D");
	pRenderer->SetMesh("UIMesh");
	pRenderer->SetShader(UI_SHADER);
	pRenderer->SetInputLayout("TexInputLayout");
	pRenderer->SetRenderState(ALPHABLEND);

	CMaterial*	pMaterial = pRenderer->GetMaterial();
	pMaterial->SetDiffuseTexture("Linear", "HpBar", L"SmallHpBar.png");
	SAFE_RELEASE(pMaterial);
	SAFE_RELEASE(pRenderer);

	m_pHpBar = pHpBarObejct->AddComponent<CUIBar>("HpBar");
	m_pHpBar->SetMinMax(0.0f, m_iHp);
	m_pHpBar->SetBarDir(BD_LEFT);

	pUILayer->AddObject(pHpBarObejct);
	SAFE_RELEASE(pHpBarObejct);

	SAFE_RELEASE(pUILayer);
}

void CPlayer::Attack()
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

void CPlayer::Jump()
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

void CPlayer::MovingJump(float _fTime)
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

CPlayer::CPlayer() :
	m_fSpeed(5.0f),
	m_iHp(100),
	m_iHpMax(100),
	m_ePlayerState(PS_DEFAULT),
	m_bAttack(false),
	m_bJump(false),
	m_bChange(false),
	m_bFrontCol(false),
	m_bBackCol(false),
	m_bLeftCol(false),
	m_bRightCol(false),
	m_pAniController(NULL),
	m_pAttCol(NULL),
	m_pHitCol(NULL),
	m_pHpBar(NULL),
	m_pCameraArm(NULL)
{
	SetTypeID<CPlayer>();
	SetTypeName("CPlayer");
}

CPlayer::~CPlayer()
{		
	SAFE_RELEASE(m_pCameraArm);
	SAFE_RELEASE(m_pHitCol);
	SAFE_RELEASE(m_pAttCol);
	SAFE_RELEASE(m_pAniController);
	SAFE_RELEASE(m_pHpBar);
}
