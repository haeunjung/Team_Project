#include "stdafx.h"
#include "ToolValue.h"

DEFINITION_SINGLE(CToolValue)

CToolValue::CToolValue()
{
}

CToolValue::~CToolValue()
{
}

string CToolValue::CStringToString(CString _CString)
{
	wstring GetString = _CString.GetString();
	return string(GetString.begin(), GetString.end());
}

CString CToolValue::StringToCString(string _String)
{	
	return CString(_String.c_str());
}

