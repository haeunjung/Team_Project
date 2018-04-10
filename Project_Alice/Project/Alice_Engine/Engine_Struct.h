#pragma once
//#include "Engine_Macro.h"
#include "Engine_Enum.h"

WOOJUN_BEGIN

// Resolution
typedef struct DLL _tagResolution
{
	unsigned int	m_iWidth;
	unsigned int	m_iHeight;

	// 생성자
	_tagResolution() : m_iWidth(0), m_iHeight(0) {}
	_tagResolution(unsigned int _iW, unsigned int _iH) : m_iWidth(_iW), m_iHeight(_iH) {}

	// 복사생성자
	_tagResolution(const _tagResolution& _Other) {
		*this = _Other;
	}

	// 대입연산자
	void operator=(const _tagResolution& _Other) {
		m_iWidth = _Other.m_iWidth;
		m_iHeight = _Other.m_iHeight;		
	}
}RESOLUTION, *pROSOLUTION;

// 버텍스 버퍼
typedef struct DLL _tagVertexBuffer
{
	ID3D11Buffer*	pBuffer;
	void*			pData;
	unsigned int	iCount;
	unsigned int	iSize;
	D3D11_PRIMITIVE_TOPOLOGY	ePrimitive;
	D3D11_USAGE		eUsage;

	_tagVertexBuffer() :
		pBuffer(NULL), pData(NULL), iCount(0), iSize(0),
		ePrimitive(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST),
		eUsage(D3D11_USAGE_DEFAULT)
	{
	}
}VERTEXBUFFER, *pVERTEXBUFFER;

// 인덱스 버퍼
typedef struct DLL _tagIndexBuffer
{
	ID3D11Buffer*	pBuffer;
	void*			pData;
	unsigned int	iCount;
	unsigned int	iSize;
	DXGI_FORMAT		eFormat;
	D3D11_USAGE		eUsage;

	_tagIndexBuffer() :
		pBuffer(NULL), pData(NULL), iCount(0), iSize(0),
		eFormat(DXGI_FORMAT_R32_UINT),
		eUsage(D3D11_USAGE_DEFAULT)
	{
	}
}INDEXBUFFER, *pINDEXBUFFER;

// Mesh Containger
typedef struct DLL _tagMeshContainer
{
	pVERTEXBUFFER				pVtxBuffer;
	vector<pINDEXBUFFER>		vecIndexBuffer;
	vector<class CMaterial*>	vecMaterial;

	_tagMeshContainer() :
		pVtxBuffer(NULL)
	{
	}
}MESHCONTAINER, *pMESHCONTAINER;

// Transform 행렬 정보 구조체
typedef struct DLL _tagTransformCBuffer
{
	XMMATRIX	matWorld;
	XMMATRIX	matView;
	XMMATRIX	matProj;
	XMMATRIX	matWV;
	XMMATRIX	matWVP;
	XMMATRIX	matVP;
	XMMATRIX	matInvProj;
	XMMATRIX	matInvView;
	XMMATRIX	matInvVP;
	DxVector3	vPivot;
	float		fEmpty1;
	DxVector3	vMeshSize;
	float		fEmpty2;
	DxVector3	vMeshMin;
	float		fEmpty3;
	DxVector3	vMeshMax;
	float		fEmpty4;
}TRANSFORMCBUFFER, *pTRANSFORMCBUFFER;

// Color Vertex
typedef struct DLL _tagVertexColor
{
	DxVector3 vPos;
	DxVector4 vColor;

	_tagVertexColor() :
		vPos(0.0f, 0.0f, 0.0f),
		vColor(0.0f, 0.0f, 0.0f, 0.0f)
	{
	}

	_tagVertexColor(const _tagVertexColor& _Other) {
		*this = _Other;
	}

	_tagVertexColor(float x, float y, float z,
		float r, float g, float b, float a) :
		vPos(x, y, z),
		vColor(r, g, b, a)
	{
	}

	void operator =(const _tagVertexColor& _Other) {
		vPos = _Other.vPos;
		vColor = _Other.vColor;
	}
}VERTEXCOLOR, *pVERTEXCOLOR;

