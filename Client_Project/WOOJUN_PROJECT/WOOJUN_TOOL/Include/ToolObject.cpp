#include "stdafx.h"
#include "ToolObject.h"
#include "ToolValue.h"
#include "01.Core/Input.h"

bool CToolObject::Init()
{
	return true;
}

void CToolObject::Input(float _fTIme)
{

}

void CToolObject::Update(float _fTime)
{

}

CToolObject * CToolObject::Clone()
{
	return new CToolObject(*this);
}

void CToolObject::OnCollisionStay(CCollider * _pSrc, CCollider * _pDest, float _fTime)
{
	if (true == KEYPRESS("LButton"))
	{
		int a = 0;
	}	
}

CToolObject::CToolObject()
{
}

CToolObject::CToolObject(const CToolObject& _Object) :
	CScript(_Object)
{
}

CToolObject::~CToolObject()
{
}
