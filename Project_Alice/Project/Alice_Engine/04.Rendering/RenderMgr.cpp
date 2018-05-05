#include "RenderMgr.h"
#include "ShaderMgr.h"
#include "RasterizerState.h"
#include "BlendState.h"
#include "DepthStencilState.h"
#include "MyRenderTarget.h"
#include "MyDepthTarget.h"
#include "../Engine_Core.h"
#include "../Device.h"
#include "../03.Resource/Sampler.h"
#include "../03.Resource/ResMgr.h"
#include "../04.Rendering/Shader.h"
#include "../05.Scene/SceneMgr.h"
#include "../05.Scene/Scene.h"
#include "../06.GameObject/GameObject.h"
#include "../07.Component/Renderer.h"
#include "../07.Component/Transform.h"
#include "../07.Component/Camera.h"
#include "../07.Component/Light.h"
#include "../07.Component/ParticleSystem.h"

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

	pMRT	pGBuffer = FindMRT("GBuffer");

	for (iter = pLightList->begin(); iter != iterEnd; ++iter)
	{
		UpdateTransform();

		CLight*		pLight = (CLight*)(*iter)->FindComponentFromType(CT_LIGHT);
		pLight->SetLight();
		SAFE_RELEASE(pLight);

		m_pBlendOne->SetState();
		m_pZDisable->SetState();

		m_pLightAcc->SetShader();

		m_pPointSampler->SetSampler(11, CUT_PIXEL);
		pGBuffer->vecTarget[1]->SetTexture(12);
		pGBuffer->vecTarget[2]->SetTexture(13);
		pGBuffer->vecTarget[3]->SetTexture(14);

		UINT	iOffset = 0;
		CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		CONTEXT->IASetVertexBuffers(0, 0, NULL, 0, &iOffset);
		CONTEXT->IASetIndexBuffer(0, DXGI_FORMAT_UNKNOWN, 0);
		CONTEXT->Draw(4, 0);

		m_pBlendOne->ResetState();
		m_pZDisable->ResetState();
	}

	// 파티클 조명을 처리한다.
	list<CParticleSystem*>::iterator	iter1;
	list<CParticleSystem*>::iterator	iter1End = m_ParticleLightList.end();

	for (iter1 = m_ParticleLightList.begin(); iter1 != iter1End; ++iter1)
	{
		m_pBlendOne->SetState();

		m_pPointSampler->SetSampler(11, CUT_PIXEL);
		pGBuffer->vecTarget[1]->SetTexture(12);
		pGBuffer->vecTarget[2]->SetTexture(13);
		pGBuffer->vecTarget[3]->SetTexture(14);
		/*pShadowMap->vecTarget[0]->SetTexture(6);
		pShadowMap->vecTarget[1]->SetTexture(7);
		pShadowMap->vecTarget[2]->SetTexture(8);*/

		(*iter1)->RenderLight(_fTime);

		m_pBlendOne->ResetState();

		ID3D11ShaderResourceView*	pSRV[1] = { NULL };
		CONTEXT->PSSetShaderResources(12, 1, pSRV);
		CONTEXT->PSSetShaderResources(13, 1, pSRV);
		CONTEXT->PSSetShaderResources(14, 1, pSRV);
		/*CONTEXT->PSSetShaderResources(6, 1, pSRV);
		CONTEXT->PSSetShaderResources(7, 1, pSRV);
		CONTEXT->PSSetShaderResources(8, 1, pSRV);*/
	}

	m_ParticleLightList.clear();

	ResetMRT("LightAcc");
}

