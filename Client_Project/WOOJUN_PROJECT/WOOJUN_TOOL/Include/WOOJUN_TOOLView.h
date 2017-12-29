
// WOOJUN_TOOLView.h : CWOOJUN_TOOLView 클래스의 인터페이스
//

#pragma once

#include "WOOJUN_TOOLDoc.h"

class CWOOJUN_TOOLView : public CView
{
protected: // serialization에서만 만들어집니다.
	CWOOJUN_TOOLView();
	DECLARE_DYNCREATE(CWOOJUN_TOOLView)

// 특성입니다.
public:
	CWOOJUN_TOOLDoc* GetDocument() const;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CWOOJUN_TOOLView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // WOOJUN_TOOLView.cpp의 디버그 버전
inline CWOOJUN_TOOLDoc* CWOOJUN_TOOLView::GetDocument() const
   { return reinterpret_cast<CWOOJUN_TOOLDoc*>(m_pDocument); }
#endif

