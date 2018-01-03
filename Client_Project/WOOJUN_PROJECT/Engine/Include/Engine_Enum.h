#pragma once
#include "Engine_Macro.h"

WOOJUN_BEGIN

enum DLL SHADER_TYPE
{
	ST_VERTEX,
	ST_PIXEL,
	ST_GEOMETRY,
	ST_END
};

enum DLL COMPONENT_TYPE
{
	CT_TRANSFROM,
	CT_RENDERER,
	CT_RENDERER2D,
	CT_CAMERA,
	CT_CAMERAARM,
	CT_SCRIPT,
	CT_COLLIDER,
	CT_LIGHT,
	CT_EFFECT,
	CT_ANIMATION2D,
	CT_UI,
	CT_FRUSTUM,
	CT_MAX,
};

// 축정보
enum DLL AXIS
{
	AXIS_X,
	AXIS_Y,
	AXIS_Z,
	AXIS_MAX
};

// 상수버퍼 업데이트
// 어느어느 쉐이더에 할꺼니??
// 비트단위연산자 활용
enum DLL CONSTBUFFER_UPDATE_TYPE
{
	CUT_VERTEX = 0x1,
	CUT_PIXEL = 0x2,
	CUT_GEOMETRY = 0x4
};

enum DLL COLLIDER_TYPE
{
	COL_SPHERE,
	COL_AABB,
	COL_OBB,
	COL_RECT,
	COL_POINT,
	COL_RAY
};

enum DLL RENDER_STATE_TYPE
{
	RST_RASTERIZER,
	RST_BLEND,
	RST_DEPTH,
	RST_END
};

enum DLL LIGHT_TYPE
{
	LT_DIR,
	LT_POINT,
	LT_SPOT,
	LT_END
};

// Sort Flag
enum DLL SORT_FLAG
{
	SF_ASCENDING,
	SF_DESCENDING
};

// Animation2D Type
enum DLL ANIMATION2D_TYPE
{
	A2D_NONE,
	A2D_ATLAS,
	A2D_FRAME,
	A2D_END
};

// Animation2D Option
enum DLL ANIMATION2D_OPTION
{
	A2DO_LOOP,
	A2DO_ONCE_RETURN,
	A2DO_ONCE_DESTROY,
	A2DO_COUNT_RETURN,
	A2DO_COUNT_DESTROY,
	A2DO_TIME_RETURN,
	A2DO_TIME_DESTROY,
	A2DO_END
};

enum DLL UI_TYPE
{
	UT_BUTTON,
	UT_CHECKBOX,
	UT_RADIOBUTTON,
	UT_BAR,
	UT_BACK,
	UT_END
};

enum DLL BUTTON_STATE
{
	BS_NONE,
	BS_MOUSEON,
	BS_CLICK
};

enum DLL SCENE_CHANGE
{
	SC_PREV = -1,
	SC_NONE = 0,
	SC_NEXT = 1
};

// Bar Dir
enum DLL BAR_DIR
{
	BD_LEFT,
	BD_RIGHT,
	BD_DOWN,
	BD_UP
};

// Frustum Plane
enum DLL FRUSTUM_PLANE
{
	FP_LEFT,
	FP_RIGHT,
	FP_TOP,
	FP_BOTTOM,
	FP_NEAR,
	FP_FAR,
	FP_END
};

WOOJUN_END