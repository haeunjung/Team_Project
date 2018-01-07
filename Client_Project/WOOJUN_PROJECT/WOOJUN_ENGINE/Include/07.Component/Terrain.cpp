#include "Terrain.h"
#include "Renderer.h"
#include "Material.h"
#include "../01.Core/PathMgr.h"
#include "../03.Resource/ResMgr.h"
#include "../03.Resource/Mesh.h"
#include "../06.GameObject/GameObject.h"
#include "../07.Component/ColliderTerrain.h"

WOOJUN_USING

CTerrain::CTerrain() :
	m_iVtxNumX(0),
	m_iVtxNumZ(0),
	m_iVtxSizeX(0),
	m_iVtxSizeZ(0)
{
	m_eComponentType = CT_TERRAIN;
	SetTag("Terrain");
	SetTypeName("CTerrain");
	SetTypeID<CTerrain>();

	m_tTerrainCBuffer.iSplatCount = 0;
}

CTerrain::CTerrain(const CTerrain & _Terrain) :
	CComponent(_Terrain)
{
	m_iVtxNumX = _Terrain.m_iVtxNumX;
	m_iVtxNumZ = _Terrain.m_iVtxNumZ;
	m_iVtxSizeX = _Terrain.m_iVtxSizeX;
	m_iVtxSizeZ = _Terrain.m_iVtxSizeZ;

	m_tTerrainCBuffer.iSplatCount = _Terrain.m_tTerrainCBuffer.iSplatCount;
}

CTerrain::~CTerrain()
{
}

const DxVector3 & CTerrain::CreateNormal(const DxVector3 & _StartPos, const DxVector3 & _XPos, const DxVector3 & _ZPos)
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	DxVector3 vec1 = _XPos - _StartPos;
	DxVector3 vec2 = _ZPos - _StartPos;

	return vec1.Cross(vec2);
}

