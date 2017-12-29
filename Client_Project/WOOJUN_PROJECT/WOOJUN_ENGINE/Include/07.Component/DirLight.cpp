#include "DirLight.h"
#include "Transform.h"
#include "../04.Rendering/ShaderMgr.h"

WOOJUN_USING

CDirLight::CDirLight()
{
	SetTag("DirLight");
	SetTypeName("CDirLight");
	SetTypeID<CDirLight>();
	m_tInfo.eType = LT_DIR;
}

CDirLight::CDirLight(const CDirLight & _DirLight) :
	CLight(_DirLight)
{
}

CDirLight::~CDirLight()
{
}

bool CDirLight::Init()
{
	return true;
}

void CDirLight::Input(float _fTime)
{
}

void CDirLight::Update(float _fTime)
{
}

void CDirLight::LateUpdate(float _fTime)
{
}

void CDirLight::Collision(float _fTime)
{
}

void CDirLight::Render(float _fTime)
{
}

CDirLight * CDirLight::Clone()
{
	return new CDirLight(*this);
}

void CDirLight::OnCollisionEnter(CCollider * _pSrc, CCollider * _pDest, float _fTime)
{
}

void CDirLight::OnCollisionStay(CCollider * _pSrc, CCollider * _pDest, float _fTime)
{
}

void CDirLight::OnCollisionLeave(CCollider * _pSrc, CCollider * _pDest, float _fTime)
{
}

void CDirLight::SetLight()
{
	// �θ� �Լ� ȣ��
	CLight::SetLight();

	// Transform�� Z�� == ������ ����
	// ���������� ���������� ����
	m_tCBuffer.vDir = m_pTransform->GetWorldAxis(AXIS_Z) * -1.0f;

	GET_SINGLE(CShaderMgr)->UpdateConstBuffer("Light", &m_tCBuffer, CUT_VERTEX | CUT_PIXEL);
}
