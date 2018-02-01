#include "RenderMgr.h"
#include "ShaderMgr.h"
#include "RasterizerState.h"
#include "BlendState.h"
#include "DepthStencilState.h"
#include "MyRenderTarget.h"
#include "MyDepthTarget.h"
#include "../Engine_Core.h"
#include "../Device.h"
#include "../05.Scene/SceneMgr.h"
#include "../05.Scene/Scene.h"
#include "../06.GameObject/GameObject.h"
#include "../07.Component/Renderer.h"
#include "../07.Component/Transform.h"
#include "../07.Component/Camera.h"
#include "../07.Component/Light.h"

WOOJUN_USING

DEFINITION_SINGLE(CRenderMgr)

void CRenderMgr::RenderGBuffer(float _fTime)
{
	ChangeMRT("GBuffer");
	ClearMRT("GBuffer");

	for (size_t i = 0; i < m_vecRender[RG_DEFAULT].size(); ++i)
	{
		m_vecRender[RG_DEFAULT][i]->Render(_fTime);
	}

	ResetMRT("GBuffer");
}

void CRenderMgr::RenderLightAcc(float _fTime)
{
	ChangeMRT("LightAcc");
	ClearMRT("LightAcc");

	CScene*		pScene = GET_SINGLE(CSceneMgr)->GetCurScene();

	const list<CGameObject*>*	pLightList = pScene->GetLightList();

	list<CGameObject*>::const_iterator	iter;
	list<CGameObject*>::const_iterator	iterEnd = pLightList->end();

	for (iter = pLightList->begin(); iter != iterEnd; ++iter)
	{
		CLight*		pLight = (CLight*)(*iter)->FindComponentFromType(CT_LIGHT);
		pLight->SetLight();
		SAFE_RELEASE(pLight);

		UINT	iOffset = 0;
		CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		CONTEXT->IASetVertexBuffers(0, 0, NULL, 0, &iOffset);
		CONTEXT->IASetIndexBuffer(0, DXGI_FORMAT_UNKNOWN, 0);
		CONTEXT->Draw(4, 0);
	}

	ResetMRT("LightAcc");
}

void CRenderMgr::AddRenderObject(CGameObject * _pGameObject)
{
	if (_pGameObject->CheckComponentFromType(CT_UI))
	{
		m_vecRender[RG_UI].push_back(_pGameObject);
	}
	else
	{
		CRenderer*	pRenderer = (CRenderer*)_pGameObject->FindComponentFromType(CT_RENDERER);

		if (pRenderer)
		{
			if (pRenderer->BlendEnable())
			{
				m_vecRender[RG_ALPHA3].push_back(_pGameObject);
			}
			else
			{
				m_vecRender[RG_DEFAULT].push_back(_pGameObject);
			}

			SAFE_RELEASE(pRenderer);
		}
		else
		{
			m_vecRender[RG_ALPHA3].push_back(_pGameObject);
		}
	}
}

bool CRenderMgr::AddMRTTarget(const string & _strMRTKey, const string & _strTargetKey)
{
	pMRT	pMRT = FindMRT(_strMRTKey);

	if (!pMRT)
	{
		pMRT = new MRT();
		pMRT->pDepth = NULL;
		pMRT->pOldDepth = NULL;

		m_mapMRT.insert(make_pair(_strMRTKey, pMRT));
	}

	CMyRenderTarget*	pTarget = FindTarget(_strTargetKey);
	pMRT->vecTarget.push_back(pTarget);

	return true;
}

bool CRenderMgr::SetMRTDepth(const string & _strMRTKey, const string & _strDepthKey)
{
	pMRT	pMRT = FindMRT(_strMRTKey);

	if (!pMRT)
	{
		pMRT = new MRT();
		pMRT->pDepth = NULL;
		pMRT->pOldDepth = NULL;

		m_mapMRT.insert(make_pair(_strMRTKey, pMRT));
	}

	CMyDepthTarget*	pTarget = FindDepthTarget(_strDepthKey);
	pMRT->pDepth = pTarget;

	return true;
}

