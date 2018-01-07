#include "Effect.h"
#include "Transform.h"
#include "Camera.h"
#include "Renderer.h"
#include "Renderer2D.h"
#include "../04.Rendering/ShaderMgr.h"
#include "../05.Scene/Scene.h"
#include "../06.GameObject/GameObject.h"

WOOJUN_USING

void CEffect::SetEffectCBuffer()
{
	GET_SINGLE(CShaderMgr)->UpdateConstBuffer("BillBoard",
		&m_tEffectCBuffer, CUT_VERTEX | CUT_GEOMETRY);
}

bool CEffect::Init()
{
	// Renderer Component가 있다면
	if (true == m_pGameObject->CheckComponentFromTypeID<CRenderer>())
	{
		// Renderer에 Effect를 위한 상수버퍼 추가
		CRenderer*	pRenderer = m_pGameObject->FindComponentFromTypeID<CRenderer>();
		pRenderer->AddConstBuffer("BillBoard", 11, sizeof(EFFECTCBUFFER), CUT_VERTEX | CUT_GEOMETRY);

		SAFE_RELEASE(pRenderer);
	}
	else if (true == m_pGameObject->CheckComponentFromTypeID<CRenderer2D>())
	{
		// Renderer2D에 Effect를 위한 상수버퍼 추가
		CRenderer2D*	pRenderer2D = m_pGameObject->FindComponentFromTypeID<CRenderer2D>();
		pRenderer2D->AddConstBuffer("BillBoard", 11, sizeof(EFFECTCBUFFER), CUT_VERTEX | CUT_GEOMETRY);

		SAFE_RELEASE(pRenderer2D);
	}

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

	// Renderer Component가 있다면
	if (true == m_pGameObject->CheckComponentFromTypeID<CRenderer>())
	{
		// Renderer에 Effect 상수버퍼 업데이트
		CRenderer*	pRenderer = m_pGameObject->FindComponentFromTypeID<CRenderer>();
		pRenderer->UpdateCBuffer("BillBoard", &m_tEffectCBuffer);

		SAFE_RELEASE(pRenderer);
	}
	else if (true == m_pGameObject->CheckComponentFromTypeID<CRenderer2D>())
	{
		// Renderer2D에 Effect 상수버퍼 업데이트
		CRenderer2D*	pRenderer2D = m_pGameObject->FindComponentFromTypeID<CRenderer2D>();
		pRenderer2D->UpdateCBuffer("BillBoard", &m_tEffectCBuffer);

		SAFE_RELEASE(pRenderer2D);
	}
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
