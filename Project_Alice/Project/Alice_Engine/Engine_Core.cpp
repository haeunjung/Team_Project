#include "Engine_Core.h"
#include "Device.h"
#include "01.Core/PathMgr.h"
#include "01.Core/TimerMgr.h"
#include "01.Core/Timer.h"
#include "01.Core/Input.h"
#include "01.Core/CollisionMgr.h"
#include "03.Resource/ResMgr.h"
#include "03.Resource/Mesh.h"
#include "04.Rendering/Shader.h"
#include "04.Rendering/RenderMgr.h"
#include "04.Rendering/ShaderMgr.h"
#include "05.Scene/SceneMgr.h"
#include "06.GameObject/GameObject.h"
#include "07.Component/RadioButtonMgr.h"

WOOJUN_BEGIN

DEFINITION_SINGLE(CEngine_Core)

bool CEngine_Core::m_IsRun = true;

bool CEngine_Core::Init(HINSTANCE _hInst, unsigned int _iWidth, unsigned int _iHeight, TCHAR * _pClass, TCHAR * _pTitle, int _iIconID, int _iSmallIconID, bool _IsWindowMode)
{
	m_hInst = _hInst;
	m_tResolution.m_iWidth = _iWidth;
	m_tResolution.m_iHeight = _iHeight;

	// 윈도우 창을 생성한다.
	MyRegisterClass(_pClass, _iIconID, _iSmallIconID);
	Create(_pClass, _pTitle);

	return Init(m_hWnd, _iWidth, _iHeight, _IsWindowMode);
}

bool CEngine_Core::Init(HWND _hWnd, unsigned int _iWidth, unsigned int _iHeight, bool _IsWindowMode)
{	
	m_hWnd = _hWnd;
	m_tResolution.m_iWidth = _iWidth;
	m_tResolution.m_iHeight = _iHeight;

	// Init Device
	if (false == GET_SINGLE(CDevice)->Init(_hWnd, _iWidth, _iHeight, _IsWindowMode))
	{
		return false;
	}

	// Init TimerMgr
	if (false == GET_SINGLE(CTimerMgr)->Init())
	{
		return false;
	}
	m_pMainTimer = GET_SINGLE(CTimerMgr)->FindTimer(MAINTIMER);

	// Init KeyMGr
	if (false == GET_SINGLE(CInput)->Init())
	{		
		return false;
	}

	// Init PathMgr
	if (false == GET_SINGLE(CPathMgr)->Init())
	{
		return false;
	}	
	
	// Init ReshMgr
	if (false == GET_SINGLE(CResMgr)->Init())
	{
		return false;
	}

	// Init RenderhMgr
	if (false == GET_SINGLE(CRenderMgr)->Init())
	{
		return false;
	}

	// Init ColMgr
	if (false == GET_SINGLE(CCollisionMgr)->Init())
	{
		return false;
	}

	// Init SceneMgr
	if (false == GET_SINGLE(CSceneMgr)->Init())
	{
		return false;
	}

	return true;
}

int CEngine_Core::Run()
{
	MSG msg;

	while (m_IsRun)
	{
		// PeekMessage 
		// 대기상태에 들어가지 않고 바로 리턴
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			// 실제 게임 루프
			Logic();
		}
	}

	// 메인 타이머 해제
	SAFE_RELEASE(m_pMainTimer);

	return (int)msg.wParam;
}

void CEngine_Core::RunFromLoop()
{
	Logic();
}

void CEngine_Core::Exit()
{
	DestroyWindow(m_hWnd);
}

void CEngine_Core::Logic()
{
	// Main Thread Timer
	m_pMainTimer->Update();
	m_fMainDeltaTime = m_pMainTimer->GetDeltaTime();

	GET_SINGLE(CInput)->Update(m_fMainDeltaTime);

	// 여기서 시간체크를 해서
	// 비율로 나타내보기

	Input(m_fMainDeltaTime);

	Update(m_fMainDeltaTime);

	LateUpdate(m_fMainDeltaTime);

	Collision(m_fMainDeltaTime);

	Render(m_fMainDeltaTime);	

	GET_SINGLE(CInput)->ClearWheel();
}

int CEngine_Core::Input(float _fTime)
{
	return GET_SINGLE(CSceneMgr)->Input(_fTime);
}

int CEngine_Core::Update(float _fTime)
{
	return GET_SINGLE(CSceneMgr)->Update(_fTime);
}

int CEngine_Core::LateUpdate(float _fTime)
{
	return GET_SINGLE(CSceneMgr)->LateUpdate(_fTime);
}

void CEngine_Core::Collision(float _fTime)
{
	GET_SINGLE(CInput)->ComputeRay();
	GET_SINGLE(CSceneMgr)->Collision(_fTime);
	GET_SINGLE(CCollisionMgr)->Collision(_fTime);
}

void CEngine_Core::Render(float _fTime)
{
	GET_SINGLE(CDevice)->Clear();

	GET_SINGLE(CSceneMgr)->Render(_fTime);

	GET_SINGLE(CRenderMgr)->Render(_fTime);

	GET_SINGLE(CDevice)->Present();
}

ATOM CEngine_Core::MyRegisterClass(TCHAR * _pClass, int _iIconID, int _iSmallIconID)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = CEngine_Core::WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = m_hInst;
	wcex.hIcon = LoadIcon(m_hInst, MAKEINTRESOURCE(_iIconID));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = _pClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(_iSmallIconID));

	return RegisterClassExW(&wcex);
}

bool CEngine_Core::Create(TCHAR * _pClass, TCHAR * _pTitle)
{
	m_hWnd = CreateWindow(_pClass, _pTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, m_hInst, nullptr);

	if (nullptr == m_hWnd)
	{
		return false;
	}

	RECT rc = { 0, 0, (long)m_tResolution.m_iWidth, (long)m_tResolution.m_iHeight };

	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, rc.right - rc.left, rc.bottom - rc.top, SWP_NOZORDER | SWP_NOMOVE);

	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);

	return true;
}

LRESULT CEngine_Core::WndProc(HWND _hWnd, unsigned int _message, WPARAM _wParam, LPARAM _lParam)
{
	switch (_message)
	{
	case WM_MOUSEWHEEL:
	{
		// 마우스 휠 움직임에 대한 가상키가 없기때문에
		// wParam에 들어오는 값을 직접 가져와야한다
		// WHEEL UP = 120
		// WHEEL DOWN = -120
		GET_SINGLE(CInput)->SetWheel(HIWORD(_wParam));
		break;
	}
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(_hWnd, &ps);
		EndPaint(_hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		m_IsRun = false;
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(_hWnd, _message, _wParam, _lParam);
	}
	return 0;
}

CEngine_Core::CEngine_Core() : m_hInst(NULL), m_fMainDeltaTime(0.0f)
{
	srand(GetCurrentTime());
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(110644);

	new int();

#ifdef _DEBUG
	//AllocConsole();
#endif
}

CEngine_Core::~CEngine_Core()
{
	DESTROY_SINGLE(CSceneMgr);
	DESTROY_SINGLE(CCollisionMgr);
	DESTROY_SINGLE(CResMgr);
	DESTROY_SINGLE(CRenderMgr);
	DESTROY_SINGLE(CPathMgr);
	DESTROY_SINGLE(CDevice);
	DESTROY_SINGLE(CInput);
	DESTROY_SINGLE(CTimerMgr);
	DESTROY_SINGLE(CRadioButtonMgr);
	CGameObject::DestroyPrototype();

	SAFE_RELEASE(m_pMainTimer);
#ifdef _DEBUG
	//FreeConsole();
#endif
}

WOOJUN_END