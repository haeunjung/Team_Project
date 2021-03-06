#include "ColliderAABB.h"
#include "Camera.h"
#include "Transform.h"
#include "ColliderSphere.h"
#include "../01.Core/Input.h"
#include "../03.Resource/Mesh.h"
#include "../05.Scene/Scene.h"
//#include "../06.GameObject/GameObject.h"

WOOJUN_USING

CColliderAABB::CColliderAABB() :
	m_tAABBInfo()
{
	m_eColType = COL_AABB;
	SetTag("ColliderAABB");
	SetTypeName("CColliderAABB");
	SetTypeID<CColliderAABB>();
}


CColliderAABB::~CColliderAABB()
{
}

CColliderAABB::CColliderAABB(const CColliderAABB & _ColliderSphere)
{
}

AABBINFO& CColliderAABB::GetAABBInfo()
{
	return m_tAABBInfo;
}

float CColliderAABB::GetTop() const
{
	return m_tAABBInfo.vCenter.y + (m_tAABBInfo.vScale.y * 0.5f);
}

float CColliderAABB::GetBottom() const
{
	return m_tAABBInfo.vCenter.y - (m_tAABBInfo.vScale.y * 0.5f);
}

float CColliderAABB::GetLeft() const
{
	return m_tAABBInfo.vCenter.x - (m_tAABBInfo.vScale.x * 0.5f);
}

float CColliderAABB::GetRight() const
{
	return m_tAABBInfo.vCenter.x + (m_tAABBInfo.vScale.x * 0.5f);
}

void CColliderAABB::SetScale(const DxVector3& _vMeshSize)
{
	m_tAABBInfo.vScale = _vMeshSize * m_pTransform->GetWorldScale();
}

bool CColliderAABB::Init()
{
	SetMesh("ColorBox");
	SetShader("ColliderColorShader");
	SetInputLayout("ColorInputLayout");

	return true;
}

void CColliderAABB::Input(float _fTime)
{
}

void CColliderAABB::Update(float _fTime)
{
	m_tAABBInfo.vCenter = m_pTransform->GetWorldPos();
	//m_tAABBInfo.vCenter.y -= 2.0f;
	//m_tAABBInfo.vScale = m_pTransform->GetWorldScale()/* * 0.95f*/;	
}

void CColliderAABB::LateUpdate(float _fTime)
{
}

void CColliderAABB::Collision(float _fTime)
{
}

void CColliderAABB::Render(float _fTime)
{
//#ifdef _DEBUG	
	if (false == GET_SINGLE(CInput)->GetIsCollider())
		return;

	CCamera*	pCamera = m_pScene->GetMainCamera();

	XMMATRIX	matScale, matRot, matPos;

	// 크기
	matScale = XMMatrixScaling(m_tAABBInfo.vScale.x, m_tAABBInfo.vScale.y, m_tAABBInfo.vScale.z);

	// 자전
	DxVector3 vRot = m_pTransform->GetWorldRot();
	matRot = m_pTransform->GetWorldRotMatrix().mat;

	// 이동
	matPos = XMMatrixTranslation(m_tAABBInfo.vCenter.x, m_tAABBInfo.vCenter.y, m_tAABBInfo.vCenter.z);

	// 공전 부모 - 없음

	// 상수버퍼 세팅
	m_tTransform.matWorld = matScale * matPos;
	//m_tTransform.matWorld = matScale * matRot * matPos;
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
//#endif // _DEBUG

	CCollider::Render(_fTime);
}

CColliderAABB * CColliderAABB::Clone()
{
	return nullptr;
}

bool CColliderAABB::Collision(CCollider * _pCollider)
{
	switch (_pCollider->GetColliderType())
	{
	case COL_SPHERE:
		return ColSphereToAABB(((CColliderSphere*)_pCollider)->GetSphereInfo(), m_tAABBInfo);
	}
	return false;
}
