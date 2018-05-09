#include "UIBar.h"
#include "Transform.h"

WOOJUN_USING

CUIBar::CUIBar()
{
	SetTag("UIBar");
	SetTypeName("CUIBar");
	SetTypeID<CUIBar>();
	m_eUIType = UT_BAR;
}

CUIBar::CUIBar(const CUIBar & _Bar) :
	CUI(_Bar)
{
	m_fMin = _Bar.m_fMin;
	m_fMax = _Bar.m_fMax;
	m_fCurValue = _Bar.m_fCurValue;
	m_fValue = _Bar.m_fValue;
	m_fMoveValue = _Bar.m_fMoveValue;
	m_fSize = _Bar.m_fSize;
	m_eBarDir = _Bar.m_eBarDir;
}

CUIBar::~CUIBar()
{
}

void CUIBar::SetBarDir(BAR_DIR _eDir)
{
	m_eBarDir = _eDir;

	switch (_eDir)
	{
	case BD_LEFT:
		m_pTransform->SetPivot(0.0f, 0.0f, 0.0f);
		break;
	case BD_RIGHT:
		m_pTransform->SetPivot(1.0f, 0.0f, 0.0f);
		break;
	case BD_DOWN:
		m_pTransform->SetPivot(0.0f, 0.0f, 0.0f);
		break;
	case BD_UP:
		m_pTransform->SetPivot(0.0f, 1.0f, 0.0f);
		break;	
	}
}

void CUIBar::SetMinMax(float _fMin, float _fMax)
{
	m_fMin = _fMin;
	m_fMax = _fMax;
	m_fCurValue = _fMax;
}

void CUIBar::SetCurValue(float _fValue)
{
	float fPreValue = m_fCurValue;
	m_fCurValue = _fValue;

	if (m_fCurValue <= m_fMin)
	{
		m_fCurValue = m_fMin;
	}
	else if (m_fCurValue >= m_fMax)
	{
		m_fCurValue = m_fMax;
	}

	m_fMoveValue = m_fCurValue - fPreValue;
	m_fMoveValue = m_fMoveValue / (m_fMax - m_fMin);
}

void CUIBar::AddValue(float _fValue)
{
	m_fCurValue += _fValue;

	if (m_fCurValue <= m_fMin)
	{
		m_fCurValue = m_fMin;
	}
	else if (m_fCurValue >= m_fMax)
	{
		m_fCurValue = m_fMax;
	}

	m_fMoveValue = _fValue / (m_fMax - m_fMin);
}

DxVector3 CUIBar::ComputeScale(DxVector3 _vScale)
{
	switch (m_eBarDir)
	{
	case BD_LEFT:
	case BD_RIGHT:
		if (_vScale.x != 0.f)
		{
			_vScale.x = _vScale.x / (m_fValue * 10.f) * 10.f;
			if (m_fSize == 0.f)
				m_fSize = _vScale.x;
		}

		else
			_vScale.x = m_fSize;
		break;
	case BD_UP:
	case BD_DOWN:
		if (_vScale.y != 0.f)
		{
			_vScale.y = _vScale.y / (m_fValue * 10.f) * 10.f;
			if (m_fSize == 0.f)
				m_fSize = _vScale.y;
		}

		else
			_vScale.y = m_fSize;
		break;
	}

	return _vScale;
}

bool CUIBar::Init()
{
	m_fMin = 0.0f;
	m_fMax = 1.0f;
	m_fCurValue = 1.0f;
	m_fValue = 1.0f;
	m_fMoveValue = 0.0f;
	m_fSize = 0.0f;
	m_eBarDir = BD_LEFT;
	m_pTransform->SetPivot(0.0f, 0.0f, 0.0f);

	return true;
}

void CUIBar::Input(float _fTime)
{
}

void CUIBar::Update(float _fTime)
{
}

void CUIBar::LateUpdate(float _fTime)
{
	DxVector3	vScale = m_pTransform->GetWorldScale();

	vScale = ComputeScale(vScale);

	m_fValue += m_fMoveValue;
	m_fMoveValue = 0.0f;

	if (m_fValue > 1.0f)
	{
		m_fValue = 1.0f;
	}
	else if (m_fValue < 0.0f)
	{
		m_fValue = 0.0f;
	}

	switch (m_eBarDir)
	{
	case BD_LEFT:
		vScale.x *= m_fValue;
		break;
	case BD_RIGHT:
		vScale.x *= m_fValue;
		break;
	case BD_DOWN:
		vScale.y *= m_fValue;
		break;
	case BD_UP:
		vScale.y *= m_fValue;
		break;	
	}

	m_pTransform->SetWorldScale(vScale);
}

void CUIBar::Collision(float _fTime)
{
}

void CUIBar::Render(float _fTime)
{
}

CUIBar * CUIBar::Clone()
{
	return new CUIBar(*this);
}

void CUIBar::OnCollisionEnter(CCollider * _pSrc, CCollider * _pDest, float _fTime)
{
}

void CUIBar::OnCollisionStay(CCollider * _pSrc, CCollider * _pDest, float _fTime)
{
}

void CUIBar::OnCollisionLeave(CCollider * _pSrc, CCollider * _pDest, float _fTime)
{
}
