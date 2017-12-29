// MenuDialog.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "WOOJUN_TOOL.h"
#include "MenuDialog.h"
#include "afxdialogex.h"

#include "ObjectDialog.h"
#include "TestDialog.h"

// CMenuDialog 대화 상자입니다.

IMPLEMENT_DYNAMIC(CMenuDialog, CDialogEx)

CMenuDialog::CMenuDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_MENU, pParent)
{
}

CMenuDialog::~CMenuDialog()
{
}

void CMenuDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_TabCtrl);
}

const TAB_INDEX & CMenuDialog::GetTabIndex()
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return m_TabIndex;
}

void CMenuDialog::ShowDialog(TAB_INDEX _Index)
{
	m_vecDialog[m_TabIndex]->ShowWindow(SW_HIDE);

	RECT rc = {};
	m_TabCtrl.GetClientRect(&rc);
	m_TabCtrl.AdjustRect(TRUE, &rc);

	m_vecDialog[_Index]->SetWindowPos(&m_TabCtrl, rc.left * -1.0f, rc.top * -1.0f, rc.right + rc.left * 5.0f/* - rc.left * -10.0f*/, rc.bottom + rc.top * 5.0f/* - rc.top * -10.0f*/, SWP_NOZORDER);
	m_vecDialog[_Index]->ShowWindow(SW_SHOW);

	m_TabIndex = _Index;
}


BEGIN_MESSAGE_MAP(CMenuDialog, CDialogEx)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CMenuDialog::OnTcnSelchangeTab1)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CMenuDialog 메시지 처리기입니다.


void CMenuDialog::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	ShowDialog((TAB_INDEX)m_TabCtrl.GetCurSel());

	*pResult = 0;
}


BOOL CMenuDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	CreateTab<CObjectDialog>(IDD_DIALOG_OBJECT, L"Object");
	CreateTab<CTestDialog>(IDD_DIALOG1, L"Test");

	ShowDialog(TI_OBJECT);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CMenuDialog::OnDestroy()
{
	Safe_Delete_VecList(m_vecDialog);
	
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CMenuDialog::OnCancel()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	return;

	CDialogEx::OnCancel();
}


void CMenuDialog::OnOK()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	return;

	CDialogEx::OnOK();
}
