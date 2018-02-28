#pragma once

#include "Engine_Header.h"
#include "Engine_Core.h"

#ifdef _DEBUG
#pragma comment(lib, "Alice_Engine_Debug")
#else
#pragma comment(lib, "Alice_Engine")
#endif // _DEBUG

WOOJUN_USING

enum TAB_INDEX
{
	TI_OBJECT,
	TI_ANIMATION,
	TI_TEST,
	TI_MAX
};