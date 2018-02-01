#pragma once
#include "../BaseObj.h"

WOOJUN_BEGIN

class CTexture;
class DLL CMyRenderTarget :
	public CBaseObj
{
private:
	friend class CRenderMgr;
private:
	CTexture*	m_pTargetTexture;
	ID3D11RenderTargetView*	m_pTargetView;
	ID3D11RenderTargetView*	m_pOldView;
	ID3D11DepthStencilView*	m_pOldDepth;
	float		m_fClearColor[4];
public:
	ID3D11RenderTargetView* GetTargetView() const;
public:
	void SetClearColor(float r, float g, float b, float a);
	void SetClearColor(float fColor[4]);
public:
	bool CreateTarget(const string& _strName, int _iWidth, int _iHeight, DXGI_FORMAT _eFormat);
	void ChangeTarget();
	void ResetTarget();
	void ClearTarget();
	void SaveTarget(const WCHAR* _pFileName, const string& _strPathKey = TEXTUREPATH);
private:
	CMyRenderTarget();
	~CMyRenderTarget();
};

WOOJUN_END