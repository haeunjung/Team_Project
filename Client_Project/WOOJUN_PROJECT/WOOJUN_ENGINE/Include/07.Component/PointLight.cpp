#include "PointLight.h"
#include "Transform.h"

#include "../06.GameObject/GameObject.h"
#include "../07.Component/Renderer.h"
#include "../04.Rendering/ShaderMgr.h"

WOOJUN_USING

bool CPointLight::Init()
{
	//m_pTransform->Move(DxVector3(-5.0f, 0.0f, 0.0f));
	
	/*CColliderSphere*	pSphere = m_pGameObject->AddComponent<CColliderSphere>("PointLight");
	pSphere->SetSphereInfo(Vec3Zero, 0.25f);
	SAFE_RELEASE(pSphere);*/

	CRenderer* pRenderer = m_pGameObject->AddComponent<CRenderer>("PointLight");
	pRenderer->SetMesh("ColorSphere");
	pRenderer->SetShader("ColliderColorShader");
	pRenderer->SetInputLayout("ColorInputLayout");
	pRenderer->SetRenderState(WIRE_FRAME);

	SAFE_RELEASE(pRenderer);

	return true;
}

void CPointLight::Input(float _fTime)
{
}

void CPointLight::Update(float _fTime)
{
}

void CPointLight::LateUpdate(float _fTime)
{
}

void CPointLight::Collision(float _fTime)
{
}

void CPointLight::Render(float _fTime)
{
}

CPointLight * CPointLight::Clone()
{
	return new CPointLight(*this);
}

void CPointLight::OnCollisionEnter(CCollider * _pSrc, CCollider * _pDest, float _fTime)
{
}

void CPointLight::OnCollisionStay(CCollider * _pSrc, CCollider * _pDest, float _fTime)
{
}

void CPointLight::OnCollisionLeave(CCollider * _pSrc, CCollider * _pDest, float _fTime)
{
}

void CPointLight::SetLight()
{
	// 부모 함수 호출
	CLight::SetLight();

	m_tCBuffer.vAttenuation = m_tInfo.vAttenuation;
	m_tCBuffer.fRange = 10.f;

	// Transform의 Z축 == 조명의 방향
	// 역방향으로 조명으로의 방향
	//m_tCBuffer.vDir = m_pTransform->GetWorldAxis(AXIS_Z) * -1.0f;
	// 점조명의 위치
	m_tCBuffer.vPos = m_pTransform->GetWorldPos();

	GET_SINGLE(CShaderMgr)->UpdateConstBuffer("Light", &m_tCBuffer, CUT_VERTEX | CUT_PIXEL);
}

CPointLight::CPointLight()
{
	m_tInfo.vAttenuation = DxVector3(0.0f, 1.0f, 0.0f);

	SetTag("PointLight");
	SetTypeName("CPointDir");
	SetTypeID<CPointLight>();
	m_tInfo.eType = LT_POINT;
}

CPointLight::CPointLight(const CPointLight & _PointLight) :
	CLight(_PointLight)
{
}

CPointLight::~CPointLight()
{
}

