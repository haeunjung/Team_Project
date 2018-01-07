#include "Material.h"
#include "../Device.h"
#include "../03.Resource/Texture.h"
#include "../03.Resource/Sampler.h"
#include "../03.Resource/ResMgr.h"
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
	if (NULL == m_tBase.pDiffuse)
	{
		m_tBase.pDiffuse = new TEXTURE();
	}

	SAFE_RELEASE(m_tBase.pDiffuse->pTexture);
	SAFE_RELEASE(m_tBase.pDiffuse->pSampler);

	if (NULL != _pFileName)
	{
		m_tBase.pDiffuse->pTexture = GET_SINGLE(CResMgr)->LoadTexture(_strTextureKey, _pFileName, _strPathKey);
	}
	else
	{
		m_tBase.pDiffuse->pTexture = GET_SINGLE(CResMgr)->FindTexture(_strTextureKey);
	}

	m_tBase.pDiffuse->pSampler = GET_SINGLE(CResMgr)->FindSampler(_strSamplerKey);

	return true;
}

bool CMaterial::SetDiffuseTexture(CTexture * _pTexture, int _iTexRegister)
{
	if (NULL == m_tBase.pDiffuse)
	{
		m_tBase.pDiffuse = new TEXTURE();
	}

	SAFE_RELEASE(m_tBase.pDiffuse->pTexture);

	m_tBase.pDiffuse->pTexture = _pTexture;
	_pTexture->AddRef();
	m_tBase.pDiffuse->iTextureRegister = _iTexRegister;

	return true;
}

bool CMaterial::SetDiffuseTextureFromFullPath(const string & _strSamplerKey, const string & _strKey, const char * _pFullPath)
{
	if (NULL == m_tBase.pDiffuse)
	{
		m_tBase.pDiffuse = new TEXTURE();
	}

	SAFE_RELEASE(m_tBase.pDiffuse->pTexture);
	SAFE_RELEASE(m_tBase.pDiffuse->pSampler);

	m_tBase.pDiffuse->pTexture = GET_SINGLE(CResMgr)->LoadTextureFromFullPath(_strKey, _pFullPath);
	m_tBase.pDiffuse->pSampler = GET_SINGLE(CResMgr)->FindSampler(_strSamplerKey);

	return true;
}

void CMaterial::SetDiffuseRegister(int _iSamplerRegister /*= 0*/, int _iTextureRegister /*= 0*/)
{
	if (NULL == m_tBase.pDiffuse)
	{
		m_tBase.pDiffuse = new TEXTURE();
	}

	m_tBase.pDiffuse->iTextureRegister = _iTextureRegister;
	m_tBase.pDiffuse->iSamplerRegister = _iSamplerRegister;
}

void CMaterial::SetDiffuseShaderConstantType(int _iShaderConstantType)
{
	if (NULL == m_tBase.pDiffuse)
	{
		m_tBase.pDiffuse = new TEXTURE();
	}

	m_tBase.pDiffuse->iShaderConstantType = _iShaderConstantType;
}

bool CMaterial::SetNormalMapTexture(const string & _strSamplerKey, const string & _strTextureKey, TCHAR * _pFileName, const string & _strPathKey)
{
	if (NULL == m_tBase.pNormal)
	{
		m_tBase.pNormal = new TEXTURE();
	}

	SAFE_RELEASE(m_tBase.pNormal->pTexture);
	SAFE_RELEASE(m_tBase.pNormal->pSampler);

	if (NULL != _pFileName)
	{
		m_tBase.pNormal->pTexture = GET_SINGLE(CResMgr)->LoadTexture(_strTextureKey, _pFileName, _strPathKey);
	}
	else
	{
		m_tBase.pNormal->pTexture = GET_SINGLE(CResMgr)->FindTexture(_strTextureKey);
	}

	m_tBase.pNormal->pSampler = GET_SINGLE(CResMgr)->FindSampler(_strSamplerKey);

	m_tInfo.iBump = 1;

	return true;
}