pMRT CRenderMgr::FindMRT(const string & _strKey)
{
	unordered_map<string, pMRT>::iterator	iter = m_mapMRT.find(_strKey);

	if (iter == m_mapMRT.end())
		return NULL;

	return iter->second;
}

void CRenderMgr::ChangeMRT(const string & _strKey)
{
	pMRT	pMRT = FindMRT(_strKey);

	if (!pMRT)
	{
		return;
	}

	// 타겟정보를 만든다.
	vector<ID3D11RenderTargetView*>	vecTarget;
	for (size_t i = 0; i < pMRT->vecTarget.size(); ++i)
	{
		vecTarget.push_back(pMRT->vecTarget[i]->GetTargetView());
	}

	ID3D11DepthStencilView*	pDepthView = pMRT->pDepth->GetDepthView();
	pMRT->vecOldTarget.clear();
	pMRT->vecOldTarget.resize(pMRT->vecTarget.size());

	CONTEXT->OMGetRenderTargets(pMRT->vecOldTarget.size(), &pMRT->vecOldTarget[0], &pMRT->pOldDepth);
	CONTEXT->OMSetRenderTargets(vecTarget.size(), &vecTarget[0], pMRT->pDepth->GetDepthView());
}

void CRenderMgr::ResetMRT(const string & _strKey)
{
	pMRT	pMRT = FindMRT(_strKey);

	if (!pMRT)
		return;

	CONTEXT->OMSetRenderTargets(pMRT->vecOldTarget.size(), &pMRT->vecOldTarget[0], pMRT->pOldDepth);
	Safe_Release_VecList(pMRT->vecOldTarget);
	SAFE_RELEASE(pMRT->pOldDepth);
}

void CRenderMgr::ClearMRT(const string & _strKey)
{
	pMRT	pMRT = FindMRT(_strKey);

	if (!pMRT)
		return;

	for (size_t i = 0; i < pMRT->vecTarget.size(); ++i)
	{
		pMRT->vecTarget[i]->ClearTarget();
	}

	pMRT->pDepth->ClearTarget();
}

CMyRenderTarget * CRenderMgr::CreateTarget(const string & _strKey, int _iWidth, int _iHeight, DXGI_FORMAT _eFormat)
{
	CMyRenderTarget*	pTarget = new CMyRenderTarget();

	if (!pTarget->CreateTarget(_strKey, _iWidth, _iHeight, _eFormat))
	{
		SAFE_RELEASE(pTarget);
		return NULL;
	}

	pTarget->AddRef();

	m_mapRenderTarget.insert(make_pair(_strKey, pTarget));

	return pTarget;
}

CMyDepthTarget * CRenderMgr::CreateDepthTarget(const string & _strKey, int _iWidth, int _iHeight, DXGI_FORMAT _eFormat)
{
	CMyDepthTarget*	pTarget = new CMyDepthTarget;

	if (!pTarget->CreateDepth(_strKey, _iWidth, _iHeight, _eFormat))
	{
		SAFE_RELEASE(pTarget);
		return NULL;
	}

	pTarget->AddRef();

	m_mapDepthTarget.insert(make_pair(_strKey, pTarget));

	return pTarget;
}

void CRenderMgr::ChangeTarget(const string & _strKey)
{
	CMyRenderTarget*	pTarget = FindTarget(_strKey);

	if (!pTarget)
	{
		return;
	}

	pTarget->ChangeTarget();

	SAFE_RELEASE(pTarget);
}

void CRenderMgr::ResetTarget(const string & _strKey)
{
	CMyRenderTarget*	pTarget = FindTarget(_strKey);

	if (!pTarget)
	{
		return;
	}

	pTarget->ResetTarget();

	SAFE_RELEASE(pTarget);
}

