#pragma once
#include "Engine_Header.h"

WOOJUN_USING

class CStageMgr
{
private:
	int m_iCurStage;
public:
	int GetCurStage();
	void StartStage();
	void NextStage();

	DECLARE_SINGLE(CStageMgr)
};

