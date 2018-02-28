#pragma once
#include "../BaseObj.h"
#include "DirectXTex.h"

WOOJUN_BEGIN

class DLL CTexture :
	public CBaseObj
{
private:
	friend class CResMgr;
	friend class CMyRenderTarget;
public:
	static CTexture* CreateTexture(const string& _strKey, UINT _iWidth, UINT _iHeight, UINT _iArrSize,
		DXGI_FORMAT _eFormat, D3D11_USAGE _eUsage, D3D11_BIND_FLAG _eBindFlag, int _iCpuFlag);
private:
	vector<::DirectX::ScratchImage*>	m_vecScratchImage;
	ID3D11ShaderResourceView*			m_pShaderResourceView;
	ID3D11Texture2D*					m_pTexture;
	string		m_strKey;
	string		m_strPathKey;
	wstring		m_strFullPath;
	wstring		m_strFileName;
public:
	ID3D11Texture2D* GetTexture() const;
	string GetKey() const;
	string GetPathKey() const;
	wstring GetFullPath() const;
	wstring GetFileName() const;
public:
	void SetPathKey(const string& _strPathKey);
public:
	bool LoadTexture(const string& _strKey, TCHAR* _pFileName, const string& _strPathKey = TEXTUREPATH);
	bool LoadTexture(const string& _strKey, CHAR* _pFileName, const string& _strPathKey = TEXTUREPATH);
	bool LoadTextureFromFullPath(const string& _strKey, const CHAR* _pFullPath);
	bool LoadTexture(const string& _strKey, const vector<wstring>& _vecFileName, const string& _strPathKey = TEXTUREPATH);
	bool LoadTextureFromMultibyte(const string& _strKey, const vector<string>& _vecFileName, const string& _strPathKey = TEXTUREPATH);
	bool LoadTextureFromFullPath(const string& _strKey, const vector<string>& _vecFullPath);
	bool CreateResource(const string& _strKey, UINT _iWidth, UINT _iHeight, UINT _iArrSize,
		DXGI_FORMAT _eFormat, D3D11_USAGE _eUsage, D3D11_BIND_FLAG _eBindFlag, int _iCpuFlag);
	void UpdateData(void* _pData, int _iSize);
	void SetTexture(int _iRegister, int _iShaderType);
	void SaveTextureFile(const WCHAR* _pFileName, const string& _strPathKey = TEXTUREPATH);
private:
	CTexture();
	~CTexture();
};

WOOJUN_END