bool CMaterial::SetNormalMapTexture(CTexture * _pTexture, int _iTexRegister)
{
	if (NULL == m_tBase.pNormal)
	{
		m_tBase.pNormal = new TEXTURE();
	}

	SAFE_RELEASE(m_tBase.pNormal->pTexture);

	m_tBase.pNormal->pTexture = _pTexture;
	_pTexture->AddRef();
	m_tBase.pNormal->iTextureRegister = _iTexRegister;

	m_tInfo.iBump = 1;

	return true;
}

bool CMaterial::SetNormalMapTextureFromFullPath(const string & _strSamplerKey, const string & _strKey, const char * _pFullPath)
{
	if (NULL == m_tBase.pNormal)
	{
		m_tBase.pNormal = new TEXTURE();
	}

	SAFE_RELEASE(m_tBase.pNormal->pTexture);
	SAFE_RELEASE(m_tBase.pNormal->pSampler);

	m_tBase.pNormal->pTexture = GET_SINGLE(CResMgr)->LoadTextureFromFullPath(_strKey, _pFullPath);
	m_tBase.pNormal->pSampler = GET_SINGLE(CResMgr)->FindSampler(_strSamplerKey);

	m_tInfo.iBump = 1;

	return true;
}

void CMaterial::SetNormalMapRegister(int _iSamplerRegister, int _iTextureRegister)
{
	if (NULL == m_tBase.pNormal)
	{
		m_tBase.pNormal = new TEXTURE();
	}

	m_tBase.pNormal->iTextureRegister = _iTextureRegister;
	m_tBase.pNormal->iSamplerRegister = _iSamplerRegister;
}

void CMaterial::SetNormalMapShaderConstantType(int _iShaderConstantType)
{
	if (NULL == m_tBase.pNormal)
	{
		m_tBase.pNormal = new TEXTURE();
	}

	m_tBase.pNormal->iShaderConstantType = _iShaderConstantType;
}

bool CMaterial::SetSpecularTexture(const string & _strSamplerKey, const string & _strTextureKey, TCHAR * _pFileName, const string & _strPathKey)
{
	if (NULL == m_tBase.pSpecular)
	{
		m_tBase.pSpecular = new TEXTURE();
	}

	SAFE_RELEASE(m_tBase.pSpecular->pTexture);
	SAFE_RELEASE(m_tBase.pSpecular->pSampler);

	if (NULL != _pFileName)
	{
		m_tBase.pSpecular->pTexture = GET_SINGLE(CResMgr)->LoadTexture(_strTextureKey, _pFileName, _strPathKey);
	}
	else
	{
		m_tBase.pSpecular->pTexture = GET_SINGLE(CResMgr)->FindTexture(_strTextureKey);
	}

	m_tBase.pSpecular->pSampler = GET_SINGLE(CResMgr)->FindSampler(_strSamplerKey);

	m_tInfo.iSpecular = 1;

	return true;
}

bool CMaterial::SetSpecularTexture(CTexture * _pTexture, int _iTexRegister)
{
	if (NULL == m_tBase.pSpecular)
	{
		m_tBase.pSpecular = new TEXTURE();
	}

	SAFE_RELEASE(m_tBase.pSpecular->pTexture);

	m_tBase.pSpecular->pTexture = _pTexture;
	_pTexture->AddRef();
	m_tBase.pSpecular->iTextureRegister = _iTexRegister;

	m_tInfo.iSpecular = 1;

	return true;
}

bool CMaterial::SetSpecularTextureFromFullPath(const string & _strSamplerKey, const string & _strKey, const char * _pFullPath)
{
	if (NULL == m_tBase.pSpecular)
	{
		m_tBase.pSpecular = new TEXTURE();
	}

	SAFE_RELEASE(m_tBase.pSpecular->pTexture);
	SAFE_RELEASE(m_tBase.pSpecular->pSampler);

	m_tBase.pSpecular->pTexture = GET_SINGLE(CResMgr)->LoadTextureFromFullPath(_strKey, _pFullPath);
	m_tBase.pSpecular->pSampler = GET_SINGLE(CResMgr)->FindSampler(_strSamplerKey);

	m_tInfo.iSpecular = 1;

	return true;
}

