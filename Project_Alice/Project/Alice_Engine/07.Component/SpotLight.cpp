#include "SpotLight.h"
#include "Transform.h"
#include "../06.GameObject/GameObject.h"
#include "../07.Component/Renderer.h"
#include "../04.Rendering/ShaderMgr.h"
#include "../07.Component/ColliderSphere.h"
#include "../07.Component/ColliderRay.h"

WOOJUN_USING

void CSpotLight::InitSpotLightInfo()
{
	CTransform* pParentTransform = m_pGameObject->GetParentTransform();
	m_pTransform->SetWorldPos(pParentTransform->GetWorldPos());
	m_pTransform->SetWorldRotX(1.3f);

	DxVector3 StartPos = m_pTransform->GetWorldPos();
	DxVector3 Dir = m_pTransform->GetWorldAxis(AXIS_Z);

	bool b = true;
	while (b)
	{
		StartPos += Dir;

		if (0 >= StartPos.y)
		{
			float v = 0 - StartPos.y;
			StartPos += v;
			b = false;
		}
	}

	m_ColPos = StartPos;
	m_ColDist = m_pTransform->GetWorldPos().Distance(m_ColPos);

	SAFE_RELEASE(pParentTransform);

	float fPosY = m_pTransform->GetWorldPos().y;
		
	if (15.0f == fPosY)
	{
		m_pColSphere->SetSphereInfo(100.0f, 100.0f, 100.0f, 3.0f);
	}
	else if (20.0f == fPosY)
	{
		m_pColSphere->SetSphereInfo(100.0f, 100.0f, 100.0f, 4.0f);
	}	
	else
	{
		m_pColSphere->SetSphereInfo(100.0f, 100.0f, 100.0f, 2.0f);
	}
}

bool CSpotLight::Init()
{
	//CRenderer* pRenderer = m_pGameObject->AddComponent<CRenderer>("SpotLight");
	//pRenderer->SetMesh("ColorSphere");
	//pRenderer->SetShader("ColliderColorShader");
	//pRenderer->SetInputLayout("ColorInputLayout");
	//pRenderer->SetRenderState(WIRE_FRAME);
	//SAFE_RELEASE(pRenderer);

	m_pColSphere = m_pGameObject->AddComponent<CColliderSphere>("SpotColSphere");
	m_pColSphere->SetColCheck(CC_SPOTLIGHT);
	
	return true;
}

void CSpotLight::Input(float _fTime)
{
}

void CSpotLight::Update(float _fTime)
{
	//if (1.2f >= m_fRad)
	//{
	//	m_fScal = 0.1f;
	//}
	//if (PI_HALF <= m_fRad)
	//{
	//	m_fScal = -0.1f;
	//}
	//m_fRad += (_fTime * m_fScal);

	//m_pTransform->SetWorldRotX(m_fRad);

	//m_pColRay->SetRay(m_pTransform->GetWorldPos(), m_pTransform->GetWorldAxis(AXIS_Z));
	
	m_Dir = m_pTransform->GetWorldAxis(AXIS_Z);
	m_ColPos = m_pTransform->GetWorldPos() + (m_Dir * m_ColDist);
	m_pColSphere->SetSpherePos(m_ColPos);
}

void CSpotLight::LateUpdate(float _fTime)
{
}

void CSpotLight::Collision(float _fTime)
{
	//m_pColSphere->AddColList()
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
	int a = 0;
}

void CSpotLight::OnCollisionLeave(CCollider * _pSrc, CCollider * _pDest, float _fTime)
{
}

void CSpotLight::SetLight()
{
	// 부모 함수 호출
	CLight::SetLight();

	m_tCBuffer.vAttenuation = m_tInfo.vAttenuation;
	m_tCBuffer.fRange = 10000.0f;
	m_tCBuffer.fSpot = 48.0f;
	m_tCBuffer.vDir = m_Dir;

	// Transform의 Z축 == 조명의 방향
	// 역방향으로 조명으로의 방향
	//m_tCBuffer.vDir = m_pTransform->GetWorldAxis(AXIS_Z) * -1.0f;
	// 점조명의 위치
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