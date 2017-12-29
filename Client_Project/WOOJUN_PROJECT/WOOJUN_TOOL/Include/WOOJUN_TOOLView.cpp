
// WOOJUN_TOOLView.cpp : CWOOJUN_TOOLView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
#ifndef SHARED_HANDLERS
#include "WOOJUN_TOOL.h"
#endif

#include "WOOJUN_TOOLDoc.h"
#include "WOOJUN_TOOLView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CWOOJUN_TOOLView

IMPLEMENT_DYNCREATE(CWOOJUN_TOOLView, CView)

BEGIN_MESSAGE_MAP(CWOOJUN_TOOLView, CView)
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CWOOJUN_TOOLView ����/�Ҹ�

CWOOJUN_TOOLView::CWOOJUN_TOOLView()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.

}

CWOOJUN_TOOLView::~CWOOJUN_TOOLView()
{
}

BOOL CWOOJUN_TOOLView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CView::PreCreateWindow(cs);
}

// CWOOJUN_TOOLView �׸���

void CWOOJUN_TOOLView::OnDraw(CDC* /*pDC*/)
{
	CWOOJUN_TOOLDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.
}


// CWOOJUN_TOOLView �μ�

BOOL CWOOJUN_TOOLView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CWOOJUN_TOOLView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CWOOJUN_TOOLView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
}


// CWOOJUN_TOOLView ����

#ifdef _DEBUG
void CWOOJUN_TOOLView::AssertValid() const
{
	CView::AssertValid();
}

void CWOOJUN_TOOLView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CWOOJUN_TOOLDoc* CWOOJUN_TOOLView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CWOOJUN_TOOLDoc)));
	return (CWOOJUN_TOOLDoc*)m_pDocument;
}
#endif //_DEBUG


// CWOOJUN_TOOLView �޽��� ó����
