#pragma once
#include "afxwin.h"


// CTestDialog ��ȭ �����Դϴ�.

class CMonsterDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CMonsterDialog)

public:
	CMonsterDialog(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CMonsterDialog();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_MONSTER	};
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};
