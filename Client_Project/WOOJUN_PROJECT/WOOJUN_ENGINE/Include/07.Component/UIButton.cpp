#include "UIButton.h"
#include "../01.Core/KeyMgr.h"
#include "../06.GameObject/GameObject.h"
#include "Renderer2D.h"
#include "Material.h"

WOOJUN_USING

void CUIButton::SetNormalColor(float _r, float _g, float _b, float _a)
{
	m_vNormalCol = DxVector4(_r, _g, _b, _a);
}

void CUIButton::SetMouseOnColor(float _r, float _g, float _b, float _a)
{
	m_vMouseOnCol = DxVector4(_r, _g, _b, _a);
}

void CUIButton::SetClickColor(float _r, float _g, float _b, float _a)
{
	m_vClickCol = DxVector4(_r, _g, _b, _a);
}

bool CUIButton::Init()
{
	SetNormalColor(0.8f, 0.8f, 0.8f, 0.8f);
	SetMouseOnColor(1.2f, 1.2f, 1.2f, 1.2f);
	SetClickColor(0.4f, 0.4f, 0.4f, 0.4f);

	return true;
}

void CUIButton::Input(float _fTime)
{
}

void CUIButton::Update(float _fTime)
{
}

void CUIButton::LateUpdate(float _fTime)
{
	CRenderer2D*	pRenderer = m_pGameObject->FindComponentFromTypeID<CRenderer2D>();

	CMaterial*	pMaterial = pRenderer->GetMaterial(0, 0);

	switch (m_eButtonState)
	{
	case BS_NONE:
		pMaterial->SetDiffuseColor(m_vNormalCol);
		break;
	case BS_MOUSEON:
		pMaterial->SetDiffuseColor(m_vMouseOnCol);
		break;
	case BS_CLICK:
		pMaterial->SetDiffuseColor(m_vClickCol);		
		if (true == m_bCallback)
		{
			m_pCallback(m_pGameObject, _fTime);
		}
		break;
	}

	SAFE_RELEASE(pMaterial);
	SAFE_RELEASE(pRenderer);
}

void CUIButton::Collision(float _fTime)
{
}

void CUIButton::Render(float _fTime)
{
}

CUIButton * CUIButton::Clone()
{
	return new CUIButton(*this);
}

void CUIButton::OnCollisionEnter(CCollider * _pSrc, CCollider * _pDest, float _fTime)
{
}

void CUIButton::OnCollisionStay(CCollider * _pSrc, CCollider * _pDest, float _fTime)
{
	if (true == KEYPUSH("LButton"))
	{
		m_eButtonState = BS_CLICK;
	}
	else
	{
		m_eButtonState = BS_MOUSEON;
	}
}

void CUIButton::OnCollisionLeave(CCollider * _pSrc, CCollider * _pDest, float _fTime)
{
	m_eButtonState = BS_NONE;	
}

CUIButton::CUIButton()
{
	SetTag("Button");
	SetTypeName("CUIButton");
	SetTypeID<CUIButton>();
	m_eUIType = UT_BUTTON;
	m_eButtonState = BS_NONE;
}

CUIButton::CUIButton(const CUIButton & _Button) :
	CUI(_Button)
{
	m_eButtonState = _Button.m_eButtonState;
	m_vNormalCol = _Button.m_vNormalCol;
	m_vMouseOnCol = _Button.m_vMouseOnCol;
	m_vClickCol = _Button.m_vClickCol;
}

CUIButton::~CUIButton()
{
}