void CRenderMgr::ClearTarget(const string & _strKey)
{
	CMyRenderTarget*	pTarget = FindTarget(_strKey);

	if (!pTarget)
	{
		return;
	}

	pTarget->ClearTarget();

	SAFE_RELEASE(pTarget);
}

void CRenderMgr::ClearDepthTarget(const string & _strKey)
{
	CMyDepthTarget*	pTarget = FindDepthTarget(_strKey);

	if (!pTarget)
	{
		return;
	}

	pTarget->ClearTarget();

	SAFE_RELEASE(pTarget);
}

void CRenderMgr::SaveTarget(const string & _strKey, const WCHAR * _pFileName, const string & _strPathKey)
{
	CMyRenderTarget*	pTarget = FindTarget(_strKey);

	if (!pTarget)
	{
		return;
	}

	pTarget->SaveTarget(_pFileName, _strPathKey);

	SAFE_RELEASE(pTarget);
}

CMyRenderTarget * CRenderMgr::FindTarget(const string & _strKey)
{
	unordered_map<string, CMyRenderTarget*>::iterator	iter = m_mapRenderTarget.find(_strKey);

	if (iter == m_mapRenderTarget.end())
	{
		return NULL;
	}

	iter->second->AddRef();

	return iter->second;
}

CMyDepthTarget * CRenderMgr::FindDepthTarget(const string & _strKey)
{
	unordered_map<string, CMyDepthTarget*>::iterator	iter = m_mapDepthTarget.find(_strKey);

	if (iter == m_mapDepthTarget.end())
	{
		return NULL;
	}

	iter->second->AddRef();

	return iter->second;
}

CRasterizerState * CRenderMgr::CreateRasterizerState(const string & _strKey, D3D11_FILL_MODE fillMode, D3D11_CULL_MODE cullMode,
	BOOL frontCounterClockwise, INT depthBias, FLOAT depthBiasClamp, FLOAT slopeScaledDepthBias,
	BOOL depthClipEnable, BOOL scissorEnable, BOOL multisampleEnable, BOOL antialiasedLineEnable)
{
	CRasterizerState*	pRasterizerState = (CRasterizerState*)FindRenderState(_strKey);

	if (NULL != pRasterizerState)
	{
		return pRasterizerState;
	}

	pRasterizerState = new CRasterizerState();

	if (false == pRasterizerState->CreateRasterizerState(_strKey, fillMode, cullMode,
		frontCounterClockwise, depthBias, depthBiasClamp,
		slopeScaledDepthBias, depthClipEnable, scissorEnable,
		multisampleEnable, antialiasedLineEnable))
	{
		SAFE_RELEASE(pRasterizerState);
		return NULL;
	}

	pRasterizerState->AddRef();

	m_mapRenderState.insert(make_pair(_strKey, pRasterizerState));

	return pRasterizerState;
}

CBlendState* CRenderMgr::CreateBlendState(const string& _strKey,
	bool _bAlphaCoverage /*= false*/, bool _bIndependent /*= false*/)
{
	CBlendState*	pState = (CBlendState*)FindRenderState(_strKey);

	if (NULL != pState)
	{
		return pState;
	}

	pState = new CBlendState();
	if (false == pState->CreateBlendState(_strKey, &m_vecRenderTargetBlend, _bAlphaCoverage, _bIndependent))
	{
		SAFE_RELEASE(pState);
		return NULL;
	}

	pState->AddRef();

	m_mapRenderState.insert(make_pair(_strKey, pState));

	return pState;
}