bool CTerrain::CreateTerrain(const string & _strKey, UINT _iVtxNumX, UINT _iVtxNumZ, UINT _iVtxSizeX, UINT _iVtxSizeZ,
	char* pHeightMap /*= NULL*/, const string& _strPathKey /*= TEXTUREPATH*/)
{
	m_vecPos.clear();

	m_iVtxNumX = _iVtxNumX;
	m_iVtxNumZ = _iVtxNumZ;
	m_iVtxSizeX = _iVtxSizeX;
	m_iVtxSizeZ = _iVtxSizeZ;
	
	// 정점 전체 NumX * NumZ만큼 reserve
	m_vecPos.reserve(m_iVtxNumX * m_iVtxNumZ);

	unsigned char*	pData = NULL;
	vector<float>	vecY;

	if (NULL != pHeightMap)
	{
		// 높이맵 풀경로 생성
		const char* pPath = GET_SINGLE(CPathMgr)->FindPathToMultiByte(_strPathKey);
		string strPath;

		if (NULL != pPath)
		{
			strPath = pPath;
		}

		strPath += pHeightMap;

		FILE*	pFile = NULL;

		fopen_s(&pFile, strPath.c_str(), "rb");

		if (NULL != pFile)
		{
			BITMAPFILEHEADER	tfh;
			BITMAPINFOHEADER	tih;

			fread(&tfh, sizeof(tfh), 1, pFile);			
			fread(&tih, sizeof(tih), 1, pFile);

			m_vecPos.reserve(tih.biWidth * tih.biHeight);

			int iByte = tih.biBitCount / 8;
			pData = new unsigned char[tih.biWidth * tih.biHeight * iByte];

			fread(pData, 1, tih.biWidth * tih.biHeight * iByte, pFile);

			m_iVtxNumX = tih.biWidth;
			m_iVtxNumZ = tih.biHeight;

			for (int i = 0; i < m_iVtxNumZ; ++i)
			{
				for (int j = 0; j < m_iVtxNumX; ++j)
				{
					float	fY = *(pData + (i * m_iVtxNumX + j) * iByte);

					fY /= 50.0f;

					vecY.push_back(fY);
				}
			}

			SAFE_DELETE_ARR(pData);

			fclose(pFile);
		}
	}

	vector<VERTEXBUMP> vecVtxBump;
	vecVtxBump.reserve(m_iVtxNumX * m_iVtxNumZ);

	int Index = 0;
	// 버텍스 버퍼 생성
	for (int i = 0; i < m_iVtxNumZ; ++i)
	{
		for (int j = 0; j < m_iVtxNumX; ++j)
		{
			
			VERTEXBUMP	tVtxBump = {};

			tVtxBump.vPos = DxVector3(j * m_iVtxSizeX, 0.0f, ((m_iVtxNumZ - 1) - i) * m_iVtxSizeZ);

			if (false == vecY.empty())
			{
				tVtxBump.vPos.y = vecY[i * m_iVtxNumX + j];
			}

			m_vecPos.push_back(tVtxBump.vPos);
			++Index;

			if (0 == j || 0 == i)
			{
				tVtxBump.vNormal = DxVector3(0.0f, 1.0f, 0.0f);
			}
			else
			{
				tVtxBump.vNormal = CreateNormal(tVtxBump.vPos, m_vecPos[Index - 2], m_vecPos[Index - m_iVtxNumX - 1]);
				//tVtxBump.vNormal = DxVector3(0.0f, 1.0f, 0.0f);
			}
			
			tVtxBump.vUV = DxVector2(j / (float)(m_iVtxNumX - 1), i / (float)(m_iVtxNumZ - 1));
			tVtxBump.vTangent = DxVector3(1.0f, 0.0f, 0.0f);
			tVtxBump.vBinormal = DxVector3(0.0f, 0.0f, -1.0f);

			vecVtxBump.push_back(tVtxBump);
		}
	}
	
	// 인덱스 버퍼 생성
	vector<UINT>	vecIndex;

	for (int i = 0; i < m_iVtxNumZ - 1; ++i)
	{
		for (int j = 0; j < m_iVtxNumX - 1; ++j)
		{
			int	iAddr = i * m_iVtxNumX + j;

			// 우상단 삼각형
			vecIndex.push_back(iAddr);
			vecIndex.push_back(iAddr + 1);
			vecIndex.push_back(iAddr + m_iVtxNumX + 1);

			// 좌하단 삼각형
			vecIndex.push_back(iAddr);
			vecIndex.push_back(iAddr + m_iVtxNumX + 1);
			vecIndex.push_back(iAddr + m_iVtxNumX);
		}
	}

	CMesh*	pMesh = GET_SINGLE(CResMgr)->CreateMesh(_strKey, &vecVtxBump[0], vecVtxBump.size(), sizeof(VERTEXBUMP),
		D3D11_USAGE_DEFAULT, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,	&vecIndex[0], vecIndex.size(), sizeof(UINT));

	CRenderer*	pRenderer = m_pGameObject->FindComponentFromTypeID<CRenderer>();
	pRenderer->SetMesh(pMesh);

	SAFE_RELEASE(pRenderer);
	SAFE_RELEASE(pMesh);

	/*CColliderTerrain*	pTerrainCol = m_pGameObject->AddComponent<CColliderTerrain>("TerrainCol");
	pTerrainCol->SetTerrainInfo(m_vecPos, m_iVtxNumX, m_iVtxNumZ);
	SAFE_RELEASE(pTerrainCol);*/

	return true;
}

void CTerrain::SetBaseTexture(const string & _strKey, TCHAR * _pFileName, const string & _strPathKey)
{
	CRenderer*	pRenderer = m_pGameObject->FindComponentFromTypeID<CRenderer>();

	CMaterial*	pMaterial = pRenderer->GetMaterial();
	pMaterial->SetDiffuseTexture("Linear", _strKey, _pFileName, _strPathKey);
	//pMaterial->SetDiffuseRegister(0, 0);

	SAFE_RELEASE(pMaterial);
	SAFE_RELEASE(pRenderer);
}

void CTerrain::SetNormalTexture(const string & _strKey, TCHAR * _pFileName, const string & _strPathKey)
{
	CRenderer*	pRenderer = m_pGameObject->FindComponentFromTypeID<CRenderer>();

	CMaterial*	pMaterial = pRenderer->GetMaterial();
	pMaterial->SetNormalMapTexture("Point", _strKey, _pFileName, _strPathKey);
	pMaterial->SetNormalMapRegister(1, 1);

	SAFE_RELEASE(pMaterial);
	SAFE_RELEASE(pRenderer);
}

void CTerrain::SetSpecularTexture(const string & _strKey, TCHAR * _pFileName, const string & _strPathKey)
{
	CRenderer*	pRenderer = m_pGameObject->FindComponentFromTypeID<CRenderer>();

	CMaterial*	pMaterial = pRenderer->GetMaterial();
	pMaterial->SetSpecularTexture("Linear", _strKey, _pFileName, _strPathKey);
	pMaterial->SetSpecularRegister(2, 2);

	SAFE_RELEASE(pMaterial);
	SAFE_RELEASE(pRenderer);
}

