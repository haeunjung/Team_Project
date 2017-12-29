// EditForm.cpp : 구현 파일입니다.
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


// CEditForm 진단입니다.

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


// CEditForm 메시지 처리기입니다.


void CEditForm::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	*pResult = 0;
}


//BOOL CEditForm::PreCreateWindow(CREATESTRUCT& cs)
//{
//	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
//
//	return CFormView::PreCreateWindow(cs);
//}


BOOL CEditForm::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	bool Temp =  CFormView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);

	if (false == m_MenuDialog.Create(IDD_DIALOG_MENU, this))
	{
		AfxMessageBox(_T("Error : Create Menu Dialog Failed"), MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

	m_MenuDialog.ShowWindow(SW_SHOW);

	return true;
}
