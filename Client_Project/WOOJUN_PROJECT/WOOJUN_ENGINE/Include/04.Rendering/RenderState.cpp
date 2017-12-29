#include "RenderState.h"

WOOJUN_USING

CRenderState::CRenderState() :
	m_pState(NULL),
	m_pDefaultState(NULL)
{
}

CRenderState::~CRenderState()
{
	SAFE_RELEASE(m_pState);
	SAFE_RELEASE(m_pDefaultState);
}
