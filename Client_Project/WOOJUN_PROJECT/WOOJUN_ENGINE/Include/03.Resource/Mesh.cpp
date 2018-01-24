#include "Mesh.h"
#include "FbxLoader.h"
#include "Texture.h"
#include "Sampler.h"
#include "../Device.h"
#include "../01.Core/PathMgr.h"
#include "../03.Resource/ResMgr.h"
#include "../07.Component/Material.h"
#include "../07.Component/Animation3D.h"

WOOJUN_USING

DxVector3 CMesh::GetMeshMin() const
{
	return m_vMin;
}

DxVector3 CMesh::GetMeshMax() const
{
	return m_vMax;
}

DxVector3 CMesh::GetMeshSize() const
{
	return m_vSize;
}

SPHEREINFO CMesh::GetSphereInfo() const
{
	return m_tSphere;
}

DxVector3 CMesh::GetCenter() const
{
	return m_tSphere.vCenter;
}

float CMesh::GetRadius() const
{
	return m_tSphere.fRadius;
}

UINT CMesh::GetContainerCount() const
{
	return m_vecMeshContainer.size();
}

UINT CMesh::GetSubsetCount(int _iContainer) const
{
	return m_vecMeshContainer[_iContainer]->vecIndexBuffer.size();
}

CAnimation3D * CMesh::CloneAnimation() const
{
	if (!m_pAnimation)
	{
		return NULL;
	}

	return m_pAnimation->Clone();
}

CAnimation3D * CMesh::GetAnimation() const
{
	if (!m_pAnimation)
	{
		return NULL;
	}

	m_pAnimation->AddRef();
	return m_pAnimation;
}

bool CMesh::CreateMesh(const string & _strKey, void * _pVertices, unsigned int _iVtxCount, unsigned int _iVtxSize, D3D11_USAGE _eVtxUsage, D3D11_PRIMITIVE_TOPOLOGY _ePrimitive,
	void* _pIndices /*= NULL*/,
	unsigned int _iIdxCount /*= 0*/,
	unsigned int _iIdxSize /*= 0*/,
	D3D11_USAGE _eIdxUsage /*= D3D11_USAGE_DEFAULT*/,
	DXGI_FORMAT _eFormat /*= DXGI_FORMAT_R32_UINT*/)
{
	m_strKey = _strKey;

	pMESHCONTAINER	pContainer = new MESHCONTAINER();

	m_vecMeshContainer.push_back(pContainer);

	if (false == CreateVertexBuffer(_pVertices, _iVtxCount, _iVtxSize, _eVtxUsage, _ePrimitive, pContainer))
	{
		return false;
	}

	if (NULL != _pIndices)
	{
		if (false == CreateIndexBuffer(_pIndices, _iIdxCount, _iIdxSize, _eIdxUsage, _eFormat, pContainer))
		{
			return false;
		}
	}	

	m_vSize = m_vMax - m_vMin;
	m_tSphere.vCenter = (m_vMin + m_vMax) / 2.0f;
	m_tSphere.fRadius = m_vSize.Length() / 2.0f;

	return true;
}

bool CMesh::CreateSphere(const string & _strKey, float _fRadius, unsigned int _iNumSub, const DxVector4 & _vColor)
{
	vector<VERTEXCOLOR> vecVertex;
	vector<unsigned int> vecIndex;

	_iNumSub = min(_iNumSub, 5u);

	const float X = 0.525731f;
	const float Z = 0.850651f;

	XMFLOAT3 pos[12] =
	{
		XMFLOAT3(-X, 0.0f, Z),  XMFLOAT3(X, 0.0f, Z),
		XMFLOAT3(-X, 0.0f, -Z), XMFLOAT3(X, 0.0f, -Z),
		XMFLOAT3(0.0f, Z, X),   XMFLOAT3(0.0f, Z, -X),
		XMFLOAT3(0.0f, -Z, X),  XMFLOAT3(0.0f, -Z, -X),
		XMFLOAT3(Z, X, 0.0f),   XMFLOAT3(-Z, X, 0.0f),
		XMFLOAT3(Z, -X, 0.0f),  XMFLOAT3(-Z, -X, 0.0f)
	};

	DWORD k[60] =
	{
		1,4,0,  4,9,0,  4,5,9,  8,5,4,  1,8,4,
		1,10,8, 10,3,8, 8,3,5,  3,2,5,  3,7,2,
		3,10,7, 10,6,7, 6,11,7, 6,0,11, 6,1,0,
		10,1,6, 11,0,9, 2,11,9, 5,2,9,  11,2,7
	};

	vecVertex.resize(12);
	vecIndex.resize(60);

	for (unsigned int i = 0; i < 12; ++i)
	{
		vecVertex[i].vPos = pos[i];
	}

	for (unsigned int i = 0; i < 60; ++i)
	{
		vecIndex[i] = k[i];
	}

	for (unsigned int i = 0; i < _iNumSub; ++i)
	{
		SubDivide(&vecVertex, &vecIndex);
	}

	size_t Size = vecVertex.size();
	for (unsigned int i = 0; i < Size; ++i)
	{
		DxVector3 n = vecVertex[i].vPos.Normalize();
		DxVector3 p = n * _fRadius;

		vecVertex[i].vPos = p;
		vecVertex[i].vColor = _vColor;
	}

	pMESHCONTAINER	pContainer = new MESHCONTAINER();

	m_vecMeshContainer.push_back(pContainer);

	if (false == CreateVertexBuffer(&vecVertex[0], vecVertex.size(), sizeof(VERTEXCOLOR), D3D11_USAGE_DEFAULT, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, pContainer))
	{
		return false;
	}

	if (false == CreateIndexBuffer(&vecIndex[0], vecIndex.size(), sizeof(unsigned int), D3D11_USAGE_DEFAULT, DXGI_FORMAT_R32_UINT, pContainer))
	{
		return false;
	}

	m_vSize = m_vMax - m_vMin;
	m_tSphere.vCenter = (m_vMin + m_vMax) / 2.0f;
	m_tSphere.fRadius = m_vSize.Length() / 2.0f;

	return true;
}

