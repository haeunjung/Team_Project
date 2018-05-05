#include "ParticleSystem.h"
#include "Transform.h"
#include "Camera.h"
#include "PointLight.h"
#include "../Device.h"
#include "../03.Resource/Texture.h"
#include "../03.Resource/ResMgr.h"
#include "../04.Rendering/Shader.h"
#include "../04.Rendering/ShaderMgr.h"
#include "../04.Rendering/RenderMgr.h"
#include "../04.Rendering/RenderState.h"
#include "../05.Scene/Scene.h"

WOOJUN_USING

CParticleSystem::CParticleSystem() :
	m_pInitVB(NULL),
	m_pStreamVB(NULL),
	m_pDrawVB(NULL),
	m_pShader(NULL),
	m_pStreamShader(NULL),
	m_pTexture(NULL),
	m_pInputLayout(NULL),
	m_pAlphaBlend(NULL),
	m_pLightShader(NULL),
	m_pDepthWriteDisable(NULL),
	m_iMaxParticles(0),
	m_fParticleTime(0.f),
	m_fLightRange(1.5f),
	m_bFirstRun(true),
	m_bLight(false)
{
	SetTag("ParticleSystem");
	SetTypeName("CParticleSystem");
	SetTypeID<CParticleSystem>();
	m_eComponentType = CT_PARTICLE;

	SetShader(PARTICLE_SHADER);
	SetStreamShader(PARTICLE_STREAMOUT_SHADER);
	SetInputLayout("Particle");

	m_pLightShader = GET_SINGLE(CShaderMgr)->FindShader(PARTICLE_LIGHT_SHADER);

	m_pDepthDisable = GET_SINGLE(CRenderMgr)->FindRenderState(DEPTH_DISABLE);
	m_pAlphaBlend = GET_SINGLE(CRenderMgr)->FindRenderState(ALPHABLEND);
}

CParticleSystem::CParticleSystem(const CParticleSystem & particle) :
	CComponent(particle)
{
	*this = particle;
	m_fParticleTime = 0.f;
	m_bFirstRun = true;
	SetParticleInfo(particle.m_iMaxParticles);

	m_pShader = particle.m_pShader;

	if (m_pShader)
		m_pShader->AddRef();

	if (m_pStreamShader)
		m_pStreamShader->AddRef();

	if (m_pTexture)
		m_pTexture->AddRef();

	if (m_pDepthDisable)
		m_pDepthDisable->AddRef();

	if (m_pAlphaBlend)
		m_pAlphaBlend->AddRef();

	if (m_pLightShader)
		m_pLightShader->AddRef();

	if (m_pDepthWriteDisable)
		m_pDepthWriteDisable->AddRef();

	m_pInputLayout = particle.m_pInputLayout;
}

CParticleSystem::~CParticleSystem()
{
	SAFE_RELEASE(m_pDepthWriteDisable);
	SAFE_RELEASE(m_pLightShader);
	SAFE_RELEASE(m_pAlphaBlend);
	SAFE_RELEASE(m_pDepthDisable);
	SAFE_RELEASE(m_pTexture);
	SAFE_RELEASE(m_pStreamShader);
	SAFE_RELEASE(m_pShader);

	SAFE_RELEASE(m_pInitVB);
	SAFE_RELEASE(m_pStreamVB);
	SAFE_RELEASE(m_pDrawVB);
}

bool CParticleSystem::GetParticleLight() const
{
	return m_bLight;
}

void CParticleSystem::SetLightRange(float fRange)
{
	m_fLightRange = fRange;
}

void CParticleSystem::SetShader(const string & strKey)
{
	SAFE_RELEASE(m_pShader);
	m_pShader = GET_SINGLE(CShaderMgr)->FindShader(strKey);
}

void CParticleSystem::SetStreamShader(const string & strKey)
{
	SAFE_RELEASE(m_pStreamShader);
	m_pStreamShader = GET_SINGLE(CShaderMgr)->FindShader(strKey);

	//m_pInputLayout = GET_SINGLE(CShaderManager)->FindInputLayout("Particle");
}

void CParticleSystem::SetInputLayout(const string & strKey)
{
	m_pInputLayout = GET_SINGLE(CShaderMgr)->FindInputLayout(strKey);
}

