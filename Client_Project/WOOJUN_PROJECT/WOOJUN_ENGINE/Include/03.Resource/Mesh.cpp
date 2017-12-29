#include "Mesh.h"
#include "../Device.h"
#include "../07.Component/Material.h"
#include "FbxLoader.h"

WOOJUN_USING

UINT CMesh::GetContainerCount() const
{
	return m_vecMeshContainer.size();
}

UINT CMesh::GetSubsetCount(int _iContainer) const
{
	return m_vecMeshContainer[_iContainer]->vecIndexBuffer.size();
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

	return true;
}

bool CMesh::LoadMesh(const string & _strKey, const wchar_t * _pFileName, const string & _strPathKey)
{
	CFbxLoader	Loader;

	if (false == Loader.LoadFBX(_pFileName, _strPathKey))
	{
		return false;
	}

	return ConvertFbxData(&Loader);
}

bool CMesh::LoadMesh(const string & _strKey, const char * _pFileName, const string & _strPathKey)
{
	CFbxLoader	Loader;

	if (false == Loader.LoadFBX(_pFileName, _strPathKey))
	{
		return false;
	}

	return ConvertFbxData(&Loader);
}

bool CMesh::LoadMeshFromFullPath(const string & _strKey, const wchar_t * _pFullPath)
{
	CFbxLoader	Loader;

	if (false == Loader.LoadFBXFullPath(_pFullPath))
	{
		return false;
	}

	return ConvertFbxData(&Loader);
}

bool CMesh::LoadMeshFromFullPath(const string & _strKey, const char * _pFullPath)
{
	CFbxLoader	Loader;

	if (false == Loader.LoadFBXFullPath(_pFullPath))
	{
		return false;
	}

	return ConvertFbxData(&Loader);
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

bool CMesh::CreateVertexBuffer(void * _pVertices, unsigned int _iVtxCount, unsigned int _iVtxSize, D3D11_USAGE _eVtxUsage, D3D11_PRIMITIVE_TOPOLOGY _ePrimitive, pMESHCONTAINER _pContainer)
{	
	pVERTEXBUFFER	pVtxBuf = new VERTEXBUFFER();

	_pContainer->pVtxBuffer = pVtxBuf;

	pVtxBuf->iCount = _iVtxCount;
	pVtxBuf->iSize = _iVtxSize;
	pVtxBuf->eUsage = _eVtxUsage;
	pVtxBuf->ePrimitive = _ePrimitive;

	D3D11_BUFFER_DESC tDesc = {};
	tDesc.ByteWidth = _iVtxSize * _iVtxCount;
	tDesc.Usage = _eVtxUsage;
	if (D3D11_USAGE_DYNAMIC == _eVtxUsage)
	{
		tDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}

	tDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	tDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA tData = {};
	tData.pSysMem = _pVertices;

	if (FAILED(DEVICE->CreateBuffer(&tDesc, &tData, &pVtxBuf->pBuffer)))
	{
		return false;
	}

	return true;
}

bool CMesh::CreateIndexBuffer(void * pIndices, unsigned int _iIdxCount, unsigned int _iIdxSize, D3D11_USAGE _eIdxUsage, DXGI_FORMAT _eFormat, pMESHCONTAINER _pContainer)
{
	pINDEXBUFFER	pIdxBuf = new INDEXBUFFER();

	_pContainer->vecIndexBuffer.push_back(pIdxBuf);

	pIdxBuf->iCount = _iIdxCount;
	pIdxBuf->iSize = _iIdxSize;
	pIdxBuf->eUsage = _eIdxUsage;
	pIdxBuf->eFormat = _eFormat;

	D3D11_BUFFER_DESC tDesc = {};
	tDesc.ByteWidth = _iIdxCount * _iIdxSize;
	tDesc.Usage = _eIdxUsage;
	if (_eIdxUsage == D3D11_USAGE_DYNAMIC)
	{
		tDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}

	tDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	tDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA tData = {};
	tData.pSysMem = pIndices;

	if (FAILED(DEVICE->CreateBuffer(&tDesc, &tData, &pIdxBuf->pBuffer)))
	{
		return false;
	}

	return true;
}

void CMesh::SubDivide(vector<VERTEXCOLOR>* _pVertex, vector<unsigned int>* _pIndex)
{
	vector<VERTEXCOLOR>	vecCopyVertex = *_pVertex;
	vector<unsigned int> vecCopyIndex = *_pIndex;

	_pVertex->resize(0);
	_pIndex->resize(0);

	unsigned int numTris = vecCopyIndex.size() / 3;
	for (unsigned int i = 0; i < numTris; ++i)
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

void CMesh::SubDividePos(vector<VERTEXPOS>* _pVertex, vector<unsigned int>* _pIndex)
{
	vector<VERTEXPOS>	vecCopyVertex = *_pVertex;
	vector<UINT> vecCopyIndex = *_pIndex;

	_pVertex->resize(0);
	_pIndex->resize(0);

	unsigned int numTris = vecCopyIndex.size() / 3;
	for (unsigned int i = 0; i < numTris; ++i)
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

		// 정점정보를 얻어와서 버텍스버퍼를 만든다
		if ((*iter)->bBump)
		{
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

			if (false == CreateVertexBuffer(&vecVtx[0], vecVtx.size(), sizeof(VERTEXBUMP), D3D11_USAGE_DEFAULT, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, pContainer))
			{
				return false;
			}
		}
		else
		{
			vector<VERTEXBUMP>	vecVtx;

			size_t Count = (*iter)->vecPos.size();
			for (size_t i = 0; i < Count; ++i)
			{
				VERTEXBUMP	tVtx = {};

				tVtx.vPos = (*iter)->vecPos[i];
				tVtx.vNormal = (*iter)->vecNormal[i];
				tVtx.vUV = (*iter)->vecUV[i];

				vecVtx.push_back(tVtx);
			}

			if (false == CreateVertexBuffer(&vecVtx[0], vecVtx.size(), sizeof(VERTEXNORMALTEXTURE), D3D11_USAGE_DEFAULT, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, pContainer))
			{
				return false;
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

	return true;
}

CMesh::CMesh()
{
}

CMesh::~CMesh()
{
	for (size_t i = 0; i < m_vecMeshContainer.size(); ++i)
	{		
		SAFE_RELEASE(m_vecMeshContainer[i]->pVtxBuffer->pBuffer);
		SAFE_DELETE(m_vecMeshContainer[i]->pVtxBuffer);

		Safe_Release_VecList(m_vecMeshContainer[i]->vecMaterial);

		for (size_t j = 0; j < m_vecMeshContainer[i]->vecIndexBuffer.size(); ++j)
		{			
			SAFE_RELEASE(m_vecMeshContainer[i]->vecIndexBuffer[j]->pBuffer);
		}

		Safe_Delete_VecList(m_vecMeshContainer[i]->vecIndexBuffer);
	}

	Safe_Delete_VecList(m_vecMeshContainer);
}
