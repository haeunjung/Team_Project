#pragma once
#include "RenderState.h"

WOOJUN_BEGIN

class DLL CDepthStencilState :
	public CRenderState
{
private:
	friend class CRenderMgr;
protected:
	UINT	m_iStencilRef;
	UINT	m_iOldStencilRef;
public:
	bool CreateDepthStencilState(const string& _strKey, bool _bDepthEnable = true,
		D3D11_DEPTH_WRITE_MASK _eDepthMask = D3D11_DEPTH_WRITE_MASK_ALL,
		D3D11_COMPARISON_FUNC _eDepthFunc = D3D11_COMPARISON_LESS,
		bool _bStencilEnable = false, UINT8 _iStencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK,
		UINT8 _iStencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK,
		D3D11_DEPTH_STENCILOP_DESC _tFrontFace = { D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_COMPARISON_ALWAYS },
		D3D11_DEPTH_STENCILOP_DESC _tBackFace = { D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_COMPARISON_ALWAYS });
public:
	virtual void SetState();
	virtual void ResetState();
private:
	CDepthStencilState();
	~CDepthStencilState();
};

WOOJUN_END