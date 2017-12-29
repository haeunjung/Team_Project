// EditForm.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "WOOJUN_TOOL.h"
#include "EditForm.h"


// CEditForm

IMPLEMENT_DYNCREATE(CEditForm, CFormView)

CEditForm::CEditForm()
	: CFormView(IDD_DIALOG_EDITFORM)
{

}

CEditForm::~CEditForm()
{
}

void CEditForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CEditForm, CFormView)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CEditForm::OnTcnSelchangeTab1)
END_MESSAGE_MAP()


// CEditForm �����Դϴ�.

#ifdef _DEBUG
void CEditForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CEditForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CEditForm �޽��� ó�����Դϴ�.


void CEditForm::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	*pResult = 0;
}


//BOOL CEditForm::PreCreateWindow(CREATESTRUCT& cs)
//{
//	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
//
//	return CFormView::PreCreateWindow(cs);
//}


BOOL CEditForm::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	bool Temp =  CFormView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);

	if (false == m_MenuDialog.Create(IDD_DIALOG_MENU, this))
	{
		AfxMessageBox(_T("Error : Create Menu Dialog Failed"), MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

	m_MenuDialog.ShowWindow(SW_SHOW);

	return true;
}
