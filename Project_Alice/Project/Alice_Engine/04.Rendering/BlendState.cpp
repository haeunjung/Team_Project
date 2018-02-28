#include "BlendState.h"
#include "../Device.h"

WOOJUN_USING

bool CBlendState::CreateBlendState(const string & _strKey, const vector<D3D11_RENDER_TARGET_BLEND_DESC>* _pTargetBlend, bool _bAlphaCoverage, bool _bIndependent)
{
	D3D11_BLEND_DESC	tDesc = {};
	tDesc.AlphaToCoverageEnable = _bAlphaCoverage;
	tDesc.IndependentBlendEnable = _bIndependent;

	int iCount = 0;
	vector<D3D11_RENDER_TARGET_BLEND_DESC>::const_iterator iter;
	vector<D3D11_RENDER_TARGET_BLEND_DESC>::const_iterator iterEnd = _pTargetBlend->end();

	for (iter = _pTargetBlend->begin(); iter != iterEnd; ++iter, ++iCount)
	{
		tDesc.RenderTarget[iCount] = *iter;
	}

	if (FAILED(DEVICE->CreateBlendState(&tDesc, (ID3D11BlendState**)&m_pState)))
	{
		return false;
	}

	return true;
}

void CBlendState::SetState()
{
	CONTEXT->OMGetBlendState((ID3D11BlendState**)&m_pDefaultState, m_fOldBlendFactor, &m_iOldSampleMask);
	CONTEXT->OMSetBlendState((ID3D11BlendState*)m_pState, m_fBlendFactor, m_iSampleMask);
}

void CBlendState::ResetState()
{
	CONTEXT->OMSetBlendState((ID3D11BlendState*)m_pDefaultState, m_fOldBlendFactor, m_iOldSampleMask);
	SAFE_RELEASE(m_pDefaultState);
}

CBlendState::CBlendState() :
	m_iSampleMask(0xffffffff)
{
	SetTag("BlendState");
	SetTypeName("CBlendState");
	SetTypeID<CBlendState>();
	m_eStateType = RST_BLEND;

	memset(m_fBlendFactor, 0, sizeof(float) * 4);
}

CBlendState::~CBlendState()
{
}
