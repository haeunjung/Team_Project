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

// ��ũ�� �Լ��� ������ �� �ٹٲ��� �Ұ���������,
// ���� ���� \�� ���̸� ���������� ����� ���ٷ� ������ش�.

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

#define KEYPUSH(Key)	CKeyMgr::GetInst()->GetPush(Key)
#define KEYPRESS(Key)	CKeyMgr::GetInst()->GetPress(Key)
#define KEYUP(Key)		CKeyMgr::GetInst()->GetUp(Key)
#define KEYSTAYUP(Key)	CKeyMgr::GetInst()->GetStayUp(Key)

#define WHEELDIR		CKeyMgr::GetInst()->GetWheelDir()
#define MOUSEPOS		CKeyMgr::GetInst()->GetMousePos()
#define MOUSEMOVE		CKeyMgr::GetInst()->GetMouseMove()

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
 