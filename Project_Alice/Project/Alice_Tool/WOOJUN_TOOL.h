
// WOOJUN_TOOL.h : WOOJUN_TOOL ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.


// CWOOJUN_TOOLApp:
// �� Ŭ������ ������ ���ؼ��� WOOJUN_TOOL.cpp�� �����Ͻʽÿ�.
//

class CWOOJUN_TOOLApp : public CWinApp
{
public:
	CWOOJUN_TOOLApp();


// �������Դϴ�.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// �����Դϴ�.
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnIdle(LONG lCount);
};

extern CWOOJUN_TOOLApp theApp;