void CParticleSystem::SetParticleInfo(int iMaxParticles)
{
	// 
	m_iMaxParticles = iMaxParticles;

	D3D11_BUFFER_DESC	tDesc = {};

	tDesc.Usage = D3D11_USAGE_DEFAULT;
	tDesc.ByteWidth = sizeof(VERTEXPARTICLE);
	tDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	VERTEXPARTICLE	vtx = {};
	vtx.fCreateTime = 0.0f;
	vtx.fLifeTime = 1.5f;
	vtx.vSize = DxVector2(0.5f, 0.5f);
	vtx.fLightRange = 1.5f;

	D3D11_SUBRESOURCE_DATA	tData = {};
	tData.pSysMem = &vtx;

	if (FAILED(DEVICE->CreateBuffer(&tDesc, &tData, &m_pInitVB)))
		return;

	tDesc.ByteWidth = sizeof(VERTEXPARTICLE) * iMaxParticles;
	tDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER | D3D11_BIND_STREAM_OUTPUT;

	if (FAILED(DEVICE->CreateBuffer(&tDesc, NULL, &m_pStreamVB)))
		return;

	if (FAILED(DEVICE->CreateBuffer(&tDesc, NULL, &m_pDrawVB)))
		return;
}

void CParticleSystem::SetParticleTexture(const string & strKey, TCHAR * pFileName, const string & strPathKey)
{
	SAFE_RELEASE(m_pTexture);
	m_pTexture = GET_SINGLE(CResMgr)->LoadTexture(strKey, pFileName, strPathKey);
}

void CParticleSystem::SetParticleLight(bool bParticleLight)
{
	m_bLight = bParticleLight;

	SAFE_RELEASE(m_pDepthWriteDisable);
	if (bParticleLight)
	{
		m_pDepthWriteDisable = GET_SINGLE(CRenderMgr)->FindRenderState(DEPTH_WRITE_DISABLE);
	}
}

bool CParticleSystem::Init()
{
	m_bFirstRun = true;

	return true;
}

void CParticleSystem::Input(float fTime)
{
}

void CParticleSystem::Update(float fTime)
{
}

void CParticleSystem::LateUpdate(float fTime)
{
}

void CParticleSystem::Collision(float fTime)
{
}

void CParticleSystem::Render(float fTime)
{
	PARTICLECBUFFER	tCBuffer = {};

	tCBuffer.vPos = m_pTransform->GetWorldPos();
	tCBuffer.fDeltaTime = fTime;

	CCamera*	pCamera = m_pScene->GetMainCamera();
	CTransform*	pCameraTr = pCamera->GetTransform();

	tCBuffer.vCamAxisY = pCamera->GetAxis(AXIS_Y);
	tCBuffer.vCamAxisX = pCamera->GetAxis(AXIS_X) * -1.f;
	tCBuffer.fCreateTime = 0.1f;
	tCBuffer.fSpeed = 8.f;
	tCBuffer.fGameTime = m_fParticleTime;
	tCBuffer.vCreateDir = DxVector3((rand() % 10001 - 5000) / 5000.f,
		(rand() % 5001) / 5000.f,
		(rand() % 10001 - 5000) / 5000.f).Normalize();

	TRANSFORMCBUFFER	tTransform;

	tTransform.matVP = pCamera->GetViewMatrix() * pCamera->GetProjMatrix();

	tTransform.matVP = XMMatrixTranspose(tTransform.matVP);

	GET_SINGLE(CShaderMgr)->UpdateConstBuffer("Transform", &tTransform, CUT_GEOMETRY);

	SAFE_RELEASE(pCameraTr);
	SAFE_RELEASE(pCamera);

	GET_SINGLE(CShaderMgr)->UpdateConstBuffer("ParticleCBuffer", &tCBuffer, CUT_GEOMETRY);

	CONTEXT->IASetInputLayout(m_pInputLayout);
	CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	UINT	iStride = sizeof(VERTEXPARTICLE);
	UINT	iOffset = 0;

	if (!m_bLight)
	{
		m_fParticleTime += fTime;

		// 스트림 버퍼에 출력한다.(셰이더에서 새로운 파티클들을 생성시켜준다.)
		// 스트림 버퍼 출력시에는 깊이를 끈다.
		m_pDepthDisable->SetState();

		if (m_bFirstRun)
			CONTEXT->IASetVertexBuffers(0, 1, &m_pInitVB, &iStride, &iOffset);

		else
			CONTEXT->IASetVertexBuffers(0, 1, &m_pDrawVB, &iStride, &iOffset);

		m_pStreamShader->SetShader();

		// Stream Buffer를 지정한다.
		CONTEXT->SOSetTargets(1, &m_pStreamVB, &iOffset);

		if (m_bFirstRun)
		{
			CONTEXT->Draw(1, 0);
			m_bFirstRun = false;
		}

		else
			CONTEXT->DrawAuto();

		ID3D11Buffer*	pBuf = NULL;

		CONTEXT->SOSetTargets(1, &pBuf, &iOffset);

		m_pDepthDisable->ResetState();

		// 버퍼 ping-pong
		swap(m_pDrawVB, m_pStreamVB);
	}

	// 실제 출력한다.
	/*GET_SINGLE(CShaderManager)->UpdateConstantBuffer("Transform", &tTransform, CUT_GEOMETRY);
	GET_SINGLE(CShaderManager)->UpdateConstantBuffer("ParticleCBuffer",
	&tCBuffer, CUT_GEOMETRY);*/

	m_pShader->SetShader();

	m_pAlphaBlend->SetState();

	if (m_pTexture)
		m_pTexture->SetTexture(0, CUT_PIXEL);

	CONTEXT->IASetInputLayout(m_pInputLayout);
	CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	CONTEXT->IASetVertexBuffers(0, 1, &m_pDrawVB, &iStride, &iOffset);

	CONTEXT->DrawAuto();

	m_pAlphaBlend->ResetState();
}

