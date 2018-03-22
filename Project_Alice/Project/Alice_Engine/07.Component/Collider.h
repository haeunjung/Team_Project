#pragma once
#include "Component.h"

WOOJUN_BEGIN

class CMesh;
class CShader;
class CRenderState;
class DLL CCollider :
	public CComponent
{
protected:
	COLLIDER_TYPE	m_eColType;
	DxVector3		m_vPrevPos;
	DxVector3		m_vMove;

	list<CCollider*>	m_listCollider;
	list<CCollider*>::iterator	m_iter;
	list<CCollider*>::iterator	m_iterEnd;

	bool			m_bCollision;

#ifdef _DEBUG
	// 디버그모드일때만 충돌체 렌더
	CMesh*				m_pMesh;
	CShader*			m_pShader;
	CRenderState*		m_pWireFrame;
	ID3D11InputLayout*	m_pInputLayout;
	TRANSFORMCBUFFER	m_tTransform;
	DxVector4			m_vColor;
	DxVector4			m_vColColor;	
#endif // _DEBUG

public:
	COLLIDER_TYPE GetColliderType() const;
	DxVector3 GetTransformWorldAxis(AXIS _Axis) const;
public:
	void AddColList(CCollider* _pCol);
	void EraseColList(CCollider* _pCol);
	bool CheckColList(CCollider* _pCol);
	void CollisionEnable();
	void SetMesh(const string& _strKey);
	void SetShader(const string& _strKey);
	void SetInputLayout(const string& _strKey);
	void SetColor(float r, float g, float b, float a);
	void Setcolor(const DxVector4& _vColor);
	void SetColColor(float r, float g, float b, float a);
	void SetColcolor(const DxVector4& _vColColor);
public:
	virtual bool Init() = 0;
	virtual void Input(float _fTime) = 0;
	virtual void Update(float _fTime);
	virtual void LateUpdate(float _fTime);
	virtual void Collision(float _fTime);
	virtual void Render(float _fTime);
	virtual CCollider* Clone() = 0;
	virtual bool Collision(CCollider* pCollider) = 0;
protected:
	bool ColSphereToSphere(const SPHEREINFO& _tSrc, const SPHEREINFO& _tDest);
	bool ColRectToRect(const RECTINFO& _tSrc, const RECTINFO& _tDest);
	bool ColRectToPoint(const RECTINFO& _tSrc, const DxVector3& _vPos);
	bool ColRayToSphere(RAY& _tRay, const SPHEREINFO& _tSphere);
	bool ColTerrainToPosition(const TERRAININFO& _tTerrainInfo, CTransform* _pDestTransform, const DxVector3& _vTerrainScale);
protected:
	CCollider();
	CCollider(const CCollider& _Collider);
	virtual ~CCollider() = 0;
};

WOOJUN_END