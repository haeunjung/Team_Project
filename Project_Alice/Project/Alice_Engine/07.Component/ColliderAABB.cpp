#include "ColliderAABB.h"
#include "Camera.h"
#include "Transform.h"
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

bool CColliderAABB::Init()
{
	SetMesh("ColorBox");
	SetShader("ColliderColorShader");
	SetInputLayout("ColorInputLayout");

	m_tAABBInfo.vMin = m_pMesh->GetMeshMin();
	m_tAABBInfo.vMax = m_pMesh->GetMeshMax();
	m_tAABBInfo.vCenter = (m_tAABBInfo.vMin + m_tAABBInfo.vMax) * 0.5f;
	
	return true;
}

void CColliderAABB::Input(float _fTime)
{
}

void CColliderAABB::Update(float _fTime)
{
}

void CColliderAABB::LateUpdate(float _fTime)
{
}

void CColliderAABB::Collision(float _fTime)
{
}

void CColliderAABB::Render(float _fTime)
{
#ifdef _DEBUG
	CCamera*	pCamera = m_pScene->GetMainCamera();

	XMMATRIX	matScale, matRot, matPos;

	//DxVector3 vScale = m_tAABBInfo.vMax - m_tAABBInfo.vMin;
	DxVector3 vScale = m_pTransform->GetWorldScale() * 1.2f;

	// 크기
	matScale = XMMatrixScaling(vScale.x, vScale.y, vScale.z);

	// 자전 - 영향 없음
	DxVector3 vRot = m_pTransform->GetWorldRot();
	matRot = m_pTransform->GetWorldRotMatrix().mat;

	// 이동
	DxVector3 vPos = m_pTransform->GetWorldPos();
	matPos = XMMatrixTranslation(vPos.x, vPos.y, vPos.z);

	// 공전 부모 - 없음

	// 상수버퍼 세팅
	//m_tTransform.matWorld = matScale * matPos;
	m_tTransform.matWorld = matScale * matRot * matPos;
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

CColliderAABB * CColliderAABB::Clone()
{
	return nullptr;
}

bool CColliderAABB::Collision(CCollider * _pCollider)
{
	return false;
}
