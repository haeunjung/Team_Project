// TestDialog.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "WOOJUN_TOOL.h"
#include "MonsterDialog.h"
#include "afxdialogex.h"

// CTestDialog 대화 상자입니다.

IMPLEMENT_DYNAMIC(CMonsterDialog, CDialogEx)

CMonsterDialog::CMonsterDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_MONSTER, pParent)
{

}

CMonsterDialog::~CMonsterDialog()
{
}

void CMonsterDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMonsterDialog, CDialogEx)
	ON_WM_CHILDACTIVATE()
END_MESSAGE_MAP()
