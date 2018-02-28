#include "RasterizerState.h"
#include "../Device.h"

WOOJUN_USING

bool CRasterizerState::CreateRasterizerState(const string & _strKey, D3D11_FILL_MODE fillMode, D3D11_CULL_MODE cullMode,
	BOOL frontCounterClockwise, INT depthBias, FLOAT depthBiasClamp, FLOAT slopeScaledDepthBias,
	BOOL depthClipEnable, BOOL scissorEnable, BOOL multisampleEnable, BOOL antialiasedLineEnable)
{
	m_strKey = _strKey;

	D3D11_RASTERIZER_DESC	tDesc = {};

	tDesc.FillMode = fillMode;
	tDesc.CullMode = cullMode;
	tDesc.FrontCounterClockwise = frontCounterClockwise;
	tDesc.DepthBias = depthBias;
	tDesc.DepthBiasClamp = depthBiasClamp;
	tDesc.SlopeScaledDepthBias = slopeScaledDepthBias;
	tDesc.DepthClipEnable = depthClipEnable;
	tDesc.ScissorEnable = scissorEnable;
	tDesc.MultisampleEnable = multisampleEnable;
	tDesc.AntialiasedLineEnable = antialiasedLineEnable;

	if (FAILED(DEVICE->CreateRasterizerState(&tDesc, (ID3D11RasterizerState**)&m_pState)))
	{
		return false;
	}

	return true;
}

void CRasterizerState::SetState()
{
	CONTEXT->RSGetState((ID3D11RasterizerState**)&m_pDefaultState);
	CONTEXT->RSSetState((ID3D11RasterizerState*)m_pState);
}

void CRasterizerState::ResetState()
{
	CONTEXT->RSSetState((ID3D11RasterizerState*)m_pDefaultState);
	SAFE_RELEASE(m_pDefaultState);
}

CRasterizerState::CRasterizerState()
{
	SetTag("RasterizerState");
	SetTypeName("CRasterizerState");
	SetTypeID<CRasterizerState>();
	m_eStateType = RST_RASTERIZER;
}

CRasterizerState::~CRasterizerState()
{
}