#pragma once
#include "../Engine_Header.h"

WOOJUN_BEGIN

class DLL CDebug
{
public:
	static void OutputConsole(char* pMsg);
	static void OutputVisual(char* pMsg);
	static void OutputTitle(char* pMsg);
};

WOOJUN_END