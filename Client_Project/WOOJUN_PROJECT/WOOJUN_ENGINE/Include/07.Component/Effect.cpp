#include "Effect.h"
#include "Transform.h"
#include "Camera.h"
#include "../05.Scene/Scene.h"
#include "../04.Rendering/ShaderMgr.h"
#include "Transform.h"

WOOJUN_USING

void CEffect::SetEffectCBuffer()
{
	GET_SINGLE(CShaderMgr)->UpdateConstBuffer("BillBoard",
		&m_tEffectCBuffer, CUT_VERTEX | CUT_GEOMETRY);
}

bool CEffect::Init()
{
	return true;
}

void CEffect::Input(float _fTime)
{
}

void CEffect::Update(float _fTime)
{		
}

void CEffect::LateUpdate(float _fTime)
{
	CCamera*	pCamera = m_pScene->GetMainCamera();
	CTransform*	pCameraTransform = pCamera->GetTransform();

	CTransform*	pParentTransform = m_pTransform->GetParentTransform();
	// 이펙트가 차일드 오브젝트라면
	// 부모의 상대적 위치가 아닌 자신의 절대적 위치를 상수버퍼에 업데이트해야한다
	if (NULL != pParentTransform)
	{
		// World의 절대좌표		
		DxVector3	vWorldPos = m_pTransform->GetWorldPos().TransformCoord(m_pTransform->GetParentMatrix());

		// 상수버퍼 업데이트
		m_tEffectCBuffer.vCenter = vWorldPos;
		
		// Parent Transform Release
		SAFE_RELEASE(pParentTransform);
	}
	else
	{
		m_tEffectCBuffer.vCenter = m_pTransform->GetWorldPos();
	}		
	
	m_tEffectCBuffer.vCamPos = pCameraTransform->GetWorldPos();
	m_tEffectCBuffer.vCamAxisY = pCamera->GetAxis(AXIS_Y);
	m_tEffectCBuffer.vSize.x = m_pTransform->GetWorldScale().x;
	m_tEffectCBuffer.vSize.y = m_pTransform->GetWorldScale().y;

	SAFE_RELEASE(pCameraTransform);
	SAFE_RELEASE(pCamera);
}

void CEffect::Collision(float _fTime)
{
}

void CEffect::Render(float _fTime)
{
}

CEffect * CEffect::Clone()
{
	return new CEffect(*this);
}

CEffect::CEffect()
{
	SetTag("Effect");
	SetTypeName("CEffect");
	SetTypeID<CEffect>();
	m_eComponentType = CT_EFFECT;
}

CEffect::CEffect(const CEffect & _Effect) :
	CComponent(_Effect)
{
}

CEffect::~CEffect()
{
}