void CMaterial::SetSpecularRegister(int _iSamplerRegister, int _iTextureRegister)
{
	if (NULL == m_tBase.pSpecular)
	{
		m_tBase.pSpecular = new TEXTURE();
	}

	m_tBase.pSpecular->iTextureRegister = _iTextureRegister;
	m_tBase.pSpecular->iSamplerRegister = _iSamplerRegister;
}

void CMaterial::SetSpecularShaderConstantType(int _iShaderConstantType)
{
	if (NULL == m_tBase.pSpecular)
	{
		m_tBase.pSpecular = new TEXTURE();
	}

	m_tBase.pSpecular->iShaderConstantType = _iShaderConstantType;
}

void CMaterial::AddDiffuseMultiTexture(const string & _strDifSmpKey, const string & _strDifTexKey,
	int _iDifTexRegister, int _iDifSmpRegister, int _iShaderType, vector<wstring> _vecFileName, const string& _strPathKey /*= TEXTUREPATH*/)
{
	if (m_tMultiTexture.pDiffuse)
	{
		SAFE_RELEASE(m_tMultiTexture.pDiffuse->pTexture);
		SAFE_RELEASE(m_tMultiTexture.pDiffuse->pSampler);
		SAFE_DELETE(m_tMultiTexture.pDiffuse);
	}

	m_tMultiTexture.pDiffuse = new TEXTURE();

	m_tMultiTexture.pDiffuse->pTexture = GET_SINGLE(CResMgr)->LoadTexture(_strDifTexKey, _vecFileName, _strPathKey);
	m_tMultiTexture.pDiffuse->pSampler = GET_SINGLE(CResMgr)->FindSampler(_strDifSmpKey);

	m_tMultiTexture.pDiffuse->iTextureRegister = _iDifTexRegister;
	m_tMultiTexture.pDiffuse->iSamplerRegister = _iDifSmpRegister;
	m_tMultiTexture.pDiffuse->iShaderConstantType = _iShaderType;
}

void CMaterial::AddNormalMultiTexture(const string & _strNrmSmpKey, const string & _strNrmTexKey,
	int _iNrmTexRegister, int _iNrmSmpRegister, int _iShaderType, vector<wstring> _vecFileName, const string& _strPathKey /*= TEXTUREPATH*/)
{
	if (m_tMultiTexture.pNormal)
	{
		SAFE_RELEASE(m_tMultiTexture.pNormal->pTexture);
		SAFE_RELEASE(m_tMultiTexture.pNormal->pSampler);
		SAFE_DELETE(m_tMultiTexture.pNormal);
	}

	m_tMultiTexture.pNormal = new TEXTURE();

	m_tMultiTexture.pNormal->pTexture = GET_SINGLE(CResMgr)->LoadTexture(_strNrmTexKey, _vecFileName, _strPathKey);
	m_tMultiTexture.pNormal->pSampler = GET_SINGLE(CResMgr)->FindSampler(_strNrmSmpKey);

	m_tMultiTexture.pNormal->iTextureRegister = _iNrmTexRegister;
	m_tMultiTexture.pNormal->iSamplerRegister = _iNrmSmpRegister;
	m_tMultiTexture.pNormal->iShaderConstantType = _iShaderType;
}

