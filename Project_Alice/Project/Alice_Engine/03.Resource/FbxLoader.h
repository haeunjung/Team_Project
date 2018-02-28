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

// Fbx Animation Clip
typedef struct DLL _tagFbxAnimationClip
{
	string			strName;
	FbxTime			tStart;
	FbxTime			tEnd;
	FbxLongLong		lTimeLength;
	FbxTime::EMode	eTimeMode;
}FBXANIMATIONCLIP, *pFBXANIMATIONCLIP;

// Key Frame
typedef struct DLL _tagFbxKeyFrame
{
	FbxAMatrix	matTransform;
	double		dTime;
}FBXKEYFRAME, *pFBXKEYFRAME;

// Bone
typedef struct DLL _tagFbxBone
{
	string		strName;
	int			iDepth;
	int			iParentIndex;
	FbxAMatrix	matOffset;
	FbxAMatrix	matBone;
	vector<FBXKEYFRAME>	vecKeyFrame;
}FBXBONE, *pFBXBONE;

// 가중치 정보
typedef struct DLL _tagFbxWeight
{
	int		iIndex;
	double	dWeight;
}FBXWEIGHT, *pFBXWEIGHT;

typedef struct DLL _tagFbxMaterial
{
	DxVector4	vDiffuse;
	DxVector4	vAmbient;
	DxVector4	vSpecular;
	DxVector4	vEmissive;
	float		fSpecularPower;
	float		fTransparencyFactor;
	float		fShininess;
	string		strDiffuseTexture;
	string		strBumpTexture;
	string		strSpecularTexture;
}FBXMATERIAL, *pFBXMATERIAL;

typedef struct DLL _tagFbxMeshContainer
{
	vector<DxVector3>		vecPos;
	vector<DxVector3>		vecNormal;
	vector<DxVector2>		vecUV;
	vector<DxVector3>		vecTangent;
	vector<DxVector3>		vecBinormal;
	vector<DxVector4>		vecBlendWeight;
	vector<DxVector4>		vecBlendIndex;
	vector<vector<UINT>>	vecIndices;
	unordered_map<int, vector<FBXWEIGHT>>	mapWeights;
	bool					bBump;
	bool					bAnimation;
}FBXMESHCONTAINER, *pFBXMESHCONTAINER;

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
	vector<pFBXBONE>			m_vecBones;
	vector<pFBXANIMATIONCLIP>	m_vecClip;
	FbxArray<FbxString*>			m_NameArr;
public:
	const vector<pFBXBONE>* GetBones() const;
	const vector<pFBXANIMATIONCLIP>* GetClip() const;
	const vector<pFBXMESHCONTAINER>* GetMeshContainer() const;
	const vector<vector<pFBXMATERIAL>>* GetMaterials() const;
public:
	bool LoadFBX(const TCHAR* _pFileName, const string& _strPathKey = MESHPATH);
	bool LoadFBX(const char* _pFileName, const string& _strPathKey = MESHPATH);
	bool LoadFBXFullPath(const TCHAR* _pFullPath);
	bool LoadFBXFullPath(const char* _pFullPath);
private:
	void Triangulate(FbxNode* _pNode);
	bool LoadMesh(FbxNode* _pNode);
	bool LoadMesh(FbxMesh* _pMesh);	
	void LoadNormal(FbxMesh* _pMesh, pFBXMESHCONTAINER _pContainer, int _iVtxID, int _iControlIndex);
	void LoadUV(FbxMesh* _pMesh, pFBXMESHCONTAINER _pContainer, int _iUVID, int _iControlIndex);
	void LoadTangent(FbxMesh* _pMesh, pFBXMESHCONTAINER _pContainer, int _iVtxID, int _iControlIndex);
	void LoadBinormal(FbxMesh* _pMesh, pFBXMESHCONTAINER _pContainer, int _iVtxID, int _iControlIndex);
	void LoadMaterial(FbxSurfaceMaterial* _pMaterial);
	DxVector4 GetMaterialColor(FbxSurfaceMaterial* _pMaterial, const char* _pPropertyName, const char* _pPropertyFactorName);
	double GetMaterialFactor(FbxSurfaceMaterial* _pMaterial, const char* _pPropertyName);
	string GetMaterialTexture(FbxSurfaceMaterial* _pMaterial, const char* _pPropertyName);
	
// Animation
private:
	void LoadSkeleton(FbxNode* _pNode);
	void LoadSkeletonRecursive(FbxNode* _pNode, int _iDepth, int _iIndex, int _iParentIndex);
	void LoadAnimationClip();
	void LoadAnimation(FbxMesh* _pMesh, pFBXMESHCONTAINER _pContainer);
	void LoadWeightAndIndex(FbxCluster* _pCluster, int _iBoneIndex, pFBXMESHCONTAINER _pContainer);
	void LoadOffsetMatrix(FbxCluster* _pCluster, const FbxAMatrix& _matTransform, int _iBoneIndex, pFBXMESHCONTAINER _pContainer);
	void LoadTimeTransform(FbxNode* _pNode, FbxCluster* _pCluster, const FbxAMatrix& _matTransform, int _iBoneIndex, pFBXMESHCONTAINER _pContainer);
	void ChangeWeightAndIndices(pFBXMESHCONTAINER _pContainer);
	int FindBoneFromName(const string& _strName);
	FbxAMatrix GetTransform(FbxNode* _pNode);
private:
	CFbxLoader();
	~CFbxLoader();
};

WOOJUN_END