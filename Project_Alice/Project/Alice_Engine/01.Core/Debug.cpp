#include "Debug.h"
#include "../Engine_Core.h"

WOOJUN_USING

void CDebug::OutputConsole(char * pMsg)
{
	_cprintf(pMsg);
}

void CDebug::OutputVisual(char * pMsg)
{
	OutputDebugStringA(pMsg);
}

void CDebug::OutputTitle(char * pMsg)
{
#ifdef _DEBUG
	SetWindowTextA(WINDOWHANDLE, pMsg);
#endif
}
