#include "Timer.h"
#include "Debug.h"

WOOJUN_USING

bool CTimer::Init()
{
	QueryPerformanceFrequency(&m_tSecond);
	QueryPerformanceCounter(&m_tCount);

	m_fDeltaTime = 0.0f;
	m_fFPS = 0.0f;
	m_fFPSTime = 0.0f;

	m_iFrame = 0;
	m_iFrameMax = 60;

	return true;
}

void CTimer::Update()
{
	LARGE_INTEGER	tCount;

	QueryPerformanceCounter(&tCount);

	m_fDeltaTime = (float)(tCount.QuadPart - m_tCount.QuadPart) / (float)m_tSecond.QuadPart;

	m_tCount = tCount;

	m_fFPSTime += m_fDeltaTime;

	++m_iFrame;
	if (m_iFrame == m_iFrameMax)
	{
		m_fFPS = m_iFrame / m_fFPSTime;
		m_iFrame = 0;
		m_fFPSTime = 0.0f;

		char	strFPS[64] = {};
		sprintf_s(strFPS, sizeof(strFPS), "FPS : %.2f", m_fFPS);
		CDebug::OutputTitle(strFPS);
	}
}

CTimer::CTimer()
{
}

CTimer::~CTimer()
{
}
