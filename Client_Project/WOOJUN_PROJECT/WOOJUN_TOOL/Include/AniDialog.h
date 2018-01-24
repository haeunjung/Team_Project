#pragma once
#include "06.GameObject/GameObject.h"
#include "afxwin.h"

// CAniDialog ��ȭ �����Դϴ�.

class CAniDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CAniDialog)

public:
	CAniDialog(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CAniDialog();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_ANIMATION };
#endif

private:
	CGameObject*	m_pPickObject;
public:
	void ChangePickObject(CGameObject* _pGameObject);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

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
