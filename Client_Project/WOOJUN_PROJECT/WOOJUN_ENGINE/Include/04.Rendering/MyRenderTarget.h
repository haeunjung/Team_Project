#pragma once
#include "../BaseObj.h"

WOOJUN_BEGIN

class CTexture;
class CMesh;
class CShader;
class CSampler;
class CRenderState;
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
#ifdef _DEBUG
	bool			m_bRender;
	CMesh*			m_pMesh;
	CShader*		m_pShader;
	MATERIALINFO	m_tMaterial;
	DxVector3		m_vScale;
	DxVector3		m_vPos;
	CSampler*		m_pSampler;
	CRenderState*	m_pZDisable;
#endif
public:
	ID3D11RenderTargetView* GetTargetView() const;
public:
	void OnRender(bool _bRender);
	void SetPos(float _x, float _y, float _z);
	void SetScale(float _x, float _y, float _z);
	void SetClearColor(float r, float g, float b, float a);
	void SetClearColor(float fColor[4]);
public:
	bool CreateTarget(const string& _strName, int _iWidth, int _iHeight, DXGI_FORMAT _eFormat);
	void ChangeTarget();
	void ResetTarget();
	void ClearTarget();
	void SaveTarget(const WCHAR* _pFileName, const string& _strPathKey = TEXTUREPATH);
	void SetTexture(int _iRegister);
	void Render();
private:
	CMyRenderTarget();
	~CMyRenderTarget();
};

WOOJUN_END