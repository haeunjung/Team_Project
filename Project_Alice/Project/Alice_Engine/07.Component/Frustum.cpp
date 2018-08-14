#include "Frustum.h"
#include "../06.GameObject/GameObject.h"
#include "../05.Scene/Scene.h"
#include "Camera.h"

WOOJUN_USING

CFrustum::CFrustum()
{
	SetTag("Frustum");
	SetTypeName("CFrustum");
	SetTypeID<CFrustum>();
	m_eComponentType = CT_FRUSTUM;
}

CFrustum::CFrustum(const CFrustum & _Frustum) :
	CComponent(_Frustum)
{
	memcpy(m_vPos, _Frustum.m_vPos, sizeof(DxVector3) * 8);
}

CFrustum::~CFrustum()
{
}

bool CFrustum::FrustumInPoint(const DxVector3 & _vPos)
{
	XMVECTOR	vDist;

	for (int i = 0; i < FP_END; ++i)
	{
		vDist = XMPlaneDotCoord(m_Plane[i], _vPos.Convert());
		DxVector3	v = vDist;

		if (v.x > 0.0f)
		{
			return true;
		}
	}

	return false;
}

bool CFrustum::FrustumInSphere(const SPHEREINFO & _tSphere)
{
	XMVECTOR	vDist;

	for (int i = 0; i < FP_END; ++i)
	{
		vDist = XMPlaneDotCoord(m_Plane[i], _tSphere.vCenter.Convert());
		DxVector3	v = vDist;

		if (v.x - _tSphere.fRadius > 0.0f)
		{
			return true;
		}
	}

	return false;
}

bool CFrustum::FrustumInSphere(const DxVector3 & _vCenter, float _fRad)
{
	XMVECTOR	vDist;

	for (int i = 0; i < FP_END; ++i)
	{
		vDist = XMPlaneDotCoord(m_Plane[i], _vCenter.Convert());
		DxVector3	v = vDist;

		if (v.x - _fRad > 0.0f)
		{
			return true;
		}
	}

	return false;
}

bool CFrustum::Init()
{
	m_vPos[0] = DxVector3(-1.f, 1.f, 0.f);
	m_vPos[1] = DxVector3(1.f, 1.f, 0.f);
	m_vPos[2] = DxVector3(-1.f, -1.f, 0.f);
	m_vPos[3] = DxVector3(1.f, -1.f, 0.f);

	m_vPos[4] = DxVector3(-1.f, 1.f, 1.f);
	m_vPos[5] = DxVector3(1.f, 1.f, 1.f);
	m_vPos[6] = DxVector3(-1.f, -1.f, 1.f);
	m_vPos[7] = DxVector3(1.f, -1.f, 1.f);

	//m_vPos[0] = DxVector3(-0.7f, 0.7f, 0.f);
	//m_vPos[1] = DxVector3(0.7f, 0.7f, 0.f);
	//m_vPos[2] = DxVector3(-0.7f, -0.7f, 0.f);
	//m_vPos[3] = DxVector3(0.7f, -0.7f, 0.f);

	//m_vPos[4] = DxVector3(-0.7f, 0.7f, 1.f);
	//m_vPos[5] = DxVector3(0.7f, 0.7f, 1.f);
	//m_vPos[6] = DxVector3(-0.7f, -0.7f, 1.f);
	//m_vPos[7] = DxVector3(0.7f, -0.7f, 1.f);

	return true;
}

void CFrustum::Input(float _fTime)
{
}

void CFrustum::Update(float _fTime)
{
}

void CFrustum::LateUpdate(float _fTime)
{
	// 투영 공간의 8개 정점을 월드공간으로 변환한다.
	// 로컬정점 * 월드행렬 * 뷰행렬 * 투영행렬 = 투영공간의 정점
	// 투영공간의 정점을 월드공간으로 변환하려면,
	// 투영행렬로 나누고 뷰행렬로 나눠줘야 한다.
	// 행렬의 나눗셈은 역행렬의 곱과 같다.
	DxVector3	vPos[8];

	// 카메라를 얻어온다.
	CCamera*	pCamera = m_pGameObject->FindComponentFromTypeID<CCamera>();

	// 투영행렬
	XMMATRIX	matProj;
	matProj = pCamera->GetProjMatrix();
	// 뷰행렬
	XMMATRIX	matView;
	matView = pCamera->GetViewMatrix();

	XMMATRIX	matVP = matView * matProj;
	matVP = XMMatrixInverse(&XMMatrixDeterminant(matVP), matVP);

	for (int i = 0; i < 8; ++i)
	{
		vPos[i] = m_vPos[i].TransformCoord(matVP);
	}

	// 평면을 구성한다.
	// Left
	m_Plane[FP_LEFT] = XMPlaneFromPoints(vPos[4].Convert(),
		vPos[0].Convert(), vPos[2].Convert());

	// Right
	m_Plane[FP_RIGHT] = XMPlaneFromPoints(vPos[1].Convert(),
		vPos[5].Convert(), vPos[7].Convert());

	// Top
	m_Plane[FP_TOP] = XMPlaneFromPoints(vPos[4].Convert(),
		vPos[5].Convert(), vPos[1].Convert());

	// Bottom
	m_Plane[FP_BOTTOM] = XMPlaneFromPoints(vPos[2].Convert(),
		vPos[3].Convert(), vPos[7].Convert());

	// Near
	m_Plane[FP_NEAR] = XMPlaneFromPoints(vPos[0].Convert(),
		vPos[1].Convert(), vPos[3].Convert());

	// Far
	m_Plane[FP_FAR] = XMPlaneFromPoints(vPos[5].Convert(),
		vPos[4].Convert(), vPos[7].Convert());

	SAFE_RELEASE(pCamera);
}

void CFrustum::Collision(float _fTime)
{
}

void CFrustum::Render(float _fTime)
{
}

CFrustum * CFrustum::Clone()
{
	return new CFrustum(*this);
}