CParticleSystem * CParticleSystem::Clone()
{
	return new CParticleSystem(*this);
}

void CParticleSystem::RenderLight(float fTime)
{
	PARTICLECBUFFER	tCBuffer = {};

	tCBuffer.vPos = m_pTransform->GetWorldPos();
	tCBuffer.fDeltaTime = fTime;

	CCamera*	pCamera = m_pScene->GetMainCamera();
	CTransform*	pCameraTr = pCamera->GetTransform();

	tCBuffer.vCamAxisY = pCamera->GetAxis(AXIS_Y);
	tCBuffer.vCamAxisX = pCamera->GetAxis(AXIS_X) * -1.f;
	tCBuffer.fCreateTime = 0.1f;
	tCBuffer.fSpeed = 8.f;
	tCBuffer.fGameTime = m_fParticleTime;
	tCBuffer.vCreateDir = DxVector3((rand() % 10001 - 5000) / 5000.f,
		(rand() % 5001) / 5000.f,
		(rand() % 10001 - 5000) / 5000.f).Normalize();

	TRANSFORMCBUFFER	tTransform;

	tTransform.matView = pCamera->GetViewMatrix();
	tTransform.matVP = pCamera->GetViewMatrix() * pCamera->GetProjMatrix();
	tTransform.matProj = pCamera->GetProjMatrix();
	tTransform.matInvProj = XMMatrixInverse(&XMMatrixDeterminant(tTransform.matProj),
		tTransform.matProj);

	tTransform.matView = XMMatrixTranspose(tTransform.matView);
	tTransform.matProj = XMMatrixTranspose(tTransform.matProj);
	tTransform.matInvProj = XMMatrixTranspose(tTransform.matInvProj);
	tTransform.matVP = XMMatrixTranspose(tTransform.matVP);

	GET_SINGLE(CShaderMgr)->UpdateConstBuffer("Transform", &tTransform, CUT_GEOMETRY);

	SAFE_RELEASE(pCameraTr);
	SAFE_RELEASE(pCamera);

	GET_SINGLE(CShaderMgr)->UpdateConstBuffer("ParticleCBuffer", &tCBuffer, CUT_GEOMETRY);

	CONTEXT->IASetInputLayout(m_pInputLayout);
	CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	UINT	iStride = sizeof(VERTEXPARTICLE);
	UINT	iOffset = 0;

	m_fParticleTime += fTime;

	// 스트림 버퍼에 출력한다.(셰이더에서 새로운 파티클들을 생성시켜준다.)
	// 스트림 버퍼 출력시에는 깊이를 끈다.
	m_pDepthDisable->SetState();

	if (m_bFirstRun)
		CONTEXT->IASetVertexBuffers(0, 1, &m_pInitVB, &iStride, &iOffset);

	else
		CONTEXT->IASetVertexBuffers(0, 1, &m_pDrawVB, &iStride, &iOffset);

	m_pStreamShader->SetShader();

	// Stream Buffer를 지정한다.
	CONTEXT->SOSetTargets(1, &m_pStreamVB, &iOffset);

	if (m_bFirstRun)
	{
		CONTEXT->Draw(1, 0);
		m_bFirstRun = false;
	}

	else
		CONTEXT->DrawAuto();

	m_pDepthDisable->ResetState();

	ID3D11Buffer*	pBuf = NULL;

	CONTEXT->SOSetTargets(1, &pBuf, &iOffset);

	// 버퍼 ping-pong
	swap(m_pDrawVB, m_pStreamVB);

	m_pLightShader->SetShader();

	m_pDepthWriteDisable->SetState();

	CONTEXT->IASetInputLayout(m_pInputLayout);
	CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	CONTEXT->IASetVertexBuffers(0, 1, &m_pDrawVB, &iStride, &iOffset);

	CONTEXT->DrawAuto();

	m_pDepthWriteDisable->ResetState();
}
