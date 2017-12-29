#pragma once
#include "01.Core/Thread.h"
#include "05.Scene/Scene.h"

WOOJUN_USING

class CLoadingThread :
	public CThread
{
private:
	bool		m_bLoadComplete;
	CScene*		m_pScene;
public:
	bool GetLoadComplete() const;
	void Change();
public:
	void Run() override;
public:
	CLoadingThread();
	~CLoadingThread();
};