bool CMesh::CreateSphere(const string & _strKey, float _fRadius, unsigned int _iNumSub)
{
	vector<VERTEXPOS> vecVertex;
	vector<unsigned int> vecIndex;

	_iNumSub = min(_iNumSub, 5u);

	const float X = 0.525731f;
	const float Z = 0.850651f;

	XMFLOAT3 pos[12] =
	{
		XMFLOAT3(-X, 0.0f, Z),  XMFLOAT3(X, 0.0f, Z),
		XMFLOAT3(-X, 0.0f, -Z), XMFLOAT3(X, 0.0f, -Z),
		XMFLOAT3(0.0f, Z, X),   XMFLOAT3(0.0f, Z, -X),
		XMFLOAT3(0.0f, -Z, X),  XMFLOAT3(0.0f, -Z, -X),
		XMFLOAT3(Z, X, 0.0f),   XMFLOAT3(-Z, X, 0.0f),
		XMFLOAT3(Z, -X, 0.0f),  XMFLOAT3(-Z, -X, 0.0f)
	};

	DWORD k[60] =
	{
		1,4,0,  4,9,0,  4,5,9,  8,5,4,  1,8,4,
		1,10,8, 10,3,8, 8,3,5,  3,2,5,  3,7,2,
		3,10,7, 10,6,7, 6,11,7, 6,0,11, 6,1,0,
		10,1,6, 11,0,9, 2,11,9, 5,2,9,  11,2,7
	};

	vecVertex.resize(12);
	vecIndex.resize(60);

	for (unsigned int i = 0; i < 12; ++i)
	{
		vecVertex[i].vPos = pos[i];
	}

	for (unsigned int i = 0; i < 60; ++i)
	{
		vecIndex[i] = k[i];
	}

	for (unsigned int i = 0; i < _iNumSub; ++i)
	{
		SubDividePos(&vecVertex, &vecIndex);
	}

	size_t Size = vecVertex.size();
	for (unsigned int i = 0; i < Size; ++i)
	{
		DxVector3 n = vecVertex[i].vPos.Normalize();
		DxVector3 p = n * _fRadius;

		vecVertex[i].vPos = p;
	}

	pMESHCONTAINER	pContainer = new MESHCONTAINER();

	m_vecMeshContainer.push_back(pContainer);

	if (false == CreateVertexBuffer(&vecVertex[0], vecVertex.size(), sizeof(VERTEXPOS), D3D11_USAGE_DEFAULT, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, pContainer))
	{
		return false;
	}

	if (false == CreateIndexBuffer(&vecIndex[0], vecIndex.size(), sizeof(unsigned int), D3D11_USAGE_DEFAULT, DXGI_FORMAT_R32_UINT, pContainer))
	{
		return false;
	}

	m_vSize = m_vMax - m_vMin;
	m_tSphere.vCenter = (m_vMin + m_vMax) / 2.0f;
	m_tSphere.fRadius = m_vSize.Length() / 2.0f;

	return true;
}

bool CMesh::LoadMesh(const string & _strKey, const wchar_t * _pFileName, const string & _strPathKey)
{
	// 확장자명 비교하여
	// FBX 파일인지
	// 자체포멧 파일인지

	char	strFileName[MAX_PATH] = {};
	WideCharToMultiByte(CP_ACP, 0, _pFileName, -1, strFileName, lstrlen(_pFileName), 0, 0);

	char	strExt[_MAX_EXT] = {};
	_splitpath_s(strFileName, 0, 0, 0, 0, 0, 0, strExt, _MAX_EXT);
	_strupr_s(strExt);

	if (0 == strcmp(strExt, ".FBX"))
	{
		CFbxLoader	Loader;

		if (!Loader.LoadFBX(_pFileName, _strPathKey))
		{
			return false;
		}

		return ConvertFbxData(&Loader);
	}	

	Load(strFileName);

	return true;
}

bool CMesh::LoadMesh(const string & _strKey, const char * _pFileName, const string & _strPathKey)
{
	// 확장자명 비교하여
	// FBX 파일인지
	// 자체포멧 파일인지

	char	strExt[_MAX_EXT] = {};
	_splitpath_s(_pFileName, 0, 0, 0, 0, 0, 0, strExt, _MAX_EXT);
	_strupr_s(strExt);

	if (0 == strcmp(strExt, ".FBX"))
	{
		CFbxLoader	Loader;

		if (!Loader.LoadFBX(_pFileName, _strPathKey))
		{
			return false;
		}

		return ConvertFbxData(&Loader);
	}

	Load(_pFileName);

	return true;
}

bool CMesh::LoadMeshFromFullPath(const string & _strKey, const wchar_t * _pFullPath)
{
	// 확장자명 비교하여
	// FBX 파일인지
	// 자체포멧 파일인지

	char	strFullPath[MAX_PATH] = {};
	WideCharToMultiByte(CP_ACP, 0, _pFullPath, -1, strFullPath, lstrlen(_pFullPath), 0, 0);

	char	strExt[_MAX_EXT] = {};
	_splitpath_s(strFullPath, 0, 0, 0, 0, 0, 0, strExt, _MAX_EXT);
	_strupr_s(strExt);

	if (0 == strcmp(strExt, ".FBX"))
	{
		CFbxLoader	Loader;

		if (!Loader.LoadFBXFullPath(_pFullPath))
		{
			return false;
		}

		return ConvertFbxData(&Loader);
	}

	Load(strFullPath);

	return true;
}

bool CMesh::LoadMeshFromFullPath(const string & _strKey, const char * _pFullPath)
{
	// 확장자명 비교하여
	// FBX 파일인지
	// 자체포멧 파일인지

	char	strExt[_MAX_EXT] = {};
	_splitpath_s(_pFullPath, 0, 0, 0, 0, 0, 0, strExt, _MAX_EXT);
	_strupr_s(strExt);

	if (0 == strcmp(strExt, ".FBX"))
	{
		CFbxLoader	Loader;

		if (!Loader.LoadFBXFullPath(_pFullPath))
		{
			return false;
		}

		return ConvertFbxData(&Loader);
	}

	Load(_pFullPath);

	return true;
}

