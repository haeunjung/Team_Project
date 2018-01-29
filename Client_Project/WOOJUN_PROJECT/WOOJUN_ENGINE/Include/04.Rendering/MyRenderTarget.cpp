#include "MyRenderTarget.h"
#include "../Device.h"

WOOJUN_USING

CMyRenderTarget::CMyRenderTarget() :
	m_pTargetTexture(NULL),
	m_pTargetView(NULL),
	m_pOldView(NULL),
	m_pOldDepth(NULL)
{
}

CMyRenderTarget::~CMyRenderTarget()
{
	SAFE_RELEASE(m_pOldDepth);
	SAFE_RELEASE(m_pOldView);
	SAFE_RELEASE(m_pTargetView);
	SAFE_RELEASE(m_pTargetTexture);
}


bool CMyRenderTarget::CreateTarget(int _iWidth, int _iHeight, DXGI_FORMAT _eFormat)
{
	D3D11_TEXTURE2D_DESC	tDesc = {};

	tDesc.Width = _iWidth;
	tDesc.Height = _iHeight;
	tDesc.ArraySize = 1;
	tDesc.Usage = D3D11_USAGE_DEFAULT;
	tDesc.MipLevels = 1;
	tDesc.SampleDesc.Count = 1;
	tDesc.SampleDesc.Quality = 0;
	tDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	tDesc.Format = _eFormat;

	if (FAILED(DEVICE->CreateTexture2D(&tDesc, NULL, &m_pTargetTexture)))
		return false;

	if (FAILED(DEVICE->CreateRenderTargetView(m_pTargetTexture, NULL,
		&m_pTargetView)))
		return false;

	return true;
}

void CMyRenderTarget::ChangeTarget()
{
	CONTEXT->OMGetRenderTargets(1, &m_pOldView, &m_pOldDepth);
	CONTEXT->OMSetRenderTargets(1, &m_pTargetView, m_pOldDepth);
}

void CMyRenderTarget::ResetTarget()
{
	CONTEXT->OMSetRenderTargets(1, &m_pOldView, m_pOldDepth);
	SAFE_RELEASE(m_pOldView);
	SAFE_RELEASE(m_pOldDepth);
}
