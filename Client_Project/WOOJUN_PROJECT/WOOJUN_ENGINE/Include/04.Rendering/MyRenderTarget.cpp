#include "MyRenderTarget.h"
#include "../Device.h"
#include "../03.Resource/Texture.h"

WOOJUN_USING

CMyRenderTarget::CMyRenderTarget() :
	m_pTargetTexture(NULL),
	m_pTargetView(NULL),
	m_pOldView(NULL),
	m_pOldDepth(NULL)
{
	memset(m_fClearColor, 0, sizeof(float) * 4);
}

CMyRenderTarget::~CMyRenderTarget()
{
	SAFE_RELEASE(m_pOldDepth);
	SAFE_RELEASE(m_pOldView);
	SAFE_RELEASE(m_pTargetView);
	SAFE_RELEASE(m_pTargetTexture);
}


ID3D11RenderTargetView * CMyRenderTarget::GetTargetView() const
{
	return m_pTargetView;
}

void CMyRenderTarget::SetClearColor(float r, float g, float b, float a)
{
	m_fClearColor[0] = r;
	m_fClearColor[1] = g;
	m_fClearColor[2] = b;
	m_fClearColor[3] = a;
}

void CMyRenderTarget::SetClearColor(float fColor[4])
{
	for (int i = 0; i < 4; ++i)
	{
		m_fClearColor[i] = fColor[i];
	}
}

bool CMyRenderTarget::CreateTarget(const string& _strName, int _iWidth, int _iHeight, DXGI_FORMAT _eFormat)
{
	D3D11_BIND_FLAG eBindFlag = (D3D11_BIND_FLAG)(D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET);
	m_pTargetTexture = CTexture::CreateTexture(_strName, _iWidth, _iHeight, 1, _eFormat,
		D3D11_USAGE_DEFAULT, eBindFlag, 0);
	//D3D11_TEXTURE2D_DESC	tDesc = {};

	//tDesc.Width = _iWidth;
	//tDesc.Height = _iHeight;
	//tDesc.ArraySize = 1;
	//tDesc.Usage = D3D11_USAGE_DEFAULT;
	//tDesc.MipLevels = 1;
	//tDesc.SampleDesc.Count = 1;
	//tDesc.SampleDesc.Quality = 0;
	//tDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	//tDesc.Format = _eFormat;

	//if (FAILED(DEVICE->CreateTexture2D(&tDesc, NULL, &m_pTargetTexture)))
	//	return false;

	if (FAILED(DEVICE->CreateRenderTargetView(m_pTargetTexture->GetTexture(), NULL,
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

void CMyRenderTarget::ClearTarget()
{
	CONTEXT->ClearRenderTargetView(m_pTargetView, m_fClearColor);
}

void CMyRenderTarget::SaveTarget(const WCHAR * _pFileName, const string & _strPathKey)
{
	m_pTargetTexture->SaveTextureFile(_pFileName, _strPathKey);
}