void CMesh::Render()
{
	size_t ContainerSize = m_vecMeshContainer.size();
	for (size_t i = 0; i < ContainerSize; ++i)
	{
		pVERTEXBUFFER	pVtxBuf = m_vecMeshContainer[i]->pVtxBuffer;
		UINT			iStride = pVtxBuf->iSize;
		UINT			iOffset = 0;
		CONTEXT->IASetVertexBuffers(0, 1, &pVtxBuf->pBuffer, &iStride, &iOffset);
		CONTEXT->IASetPrimitiveTopology(pVtxBuf->ePrimitive);

		if (false == m_vecMeshContainer[i]->vecIndexBuffer.empty())
		{
			size_t SubsetSize = m_vecMeshContainer[i]->vecIndexBuffer.size();
			for (size_t j = 0; j < SubsetSize; j++)
			{
				pINDEXBUFFER	pIdxBuf = m_vecMeshContainer[i]->vecIndexBuffer[j];
				CONTEXT->IASetIndexBuffer(pIdxBuf->pBuffer, pIdxBuf->eFormat, 0);
				CONTEXT->DrawIndexed(pIdxBuf->iCount, 0, 0);
			}
		}
		else
		{
			CONTEXT->Draw(pVtxBuf->iCount, 0);
		}
	}
}

void CMesh::Render(int _iContainer, int _iSubset)
{	
	pVERTEXBUFFER	pVtxBuf = m_vecMeshContainer[_iContainer]->pVtxBuffer;
	UINT			iStride = pVtxBuf->iSize;
	UINT			iOffset = 0;
	CONTEXT->IASetVertexBuffers(0, 1, &pVtxBuf->pBuffer, &iStride, &iOffset);
	CONTEXT->IASetPrimitiveTopology(pVtxBuf->ePrimitive);

	if (false == m_vecMeshContainer[_iContainer]->vecIndexBuffer.empty())
	{	
		pINDEXBUFFER	pIdxBuf = m_vecMeshContainer[_iContainer]->vecIndexBuffer[_iSubset];
		CONTEXT->IASetIndexBuffer(pIdxBuf->pBuffer, pIdxBuf->eFormat, 0);
		CONTEXT->DrawIndexed(pIdxBuf->iCount, 0, 0);
	}
	else
	{
		CONTEXT->Draw(pVtxBuf->iCount, 0);
	}
}

CMaterial * CMesh::CloneMaterial(int _iContainer /*= 0*/, int _iSubset /*= 0*/)
{
	if (_iContainer < 0 || _iContainer >= m_vecMeshContainer.size())
	{
		return NULL;
	}

	pMESHCONTAINER	pContainer = m_vecMeshContainer[_iContainer];

	if (_iSubset < 0 || _iSubset >= pContainer->vecMaterial.size())
	{
		return NULL;
	}

	return pContainer->vecMaterial[_iSubset]->Clone();
}

void CMesh::Save(const char * _pFileName, const string & _strPathKey)
{
	// 풀경로를 만든다
	const char*	pPath = GET_SINGLE(CPathMgr)->FindPathToMultiByte(_strPathKey);

	string	strPath;
	if (pPath)
	{
		strPath = pPath;
	}

	strPath += _pFileName;

	return SaveFromFullPath(strPath.c_str());
}

void CMesh::SaveFromFullPath(const char * _pFileName)
{
	FILE*	pFile = NULL;
	
	fopen_s(&pFile, _pFileName, "wb");

	if (!pFile)
	{
		return;
	}

	// 컨테이너 수를 저장
	int	iCount = m_vecMeshContainer.size();

	fwrite(&iCount, 4, 1, pFile);

	for (int i = 0; i < iCount; ++i)
	{
		// 정점 수
		fwrite(&m_vecMeshContainer[i]->pVtxBuffer->iCount, 4, 1, pFile);

		// 정점 하나의 크기
		fwrite(&m_vecMeshContainer[i]->pVtxBuffer->iSize, 4, 1, pFile);

		// 위상구조
		fwrite(&m_vecMeshContainer[i]->pVtxBuffer->ePrimitive, 4, 1, pFile);

		// 용도
		fwrite(&m_vecMeshContainer[i]->pVtxBuffer->eUsage, 4, 1, pFile);

		// 정점 정보
		fwrite(m_vecMeshContainer[i]->pVtxBuffer->pData,
			m_vecMeshContainer[i]->pVtxBuffer->iSize,
			m_vecMeshContainer[i]->pVtxBuffer->iCount,
			pFile);

		// 서브셋 수
		int	iSubset = m_vecMeshContainer[i]->vecIndexBuffer.size();
		fwrite(&iSubset, 4, 1, pFile);

		for (int j = 0; j < iSubset; ++j)
		{
			// 인덱스 수 저장
			fwrite(&m_vecMeshContainer[i]->vecIndexBuffer[j]->iCount, 4, 1, pFile);

			// 인덱스 크기 저장
			fwrite(&m_vecMeshContainer[i]->vecIndexBuffer[j]->iSize, 4, 1, pFile);

			// 인덱스 포멧 저장
			fwrite(&m_vecMeshContainer[i]->vecIndexBuffer[j]->eFormat, 4, 1, pFile);

			// 인덱스 용도 저장
			fwrite(&m_vecMeshContainer[i]->vecIndexBuffer[j]->eUsage, 4, 1, pFile);

			// 현재 서브셋 인덱스 정보를 저장
			fwrite(m_vecMeshContainer[i]->vecIndexBuffer[j]->pData,
				m_vecMeshContainer[i]->vecIndexBuffer[j]->iSize,
				m_vecMeshContainer[i]->vecIndexBuffer[j]->iCount,
				pFile);
		}

		// 재질 정보 저장
		int	iMtrlCount = m_vecMeshContainer[i]->vecMaterial.size();
		fwrite(&iMtrlCount, 4, 1, pFile);

		for (int j = 0; j < iMtrlCount; ++j)
		{
			MATERIALINFO	tMtrlInfo = m_vecMeshContainer[i]->vecMaterial[j]->GetMaterialInfo();
			SKININFO	tBaseSkin = m_vecMeshContainer[i]->vecMaterial[j]->GetBaseSkin();

			// 재질 색상정보 저장
			fwrite(&tMtrlInfo, sizeof(MATERIALINFO), 1, pFile);

			bool	bTexture = false;

			if (tBaseSkin.pDiffuse)
			{
				bTexture = true;
				fwrite(&bTexture, 1, 1, pFile);
				SaveTexture(tBaseSkin.pDiffuse, pFile);
			}
			else
			{				
				fwrite(&bTexture, 1, 1, pFile);
			}

			bTexture = false;

			if (tBaseSkin.pNormal)
			{
				bTexture = true;				
				fwrite(&bTexture, 1, 1, pFile);
				SaveTexture(tBaseSkin.pNormal, pFile);
			}
			else
			{				
				fwrite(&bTexture, 1, 1, pFile);
			}

			bTexture = false;

			if (tBaseSkin.pSpecular)
			{
				bTexture = true;
				fwrite(&bTexture, 1, 1, pFile);
				SaveTexture(tBaseSkin.pSpecular, pFile);
			}
			else
			{
				fwrite(&bTexture, 1, 1, pFile);
			}
		}
	}

	fclose(pFile);

	if (m_pAnimation)
	{
		char	strAniPath[MAX_PATH] = {};
		memcpy(strAniPath, _pFileName, strlen(_pFileName));

		memset(strAniPath + (strlen(_pFileName) - 3), 0, 3);
		strcat_s(strAniPath, "anm");

		m_pAnimation->SaveFromFullPath(strAniPath);
	}
}

