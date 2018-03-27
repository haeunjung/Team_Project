#include "Component.h"
#include "../06.GameObject/GameObject.h"
#include "../05.Scene/Scene.h"
#include "../05.Scene/Layer.h"
#include "Transform.h"

WOOJUN_USING

CGameObject * CComponent::GetGameObject() const
{
	m_pGameObject->AddRef();
	return m_pGameObject;
}

CTransform * CComponent::GetTransform() const
{
	m_pTransform->AddRef();
	return m_pTransform;
}

void CComponent::OnCollisionEnter(CCollider * _pSrc, CCollider * _pDest, float _fTime)
{
}

void CComponent::OnCollisionStay(CCollider * _pSrc, CCollider * _pDest, float _fTime)
{
}

void CComponent::OnCollisionLeave(CCollider * _pSrc, CCollider * _pDest, float _fTime)
{
}

CComponent::CComponent() :
	m_pScene(NULL),
	m_pLayer(NULL),
	m_pGameObject(NULL),
	m_pTransform(NULL)
{
}

CComponent::CComponent(const CComponent & _Component)
{
	*this = _Component;
}

CComponent::~CComponent()
{
}
