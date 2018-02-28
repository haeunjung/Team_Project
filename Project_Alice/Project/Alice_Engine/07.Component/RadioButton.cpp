#include "RadioButton.h"
#include "../01.Core/Input.h"
#include "../06.GameObject/GameObject.h"
#include "Renderer2D.h"
#include "Material.h"
#include "CheckBox.h"
#include "RadioButtonMgr.h"

WOOJUN_USING

void CRadioButton::RadioButtonOff()
{
	m_bClick = false;
}

bool CRadioButton::GetClick()
{
	return m_bClick;
}

bool CRadioButton::Init()
{
	SetNormalColor(0.5f, 0.5f, 0.5f, 1.0f);
	SetClickColor(1.0f, 1.0f, 1.0f, 1.0f);

	return true;
}

void CRadioButton::Input(float _fTime)
{
}

void CRadioButton::Update(float _fTime)
{
	if (false == m_bClick)
	{
		m_eButtonState = BS_NONE;
	}
	else
	{
		m_eButtonState = BS_CLICK;		
	}
}

void CRadioButton::LateUpdate(float _fTime)
{
	CRenderer2D*	pRenderer = m_pGameObject->FindComponentFromTypeID<CRenderer2D>();

	CMaterial*	pMaterial = pRenderer->GetMaterial(0, 0);

	switch (m_eButtonState)
	{
	case BS_NONE:
		pMaterial->SetDiffuseColor(m_vNormalCol);
		break;
	case BS_CLICK:
		pMaterial->SetDiffuseColor(m_vClickCol);
		break;
	}

	SAFE_RELEASE(pMaterial);
	SAFE_RELEASE(pRenderer);
}

void CRadioButton::Collision(float _fTime)
{
}

void CRadioButton::Render(float _fTime)
{
}

CRadioButton * CRadioButton::Clone()
{
	return new CRadioButton(*this);
}

void CRadioButton::OnCollisionEnter(CCollider * _pSrc, CCollider * _pDest, float _fTime)
{
}

void CRadioButton::OnCollisionStay(CCollider * _pSrc, CCollider * _pDest, float _fTime)
{
	if (true == KEYPUSH("LButton"))
	{
		m_bClick = !m_bClick;		

		GET_SINGLE(CRadioButtonMgr)->ButtonCheck("One", this);
		
		if (true == m_bCallback)
		{
			m_pCallback(m_pGameObject, _fTime);
		}
	}
}

void CRadioButton::OnCollisionLeave(CCollider * _pSrc, CCollider * _pDest, float _fTime)
{
}

CRadioButton::CRadioButton()
{
	SetTag("CRadioButton");
	SetTypeName("CRadioButton");
	SetTypeID<CRadioButton>();
	m_eUIType = UT_CHECKBOX;
	m_eButtonState = BS_NONE;

	m_bClick = false;
}

CRadioButton::CRadioButton(const CRadioButton & _RadioButton) :
	CUIButton(_RadioButton)
{
}

CRadioButton::~CRadioButton()
{
}