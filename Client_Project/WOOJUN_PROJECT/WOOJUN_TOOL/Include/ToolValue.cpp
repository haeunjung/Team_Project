#include "stdafx.h"
#include "ToolValue.h"

DEFINITION_SINGLE(CToolValue)

CToolValue::CToolValue()
{

}

CToolValue::~CToolValue()
{
	SAFE_RELEASE(pPlayerObj);
	SAFE_RELEASE(pToolObject);
}

string CToolValue::CStringToString(CString _CString)
{
	wstring GetString = _CString.GetString();

	return string(GetString.begin(), GetString.end());
}

void CToolValue::ChangeToolObject(CGameObject * _pToolObject)
{
	SAFE_RELEASE(pToolObject);

	pToolObject = _pToolObject;
	pToolObject->AddRef();
}
