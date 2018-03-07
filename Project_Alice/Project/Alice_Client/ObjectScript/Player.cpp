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
#include "07.Component/ColliderSphere.h"
#include "07.Component/Material.h"
#include "07.Component/Effect.h"
#include "07.Component/Animation2D.h"
#include "RotBullet.h"
#include "PlayerBullet.h"

void CPlayer::AniCallback(float _fTime)
{
	int a = 0;
}

bool CPlayer::Init()
{		
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
	
	CColliderSphere*	pSphere = m_pGameObject->AddComponent<CColliderSphere>("Player");
	pSphere->SetSphereInfo(Vector3(30.0f, 1.0f, 20.0f), 0.5f);
	SAFE_RELEASE(pSphere);

	m_pAniController = (CAnimation3D*)m_pGameObject->FindComponentFromType(CT_ANIMATION3D);
	//m_pAniController->AddClipCallback<CPlayer>("Run", 0.5f, this, &CPlayer::AniCallback);

	GET_SINGLE(CInput)->CreateKey("MoveForward", VK_UP);
	GET_SINGLE(CInput)->CreateKey("MoveBack", VK_DOWN);
	GET_SINGLE(CInput)->CreateKey("RotLeft", VK_LEFT);
	GET_SINGLE(CInput)->CreateKey("RotRight", VK_RIGHT);

	GET_SINGLE(CInput)->CreateKey("Init", VK_CONTROL, VK_MENU);	
	GET_SINGLE(CInput)->CreateKey("Attack", VK_CONTROL);
	GET_SINGLE(CInput)->CreateKey("Jump", 'Z');

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

	if (true == KEYPUSH("MoveForward"))
	{
		if (true == KEYPRESS("Jump"))
		{
			m_ePlayerState = PS_MOVINGJUMP;
			return;
		}
		m_pTransform->Forward(m_fSpeed, _fTime);
		m_pAniController->ChangeClip("Run");
	}
	else if (true == KEYUP("MoveForward"))
	{
		m_pAniController->ReturnToDefaultClip();
	}

	if (true == KEYPUSH("MoveBack"))
	{
		m_pTransform->Forward(-m_fSpeed, _fTime);
		m_pAniController->ChangeClip("Run");
	}
	else if (true == KEYUP("MoveBack"))
	{
		m_pAniController->ReturnToDefaultClip();
	}

	if (true == KEYPUSH("RotLeft"))
	{
		m_pTransform->RotateY(-PI, _fTime);
	}
	if (true == KEYPUSH("RotRight"))
	{
		m_pTransform->RotateY(PI, _fTime);
	}

	if (true == KEYPRESS("Jump"))
	{
		m_ePlayerState = PS_JUMP;
		return;
	}

	if (true == KEYPRESS("Init"))
	{
		m_pTransform->SetWorldPos(DxVector3(0.0f, 0.0f, 0.0f));
		m_pTransform->SetWorldRot(DxVector3(0.0f, 0.0f, 0.0f));
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

void CPlayer::OnCollisionEnter(CCollider * _pSrc, CCollider * _pDest, float _fTime)
{
}

void CPlayer::Attack()
{
	if (true == m_pAniController->CheckClipName("Attack01"))
	{
		if (true == m_pAniController->GetAnimationEnd())
		{
			m_pAniController->ReturnToDefaultClip();
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
	m_ePlayerState(PS_DEFAULT),
	m_bAttack(false),
	m_bJump(false)
{
	SetTypeID<CPlayer>();
}

CPlayer::~CPlayer()
{		
	SAFE_RELEASE(m_pAniController);
}
