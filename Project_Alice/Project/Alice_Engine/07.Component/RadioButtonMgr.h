#pragma once
#include "../Engine_Header.h"
#include "RadioButton.h"

WOOJUN_BEGIN

class DLL CRadioButtonMgr
{
private:
	unordered_map<string, vector<CRadioButton*>>	m_RadioButtonMgr;
	unordered_map<string, int>	m_RadioButtonMgrIndex;
public:
	int GetRadioButtonMgrIndex(const string _strKey);
private:
	void CreateRadioButtonMgr(const string& _strKey, CRadioButton* _pRadioButton);
public:
	void PushRadioButton(const string& _strKey, CRadioButton* _pRadioButton);
	void ButtonCheck(const string& _strKey, CRadioButton* const _pRadioButton);
	DECLARE_SINGLE(CRadioButtonMgr)
};

WOOJUN_END