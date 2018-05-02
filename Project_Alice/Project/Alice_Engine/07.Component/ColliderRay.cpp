#include "ColliderRay.h"
#include "Camera.h"
#include "Transform.h"
#include "ColliderSphere.h"
#include "ColliderTerrain.h"
#include "../05.Scene/Scene.h"

WOOJUN_USING

RAY CColliderRay::GetRay() const
{
	return m_tRay;
}

void CColliderRay::SetRay(const RAY & _Ray)
{
	m_tRay = _Ray;
}

void CColliderRay::SetRay(const DxVector3 & _Pos, const DxVector3 & _Dir)
{
	m_tRay.vPos = _Pos;
	m_tRay.vDir = _Dir;
}

bool CColliderRay::Init()
{
	return true;
}

void CColliderRay::Input(float _fTime)
{
}

void CColliderRay::Update(float _fTime)
{
}

void CColliderRay::LateUpdate(float _fTime)
{
	if (m_tRay.bIntersect)
	{
		int a = 0;
	}

	m_tRay.bIntersect = false;
}

void CColliderRay::Collision(float _fTime)
{	
}

CCollider * CColliderRay::Clone()
{
	return new CColliderRay(*this);
}

bool CColliderRay::Collision(CCollider * pCollider)
{
	switch (pCollider->GetColliderType())
	{
	case COL_SPHERE:
		return ColRayToSphere(m_tRay, ((CColliderSphere*)pCollider)->GetSphereInfo());
	case COL_TERRAIN:
	{
		/*CTransform*	pTransform = pCollider->GetTransform();
		bool bReturn = ColTerrainToRay(((CColliderTerrain*)pCollider)->GetTerrainInfo(), m_tRay, pTransform->GetWorldScale());
		SAFE_RELEASE(pTransform);
		return bReturn;*/
	}
	}

	return false;
}

CColliderRay::CColliderRay()
{
	m_eColType = COL_RAY;
	SetTag("ColliderRay");
	SetTypeName("CColliderRay");
	SetTypeID<CColliderRay>();

	m_tRay.bIntersect = false;
}

CColliderRay::CColliderRay(const CColliderRay& _ColliderRay) :
	CCollider(_ColliderRay)
{
}

CColliderRay::~CColliderRay()
{
}
