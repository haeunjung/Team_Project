#include "ColliderRect.h"
#include "../05.Scene/Scene.h"
#include "07.Component/Camera.h"
#include "ColliderPoint.h"

WOOJUN_USING

CColliderRect::CColliderRect() :
	m_bFirst(true)
{
	SetTag("ColliderRect");
	SetTypeName("CColliderRect");
	SetTypeID<CColliderRect>();

	m_eColType = COL_RECT;
}

CColliderRect::CColliderRect(const CColliderRect & _ColliderRect) :
	CCollider(_ColliderRect)
{
	m_tRectInfo = _ColliderRect.m_tRectInfo;
}

CColliderRect::~CColliderRect()
{
}

RECTINFO CColliderRect::GetRectInfo() const
{
	return m_tRectInfo;
}

void CColliderRect::SetRectInfo(float _fTop, float _fBottom, float _fLeft, float _fRight, bool _bCenter)
{
	SetRectSize(_fRight - _fLeft, _fBottom - _fTop);

	if (true == _bCenter)
	{
		m_tRectInfo = RECTINFO(_fTop - m_tRectInfo.fSizeHY, _fBottom - m_tRectInfo.fSizeHY, _fLeft - m_tRectInfo.fSizeHX, _fRight - m_tRectInfo.fSizeHX);
	}
	else
	{
		m_tRectInfo = RECTINFO(_fTop, _fBottom, _fLeft, _fRight);
	}
	
	
}

void CColliderRect::SetRectSize(float _fSizeX, float _fSizeY)
{
	m_tRectInfo.fSizeX = _fSizeX;
	m_tRectInfo.fSizeY = _fSizeY;

	m_tRectInfo.fSizeHX = _fSizeX * 0.5f;
	m_tRectInfo.fSizeHY = _fSizeY * 0.5f;
}

bool CColliderRect::Init()
{
	SetMesh("RectLine");
	SetShader(COLLIDER_COLOR_SHADER);
	SetInputLayout("ColorInputLayout");	

	return true;
}

void CColliderRect::Input(float _fTime)
{
}

void CColliderRect::Update(float _fTime)
{
	CCollider::Update(_fTime);

	//if (true == m_bFirst)
	//{
	//	m_bFirst = false;
	//	return;
	//}

	m_tRectInfo.MoveRect(m_vMove.x, m_vMove.y);
	//// MoveRect 수정??
	//if (Vec3Zero != m_vMove)
	//{
	//	m_tRectInfo.MoveRect(m_vMove.x, m_vMove.y);
	//}	
}

void CColliderRect::LateUpdate(float _fTime)
{
}

void CColliderRect::Collision(float _fTime)
{
}

void CColliderRect::Render(float _fTime)
{
#ifdef _DEBUG
	CCamera*	pCamera = m_pScene->FindCamera("UICamera");

	XMMATRIX	matScale, matPos;

	// 크기
	matScale = XMMatrixScaling(m_tRectInfo.fRight - m_tRectInfo.fLeft, m_tRectInfo.fBottom - m_tRectInfo.fTop, 1.0f);

	// 자전 - 영향 없음

	// 이동
	matPos = XMMatrixTranslation(m_tRectInfo.fLeft, m_tRectInfo.fTop, 0.0f);

	// 공전 부모 - 없음

	// 상수버퍼 세팅
	m_tTransform.matWorld = matScale * matPos;
	m_tTransform.matView = pCamera->GetViewMatrix();
	m_tTransform.matProj = pCamera->GetProjMatrix();
	m_tTransform.matWV = m_tTransform.matWorld * m_tTransform.matView;
	m_tTransform.matWVP = m_tTransform.matWV* m_tTransform.matProj;

	SAFE_RELEASE(pCamera);

	m_tTransform.matWorld = XMMatrixTranspose(m_tTransform.matWorld);
	m_tTransform.matView = XMMatrixTranspose(m_tTransform.matView);
	m_tTransform.matProj = XMMatrixTranspose(m_tTransform.matProj);
	m_tTransform.matWV = XMMatrixTranspose(m_tTransform.matWV);
	m_tTransform.matWVP = XMMatrixTranspose(m_tTransform.matWVP);
#endif // _DEBUG

	CCollider::Render(_fTime);
}

CColliderRect * CColliderRect::Clone()
{
	return new CColliderRect(*this);
}

bool CColliderRect::Collision(CCollider * _pCollider)
{
	switch (_pCollider->GetColliderType())
	{
	case COL_RECT:
		return ColRectToRect(m_tRectInfo, ((CColliderRect*)_pCollider)->m_tRectInfo);
	case COL_POINT:
		return ColRectToPoint(m_tRectInfo, ((CColliderPoint*)_pCollider)->GetPoint());
	}

	return false;
}