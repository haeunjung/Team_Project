// TestDialog.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "WOOJUN_TOOL.h"
#include "TestDialog.h"
#include "afxdialogex.h"

// CTestDialog ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CTestDialog, CDialogEx)

CTestDialog::CTestDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

}

CTestDialog::~CTestDialog()
{
}

void CTestDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTestDialog, CDialogEx)
	ON_WM_CHILDACTIVATE()
END_MESSAGE_MAP()


// CTestDialog �޽��� ó�����Դϴ�.

void CTestDialog::OnChildActivate()
{
	CDialogEx::OnChildActivate();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.	
}
