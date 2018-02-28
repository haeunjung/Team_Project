#pragma once
#include "..\Engine_Header.h"

WOOJUN_BEGIN

class CMesh;
class CTexture;
class CSampler;
class DLL CResMgr
{
private:
	unordered_map<string, CMesh*> m_mapMesh;
	unordered_map<string, CMesh*>::iterator m_iterMesh;

	unordered_map<string, CTexture*> m_mapTexture;
	unordered_map<string, CTexture*>::iterator m_iterTexture;

	unordered_map<string, CSampler*> m_mapSampler;
	unordered_map<string, CSampler*>::iterator m_iterSampler;
public:
	bool Init();

	// Mesh
	CMesh* CreateMesh(const string& _strKey, void* _pVertices, unsigned int _iVtxCount, unsigned int _iVtxSize, D3D11_USAGE _eVtxUsage, D3D11_PRIMITIVE_TOPOLOGY _ePrimitive,
		void* _pIndices = NULL, unsigned int _iIdxCount = 0, unsigned int _iIdxSize = 0, D3D11_USAGE _eIdxUsage = D3D11_USAGE_DEFAULT, DXGI_FORMAT _eFormat = DXGI_FORMAT_R32_UINT);
	CMesh* CreateSphere(const string& _strKey, float _fRadius, unsigned int _iNumSub, const DxVector4& vColor);
	CMesh* CreateSphere(const string& _strKey, float _fRadius, unsigned int _iNumSub);
	CMesh* LoadMesh(const string& _strKey, const WCHAR* _pFileName, const string& _strPathKey = MESHPATH);
	CMesh* LoadMesh(const string& _strKey, const CHAR* _pFileName, const string& _strPathKey = MESHPATH);
	CMesh* LoadMeshFromFullPath(const string& _strKey, const WCHAR* _pFullPath);
	CMesh* LoadMeshFromFullPath(const string& _strKey, const CHAR* _pFullPath);
	CMesh* FindMesh(const string& _strKey);

	// Texture
	CTexture* LoadTexture(const string& _strKey, TCHAR* _pFileName, const string& _strPathKey = TEXTUREPATH);
	CTexture* LoadTexture(const string& _strKey, CHAR* _pFileName, const string& _strPathKey = TEXTUREPATH);
	CTexture* LoadTextureFromFullPath(const string& _strKey, const char* _pFullPath);
	CTexture* LoadTexture(const string& _strKey, const vector<wstring>& _vecFileName, const string& _strPathKey = TEXTUREPATH);
	CTexture* LoadTextureFromMultibyte(const string& _strKey, const vector<string>& _vecFileName, const string& _strPathKey = TEXTUREPATH);
	CTexture* LoadTextureFromFullPath(const string& _strKey, const vector<string>& _vecFullPath);
	CTexture* CreateTexture(const string& _strKey, UINT _iWidth, UINT _iHeight, UINT _iArrSize,
		DXGI_FORMAT _eFmt, D3D11_USAGE _eUsage, D3D11_BIND_FLAG _eBindFlag, int _iCpuFlag);
	CTexture* FindTexture(const string& _strKey);

	// Sampler
	CSampler* CreateSampler(const string& _strKey,
		D3D11_FILTER _eFilter = D3D11_FILTER_MIN_MAG_MIP_LINEAR,
		D3D11_TEXTURE_ADDRESS_MODE _eAddrU = D3D11_TEXTURE_ADDRESS_WRAP,
		D3D11_TEXTURE_ADDRESS_MODE _eAddrV = D3D11_TEXTURE_ADDRESS_WRAP,
		D3D11_TEXTURE_ADDRESS_MODE _eAddrW = D3D11_TEXTURE_ADDRESS_WRAP);
	CSampler* FindSampler(const string& _strKey);

	DECLARE_SINGLE(CResMgr)
};

WOOJUN_END