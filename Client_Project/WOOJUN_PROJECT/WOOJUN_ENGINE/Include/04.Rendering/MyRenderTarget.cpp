#include "MyRenderTarget.h"
#include "ShaderMgr.h"
#include "RenderMgr.h"
#include "Shader.h"
#include "RenderState.h"
#include "../Device.h"
#include "../03.Resource/Texture.h"
#include "../03.Resource/Mesh.h"
#include "../03.Resource/ResMgr.h"
#include "../03.Resource/Sampler.h"
#include "../05.Scene/SceneMgr.h"
#include "../05.Scene/Scene.h"
#include "../07.Component/Camera.h"

WOOJUN_USING

CMyRenderTarget::CMyRenderTarget() :
	m_pTargetTexture(NULL),
	m_pTargetView(NULL),
	m_pOldView(NULL),
	m_pOldDepth(NULL)
{
	memset(m_fClearColor, 0, sizeof(float) * 4);
#ifdef _DEBUG
	m_bRender = false;
	m_pMesh = GET_SINGLE(CResMgr)->FindMesh("UIMesh");
	m_pShader = GET_SINGLE(CShaderMgr)->FindShader(UI_SHADER);
	m_pZDisable = GET_SINGLE(CRenderMgr)->FindRenderState(DEPTH_DISABLE);
	m_pSampler = GET_SINGLE(CResMgr)->FindSampler("Linear");
	m_tMaterial.vDiffuse = DxVector4(1.f, 1.f, 1.f, 1.f);
#endif
}

CMyRenderTarget::~CMyRenderTarget()
{
#ifdef _DEBUG
	SAFE_RELEASE(m_pSampler);
	SAFE_RELEASE(m_pMesh);
	SAFE_RELEASE(m_pShader);
	SAFE_RELEASE(m_pZDisable);
#endif 
	SAFE_RELEASE(m_pOldDepth);
	SAFE_RELEASE(m_pOldView);
	SAFE_RELEASE(m_pTargetView);
	SAFE_RELEASE(m_pTargetTexture);
}

ID3D11RenderTargetView * CMyRenderTarget::GetTargetView() const
{
	return m_pTargetView;
}

void CMyRenderTarget::OnRender(bool _bRender)
{
	m_bRender = _bRender;
}

void CMyRenderTarget::SetPos(float _x, float _y, float _z)
{
#ifdef _DEBUG
	m_vPos = DxVector3(_x, _y, _z);
#endif 
}

void CMyRenderTarget::SetScale(float _x, float _y, float _z)
{
#ifdef _DEBUG
	m_vScale = DxVector3(_x, _y, _z);
#endif 
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

void CMyRenderTarget::SetTexture(int _iRegister)
{
	m_pTargetTexture->SetTexture(_iRegister, CUT_PIXEL);
}

void CMyRenderTarget::Render()
{
#ifdef _DEBUG
	if (m_bRender)
	{
		TRANSFORMCBUFFER	tTransform = {};

		CScene*		pScene = GET_SINGLE(CSceneMgr)->GetCurScene();
		CCamera*	pCamera = pScene->FindCamera("UICamera");

		MATRIX	matScale, matTrans, matWorld;

		matScale = XMMatrixScaling(m_vScale.x, m_vScale.y, m_vScale.z);
		matTrans = XMMatrixTranslation(m_vPos.x, m_vPos.y, m_vPos.z);
		matWorld = matScale * matTrans;

		tTransform.matWorld = matWorld.mat;
		tTransform.matView = XMMatrixIdentity();
		tTransform.matProj = pCamera->GetProjMatrix();
		tTransform.matWV = tTransform.matWorld * tTransform.matView;
		tTransform.matWVP = tTransform.matWV * tTransform.matProj;
		tTransform.matVP = tTransform.matView * tTransform.matProj;
		tTransform.vPivot = Vec3Zero;
		tTransform.vMeshSize = m_pMesh->GetMeshSize();
		tTransform.vMeshMin = m_pMesh->GetMeshMin();
		tTransform.vMeshMax = m_pMesh->GetMeshMax();
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

		m_pZDisable->SetState();
		m_pShader->SetShader();

		GET_SINGLE(CShaderMgr)->UpdateConstBuffer("Material", &m_tMaterial, CUT_VERTEX | CUT_GEOMETRY | CUT_PIXEL);

		GET_SINGLE(CShaderMgr)->SetInputLayout("TexInputLayout");

		m_pSampler->SetSampler(0, CUT_PIXEL);
		m_pTargetTexture->SetTexture(0, CUT_PIXEL);

		m_pMesh->Render();
		m_pZDisable->ResetState();
	}
#endif // _DEBUG

}
