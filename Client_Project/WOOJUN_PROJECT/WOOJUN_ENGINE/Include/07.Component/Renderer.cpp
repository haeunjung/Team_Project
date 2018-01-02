#include "Renderer.h"
#include "../04.Rendering/ShaderMgr.h"
#include "../04.Rendering/Shader.h"
#include "../03.Resource/ResMgr.h"
#include "../03.Resource/Mesh.h"
#include "../Device.h"
#include "../Engine_Core.h"
#include "../05.Scene/Scene.h"
#include "../06.GameObject/GameObject.h"
#include "Transform.h"
#include "Camera.h"
#include "../04.Rendering/RenderState.h"
#include "../04.Rendering/RenderMgr.h"
#include "Material.h"
#include "Light.h"
#include "Effect.h"
#include "Animation2D.h"

WOOJUN_USING

void CRenderer::UpdateTransform()
{
	TRANSFORMCBUFFER	tTransform = {};

	CCamera*	pCamera = m_pScene->GetMainCamera();

	tTransform.matWorld = m_pTransform->GetLocalMatrix().mat * m_pTransform->GetWorldMatrix().mat;
	tTransform.matView = pCamera->GetViewMatrix();
	tTransform.matProj = pCamera->GetProjMatrix();
	tTransform.matWV = tTransform.matWorld * tTransform.matView;
	tTransform.matWVP = tTransform.matWV * tTransform.matProj;
	tTransform.matVP = tTransform.matView * tTransform.matProj;

	SAFE_RELEASE(pCamera);

	// 셰이더는 세로로 읽기 때문에
	// 전치행렬로 바꿔줘야 한다
	// XMMatrixTranspose
	tTransform.matWorld = XMMatrixTranspose(tTransform.matWorld);
	tTransform.matView = XMMatrixTranspose(tTransform.matView);
	tTransform.matProj = XMMatrixTranspose(tTransform.matProj);
	tTransform.matWV = XMMatrixTranspose(tTransform.matWV);
	tTransform.matWVP = XMMatrixTranspose(tTransform.matWVP);
	tTransform.matVP = XMMatrixTranspose(tTransform.matVP);

	GET_SINGLE(CShaderMgr)->UpdateConstBuffer("Transform", &tTransform, CUT_VERTEX | CUT_PIXEL | CUT_GEOMETRY);	
}

CMesh * CRenderer::GetMesh() const
{
	m_pMesh->AddRef();
	return m_pMesh;
}

void CRenderer::SetMesh(const string & _strKey)
{
	SAFE_RELEASE(m_pMesh);
	m_pMesh = GET_SINGLE(CResMgr)->FindMesh(_strKey);

	if (NULL == m_pMesh)
	{
		assert(m_pMesh);		
	}
}

void CRenderer::SetMesh(const string & _strKey, const TCHAR * _pFileName, const string & _strPathKey)
{
	SAFE_RELEASE(m_pMesh);
	m_pMesh = GET_SINGLE(CResMgr)->LoadMesh(_strKey, _pFileName, _strPathKey);

	size_t size = m_vecMaterial.size();
	for (size_t i = 0; i < size; ++i)
	{
		Safe_Release_VecList(m_vecMaterial[i]);
	}

	m_vecMaterial.clear();

	size_t ContainerCount = m_pMesh->GetContainerCount();
	for (size_t i = 0; i < ContainerCount; ++i)
	{
		if (i == m_vecMaterial.size())
		{
			AddContainerMaterial();
		}

		size_t SubsetCount = m_pMesh->GetSubsetCount(i);
		for (size_t j = 0; j < SubsetCount; ++j)
		{
			CMaterial*	pMaterial = m_pMesh->CloneMaterial(i, j);

			m_vecMaterial[i].push_back(pMaterial);
		}		
	}
}

void CRenderer::SetShader(const string & _strKey)
{
	SAFE_RELEASE(m_pShader);
	m_pShader = GET_SINGLE(CShaderMgr)->FindShader(_strKey);

	if (NULL == m_pShader)
	{
		assert(m_pShader);
		return;
	}
}

void CRenderer::SetInputLayout(const string & _strKey)
{
	m_pInputLayout = GET_SINGLE(CShaderMgr)->FindInputLayout(_strKey);

	if (NULL == m_pShader)
	{
		assert(m_pInputLayout);
		return;
	}
}

void CRenderer::SetRenderState(const string & _strKey)
{
	CRenderState*	pRenderState = GET_SINGLE(CRenderMgr)->FindRenderState(_strKey);

	if (NULL == pRenderState)
	{
		return;
	}

	m_pRenderState[pRenderState->GetType()] = pRenderState;
}

CMaterial * CRenderer::GetMaterial(int _iContainer, int _iSubSet)
{
	if (_iContainer >= m_vecMaterial.size())
	{
		return NULL;
	}
	else if (_iSubSet >= m_vecMaterial[_iContainer].size())
	{
		return NULL;
	}

	m_vecMaterial[_iContainer][_iSubSet]->AddRef();

	return m_vecMaterial[_iContainer][_iSubSet];
}

