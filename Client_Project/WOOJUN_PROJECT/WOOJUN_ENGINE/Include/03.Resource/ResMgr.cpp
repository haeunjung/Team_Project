#include "ResMgr.h"
#include "Mesh.h"
#include "Sampler.h"
#include "Texture.h"

WOOJUN_USING

DEFINITION_SINGLE(CResMgr)

bool CResMgr::Init()
{	
	// Sample Color Triangle
	VERTEXCOLOR tVtx[3] = {
		VERTEXCOLOR(0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f),
		VERTEXCOLOR(1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f),
		VERTEXCOLOR(-1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f)
	};

	UINT iIdx[3] = { 0, 1, 2 };

	CMesh* pMesh = CreateMesh("ColorTriangle", tVtx, 3, sizeof(VERTEXCOLOR), D3D11_USAGE_DEFAULT, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
		iIdx, 3, sizeof(UINT));
	SAFE_RELEASE(pMesh);

	// Rectangle Mesh
	VERTEXCOLOR tVtxRect[4] = {
		VERTEXCOLOR(-1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f),
		VERTEXCOLOR(1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f),
		VERTEXCOLOR(1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f),
		VERTEXCOLOR(-1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f)
	};

	UINT iIdxRect[2][3] = { { 0, 1, 2 }, {0, 2, 3} };

	pMesh = CreateMesh("ColorRectangle", tVtxRect, 4, sizeof(VERTEXCOLOR), D3D11_USAGE_DEFAULT, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
		iIdxRect, 6, sizeof(UINT));
	SAFE_RELEASE(pMesh);

	// Triangle Pyramid
	VERTEXCOLOR tVtxBox[8] = {
		VERTEXCOLOR(-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f),
		VERTEXCOLOR(-0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f),
		VERTEXCOLOR(0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f),
		VERTEXCOLOR(0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f),
		VERTEXCOLOR(-0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 1.0f),
		VERTEXCOLOR(-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f),
		VERTEXCOLOR(0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 1.0f),
		VERTEXCOLOR(0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f),
	};

	UINT iBoxIdx[36] = { 1,6,5,
		1,2,6,
		2,7,6,
		2,3,7,
		3,4,7,
		3,0,4,
		0,5,4,
		0,1,5,
		0,2,1,
		0,3,2,
		4,5,6,
		4,6,7 };

	pMesh = CreateMesh("ColorBox", tVtxBox, 8, sizeof(VERTEXCOLOR), D3D11_USAGE_DEFAULT, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
		iBoxIdx, 36, sizeof(UINT));
	SAFE_RELEASE(pMesh);

	VERTEXCOLOR	tVtxPyramid[5] =
	{
		VERTEXCOLOR(0.f, 0.5f, 0.f, 1.f, 0.f, 0.f, 1.f),
		VERTEXCOLOR(-0.5f, -0.5f, 0.5f, 0.f, 1.f, 0.f, 1.f),
		VERTEXCOLOR(0.5f, -0.5f, 0.5f, 0.f, 0.f, 1.f, 1.f),
		VERTEXCOLOR(-0.5f, -0.5f, -0.5f, 1.f, 0.f, 1.f, 1.f),
		VERTEXCOLOR(0.5f, -0.5f, -0.5f, 0.f, 1.f, 1.f, 1.f)
	};

	UINT	iPyramidIdx[18] =
	{
		0, 4, 3, 0, 2, 4, 0, 1, 2, 0, 3, 1, 3, 4, 1, 4, 2, 1
	};

	pMesh = CreateMesh("ColorPyramid", tVtxPyramid, 5, sizeof(VERTEXCOLOR), D3D11_USAGE_DEFAULT, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
		iPyramidIdx, 18, sizeof(UINT));
		SAFE_RELEASE(pMesh);

	pMesh = CreateSphere("ColorSphere", 0.5f, 2, DxVector4(0.0f, 1.0f, 0.0f, 1.0f));
	SAFE_RELEASE(pMesh);

	pMesh = CreateSphere("PosSphere", 1.0f, 2);
	SAFE_RELEASE(pMesh);

	VERTEXNORMALTEXTURE	tVtxTexPyramid[8] =
	{
		VERTEXNORMALTEXTURE(0.f, 0.5f, 0.f, 0.f, 1.f, 0.f, 0.5f, 0.f),
		VERTEXNORMALTEXTURE(0.f, 0.5f, 0.f, 0.f, 1.f, 0.f, 1.f, 0.5f),
		VERTEXNORMALTEXTURE(0.f, 0.5f, 0.f, 0.f, 1.f, 0.f, 0.5f, 1.f),
		VERTEXNORMALTEXTURE(0.f, 0.5f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.5f),
		VERTEXNORMALTEXTURE(-0.5f, -0.5f, 0.5f, 0.f, 1.f, 0.f, 0.25f, 0.25f),
		VERTEXNORMALTEXTURE(0.5f, -0.5f, 0.5f, 0.f, 1.f, 0.f, 0.75f, 0.25f),
		VERTEXNORMALTEXTURE(-0.5f, -0.5f, -0.5f, 0.f, 1.f, 0.f, 0.25f, 0.75f),
		VERTEXNORMALTEXTURE(0.5f, -0.5f, -0.5f, 0.f, 1.f, 0.f, 0.75f, 0.75f)
	};

	UINT	iPyramidTexIdx[18] =
	{
		2, 7, 6, 3, 6, 4, 0, 4, 5, 1, 5, 7, 6, 7, 4, 7, 5, 4
	};

	pMesh = CreateMesh("TexNormalPyramid", tVtxTexPyramid, 8, sizeof(VERTEXNORMALTEXTURE),
		D3D11_USAGE_DEFAULT, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
		iPyramidTexIdx, 18, 4);
	SAFE_RELEASE(pMesh);

	VERTEXPOS	tVtxPos = VERTEXPOS(0, 0, 0);

	pMesh = CreateMesh("PosMesh", &tVtxPos, 1, sizeof(VERTEXPOS),
		D3D11_USAGE_DEFAULT, D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	SAFE_RELEASE(pMesh);

	// UI Mesh
	VERTEXTEXTURE	tUICenterVtx[4] =
	{
		VERTEXTEXTURE(-0.5f, -0.5f, 0.f, 0.f, 0.f),
		VERTEXTEXTURE(0.5f, -0.5f, 0.f, 1.f, 0.f),
		VERTEXTEXTURE(-0.5f, 0.5f, 0.f, 0.f, 1.f),
		VERTEXTEXTURE(0.5f, 0.5f, 0.f, 1.f, 1.f)
	};

	UINT	iRectIdx[6] = { 0, 1, 3, 0, 3, 2 };

	pMesh = CreateMesh("CenterOrthoRect", tUICenterVtx, 4, sizeof(VERTEXTEXTURE),
		D3D11_USAGE_DEFAULT, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
		iRectIdx, 6, 4);

	SAFE_RELEASE(pMesh);

	VERTEXTEXTURE	tUILTVtx[4] =
	{
		VERTEXTEXTURE(0.f, 0.f, 0.f, 0.f, 0.f),
		VERTEXTEXTURE(1.f, 0.f, 0.f, 1.f, 0.f),
		VERTEXTEXTURE(0.f, 1.f, 0.f, 0.f, 1.f),
		VERTEXTEXTURE(1.f, 1.f, 0.f, 1.f, 1.f)
	};

	pMesh = CreateMesh("UIMesh", tUILTVtx, 4, sizeof(VERTEXTEXTURE),
		D3D11_USAGE_DEFAULT, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
		iRectIdx, 6, 4);
	SAFE_RELEASE(pMesh);

	VERTEXCOLOR	tRectLine[5] =
	{
		VERTEXCOLOR(0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 1.f),
		VERTEXCOLOR(1.f, 0.f, 0.f, 0.f, 1.f, 0.f, 1.f),
		VERTEXCOLOR(1.f, 1.f, 0.f, 0.f, 1.f, 0.f, 1.f),
		VERTEXCOLOR(0.f, 1.f, 0.f, 0.f, 1.f, 0.f, 1.f),
		VERTEXCOLOR(0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 1.f)
	};

	// 충돌처리용 렉트 LineStrip 메쉬
	pMesh = CreateMesh("RectLine", tRectLine, 5, sizeof(VERTEXCOLOR), D3D11_USAGE_DEFAULT,
		D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
	SAFE_RELEASE(pMesh);

	CTexture*	pTexture = LoadTexture("Texture", L"Texture.png");	
	SAFE_RELEASE(pTexture);
	pTexture = LoadTexture("Pyramid", L"Pyramid.png");
	SAFE_RELEASE(pTexture);
	pTexture = LoadTexture("mjh", L"mjh.jpg");
	SAFE_RELEASE(pTexture);

	CSampler*	pSampler = CreateSampler("Linear");
	SAFE_RELEASE(pSampler);

	pSampler = CreateSampler("Point", D3D11_FILTER_MIN_MAG_MIP_POINT);
	SAFE_RELEASE(pSampler);

	return true;
}

CMesh * CResMgr::CreateMesh(const string & _strKey, void * _pVertices, unsigned int _iVtxCount, unsigned int _iVtxSize, D3D11_USAGE _eVtxUsage, D3D11_PRIMITIVE_TOPOLOGY _ePrimitive,
	void* _pIndices /*= NULL*/,
	unsigned int _iIdxCount /*= 0*/,
	unsigned int _iIdxSize /*= 0*/,
	D3D11_USAGE _eIdxUsage /*= D3D11_USAGE_DEFAULT*/,
	DXGI_FORMAT _eFormat /*= DXGI_FORMAT_R32_UINT*/)
{
	CMesh* pMesh = FindMesh(_strKey);

	// 동일한 _strKey값으로 이미 생성된 경우
	if (NULL != pMesh)
	{		
		return pMesh;
	}

	// CreateMesh
	pMesh = new CMesh();

	if (false == pMesh->CreateMesh(_strKey, _pVertices, _iVtxCount, _iVtxSize, _eVtxUsage, _ePrimitive, 
		_pIndices, _iIdxCount, _iIdxSize, _eIdxUsage, _eFormat))	
	{
		SAFE_RELEASE(pMesh);
		return NULL;
	}
	
	pMesh->AddRef();

	m_mapMesh.insert(make_pair(_strKey, pMesh));

	return pMesh;
}

CMesh * CResMgr::CreateSphere(const string & _strKey, float _fRadius, unsigned int _iNumSub, const DxVector4 & vColor)
{
	CMesh*	pMesh = FindMesh(_strKey);

	// 동일한 _strKey값으로 이미 생성된 경우
	if (NULL != pMesh)
	{
		return pMesh;
	}

	// CreateMesh
	pMesh = new CMesh();
	if (false == pMesh->CreateSphere(_strKey, _fRadius, _iNumSub, vColor))
	{
		SAFE_RELEASE(pMesh);
		return NULL;
	}

	pMesh->AddRef();
	m_mapMesh.insert(make_pair(_strKey, pMesh));

	return pMesh;
}

CMesh * CResMgr::CreateSphere(const string & _strKey, float _fRadius, unsigned int _iNumSub)
{
	CMesh*	pMesh = FindMesh(_strKey);

	// 동일한 _strKey값으로 이미 생성된 경우
	if (NULL != pMesh)
	{
		return pMesh;
	}

	// CreateMesh
	pMesh = new CMesh();
	if (false == pMesh->CreateSphere(_strKey, _fRadius, _iNumSub))
	{
		SAFE_RELEASE(pMesh);
		return NULL;
	}

	pMesh->AddRef();
	m_mapMesh.insert(make_pair(_strKey, pMesh));

	return pMesh;
}

CMesh * CResMgr::LoadMesh(const string & _strKey, const WCHAR * _pFileName, const string & _strPathKey)
{
	CMesh* pMesh = FindMesh(_strKey);

	if (NULL != pMesh)
	{
		return pMesh;
	}

	pMesh = new CMesh();

	if (false == pMesh->LoadMesh(_strKey, _pFileName, _strPathKey))
	{
		SAFE_RELEASE(pMesh);
		return NULL;
	}

	pMesh->AddRef();

	m_mapMesh.insert(make_pair(_strKey, pMesh));

	return pMesh;
}

CMesh * CResMgr::LoadMesh(const string & _strKey, const CHAR * _pFileName, const string & _strPathKey)
{
	CMesh* pMesh = FindMesh(_strKey);

	if (NULL != pMesh)
	{
		return pMesh;
	}

	pMesh = new CMesh();

	if (false == pMesh->LoadMesh(_strKey, _pFileName, _strPathKey))
	{
		SAFE_RELEASE(pMesh);
		return NULL;
	}

	pMesh->AddRef();

	m_mapMesh.insert(make_pair(_strKey, pMesh));

	return pMesh;
}

CMesh * CResMgr::LoadMeshFromFullPath(const string & _strKey, const WCHAR * _pFullPath)
{
	CMesh* pMesh = FindMesh(_strKey);

	if (NULL != pMesh)
	{
		return pMesh;
	}

	pMesh = new CMesh();

	if (false == pMesh->LoadMeshFromFullPath(_strKey, _pFullPath))
	{
		SAFE_RELEASE(pMesh);
		return NULL;
	}

	pMesh->AddRef();

	m_mapMesh.insert(make_pair(_strKey, pMesh));

	return pMesh;
}

CMesh * CResMgr::LoadMeshFromFullPath(const string & _strKey, const CHAR * _pFullPath)
{
	CMesh* pMesh = FindMesh(_strKey);

	if (NULL != pMesh)
	{
		return pMesh;
	}

	pMesh = new CMesh();

	if (false == pMesh->LoadMeshFromFullPath(_strKey, _pFullPath))
	{
		SAFE_RELEASE(pMesh);
		return NULL;
	}

	pMesh->AddRef();

	m_mapMesh.insert(make_pair(_strKey, pMesh));

	return pMesh;
}

CMesh * CResMgr::FindMesh(const string & _strKey)
{
	// _strKey값으로 탐색
	m_iterMesh = m_mapMesh.find(_strKey);

	if (m_iterMesh == m_mapMesh.end())
	{
		// 못찾으면 NULL리턴
		return NULL;
	}

	m_iterMesh->second->AddRef();

	// 찾으면 second값(CMesh* Type) 리턴
	return m_iterMesh->second;
}

CTexture * CResMgr::LoadTexture(const string & _strKey, TCHAR * _pFileName, const string & _strPathKey /*= TEXTUREPATH*/)
{
	CTexture*	pTexture = FindTexture(_strKey);

	if (NULL != pTexture)
	{
		return pTexture;
	}

	pTexture = new CTexture();
	if (false == pTexture->LoadTexture(_strKey, _pFileName, _strPathKey))
	{
		SAFE_RELEASE(pTexture);
		//assert(false);
		return NULL;
	}

	pTexture->AddRef();

	m_mapTexture.insert(make_pair(_strKey, pTexture));

	return pTexture;
}

CTexture * CResMgr::LoadTexture(const string & _strKey, CHAR * _pFileName, const string & _strPathKey)
{
	CTexture*	pTexture = FindTexture(_strKey);

	if (NULL != pTexture)
	{
		return pTexture;
	}

	pTexture = new CTexture();
	if (false == pTexture->LoadTexture(_strKey, _pFileName, _strPathKey))
	{
		SAFE_RELEASE(pTexture);
		assert(false);
		return NULL;
	}

	pTexture->AddRef();

	m_mapTexture.insert(make_pair(_strKey, pTexture));

	return pTexture;
}

CTexture * CResMgr::LoadTextureFromFullPath(const string & _strKey, const char * _pFullPath)
{
	CTexture*	pTexture = FindTexture(_strKey);

	if (NULL != pTexture)
	{
		return pTexture;
	}

	pTexture = new CTexture();
	if (false == pTexture->LoadTextureFromFullPath(_strKey, _pFullPath))
	{
		SAFE_RELEASE(pTexture);
		assert(false);
		return NULL;
	}

	pTexture->AddRef();

	m_mapTexture.insert(make_pair(_strKey, pTexture));

	return pTexture;
}

CTexture * CResMgr::LoadTexture(const string & _strKey, const vector<wstring>& _vecFileName, const string & _strPathKey)
{
	CTexture*	pTexture = FindTexture(_strKey);

	if (NULL != pTexture)
	{
		return pTexture;
	}

	pTexture = new CTexture();
	if (false == pTexture->LoadTexture(_strKey, _vecFileName, _strPathKey))
	{
		SAFE_RELEASE(pTexture);
		assert(false);
		return NULL;
	}

	pTexture->AddRef();

	m_mapTexture.insert(make_pair(_strKey, pTexture));

	return pTexture;
}

CTexture * CResMgr::LoadTextureFromMultibyte(const string & _strKey, const vector<string>& _vecFileName, const string & _strPathKey)
{
	CTexture*	pTexture = FindTexture(_strKey);

	if (NULL != pTexture)
	{
		return pTexture;
	}

	pTexture = new CTexture();
	if (false == pTexture->LoadTextureFromMultibyte(_strKey, _vecFileName, _strPathKey))
	{
		SAFE_RELEASE(pTexture);
		assert(false);
		return NULL;
	}

	pTexture->AddRef();

	m_mapTexture.insert(make_pair(_strKey, pTexture));

	return pTexture;
}

CTexture * CResMgr::LoadTextureFromFullPath(const string & _strKey, const vector<string>& _vecFullPath)
{
	CTexture*	pTexture = FindTexture(_strKey);

	if (NULL != pTexture)
	{
		return pTexture;
	}

	pTexture = new CTexture();
	if (false == pTexture->LoadTextureFromFullPath(_strKey, _vecFullPath))
	{
		SAFE_RELEASE(pTexture);
		assert(false);
		return NULL;
	}

	pTexture->AddRef();

	m_mapTexture.insert(make_pair(_strKey, pTexture));

	return pTexture;
}

CTexture * CResMgr::FindTexture(const string & _strKey)
{
	m_iterTexture = m_mapTexture.find(_strKey);

	if (m_mapTexture.end() == m_iterTexture)
	{
		return NULL;
	}

	m_iterTexture->second->AddRef();

	return m_iterTexture->second;
}

CSampler * CResMgr::CreateSampler(const string & _strKey, D3D11_FILTER _eFilter, D3D11_TEXTURE_ADDRESS_MODE _eAddrU, D3D11_TEXTURE_ADDRESS_MODE _eAddrV, D3D11_TEXTURE_ADDRESS_MODE _eAddrW)
{
	CSampler*	pSampler = FindSampler(_strKey);

	if (NULL != pSampler)
	{
		return pSampler;
	}

	pSampler = new CSampler();
	if (false == pSampler->CreateSampler(_strKey, _eFilter, _eAddrU, _eAddrV, _eAddrW))
	{
		SAFE_RELEASE(pSampler);
		return NULL;
	}

	pSampler->AddRef();

	m_mapSampler.insert(make_pair(_strKey, pSampler));

	return pSampler;
}

CSampler * CResMgr::FindSampler(const string & _strKey)
{
	m_iterSampler = m_mapSampler.find(_strKey);

	if (m_mapSampler.end() == m_iterSampler)
	{
		return NULL;
	}

	m_iterSampler->second->AddRef();

	return m_iterSampler->second;
}

CResMgr::CResMgr()
{
}

CResMgr::~CResMgr()
{
	Safe_Release_Map(m_mapMesh);
	Safe_Release_Map(m_mapTexture);
	Safe_Release_Map(m_mapSampler);
}
