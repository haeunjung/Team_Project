#pragma once

#include "Engine_Header.h"

#ifdef _DEBUG
#pragma comment(lib, "Alice_Engine_Debug")
#else
#pragma comment(lib, "Alice_Engine")
#endif // _DEBUG

WOOJUN_USING

enum RESOLUTION_TYPE
{
	RT_SMALL,
	RT_HD,
	RT_FHD,
	RT_MAX
};

static RESOLUTION g_Resolution_Type[RT_MAX] = {
	RESOLUTION(800, 600),
	RESOLUTION(1280, 720),
	RESOLUTION(1920, 1280)
};