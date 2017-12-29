#pragma once

#include "Engine_Header.h"
#include "Engine_Core.h"

#ifdef _DEBUG
#pragma comment(lib, "WOOJUN_ENGINE_Debug")
#else
#pragma comment(lib, "WOOJUN_ENGINE")
#endif // _DEBUG

WOOJUN_USING

enum TAB_INDEX
{
	TI_OBJECT,
	TI_TEST,
	TI_MAX
};