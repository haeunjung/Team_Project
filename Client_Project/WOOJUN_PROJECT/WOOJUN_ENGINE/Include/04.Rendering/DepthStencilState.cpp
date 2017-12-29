#include "DepthStencilState.h"
#include "../Device.h"

WOOJUN_USING

bool CDepthStencilState::CreateDepthStencilState(const string& _strKey, 
	bool _bDepthEnable /*= true*/,
	D3D11_DEPTH_WRITE_MASK _eDepthMask /*= D3D11_DEPTH_WRITE_MASK_ALL*/,
	D3D11_COMPARISON_FUNC _eDepthFunc /*= D3D11_COMPARISON_LESS*/,
	bool _bStencilEnable /*= false*/,
	UINT8 _iStencilReadMask /*= D3D11_DEFAULT_STENCIL_READ_MASK*/,
	UINT8 _iStencilWriteMask /*= D3D11_DEFAULT_STENCIL_WRITE_MASK*/,
	D3D11_DEPTH_STENCILOP_DESC _tFrontFace /*= { D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_COMPARISON_ALWAYS }*/,
	D3D11_DEPTH_STENCILOP_DESC _tBackFace /*= { D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_COMPARISON_ALWAYS }*/)
{
	D3D11_DEPTH_STENCIL_DESC	tDesc = {};
	tDesc.DepthEnable = _bDepthEnable;
	tDesc.DepthWriteMask = _eDepthMask;
	tDesc.DepthFunc = _eDepthFunc;
	tDesc.StencilEnable = _bStencilEnable;
	tDesc.StencilReadMask = _iStencilReadMask;
	tDesc.StencilWriteMask = _iStencilWriteMask;
	tDesc.FrontFace = _tFrontFace;
	tDesc.BackFace = _tBackFace;

	if (FAILED(DEVICE->CreateDepthStencilState(&tDesc, (ID3D11DepthStencilState**)&m_pState)))
	{
		return false;
	}

	return true;
}

void CDepthStencilState::SetState()
{
	CONTEXT->OMGetDepthStencilState((ID3D11DepthStencilState**)&m_pDefaultState, &m_iOldStencilRef);
	CONTEXT->OMSetDepthStencilState((ID3D11DepthStencilState*)m_pState, m_iStencilRef);
}

void CDepthStencilState::ResetState()
{
	CONTEXT->OMSetDepthStencilState((ID3D11DepthStencilState*)m_pDefaultState, m_iOldStencilRef);
	SAFE_RELEASE(m_pDefaultState);
}

CDepthStencilState::CDepthStencilState()
{
	SetTag("DetphStencilState");
	SetTypeName("CDepthStencilState");
	SetTypeID<CDepthStencilState>();
	m_eStateType = RST_DEPTH;
	m_iStencilRef = 0xffffffff;
}

CDepthStencilState::~CDepthStencilState()
{
}