void CMaterial::AddSpecularMultiTexture(const string & _strSpcSmpKey, const string & _strSpcTexKey,
	int _iSpcTexRegister, int _iSpcSmpRegister, int _iShaderType, vector<wstring> _vecFileName, const string& _strPathKey /*= TEXTUREPATH*/)
{
	if (m_tMultiTexture.pSpecular)
	{
		SAFE_RELEASE(m_tMultiTexture.pSpecular->pTexture);
		SAFE_RELEASE(m_tMultiTexture.pSpecular->pSampler);
		SAFE_DELETE(m_tMultiTexture.pSpecular);
	}

	m_tMultiTexture.pSpecular = new TEXTURE();

	m_tMultiTexture.pSpecular->pTexture = GET_SINGLE(CResMgr)->LoadTexture(_strSpcTexKey, _vecFileName, _strPathKey);
	m_tMultiTexture.pSpecular->pSampler = GET_SINGLE(CResMgr)->FindSampler(_strSpcSmpKey);

	m_tMultiTexture.pSpecular->iTextureRegister = _iSpcTexRegister;
	m_tMultiTexture.pSpecular->iSamplerRegister = _iSpcSmpRegister;
	m_tMultiTexture.pSpecular->iShaderConstantType = _iShaderType;
}

void CMaterial::AddAlphaMultiTexture(const string & _strAlphaSmpKey, const string & _strAlphaTexKey,
	int _iAlphaTexRegister, int _iAlphaSmpRegister, int _iShaderType, vector<wstring> _vecFileName, const string & _strPathKey)
{
	if (m_tMultiTexture.pAlpha)
	{
		SAFE_RELEASE(m_tMultiTexture.pAlpha->pTexture);
		SAFE_RELEASE(m_tMultiTexture.pAlpha->pSampler);
		SAFE_DELETE(m_tMultiTexture.pAlpha);
	}

	m_tMultiTexture.pAlpha = new TEXTURE();

	m_tMultiTexture.pAlpha->pTexture = GET_SINGLE(CResMgr)->LoadTexture(_strAlphaTexKey, _vecFileName, _strPathKey);
	m_tMultiTexture.pAlpha->pSampler = GET_SINGLE(CResMgr)->FindSampler(_strAlphaSmpKey);

	m_tMultiTexture.pAlpha->iTextureRegister = _iAlphaTexRegister;
	m_tMultiTexture.pAlpha->iSamplerRegister = _iAlphaSmpRegister;
	m_tMultiTexture.pAlpha->iShaderConstantType = _iShaderType;
}

bool CMaterial::Init()
{
	return true;
}

