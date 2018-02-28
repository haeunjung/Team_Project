#include "UIBack.h"

WOOJUN_USING

CUIBack::CUIBack()
{
	SetTag("Back");
	SetTypeName("CUIBack");
	SetTypeID<CUIBack>();
	m_eUIType = UT_BACK;
}

CUIBack::CUIBack(const CUIBack & _UIBack) :
	CUI(_UIBack)
{
}

CUIBack::~CUIBack()
{
}

bool CUIBack::Init()
{
	return true;
}

void CUIBack::Input(float _fTime)
{
}

void CUIBack::Update(float _fTime)
{
}

void CUIBack::LateUpdate(float _fTime)
{
}

void CUIBack::Collision(float _fTime)
{
}

void CUIBack::Render(float _fTime)
{
}

CUIBack * CUIBack::Clone()
{
	return new CUIBack(*this);
}

void CUIBack::OnCollisionEnter(CCollider * _pSrc, CCollider * _pDest, float _fTime)
{
}

void CUIBack::OnCollisionStay(CCollider * _pSrc, CCollider * _pDest, float _fTime)
{
}

void CUIBack::OnCollisionLeave(CCollider * _pSrc, CCollider * _pDest, float _fTime)
{
}
