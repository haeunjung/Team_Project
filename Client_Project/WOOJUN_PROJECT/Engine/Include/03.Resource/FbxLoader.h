#pragma once

#ifdef _DEBUG
#pragma comment(lib, "libfbxsdk-md_Debug.lib")
#else
#pragma comment(lib, "libfbxsdk-md.lib")
#endif // _DEBUG
// Library에서 내부적으로 DLL을 끌어다 쓴다

#include <fbxsdk.h>
#include "../Engine_Header.h"

WOOJUN_BEGIN

class DLL CFbxLoader
{
private:
	friend class CMesh;
private:
	FbxManager*		m_pManager;
	FbxScene*		m_pScene;
	FbxImporter*	m_pImporter;
	vector<pFBXMESHCONTAINER>		m_vecMeshContainger;
	vector<vector<pFBXMATERIAL>>	m_vecMaterial;
public:
	const vector<pFBXMESHCONTAINER>* GetMeshContainer() const;
	const vector<vector<pFBXMATERIAL>>* GetMaterials() const;
public:
	bool LoadFBX(const TCHAR* _pFileName, const string& _strPathKey = MESHPATH);
	bool LoadFBX(const char* _pFileName, const string& _strPathKey = MESHPATH);
	bool LoadFBXFullPath(const TCHAR* _pFullPath);
	bool LoadFBXFullPath(const char* _pFullPath);
private:
	void Triangulate(FbxNode* _pNode);
	void LoadMaterial(FbxSurfaceMaterial* _pMaterial);
	DxVector4 GetMaterialColor(FbxSurfaceMaterial* _pMaterial, const char* _pPropertyName, const char* _pPropertyFactorName);
	double GetMaterialFactor(FbxSurfaceMaterial* _pMaterial, const char* _pPropertyName);
	string GetMaterialTexture(FbxSurfaceMaterial* _pMaterial, const char* _pPropertyName);

	bool LoadMesh(FbxNode* _pNode);
	bool LoadMesh(FbxMesh* _pMesh);
	void LoadNormal(FbxMesh* _pMesh, pFBXMESHCONTAINER _pContainer, int _iVtxID, int _iControlIndex);
	void LoadUV(FbxMesh* _pMesh, pFBXMESHCONTAINER _pContainer, int _iUVID, int _iControlIndex);
	void LoadTangent(FbxMesh* _pMesh, pFBXMESHCONTAINER _pContainer, int _iVtxID, int _iControlIndex);
	void LoadBinormal(FbxMesh* _pMesh, pFBXMESHCONTAINER _pContainer, int _iVtxID, int _iControlIndex);
	
private:
	CFbxLoader();
	~CFbxLoader();
};

WOOJUN_END