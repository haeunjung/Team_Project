#include "Gear.h"
#include "05.Scene/Scene.h"
#include "05.Scene/Layer.h"
#include "06.GameObject/GameObject.h"
#include "07.Component/Transform.h"
#include "07.Component/Renderer.h"
#include "07.Component/ColliderSphere.h"

CGear::CGear()
{
	SetTag("Gear");
	SetTypeName("CGear");
	SetTypeID<CGear>();
}

CGear::CGear(const CGear & _Gear) :
	CScript(_Gear)
{
	*this = _Gear;
}

CGear::~CGear()
{
	SAFE_RELEASE(m_pGetGearSound);
}

void CGear::SetGearPos(const DxVector3 & _vPos)
{
	m_pTransform->SetWorldPos(_vPos);
}

bool CGear::Init()
{	
	CTransform* pTransform = m_pGameObject->GetTransform();
	pTransform->SetLocalRotX(PI_HALF);
	pTransform->SetWorldPos(0.0f, 0.0f, 0.0f);
	pTransform->SetWorldScale(0.07f, 0.07f, 0.07f);
	SAFE_RELEASE(pTransform);

	CRenderer* pRenderer = m_pGameObject->AddComponent<CRenderer>("GearRenderer");
	pRenderer->SetMesh("Gear", "Gear.FBX");
	pRenderer->SetShader(STANDARD_BUMP_SHADER);
	pRenderer->SetInputLayout("BumpInputLayout");
	SAFE_RELEASE(pRenderer);

	CColliderSphere* pColSphere = m_pGameObject->AddComponent<CColliderSphere>("GearColSphere");
	pColSphere->SetColCheck(CC_GEAR);
	pColSphere->SetSphereInfo(0.0f, 0.0f, 0.0f, 0.3f);
	SAFE_RELEASE(pColSphere);

	m_pGetGearSound = m_pGameObject->AddComponent<CSoundPlayer>("GetGearSound");

	return true;
}

void CGear::Update(float _fTime)
{
	m_pTransform->RotateY(PI, _fTime, true);
	//m_pTransform->RotateZ(PI, _fTime, true);
}

CGear * CGear::Clone()
{
	return new CGear(*this);
}

void CGear::OnCollisionEnter(CCollider * _pSrc, CCollider * _pDest, float _fTime)
{
	//if (CC_PLAYER_HIT == _pDest->GetColliderCheck())
	//{
	//	m_pGetGearSound->MyPlaySound("GetBattery.mp3");
	//	m_pGameObject->Death();
	//}
}
