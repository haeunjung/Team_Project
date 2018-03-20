#include "Minion.h"
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
	// 兄希君 持失
	//CRenderer*	pRenderer = m_pGameObject->AddComponent<CRenderer>("Renderer");
	//pRenderer->SetMesh("ColorPyramid");
	//pRenderer->SetShader("StandardColorShader");
	//pRenderer->SetInputLayout("ColorInputLayout");
	//SAFE_RELEASE(pRenderer);

	//CColliderSphere*	pSphere = m_pGameObject->AddComponent<CColliderSphere>("Minion");
	//pSphere->SetSphereInfo(Vec3Zero, 0.5f);
	//SAFE_RELEASE(pSphere);

	CTransform*		pTransform = m_pGameObject->GetTransform();
	pTransform->SetWorldPos(40.0f, 0.0f, 20.0f);
	pTransform->SetWorldScale(0.05f, 0.05f, 0.05f);
	pTransform->SetLocalRotY(-PI * 0.5f);
	SAFE_RELEASE(pTransform);

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
	// Bone25
	// FxCenter
	SAFE_RELEASE(pSphere);

	return true;
}

void CMinion::Update(float _fTime)
{
	//m_pTransform->LookAt(m_pPlayer, AXIS_Y);

	//m_pTransform->Forward(1.0f, _fTime);
	
}

CMinion * CMinion::Clone()
{
	return new CMinion(*this);
}

void CMinion::OnCollisionEnter(CCollider * _pSrc, CCollider * _pDest, float _fTime)
{		
	if ("PlayerAtt" == _pDest->GetTagStr())
	{
		_pDest->Death();
		m_pGameObject->Death();
	}
}

CMinion::CMinion() :
	m_pPlayer(NULL),
	m_pAniController(NULL),
	m_fTime(0.0f)
{
	SetTypeID<CMinion>();
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
	SAFE_RELEASE(m_pPlayer);
	SAFE_RELEASE(m_pAniController);
}
