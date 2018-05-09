#include "PlayerLight.h"
#include "Transform.h"
#include "../01.Core/Input.h"
#include "../06.GameObject/GameObject.h"
#include "../04.Rendering/ShaderMgr.h"


WOOJUN_USING

CPlayerLight::CPlayerLight() :
	m_pAttachTransform(NULL),
	m_bLight(true)
{
	SetTag("PlayerLight");
	SetTypeName("CPlayerLight");
	SetTypeID<CPlayerLight>();
	m_tInfo.eType = LT_PLAYERLIGHT;
}

CPlayerLight::CPlayerLight(const CPlayerLight & _PlayerLight) :
	CLight(_PlayerLight)
{	
	SAFE_RELEASE(m_pAttachTransform);
	m_pAttachTransform = _PlayerLight.m_pAttachTransform;
	m_pAttachTransform->AddRef();
}

CPlayerLight::~CPlayerLight()
{
	SAFE_RELEASE(m_pAttachTransform);
}

void CPlayerLight::SetAttachObject(CTransform* _pAttachTransform)
{
	m_pAttachTransform = _pAttachTransform;
	m_pAttachTransform->AddRef();
}

bool CPlayerLight::Init()
{
	m_pTransform->SetWorldRotX(1.15f);
	m_vDir = m_pTransform->GetWorldAxis(AXIS_Z);

	return true;
}

void CPlayerLight::Input(float _fTime)
{
	if (KEYPRESS("PlayerLight"))
	{
		m_bLight = !m_bLight;
	}
}

void CPlayerLight::Update(float _fTime)
{	
	if (!m_bLight)
	{
		return;
	}

	if (m_pAttachTransform)
	{
		//m_pTransform->LookAt(m_pAttachTransform);
		m_pTransform->SetWorldPos(m_pAttachTransform->GetWorldPos());
		m_pTransform->Move(DxVector3(0.0f, 10.0f, 0.0f));
	}	
}

void CPlayerLight::LateUpdate(float _fTime)
{
}

void CPlayerLight::Collision(float _fTime)
{
}

void CPlayerLight::Render(float _fTime)
{
}

CPlayerLight * CPlayerLight::Clone()
{
	return new CPlayerLight(*this);
}

void CPlayerLight::SetLight()
{
	if (!m_bLight)
	{
		return;
	}

	// 부모 함수 호출
	CLight::SetLight();

	m_tCBuffer.vAttenuation = m_tInfo.vAttenuation;
	m_tCBuffer.fRange = 100.0f;
	m_tCBuffer.fSpot = 48.0f;
	m_tCBuffer.vDir = m_vDir;

	// Transform의 Z축 == 조명의 방향
	// 역방향으로 조명으로의 방향
	//m_tCBuffer.vDir = m_pTransform->GetWorldAxis(AXIS_Z) * -1.0f;
	// 점조명의 위치
	m_tCBuffer.vPos = m_pTransform->GetWorldPos();

	GET_SINGLE(CShaderMgr)->UpdateConstBuffer("Light", &m_tCBuffer, CUT_VERTEX | CUT_PIXEL);
}