bool CRenderMgr::AddRenderTargetBlendInfo(bool _bEnable /*= true*/,
	D3D11_BLEND _eSrcBlend /*= D3D11_BLEND_SRC_ALPHA*/, D3D11_BLEND _eDestBlend /*= D3D11_BLEND_INV_SRC_ALPHA*/,
	D3D11_BLEND_OP _eBlendOp /*= D3D11_BLEND_OP_ADD*/,
	D3D11_BLEND _eSrcBlendAplha /*= D3D11_BLEND_ONE*/, D3D11_BLEND _eDestBlendAlpha /*= D3D11_BLEND_ZERO*/,
	D3D11_BLEND_OP _eBlendOpAlpha /*= D3D11_BLEND_OP_ADD*/,
	UINT8 _iTargetWriteMask /*= D3D11_COLOR_WRITE_ENABLE_ALL*/)
{
	if (m_vecRenderTargetBlend.size() >= 8)
	{
		assert(false);
		return false;
	}

	D3D11_RENDER_TARGET_BLEND_DESC	tDesc = {};

	tDesc.BlendEnable = _bEnable;
	tDesc.SrcBlend = _eSrcBlend;
	tDesc.DestBlend = _eDestBlend;
	tDesc.BlendOp = _eBlendOp;
	tDesc.SrcBlendAlpha = _eSrcBlendAplha;
	tDesc.DestBlendAlpha = _eDestBlendAlpha;
	tDesc.BlendOpAlpha = _eBlendOpAlpha;
	tDesc.RenderTargetWriteMask = _iTargetWriteMask;

	m_vecRenderTargetBlend.push_back(tDesc);

	return true;
}

CDepthStencilState*  CRenderMgr::CreateDepthStencilState(const string& _strKey,
	bool _bDepthEnable /*= true*/,
	D3D11_DEPTH_WRITE_MASK _eDepthMask /*= D3D11_DEPTH_WRITE_MASK_ALL*/,
	D3D11_COMPARISON_FUNC _eDepthFunc /*= D3D11_COMPARISON_LESS*/,
	bool _bStencilEnable /*= false*/, 
	UINT8 _iStencilReadMask /*= D3D11_DEFAULT_STENCIL_READ_MASK*/,
	UINT8 _iStencilWriteMask /*= D3D11_DEFAULT_STENCIL_WRITE_MASK*/,
	D3D11_DEPTH_STENCILOP_DESC _tFrontFace /*= { D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_COMPARISON_ALWAYS }*/,
	D3D11_DEPTH_STENCILOP_DESC _tBackFace /*= { D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_COMPARISON_ALWAYS }*/)
{
	CDepthStencilState*	pState = (CDepthStencilState*)FindRenderState(_strKey);

	if (NULL != pState)
	{
		return pState;
	}

	pState = new CDepthStencilState();
	if (false == pState->CreateDepthStencilState(_strKey, _bDepthEnable, _eDepthMask, _eDepthFunc,
		_bStencilEnable, _iStencilReadMask, _iStencilWriteMask, _tFrontFace, _tBackFace))
	{
		SAFE_RELEASE(pState);
		return NULL;
	}

	pState->AddRef();

	m_mapRenderState.insert(make_pair(_strKey, pState));

	return pState;
}

CRenderState * CRenderMgr::FindRenderState(const string & _strKey)
{
unordered_map<string, CRenderState*>::iterator iter = m_mapRenderState.find(_strKey);

if (m_mapRenderState.end() == iter)
{
	return NULL;
}

iter->second->AddRef();

return iter->second;
}

