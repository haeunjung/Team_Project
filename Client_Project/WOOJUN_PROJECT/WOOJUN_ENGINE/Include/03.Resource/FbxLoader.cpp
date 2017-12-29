#include "FbxLoader.h"
#include "../01.Core/PathMgr.h"

WOOJUN_USING

CFbxLoader::CFbxLoader() :
	m_pManager(NULL),
	m_pScene(NULL),
	m_pImporter(NULL)
{
}

CFbxLoader::~CFbxLoader()
{
	Safe_Delete_VecList(m_vecMeshContainger);

	for (size_t i = 0; i < m_vecMaterial.size(); ++i)
	{
		Safe_Delete_VecList(m_vecMaterial[i]);
	}

	m_pScene->Destroy();
	m_pManager->Destroy();
}

const vector<pFBXMESHCONTAINER>* CFbxLoader::GetMeshContainer() const
{
	return &m_vecMeshContainger;
}

const vector<vector<pFBXMATERIAL>>* CFbxLoader::GetMaterials() const
{
	return &m_vecMaterial;;
}

bool CFbxLoader::LoadFBX(const TCHAR * _pFileName, const string & _strPathKey)
{
	char	strFileName[MAX_PATH] = {};
	WideCharToMultiByte(CP_ACP, 0, _pFileName, -1, strFileName, lstrlen(_pFileName), 0, 0);
	string	strPath = GET_SINGLE(CPathMgr)->FindPathToMultiByte(_strPathKey);

	strPath += strFileName;

	return LoadFBXFullPath(strPath.c_str());
}

bool CFbxLoader::LoadFBX(const char * _pFileName, const string & _strPathKey)
{
	string	strPath = GET_SINGLE(CPathMgr)->FindPathToMultiByte(_strPathKey);

	strPath += _pFileName;

	return LoadFBXFullPath(strPath.c_str());
}

bool CFbxLoader::LoadFBXFullPath(const TCHAR * _pFullPath)
{
	char	strFileName[MAX_PATH] = {};
	WideCharToMultiByte(CP_ACP, 0, _pFullPath, -1, strFileName, lstrlen(_pFullPath), 0, 0);
	
	return LoadFBX(strFileName);
}

bool CFbxLoader::LoadFBXFullPath(const char * _pFullPath)
{
	// Fbx Manager 생성
	m_pManager = FbxManager::Create();

	// IO Setting 생성 (Fbx Manager 필요)
	FbxIOSettings*	pIOSetting = FbxIOSettings::Create(m_pManager,IOSROOT);

	m_pManager->SetIOSettings(pIOSetting);
	
	// Fbx Scene 생성 (Fbx Manager 필요)
	m_pScene = FbxScene::Create(m_pManager, "Fbx Scene");

	// Fbx 정보 읽어올 Importer 생성
	// Import만 하기때문에 Export 필요없음
	m_pImporter = FbxImporter::Create(m_pManager, "Fbx Importer");

	m_pImporter->Initialize(_pFullPath, -1, m_pManager->GetIOSettings());

	m_pImporter->Import(m_pScene);

	// FbxAxisSystem이 Max가 아니라면
	// Max축으로 세팅한다.
	if (m_pScene->GetGlobalSettings().GetAxisSystem() != FbxAxisSystem::Max)
	{
		m_pScene->GetGlobalSettings().SetAxisSystem(FbxAxisSystem::Max);
	}

	Triangulate(m_pScene->GetRootNode());

	LoadMesh(m_pScene->GetRootNode());

	return true;
}

void CFbxLoader::Triangulate(FbxNode * _pNode)
{
	FbxNodeAttribute*	pAttribute = _pNode->GetNodeAttribute();
	//FbxNodeAttribute::EType	eType = pAttribute->GetAttributeType();

	if (pAttribute && (pAttribute->GetAttributeType() == FbxNodeAttribute::eMesh ||
		pAttribute->GetAttributeType() == FbxNodeAttribute::eNurbs ||
		pAttribute->GetAttributeType() == FbxNodeAttribute::eNurbsSurface))
	{
		FbxGeometryConverter	converter(m_pManager);

		converter.Triangulate(pAttribute, true);
	}

	int iMaterialCount = _pNode->GetMaterialCount();

	if (iMaterialCount > 0)
	{
		vector<pFBXMATERIAL>	vecMaterial;

		m_vecMaterial.push_back(vecMaterial);

		for (int i = 0; i < iMaterialCount; ++i)
		{
			FbxSurfaceMaterial*	pMaterial = _pNode->GetMaterial(i);

			if (NULL == pMaterial)
			{
				continue;
			}

			LoadMaterial(pMaterial);
		}
	}

	int iChildCount = _pNode->GetChildCount();

	for (int i = 0; i < iChildCount; ++i)
	{
		Triangulate(_pNode->GetChild(i));
	}
}

