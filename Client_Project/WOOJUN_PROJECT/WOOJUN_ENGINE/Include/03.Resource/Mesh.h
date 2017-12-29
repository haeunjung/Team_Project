#pragma once

#include "..\BaseObj.h"

WOOJUN_BEGIN

class CMaterial;
class CFbxLoader;
class DLL CMesh : public CBaseObj
{
private:
	friend class CResMgr;
private:
	vector<pMESHCONTAINER>	m_vecMeshContainer;
	string			m_strKey;
public:
	UINT GetContainerCount() const;
	UINT GetSubsetCount(int _iContainer = 0) const;
public:
	bool CreateMesh(const string& _strKey, void* _pVertices, unsigned int _iVtxCount, unsigned int _iVtxSize, D3D11_USAGE _eVtxUsage, D3D11_PRIMITIVE_TOPOLOGY _ePrimitive,
		void* _pIndices = NULL, unsigned int _iIdxCount = 0, unsigned int _iIdxSize = 0, D3D11_USAGE _eIdxUsage = D3D11_USAGE_DEFAULT, DXGI_FORMAT _eFormat = DXGI_FORMAT_R32_UINT);
	bool CreateSphere(const string& _strKey, float _fRadius, unsigned int _iNumSub, const DxVector4& _vColor);
	bool CreateSphere(const string& _strKey, float _fRadius, unsigned int _iNumSub);
	bool LoadMesh(const string& _strKey, const wchar_t* _pFileName, const string& _strPathKey = TEXTUREPATH);
	bool LoadMesh(const string& _strKey, const char* _pFileName, const string& _strPathKey = TEXTUREPATH);
	bool LoadMeshFromFullPath(const string& _strKey, const wchar_t* _pFullPath);
	bool LoadMeshFromFullPath(const string& _strKey, const char* _pFullPath);
	void Render();
	void Render(int _iContainer, int _iSubset);
	CMaterial* CloneMaterial(int _iContainer = 0, int _iSubset = 0);
private:
	bool CreateVertexBuffer(void* _pVertices, unsigned int _iVtxCount, unsigned int _iVtxSize, D3D11_USAGE _eVtxUsage, D3D11_PRIMITIVE_TOPOLOGY _ePrimitive, pMESHCONTAINER _pContainer);
	bool CreateIndexBuffer(void* _pIndices, unsigned int _iIdxCount, unsigned int _iIdxSize, D3D11_USAGE _eIdxUsage, DXGI_FORMAT _eFormat, pMESHCONTAINER _pContainer);
	void SubDivide(vector<VERTEXCOLOR>* _pVertex, vector<unsigned int>* _pIndex);
	void SubDividePos(vector<VERTEXPOS>* _pVertex, vector<unsigned int>* _pIndex);
	bool ConvertFbxData(CFbxLoader* _pLoader);
public:
	CMesh();
	~CMesh();
};

WOOJUN_END