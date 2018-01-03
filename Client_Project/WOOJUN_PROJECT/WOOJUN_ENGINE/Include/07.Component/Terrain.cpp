#include "Terrain.h"
#include "Renderer.h"
#include "Material.h"
#include "../03.Resource/ResMgr.h"
#include "../03.Resource/Mesh.h"
#include "../06.GameObject/GameObject.h"

WOOJUN_USING

CTerrain::CTerrain() :
	m_iVtxNumX(0),
	m_iVtxNumZ(0),
	m_iVtxSizeX(0),
	m_iVtxSizeZ(0)
{
}

CTerrain::CTerrain(const CTerrain & _Terrain) :
	CComponent(_Terrain)
{
	m_iVtxNumX = _Terrain.m_iVtxNumX;
	m_iVtxNumZ = _Terrain.m_iVtxNumZ;
	m_iVtxSizeX = _Terrain.m_iVtxSizeX;
	m_iVtxSizeZ = _Terrain.m_iVtxSizeZ;
}

CTerrain::~CTerrain()
{
}

bool CTerrain::CreateTerrain(const string & _strKey, UINT _iVtxNumX, UINT _iVtxNumZ, UINT _iVtxSizeX, UINT _iVtxSizeZ)
{
	m_vecPos.clear();

	m_iVtxNumX = _iVtxNumX;
	m_iVtxNumZ = _iVtxNumZ;
	m_iVtxSizeX = _iVtxSizeX;
	m_iVtxSizeZ = _iVtxSizeZ;
	
	// 정점 전체 NumX * NumZ만큼 reserve
	m_vecPos.reserve(m_iVtxNumX * m_iVtxNumZ);

	vector<VERTEXBUMP> vecVtxBump;
	vecVtxBump.reserve(m_iVtxNumX * m_iVtxNumZ);

	// 버텍스 버퍼 생성
	for (int i = 0; i < m_iVtxNumZ; ++i)
	{
		for (int j = 0; j < m_iVtxNumX; ++j)
		{
			VERTEXBUMP	tVtxBump = {};

			tVtxBump.vPos = DxVector3(j * m_iVtxSizeX, 0.0f, ((m_iVtxNumZ - 1) - i) * m_iVtxSizeZ);
			tVtxBump.vNormal = DxVector3(0.0f, 1.0f, 0.0f);
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

	return true;
}

void CTerrain::SetBaseTexture(const string & _strKey, TCHAR * _pFileName, const string & _strPathKey)
{
	CRenderer*	pRenderer = m_pGameObject->FindComponentFromTypeID<CRenderer>();

	CMaterial*	pMaterial = pRenderer->GetMaterial();
	pMaterial->SetDiffuseTexture("Linear", _strKey, _pFileName, _strPathKey);
	pMaterial->SetDiffuseRegister(0, 0);

	SAFE_RELEASE(pMaterial);
	SAFE_RELEASE(pRenderer);
}

void CTerrain::SetNormalTexture(const string & _strKey, TCHAR * _pFileName, const string & _strPathKey)
{
	CRenderer*	pRenderer = m_pGameObject->FindComponentFromTypeID<CRenderer>();

	CMaterial*	pMaterial = pRenderer->GetMaterial();
	pMaterial->SetNormalMapTexture("Linear", _strKey, _pFileName, _strPathKey);
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

bool CTerrain::Init()
{
	CRenderer*	pRenderer = m_pGameObject->AddComponent<CRenderer>("Terrain Renderer");
	pRenderer->SetShader(STANDARD_BUMP_SHADER);
	pRenderer->SetInputLayout("BumpInputLayout");

	SAFE_RELEASE(pRenderer);

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

