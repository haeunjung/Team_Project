
// MainFrm.cpp : CMainFrame 클래스의 구현
//

#include "stdafx.h"
#include "WOOJUN_TOOL.h"
#include "MainFrm.h"

#include "05.Scene/SceneMgr.h"
#include "05.Scene/Scene.h"
#include "ToolScene.h"
#include "ToolValue.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 상태 줄 표시기
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame 생성/소멸

CMainFrame::CMainFrame()
{
	// TODO: 여기에 멤버 초기화 코드를 추가합니다.
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	//if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
	//	!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	//{
	//	TRACE0("도구 모음을 만들지 못했습니다.\n");
	//	return -1;      // 만들지 못했습니다.
	//}

	//if (!m_wndStatusBar.Create(this))
	//{
	//	TRACE0("상태 표시줄을 만들지 못했습니다.\n");
	//	return -1;      // 만들지 못했습니다.
	//}
	//m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	// TODO: 도구 모음을 도킹할 수 없게 하려면 이 세 줄을 삭제하십시오.
	/*m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);*/


	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.
		
	cs.cx = 1800;
	cs.cy = 760;

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);

	DestroyMenu(cs.hMenu);
	cs.hMenu = nullptr;

	return TRUE;
}

// CMainFrame 진단

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 메시지 처리기



BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	
	// 정적분할
	m_SplitWnd.CreateStatic(this, 1, 2);

	m_SplitWnd.CreateView(0, 0, RUNTIME_CLASS(CWOOJUN_TOOLView), SIZE{ 1280, 720 }, pContext);
	m_SplitWnd.CreateView(0, 1, RUNTIME_CLASS(CEditForm), SIZE{ 360, 720 }, pContext);

	m_pView = (CWOOJUN_TOOLView*)m_SplitWnd.GetPane(0, 0);
	m_pForm = (CEditForm*)m_SplitWnd.GetPane(0, 1);

	// 다이렉트 초기화
	if (!GET_SINGLE(CEngine_Core)->Init(m_pView->m_hWnd, 1280, 720, true))
	{
		DESTROY_SINGLE(CEngine_Core);
		return FALSE;
	}

	CScene*		pCurScene = GET_SINGLE(CSceneMgr)->GetCurScene();
	pCurScene->CreateScript<CToolScene>();

	return TRUE;
}


void CMainFrame::OnDestroy()
{
	CFrameWnd::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	DESTROY_SINGLE(CEngine_Core);
	DESTROY_SINGLE(CToolValue);
}
