#include "ColliderPoint.h"
#include "Transform.h"
#include "ColliderRect.h"

WOOJUN_USING

CColliderPoint::CColliderPoint()
{
	SetTag("ColliderPoint");
	SetTypeName("CColliderPoint");
	SetTypeID<CColliderPoint>();

	m_eColType = COL_POINT;
}

CColliderPoint::CColliderPoint(const CColliderPoint & _ColliderPoint) :
	CCollider(_ColliderPoint)
{
	m_vPos = _ColliderPoint.m_vPos;
}

CColliderPoint::~CColliderPoint()
{
}

DxVector3 CColliderPoint::GetPoint() const
{
	return m_vPos;
}

bool CColliderPoint::Init()
{
	return true;
}

void CColliderPoint::Input(float _fTime)
{
}

void CColliderPoint::Update(float _fTime)
{
	m_vPos = m_pTransform->GetWorldPos();
}

void CColliderPoint::LateUpdate(float _fTime)
{
}

void CColliderPoint::Collision(float _fTime)
{
}

void CColliderPoint::Render(float _fTime)
{
}

CColliderPoint * CColliderPoint::Clone()
{
	return new CColliderPoint(*this);
}

bool CColliderPoint::Collision(CCollider * _pCollider)
{
	switch (_pCollider->GetColliderType())
	{
	case COL_RECT:
		return ColRectToPoint(((CColliderRect*)_pCollider)->GetRectInfo(), m_vPos);
	case COL_POINT:
		return m_vPos == ((CColliderPoint*)_pCollider)->m_vPos;
	}

	return false;
}