void CMesh::SaveTexture(_tagTexture* _pTexture, FILE * _pFile)
{
	string	strKey = _pTexture->pTexture->GetKey();
	wstring	pFullPath = _pTexture->pTexture->GetFullPath().c_str();

	WCHAR	strPath[MAX_PATH] = {};

	int	iCount = 0;
	for (int i = pFullPath.length() - 1; i >= 0; --i)
	{
		if (pFullPath[i] == '\\' || pFullPath[i] == '/')
		{
			++iCount;
		}

		if (2 == iCount)
		{
			memcpy(strPath, pFullPath.c_str() + (i + 1), sizeof(WCHAR) * (MAX_PATH - (i + 1)));
			break;
		}
	}

	int iLength = strKey.length();
	// 키값 저장
	fwrite(&iLength, 4, 1, _pFile);
	fwrite(strKey.c_str(), 1, iLength, _pFile);

	// 풀패스 저장
	iLength = lstrlen(strPath);
	fwrite(&iLength, 4, 1, _pFile);
	fwrite(strPath, 2, iLength, _pFile);

	// 패스 키 저장
	iLength = strlen(MESHPATH);
	fwrite(&iLength, 4, 1, _pFile);
	fwrite(MESHPATH, 1, iLength, _pFile);

	// 레지스터 저장
	fwrite(&_pTexture->iTextureRegister, 4, 1, _pFile);

	strKey = _pTexture->pSampler->GetKey();
	iLength = strKey.length();

	fwrite(&iLength, 4, 1, _pFile);
	fwrite(strKey.c_str(), 1, iLength, _pFile);

	fwrite(&_pTexture->iSamplerRegister, 4, 1, _pFile);
	fwrite(&_pTexture->iShaderConstantType, 4, 1, _pFile);
}

void CMesh::Load(const char * _pFileName, const string & _strPathKey)
{
	// 전체 경로를 만든다
	const char*	pPath = GET_SINGLE(CPathMgr)->FindPathToMultiByte(_strPathKey);

	string strPath;
	if (pPath)
	{
		strPath = pPath;
	}

	strPath += _pFileName;

	return LoadFromFullPath(strPath.c_str());
}

void CMesh::LoadFromFullPath(const char * _pFileName)
{
	FILE* pFile = NULL;

	fopen_s(&pFile, _pFileName, "rb");

	if (!pFile)
	{
		return;
	}

	for (size_t i = 0; i < m_vecMeshContainer.size(); ++i)
	{
		SAFE_DELETE_ARR(m_vecMeshContainer[i]->pVtxBuffer->pData);
		SAFE_RELEASE(m_vecMeshContainer[i]->pVtxBuffer->pBuffer);
		SAFE_DELETE(m_vecMeshContainer[i]->pVtxBuffer);

		Safe_Release_VecList(m_vecMeshContainer[i]->vecMaterial);

		for (size_t j = 0; j < m_vecMeshContainer[i]->vecIndexBuffer.size(); ++j)
		{
			SAFE_DELETE_ARR(m_vecMeshContainer[i]->vecIndexBuffer[j]->pData);
			SAFE_RELEASE(m_vecMeshContainer[i]->vecIndexBuffer[j]->pBuffer);
		}

		Safe_Delete_VecList(m_vecMeshContainer[i]->vecIndexBuffer);
	}

	Safe_Delete_VecList(m_vecMeshContainer);

	// 컨테이너 수 저장
	int	iCount = 0;

	fread(&iCount, 4, 1, pFile);

	for (int i = 0; i < iCount; ++i)
	{
		pMESHCONTAINER	pContainer = new MESHCONTAINER();

		int	iSize, iCount = 0;
		D3D11_USAGE	eUsage = (D3D11_USAGE)0;
		D3D11_PRIMITIVE_TOPOLOGY	ePrimitive = (D3D11_PRIMITIVE_TOPOLOGY)0;

		// 정점 수 저장
		fread(&iCount, 4, 1, pFile);

		// 정점 하나의 크기를 저장
		fread(&iSize, 4, 1, pFile);

		// 위상구조 저장
		fread(&ePrimitive, 4, 1, pFile);

		// 용도를 저장
		fread(&eUsage, 4, 1, pFile);

		void*	pData = new char[iSize * iCount];

		// 정점정보 저장
		fread(pData, iSize, iCount, pFile);

		// 버텍스 버퍼 생성
		CreateVertexBuffer(pData, iCount, iSize, eUsage, ePrimitive, pContainer);

		SAFE_DELETE_ARR(pData);

		// 서브셋 수 저장
		int	iSubset = 0;
		fread(&iSubset, 4, 1, pFile);

		for (int j = 0; j < iSubset; ++j)
		{
			// 인덱스 수 저장
			fread(&iCount, 4, 1, pFile);
			
			// 인덱스 크기 저장
			fread(&iSize, 4, 1, pFile);

			// 인덱스 포멧 저장
			DXGI_FORMAT	eFormat = (DXGI_FORMAT)0;
			fread(&eFormat, 4, 1, pFile);

			// 인덱스 용도 저장
			fread(&eUsage, 4, 1, pFile);

			void*	pData = new char[iSize * iCount];

			// 현재 서브셋 인덱스 정보 저장
			fread(pData, iSize, iCount, pFile);

			// 인덱스 버퍼 생성
			CreateIndexBuffer(pData, iCount, iSize, eUsage, eFormat, pContainer);

			SAFE_DELETE_ARR(pData);
		}

		// 재질 정보 저장
		int iMtrlCount = 0;
		fread(&iMtrlCount, 4, 1, pFile);

		for (int j = 0; j < iMtrlCount; ++j)
		{
			CMaterial*	pMaterial = new CMaterial();
			pMaterial->Init();

			MATERIALINFO	tMtrlInfo;
			SKININFO	tBaseSkin = {};

			// 재질 색상정보 저장
			fread(&tMtrlInfo, sizeof(MATERIALINFO), 1, pFile);

			pMaterial->SetMaterialInfo(tMtrlInfo);

			bool	bTexture = false;

			fread(&bTexture, 1, 1, pFile);
			if (bTexture)
			{
				tBaseSkin.pDiffuse = new TEXTURE();
				LoadTexture(tBaseSkin.pDiffuse, pFile);
			}

			fread(&bTexture, 1, 1, pFile);
			if (bTexture)
			{
				tBaseSkin.pNormal = new TEXTURE();
				LoadTexture(tBaseSkin.pNormal, pFile);
			}

			fread(&bTexture, 1, 1, pFile);
			if (bTexture)
			{
				tBaseSkin.pSpecular = new TEXTURE();
				LoadTexture(tBaseSkin.pSpecular, pFile);
			}

			pMaterial->SetBaseSkin(tBaseSkin);

			pContainer->vecMaterial.push_back(pMaterial);
		}

		m_vecMeshContainer.push_back(pContainer);
	}

	fclose(pFile);

	SAFE_RELEASE(m_pAnimation);

	m_pAnimation = new CAnimation3D();
	m_pAnimation->Init();

	char	strAniPath[MAX_PATH] = {};
	memcpy(strAniPath, _pFileName, strlen(_pFileName));

	memset(strAniPath + (strlen(_pFileName) - 3), 0, 3);
	strcat_s(strAniPath, "anm");

	if (!m_pAnimation->LoadFromFullPath(strAniPath))
	{
		SAFE_RELEASE(m_pAnimation);
	}
}

