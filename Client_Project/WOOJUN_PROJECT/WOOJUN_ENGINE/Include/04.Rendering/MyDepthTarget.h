#pragma once
#include "../BaseObj.h"

WOOJUN_BEGIN

class DLL CMyDepthTarget :
	public CBaseObj
{
private:
	friend class CRenderMgr;
private:
	ID3D11Texture2D*		m_pDepthTexture;
	ID3D11DepthStencilView*	m_pDepthView;
	ID3D11DepthStencilView*	m_pOldView;
public:
	ID3D11DepthStencilView* GetDepthView() const;
public:
	bool CreateDepth(const string& _strName, int _iWidth, int _iHeight, DXGI_FORMAT _eFormat);
	void ClearTarget();
private:
	CMyDepthTarget();
	~CMyDepthTarget();
};

WOOJUN_END