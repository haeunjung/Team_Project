#pragma once
#include "../BaseObj.h"

WOOJUN_BEGIN

class DLL CTimer : public CBaseObj
{
private:
	friend class CTimerMgr;
private:
	LARGE_INTEGER	m_tSecond;
	LARGE_INTEGER	m_tCount;

	float			m_fDeltaTime;
	float			m_fFPS;
	float			m_fFPSTime;
	int				m_iFrame;
	int				m_iFrameMax;
public:
	inline float GetDeltaTime() const {
		return m_fDeltaTime;
	}
	inline float GetFPS() const {
		return m_fFPS;
	}
public:
	bool Init();
	void Update();
private:
	CTimer();
	virtual ~CTimer();
};

WOOJUN_END