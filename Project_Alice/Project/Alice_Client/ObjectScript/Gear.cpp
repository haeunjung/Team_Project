#include "Gear.h"
#include "../ClientMgr/UIMgr.h"
#include "05.Scene/Scene.h"
#include "05.Scene/Layer.h"
#include "06.GameObject/GameObject.h"
#include "07.Component/Transform.h"
#include "07.Component/Renderer.h"

CGear::CGear() :
	m_pGearCol(NULL)
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
	SAFE_RELEASE(m_pGearCol);
}

void CGear::SetGearPos(const DxVector3 & _vPos)
{
	m_pTransform->SetWorldPos(_vPos);
	m_pGearCol->SetSpherePos(_vPos);
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

	m_pGearCol = m_pGameObject->AddComponent<CColliderSphere>("GearColSphere");
	m_pGearCol->SetColCheck(CC_GEAR);
	m_pGearCol->SetSphereInfo(100.0f, 100.0f, 100.0f, 0.3f);

	return true;
}

void CGear::Update(float _fTime)
{
	m_pTransform->RotateY(PI, _fTime);
}

CGear * CGear::Clone()
{
	return new CGear(*this);
}