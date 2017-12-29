#include "Thread.h"
#include "ThreadQueue.h"

WOOJUN_USING

bool CThread::Init()
{
	m_bLoop = true;

	m_hStart = CreateEvent(NULL, FALSE, FALSE, NULL);

	m_hThread = (HANDLE)_beginthreadex(NULL, 0, CThread::ThreadFunc, this, 0, NULL);

	return true;
}

void CThread::WaitStart()
{
	WaitForSingleObject(m_hStart, INFINITE);
}

void CThread::Start()
{
	SetEvent(m_hStart);
}

CThread::CThread() :
	m_hThread(NULL),
	m_hStart(NULL)
{
}

CThread::~CThread()
{
	if (m_hStart)
	{
		CloseHandle(m_hStart);
	}

	if (m_hThread)
	{
		WaitForSingleObject(m_hThread, INFINITE);
		CloseHandle(m_hThread);
	}
}

UINT CThread::ThreadFunc(void * _pArg)
{
	CThread*	pThread = (CThread*)_pArg;

	pThread->WaitStart();

	while (pThread->m_bLoop)
	{
		pThread->Run();
	}

	return 0;
}
