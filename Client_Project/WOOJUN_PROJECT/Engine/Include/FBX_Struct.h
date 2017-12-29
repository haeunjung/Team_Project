#pragma once
#include "Engine_Macro.h"

WOOJUN_BEGIN

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
	vector<vector<UINT>>	vecIndices;
	bool					bBump;
}FBXMESHCONTAINER, *pFBXMESHCONTAINER;

WOOJUN_END