void CMaterial::SetMaterial()
{
	GET_SINGLE(CShaderMgr)->UpdateConstBuffer("Material", &m_tInfo, CUT_VERTEX | CUT_GEOMETRY | CUT_PIXEL);

	if (m_tBase.pDiffuse)
	{	
		if (m_tBase.pDiffuse->pTexture)
			m_tBase.pDiffuse->pTexture->SetTexture(m_tBase.pDiffuse->iTextureRegister, m_tBase.pDiffuse->iShaderConstantType);

		if (m_tBase.pDiffuse->pSampler)
			m_tBase.pDiffuse->pSampler->SetSampler(m_tBase.pDiffuse->iSamplerRegister, m_tBase.pDiffuse->iShaderConstantType);
	}

	if (m_tBase.pNormal)
	{
		if (m_tBase.pNormal->pTexture)
			m_tBase.pNormal->pTexture->SetTexture(m_tBase.pNormal->iTextureRegister, m_tBase.pNormal->iShaderConstantType);

		if (m_tBase.pNormal->pSampler)
			m_tBase.pNormal->pSampler->SetSampler(m_tBase.pNormal->iSamplerRegister, m_tBase.pNormal->iShaderConstantType);
	}

	if (m_tBase.pSpecular)
	{
		if (m_tBase.pSpecular->pTexture)
			m_tBase.pSpecular->pTexture->SetTexture(m_tBase.pSpecular->iTextureRegister, m_tBase.pSpecular->iShaderConstantType);

		if (m_tBase.pSpecular->pSampler)
			m_tBase.pSpecular->pSampler->SetSampler(m_tBase.pSpecular->iSamplerRegister, m_tBase.pSpecular->iShaderConstantType);
	}

	if (m_tMultiTexture.pDiffuse)
	{
		if (m_tMultiTexture.pDiffuse->pTexture)
			m_tMultiTexture.pDiffuse->pTexture->SetTexture(m_tMultiTexture.pDiffuse->iTextureRegister, m_tMultiTexture.pDiffuse->iShaderConstantType);

		if (m_tMultiTexture.pDiffuse->pSampler)
			m_tMultiTexture.pDiffuse->pSampler->SetSampler(m_tMultiTexture.pDiffuse->iSamplerRegister, m_tMultiTexture.pDiffuse->iShaderConstantType);
	}

	if (m_tMultiTexture.pNormal)
	{
		if (m_tMultiTexture.pNormal->pTexture)
			m_tMultiTexture.pNormal->pTexture->SetTexture(m_tMultiTexture.pNormal->iTextureRegister, m_tMultiTexture.pNormal->iShaderConstantType);

		if (m_tMultiTexture.pNormal->pSampler)
			m_tMultiTexture.pNormal->pSampler->SetSampler(m_tMultiTexture.pNormal->iSamplerRegister, m_tMultiTexture.pNormal->iShaderConstantType);
	}

	if (m_tMultiTexture.pSpecular)
	{
		if (m_tMultiTexture.pSpecular->pTexture)
			m_tMultiTexture.pSpecular->pTexture->SetTexture(m_tMultiTexture.pSpecular->iTextureRegister, m_tMultiTexture.pSpecular->iShaderConstantType);

		if (m_tMultiTexture.pSpecular->pSampler)
			m_tMultiTexture.pSpecular->pSampler->SetSampler(m_tMultiTexture.pSpecular->iSamplerRegister, m_tMultiTexture.pSpecular->iShaderConstantType);
	}

	if (m_tMultiTexture.pAlpha)
	{
		if (m_tMultiTexture.pAlpha->pTexture)
			m_tMultiTexture.pAlpha->pTexture->SetTexture(m_tMultiTexture.pAlpha->iTextureRegister, m_tMultiTexture.pAlpha->iShaderConstantType);

		if (m_tMultiTexture.pAlpha->pSampler)
			m_tMultiTexture.pAlpha->pSampler->SetSampler(m_tMultiTexture.pAlpha->iSamplerRegister, m_tMultiTexture.pAlpha->iShaderConstantType);
	}
}

CMaterial * CMaterial::Clone()
{
	return new CMaterial(*this);
}

CMaterial::CMaterial()
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

	m_tBase.pDiffuse = NULL;
	m_tBase.pNormal = NULL;
	m_tBase.pSpecular = NULL;
	m_tBase.pAlpha = NULL;
}

