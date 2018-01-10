#pragma once
#include "Tool_Header.h"
#include "06.GameObject/GameObject.h"

class CToolValue
{
public:
	CGameObject* pPlayerObj;
	CGameObject* pToolObject;
public:
	string CStringToString(CString _CString);
	void ChangeToolObject(CGameObject* _pToolObject);

	DECLARE_SINGLE(CToolValue);
};