bool CRenderMgr::Init()
{
	if (false == GET_SINGLE(CShaderMgr)->Init())
	{
		return false;
	}

	CRenderState*	 pRenderState = CreateRasterizerState(WIRE_FRAME, D3D11_FILL_WIREFRAME);
	SAFE_RELEASE(pRenderState);
	pRenderState = CreateRasterizerState(CULLING_CW, D3D11_FILL_SOLID, D3D11_CULL_FRONT);
	SAFE_RELEASE(pRenderState);

	AddRenderTargetBlendInfo();		// 디폴트 설정 : 알파블렌딩
	pRenderState = CreateBlendState(ALPHABLEND);
	SAFE_RELEASE(pRenderState);

	AddRenderTargetBlendInfo(true, D3D11_BLEND_ONE, D3D11_BLEND_ONE);
	pRenderState = CreateBlendState(ACC_BLEND);
	SAFE_RELEASE(pRenderState);

	pRenderState = CreateDepthStencilState(DEPTH_LESS_EQUAL, true, D3D11_DEPTH_WRITE_MASK_ALL, D3D11_COMPARISON_LESS_EQUAL);
	SAFE_RELEASE(pRenderState);

	pRenderState = CreateDepthStencilState(DEPTH_DISABLE, false);
	SAFE_RELEASE(pRenderState);

	// Albedo Target
	CMyRenderTarget*	pTarget = CreateTarget("Albedo", _RESOLUTION.m_iWidth, _RESOLUTION.m_iHeight, DXGI_FORMAT_R8G8B8A8_UNORM);
	SAFE_RELEASE(pTarget);

	// Normal Target
	pTarget = CreateTarget("Normal", _RESOLUTION.m_iWidth, _RESOLUTION.m_iHeight, DXGI_FORMAT_R8G8B8A8_UNORM);
	SAFE_RELEASE(pTarget);

	pTarget = CreateTarget("Depth", _RESOLUTION.m_iWidth, _RESOLUTION.m_iHeight, DXGI_FORMAT_R32G32B32A32_FLOAT);
	SAFE_RELEASE(pTarget);

	pTarget = CreateTarget("Shininess", _RESOLUTION.m_iWidth, _RESOLUTION.m_iHeight, DXGI_FORMAT_R8G8B8A8_UNORM);
	SAFE_RELEASE(pTarget);

	// 깊이를 생성한다.
	CMyDepthTarget*	pDepth = CreateDepthTarget("GBufferDepth",
		_RESOLUTION.m_iWidth, _RESOLUTION.m_iHeight, DXGI_FORMAT_D24_UNORM_S8_UINT);
	SAFE_RELEASE(pDepth);

	// GBuffer MRT 생성
	AddMRTTarget("GBuffer", "Albedo");
	AddMRTTarget("GBuffer", "Normal");
	AddMRTTarget("GBuffer", "Depth");
	AddMRTTarget("GBuffer", "Shininess");
	SetMRTDepth("GBuffer", "GBufferDepth");

	// 조명 Diffuse, Ambient 누적버퍼 생성
	pTarget = CreateTarget("LightAccDif", _RESOLUTION.m_iWidth, _RESOLUTION.m_iHeight, DXGI_FORMAT_R32G32B32A32_FLOAT);
	SAFE_RELEASE(pTarget);

	pTarget = CreateTarget("LightAccSpc", _RESOLUTION.m_iWidth, _RESOLUTION.m_iHeight, DXGI_FORMAT_R32G32B32A32_FLOAT);
	SAFE_RELEASE(pTarget);

	pDepth = CreateDepthTarget("LightAccDepth", _RESOLUTION.m_iWidth, _RESOLUTION.m_iHeight, DXGI_FORMAT_D24_UNORM_S8_UINT);
	SAFE_RELEASE(pDepth);

	AddMRTTarget("LightAcc", "LightAccDif");
	AddMRTTarget("LightAcc", "LightAccSpc");
	SetMRTDepth("LightAcc", "LightAccDepth");

	return true;
}

void CRenderMgr::Render(float _fTime)
{
	// 출력 오브젝트들을 정렬한다.
	for (int i = 0; i < RG_END; ++i)
	{
		if (i == RG_DEFAULT || i == RG_UI)
		{
			if (m_vecRender[i].size() >= 2)
				sort(m_vecRender[i].begin(), m_vecRender[i].end(), CRenderMgr::ObjectZSortDescending);
		}

		else
		{
			if (m_vecRender[i].size() >= 2)
				sort(m_vecRender[i].begin(), m_vecRender[i].end(), CRenderMgr::ObjectZSort);
		}
	}

	// GBuffer를 만들어준다
	RenderGBuffer(_fTime);

	// 조명 누적 버퍼를 만들어준다
	RenderLightAcc(_fTime);

	// 알파오브젝트와 UI를 그린다
	for (int i = RG_ALPHA1; i < RG_END; ++i)
	{
		for (size_t j = 0; j < m_vecRender[i].size(); ++j)
		{
			m_vecRender[i][j]->Render(_fTime);
		}
	}

	if (GetAsyncKeyState(VK_RETURN) & 0x8000)
	{
		SaveTarget("Albedo", L"Albedo.png");
		SaveTarget("Normal", L"Normal.png");
		SaveTarget("Depth", L"Depth.png");
		SaveTarget("Shininess", L"Shininess.png");
	}

	for (int i = 0; i < RG_END; ++i)
	{
		m_vecRender[i].clear();
	}
}

