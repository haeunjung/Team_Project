#include "RenderMgr.h"
#include "ShaderMgr.h"
#include "RasterizerState.h"
#include "BlendState.h"
#include "DepthStencilState.h"

WOOJUN_USING

DEFINITION_SINGLE(CRenderMgr)

CRasterizerState * CRenderMgr::CreateRasterizerState(const string & _strKey, D3D11_FILL_MODE fillMode, D3D11_CULL_MODE cullMode,
	BOOL frontCounterClockwise, INT depthBias, FLOAT depthBiasClamp, FLOAT slopeScaledDepthBias,
	BOOL depthClipEnable, BOOL scissorEnable, BOOL multisampleEnable, BOOL antialiasedLineEnable)
{
	CRasterizerState*	pRasterizerState = (CRasterizerState*)FindRenderState(_strKey);

	if (NULL != pRasterizerState)
	{
		return pRasterizerState;
	}

	pRasterizerState = new CRasterizerState();

	if (false == pRasterizerState->CreateRasterizerState(_strKey, fillMode, cullMode,
		frontCounterClockwise, depthBias, depthBiasClamp,
		slopeScaledDepthBias, depthClipEnable, scissorEnable,
		multisampleEnable, antialiasedLineEnable))
	{
		SAFE_RELEASE(pRasterizerState);
		return NULL;
	}

	pRasterizerState->AddRef();

	m_mapRenderState.insert(make_pair(_strKey, pRasterizerState));

	return pRasterizerState;
}

CBlendState* CRenderMgr::CreateBlendState(const string& _strKey,
	bool _bAlphaCoverage /*= false*/, bool _bIndependent /*= false*/)
{
	CBlendState*	pState = (CBlendState*)FindRenderState(_strKey);

	if (NULL != pState)
	{
		return pState;
	}

	pState = new CBlendState();
	if (false == pState->CreateBlendState(_strKey, &m_vecRenderTargetBlend, _bAlphaCoverage, _bIndependent))
	{
		SAFE_RELEASE(pState);
		return NULL;
	}

	pState->AddRef();

	m_mapRenderState.insert(make_pair(_strKey, pState));

	return pState;
}

bool CRenderMgr::AddRenderTargetBlendInfo(bool _bEnable /*= true*/,
	D3D11_BLEND _eSrcBlend /*= D3D11_BLEND_SRC_ALPHA*/, D3D11_BLEND _eDestBlend /*= D3D11_BLEND_INV_SRC_ALPHA*/,
	D3D11_BLEND_OP _eBlendOp /*= D3D11_BLEND_OP_ADD*/,
	D3D11_BLEND _eSrcBlendAplha /*= D3D11_BLEND_ONE*/, D3D11_BLEND _eDestBlendAlpha /*= D3D11_BLEND_ZERO*/,
	D3D11_BLEND_OP _eBlendOpAlpha /*= D3D11_BLEND_OP_ADD*/,
	UINT8 _iTargetWriteMask /*= D3D11_COLOR_WRITE_ENABLE_ALL*/)
{
	if (m_vecRenderTargetBlend.size() >= 8)
	{
		assert(false);
		return false;
	}

	D3D11_RENDER_TARGET_BLEND_DESC	tDesc = {};

	tDesc.BlendEnable = _bEnable;
	tDesc.SrcBlend = _eSrcBlend;
	tDesc.DestBlend = _eDestBlend;
	tDesc.BlendOp = _eBlendOp;
	tDesc.SrcBlendAlpha = _eSrcBlendAplha;
	tDesc.DestBlendAlpha = _eDestBlendAlpha;
	tDesc.BlendOpAlpha = _eBlendOpAlpha;
	tDesc.RenderTargetWriteMask = _iTargetWriteMask;

	m_vecRenderTargetBlend.push_back(tDesc);

	return true;
}

CDepthStencilState*  CRenderMgr::CreateDepthStencilState(const string& _strKey,
	bool _bDepthEnable /*= true*/,
	D3D11_DEPTH_WRITE_MASK _eDepthMask /*= D3D11_DEPTH_WRITE_MASK_ALL*/,
	D3D11_COMPARISON_FUNC _eDepthFunc /*= D3D11_COMPARISON_LESS*/,
	bool _bStencilEnable /*= false*/, 
	UINT8 _iStencilReadMask /*= D3D11_DEFAULT_STENCIL_READ_MASK*/,
	UINT8 _iStencilWriteMask /*= D3D11_DEFAULT_STENCIL_WRITE_MASK*/,
	D3D11_DEPTH_STENCILOP_DESC _tFrontFace /*= { D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_COMPARISON_ALWAYS }*/,
	D3D11_DEPTH_STENCILOP_DESC _tBackFace /*= { D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_COMPARISON_ALWAYS }*/)
{
	CDepthStencilState*	pState = (CDepthStencilState*)FindRenderState(_strKey);

	if (NULL != pState)
	{
		return pState;
	}

	pState = new CDepthStencilState();
	if (false == pState->CreateDepthStencilState(_strKey, _bDepthEnable, _eDepthMask, _eDepthFunc,
		_bStencilEnable, _iStencilReadMask, _iStencilWriteMask, _tFrontFace, _tBackFace))
	{
		SAFE_RELEASE(pState);
		return NULL;
	}

	pState->AddRef();

	m_mapRenderState.insert(make_pair(_strKey, pState));

	return pState;
}

CRenderState * CRenderMgr::FindRenderState(const string & _strKey)
{
	unordered_map<string, CRenderState*>::iterator iter = m_mapRenderState.find(_strKey);

	if (m_mapRenderState.end() == iter)
	{
		return NULL;
	}

	iter->second->AddRef();

	return iter->second;
}

bool CRenderMgr::Init()
{
	if (false == GET_SINGLE(CShaderMgr)->Init())
	{
		return false;
	}

	CRenderState*	 pRenderState = CreateRasterizerState(WIRE_FRAME, D3D11_FILL_WIREFRAME);
	SAFE_RELEASE(pRenderState);
	pRenderState = CreateRasterizerState(CULLING_CW, D3D11_FILL_SOLID, D3D11_CULL_FRONT);
	SAFE_RELEASE(pRenderState);

	AddRenderTargetBlendInfo();		// 디폴트 설정 : 알파블렌딩
	pRenderState = CreateBlendState(ALPHABLEND);
	SAFE_RELEASE(pRenderState);

	pRenderState = CreateDepthStencilState(DEPTH_LESS_EQUAL, true, D3D11_DEPTH_WRITE_MASK_ALL, D3D11_COMPARISON_LESS_EQUAL);
	SAFE_RELEASE(pRenderState);

	pRenderState = CreateDepthStencilState(DEPTH_DISABLE, false);
	SAFE_RELEASE(pRenderState);

	return true;
}

CRenderMgr::CRenderMgr()
{
}

CRenderMgr::~CRenderMgr()
{
	Safe_Release_Map(m_mapRenderState);
	DESTROY_SINGLE(CShaderMgr);
}
