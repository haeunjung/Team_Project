#include "ColliderSphere.h"
#include "Transform.h"
#include "Camera.h"
#include "ColliderRay.h"
#include "ColliderTerrain.h"
#include "ColliderAABB.h"
#include "Animation3D.h"
#include "../01.Core/Input.h"
#include "../05.Scene/Scene.h"
#include "../06.GameObject/GameObject.h"

WOOJUN_USING

SPHEREINFO CColliderSphere::GetSphereInfo() const
{
	return m_tSphereInfo;
}

float CColliderSphere::GetSphereTop() const
{
	return m_tSphereInfo.vCenter.y + m_tSphereInfo.fRadius;
}

float CColliderSphere::GetSphereBottom() const
{
	return m_tSphereInfo.vCenter.y - m_tSphereInfo.fRadius;
}

float CColliderSphere::GetSphereLeft() const
{
	return m_tSphereInfo.vCenter.x - m_tSphereInfo.fRadius;
}

float CColliderSphere::GetSphereRight() const
{
	return m_tSphereInfo.vCenter.x + m_tSphereInfo.fRadius;
}

void CColliderSphere::SetBoneIndex(int _BoneIndex)
{
	m_iBoneIndex = _BoneIndex;
	m_bAniCollider = true;
}

void CColliderSphere::SetSphereInfo(const SPHEREINFO & _tSphereInfo)
{
	m_tSphereInfo = _tSphereInfo;

	m_vPrevPos = m_pTransform->GetWorldPos();
}

void CColliderSphere::SetSphereInfo(const DxVector3 & _vCenter, float _fRadius)
{
	m_tSphereInfo.vCenter = _vCenter;
	m_tSphereInfo.fRadius = _fRadius;

	m_vPrevPos = m_pTransform->GetWorldPos();
}

void CColliderSphere::SetSphereInfo(float _fX, float _fY, float _fZ, float _fRadius)
{
	m_tSphereInfo.vCenter = DxVector3(_fX, _fY, _fZ);
	m_tSphereInfo.fRadius = _fRadius;

	m_vPrevPos = m_pTransform->GetWorldPos();
}

bool CColliderSphere::Init()
{
	SetMesh("ColorSphere");
	SetShader("ColliderColorShader");
	SetInputLayout("ColorInputLayout");

	return true;
}

void CColliderSphere::Input(float _fTime)
{
}

void CColliderSphere::Update(float _fTime)
{
	CCollider::Update(_fTime);
	m_tSphereInfo.vCenter += m_vMove;

	if (true == m_bAniCollider)
	{
		CAnimation3D* pAnimation3D = m_pGameObject->FindComponentFromTypeID<CAnimation3D>();
		MATRIX matBone = pAnimation3D->GetBoneMatrixFromIndex(m_iBoneIndex);
		//MATRIX matWorld = m_pTransform->GetWorldMatrix();
		//MATRIX matLocal = m_pTransform->GetLocalMatrix();

		// Position
		matBone = matBone * m_pTransform->GetLocalMatrix() * m_pTransform->GetWorldMatrix();

		DxVector3 vOffset;
		vOffset.x = matBone._41;
		vOffset.y = matBone._42;
		vOffset.z = matBone._43;

		m_tSphereInfo.vCenter = vOffset;

		SAFE_RELEASE(pAnimation3D);
	}
}

void CColliderSphere::LateUpdate(float _fTime)
{
}

void CColliderSphere::Collision(float _fTime)
{
}

void CColliderSphere::Render(float _fTime)
{
//#ifdef _DEBUG
	if (false == GET_SINGLE(CInput)->GetIsCollider())
		return;

	CCamera*	pCamera = m_pScene->GetMainCamera();

	XMMATRIX	matScale, matPos;

	// 크기
	matScale = XMMatrixScaling(m_tSphereInfo.fRadius * 2.0f, m_tSphereInfo.fRadius * 2.0f, m_tSphereInfo.fRadius * 2.0f);

	// 자전 - 영향 없음

	// 이동
	matPos = XMMatrixTranslation(m_tSphereInfo.vCenter.x, m_tSphereInfo.vCenter.y, m_tSphereInfo.vCenter.z);

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
//#endif // _DEBUG

	CCollider::Render(_fTime);
}

CColliderSphere * CColliderSphere::Clone()
{
	return new CColliderSphere(*this);
}

bool CColliderSphere::Collision(CCollider * _pCollider)
{
	switch (_pCollider->GetColliderType())
	{
	case COL_SPHERE:
		return ColSphereToSphere(m_tSphereInfo, ((CColliderSphere*)_pCollider)->m_tSphereInfo);
	case COL_RAY:
		return ColRayToSphere(((CColliderRay*)_pCollider)->GetRay(), m_tSphereInfo);
	case COL_TERRAIN:
	{
		CTransform*	pTransform = _pCollider->GetTransform();
		bool bReturn = ColTerrainToPosition(((CColliderTerrain*)_pCollider)->GetTerrainInfo(), m_pTransform, pTransform->GetWorldScale());
		SAFE_RELEASE(pTransform);
		return bReturn;
	}
	case COL_AABB:
		return ColSphereToAABB(m_tSphereInfo, ((CColliderAABB*)_pCollider)->GetAABBInfo());
	}
	return false;
}

CColliderSphere::CColliderSphere() :
	m_bAniCollider(false),
	m_iBoneIndex(0)
{
	m_eColType = COL_SPHERE;
	SetTag("ColliderSphere");
	SetTypeName("CColliderSphere");
	SetTypeID<CColliderSphere>();
}

CColliderSphere::CColliderSphere(const CColliderSphere & _ColliderSphere) :
	CCollider(_ColliderSphere)
{
	m_tSphereInfo = _ColliderSphere.m_tSphereInfo;
	m_bAniCollider = _ColliderSphere.m_bAniCollider;
	m_iBoneIndex = _ColliderSphere.m_iBoneIndex;
}

CColliderSphere::~CColliderSphere()
{
}
