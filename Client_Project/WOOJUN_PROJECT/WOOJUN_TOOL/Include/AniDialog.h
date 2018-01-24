#pragma once
#include "06.GameObject/GameObject.h"
#include "afxwin.h"

// CAniDialog 대화 상자입니다.

class CAniDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CAniDialog)

public:
	CAniDialog(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CAniDialog();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_ANIMATION };
#endif

private:
	CGameObject*	m_pPickObject;
public:
	void ChangePickObject(CGameObject* _pGameObject);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnChildActivate();
private:
	CString		m_strClipName;
	CComboBox	m_ComboOption;
	int		m_iStartFrame;
	int		m_iEndFrame;
	CListBox	m_ClipList;
	CString		m_strDefaultClip;
	CString		m_strCurFBX;
	int		m_iPosX;
private:
	void CreateAniObject(const string & _strKey, const wstring & _FullPath);
public:
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnBnClickedButtonAddclip();
	afx_msg void OnBnClickedButtonModifyclip();
	afx_msg void OnBnClickedButtonSaveani();	
	afx_msg void OnBnClickedButtonSetdefaultclip();
	afx_msg void OnBnClickedButtonLoadfbx();
};