// 텍스쳐 버퍼
typedef struct DLL _tagVertexTexture
{
	DxVector3	vPos;
	DxVector2	vUV;

	_tagVertexTexture() :
		vPos(0.f, 0.f, 0.f),
		vUV(0.f, 0.f)
	{
	}

	_tagVertexTexture(const _tagVertexTexture& vtx)
	{
		*this = vtx;
	}

	_tagVertexTexture(float x, float y, float z, float u, float v) :
		vPos(x, y, z),
		vUV(u, v)
	{
	}

	void operator =(const _tagVertexTexture& vtx)
	{
		vPos = vtx.vPos;
		vUV = vtx.vUV;
	}
}VERTEXTEXTURE, *pVERTEXTEXTURE;

// 텍츠쳐 노말 버퍼
typedef struct DLL _tagVertexNormalTexture
{
	DxVector3	vPos;
	DxVector3	vNormal;
	DxVector2	vUV;

	_tagVertexNormalTexture() :
		vPos(0.f, 0.f, 0.f),
		vNormal(0.f, 0.f, 0.f),
		vUV(0.f, 0.f)
	{
	}

	_tagVertexNormalTexture(const _tagVertexNormalTexture& vtx)
	{
		*this = vtx;
	}

	_tagVertexNormalTexture(float x, float y, float z, float nx, float ny,
		float nz, float u, float v) :
		vPos(x, y, z),
		vNormal(nx, ny, nz),
		vUV(u, v)
	{
	}

	void operator =(const _tagVertexNormalTexture& vtx)
	{
		vPos = vtx.vPos;
		vNormal = vtx.vNormal;
		vUV = vtx.vUV;
	}
}VERTEXNORMALTEXTURE, *pVERTEXNORMALTEXTURE;

// 위치 버텍스 버퍼
typedef struct DLL _tagVertexPos
{
	DxVector3	vPos;

	_tagVertexPos() :
		vPos(0.0f, 0.0f, 0.0f)
	{
	}

	_tagVertexPos(const _tagVertexPos& _Other) {
		*this = _Other;
	}

	_tagVertexPos(float x, float y, float z) :
		vPos(x, y, z)
	{
	}

	void operator =(const _tagVertexPos& _Other) {
		vPos = _Other.vPos;
	}
}VERTEXPOS, *pVERTEXPOS;

// 범프 버텍스 버퍼
typedef struct DLL _tagVertexBump
{
	DxVector3	vPos;
	DxVector3	vNormal;
	DxVector2	vUV;
	DxVector3	vTangent;
	DxVector3	vBinormal;

	_tagVertexBump() :
		vPos(0.f, 0.f, 0.f),
		vNormal(0.f, 0.f, 0.f),
		vUV(0.f, 0.f),
		vTangent(0.f, 0.f, 0.f),
		vBinormal(0.f, 0.f, 0.f)
	{
	}

	_tagVertexBump(const _tagVertexBump& vtx)
	{
		*this = vtx;
	}

	_tagVertexBump(float x, float y, float z, 
		float nx, float ny,	float nz,
		float u, float v,
		float tx, float ty, float tz,
		float bx, float by, float bz) :
		vPos(x, y, z),
		vNormal(nx, ny, nz),
		vUV(u, v),
		vTangent(tx, ty, tz),
		vBinormal(bx, by, bz)
	{
	}

	void operator =(const _tagVertexBump& vtx)
	{
		vPos = vtx.vPos;
		vNormal = vtx.vNormal;
		vUV = vtx.vUV;
		vTangent = vtx.vTangent;
		vBinormal = vtx.vBinormal;
	}
}VERTEXBUMP, *pVERTEXBUMP;

// 애니메이션 범프 버텍스 버퍼
typedef struct DLL _tagVertexAniBump
{
	DxVector3	vPos;
	DxVector3	vNormal;
	DxVector2	vUV;
	DxVector3	vTangent;
	DxVector3	vBinormal;
	DxVector4	vWeight;
	DxVector4	vIndices;

	_tagVertexAniBump() :
		vPos(0.f, 0.f, 0.f),
		vNormal(0.f, 0.f, 0.f),
		vUV(0.f, 0.f),
		vTangent(0.f, 0.f, 0.f),
		vBinormal(0.f, 0.f, 0.f),
		vWeight(0.f, 0.f, 0.f, 0.f),
		vIndices(0.f, 0.f, 0.f, 0.f)
	{
	}

	_tagVertexAniBump(const _tagVertexAniBump& vtx)
	{
		*this = vtx;
	}

	_tagVertexAniBump(float x, float y, float z,
		float nx, float ny, float nz,
		float u, float v,
		float tx, float ty, float tz,
		float bx, float by, float bz,
		float wx, float wy, float wz, float ww,
		float ix, float iy, float iz, float iw) :
		vPos(x, y, z),
		vNormal(nx, ny, nz),
		vUV(u, v),
		vTangent(tx, ty, tz),
		vBinormal(bx, by, bz),
		vWeight(wx, wy, wz, ww),
		vIndices(ix, iy, iz, iw)
	{
	}

	void operator =(const _tagVertexAniBump& vtx)
	{
		vPos = vtx.vPos;
		vNormal = vtx.vNormal;
		vUV = vtx.vUV;
		vTangent = vtx.vTangent;
		vBinormal = vtx.vBinormal;
		vWeight = vtx.vWeight;
		vIndices = vtx.vIndices;
	}
}VERTEXANIBUMP, *pVERTEXANIBUMP;

