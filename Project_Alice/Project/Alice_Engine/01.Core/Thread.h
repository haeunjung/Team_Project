#pragma once
#include "../Engine_Header.h"
#include "ThreadQueue.h"

WOOJUN_BEGIN

class DLL CThread
{
protected:
	friend class CThreadMgr;
protected:
	HANDLE		m_hThread;
	HANDLE		m_hStart;
	bool		m_bLoop;
public:
	bool Init();
	void WaitStart();
	void Start();
public:
	virtual void Run() = 0;
protected:
	CThread();
	virtual ~CThread() = 0;
protected:
	static UINT __stdcall ThreadFunc(void* _pArg);
};

WOOJUN_END