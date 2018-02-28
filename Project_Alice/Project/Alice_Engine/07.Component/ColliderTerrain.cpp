#include "ColliderTerrain.h"
#include "Transform.h"

WOOJUN_USING

CColliderTerrain::CColliderTerrain()
{
	SetTag("ColliderTerrain");
	SetTypeName("CColliderTerrain");
	SetTypeID<CColliderTerrain>();

	m_eColType = COL_TERRAIN;
}

CColliderTerrain::CColliderTerrain(const CColliderTerrain & _ColliderTerrain) :
	CCollider(_ColliderTerrain)
{
	m_tTerrainInfo = _ColliderTerrain.m_tTerrainInfo;
}

CColliderTerrain::~CColliderTerrain()
{
}

TERRAININFO CColliderTerrain::GetTerrainInfo() const
{
	return m_tTerrainInfo;
}

void CColliderTerrain::SetTerrainInfo(const vector<DxVector3>& _vecPos, UINT _iNumW, UINT _iNumH)
{
	m_tTerrainInfo.vecPos = _vecPos;
	m_tTerrainInfo.iNumW = _iNumW;
	m_tTerrainInfo.iNumH = _iNumH;
}

bool CColliderTerrain::Init()
{
	return true;
}

void CColliderTerrain::Input(float _fTime)
{
}

void CColliderTerrain::Update(float _fTime)
{
}

void CColliderTerrain::LateUpdate(float _fTime)
{
}

void CColliderTerrain::Collision(float _fTime)
{
}

void CColliderTerrain::Render(float _fTime)
{
}

CColliderTerrain * CColliderTerrain::Clone()
{
	return new CColliderTerrain(*this);
}

bool CColliderTerrain::Collision(CCollider * _pCollider)
{
	switch (_pCollider->GetColliderType())
	{
	case COL_SPHERE:
	{
		CTransform*	pTransform = _pCollider->GetTransform();
		bool bReturn = ColTerrainToPosition(m_tTerrainInfo, pTransform, m_pTransform->GetWorldScale());
		SAFE_RELEASE(pTransform);
		return bReturn;
	}
	}

	return false;
}
