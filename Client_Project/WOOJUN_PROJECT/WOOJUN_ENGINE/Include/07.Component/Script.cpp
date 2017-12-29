#include "Script.h"

WOOJUN_USING

CScript::CScript()
{
	m_eComponentType = CT_SCRIPT;
}

CScript::CScript(const CScript & _Script) : CComponent(_Script)
{
}

CScript::~CScript()
{
}

void CScript::Input(float _fTime)
{
}

void CScript::Update(float _fTime)
{
}

void CScript::LateUpdate(float _fTime)
{
}

void CScript::Collision(float _fTime)
{
}

void CScript::Render(float _fTime)
{
}

CScript * CScript::Clone()
{
	return nullptr;
}

void CScript::OnCollisionEnter(CCollider * _pSrc, CCollider * _pDest, float _fTime)
{
}

void CScript::OnCollisionStay(CCollider * _pSrc, CCollider * _pDest, float _fTime)
{
}

void CScript::OnCollisionLeave(CCollider * _pSrc, CCollider * _pDest, float _fTime)
{
}