void CMesh::LoadTexture(_tagTexture * _pTexture, FILE * _pFile)
{
	char	strKey[256] = {};

	int	iLength = 0;

	fread(&iLength, 4, 1, _pFile);
	fread(strKey, 1, iLength, _pFile);

	WCHAR	strPath[MAX_PATH] = {};
	fread(&iLength, 4, 1, _pFile);
	fread(strPath, 2, iLength, _pFile);

	// 패스 키 저장
	char	strPathKey[256] = {};
	fread(&iLength, 4, 1, _pFile);
	fread(strPathKey, 1, iLength, _pFile);

	// 텍스쳐 로딩
	_pTexture->pTexture = GET_SINGLE(CResMgr)->LoadTexture(strKey, strPath, strPathKey);

	// 레지스터 저장
	fread(&_pTexture->iTextureRegister, 4, 1, _pFile);

	iLength = 0;
	memset(strKey, 0, 256);
	
	fread(&iLength, 4, 1, _pFile);
	fread(strKey, 1, iLength, _pFile);

	_pTexture->pSampler = GET_SINGLE(CResMgr)->FindSampler(strKey);

	fread(&_pTexture->iSamplerRegister, 4, 1, _pFile);
	fread(&_pTexture->iShaderConstantType, 4, 1, _pFile);
}

bool CMesh::CreateVertexBuffer(void * _pVertices, unsigned int _iVtxCount, unsigned int _iVtxSize, D3D11_USAGE _eVtxUsage, D3D11_PRIMITIVE_TOPOLOGY _ePrimitive, pMESHCONTAINER _pContainer)
{	
	pVERTEXBUFFER	pVtxBuf = new VERTEXBUFFER();

	// 정점 수만큼 반복하며 Min과 Max값을 구한다.
	for (int i = 0; i < _iVtxCount; ++i)
	{
		DxVector3	vPos;
		memcpy(&vPos, ((char*)_pVertices) + i * _iVtxSize,
			sizeof(DxVector3));
		if (m_vMin.x > vPos.x)
			m_vMin.x = vPos.x;

		if (m_vMax.x < vPos.x)
			m_vMax.x = vPos.x;

		if (m_vMin.y > vPos.y)
			m_vMin.y = vPos.y;

		if (m_vMax.y < vPos.y)
			m_vMax.y = vPos.y;

		if (m_vMin.z > vPos.z)
			m_vMin.z = vPos.z;

		if (m_vMax.z < vPos.z)
			m_vMax.z = vPos.z;
	}

	_pContainer->pVtxBuffer = pVtxBuf;

	pVtxBuf->iCount = _iVtxCount;
	pVtxBuf->iSize = _iVtxSize;
	pVtxBuf->eUsage = _eVtxUsage;
	pVtxBuf->ePrimitive = _ePrimitive;

	SAFE_DELETE_ARR(pVtxBuf->pData);

	D3D11_BUFFER_DESC tDesc = {};
	tDesc.ByteWidth = _iVtxSize * _iVtxCount;
	tDesc.Usage = _eVtxUsage;
	if (D3D11_USAGE_DYNAMIC == _eVtxUsage)
	{
		tDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}

	tDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	tDesc.StructureByteStride = 0;

	pVtxBuf->pData = new char[_iVtxSize * _iVtxCount];
	memcpy(pVtxBuf->pData, _pVertices, _iVtxSize * _iVtxCount);

	D3D11_SUBRESOURCE_DATA tData = {};
	tData.pSysMem = pVtxBuf->pData;

	if (FAILED(DEVICE->CreateBuffer(&tDesc, &tData, &pVtxBuf->pBuffer)))
	{
		return false;
	}

	return true;
}

