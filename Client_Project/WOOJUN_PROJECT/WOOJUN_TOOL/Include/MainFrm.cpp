
// MainFrm.cpp : CMainFrame Ŭ������ ����
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
	ID_SEPARATOR,           // ���� �� ǥ�ñ�
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame ����/�Ҹ�

CMainFrame::CMainFrame()
{
	// TODO: ���⿡ ��� �ʱ�ȭ �ڵ带 �߰��մϴ�.
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
	//	TRACE0("���� ������ ������ ���߽��ϴ�.\n");
	//	return -1;      // ������ ���߽��ϴ�.
	//}

	//if (!m_wndStatusBar.Create(this))
	//{
	//	TRACE0("���� ǥ������ ������ ���߽��ϴ�.\n");
	//	return -1;      // ������ ���߽��ϴ�.
	//}
	//m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	// TODO: ���� ������ ��ŷ�� �� ���� �Ϸ��� �� �� ���� �����Ͻʽÿ�.
	/*m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);*/


	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.
		
	cs.cx = 1800;
	cs.cy = 760;

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);

	DestroyMenu(cs.hMenu);
	cs.hMenu = nullptr;

	return TRUE;
}

// CMainFrame ����

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


// CMainFrame �޽��� ó����



BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	
	// ��������
	m_SplitWnd.CreateStatic(this, 1, 2);

	m_SplitWnd.CreateView(0, 0, RUNTIME_CLASS(CWOOJUN_TOOLView), SIZE{ 1280, 720 }, pContext);
	m_SplitWnd.CreateView(0, 1, RUNTIME_CLASS(CEditForm), SIZE{ 360, 720 }, pContext);

	m_pView = (CWOOJUN_TOOLView*)m_SplitWnd.GetPane(0, 0);
	m_pForm = (CEditForm*)m_SplitWnd.GetPane(0, 1);

	// ���̷�Ʈ �ʱ�ȭ
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

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	DESTROY_SINGLE(CEngine_Core);
	DESTROY_SINGLE(CToolValue);
}
