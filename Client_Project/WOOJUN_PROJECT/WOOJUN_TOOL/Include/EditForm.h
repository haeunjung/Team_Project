#pragma once
#include "afxcmn.h"
#include "MenuDialog.h"


// CEditForm �� ���Դϴ�.

class CEditForm : public CFormView
{
	DECLARE_DYNCREATE(CEditForm)
private:
	CMenuDialog		m_MenuDialog;
protected:
	CEditForm();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CEditForm();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_EDITFORM };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
//	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
};


