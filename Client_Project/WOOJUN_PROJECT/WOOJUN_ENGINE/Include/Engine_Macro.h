#pragma once

#ifdef DLL_EXPORT
#define DLL __declspec(dllexport)
#else
#define DLL __declspec(dllimport)
#endif // DLL_EXPORT

#define WOOJUN_BEGIN	namespace WOOJUN {
#define WOOJUN_END		}
#define WOOJUN_USING	using namespace WOOJUN;

#define SAFE_DELETE(p)	if(p)	{ delete p; p = NULL; }
#define SAFE_DELETE_ARR(p)	if(p)	{ delete[] p; p = NULL; }
#define SAFE_RELEASE(p)	if(p)	{ p->Release(); p = NULL; }

// 매크로 함수를 정의할 때 줄바꿈이 불가능하지만,
// 문장 끝에 \를 붙이면 다음라인을 끌어와 한줄로 만들어준다.

#define DECLARE_SINGLE(Type)\
	private:\
		static Type* m_pInst;\
	public:\
		static Type* GetInst()\
		{\
			if (!m_pInst)\
				m_pInst = new Type;\
			return m_pInst;\
		}\
		static void DestroyInst()\
		{\
			SAFE_DELETE(m_pInst);\
		}\
	public:\
		Type();\
		~Type();

#define DEFINITION_SINGLE(Type) Type* Type::m_pInst = NULL;
#define GET_SINGLE(Type) Type::GetInst()
#define DESTROY_SINGLE(Type) Type::DestroyInst()

#define _RESOLUTION		CEngine_Core::GetInst()->GetResolution()

#define DEVICE			CDevice::GetInst()->GetDevice()
#define CONTEXT			CDevice::GetInst()->GetDeviceContext()
#define SWAPCHAIN		CDevice::GetInst()->GetSwapChain()

#define BASEPATH		"BasePath"
#define SHADERPATH		"ShaderPath"
#define TEXTUREPATH		"TexturePath"
#define MESHPATH		"MeshPath"

#define MAINTIMER		"MainThread"
#define DEFAULTSCENE	"DefaultScene"
#define DEFAULTLAYER	"DefaultLayer"
#define MAPLAYER		"MapLayer"
#define MAINCAMERA		"MainCamera"

#define WINDOWHANDLE	CEngine_Core::GetInst()->GetWindowHandle()

#define PI				3.141592f

#define	KEYPUSH(key)	CInput::GetInst()->KeyPress(key)
#define	KEYPRESS(key)	CInput::GetInst()->KeyPush(key)
#define	KEYUP(key)		CInput::GetInst()->KeyUp(key)

#define	WHEELDIR		CInput::GetInst()->GetWheelDir()
#define	MOUSEPOS		CInput::GetInst()->GetMousePos()
#define	MOUSEMOVE		CInput::GetInst()->GetMouseMove()

#define WIRE_FRAME			"WireFrame"
#define CULLING_CW			"CWCulling"
#define ALPHABLEND			"AlphaBlend"
#define DEPTH_LESS_EQUAL	"DepthLessEqual"
#define DEPTH_DISABLE		"DepthDisable"

// Shader Key
#define	STANDARD_TEXNORMAL_SHADER	"StandardTextureNormalShader"
#define EFFECT_SHADER				"EffectShader"
#define COLLIDER_COLOR_SHADER		"ColliderColorShader"
#define STANDARD_BUMP_SHADER		"StandardBumpShader"
#define UI_SHADER					"UIShader"
#define TERRAIN_SHADER				"TerrainShader"
 