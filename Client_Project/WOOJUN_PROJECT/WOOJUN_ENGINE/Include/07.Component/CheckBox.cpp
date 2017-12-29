#include "CheckBox.h"
#include "../01.Core/KeyMgr.h"
#include "../06.GameObject/GameObject.h"
#include "Renderer2D.h"
#include "Material.h"

WOOJUN_USING

bool CCheckBox::Init()
{
	SetNormalColor(0.5f, 0.5f, 0.5f, 1.0f);
	SetClickColor(1.0f, 1.0f, 1.0f, 1.0f);

	return true;
}

void CCheckBox::Input(float _fTime)
{
}

void CCheckBox::Update(float _fTime)
{
	if (false == m_bCheck)
	{
		m_eButtonState = BS_NONE;
	}
	else
	{
		m_eButtonState = BS_CLICK;
	}
}

void CCheckBox::LateUpdate(float _fTime)
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

void CCheckBox::Collision(float _fTime)
{
}

void CCheckBox::Render(float _fTime)
{
}

CCheckBox * CCheckBox::Clone()
{
	return new CCheckBox(*this);
}

void CCheckBox::OnCollisionEnter(CCollider * _pSrc, CCollider * _pDest, float _fTime)
{
}

void CCheckBox::OnCollisionStay(CCollider * _pSrc, CCollider * _pDest, float _fTime)
{
	if (true == KEYPUSH("LButton"))
	{
		m_bCheck = !m_bCheck;

		if (true == m_bCallback)
		{
			m_pCallback(m_pGameObject, _fTime);
		}
	}
}

void CCheckBox::OnCollisionLeave(CCollider * _pSrc, CCollider * _pDest, float _fTime)
{	
}

CCheckBox::CCheckBox()
{
	SetTag("CheckBox");
	SetTypeName("CCheckBox");
	SetTypeID<CCheckBox>();
	m_eUIType = UT_CHECKBOX;
	m_eButtonState = BS_NONE;

	m_bCheck = false;
}

CCheckBox::CCheckBox(const CCheckBox & _CheckBox) :
	CUIButton(_CheckBox)
{
	m_bCheck = _CheckBox.m_bCheck;
}

CCheckBox::~CCheckBox()
{
}
