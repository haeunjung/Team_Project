#pragma once
#include "RenderState.h"

WOOJUN_BEGIN

class DLL CBlendState :
	public CRenderState
{
private:
	friend class CRenderMgr;
private:
	UINT	m_iSampleMask;
	UINT	m_iOldSampleMask;
	float	m_fBlendFactor[4];
	float	m_fOldBlendFactor[4];
public:
	bool CreateBlendState(const string& _strKey, const vector<D3D11_RENDER_TARGET_BLEND_DESC>* _pTargetBlend,
		bool _bAlphaCoverage = false, bool _bIndependent = false);
public:
	void SetState() override;
	void ResetState() override;
private:
	CBlendState();
	~CBlendState();
};

WOOJUN_END