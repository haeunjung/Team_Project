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

class DLL CMaterial :
	public CBaseObj
{
private:
	friend class CRenderer2D;
	friend class CRenderer;
	friend class CMesh;
private:
	MATERIALINFO	m_tInfo;
	pTEXTURE		m_pDiffuse;
	pTEXTURE		m_pNormal;
	pTEXTURE		m_pSpecular;
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
	bool Init();
	void SetMaterial();
	CMaterial* Clone();
private:
	CMaterial();
	CMaterial(const CMaterial& _Material);
	~CMaterial();
};

WOOJUN_END