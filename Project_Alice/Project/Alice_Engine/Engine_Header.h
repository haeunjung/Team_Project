#pragma once

#include <Windows.h>
#include <conio.h>
#include <list>
#include <vector>
#include <map>
#include <unordered_map>
#include <string>
#include <algorithm>
#include <functional>
#include <process.h>

// Fmod
#include "Fmod_Inc/fmod.hpp"

#ifdef _WIN32
#pragma comment(lib, "fmod_vc.lib")
#else
#pragma comment(lib, "fmod64_vc.lib")
#endif

// DirectX
#include <d3d11.h>
#include <d3dcompiler.h>

#include <assert.h>
#include <crtdbg.h>

using namespace std;

#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler")

#pragma warning( disable : 4251 )
#pragma warning( disable : 4275 )

// 사용자 정의 헤더파일
#include "Engine_Macro.h"

#include "02.Math/Vector2.h"
#include "02.Math/Vector3.h"
#include "02.Math/Vector4.h"
#include "02.Math/Matrix.h"

#include "Engine_Struct.h"
#include "Engine_TemplateFunc.h"
#include "Engine_Enum.h"

#ifdef _DEBUG
#pragma comment(lib, "DirectXTex_Debug")
#else
#pragma comment(lib, "DirectXTex")
#endif // _DEBUG

WOOJUN_BEGIN

static Vector3 WORLDAXIS[AXIS_MAX] =
{
	// Right
	Vector3(1.f, 0.f, 0.f),
	// Up
	Vector3(0.f, 1.f, 0.f),
	// Forward
	Vector3(0.f, 0.f, 1.f)
};

WOOJUN_END