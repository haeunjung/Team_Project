#pragma once
#include "RenderState.h"

WOOJUN_BEGIN

class DLL CRasterizerState :
	public CRenderState
{
private:
	friend class CRenderMgr;
public:
	bool CreateRasterizerState(const string& _strKey, D3D11_FILL_MODE fillMode = D3D11_FILL_SOLID,
		D3D11_CULL_MODE cullMode = D3D11_CULL_BACK,
		BOOL frontCounterClockwise = FALSE,
		INT depthBias = D3D11_DEFAULT_DEPTH_BIAS,
		FLOAT depthBiasClamp = D3D11_DEFAULT_DEPTH_BIAS_CLAMP,
		FLOAT slopeScaledDepthBias = D3D11_DEFAULT_SLOPE_SCALED_DEPTH_BIAS,
		BOOL depthClipEnable = TRUE,
		BOOL scissorEnable = FALSE,
		BOOL multisampleEnable = FALSE,
		BOOL antialiasedLineEnable = FALSE);
public:
	void SetState() override;
	void ResetState() override;
private:
	CRasterizerState();
	~CRasterizerState();
};

WOOJUN_END