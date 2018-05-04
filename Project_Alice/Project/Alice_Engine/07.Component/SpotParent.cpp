#include "SpotParent.h"
#include "../06.GameObject/GameObject.h"
#include "../07.Component/Transform.h"
#include "../07.Component/SpotLight.h"
#include "../07.Component/Renderer.h"

WOOJUN_USING

CSpotParent::CSpotParent() :
	m_pChild(NULL),
	m_pChildSpotLight(NULL),
	m_pAttachObj(NULL)
{
	SetTag("SpotParent");
	SetTypeName("CSpotParent");
	SetTypeID<CSpotParent>();
	m_tInfo.eType = LT_SPOTPARENT;
}

CSpotParent::CSpotParent(const CSpotParent & _SpotParent) :
	CLight(_SpotParent)
{
	SAFE_RELEASE(m_pChild);
	m_pChild = _SpotParent.m_pChild;
	m_pChild->AddRef();

	SAFE_RELEASE(m_pChildSpotLight);
	m_pChildSpotLight = _SpotParent.m_pChildSpotLight;
	m_pChildSpotLight->AddRef();

	if (m_pAttachObj)
	{
		SAFE_RELEASE(m_pAttachObj);
		m_pAttachObj = _SpotParent.m_pAttachObj;
		m_pAttachObj->AddRef();
	}
}

CSpotParent::~CSpotParent()
{
	SAFE_RELEASE(m_pAttachObj);
	SAFE_RELEASE(m_pChildSpotLight);
	SAFE_RELEASE(m_pChild);
}

void CSpotParent::SetLightPos(const DxVector3 & _vPos)
{
	m_pTransform->SetWorldPos(_vPos);
	m_pTransform->Move(DxVector3(0.0f, 10.0f, 0.0f));

	CTransform* pChildTransform = m_pChild->GetTransform();
	pChildTransform->SetWorldPos(_vPos);
	pChildTransform->Move(DxVector3(0.0f, 10.0f, 0.0f));
	SAFE_RELEASE(pChildTransform);
}

void CSpotParent::AttachObject(CGameObject * _pGameObject)
{
	SAFE_RELEASE(m_pAttachObj);
	m_pAttachObj = _pGameObject;
	m_pAttachObj->AddRef();
}

bool CSpotParent::Init()
{
	//CRenderer* pRenderer = m_pGameObject->AddComponent<CRenderer>("SpotLight");
	//pRenderer->SetMesh("ColorSphere");
	//pRenderer->SetShader("ColliderColorShader");
	//pRenderer->SetInputLayout("ColorInputLayout");
	//pRenderer->SetRenderState(WIRE_FRAME);

	m_pChild = CGameObject::Create("SpotChild");

	m_pChildSpotLight = m_pChild->AddComponent<CSpotLight>("SpotLight");
	
	m_pGameObject->AddChild(m_pChild);

	return true;
}

void CSpotParent::Input(float _fTime)
{
}

void CSpotParent::Update(float _fTime)
{
	/*CTransform* pAttachTransform = m_pAttachObj->GetTransform();
	m_pTransform->SetWorldPos(pAttachTransform->GetWorldPos());
	m_pTransform->Move(DxVector3(0.0f, 10.0f, 10.0f));
	SAFE_RELEASE(pAttachTransform);*/

	CTransform* pChildTransform = m_pChild->GetTransform();
	pChildTransform->SetWorldPos(m_pTransform->GetWorldPos());
	//pChildTransform->LookAt(m_pAttachObj);
	SAFE_RELEASE(pChildTransform);

	//m_pTransform->RotateY(0.8f, _fTime);
}

void CSpotParent::LateUpdate(float _fTime)
{
}

void CSpotParent::Collision(float _fTime)
{
}

void CSpotParent::Render(float _fTime)
{
}

CSpotParent * CSpotParent::Clone()
{
	return new CSpotParent(*this);
}

void CSpotParent::OnCollisionEnter(CCollider * _pSrc, CCollider * _pDest, float _fTime)
{
}

void CSpotParent::OnCollisionStay(CCollider * _pSrc, CCollider * _pDest, float _fTime)
{
}

void CSpotParent::OnCollisionLeave(CCollider * _pSrc, CCollider * _pDest, float _fTime)
{
}

void CSpotParent::SetLight()
{
	m_pChildSpotLight->SetLightInfo(m_tInfo);
	m_pChildSpotLight->SetLight();
}
