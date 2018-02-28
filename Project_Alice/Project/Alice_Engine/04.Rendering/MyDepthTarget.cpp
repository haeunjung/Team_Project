#include "MyDepthTarget.h"
#include "../Device.h"
#include "../03.Resource/Texture.h"

WOOJUN_USING

CMyDepthTarget::CMyDepthTarget() :
	m_pDepthTexture(NULL),
	m_pDepthView(NULL),
	m_pOldView(NULL)
{
}

CMyDepthTarget::~CMyDepthTarget()
{
	SAFE_RELEASE(m_pOldView);
	SAFE_RELEASE(m_pDepthView);
	SAFE_RELEASE(m_pDepthTexture);
}

ID3D11DepthStencilView * CMyDepthTarget::GetDepthView() const
{
	return m_pDepthView;
}

bool CMyDepthTarget::CreateDepth(const string & _strName, int _iWidth, int _iHeight, DXGI_FORMAT _eFormat)
{
	// ±íÀÌ Å¸°ÙÀ» ¸¸µç´Ù.
	D3D11_TEXTURE2D_DESC	tDesc = {};
	tDesc.Width = _iWidth;
	tDesc.Height = _iHeight;
	tDesc.MipLevels = 1;
	tDesc.ArraySize = 1;
	tDesc.Format = _eFormat;
	tDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	tDesc.SampleDesc.Count = 1;
	tDesc.SampleDesc.Quality = 0;
	tDesc.Usage = D3D11_USAGE_DEFAULT;

	if (FAILED(DEVICE->CreateTexture2D(&tDesc, NULL, &m_pDepthTexture)))
	{
		return false;
	}

	if (FAILED(DEVICE->CreateDepthStencilView(m_pDepthTexture, NULL, &m_pDepthView)))
	{
		return false;
	}

	return true;
}

void CMyDepthTarget::ClearTarget()
{
	CONTEXT->ClearDepthStencilView(m_pDepthView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
}