void CFbxLoader::LoadMaterial(FbxSurfaceMaterial * _pMaterial)
{
	pFBXMATERIAL	pMaterialInfo = new FBXMATERIAL();

	m_vecMaterial[m_vecMaterial.size() - 1].push_back(pMaterialInfo);

	pMaterialInfo->vDiffuse = GetMaterialColor(_pMaterial, FbxSurfaceMaterial::sDiffuse, FbxSurfaceMaterial::sDiffuseFactor);
	pMaterialInfo->vAmbient = GetMaterialColor(_pMaterial, FbxSurfaceMaterial::sAmbient, FbxSurfaceMaterial::sAmbientFactor);
	pMaterialInfo->vSpecular = GetMaterialColor(_pMaterial, FbxSurfaceMaterial::sSpecular, FbxSurfaceMaterial::sSpecularFactor);
	pMaterialInfo->vEmissive = GetMaterialColor(_pMaterial, FbxSurfaceMaterial::sEmissive, FbxSurfaceMaterial::sEmissiveFactor);

	pMaterialInfo->fSpecularPower = GetMaterialFactor(_pMaterial, FbxSurfaceMaterial::sSpecularFactor);
	pMaterialInfo->fShininess = GetMaterialFactor(_pMaterial, FbxSurfaceMaterial::sShininess);
	pMaterialInfo->fTransparencyFactor = GetMaterialFactor(_pMaterial, FbxSurfaceMaterial::sTransparencyFactor);

	pMaterialInfo->strDiffuseTexture = GetMaterialTexture(_pMaterial, FbxSurfaceMaterial::sDiffuse);
	pMaterialInfo->strBumpTexture = GetMaterialTexture(_pMaterial, FbxSurfaceMaterial::sNormalMap);

	if (true == pMaterialInfo->strBumpTexture.empty())
	{
		pMaterialInfo->strBumpTexture = GetMaterialTexture(_pMaterial, FbxSurfaceMaterial::sBump);
	}

	pMaterialInfo->strSpecularTexture = GetMaterialTexture(_pMaterial, FbxSurfaceMaterial::sSpecular);
}

DxVector4 CFbxLoader::GetMaterialColor(FbxSurfaceMaterial * _pMaterial, const char * _pPropertyName, const char * _pPropertyFactorName)
{
	FbxDouble3	vResult(0.0f, 0.0f, 0.0f);
	double		dFactor = 0.0f;

	FbxProperty	tProperty = _pMaterial->FindProperty(_pPropertyName);
	FbxProperty	tPropertyFactor = _pMaterial->FindProperty(_pPropertyFactorName);

	// 유효성 체크
	if (true == tProperty.IsValid()
		&& true == tPropertyFactor.IsValid())
	{
		// 템플릿 함수 Get
		vResult = tProperty.Get<FbxDouble3>();
		dFactor = tPropertyFactor.Get<FbxDouble>();

		if (1 != dFactor)
		{
			vResult[0] *= dFactor;
			vResult[1] *= dFactor;
			vResult[2] *= dFactor;
		}
	}

	return DxVector4(vResult[0], vResult[1], vResult[2], dFactor);
}

double CFbxLoader::GetMaterialFactor(FbxSurfaceMaterial * _pMaterial, const char * _pPropertyName)
{
	FbxProperty	tProperty = _pMaterial->FindProperty(_pPropertyName);
	double	dFactor = 0.0f;

	if (true == tProperty.IsValid())
	{
		dFactor = tProperty.Get<FbxDouble>();
	}

	return dFactor;
}

string CFbxLoader::GetMaterialTexture(FbxSurfaceMaterial * _pMaterial, const char * _pPropertyName)
{
	FbxProperty	tProperty = _pMaterial->FindProperty(_pPropertyName);

	string	str = "";

	if (true == tProperty.IsValid())
	{
		int iTexCount = tProperty.GetSrcObjectCount<FbxFileTexture>();

		if (0 < iTexCount)
		{
			FbxFileTexture*	pFileTexture = tProperty.GetSrcObject<FbxFileTexture>(0);

			if (NULL != pFileTexture)
			{
				str = pFileTexture->GetFileName();
			}
		}
	}

	return str;
}

