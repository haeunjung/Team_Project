#pragma once
#include "afxcmn.h"


// CMenuDialog ��ȭ �����Դϴ�.

class CMenuDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CMenuDialog)

public:
	CMenuDialog(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CMenuDialog();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_MENU };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
private:
	CTabCtrl		m_TabCtrl;
	TAB_INDEX		m_TabIndex;
	vector<CDialog*>	m_vecDialog;
public:
	const TAB_INDEX& GetTabIndex();
	void ShowDialog(TAB_INDEX _Index);
public:
	template <typename T>
	void CreateTab(UINT _ID, TCHAR* _Key)
	{
		T* NewDialog = new T();

		if (FALSE == NewDialog->Create(_ID, &m_TabCtrl))
		{
			AfxMessageBox(L"Error : Create Menu Dialog Failed", MB_OK | MB_ICONEXCLAMATION);
			return;
		}

		m_TabCtrl.InsertItem((int)m_vecDialog.size(), _Key);
		m_vecDialog.push_back(NewDialog);
	}

	//void CreateTab(UINT _ID, wstring _Name)
	//{
	//	CDialog* NewDialog = new CDialog();

	//	if (FALSE == NewDialog->Create(_ID, &m_TabCtrl))
	//	{
	//		AfxMessageBox(L"Error : Create Menu Dialog Failed", MB_OK | MB_ICONEXCLAMATION);
	//		return;
	//	}
	//	m_TabCtrl.InsertItem((int)m_vecDialog.size(), _Name.c_str());
	//	//m_TabCtrl.InsertItem((int)m_vecDialog.size(), _Name.c_str());
	//	m_vecDialog.push_back(NewDialog);
	//}
public:
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	virtual void OnCancel();
	virtual void OnOK();
};