// 애니메이션 범프 버텍스 버퍼
typedef struct DLL _tagVertexAni
{
	DxVector3	vPos;
	DxVector3	vNormal;
	DxVector2	vUV;
	DxVector4	vWeight;
	DxVector4	vIndices;

	_tagVertexAni() :
		vPos(0.f, 0.f, 0.f),
		vNormal(0.f, 0.f, 0.f),
		vUV(0.f, 0.f),
		vWeight(0.f, 0.f, 0.f, 0.f),
		vIndices(0.f, 0.f, 0.f, 0.f)
	{
	}

	_tagVertexAni(const _tagVertexAni& vtx)
	{
		*this = vtx;
	}

	_tagVertexAni(float x, float y, float z,
		float nx, float ny, float nz,
		float u, float v,
		float wx, float wy, float wz, float ww,
		float ix, float iy, float iz, float iw) :
		vPos(x, y, z),
		vNormal(nx, ny, nz),
		vUV(u, v),
		vWeight(wx, wy, wz, ww),
		vIndices(ix, iy, iz, iw)
	{
	}

	void operator =(const _tagVertexAni& vtx)
	{
		vPos = vtx.vPos;
		vNormal = vtx.vNormal;
		vUV = vtx.vUV;
		vWeight = vtx.vWeight;
		vIndices = vtx.vIndices;
	}
}VERTEXANI, *pVERTEXANI;

// 상수버퍼
typedef struct DLL _tagConstBuffer
{
	ID3D11Buffer*	pBuffer;
	int		iSize;
	int		iRegister;
}CONSTBUFFER, *pCONSTBUFFER;

// 충돌체 컬러 상수버퍼
typedef struct DLL _tagColColorCBuffer
{
	DxVector4	vColor;
}COLCOLORCBUFFER, *pCOLCOLORCBUFFER;

typedef struct DLL _tagSphereInfo
{
	DxVector3	vCenter;
	float		fRadius;

	_tagSphereInfo() :
		vCenter(0.0f, 0.0f, 0.0f),
		fRadius(0.0f)
	{
	}

	_tagSphereInfo(const _tagSphereInfo& _tSphere)
	{
		*this = _tSphere;
	}

	_tagSphereInfo(const DxVector3& _vCenter, float _fRadius) :
		vCenter(_vCenter),
		fRadius(_fRadius)
	{
	}

	_tagSphereInfo(float _fX, float _fY, float _fZ, float _fRadius) :
		vCenter(_fX, _fY, _fZ),
		fRadius(_fRadius)
	{
	}

	float Distance(const _tagSphereInfo& _tSphere) const
	{
		return vCenter.Distance(_tSphere.vCenter);
	}

	float Distance(const _tagSphereInfo* _tSphere) const
	{
		return Distance(*_tSphere);
	}
}SPHEREINFO, *pSPHEREINFO;

typedef struct DLL _tagAABBInfo
{
	DxVector3		vCenter;
	DxVector3		vScale;
	COL_AABB_POS	eColAABB;

	_tagAABBInfo() :
		vCenter(0.0f, 0.0f, 0.0f),
		vScale(0.0f, 0.0f, 0.0f),
		eColAABB(CAP_DEFAULT)
	{
	}

	_tagAABBInfo(const _tagAABBInfo& _tAABB)
	{
		*this = _tAABB;
	}

	_tagAABBInfo(const DxVector3& _vCenter, const DxVector3& _vLength) :
		vCenter(_vCenter),
		vScale(_vLength)
	{
	}
}AABBINFO, *pAABBINFO;

