#pragma once
#include "Engine_Header.h"

WOOJUN_BEGIN

class CMesh;
class CTimer;
class DLL CEngine_Core
{
private:
	RESOLUTION		m_tResolution;
	HINSTANCE		m_hInst;
	HWND			m_hWnd;
	static bool		m_IsRun;

	// Main Timer
	CTimer*			m_pMainTimer;
	float			m_fMainDeltaTime;
public:
	inline RESOLUTION GetResolution() const {
		return m_tResolution;
	}
	inline HWND GetWindowHandle() const {
		return m_hWnd;
	}
public:
	// 윈도우부터 만들어야 하는 Init
	bool Init(HINSTANCE _hInst, unsigned int _iWidth, unsigned int _iHeight, 
		TCHAR* _pClass, TCHAR* _pTitle, int _iIconID, int _iSmallIconID,
		bool _IsWindowMode);
	bool Init(HWND _hWnd, unsigned int _iWidth, unsigned int _iHeight,
		bool _IsWindowMode);
	int Run();
	void RunFromLoop();
	void Exit();
private:
	void Logic();
	int Input(float _fTime);
	int Update(float _fTime);
	int LateUpdate(float _fTime);
	void Collision(float _fTime);
	void Render(float _fTime);	
private:
	ATOM MyRegisterClass(TCHAR* _pClass, int _iIconID, int _iSmallIconID);
	bool Create(TCHAR* _pClass, TCHAR* _pTitle);
	static LRESULT CALLBACK WndProc(HWND _hWnd, unsigned int _message, WPARAM _wParam, LPARAM _lParam);

	DECLARE_SINGLE(CEngine_Core)
};

WOOJUN_END