bool CFbxLoader::LoadMesh(FbxNode * _pNode)
{
	FbxNodeAttribute*	pAttribute = _pNode->GetNodeAttribute();
	//FbxNodeAttribute::EType	eType = pAttribute->GetAttributeType();

	if (pAttribute && pAttribute->GetAttributeType() == FbxNodeAttribute::eMesh)
	{
		FbxMesh*	pMesh = _pNode->GetMesh();

		if (NULL != pMesh)
		{
			LoadMesh(pMesh);
		}				
	}

	int iChildCount = _pNode->GetChildCount();

	for (int i = 0; i < iChildCount; ++i)
	{
		LoadMesh(_pNode->GetChild(i));
	}
	
	return true;
}

bool CFbxLoader::LoadMesh(FbxMesh * _pMesh)
{
	pFBXMESHCONTAINER	pContainer = new FBXMESHCONTAINER();		
	// FBXMESHCONTAINER 구조체는 STL(vector)를 가지고 있다
	// 함부로 메모리셋(pContainer = {};)를 하다가는
	// STL 자체 정보까지 밀어버릴 수 있다

	pContainer->bBump = false;

	m_vecMeshContainger.push_back(pContainer);

	// ControlPoint는 위치정보를 담고 있는 배열
	// 이 배열의 개수는 곧 정점의 개수
	int	iVtxCount = _pMesh->GetControlPointsCount();
	FbxVector4*	pVtxPos = _pMesh->GetControlPoints();

	pContainer->vecPos.resize(iVtxCount);
	pContainer->vecNormal.resize(iVtxCount);
	pContainer->vecUV.resize(iVtxCount);
	pContainer->vecTangent.resize(iVtxCount);
	pContainer->vecBinormal.resize(iVtxCount);

	for (int i = 0; i < iVtxCount; ++i)
	{
		// 왼손좌표	: X, Y, Z 축이라면 
		// 3DMax축	: X, Z, Y 축이기 때문에
		// 인덱스 변경이 필요함
		pContainer->vecPos[i].x = pVtxPos[i].mData[0];
		pContainer->vecPos[i].y = pVtxPos[i].mData[2];
		pContainer->vecPos[i].z = pVtxPos[i].mData[1];
	}

	int	iPolygonCount = _pMesh->GetPolygonCount();

	// 재질 수를 얻어온다
	int	iMaterialCount = _pMesh->GetNode()->GetMaterialCount();

	pContainer->vecIndices.resize(iMaterialCount);

	UINT	iVtxID = 0;

	// 재질 정보를 얻어온다
	FbxGeometryElementMaterial*	pMaterial = _pMesh->GetElementMaterial();

	// 삼각형 수만큼 반복한다
	for (int i = 0; i < iPolygonCount; ++i)
	{
		int	iPolygonSize = _pMesh->GetPolygonSize(i);

		int iIdx[3] = {};

		for (int j = 0; j < iPolygonSize; ++j)
		{
			// 현재 삼각형을 구성하고 있는 버텍스정보 내에서의 인덱스를 구한다
			int	iControlIndex = _pMesh->GetPolygonVertex(i, j);

			iIdx[j] = iControlIndex;

			LoadNormal(_pMesh, pContainer, iVtxID, iControlIndex);
			LoadUV(_pMesh, pContainer, _pMesh->GetTextureUVIndex(i, j), iControlIndex);
			LoadTangent(_pMesh, pContainer, iVtxID, iControlIndex);
			LoadBinormal(_pMesh, pContainer, iVtxID, iControlIndex);

			++iVtxID;
		}

		int iMaterialID = pMaterial->GetIndexArray().GetAt(i);

		pContainer->vecIndices[iMaterialID].push_back(iIdx[0]);
		pContainer->vecIndices[iMaterialID].push_back(iIdx[2]);
		pContainer->vecIndices[iMaterialID].push_back(iIdx[1]);
	}

	return true;
}

void CFbxLoader::LoadNormal(FbxMesh * _pMesh, pFBXMESHCONTAINER _pContainer, int _iVtxID, int _iControlIndex)
{
	FbxGeometryElementNormal*	pNormal = _pMesh->GetElementNormal();

	int	iNormalIndex = _iVtxID;

	if (FbxGeometryElement::eByPolygonVertex == pNormal->GetMappingMode())
	{
		switch (pNormal->GetReferenceMode())
		{
		case FbxGeometryElement::eIndexToDirect:
			iNormalIndex = pNormal->GetIndexArray().GetAt(_iVtxID);
			break;
		}
	}
	else if (FbxGeometryElement::eByControlPoint == pNormal->GetMappingMode())
	{
		switch (pNormal->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
			iNormalIndex = _iControlIndex;
			break;
		case FbxGeometryElement::eIndexToDirect:
			iNormalIndex = pNormal->GetIndexArray().GetAt(_iControlIndex);
			break;
		}
	}

	FbxVector4	vNormal = pNormal->GetDirectArray().GetAt(iNormalIndex);

	_pContainer->vecNormal[_iControlIndex].x = vNormal.mData[0];
	_pContainer->vecNormal[_iControlIndex].y = vNormal.mData[2];
	_pContainer->vecNormal[_iControlIndex].z = vNormal.mData[1];
}

