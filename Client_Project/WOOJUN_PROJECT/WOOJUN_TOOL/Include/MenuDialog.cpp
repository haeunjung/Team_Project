// MenuDialog.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "WOOJUN_TOOL.h"
#include "MenuDialog.h"
#include "afxdialogex.h"

#include "ObjectDialog.h"
#include "TestDialog.h"

// CMenuDialog ��ȭ �����Դϴ�.

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
	// TODO: ���⿡ ��ȯ ������ �����մϴ�.
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


// CMenuDialog �޽��� ó�����Դϴ�.


void CMenuDialog::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	ShowDialog((TAB_INDEX)m_TabCtrl.GetCurSel());

	*pResult = 0;
}


BOOL CMenuDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	CreateTab<CObjectDialog>(IDD_DIALOG_OBJECT, L"Object");
	CreateTab<CTestDialog>(IDD_DIALOG1, L"Test");

	ShowDialog(TI_OBJECT);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CMenuDialog::OnDestroy()
{
	Safe_Delete_VecList(m_vecDialog);
	
	CDialogEx::OnDestroy();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}


void CMenuDialog::OnCancel()
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	return;

	CDialogEx::OnCancel();
}


void CMenuDialog::OnOK()
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	return;

	CDialogEx::OnOK();
}
