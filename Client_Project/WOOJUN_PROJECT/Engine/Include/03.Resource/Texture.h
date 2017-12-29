#pragma once
#include "../BaseObj.h"
#include "DirectXTex.h"

WOOJUN_BEGIN

class DLL CTexture :
	public CBaseObj
{
private:
	friend class CResMgr;
private:
	vector<::DirectX::ScratchImage*>	m_vecScratchImage;
	ID3D11ShaderResourceView*			m_pShaderResourceView;
	string		m_strKey;
public:
	bool LoadTexture(const string& _strKey, TCHAR* _pFileName, const string& _strPathKey = TEXTUREPATH);
	bool LoadTexture(const string& _strKey, CHAR* _pFileName, const string& _strPathKey = TEXTUREPATH);
	bool LoadTextureFromFullPath(const string& _strKey, const CHAR* _pFullPath);
	bool LoadTexture(const string& _strKey, const vector<wstring>& _vecFileName, const string& _strPathKey = TEXTUREPATH);
	bool LoadTextureFromMultibyte(const string& _strKey, const vector<string>& _vecFileName, const string& _strPathKey = TEXTUREPATH);
	bool LoadTextureFromFullPath(const string& _strKey, const vector<string>& _vecFullPath);
	void SetTexture(int _iRegister, int _iShaderType);
private:
	CTexture();
	~CTexture();
};

WOOJUN_END
