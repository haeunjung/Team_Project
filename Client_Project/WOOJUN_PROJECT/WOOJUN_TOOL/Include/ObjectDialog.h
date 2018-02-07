#pragma once
#include "afxwin.h"
#include "06.GameObject/GameObject.h"

// CObjectDialog ��ȭ �����Դϴ�.

class CObjectDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CObjectDialog)

public:
	CObjectDialog(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CObjectDialog();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_OBJECT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

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
	void LoadObject(const string& _strKey, const DxVector3& _vPos, const DxVector3& _vScale, const DxVector3& _vRot);
public:
	afx_msg void OnLbnSelchangeList1();
private:
	bool			m_bLoad;
	CListBox		m_ObjectListBox;
	int				m_iCurIndex;
	CGameObject*	m_pPickObject;
	vector<CGameObject*>	m_vecObject;
public:
	void ChangePickObject(CGameObject* _pGameObject);
public:
	afx_msg void OnBnClickedLoadButton();
	afx_msg void OnBnClickedCreateButton();
	afx_msg void OnChildActivate();
	afx_msg void OnBnClickedSaveobject();
	afx_msg void OnBnClickedLoadobject();
	afx_msg void OnBnClickedDeleteButton();
};
