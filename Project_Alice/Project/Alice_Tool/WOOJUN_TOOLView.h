
// WOOJUN_TOOLView.h : CWOOJUN_TOOLView Ŭ������ �������̽�
//

#pragma once

#include "WOOJUN_TOOLDoc.h"

class CWOOJUN_TOOLView : public CView
{
protected: // serialization������ ��������ϴ�.
	CWOOJUN_TOOLView();
	DECLARE_DYNCREATE(CWOOJUN_TOOLView)

// Ư���Դϴ�.
public:
	CWOOJUN_TOOLDoc* GetDocument() const;

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// �����Դϴ�.
public:
	virtual ~CWOOJUN_TOOLView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // WOOJUN_TOOLView.cpp�� ����� ����
inline CWOOJUN_TOOLDoc* CWOOJUN_TOOLView::GetDocument() const
   { return reinterpret_cast<CWOOJUN_TOOLDoc*>(m_pDocument); }
#endif

