#pragma once
#include "..\Engine_Header.h"

WOOJUN_BEGIN

class CRenderState;
class CRasterizerState;
class CBlendState;
class CDepthStencilState;
class DLL CRenderMgr
{
private:
	unordered_map<string, CRenderState*> m_mapRenderState;	
	vector<D3D11_RENDER_TARGET_BLEND_DESC> m_vecRenderTargetBlend;
public:
	// Rasterizer State
	CRasterizerState* CreateRasterizerState(const string& _strKey, D3D11_FILL_MODE fillMode = D3D11_FILL_SOLID,
		D3D11_CULL_MODE cullMode = D3D11_CULL_BACK,
		BOOL frontCounterClockwise = FALSE,
		INT depthBias = D3D11_DEFAULT_DEPTH_BIAS,
		FLOAT depthBiasClamp = D3D11_DEFAULT_DEPTH_BIAS_CLAMP,
		FLOAT slopeScaledDepthBias = D3D11_DEFAULT_SLOPE_SCALED_DEPTH_BIAS,
		BOOL depthClipEnable = TRUE,
		BOOL scissorEnable = FALSE,
		BOOL multisampleEnable = FALSE,
		BOOL antialiasedLineEnable = FALSE);
	// Blend State
	CBlendState* CreateBlendState(const string& _strKey,
		bool _bAlphaCoverage = false, bool _bIndependent = false);
	bool AddRenderTargetBlendInfo(bool _bEnable = true,
		D3D11_BLEND _eSrcBlend = D3D11_BLEND_SRC_ALPHA, D3D11_BLEND _eDestBlend = D3D11_BLEND_INV_SRC_ALPHA,
		D3D11_BLEND_OP _eBlendOp = D3D11_BLEND_OP_ADD,
		D3D11_BLEND _eSrcBlendAplha = D3D11_BLEND_ONE, D3D11_BLEND _eDestBlendAlpha = D3D11_BLEND_ZERO, 
		D3D11_BLEND_OP _eBlendOpAlpha = D3D11_BLEND_OP_ADD,
		UINT8 _iTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL);
	// DepthStencil State
	CDepthStencilState* CreateDepthStencilState(const string& _strKey, bool _bDepthEnable = true,
		D3D11_DEPTH_WRITE_MASK _eDepthMask = D3D11_DEPTH_WRITE_MASK_ALL,
		D3D11_COMPARISON_FUNC _eDepthFunc = D3D11_COMPARISON_LESS,
		bool _bStencilEnable = false, UINT8 _iStencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK,
		UINT8 _iStencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK,
		D3D11_DEPTH_STENCILOP_DESC _tFrontFace = { D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_COMPARISON_ALWAYS },
		D3D11_DEPTH_STENCILOP_DESC _tBackFace = { D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_COMPARISON_ALWAYS });
	CRenderState* FindRenderState(const string& _strKey);	
public:
	bool Init();
	
	DECLARE_SINGLE(CRenderMgr)
};

WOOJUN_END