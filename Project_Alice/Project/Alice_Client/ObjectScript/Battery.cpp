#include "Battery.h"
#include "../ClientMgr/UIMgr.h"
#include "05.Scene/Scene.h"
#include "05.Scene/Layer.h"
#include "06.GameObject/GameObject.h"
#include "07.Component/Transform.h"
#include "07.Component/Renderer.h"
#include "07.Component/ColliderSphere.h"

CBattery::CBattery()
{
}

CBattery::CBattery(const CBattery & _Battery) :
	CScript(_Battery)
{
}

CBattery::~CBattery()
{
}

void CBattery::SetBatteryPos(const DxVector3 & _vPos)
{
	m_pTransform->SetWorldPos(_vPos);
}

bool CBattery::Init()
{	
	CTransform* pTransform = m_pGameObject->GetTransform();
	pTransform->SetWorldPos(35.0f, 1.0f, 15.0f);
	pTransform->SetWorldScale(0.1f, 0.1f, 0.1f);
	SAFE_RELEASE(pTransform);

	CRenderer* pRenderer = m_pGameObject->AddComponent<CRenderer>("BatteryRenderer");
	pRenderer->SetMesh("Battery", "battery.FBX");
	pRenderer->SetShader(STANDARD_BUMP_SHADER);
	pRenderer->SetInputLayout("BumpInputLayout");
	SAFE_RELEASE(pRenderer);

	CColliderSphere* pColSphere = m_pGameObject->AddComponent<CColliderSphere>("BatteryColSphere");
	pColSphere->SetSphereInfo(35.0f, 1.0f, 15.0f, 0.5f);
	SAFE_RELEASE(pColSphere);

	return true;
}

void CBattery::Update(float _fTime)
{
	m_pTransform->RotateX(3.14f, _fTime);
	m_pTransform->RotateZ(3.14f, _fTime);	
}

CBattery * CBattery::Clone()
{
	return new CBattery(*this);
}

void CBattery::OnCollisionEnter(CCollider * _pSrc, CCollider * _pDest, float _fTime)
{
	if (CC_PLAYER_HIT == _pDest->GetColliderCheck())
	{
		GET_SINGLE(CUIMgr)->GetBattery();
		m_pGameObject->Death();
	}
}