bool CMesh::CreateIndexBuffer(void * _pIndices, unsigned int _iIdxCount, unsigned int _iIdxSize, D3D11_USAGE _eIdxUsage, DXGI_FORMAT _eFormat, pMESHCONTAINER _pContainer)
{
	pINDEXBUFFER	pIdxBuf = new INDEXBUFFER();

	_pContainer->vecIndexBuffer.push_back(pIdxBuf);

	pIdxBuf->iCount = _iIdxCount;
	pIdxBuf->iSize = _iIdxSize;
	pIdxBuf->eUsage = _eIdxUsage;
	pIdxBuf->eFormat = _eFormat;

	SAFE_DELETE_ARR(pIdxBuf->pData);

	D3D11_BUFFER_DESC tDesc = {};
	tDesc.ByteWidth = _iIdxCount * _iIdxSize;
	tDesc.Usage = _eIdxUsage;
	if (_eIdxUsage == D3D11_USAGE_DYNAMIC)
	{
		tDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}

	tDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	tDesc.StructureByteStride = 0;

	pIdxBuf->pData = new char[_iIdxSize * _iIdxCount];
	memcpy(pIdxBuf->pData, _pIndices, _iIdxSize * _iIdxCount);

	D3D11_SUBRESOURCE_DATA tData = {};
	tData.pSysMem = pIdxBuf->pData;

	if (FAILED(DEVICE->CreateBuffer(&tDesc, &tData, &pIdxBuf->pBuffer)))
	{
		return false;
	}

	return true;
}

void CMesh::SubDivide(vector<VERTEXCOLOR>* _pVertex, vector<UINT>* _pIndex)
{
	vector<VERTEXCOLOR>	vecCopyVertex = *_pVertex;
	vector<UINT> vecCopyIndex = *_pIndex;

	_pVertex->resize(0);
	_pIndex->resize(0);

	unsigned int numTris = vecCopyIndex.size() / 3;
	for (UINT i = 0; i < numTris; ++i)
	{
		VERTEXCOLOR v0 = vecCopyVertex[vecCopyIndex[i * 3 + 0]];
		VERTEXCOLOR v1 = vecCopyVertex[vecCopyIndex[i * 3 + 1]];
		VERTEXCOLOR v2 = vecCopyVertex[vecCopyIndex[i * 3 + 2]];

		//
		// Generate the midpoints.
		//

		VERTEXCOLOR m0, m1, m2;

		// For subdivision, we just care about the position component.  We derive the other
		// vertex components in CreateGeosphere.

		m0.vPos = DxVector3(
			0.5f*(v0.vPos.x + v1.vPos.x),
			0.5f*(v0.vPos.y + v1.vPos.y),
			0.5f*(v0.vPos.z + v1.vPos.z));

		m1.vPos = DxVector3(
			0.5f*(v1.vPos.x + v2.vPos.x),
			0.5f*(v1.vPos.y + v2.vPos.y),
			0.5f*(v1.vPos.z + v2.vPos.z));

		m2.vPos = XMFLOAT3(
			0.5f*(v0.vPos.x + v2.vPos.x),
			0.5f*(v0.vPos.y + v2.vPos.y),
			0.5f*(v0.vPos.z + v2.vPos.z));

		//
		// Add new geometry.
		//

		_pVertex->push_back(v0); // 0
		_pVertex->push_back(v1); // 1
		_pVertex->push_back(v2); // 2
		_pVertex->push_back(m0); // 3
		_pVertex->push_back(m1); // 4
		_pVertex->push_back(m2); // 5

		_pIndex->push_back(i * 6 + 0);
		_pIndex->push_back(i * 6 + 3);
		_pIndex->push_back(i * 6 + 5);

		_pIndex->push_back(i * 6 + 3);
		_pIndex->push_back(i * 6 + 4);
		_pIndex->push_back(i * 6 + 5);

		_pIndex->push_back(i * 6 + 5);
		_pIndex->push_back(i * 6 + 4);
		_pIndex->push_back(i * 6 + 2);

		_pIndex->push_back(i * 6 + 3);
		_pIndex->push_back(i * 6 + 1);
		_pIndex->push_back(i * 6 + 4);
	}
}

void CMesh::SubDividePos(vector<VERTEXPOS>* _pVertex, vector<UINT>* _pIndex)
{
	vector<VERTEXPOS>	vecCopyVertex = *_pVertex;
	vector<UINT> vecCopyIndex = *_pIndex;

	_pVertex->resize(0);
	_pIndex->resize(0);

	unsigned int numTris = vecCopyIndex.size() / 3;
	for (UINT i = 0; i < numTris; ++i)
	{
		VERTEXPOS v0 = vecCopyVertex[vecCopyIndex[i * 3 + 0]];
		VERTEXPOS v1 = vecCopyVertex[vecCopyIndex[i * 3 + 1]];
		VERTEXPOS v2 = vecCopyVertex[vecCopyIndex[i * 3 + 2]];

		//
		// Generate the midpoints.
		//

		VERTEXPOS m0, m1, m2;

		// For subdivision, we just care about the position component.  We derive the other
		// vertex components in CreateGeosphere.

		m0.vPos = DxVector3(
			0.5f*(v0.vPos.x + v1.vPos.x),
			0.5f*(v0.vPos.y + v1.vPos.y),
			0.5f*(v0.vPos.z + v1.vPos.z));

		m1.vPos = DxVector3(
			0.5f*(v1.vPos.x + v2.vPos.x),
			0.5f*(v1.vPos.y + v2.vPos.y),
			0.5f*(v1.vPos.z + v2.vPos.z));

		m2.vPos = XMFLOAT3(
			0.5f*(v0.vPos.x + v2.vPos.x),
			0.5f*(v0.vPos.y + v2.vPos.y),
			0.5f*(v0.vPos.z + v2.vPos.z));

		//
		// Add new geometry.
		//

		_pVertex->push_back(v0); // 0
		_pVertex->push_back(v1); // 1
		_pVertex->push_back(v2); // 2
		_pVertex->push_back(m0); // 3
		_pVertex->push_back(m1); // 4
		_pVertex->push_back(m2); // 5

		_pIndex->push_back(i * 6 + 0);
		_pIndex->push_back(i * 6 + 3);
		_pIndex->push_back(i * 6 + 5);

		_pIndex->push_back(i * 6 + 3);
		_pIndex->push_back(i * 6 + 4);
		_pIndex->push_back(i * 6 + 5);

		_pIndex->push_back(i * 6 + 5);
		_pIndex->push_back(i * 6 + 4);
		_pIndex->push_back(i * 6 + 2);

		_pIndex->push_back(i * 6 + 3);
		_pIndex->push_back(i * 6 + 1);
		_pIndex->push_back(i * 6 + 4);
	}
}

