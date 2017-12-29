#include "Material.h"
#include "../03.Resource/Texture.h"
#include "../03.Resource/Sampler.h"
#include "../03.Resource/ResMgr.h"
#include "../Device.h"
#include "../04.Rendering/ShaderMgr.h"

WOOJUN_USING

void CMaterial::SetMaterialInfo(const MATERIALINFO & _tInfo)
{
	m_tInfo = _tInfo;
}

void CMaterial::SetMaterialInfo(const DxVector4 & _vDiffuse, const DxVector4 & _vAmbient, const DxVector4 & _vSpecular, const DxVector4 & _vEmissive, float _fSpecularPower)
{
	m_tInfo.vDiffuse = _vDiffuse;
	m_tInfo.vAmbient = _vAmbient;
	m_tInfo.vSpecular = _vSpecular;
	m_tInfo.vEmissive = _vEmissive;
	m_tInfo.fSpecularPower = _fSpecularPower;
}

void CMaterial::SetDiffuseColor(const DxVector4 & _vDiffuse)
{
	m_tInfo.vDiffuse = _vDiffuse;
}

bool CMaterial::SetDiffuseTexture(const string & _strSamplerKey, const string & _strTextureKey, TCHAR* _pFileName /*= NULL*/, const string& _strPathKey /*= TEXTUREPATH*/)
{
	if (NULL == m_pDiffuse)
	{
		m_pDiffuse = new TEXTURE();
	}

	SAFE_RELEASE(m_pDiffuse->pTexture);
	SAFE_RELEASE(m_pDiffuse->pSampler);

	if (NULL != _pFileName)
	{
		m_pDiffuse->pTexture = GET_SINGLE(CResMgr)->LoadTexture(_strTextureKey, _pFileName, _strPathKey);
	}
	else
	{
		m_pDiffuse->pTexture = GET_SINGLE(CResMgr)->FindTexture(_strTextureKey);
	}

	m_pDiffuse->pSampler = GET_SINGLE(CResMgr)->FindSampler(_strSamplerKey);

	return true;
}

bool CMaterial::SetDiffuseTexture(CTexture * _pTexture, int _iTexRegister)
{
	if (NULL == m_pDiffuse)
	{
		m_pDiffuse = new TEXTURE();
	}

	SAFE_RELEASE(m_pDiffuse->pTexture);

	m_pDiffuse->pTexture = _pTexture;
	_pTexture->AddRef();
	m_pDiffuse->iTextureRegister = _iTexRegister;

	return true;
}

bool CMaterial::SetDiffuseTextureFromFullPath(const string & _strSamplerKey, const string & _strKey, const char * _pFullPath)
{
	if (NULL == m_pDiffuse)
	{
		m_pDiffuse = new TEXTURE();
	}

	SAFE_RELEASE(m_pDiffuse->pTexture);
	SAFE_RELEASE(m_pDiffuse->pSampler);

	m_pDiffuse->pTexture = GET_SINGLE(CResMgr)->LoadTextureFromFullPath(_strKey, _pFullPath);
	m_pDiffuse->pSampler = GET_SINGLE(CResMgr)->FindSampler(_strSamplerKey);

	return true;
}

void CMaterial::SetDiffuseRegister(int _iSamplerRegister /*= 0*/, int _iTextureRegister /*= 0*/)
{
	if (NULL == m_pDiffuse)
	{
		m_pDiffuse = new TEXTURE();
	}

	m_pDiffuse->iTextureRegister = _iTextureRegister;
	m_pDiffuse->iSamplerRegister = _iSamplerRegister;
}

void CMaterial::SetDiffuseShaderConstantType(int _iShaderConstantType)
{
	if (NULL == m_pDiffuse)
	{
		m_pDiffuse = new TEXTURE();
	}

	m_pDiffuse->iShaderConstantType = _iShaderConstantType;
}

bool CMaterial::SetNormalMapTexture(const string & _strSamplerKey, const string & _strTextureKey, TCHAR * _pFileName, const string & _strPathKey)
{
	if (NULL == m_pNormal)
	{
		m_pNormal = new TEXTURE();
	}

	SAFE_RELEASE(m_pNormal->pTexture);
	SAFE_RELEASE(m_pNormal->pSampler);

	if (NULL != _pFileName)
	{
		m_pNormal->pTexture = GET_SINGLE(CResMgr)->LoadTexture(_strTextureKey, _pFileName, _strPathKey);
	}
	else
	{
		m_pNormal->pTexture = GET_SINGLE(CResMgr)->FindTexture(_strTextureKey);
	}

	m_pNormal->pSampler = GET_SINGLE(CResMgr)->FindSampler(_strSamplerKey);

	m_tInfo.iBump = 1;

	return true;
}