void CRenderMgr::RenderLightBlend(float _fTime)
{
	ChangeTarget("LightBlend");
	ClearTarget("LightBlend");

	CMyRenderTarget*	pAlbedo = FindTarget("Albedo");
	CMyRenderTarget*	pLightDif = FindTarget("LightAccDif");
	CMyRenderTarget*	pLightSpc = FindTarget("LightAccSpc");

	m_pZDisable->SetState();

	CONTEXT->IASetInputLayout(NULL);
	m_pLightBlend->SetShader();

	m_pPointSampler->SetSampler(11, CUT_PIXEL);
	pAlbedo->SetTexture(11);
	pLightDif->SetTexture(12);
	pLightSpc->SetTexture(13);

	UINT	iOffset = 0;
	CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	CONTEXT->IASetVertexBuffers(0, 0, NULL, 0, &iOffset);
	CONTEXT->IASetIndexBuffer(0, DXGI_FORMAT_UNKNOWN, 0);
	CONTEXT->Draw(4, 0);

	m_pZDisable->ResetState();

	ResetTarget("LightBlend");

	SAFE_RELEASE(pAlbedo);
	SAFE_RELEASE(pLightDif);
	SAFE_RELEASE(pLightSpc);
}

void CRenderMgr::RenderLightBlendTarget(float _fTime)
{
	CMyRenderTarget*	pAlbedo = FindTarget("LightBlend");

	m_pZDisable->SetState();
	m_pAlphaBlend->SetState();

	CONTEXT->IASetInputLayout(NULL);
	m_pLightBlendTarget->SetShader();

	m_pPointSampler->SetSampler(11, CUT_PIXEL);
	pAlbedo->SetTexture(11);

	UINT	iOffset = 0;
	CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	CONTEXT->IASetVertexBuffers(0, 0, NULL, 0, &iOffset);
	CONTEXT->IASetIndexBuffer(0, DXGI_FORMAT_UNKNOWN, 0);
	CONTEXT->Draw(4, 0);

	m_pAlphaBlend->ResetState();
	m_pZDisable->ResetState();

	SAFE_RELEASE(pAlbedo);
}

void CRenderMgr::UpdateTransform()
{
	TRANSFORMCBUFFER	tTransform = {};

	CScene*	pScene = GET_SINGLE(CSceneMgr)->GetCurScene();
	CCamera*	pCamera = pScene->GetMainCamera();

	tTransform.matWorld = XMMatrixIdentity();
	tTransform.matView = pCamera->GetViewMatrix();
	tTransform.matProj = pCamera->GetProjMatrix();
	tTransform.matWV = tTransform.matWorld * tTransform.matView;
	tTransform.matWVP = tTransform.matWV * tTransform.matProj;
	tTransform.matVP = tTransform.matView * tTransform.matProj;
	tTransform.matInvProj = XMMatrixInverse(&XMMatrixDeterminant(tTransform.matProj),
		tTransform.matProj);
	tTransform.matInvView = XMMatrixInverse(&XMMatrixDeterminant(tTransform.matView),
		tTransform.matView);
	tTransform.matInvVP = XMMatrixInverse(&XMMatrixDeterminant(tTransform.matVP),
		tTransform.matVP);

	SAFE_RELEASE(pCamera);

	tTransform.matWorld = XMMatrixTranspose(tTransform.matWorld);
	tTransform.matView = XMMatrixTranspose(tTransform.matView);
	tTransform.matProj = XMMatrixTranspose(tTransform.matProj);
	tTransform.matWV = XMMatrixTranspose(tTransform.matWV);
	tTransform.matWVP = XMMatrixTranspose(tTransform.matWVP);
	tTransform.matVP = XMMatrixTranspose(tTransform.matVP);
	tTransform.matInvProj = XMMatrixTranspose(tTransform.matInvProj);
	tTransform.matInvView = XMMatrixTranspose(tTransform.matInvView);
	tTransform.matInvVP = XMMatrixTranspose(tTransform.matInvVP);

	GET_SINGLE(CShaderMgr)->UpdateConstBuffer("Transform", &tTransform, CUT_VERTEX | CUT_GEOMETRY | CUT_PIXEL);
}

