#include "Light.h"

WOOJUN_USING

void CLight::SetLightInfo(const LIGHTINFO & _tInfo)
{
	LIGHT_TYPE eType = m_tInfo.eType;

	m_tInfo = _tInfo;
	m_tInfo.eType = eType;
}

void CLight::SetLightInfo(const DxVector4 & _vDiffuse, const DxVector4 & _vAmbient, const DxVector4 & _vSpecular)
{
	m_tInfo.vDiffuse = _vDiffuse;
	m_tInfo.vAmbient = _vAmbient;
	m_tInfo.vSpecular = _vSpecular;
}

void CLight::OnCollisionEnter(CCollider * _pSrc, CCollider * _pDest, float _fTime)
{
}

void CLight::OnCollisionStay(CCollider * _pSrc, CCollider * _pDest, float _fTime)
{
}

void CLight::OnCollisionLeave(CCollider * _pSrc, CCollider * _pDest, float _fTime)
{
}

void CLight::SetLight()
{
	// 상수버퍼 세팅
	m_tCBuffer.eType = m_tInfo.eType;
	m_tCBuffer.vDiffuse = m_tInfo.vDiffuse;
	m_tCBuffer.vAmbient = m_tInfo.vAmbient;
	m_tCBuffer.vSpecular = m_tInfo.vSpecular;
}

CLight::CLight()
{
	m_eComponentType = CT_LIGHT;

	m_tInfo.vDiffuse = DxVector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_tInfo.vAmbient = DxVector4(0.3f, 0.3f, 0.3f, 1.0f);
	m_tInfo.vSpecular = DxVector4(1.0f, 1.0f, 1.0f, 1.0f);
}

CLight::CLight(const CLight & _Light) :
	CComponent(_Light)
{
	m_tInfo = _Light.m_tInfo;
}

CLight::~CLight()
{
}