bool CMaterial::SetNormalMapTexture(CTexture * _pTexture, int _iTexRegister)
{
	if (NULL == m_pNormal)
	{
		m_pNormal = new TEXTURE();
	}

	SAFE_RELEASE(m_pNormal->pTexture);

	m_pNormal->pTexture = _pTexture;
	_pTexture->AddRef();
	m_pNormal->iTextureRegister = _iTexRegister;

	m_tInfo.iBump = 1;

	return true;
}

bool CMaterial::SetNormalMapTextureFromFullPath(const string & _strSamplerKey, const string & _strKey, const char * _pFullPath)
{
	if (NULL == m_pNormal)
	{
		m_pNormal = new TEXTURE();
	}

	SAFE_RELEASE(m_pNormal->pTexture);
	SAFE_RELEASE(m_pNormal->pSampler);

	m_pNormal->pTexture = GET_SINGLE(CResMgr)->LoadTextureFromFullPath(_strKey, _pFullPath);
	m_pNormal->pSampler = GET_SINGLE(CResMgr)->FindSampler(_strSamplerKey);

	m_tInfo.iBump = 1;

	return true;
}

void CMaterial::SetNormalMapRegister(int _iSamplerRegister, int _iTextureRegister)
{
	if (NULL == m_pNormal)
	{
		m_pNormal = new TEXTURE();
	}

	m_pNormal->iTextureRegister = _iTextureRegister;
	m_pNormal->iSamplerRegister = _iSamplerRegister;
}

void CMaterial::SetNormalMapShaderConstantType(int _iShaderConstantType)
{
	if (NULL == m_pNormal)
	{
		m_pNormal = new TEXTURE();
	}

	m_pNormal->iShaderConstantType = _iShaderConstantType;
}

bool CMaterial::SetSpecularTexture(const string & _strSamplerKey, const string & _strTextureKey, TCHAR * _pFileName, const string & _strPathKey)
{
	if (NULL == m_pSpecular)
	{
		m_pSpecular = new TEXTURE();
	}

	SAFE_RELEASE(m_pSpecular->pTexture);
	SAFE_RELEASE(m_pSpecular->pSampler);

	if (NULL != _pFileName)
	{
		m_pSpecular->pTexture = GET_SINGLE(CResMgr)->LoadTexture(_strTextureKey, _pFileName, _strPathKey);
	}
	else
	{
		m_pSpecular->pTexture = GET_SINGLE(CResMgr)->FindTexture(_strTextureKey);
	}

	m_pSpecular->pSampler = GET_SINGLE(CResMgr)->FindSampler(_strSamplerKey);

	m_tInfo.iSpecular = 1;

	return true;
}

bool CMaterial::SetSpecularTexture(CTexture * _pTexture, int _iTexRegister)
{
	if (NULL == m_pSpecular)
	{
		m_pSpecular = new TEXTURE();
	}

	SAFE_RELEASE(m_pSpecular->pTexture);

	m_pSpecular->pTexture = _pTexture;
	_pTexture->AddRef();
	m_pSpecular->iTextureRegister = _iTexRegister;

	m_tInfo.iSpecular = 1;

	return true;
}

bool CMaterial::SetSpecularTextureFromFullPath(const string & _strSamplerKey, const string & _strKey, const char * _pFullPath)
{
	if (NULL == m_pSpecular)
	{
		m_pSpecular = new TEXTURE();
	}

	SAFE_RELEASE(m_pSpecular->pTexture);
	SAFE_RELEASE(m_pSpecular->pSampler);

	m_pSpecular->pTexture = GET_SINGLE(CResMgr)->LoadTextureFromFullPath(_strKey, _pFullPath);
	m_pSpecular->pSampler = GET_SINGLE(CResMgr)->FindSampler(_strSamplerKey);

	m_tInfo.iSpecular = 1;

	return true;
}

void CMaterial::SetSpecularRegister(int _iSamplerRegister, int _iTextureRegister)
{
	if (NULL == m_pSpecular)
	{
		m_pSpecular = new TEXTURE();
	}

	m_pSpecular->iTextureRegister = _iTextureRegister;
	m_pSpecular->iSamplerRegister = _iSamplerRegister;
}