typedef struct _tagRectInfo
{
	float	fTop;
	float	fBottom;
	float	fLeft;
	float	fRight;

	_tagRectInfo() :
		fLeft(0.0f),
		fTop(0.0f),
		fRight(0.0f),
		fBottom(0.0f)
	{
	}

	_tagRectInfo(float _fLeft, float _fTop, float _fRight, float _fBottom) :
		fLeft(_fLeft),
		fTop(_fTop),
		fRight(_fRight),
		fBottom(_fBottom)
	{
	}

	_tagRectInfo(const _tagRectInfo& _rc)
	{
		fLeft = _rc.fLeft;
		fTop = _rc.fTop;
		fRight = _rc.fRight;
		fBottom = _rc.fBottom;
	}

	void MoveRect(float _fX, float _fY)
	{
		fLeft += _fX;
		fTop += _fY;
		fRight += _fX;
		fBottom += _fY;
	}
}RECTINFO, *PRECTINFO;

typedef struct DLL _tagRay
{
	DxVector3	vPos;
	DxVector3	vDir;
	DxVector3	vIntersect;
	bool		bIntersect;
}RAY, *pRAY;

typedef struct DLL _tagTerrainInfo
{
	vector<DxVector3>	vecPos;
	UINT				iNumW;
	UINT				iNumH;
}TERRAININFO, *pTERRAININFO;

typedef struct DLL _tagLightInfo
{
	DxVector4	vDiffuse;
	DxVector4	vAmbient;
	DxVector4	vSpecular;
	DxVector3	vAttenuation;	// 감쇠상수	
	LIGHT_TYPE	eType;	
}LIGHTINFO, *pLIGHTINFO;

typedef struct DLL _tagLightCBuffr
{
	DxVector4	vDiffuse;
	DxVector4	vAmbient;
	DxVector4	vSpecular;
	LIGHT_TYPE	eType;
	DxVector3	vDir;
	DxVector3	vPos;
	float		fRange;
	DxVector3	vAttenuation;
	float		fSpot;
}LIGHTCBUFFER, *pLIGHTCBUFFER;

typedef struct DLL _tagMaterialInfo
{
	DxVector4	vDiffuse;
	DxVector4	vAmbient;
	DxVector4	vSpecular;
	DxVector4	vEmissive;
	int			iBump;
	int			iSpecular;
	float		fSpecularPower;
	int			iEmpty;
}MATERIALINFO, *pMATERIALINFO;

// Effect CBuffer
typedef struct DLL _tagEffectCBuffer
{
	// SIMD 레지스터 사용이므로
	// 128비트 단위로 맞춰야한다
	// 안그러면 셰이더에 이상한값 세팅됨
	DxVector3	vCenter;
	float		fEmpty1;
	DxVector3	vCamAxisY;
	float		fEmpty2;
	DxVector3	vCamPos;
	float		fEmpty3;
	DxVector2	vSize;
	DxVector2	vEmpty4;
}EFFECTCBUFFER, *pEFFECTCBUFFER;

// Animation2D Clip
typedef struct DLL _tagAnimationClip2D
{
	string				strName;
	class CTexture*		pTexture;
	int					iTexRegister;
	ANIMATION2D_TYPE	eType;
	ANIMATION_OPTION	eOption;
	int					iFrameX;
	int					iFrameY;
	int					iFrameMaxX;
	int					iFrameMaxY;
	float				fFrameTime;
	float				fLimitTime;
	int					iLoopCount;
	float				fLoopTime;
}ANIMATIONCLIP2D, *pANIMATIONCLIP2D;

// Animation2D CBuffer
typedef struct DLL _tagAnimation2DCBuffer
{
	ANIMATION2D_TYPE	eType;
	int					iFrameX;
	int					iFrameY;
	int					iFrameMaxX;
	int					iFrameMaxY;
	DxVector3			vEmpty;
}ANIMATION2DCBUFFER, *pANIMATION2DCBUFFER;

typedef struct DLL _tagTerrainCBuffer
{
	float		fDetailLevel;
	int			iSplatCount;
	DxVector2	vEmpty;
}TERRAINCBUFFER, *pTERRAINCBUFFER;

typedef struct DLL _tagRendererCBuffer
{
	int		iRegister;
	int		iSize;
	void*	pData;
	int		iShaderType;
}RENDERERCBUFFER, *pRENDERERCBUFFER;

// Static Object Data
typedef struct DLL _tagObjectData
{
	string		strKey;
	wstring		wstrFileName;
	DxVector3	vPos;
	DxVector3	vScale;
	DxVector3	vRot;
}OBJECTDATA, *pOBJECTDATA;

WOOJUN_END