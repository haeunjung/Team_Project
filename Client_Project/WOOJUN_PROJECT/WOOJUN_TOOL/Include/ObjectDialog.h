#pragma once
#include "afxwin.h"


// CObjectDialog 대화 상자입니다.

class CObjectDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CObjectDialog)

public:
	CObjectDialog(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CObjectDialog();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_OBJECT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnCancel();
	virtual void OnOK();
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
public:
	float m_fScaleX;
	float m_fScaleY;
	float m_fScaleZ;

	float m_fRotationX;
	float m_fRotationY;
	float m_fRotationZ;

	float m_fPosX;
	float m_fPosY;
	float m_fPosZ;
private:
	void SetObjectValue();
	bool LoadFBX();
	void CreateObject(const string& _strKey, const wstring & _FileName);
public:
	afx_msg void OnPaint();
	afx_msg void OnLbnSelchangeList1();
private:
	bool		m_bLoad;
	CListBox	m_ObjectListBox;
	int			m_iCurIndex;
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedLoadButton();
	afx_msg void OnBnClickedCreateButton();
};