void CMaterial::SetSpecularShaderConstantType(int _iShaderConstantType)
{
	if (NULL == m_pSpecular)
	{
		m_pSpecular = new TEXTURE();
	}

	m_pSpecular->iShaderConstantType = _iShaderConstantType;
}

bool CMaterial::Init()
{
	return true;
}

void CMaterial::SetMaterial()
{
	if (1 == m_tInfo.vAmbient.w)
	{
		int a = 0;
	}

	GET_SINGLE(CShaderMgr)->UpdateConstBuffer("Material", &m_tInfo, CUT_VERTEX | CUT_GEOMETRY | CUT_PIXEL);

	if (m_pDiffuse)
	{	
		if (m_pDiffuse->pTexture)
			m_pDiffuse->pTexture->SetTexture(m_pDiffuse->iTextureRegister, m_pDiffuse->iShaderConstantType);

		if (m_pDiffuse->pSampler)
			m_pDiffuse->pSampler->SetSampler(m_pDiffuse->iSamplerRegister, m_pDiffuse->iShaderConstantType);
	}

	if (m_pNormal)
	{
		if (m_pNormal->pTexture)
			m_pNormal->pTexture->SetTexture(m_pNormal->iTextureRegister, m_pNormal->iShaderConstantType);

		if (m_pNormal->pSampler)
			m_pNormal->pSampler->SetSampler(m_pNormal->iSamplerRegister, m_pNormal->iShaderConstantType);
	}

	if (m_pSpecular)
	{
		if (m_pSpecular->pTexture)
			m_pSpecular->pTexture->SetTexture(m_pSpecular->iTextureRegister, m_pSpecular->iShaderConstantType);

		if (m_pSpecular->pSampler)
			m_pSpecular->pSampler->SetSampler(m_pSpecular->iSamplerRegister, m_pSpecular->iShaderConstantType);
	}
}

CMaterial * CMaterial::Clone()
{
	return new CMaterial(*this);
}

CMaterial::CMaterial() :
	m_pDiffuse(NULL),
	m_pNormal(NULL),
	m_pSpecular(NULL)
{
	SetTag("Material");
	SetTypeName("CMaterial");
	SetTypeID<CMaterial>();

	m_tInfo.vDiffuse = DxVector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_tInfo.vAmbient= DxVector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_tInfo.vSpecular = DxVector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_tInfo.vEmissive = DxVector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_tInfo.fSpecularPower = 3.2f;
	m_tInfo.iBump = 0;
	m_tInfo.iSpecular = 0;
}

CMaterial::CMaterial(const CMaterial & _Material)
{
	*this = _Material;

	if (NULL != m_pDiffuse)
	{
		m_pDiffuse = new TEXTURE();
		*m_pDiffuse = *_Material.m_pDiffuse;

		if (m_pDiffuse->pTexture)
			m_pDiffuse->pTexture->AddRef();

		if (m_pDiffuse->pSampler)
			m_pDiffuse->pSampler->AddRef();
	}

	if (NULL != m_pNormal)
	{
		m_pNormal = new TEXTURE();
		*m_pNormal = *_Material.m_pNormal;

		if (m_pNormal->pTexture)
			m_pNormal->pTexture->AddRef();

		if (m_pNormal->pSampler)
			m_pNormal->pSampler->AddRef();
	}

	if (NULL != m_pSpecular)
	{
		m_pSpecular = new TEXTURE();
		*m_pSpecular = *_Material.m_pSpecular;

		if (m_pSpecular->pTexture)
			m_pSpecular->pTexture->AddRef();

		if (m_pSpecular->pSampler)
			m_pSpecular->pSampler->AddRef();
	}
}

CMaterial::~CMaterial()
{
	if (NULL != m_pDiffuse)
	{
		SAFE_RELEASE(m_pDiffuse->pTexture);
		SAFE_RELEASE(m_pDiffuse->pSampler);
		SAFE_DELETE(m_pDiffuse);
	}

	if (NULL != m_pNormal)
	{
		SAFE_RELEASE(m_pNormal->pTexture);
		SAFE_RELEASE(m_pNormal->pSampler);
		SAFE_DELETE(m_pNormal);
	}

	if (NULL != m_pSpecular)
	{
		SAFE_RELEASE(m_pSpecular->pTexture);
		SAFE_RELEASE(m_pSpecular->pSampler);
		SAFE_DELETE(m_pSpecular);
	}
}
