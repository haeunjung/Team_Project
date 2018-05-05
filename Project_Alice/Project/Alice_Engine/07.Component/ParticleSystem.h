#pragma once
#include "Component.h"

WOOJUN_BEGIN

class CShader;
class CTexture;
class CRenderState;
class DLL CParticleSystem :
	public CComponent
{
private:
	friend class CGameObject;

private:
	CParticleSystem();
	CParticleSystem(const CParticleSystem& particle);
	~CParticleSystem();

private:
	ID3D11Buffer * m_pInitVB;
	ID3D11Buffer*	m_pStreamVB;
	ID3D11Buffer*	m_pDrawVB;	
	CShader*	m_pStreamShader;
	CShader*	m_pShader;
	CShader*	m_pLightShader;
	ID3D11InputLayout*	m_pInputLayout;
	CTexture*	m_pTexture;
	CRenderState*	m_pDepthDisable;
	CRenderState*	m_pAlphaBlend;
	CRenderState*	m_pDepthWriteDisable;
	int		m_iMaxParticles;
	bool	m_bFirstRun;
	float	m_fParticleTime;
	bool	m_bLight;
	float	m_fLightRange;

public:
	bool GetParticleLight()	const;

public:
	void SetLightRange(float fRange);
	void SetShader(const string& strKey);
	void SetStreamShader(const string& strKey);
	void SetInputLayout(const string& strKey);
	void SetParticleInfo(int iMaxParticles = MAX_PARTICLES);
	void SetParticleTexture(const string& strKey, TCHAR* pFileName,
		const string& strPathKey = TEXTUREPATH);
	void SetParticleLight(bool bParticleLight);

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual void Update(float fTime);
	virtual void LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CParticleSystem* Clone();

public:
	void RenderLight(float fTime);
};

WOOJUN_END
