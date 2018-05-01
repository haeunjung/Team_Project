#include "SpotLight.h"
#include "Transform.h"

#include "../06.GameObject/GameObject.h"
#include "../07.Component/Renderer.h"
#include "../04.Rendering/ShaderMgr.h"

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

	SAFE_RELEASE(pRenderer);

	m_pTransform->SetWorldRotY(PI_HALF);
	m_pTransform->SetWorldRotX(PI_HALF);

	return true;
}

void CSpotLight::Input(float _fTime)
{
}

void CSpotLight::Update(float _fTime)
{

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
	// 부모 함수 호출
	CLight::SetLight();

	m_tCBuffer.vAttenuation = m_tInfo.vAttenuation;
	m_tCBuffer.fRange = 1000.0f;
	m_tCBuffer.fSpot = 4.0f;
	m_tCBuffer.vDir = m_pTransform->GetWorldAxis(AXIS_Z);

	// Transform의 Z축 == 조명의 방향
	// 역방향으로 조명으로의 방향
	//m_tCBuffer.vDir = m_pTransform->GetWorldAxis(AXIS_Z) * -1.0f;
	// 점조명의 위치
	m_tCBuffer.vPos = m_pTransform->GetWorldPos();

	GET_SINGLE(CShaderMgr)->UpdateConstBuffer("Light", &m_tCBuffer, CUT_VERTEX | CUT_PIXEL);
}

CSpotLight::CSpotLight()
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
}