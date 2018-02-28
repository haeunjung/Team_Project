#include "UI.h"

WOOJUN_USING

CUI::CUI()
{
	SetTag("UI");
	SetTypeName("CUI");
	SetTypeID<CUI>();
	m_eComponentType = CT_UI;
}

CUI::CUI(const CUI & _UI) :
	CComponent(_UI)
{
	m_eUIType = _UI.m_eUIType;
}

CUI::~CUI()
{
}

UI_TYPE CUI::GetUIType() const
{
	return m_eUIType;
}

void CUI::OnCollisionEnter(CCollider * _pSrc, CCollider * _pDest, float _fTime)
{
}

void CUI::OnCollisionStay(CCollider * _pSrc, CCollider * _pDest, float _fTime)
{
}

void CUI::OnCollisionLeave(CCollider * _pSrc, CCollider * _pDest, float _fTime)
{
}