bool CMesh::ConvertFbxData(CFbxLoader * _pLoader)
{
	const vector<pFBXMESHCONTAINER>*	pMeshContainer = _pLoader->GetMeshContainer();

	vector<pFBXMESHCONTAINER>::const_iterator	iter = pMeshContainer->begin();
	vector<pFBXMESHCONTAINER>::const_iterator	iterEnd = pMeshContainer->end();

	for (iter; iter != iterEnd; ++iter)
	{
		pMESHCONTAINER	pContainer = new MESHCONTAINER();

		m_vecMeshContainer.push_back(pContainer);

		int	iVtxSize = 0;

		// 정점정보를 얻어와서 버텍스버퍼를 만든다

		// 애니메이션이 있는경우
		if ((*iter)->bAnimation)
		{
			// 애니메이션 있고 범프 있고
			if ((*iter)->bBump)
			{
				iVtxSize = sizeof(VERTEXANIBUMP);

				vector<VERTEXANIBUMP>	vecVtx;

				size_t Count = (*iter)->vecPos.size();
				for (size_t i = 0; i < Count; ++i)
				{
					VERTEXANIBUMP	tVtx = {};

					tVtx.vPos = (*iter)->vecPos[i];
					tVtx.vNormal = (*iter)->vecNormal[i];
					tVtx.vUV = (*iter)->vecUV[i];
					tVtx.vTangent = (*iter)->vecTangent[i];
					tVtx.vBinormal = (*iter)->vecBinormal[i];
					tVtx.vWeight = (*iter)->vecBlendWeight[i];
					tVtx.vIndices = (*iter)->vecBlendIndex[i];

					vecVtx.push_back(tVtx);
				}

				if (false == CreateVertexBuffer(&vecVtx[0], vecVtx.size(), iVtxSize,
					D3D11_USAGE_DEFAULT, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, pContainer))
				{
					return false;
				}
			}
			// 애니메이션 있고 범프 없고
			else
			{
				iVtxSize = sizeof(VERTEXANI);

				vector<VERTEXANI>	vecVtx;

				size_t Count = (*iter)->vecPos.size();
				for (size_t i = 0; i < Count; ++i)
				{
					VERTEXANI	tVtx = {};

					tVtx.vPos = (*iter)->vecPos[i];
					tVtx.vNormal = (*iter)->vecNormal[i];
					tVtx.vUV = (*iter)->vecUV[i];
					tVtx.vWeight = (*iter)->vecBlendWeight[i];
					tVtx.vIndices = (*iter)->vecBlendIndex[i];

					vecVtx.push_back(tVtx);
				}

				if (false == CreateVertexBuffer(&vecVtx[0], vecVtx.size(), iVtxSize,
					D3D11_USAGE_DEFAULT, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, pContainer))
				{
					return false;
				}
			}
		}
		// 애니메이션이 없는경우
		else
		{
			// 애니메이션 없고 범프 있는경우
			if ((*iter)->bBump)
			{
				iVtxSize = sizeof(VERTEXBUMP);

				vector<VERTEXBUMP>	vecVtx;

				size_t Count = (*iter)->vecPos.size();
				for (size_t i = 0; i < Count; ++i)
				{
					VERTEXBUMP	tVtx = {};

					tVtx.vPos = (*iter)->vecPos[i];
					tVtx.vNormal = (*iter)->vecNormal[i];
					tVtx.vUV = (*iter)->vecUV[i];
					tVtx.vTangent = (*iter)->vecTangent[i];
					tVtx.vBinormal = (*iter)->vecBinormal[i];

					vecVtx.push_back(tVtx);
				}

				if (false == CreateVertexBuffer(&vecVtx[0], vecVtx.size(), iVtxSize,
					D3D11_USAGE_DEFAULT, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, pContainer))
				{
					return false;
				}
			}
			// 애니메이션 없고 범프 없고
			else
			{
				iVtxSize = sizeof(VERTEXNORMALTEXTURE);

				vector<VERTEXNORMALTEXTURE>	vecVtx;

				size_t Count = (*iter)->vecPos.size();
				for (size_t i = 0; i < Count; ++i)
				{
					VERTEXNORMALTEXTURE	tVtx = {};

					tVtx.vPos = (*iter)->vecPos[i];
					tVtx.vNormal = (*iter)->vecNormal[i];
					tVtx.vUV = (*iter)->vecUV[i];

					vecVtx.push_back(tVtx);
				}

				if (false == CreateVertexBuffer(&vecVtx[0], vecVtx.size(), iVtxSize,
					D3D11_USAGE_DEFAULT, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, pContainer))
				{
					return false;
				}
			}
		}

		// 인덱스버퍼 생성
		for (size_t i = 0; i < (*iter)->vecIndices.size(); ++i)
		{
			if (!CreateIndexBuffer(&(*iter)->vecIndices[i][0],
				(*iter)->vecIndices[i].size(), 4,
				D3D11_USAGE_DEFAULT, DXGI_FORMAT_R32_UINT,
				pContainer))
				return false;
		}
	}

	// 재질 정보를 읽어온다
	const vector<vector<pFBXMATERIAL>>*	pMaterials = _pLoader->GetMaterials();

	vector<vector<pFBXMATERIAL>>::const_iterator	iterMaterial = pMaterials->begin();
	vector<vector<pFBXMATERIAL>>::const_iterator	iterMaterialEnd = pMaterials->end();

	int	iContainer = 0;
	for (iterMaterial; iterMaterial != iterMaterialEnd; ++iterMaterial, ++iContainer)
	{
		pMESHCONTAINER	pContainer = m_vecMeshContainer[iContainer];

		size_t Count = (*iterMaterial).size();
		for (size_t i = 0; i < Count; ++i)
		{
			pFBXMATERIAL	pFbxMaterial = (*iterMaterial)[i];

			CMaterial*		pMaterial = new CMaterial();

			if (false == pMaterial->Init())
			{
				SAFE_RELEASE(pMaterial);
				return NULL;
			}

			pMaterial->SetMaterialInfo(pFbxMaterial->vDiffuse, pFbxMaterial->vAmbient,
				pFbxMaterial->vSpecular, pFbxMaterial->vEmissive, pFbxMaterial->fShininess);

			// 디퓨즈 텍스쳐 이름 불러오기
			char	strName[MAX_PATH] = {};
			_splitpath_s(pFbxMaterial->strDiffuseTexture.c_str(), NULL, 0, NULL, 0, strName, MAX_PATH, NULL, 0);

			pMaterial->SetDiffuseTextureFromFullPath("Linear", strName, pFbxMaterial->strDiffuseTexture.c_str());
			pMaterial->SetDiffuseRegister(0, 0);

			// 노말과 스펙큘러 텍스쳐는 경우에따라 없을수도 있다
			if (!pFbxMaterial->strBumpTexture.empty())
			{
				// strName배열 멤셋
				memset(strName, 0, MAX_PATH);

				_splitpath_s(pFbxMaterial->strBumpTexture.c_str(), NULL, 0, NULL, 0, strName, MAX_PATH, NULL, 0);

				pMaterial->SetNormalMapTextureFromFullPath("Point", strName, pFbxMaterial->strBumpTexture.c_str());
				pMaterial->SetNormalMapRegister(1, 1);
			}

			if (!pFbxMaterial->strSpecularTexture.empty())
			{
				memset(strName, 0, MAX_PATH);

				_splitpath_s(pFbxMaterial->strSpecularTexture.c_str(), NULL, 0, NULL, 0, strName, MAX_PATH, NULL, 0);

				pMaterial->SetSpecularTextureFromFullPath("Point", strName, pFbxMaterial->strSpecularTexture.c_str());
				pMaterial->SetSpecularRegister(2, 2);
			}

			pContainer->vecMaterial.push_back(pMaterial);
		}
	}

	m_vSize = m_vMax - m_vMin;
	m_tSphere.vCenter = (m_vMin + m_vMax) / 2.0f;
	m_tSphere.fRadius = m_vSize.Length() / 2.0f;

	// 애니메이션
	const vector<pFBXBONE>* pvecBone = _pLoader->GetBones();
	const vector<pFBXANIMATIONCLIP>* pvecClip = _pLoader->GetClip();

	if (pvecBone->empty() || pvecClip->empty())
	{
		return true;
	}

	SAFE_RELEASE(m_pAnimation);

	m_pAnimation = new CAnimation3D();

	if (!m_pAnimation->Init())
	{
		SAFE_RELEASE(m_pAnimation);
		return false;
	}

	// 본 갯수만큼 반복
	vector<pFBXBONE>::const_iterator	iterB;
	vector<pFBXBONE>::const_iterator	iterBEnd = pvecBone->end();

	for (iterB = pvecBone->begin(); iterB != iterBEnd; ++iterB)
	{
		pBONE	pBone = new BONE();

		pBone->strName = (*iterB)->strName;
		pBone->iDepth = (*iterB)->iDepth;
		pBone->iParentIndex = (*iterB)->iParentIndex;

		float	fMat[4][4];

		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				fMat[i][j] = (*iterB)->matOffset.mData[i].mData[j];
			}
		}		

		pBone->matOffset = new MATRIX();
		*pBone->matOffset = fMat;

		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				fMat[i][j] = (*iterB)->matBone.mData[i].mData[j];
			}
		}

		pBone->matBone = new MATRIX();
		*pBone->matBone = fMat;

		pBone->vecKeyFrame.reserve((*iterB)->vecKeyFrame.size());

		for (int i = 0; i < (*iterB)->vecKeyFrame.size(); ++i)
		{
			pKEYFRAME	pKeyFrame = new KEYFRAME();
			pKeyFrame->dTime = (*iterB)->vecKeyFrame[i].dTime;

			FbxAMatrix	mat = (*iterB)->vecKeyFrame[i].matTransform;
			FbxVector4	vPos, vScale;
			FbxQuaternion	qRot;

			vPos = mat.GetT();
			vScale = mat.GetS();
			qRot = mat.GetQ();

			pKeyFrame->vScale = DxVector3(vScale.mData[0], vScale.mData[1], vScale.mData[2]);
			pKeyFrame->vPos = DxVector3(vPos.mData[0], vPos.mData[1], vPos.mData[2]);
			pKeyFrame->vRot = DxVector4(qRot.mData[0], qRot.mData[1], qRot.mData[2], qRot.mData[3]);

			pBone->vecKeyFrame.push_back(pKeyFrame);
		}	

		m_pAnimation->AddBone(pBone);
	}

	m_pAnimation->CreateBoneTexture();

	// 클립을 읽어온다
	vector<pFBXANIMATIONCLIP>::const_iterator	iterC;
	vector<pFBXANIMATIONCLIP>::const_iterator	iterCEnd = pvecClip->end();

	for (iterC = pvecClip->begin(); iterC != iterCEnd; ++iterC)
	{
		m_pAnimation->AddClip(AO_LOOP, *iterC);
	}

	return true;
}

CMesh::CMesh() :
	m_pAnimation(NULL)
{
}

CMesh::~CMesh()
{
	SAFE_RELEASE(m_pAnimation);

	for (size_t i = 0; i < m_vecMeshContainer.size(); ++i)
	{		
		SAFE_DELETE_ARR(m_vecMeshContainer[i]->pVtxBuffer->pData);
		SAFE_RELEASE(m_vecMeshContainer[i]->pVtxBuffer->pBuffer);
		SAFE_DELETE(m_vecMeshContainer[i]->pVtxBuffer);

		Safe_Release_VecList(m_vecMeshContainer[i]->vecMaterial);

		for (size_t j = 0; j < m_vecMeshContainer[i]->vecIndexBuffer.size(); ++j)
		{	
			SAFE_DELETE_ARR(m_vecMeshContainer[i]->vecIndexBuffer[j]->pData);
			SAFE_RELEASE(m_vecMeshContainer[i]->vecIndexBuffer[j]->pBuffer);
		}

		Safe_Delete_VecList(m_vecMeshContainer[i]->vecIndexBuffer);
	}

	Safe_Delete_VecList(m_vecMeshContainer);
}