CMaterial::CMaterial(const CMaterial & _Material)
{
	*this = _Material;

	if (NULL != m_tBase.pDiffuse)
	{
		m_tBase.pDiffuse = new TEXTURE();
		*m_tBase.pDiffuse = *_Material.m_tBase.pDiffuse;

		if (m_tBase.pDiffuse->pTexture)
			m_tBase.pDiffuse->pTexture->AddRef();

		if (m_tBase.pDiffuse->pSampler)
			m_tBase.pDiffuse->pSampler->AddRef();
	}

	if (NULL != m_tBase.pNormal)
	{
		m_tBase.pNormal = new TEXTURE();
		*m_tBase.pNormal = *_Material.m_tBase.pNormal;

		if (m_tBase.pNormal->pTexture)
			m_tBase.pNormal->pTexture->AddRef();

		if (m_tBase.pNormal->pSampler)
			m_tBase.pNormal->pSampler->AddRef();
	}

	if (NULL != m_tBase.pSpecular)
	{
		m_tBase.pSpecular = new TEXTURE();
		*m_tBase.pSpecular = *_Material.m_tBase.pSpecular;

		if (m_tBase.pSpecular->pTexture)
			m_tBase.pSpecular->pTexture->AddRef();

		if (m_tBase.pSpecular->pSampler)
			m_tBase.pSpecular->pSampler->AddRef();
	}

	if (NULL != m_tMultiTexture.pDiffuse)
	{
		m_tMultiTexture.pDiffuse = new TEXTURE();
		*m_tMultiTexture.pDiffuse = *_Material.m_tMultiTexture.pDiffuse;

		if (m_tMultiTexture.pDiffuse->pTexture)
			m_tMultiTexture.pDiffuse->pTexture->AddRef();

		if (m_tMultiTexture.pDiffuse->pSampler)
			m_tMultiTexture.pDiffuse->pSampler->AddRef();
	}

	if (NULL != m_tMultiTexture.pNormal)
	{
		m_tMultiTexture.pNormal = new TEXTURE();
		*m_tMultiTexture.pNormal = *_Material.m_tMultiTexture.pNormal;

		if (m_tMultiTexture.pNormal->pTexture)
			m_tMultiTexture.pNormal->pTexture->AddRef();

		if (m_tMultiTexture.pNormal->pSampler)
			m_tMultiTexture.pNormal->pSampler->AddRef();
	}

	if (NULL != m_tMultiTexture.pSpecular)
	{
		m_tMultiTexture.pSpecular = new TEXTURE();
		*m_tMultiTexture.pSpecular = *_Material.m_tMultiTexture.pSpecular;

		if (m_tMultiTexture.pSpecular->pTexture)
			m_tMultiTexture.pSpecular->pTexture->AddRef();

		if (m_tMultiTexture.pSpecular->pSampler)
			m_tMultiTexture.pSpecular->pSampler->AddRef();
	}

	if (NULL != m_tMultiTexture.pAlpha)
	{
		m_tMultiTexture.pAlpha = new TEXTURE();
		*m_tMultiTexture.pAlpha = *_Material.m_tMultiTexture.pAlpha;

		if (m_tMultiTexture.pAlpha->pTexture)
			m_tMultiTexture.pAlpha->pTexture->AddRef();

		if (m_tMultiTexture.pAlpha->pSampler)
			m_tMultiTexture.pAlpha->pSampler->AddRef();
	}
}

CMaterial::~CMaterial()
{
	if (NULL != m_tBase.pDiffuse)
	{
		SAFE_RELEASE(m_tBase.pDiffuse->pTexture);
		SAFE_RELEASE(m_tBase.pDiffuse->pSampler);
		SAFE_DELETE(m_tBase.pDiffuse);
	}

	if (NULL != m_tBase.pNormal)
	{
		SAFE_RELEASE(m_tBase.pNormal->pTexture);
		SAFE_RELEASE(m_tBase.pNormal->pSampler);
		SAFE_DELETE(m_tBase.pNormal);
	}

	if (NULL != m_tBase.pSpecular)
	{
		SAFE_RELEASE(m_tBase.pSpecular->pTexture);
		SAFE_RELEASE(m_tBase.pSpecular->pSampler);
		SAFE_DELETE(m_tBase.pSpecular);
	}

	if (NULL != m_tMultiTexture.pDiffuse)
	{
		SAFE_RELEASE(m_tMultiTexture.pDiffuse->pTexture);
		SAFE_RELEASE(m_tMultiTexture.pDiffuse->pSampler);
		SAFE_DELETE(m_tMultiTexture.pDiffuse);
	}

	if (NULL != m_tMultiTexture.pNormal)
	{
		SAFE_RELEASE(m_tMultiTexture.pNormal->pTexture);
		SAFE_RELEASE(m_tMultiTexture.pNormal->pSampler);
		SAFE_DELETE(m_tMultiTexture.pNormal);
	}

	if (NULL != m_tMultiTexture.pSpecular)
	{
		SAFE_RELEASE(m_tMultiTexture.pSpecular->pTexture);
		SAFE_RELEASE(m_tMultiTexture.pSpecular->pSampler);
		SAFE_DELETE(m_tMultiTexture.pSpecular);
	}

	if (NULL != m_tMultiTexture.pAlpha)
	{
		SAFE_RELEASE(m_tMultiTexture.pAlpha->pTexture);
		SAFE_RELEASE(m_tMultiTexture.pAlpha->pSampler);
		SAFE_DELETE(m_tMultiTexture.pAlpha);
	}	
}