bool CRenderMgr::ObjectZSort(CGameObject * _p1, CGameObject * _p2)
{
	// 두개의 비교대상
	CTransform*	pTransform1 = _p1->GetTransform();
	CTransform*	pTransform2 = _p2->GetTransform();

	// 카메라
	CGameObject*	pCameraObject = GET_SINGLE(CSceneMgr)->GetCurScene()->GetMainCameraObject();
	CTransform*		pCameraTransform = pCameraObject->GetTransform();

	// 카메라로부터 거리
	float	fDist1, fDist2 = 0.0f;
	fDist1 = pTransform1->GetWorldPos().Distance(pCameraTransform->GetWorldPos());
	fDist2 = pTransform2->GetWorldPos().Distance(pCameraTransform->GetWorldPos());

	SAFE_RELEASE(pCameraTransform);
	SAFE_RELEASE(pTransform1);
	SAFE_RELEASE(pTransform2);

	SAFE_RELEASE(pCameraObject);

	return fDist1 < fDist2;
}

bool CRenderMgr::ObjectZSortDescending(CGameObject * _p1, CGameObject * _p2)
{
	// 두개의 비교대상
	CTransform*	pTransform1 = _p1->GetTransform();
	CTransform*	pTransform2 = _p2->GetTransform();

	// 카메라
	CGameObject*	pCameraObject = GET_SINGLE(CSceneMgr)->GetCurScene()->GetMainCameraObject();
	CTransform*		pCameraTransform = pCameraObject->GetTransform();

	// 카메라로부터 거리
	float	fDist1, fDist2 = 0.0f;
	fDist1 = pTransform1->GetWorldPos().Distance(pCameraTransform->GetWorldPos());
	fDist2 = pTransform2->GetWorldPos().Distance(pCameraTransform->GetWorldPos());

	SAFE_RELEASE(pCameraTransform);
	SAFE_RELEASE(pTransform1);
	SAFE_RELEASE(pTransform2);

	SAFE_RELEASE(pCameraObject);

	return fDist1 > fDist2;
}

CRenderMgr::CRenderMgr()
{
	for (int i = 0; i < RG_END; ++i)
	{
		m_vecRender[i].reserve(200);
	}
}

CRenderMgr::~CRenderMgr()
{
	unordered_map<string, pMRT>::iterator	iter;
	unordered_map<string, pMRT>::iterator	iterEnd = m_mapMRT.end();

	for (iter = m_mapMRT.begin(); iter != iterEnd; ++iter)
	{
		for (size_t i = 0; i < iter->second->vecTarget.size(); ++i)
		{
			SAFE_RELEASE(iter->second->vecTarget[i]);
		}

		for (size_t i = 0; i < iter->second->vecOldTarget.size(); ++i)
		{
			SAFE_RELEASE(iter->second->vecOldTarget[i]);
		}

		SAFE_RELEASE(iter->second->pDepth);
		SAFE_RELEASE(iter->second->pOldDepth);
		SAFE_DELETE(iter->second);
	}	

	Safe_Release_Map(m_mapRenderTarget);
	Safe_Release_Map(m_mapRenderState);
	Safe_Release_Map(m_mapDepthTarget);
	DESTROY_SINGLE(CShaderMgr);
}
