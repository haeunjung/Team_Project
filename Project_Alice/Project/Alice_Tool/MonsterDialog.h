#pragma once
#include "afxwin.h"


// CTestDialog 대화 상자입니다.

class CMonsterDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CMonsterDialog)

public:
	CMonsterDialog(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CMonsterDialog();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_MONSTER	};
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