CMaterial * CRenderer::CreateMaterial(const string & _strSamplerKey, const string & _strTextureKey,
	int _iSamplerRegister /*= 0*/, int _iTextureRegister /*= 0*/,
	TCHAR* _pFileName /*= NULL*/, const string& _strPathKey /*= TEXTUREPATH*/,
	int _iContainer /*= 0*/)
{
	if (m_vecMaterial.empty())
	{
		vector<CMaterial*>	vecMaterial;
		m_vecMaterial.push_back(vecMaterial);
	}
	else if (m_vecMaterial.size() <= _iContainer)
	{
		return NULL;
	}

	CMaterial*	pMaterial = new CMaterial();

	if (false == pMaterial->Init())
	{
		SAFE_RELEASE(pMaterial);
		return NULL;
	}

	pMaterial->SetDiffuseTexture(_strSamplerKey, _strTextureKey, _pFileName, _strPathKey);
	pMaterial->SetDiffuseRegister(_iSamplerRegister, _iTextureRegister);

	pMaterial->AddRef();
	m_vecMaterial[_iContainer].push_back(pMaterial);

	return pMaterial;
}

void CRenderer::AddMaterial(CMaterial * _pMaterial, int _iContainer)
{
	if (true == m_vecMaterial.empty())
	{
		vector<CMaterial*>	vecMaterial;
		m_vecMaterial.push_back(vecMaterial);
	}
	else if (m_vecMaterial.size() <= _iContainer)
	{
		return;
	}

	_pMaterial->AddRef();
	m_vecMaterial[_iContainer].push_back(_pMaterial);
}

void CRenderer::AddContainerMaterial()
{
	vector<CMaterial*>	vecMaterial;
	m_vecMaterial.push_back(vecMaterial);
}

bool CRenderer::Init()
{
	CMaterial*	pMaterial = CreateMaterial("Linear", "");

	SAFE_RELEASE(pMaterial);

	return true;
}

void CRenderer::Input(float _fTime)
{
}

void CRenderer::Update(float _fTime)
{
}

void CRenderer::LateUpdate(float _fTime)
{
}

void CRenderer::Collision(float _fTime)
{
}

void CRenderer::Render(float _fTime)
{
	UpdateTransform();

	for (int i = 0; i < RST_END; ++i)
	{
		if (NULL != m_pRenderState[i])
		{
			m_pRenderState[i]->SetState();
		}
	}

	CONTEXT->IASetInputLayout(m_pInputLayout);
	m_pShader->SetShader();

	// 조명 설정
	const list<CGameObject*>* pLightList = m_pScene->GetLightList();

	if (false == pLightList->empty())
	{
		list<CGameObject*>::const_iterator iter = pLightList->begin();

		CLight*	pLight = (CLight*)(*iter)->FindComponentFromType(CT_LIGHT);

		pLight->SetLight();

		SAFE_RELEASE(pLight);
	}

	// 이펙트가 있다면 이펙트 상수버퍼 넘긴다
	CEffect*	pEffect = m_pGameObject->FindComponentFromTypeID<CEffect>();
	if (NULL != pEffect)
	{		
		pEffect->SetEffectCBuffer();
		SAFE_RELEASE(pEffect);
	}

	// 애니메이션
	CAnimation2D*	pAnimation2D = m_pGameObject->FindComponentFromTypeID<CAnimation2D>();
	if (NULL != pAnimation2D)
	{
		pAnimation2D->SetShader();
		SAFE_RELEASE(pAnimation2D);
	}

	// 재질 설정
	size_t ContainerCount = m_vecMaterial.size();
	for (size_t i = 0; i < ContainerCount; ++i)
	{
		size_t SubsetCount = m_vecMaterial[i].size();
		for (size_t j = 0; j < SubsetCount; ++j)
		{
			m_vecMaterial[i][j]->SetMaterial();

			m_pMesh->Render(i, j);
		}
	}
	
	for (int i = 0; i < RST_END; ++i)
	{
		if (NULL != m_pRenderState[i])
		{
			m_pRenderState[i]->ResetState();
		}
	}
}

CRenderer * CRenderer::Clone()
{
	return new CRenderer(*this);
}

CRenderer::CRenderer() :
	m_pMesh(NULL),
	m_pShader(NULL),
	m_pInputLayout(NULL)
{
	SetTypeName("CRenderer");
	SetTypeID<CRenderer>();
	SetTag("Renderer");
	m_eComponentType = CT_RENDERER;

	for (int i = 0; i < RST_END; ++i)
	{
		m_pRenderState[i] = NULL;
	}
}

CRenderer::CRenderer(const CRenderer & _Renderer)
{
	*this = _Renderer;

	if (NULL != m_pMesh)
	{
		m_pMesh->AddRef();
	}

	if (NULL != m_pShader)
	{
		m_pShader->AddRef();
	}

	for (int i = 0; i < RST_END; ++i)
	{
		if (NULL != m_pRenderState[i])
		{
			m_pRenderState[i]->AddRef();
		}
	}

	m_vecMaterial.clear();
	for (size_t i = 0; i < _Renderer.m_vecMaterial.size(); ++i)
	{
		vector<CMaterial*>	vecMaterial;
		m_vecMaterial.push_back(vecMaterial);
		for (size_t j = 0; j < _Renderer.m_vecMaterial[i].size(); ++j)
		{
			CMaterial*	pMaterial = _Renderer.m_vecMaterial[i][j]->Clone();

			m_vecMaterial[i].push_back(pMaterial);
		}
	}
}

CRenderer::~CRenderer()
{
	for (int i = 0; i < RST_END; ++i)
	{
		SAFE_RELEASE(m_pRenderState[i]);
	}

	SAFE_RELEASE(m_pShader);
	SAFE_RELEASE(m_pMesh);

	size_t size = m_vecMaterial.size();
	for (size_t i = 0; i < size; ++i)
	{
		Safe_Release_VecList(m_vecMaterial[i]);
	}
}