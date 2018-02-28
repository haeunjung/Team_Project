// TestDialog.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "WOOJUN_TOOL.h"
#include "TestDialog.h"
#include "afxdialogex.h"

// CTestDialog 대화 상자입니다.

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


// CTestDialog 메시지 처리기입니다.

void CTestDialog::OnChildActivate()
{
	CDialogEx::OnChildActivate();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.	
}