void CTerrain::SetSplatTexture(const string & _strKey, vector<wstring> _vecFileName, const string & _strPathKey)
{
	CRenderer*	pRenderer = m_pGameObject->FindComponentFromTypeID<CRenderer>();

	CMaterial*	pMaterial = pRenderer->GetMaterial();
	pMaterial->AddDiffuseMultiTexture("Linear", _strKey, 11, 11, CUT_PIXEL, _vecFileName, _strPathKey);

	SAFE_RELEASE(pMaterial);
	SAFE_RELEASE(pRenderer);
}

void CTerrain::SetSplatNormalTexture(const string & _strKey, vector<wstring> _vecFileName, const string & _strPathKey)
{
	CRenderer*	pRenderer = m_pGameObject->FindComponentFromTypeID<CRenderer>();

	CMaterial*	pMaterial = pRenderer->GetMaterial();
	pMaterial->AddNormalMultiTexture("Point", _strKey, 12, 12, CUT_PIXEL, _vecFileName, _strPathKey);

	SAFE_RELEASE(pMaterial);
	SAFE_RELEASE(pRenderer);
}

void CTerrain::SetSplatSpecularTexture(const string & _strKey, vector<wstring> _vecFileName, const string & _strPathKey)
{
	CRenderer*	pRenderer = m_pGameObject->FindComponentFromTypeID<CRenderer>();

	CMaterial*	pMaterial = pRenderer->GetMaterial();
	pMaterial->AddSpecularMultiTexture("Linear", _strKey, 13, 13, CUT_PIXEL, _vecFileName, _strPathKey);

	SAFE_RELEASE(pMaterial);
	SAFE_RELEASE(pRenderer);
}

void CTerrain::SetSplatAlphaTexture(const string & _strKey, vector<wstring> _vecFileName, const string & _strPathKey)
{
	CRenderer*	pRenderer = m_pGameObject->FindComponentFromTypeID<CRenderer>();

	CMaterial*	pMaterial = pRenderer->GetMaterial();
	pMaterial->AddAlphaMultiTexture("Point", _strKey, 14, 14, CUT_PIXEL, _vecFileName, _strPathKey);

	SAFE_RELEASE(pMaterial);
	SAFE_RELEASE(pRenderer);
}

void CTerrain::SetSplatCount(int _iCount)
{
	m_tTerrainCBuffer.iSplatCount = _iCount;
}

void CTerrain::SetDetailLevel(float _fLevel)
{
	m_fDetailLevel = _fLevel;
}

bool CTerrain::Init()
{
	CRenderer*	pRenderer = m_pGameObject->AddComponent<CRenderer>("Terrain Renderer");

	assert(pRenderer);

	pRenderer->SetShader(TERRAIN_SHADER);
	pRenderer->SetInputLayout("BumpInputLayout");
	pRenderer->AddConstBuffer("Terrain", 10, sizeof(TERRAINCBUFFER), CUT_VERTEX | CUT_PIXEL);

	SAFE_RELEASE(pRenderer);

	m_fDetailLevel = 16.0f;

	return true;
}

void CTerrain::Input(float _fTime)
{
}

void CTerrain::Update(float _fTime)
{
}

void CTerrain::LateUpdate(float _fTime)
{
	// Terrain은 Renderer2D가 있을 수 없다

	m_tTerrainCBuffer.fDetailLevel = m_fDetailLevel;

	CRenderer*	pRenderer = m_pGameObject->FindComponentFromTypeID<CRenderer>();
	assert(pRenderer);

	pRenderer->UpdateCBuffer("Terrain", &m_tTerrainCBuffer);

	SAFE_RELEASE(pRenderer);
}

void CTerrain::Collision(float _fTime)
{
}

void CTerrain::Render(float _fTime)
{
}

CTerrain * CTerrain::Clone()
{
	return new CTerrain(*this);
}

void CTerrain::OnCollisionEnter(CCollider * _pSrc, CCollider * _pDest, float _fTime)
{
}

void CTerrain::OnCollisionStay(CCollider * _pSrc, CCollider * _pDest, float _fTime)
{
}

void CTerrain::OnCollisionLeave(CCollider * _pSrc, CCollider * _pDest, float _fTime)
{
}

