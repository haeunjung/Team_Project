#include "Battery.h"
#include "../ClientMgr/UIMgr.h"
#include "05.Scene/Scene.h"
#include "05.Scene/Layer.h"
#include "06.GameObject/GameObject.h"
#include "07.Component/Transform.h"
#include "07.Component/Renderer.h"
#include "07.Component/ColliderSphere.h"

CBattery::CBattery() :
	m_fSpeed(10.0f)
{
}

CBattery::CBattery(const CBattery & _Battery) :
	CScript(_Battery)
{
	m_fSpeed = _Battery.m_fSpeed;
	m_IsCreate = _Battery.m_IsCreate;

	SAFE_RELEASE(m_pGetBatterySound);
	m_pGetBatterySound = _Battery.m_pGetBatterySound;
	m_pGetBatterySound->AddRef();
}

CBattery::~CBattery()
{
	SAFE_RELEASE(m_pGetBatterySound);
}

void CBattery::SetBatteryPos(const DxVector3 & _vPos)
{
	m_pTransform->SetWorldPos(_vPos);
}

bool CBattery::Init()
{	
	CTransform* pTransform = m_pGameObject->GetTransform();
	pTransform->SetWorldPos(0.0f, 0.0f, 0.0f);
	pTransform->SetWorldScale(0.1f, 0.1f, 0.1f);
	SAFE_RELEASE(pTransform);

	CRenderer* pRenderer = m_pGameObject->AddComponent<CRenderer>("BatteryRenderer");
	pRenderer->SetMesh("Battery", "battery.FBX");
	pRenderer->SetShader(STANDARD_BUMP_SHADER);
	pRenderer->SetInputLayout("BumpInputLayout");
	SAFE_RELEASE(pRenderer);

	CColliderSphere* pColSphere = m_pGameObject->AddComponent<CColliderSphere>("BatteryColSphere");
	pColSphere->SetSphereInfo(0.0f, 0.0f, 0.0f, 0.5f);
	SAFE_RELEASE(pColSphere);

	m_pGetBatterySound = m_pGameObject->AddComponent<CSoundPlayer>("GetBatterySound");

	return true;
}

void CBattery::Update(float _fTime)
{
	if (m_IsCreate)
	{
		m_pTransform->Up(m_fSpeed, _fTime);
		m_fSpeed -= _fTime * 20;
	}

	if (1.0f > m_pTransform->GetWorldPos().y)
	{
		m_pTransform->SetWorldPosY(1.0f);
		m_IsCreate = false;
	}

	m_pTransform->RotateX(PI, _fTime, true);
	m_pTransform->RotateZ(PI, _fTime, true);
}

CBattery * CBattery::Clone()
{
	m_IsCreate = true;
	return new CBattery(*this);
}

void CBattery::OnCollisionEnter(CCollider * _pSrc, CCollider * _pDest, float _fTime)
{
	if (m_IsCreate)
	{
		return;
	}

	if (CC_PLAYER_HIT == _pDest->GetColliderCheck())
	{
		m_pGetBatterySound->MyPlaySound("GetBattery.mp3");
		GET_SINGLE(CUIMgr)->GetBattery();
		m_pGameObject->Death();
	}
}

