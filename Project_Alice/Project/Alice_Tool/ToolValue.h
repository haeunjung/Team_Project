#pragma once
#include "Tool_Header.h"

WOOJUN_USING

class CToolValue
{
public:
	class CObjectDialog*	m_pObjectDialog;
	class CAniDialog*		m_pAnimationDialog;
public:
	string CStringToString(CString _CString);	
	CString StringToCString(string _String);

	DECLARE_SINGLE(CToolValue);
};

