#pragma once
#include "afxwin.h"
#include "07.Component/Transform.h"

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
private:
	CListBox m_MonsterListBox;
	vector<CTransform*> m_vecTransform;
	int m_iCurSel;

	float m_fPosX;
	float m_fPosY;
	float m_fPosZ;

public:
	afx_msg void OnBnClickedSavemonster();
	afx_msg void OnBnClickedLoadmonster();
	afx_msg void OnBnClickedCreatemonster();
	afx_msg void OnBnClickedCreatemonster2();
	afx_msg void OnBnClickedCreatemonster3();
	afx_msg void OnBnClickedCreatemonster4();
	afx_msg void OnBnClickedDeletemonster();
	afx_msg void OnLbnSelchangeList1();
	afx_msg void OnEnChangeMonsterposx();
	afx_msg void OnEnChangeMonsterposy();
	afx_msg void OnEnChangeMonsterposz();
private:
	void CreatePlant(DxVector3 vPos = { 0.0f, 0.0f, 0.0f });
	void CreatePlantRange(DxVector3 vPos = { 0.0f, 0.0f, 0.0f });
	void CreateMutant(DxVector3 vPos = { 0.0f, 0.0f, 0.0f });
	void CreateWarrok(DxVector3 vPos = { 0.0f, 0.0f, 0.0f });
};
