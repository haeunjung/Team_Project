#pragma once
#include "afxwin.h"
#include "07.Component/Light.h"

// CTestDialog 대화 상자입니다.

class CTestDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CTestDialog)
private:
	vector<CLight*> m_vecLight;
	int m_iCurSel;

	float m_fPosX;
	float m_fPosY;
	float m_fPosZ;
public:
	CTestDialog(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CTestDialog();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

private:
	void CreatePointLight(DxVector3 _vPos = { 0, 0, 0 });
	void CreateSpotLight(DxVector3 _vPos = { 0, 0, 0 });

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CListBox m_LightListBox;
	afx_msg void OnChildActivate();
	afx_msg void OnBnClickedButtonCreateSpot();
	afx_msg void OnBnClickedButtonCreatePoint();
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnBnClickedButtonLoad();
	afx_msg void OnLbnSelchangeList1();
	afx_msg void OnEnChangeEdit1Posx2();
	afx_msg void OnEnChangeEdit1Posy2();
	afx_msg void OnEnChangeEdit1Posz2();
};
