
// WOOJUN_TOOLView.cpp : CWOOJUN_TOOLView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
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
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CWOOJUN_TOOLView 생성/소멸

CWOOJUN_TOOLView::CWOOJUN_TOOLView()
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CWOOJUN_TOOLView::~CWOOJUN_TOOLView()
{
}

BOOL CWOOJUN_TOOLView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CWOOJUN_TOOLView 그리기

void CWOOJUN_TOOLView::OnDraw(CDC* /*pDC*/)
{
	CWOOJUN_TOOLDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}


// CWOOJUN_TOOLView 인쇄

BOOL CWOOJUN_TOOLView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CWOOJUN_TOOLView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CWOOJUN_TOOLView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CWOOJUN_TOOLView 진단

#ifdef _DEBUG
void CWOOJUN_TOOLView::AssertValid() const
{
	CView::AssertValid();
}

void CWOOJUN_TOOLView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CWOOJUN_TOOLDoc* CWOOJUN_TOOLView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CWOOJUN_TOOLDoc)));
	return (CWOOJUN_TOOLDoc*)m_pDocument;
}
#endif //_DEBUG


// CWOOJUN_TOOLView 메시지 처리기
