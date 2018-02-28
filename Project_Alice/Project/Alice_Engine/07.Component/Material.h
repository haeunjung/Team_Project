#pragma once
#include "../BaseObj.h"

WOOJUN_BEGIN

typedef struct DLL _tagTexture
{
	class CTexture*	pTexture;
	int	iTextureRegister;
	class CSampler*	pSampler;
	int iSamplerRegister;
	int	iShaderConstantType;

	_tagTexture() :
		pTexture(NULL),
		pSampler(NULL),
		iTextureRegister(-1),
		iSamplerRegister(-1),
		iShaderConstantType(CUT_PIXEL)
	{
	}
}TEXTURE, *pTEXTURE;

typedef struct DLL _tagSkinInfo
{
	pTEXTURE	pDiffuse;
	pTEXTURE	pNormal;
	pTEXTURE	pSpecular;
	pTEXTURE	pAlpha;

	_tagSkinInfo() :
		pDiffuse(NULL),
		pNormal(NULL),
		pSpecular(NULL),
		pAlpha(NULL)
	{
	}
}SKININFO, *pSKININFO;

class DLL CMaterial :
	public CBaseObj
{
private:
	friend class CRenderer2D;
	friend class CRenderer;
	friend class CMesh;
private:
	MATERIALINFO	m_tInfo;
	SKININFO		m_tBase;
	SKININFO		m_tMultiTexture;
public:
	MATERIALINFO GetMaterialInfo() const;
	SKININFO GetBaseSkin() const;
	void SetBaseSkin(const SKININFO& _tBaseSkin);
public:
	void SetMaterialInfo(const MATERIALINFO& _tInfo);
	void SetMaterialInfo(const DxVector4& _vDiffuse, const DxVector4& _vAmbient, const DxVector4& _vSpecular, const DxVector4& _vEmissive,
		float _fSpecularPower);
	void SetDiffuseColor(const DxVector4& _vDiffuse);
public:
	// Diffuse
	bool SetDiffuseTexture(const string& _strSamplerKey, const string& _strTextureKey, TCHAR* _pFileName = NULL, const string& _strPathKey = TEXTUREPATH);
	bool SetDiffuseTexture(class CTexture* _pTexture, int _iTexRegister);
	bool SetDiffuseTextureFromFullPath(const string& _strSamplerKey, const string& _strKey, const char* _pFullPath);
	void SetDiffuseRegister(int _iSamplerRegister = 0, int _iTextureRegister = 0);
	void SetDiffuseShaderConstantType(int _iShaderConstantType);

	// NormalMap
	bool SetNormalMapTexture(const string& _strSamplerKey, const string& _strTextureKey, TCHAR* _pFileName = NULL, const string& _strPathKey = TEXTUREPATH);
	bool SetNormalMapTexture(class CTexture* _pTexture, int _iTexRegister);
	bool SetNormalMapTextureFromFullPath(const string& _strSamplerKey, const string& _strKey, const char* _pFullPath);
	void SetNormalMapRegister(int _iSamplerRegister = 0, int _iTextureRegister = 0);
	void SetNormalMapShaderConstantType(int _iShaderConstantType);

	// Specular
	bool SetSpecularTexture(const string& _strSamplerKey, const string& _strTextureKey, TCHAR* _pFileName = NULL, const string& _strPathKey = TEXTUREPATH);
	bool SetSpecularTexture(class CTexture* _pTexture, int _iTexRegister);
	bool SetSpecularTextureFromFullPath(const string& _strSamplerKey, const string& _strKey, const char* _pFullPath);
	void SetSpecularRegister(int _iSamplerRegister = 0, int _iTextureRegister = 0);
	void SetSpecularShaderConstantType(int _iShaderConstantType);
public:	
	void AddDiffuseMultiTexture(const string& _strDifSmpKey, const string& _strDifTexKey,
		int _iDifTexRegister, int _iDifSmpRegister,	int _iShaderType, vector<wstring> _vecFileName, const string& _strPathKey = TEXTUREPATH);
	void AddNormalMultiTexture(const string& _strNrmSmpKey, const string& _strNrmTexKey,
		int _iNrmTexRegister, int _iNrmSmpRegister, int _iShaderType, vector<wstring> _vecFileName, const string& _strPathKey = TEXTUREPATH);
	void AddSpecularMultiTexture(const string& _strSpcSmpKey, const string& _strSpcTexKey,
		int _iSpcTexRegister, int _iSpcSmpRegister, int _iShaderType, vector<wstring> _vecFileName, const string& _strPathKey = TEXTUREPATH);
	void AddAlphaMultiTexture(const string& _strAlphaSmpKey, const string& _strAlphaTexKey,
		int _iAlphaTexRegister, int _iAlphaSmpRegister, int _iShaderType, vector<wstring> _vecFileName, const string& _strPathKey = TEXTUREPATH);
public:
	bool Init();
	void SetMaterial();
	CMaterial* Clone();
private:
	CMaterial();
	CMaterial(const CMaterial& _Material);
	~CMaterial();
};

WOOJUN_END