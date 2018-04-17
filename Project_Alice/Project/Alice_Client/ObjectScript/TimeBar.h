#pragma once
#include "07.Component/Script.h"
#include "07.Component/UIBar.h"

WOOJUN_USING

class CTimeBar : public CScript
{
private:
	CUIBar * m_pUIBar;
	float m_fTime;
public:
	bool Init() override;
	void Update(float _fTime) override;
	CTimeBar* Clone() override;
public:
	CTimeBar();
	CTimeBar(const CTimeBar& _Spring);
	~CTimeBar();
};