void CFbxLoader::LoadUV(FbxMesh * _pMesh, pFBXMESHCONTAINER _pContainer, int _iUVID, int _iControlIndex)
{
	FbxGeometryElementUV*	pUV = _pMesh->GetElementUV(0);

	int	iUVIndex = _iUVID;

	if (FbxGeometryElement::eByControlPoint == pUV->GetMappingMode())
	{
		switch (pUV->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
			iUVIndex = _iControlIndex;
			break;
		case FbxGeometryElement::eIndexToDirect:
			iUVIndex = pUV->GetIndexArray().GetAt(_iControlIndex);
			break;
		}
	}

	FbxVector2	vUV = pUV->GetDirectArray().GetAt(iUVIndex);

	_pContainer->vecUV[_iControlIndex].x = vUV.mData[0];
	_pContainer->vecUV[_iControlIndex].y = 1.0f - vUV.mData[1];
}

void CFbxLoader::LoadTangent(FbxMesh * _pMesh, pFBXMESHCONTAINER _pContainer, int _iVtxID, int _iControlIndex)
{
	FbxGeometryElementTangent*	pTangent = _pMesh->GetElementTangent();

	// Tangent, Binormal은 항상 있는 경우는 아니다
	// 범프매핑 관련 값이 있는경우에만 존재한다
	
	// pTangent가 NULL이라면
	if (NULL == pTangent)
	{
		// 더이상 불러오면 안된다.
		return;
	}

	// 범프매핑이 가능한 경우이기 때문에 true
	_pContainer->bBump = true;

	int	iTangentIndex = _iVtxID;

	if (FbxGeometryElement::eByPolygonVertex == pTangent->GetMappingMode())
	{
		switch (pTangent->GetReferenceMode())
		{
		case FbxGeometryElement::eIndexToDirect:
			iTangentIndex = pTangent->GetIndexArray().GetAt(_iVtxID);
			break;
		}
	}
	else if (FbxGeometryElement::eByControlPoint == pTangent->GetMappingMode())
	{
		switch (pTangent->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
			iTangentIndex = _iControlIndex;
			break;
		case FbxGeometryElement::eIndexToDirect:
			iTangentIndex = pTangent->GetIndexArray().GetAt(_iControlIndex);
			break;
		}
	}

	FbxVector4	vTangent = pTangent->GetDirectArray().GetAt(iTangentIndex);

	_pContainer->vecTangent[_iControlIndex].x = vTangent.mData[0];
	_pContainer->vecTangent[_iControlIndex].y = vTangent.mData[2];
	_pContainer->vecTangent[_iControlIndex].z = vTangent.mData[1];
}

void CFbxLoader::LoadBinormal(FbxMesh * _pMesh, pFBXMESHCONTAINER _pContainer, int _iVtxID, int _iControlIndex)
{
	FbxGeometryElementBinormal*	pBinormal= _pMesh->GetElementBinormal();

	// Tangent, Binormal은 항상 있는 경우는 아니다
	// 범프매핑 관련 값이 있는경우에만 존재한다

	// pBinormal가 NULL이라면
	if (NULL == pBinormal)
	{
		// 더이상 불러오면 안된다.
		return;
	}

	// 범프매핑이 가능한 경우이기 때문에 true
	_pContainer->bBump = true;

	int	iBinormalIndex = _iVtxID;

	if (FbxGeometryElement::eByPolygonVertex == pBinormal->GetMappingMode())
	{
		switch (pBinormal->GetReferenceMode())
		{
		case FbxGeometryElement::eIndexToDirect:
			iBinormalIndex = pBinormal->GetIndexArray().GetAt(_iVtxID);
			break;
		}
	}
	else if (FbxGeometryElement::eByControlPoint == pBinormal->GetMappingMode())
	{
		switch (pBinormal->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
			iBinormalIndex = _iControlIndex;
			break;
		case FbxGeometryElement::eIndexToDirect:
			iBinormalIndex = pBinormal->GetIndexArray().GetAt(_iControlIndex);
			break;
		}
	}

	FbxVector4	vBinormal = pBinormal->GetDirectArray().GetAt(iBinormalIndex);

	_pContainer->vecBinormal[_iControlIndex].x = vBinormal.mData[0];
	_pContainer->vecBinormal[_iControlIndex].y = vBinormal.mData[2];
	_pContainer->vecBinormal[_iControlIndex].z = vBinormal.mData[1];
}