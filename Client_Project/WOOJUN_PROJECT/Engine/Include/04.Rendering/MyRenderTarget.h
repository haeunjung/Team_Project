#pragma once
#include "../BaseObj.h"

WOOJUN_BEGIN

class DLL CMyRenderTarget :
	public CBaseObj
{
private:
	friend class CRenderMgr;
private:
	ID3D11Texture2D*		m_pTargetTexture;
	ID3D11RenderTargetView*	m_pTargetView;
	ID3D11RenderTargetView*	m_pOldView;
	ID3D11DepthStencilView*	m_pOldDepth;
public:
	bool CreateTarget(int _iWidth, int _iHeight, DXGI_FORMAT _eFormat);
	void ChangeTarget();
	void ResetTarget();
private:
	CMyRenderTarget();
	~CMyRenderTarget();
};

WOOJUN_END