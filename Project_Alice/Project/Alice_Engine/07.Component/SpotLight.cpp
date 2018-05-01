#include "SpotLight.h"
#include "Transform.h"

#include "../06.GameObject/GameObject.h"
#include "../07.Component/Renderer.h"
#include "../04.Rendering/ShaderMgr.h"
#include "../07.Component/ColliderSphere.h"
#include "../07.Component/ColliderRay.h"

WOOJUN_USING

bool CSpotLight::Init()
{
	//m_pTransform->Move(DxVector3(-5.0f, 0.0f, 0.0f));

	/*CColliderSphere*	pSphere = m_pGameObject->AddComponent<CColliderSphere>("PointLight");
	pSphere->SetSphereInfo(Vec3Zero, 0.25f);
	SAFE_RELEASE(pSphere);*/

	CRenderer* pRenderer = m_pGameObject->AddComponent<CRenderer>("SpotLight");
	pRenderer->SetMesh("ColorSphere");
	pRenderer->SetShader("ColliderColorShader");
	pRenderer->SetInputLayout("ColorInputLayout");
	pRenderer->SetRenderState(WIRE_FRAME);

	m_pColRay = m_pGameObject->AddComponent<CColliderRay>("SpotColRay");

	m_pColSphere = m_pGameObject->AddComponent<CColliderSphere>("SpotColSphere");
	m_pColSphere->SetSphereInfo(0.0f, 0.0f, 0.0f, 2.0f);

	SAFE_RELEASE(pRenderer);

	//m_pTransform->SetWorldRotY(PI_HALF);
	m_pTransform->SetWorldRotX(1.3f);

	return true;
}

void CSpotLight::Input(float _fTime)
{
}

void CSpotLight::Update(float _fTime)
{
	m_pColRay->SetRay(m_pTransform->GetWorldPos(), m_pTransform->GetWorldAxis(AXIS_Z));
	m_pColSphere->SetSphereInfo(m_pColRay->GetRay().vPos, 2.5f);

}

void CSpotLight::LateUpdate(float _fTime)
{
}

void CSpotLight::Collision(float _fTime)
{
}

void CSpotLight::Render(float _fTime)
{
}

CSpotLight * CSpotLight::Clone()
{
	return new CSpotLight(*this);
}

void CSpotLight::OnCollisionEnter(CCollider * _pSrc, CCollider * _pDest, float _fTime)
{
}

void CSpotLight::OnCollisionStay(CCollider * _pSrc, CCollider * _pDest, float _fTime)
{
}

void CSpotLight::OnCollisionLeave(CCollider * _pSrc, CCollider * _pDest, float _fTime)
{
}

void CSpotLight::SetLight()
{
	// �θ� �Լ� ȣ��
	CLight::SetLight();

	m_tCBuffer.vAttenuation = m_tInfo.vAttenuation;
	m_tCBuffer.fRange = 10000.0f;
	m_tCBuffer.fSpot = 8.0f;
	m_tCBuffer.vDir = m_pTransform->GetWorldAxis(AXIS_Z);

	// Transform�� Z�� == ������ ����
	// ���������� ���������� ����
	//m_tCBuffer.vDir = m_pTransform->GetWorldAxis(AXIS_Z) * -1.0f;
	// �������� ��ġ
	m_tCBuffer.vPos = m_pTransform->GetWorldPos();

	GET_SINGLE(CShaderMgr)->UpdateConstBuffer("Light", &m_tCBuffer, CUT_VERTEX | CUT_PIXEL);
}

CSpotLight::CSpotLight() :
	m_pColSphere(NULL),
	m_pColRay(NULL)
{	
	SetTag("SpotLight");
	SetTypeName("CSpotLight");
	SetTypeID<CSpotLight>();
	m_tInfo.eType = LT_SPOT;
}

CSpotLight::CSpotLight(const CSpotLight & _SpotLight) :
	CLight(_SpotLight)
{
}

CSpotLight::~CSpotLight()
{
	SAFE_RELEASE(m_pColSphere);
	SAFE_RELEASE(m_pColRay);
}