void CRenderMgr::AddRenderObject(CGameObject * _pGameObject)
{
	if (_pGameObject->CheckComponentFromType(CT_UI))
	{
		m_vecRender[RG_UI].push_back(_pGameObject);
	}
	else if (_pGameObject->CheckComponentFromType(CT_PARTICLE))
	{
		m_vecRender[RG_ALPHA3].push_back(_pGameObject);

		CParticleSystem*	pParticle = (CParticleSystem*)_pGameObject->FindComponentFromType(CT_PARTICLE);

		if (pParticle->GetParticleLight())
			m_ParticleLightList.push_back(pParticle);

		SAFE_RELEASE(pParticle);
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

void CRenderMgr::EraseRenderObject(CGameObject * _pGameObject)
{
	size_t Size = 0;

	if (_pGameObject->CheckComponentFromType(CT_UI))
	{
		Size = m_vecRender[RG_UI].size();
		for (size_t i = 0; i < Size; ++i)
		{
			if (_pGameObject == m_vecRender[RG_UI][i])
			{
				m_vecRender[RG_UI].erase(m_vecRender[RG_UI].begin() + i);
			}
		}
	}
	else
	{
		CRenderer*	pRenderer = (CRenderer*)_pGameObject->FindComponentFromType(CT_RENDERER);

		if (pRenderer)
		{
			if (pRenderer->BlendEnable())
			{
				Size = m_vecRender[RG_ALPHA3].size();
				for (size_t i = 0; i < Size; ++i)
				{
					if (_pGameObject == m_vecRender[RG_ALPHA3][i])
					{
						m_vecRender[RG_ALPHA3].erase(m_vecRender[RG_ALPHA3].begin() + i);
					}
				}
				//m_vecRender[RG_ALPHA3].push_back(_pGameObject);
			}
			else
			{
				Size = m_vecRender[RG_DEFAULT].size();
				for (size_t i = 0; i < Size; ++i)
				{
					if (_pGameObject == m_vecRender[RG_DEFAULT][i])
					{
						m_vecRender[RG_DEFAULT].erase(m_vecRender[RG_DEFAULT].begin() + i);
					}
				}
				//m_vecRender[RG_DEFAULT].push_back(_pGameObject);
			}

			SAFE_RELEASE(pRenderer);
		}
		else
		{
			Size = m_vecRender[RG_ALPHA3].size();
			for (size_t i = 0; i < Size; ++i)
			{
				if (_pGameObject == m_vecRender[RG_ALPHA3][i])
				{
					m_vecRender[RG_ALPHA3].erase(m_vecRender[RG_ALPHA3].begin() + i);
				}
			}
			//m_vecRender[RG_ALPHA3].push_back(_pGameObject);
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
		pMRT->pNullDepth = NULL;

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
		pMRT->pNullDepth = NULL;

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

	ID3D11DepthStencilView*	pDepthView = NULL;

	if (pMRT->pDepth)
	{
		pDepthView = pMRT->pDepth->GetDepthView();
	}
	else
	{
		CONTEXT->OMGetRenderTargets(0, NULL, &pMRT->pNullDepth);
		pDepthView = pMRT->pNullDepth;
	}	

	pMRT->vecOldTarget.clear();
	pMRT->vecOldTarget.resize(pMRT->vecTarget.size());

	CONTEXT->OMGetRenderTargets(pMRT->vecOldTarget.size(), &pMRT->vecOldTarget[0], &pMRT->pOldDepth);
	CONTEXT->OMSetRenderTargets(vecTarget.size(), &vecTarget[0], pDepthView);
}

void CRenderMgr::ResetMRT(const string & _strKey)
{
	pMRT	pMRT = FindMRT(_strKey);

	if (!pMRT)
		return;

	CONTEXT->OMSetRenderTargets(pMRT->vecOldTarget.size(), &pMRT->vecOldTarget[0], pMRT->pOldDepth);

	Safe_Release_VecList(pMRT->vecOldTarget);
	SAFE_RELEASE(pMRT->pOldDepth);
	SAFE_RELEASE(pMRT->pNullDepth);
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

	if (pMRT->pDepth)
	{
		pMRT->pDepth->ClearTarget();
	}
	else
	{
		CONTEXT->ClearDepthStencilView(pMRT->pNullDepth, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
	}
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

	m_vecRenderTargetBlend.clear();

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
	pRenderState = CreateRasterizerState("CULLING_NONE", D3D11_FILL_SOLID, D3D11_CULL_NONE);
	SAFE_RELEASE(pRenderState);

	AddRenderTargetBlendInfo();		// 디폴트 설정 : 알파블렌딩
	m_pAlphaBlend = CreateBlendState(ALPHABLEND);	

	AddRenderTargetBlendInfo(true, D3D11_BLEND_ONE, D3D11_BLEND_ONE);
	m_pBlendOne = CreateBlendState(ACC_BLEND);
	
	pRenderState = CreateDepthStencilState(DEPTH_LESS_EQUAL, true, D3D11_DEPTH_WRITE_MASK_ALL, D3D11_COMPARISON_LESS_EQUAL);
	SAFE_RELEASE(pRenderState);

	m_pZDisable = CreateDepthStencilState(DEPTH_DISABLE, false);	

	pRenderState = CreateDepthStencilState(DEPTH_WRITE_DISABLE, true, D3D11_DEPTH_WRITE_MASK_ZERO);
	SAFE_RELEASE(pRenderState);

	// Albedo Target
	CMyRenderTarget*	pTarget = CreateTarget("Albedo", _RESOLUTION.m_iWidth, _RESOLUTION.m_iHeight, DXGI_FORMAT_R8G8B8A8_UNORM);
	pTarget->OnRender(true);
	pTarget->SetScale(100.f, 100.f, 1.f);
	pTarget->SetPos(0.f, 0.f, 0.f);
	SAFE_RELEASE(pTarget);

	// Normal Target
	pTarget = CreateTarget("Normal", _RESOLUTION.m_iWidth, _RESOLUTION.m_iHeight, DXGI_FORMAT_R8G8B8A8_UNORM);
	pTarget->OnRender(true);
	pTarget->SetScale(100.f, 100.f, 1.f);
	pTarget->SetPos(0.f, 100.f, 0.f);
	SAFE_RELEASE(pTarget);

	pTarget = CreateTarget("Depth", _RESOLUTION.m_iWidth, _RESOLUTION.m_iHeight, DXGI_FORMAT_R32G32B32A32_FLOAT);
	pTarget->OnRender(true);
	pTarget->SetScale(100.f, 100.f, 1.f);
	pTarget->SetPos(0.f, 200.f, 0.f);
	SAFE_RELEASE(pTarget);

	pTarget = CreateTarget("Shininess", _RESOLUTION.m_iWidth, _RESOLUTION.m_iHeight, DXGI_FORMAT_R8G8B8A8_UNORM);
	pTarget->OnRender(true);
	pTarget->SetScale(100.f, 100.f, 1.f);
	pTarget->SetPos(0.f, 300.f, 0.f);
	SAFE_RELEASE(pTarget);

	// 깊이를 생성한다.
	CMyDepthTarget*	pDepth = CreateDepthTarget("GBufferDepth", _RESOLUTION.m_iWidth, _RESOLUTION.m_iHeight, DXGI_FORMAT_D24_UNORM_S8_UINT);
	SAFE_RELEASE(pDepth);

	// GBuffer MRT 생성
	AddMRTTarget("GBuffer", "Albedo");
	AddMRTTarget("GBuffer", "Normal");
	AddMRTTarget("GBuffer", "Depth");
	AddMRTTarget("GBuffer", "Shininess");
	//SetMRTDepth("GBuffer", "GBufferDepth");

	// 조명 Diffuse, Ambient 누적버퍼 생성
	pTarget = CreateTarget("LightAccDif", _RESOLUTION.m_iWidth, _RESOLUTION.m_iHeight, DXGI_FORMAT_R32G32B32A32_FLOAT);
	pTarget->OnRender(true);
	pTarget->SetScale(100.f, 100.f, 1.f);
	pTarget->SetPos(100.f, 0.f, 0.f);
	SAFE_RELEASE(pTarget);

	pTarget = CreateTarget("LightAccSpc", _RESOLUTION.m_iWidth, _RESOLUTION.m_iHeight, DXGI_FORMAT_R32G32B32A32_FLOAT);
	pTarget->OnRender(true);
	pTarget->SetScale(100.f, 100.f, 1.f);
	pTarget->SetPos(100.f, 100.f, 0.f);
	SAFE_RELEASE(pTarget);

	pTarget = CreateTarget("LightBlend", _RESOLUTION.m_iWidth, _RESOLUTION.m_iHeight, DXGI_FORMAT_R32G32B32A32_FLOAT);
	pTarget->OnRender(true);
	pTarget->SetScale(100.f, 100.f, 1.f);
	pTarget->SetPos(100.f, 200.f, 0.f);
	SAFE_RELEASE(pTarget);

	pDepth = CreateDepthTarget("LightAccDepth", _RESOLUTION.m_iWidth, _RESOLUTION.m_iHeight, DXGI_FORMAT_D24_UNORM_S8_UINT);
	SAFE_RELEASE(pDepth);

	AddMRTTarget("LightAcc", "LightAccDif");
	AddMRTTarget("LightAcc", "LightAccSpc");		
	SetMRTDepth("LightAcc", "LightAccDepth");

	m_pPointSampler = GET_SINGLE(CResMgr)->FindSampler("Point");

	m_pLightAcc = GET_SINGLE(CShaderMgr)->FindShader(LIGHT_ACC_SHADER);
	m_pLightBlend = GET_SINGLE(CShaderMgr)->FindShader(LIGHT_BLEND_SHADER);
	m_pLightBlendTarget = GET_SINGLE(CShaderMgr)->FindShader(LIGHT_BLEND_OUTPUT_SHADER);

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

	// 조명버퍼와 GBuffer를 합성한다
	RenderLightBlend(_fTime);

	// 백버퍼에 최종타겟을 그린다
	RenderLightBlendTarget(_fTime);

	// 알파오브젝트와 UI를 그린다
	for (int i = RG_ALPHA1; i < RG_END; ++i)
	{
		for (size_t j = 0; j < m_vecRender[i].size(); ++j)
		{
			m_vecRender[i][j]->Render(_fTime);
		}
	}

	for (int i = 0; i < RG_END; ++i)
	{
		m_vecRender[i].clear();
	}
	
	// 렌더타겟을 출력해준다.
	unordered_map<string, CMyRenderTarget*>::iterator	iter;
	unordered_map<string, CMyRenderTarget*>::iterator	iterEnd = m_mapRenderTarget.end();

	for (iter = m_mapRenderTarget.begin(); iter != iterEnd; ++iter)
	{
		iter->second->Render();
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

CRenderMgr::CRenderMgr() :
	m_pPointSampler(NULL),
	m_pBlendOne(NULL),
	m_pZDisable(NULL),
	m_pAlphaBlend(NULL),
	m_pLightAcc(NULL),
	m_pLightBlend(NULL),
	m_pLightBlendTarget(NULL)
{
	for (int i = 0; i < RG_END; ++i)
	{
		m_vecRender[i].reserve(200);
	}
}

CRenderMgr::~CRenderMgr()
{
	SAFE_RELEASE(m_pLightBlendTarget);
	SAFE_RELEASE(m_pLightBlend);
	SAFE_RELEASE(m_pLightAcc);
	SAFE_RELEASE(m_pAlphaBlend);
	SAFE_RELEASE(m_pZDisable);
	SAFE_RELEASE(m_pBlendOne);
	SAFE_RELEASE(m_pPointSampler);

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
