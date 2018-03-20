#pragma once
#include "..\Engine_Header.h"

WOOJUN_BEGIN

typedef struct DLL _tagMRT
{
	vector<class CMyRenderTarget*>	vecTarget;
	class CMyDepthTarget*		pDepth;
	ID3D11DepthStencilView*		pNullDepth;
	vector<ID3D11RenderTargetView*>	vecOldTarget;
	ID3D11DepthStencilView*		pOldDepth;
}MRT, *pMRT;

class CRenderState;
class CRasterizerState;
class CBlendState;
class CDepthStencilState;
class CMyRenderTarget;
class CMyDepthTarget;
class CGameObject;
class CSampler;
class CShader;
class DLL CRenderMgr
{
private:
	unordered_map<string, CRenderState*>	m_mapRenderState;	
	vector<D3D11_RENDER_TARGET_BLEND_DESC>	m_vecRenderTargetBlend;
	unordered_map<string, CMyRenderTarget*> m_mapRenderTarget;
	unordered_map<string, CMyDepthTarget*>	m_mapDepthTarget;
	vector<CGameObject*>			m_vecRender[RG_END];
	unordered_map<string, pMRT>		m_mapMRT;
	CSampler*		m_pPointSampler;
	CRenderState*	m_pBlendOne;
	CRenderState*	m_pZDisable;
	CRenderState*	m_pAlphaBlend;
	CShader*		m_pLightAcc;
	CShader*		m_pLightBlend;
	CShader*		m_pLightBlendTarget;
private:
	void RenderGBuffer(float _fTime);
	void RenderLightAcc(float _fTime);
	void RenderLightBlend(float _fTime);
	void RenderLightBlendTarget(float _fTime);
	void UpdateTransform();
public:
	void AddRenderObject(CGameObject* _pGameObject);
	void EraseRenderObject(CGameObject* _pGameObject);
public:
	bool AddMRTTarget(const string& _strMRTKey, const string& _strTargetKey);
	bool SetMRTDepth(const string& _strMRTKey, const string& _strDepthKey);
	pMRT FindMRT(const string& _strKey);
	void ChangeMRT(const string& _strKey);
	void ResetMRT(const string& _strKey);
	void ClearMRT(const string& _strKey);
	CMyRenderTarget* CreateTarget(const string& _strKey, int _iWidth, int _iHeight, DXGI_FORMAT _eFormat);
	CMyDepthTarget* CreateDepthTarget(const string& _strKey, int _iWidth, int _iHeight, DXGI_FORMAT _eFormat);
	void ChangeTarget(const string& _strKey);	
	void ResetTarget(const string& _strKey);
	void ClearTarget(const string& _strKey);
	void ClearDepthTarget(const string& _strKey);
	void SaveTarget(const string& _strKey, const WCHAR* _pFileName, const string& _strPathKey = TEXTUREPATH);
	CMyRenderTarget* FindTarget(const string& _strKey);
	CMyDepthTarget* FindDepthTarget(const string& _strKey);
public:
	// Rasterizer State
	CRasterizerState* CreateRasterizerState(const string& _strKey, D3D11_FILL_MODE fillMode = D3D11_FILL_SOLID,
		D3D11_CULL_MODE cullMode = D3D11_CULL_BACK,
		BOOL frontCounterClockwise = FALSE,
		INT depthBias = D3D11_DEFAULT_DEPTH_BIAS,
		FLOAT depthBiasClamp = D3D11_DEFAULT_DEPTH_BIAS_CLAMP,
		FLOAT slopeScaledDepthBias = D3D11_DEFAULT_SLOPE_SCALED_DEPTH_BIAS,
		BOOL depthClipEnable = TRUE,
		BOOL scissorEnable = FALSE,
		BOOL multisampleEnable = FALSE,
		BOOL antialiasedLineEnable = FALSE);
	// Blend State
	CBlendState* CreateBlendState(const string& _strKey,
		bool _bAlphaCoverage = false, bool _bIndependent = false);
	bool AddRenderTargetBlendInfo(bool _bEnable = true,
		D3D11_BLEND _eSrcBlend = D3D11_BLEND_SRC_ALPHA, D3D11_BLEND _eDestBlend = D3D11_BLEND_INV_SRC_ALPHA,
		D3D11_BLEND_OP _eBlendOp = D3D11_BLEND_OP_ADD,
		D3D11_BLEND _eSrcBlendAplha = D3D11_BLEND_ONE, D3D11_BLEND _eDestBlendAlpha = D3D11_BLEND_ZERO, 
		D3D11_BLEND_OP _eBlendOpAlpha = D3D11_BLEND_OP_ADD,
		UINT8 _iTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL);
	// DepthStencil State
	CDepthStencilState* CreateDepthStencilState(const string& _strKey, bool _bDepthEnable = true,
		D3D11_DEPTH_WRITE_MASK _eDepthMask = D3D11_DEPTH_WRITE_MASK_ALL,
		D3D11_COMPARISON_FUNC _eDepthFunc = D3D11_COMPARISON_LESS,
		bool _bStencilEnable = false, UINT8 _iStencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK,
		UINT8 _iStencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK,
		D3D11_DEPTH_STENCILOP_DESC _tFrontFace = { D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_COMPARISON_ALWAYS },
		D3D11_DEPTH_STENCILOP_DESC _tBackFace = { D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_COMPARISON_ALWAYS });
	CRenderState* FindRenderState(const string& _strKey);	
public:
	bool Init();
	void Render(float _fTime);
public:
	static bool ObjectZSort(CGameObject* _p1, CGameObject* _p2);
	static bool ObjectZSortDescending(CGameObject* _p1, CGameObject* _p2);

	DECLARE_SINGLE(CRenderMgr)
};

WOOJUN_END