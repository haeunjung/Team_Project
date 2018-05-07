#include "Collider.h"
#include "Transform.h"
#include "../Device.h"
#include "DirectXCollision.h"
#ifdef _DEBUG
#include "../04.Rendering/Shader.h"
#include "../04.Rendering/ShaderMgr.h"
#include "../03.Resource/Mesh.h"
#include "../03.Resource/ResMgr.h"
#include "../04.Rendering/RenderMgr.h"
#include "../04.Rendering/RenderState.h"
#endif // _DEBUG

WOOJUN_USING

COLLIDER_TYPE CCollider::GetColliderType() const
{
	return m_eColType;
}

COLLIDER_CHECK CCollider::GetColliderCheck() const
{
	return m_eColCheck;
}

DxVector3 CCollider::GetTransformWorldAxis(AXIS _Axis) const
{
	return m_pTransform->GetWorldAxis(_Axis);
}

void CCollider::AddColList(CCollider * _pCol)
{
	m_listCollider.push_back(_pCol);
}

void CCollider::EraseColList(CCollider * _pCol)
{
	m_iterEnd = m_listCollider.end();

	for (m_iter = m_listCollider.begin(); m_iter != m_iterEnd; ++m_iter)
	{
		if (*m_iter == _pCol)
		{
			m_listCollider.erase(m_iter);
			break;
		}
	}
}

bool CCollider::CheckColList(CCollider * _pCol)
{
	m_iterEnd = m_listCollider.end();

	for (m_iter = m_listCollider.begin(); m_iter != m_iterEnd; ++m_iter)
	{
		if (*m_iter == _pCol)
		{
			return true;
		}
	}
	return false;
}

void CCollider::CollisionEnable()
{
	m_bCollision = true;
}

void CCollider::SetMesh(const string & _strKey)
{
#ifdef _DEBUG
	m_pMesh = GET_SINGLE(CResMgr)->FindMesh(_strKey);
	m_pWireFrame = GET_SINGLE(CRenderMgr)->FindRenderState(WIRE_FRAME);
#endif // _DEBUG

}

void CCollider::SetShader(const string & _strKey)
{
#ifdef _DEBUG
	m_pShader = GET_SINGLE(CShaderMgr)->FindShader(_strKey);
#endif // _DEBUG

}

void CCollider::SetInputLayout(const string & _strKey)
{
#ifdef _DEBUG
	m_pInputLayout = GET_SINGLE(CShaderMgr)->FindInputLayout(_strKey);
#endif // _DEBUG

}

void CCollider::SetColor(float r, float g, float b, float a)
{
#ifdef _DEBUG
	m_vColor = DxVector4(r, g, b, a);
#endif // _DEBUG

}

void CCollider::Setcolor(const DxVector4 & _vColor)
{
#ifdef _DEBUG
	m_vColor = _vColor;
#endif // _DEBUG

}

void CCollider::SetColColor(float r, float g, float b, float a)
{
#ifdef _DEBUG
	m_vColColor = DxVector4(r, g, b, a);
#endif // _DEBUG

}

void CCollider::SetColcolor(const DxVector4 & _vColColor)
{
#ifdef _DEBUG
	m_vColColor = _vColColor;
#endif // _DEBUG

}

void CCollider::SetColCheck(COLLIDER_CHECK _eColCheck)
{
	m_eColCheck = _eColCheck;
}

void CCollider::Update(float _fTime)
{
	m_bCollision = false;

	DxVector3	vPos = m_pTransform->GetWorldPos();

	m_vMove = vPos - m_vPrevPos;

	m_vPrevPos = vPos;
}

void CCollider::LateUpdate(float _fTime)
{
}

void CCollider::Collision(float _fTime)
{
}

void CCollider::Render(float _fTime)
{
#ifdef _DEBUG
	if (NULL != m_pMesh)
	{		
		COLCOLORCBUFFER		tCBuffer = {};
		
		// 충돌중인경우
		if (true == m_bCollision)
		{
			// 충돌색
			tCBuffer.vColor = m_vColColor;
		}
		else
		{
			// 보통색
			tCBuffer.vColor = m_vColor;
		}

		m_pWireFrame->SetState();

		GET_SINGLE(CShaderMgr)->UpdateConstBuffer("ColliderColor", &tCBuffer, CUT_VERTEX);
		GET_SINGLE(CShaderMgr)->UpdateConstBuffer("Transform", &m_tTransform, CUT_VERTEX);
		CONTEXT->IASetInputLayout(m_pInputLayout);
		m_pShader->SetShader();
		m_pMesh->Render();

		m_pWireFrame->ResetState();
	}
#endif // _DEBUG

}

bool CCollider::ColSphereToSphere(const SPHEREINFO & _tSrc, const SPHEREINFO & _tDest)
{
	float	fDist = _tSrc.Distance(_tDest);

	return fDist <= _tSrc.fRadius + _tDest.fRadius;
}

bool CCollider::ColRectToRect(const RECTINFO & _tSrc, const RECTINFO & _tDest)
{
	if (_tSrc.fLeft > _tDest.fRight)
	{
		return false;
	}
	else if (_tSrc.fRight < _tDest.fLeft)
	{
		return false;
	}
	else if (_tSrc.fTop > _tDest.fBottom)
	{
		return false;
	}
	else if (_tSrc.fBottom < _tDest.fTop)
	{
		return false;
	}

	return true;
}

bool CCollider::ColRectToPoint(const RECTINFO & _tSrc, const DxVector3 & _vPos)
{
	if (_tSrc.fLeft > _vPos.x)
	{
		return false;
	}
	else if (_tSrc.fRight < _vPos.x)
	{
		return false;
	}
	else if (_tSrc.fTop > _vPos.y)
	{
		return false;
	}
	else if (_tSrc.fBottom < _vPos.y)
	{
		return false;
	}
	return true;
}

bool CCollider::ColRayToSphere(RAY & _tRay, const SPHEREINFO & _tSphere)
{
	if (_tRay.bIntersect)
		return false;

	DxVector3	vM = _tRay.vPos - _tSphere.vCenter;

	float	b = 2.f * vM.Dot(_tRay.vDir);
	float	c = vM.Dot(vM) - _tSphere.fRadius * _tSphere.fRadius;

	float	fDet = b * b - 4.f * c;

	if (fDet < 0.f)
		return false;

	fDet = sqrtf(fDet);

	float	t0, t1;

	t0 = (-b + fDet) / 2.f;
	t1 = (-b - fDet) / 2.f;

	if (t0 >= 0.f || t1 >= 0.f)
	{
		if (t0 >= 0.f && t0 < t1)
			_tRay.vIntersect = _tRay.vPos + _tRay.vDir * t0;

		else if (t1 >= 0.f && t1 < t0)
			_tRay.vIntersect = _tRay.vPos + _tRay.vDir * t1;

		_tRay.bIntersect = true;

		return true;
	}

	return false;
}

bool CCollider::ColTerrainToPosition(const TERRAININFO & _tTerrainInfo, CTransform * _pDestTransform, const DxVector3 & _vTerrainScale)
{
	return true;

	DxVector3	vPos = _pDestTransform->GetWorldPos();

	// 지형 그리드의 인덱스를 구한다.
	float	x = vPos.x / _vTerrainScale.x;
	float	z = vPos.z / _vTerrainScale.z;

	int	idx = (_tTerrainInfo.iNumH - 1 - ((int)z + 1)) * _tTerrainInfo.iNumW + (int)x;

	// 사각형을 구성하는 4개의 정점 정보를 구한다.
	DxVector3	vRectPos[4];
	vRectPos[0] = _tTerrainInfo.vecPos[idx];
	vRectPos[1] = _tTerrainInfo.vecPos[idx + 1];
	vRectPos[2] = _tTerrainInfo.vecPos[idx + _tTerrainInfo.iNumW];
	vRectPos[3] = _tTerrainInfo.vecPos[idx + _tTerrainInfo.iNumW + 1];

	// 우상단, 좌하단 삼각형 체크
	// x와 z중 x값이 크면 우상단, z값이 크면 좌하단
	x -= (int)x;
	z = 1.0f - (z - (int)z);

	float fY = 0.0f;
	if (x >= z)
	{
		fY = vRectPos[0].y + (vRectPos[1].y - vRectPos[0].y) * x + (vRectPos[3].y - vRectPos[1].y) * z;
	}
	else
	{
		fY = vRectPos[0].y + (vRectPos[3].y - vRectPos[2].y) * x + (vRectPos[2].y - vRectPos[0].y) * z;
	}

	vPos.y = fY;
	_pDestTransform->SetWorldPos(vPos);

	return true;
}

bool CCollider::ColTerrainToRay(const TERRAININFO & _tTerrainInfo, _tagRay& _tRay, const DxVector3 & _vTerrainScale)
{
	// 지형 그리드의 인덱스를 구한다.
	float	x = _tRay.vPos.x / _vTerrainScale.x;
	float	z = _tRay.vPos.z / _vTerrainScale.z;

	int	idx = (_tTerrainInfo.iNumH - 1 - ((int)z + 1)) * _tTerrainInfo.iNumW + (int)x;

	// 사각형을 구성하는 4개의 정점 정보를 구한다.
	DxVector3	vRectPos[4];
	vRectPos[0] = _tTerrainInfo.vecPos[idx];
	vRectPos[1] = _tTerrainInfo.vecPos[idx + 1];
	vRectPos[2] = _tTerrainInfo.vecPos[idx + _tTerrainInfo.iNumW];
	vRectPos[3] = _tTerrainInfo.vecPos[idx + _tTerrainInfo.iNumW + 1];

	float fDist = 0.0f;
	if (true == DirectX::TriangleTests::Intersects(_tRay.vPos.Convert(), _tRay.vDir.Convert(),
		vRectPos[0].Convert(), vRectPos[1].Convert(), vRectPos[2].Convert(), fDist))
	{
		int a = 0;
	}



	// 우상단, 좌하단 삼각형 체크
	// x와 z중 x값이 크면 우상단, z값이 크면 좌하단
	x -= (int)x;
	z = 1.0f - (z - (int)z);

	float fY = 0.0f;
	if (x >= z)
	{
		fY = vRectPos[0].y + (vRectPos[1].y - vRectPos[0].y) * x + (vRectPos[3].y - vRectPos[1].y) * z;
	}
	else
	{
		fY = vRectPos[0].y + (vRectPos[3].y - vRectPos[2].y) * x + (vRectPos[2].y - vRectPos[0].y) * z;
	}

	_tRay.vPos.y = fY;

	return true;
}

bool CCollider::ColSphereToAABB(const SPHEREINFO & _tSrc, AABBINFO & _tDest)
{
	if (_tSrc.vCenter.x < _tDest.vCenter.x - (_tDest.vScale.x * 0.5f))
	{
		m_Point.x = _tDest.vCenter.x - (_tDest.vScale.x * 0.5f);
	}
	else if (_tSrc.vCenter.x > _tDest.vCenter.x + (_tDest.vScale.x * 0.5f))
	{
		m_Point.x = _tDest.vCenter.x + (_tDest.vScale.x * 0.5f);
	}
	else
	{
		m_Point.x = _tSrc.vCenter.x;
	}

	if (_tSrc.vCenter.y < _tDest.vCenter.y - (_tDest.vScale.y * 0.5f))
	{
		m_Point.y = _tDest.vCenter.y - (_tDest.vScale.y * 0.5f);
	}
	else if (_tSrc.vCenter.y > _tDest.vCenter.y + (_tDest.vScale.y * 0.5f))
	{
		m_Point.y = _tDest.vCenter.y + (_tDest.vScale.y * 0.5f);
	}
	else
	{
		m_Point.y = _tSrc.vCenter.y;
	}

	if (_tSrc.vCenter.z < _tDest.vCenter.z - (_tDest.vScale.z * 0.5f))
	{
		m_Point.z = _tDest.vCenter.z - (_tDest.vScale.z * 0.5f);
	}
	else if (_tSrc.vCenter.z > _tDest.vCenter.z + (_tDest.vScale.z * 0.5f))
	{
		m_Point.z = _tDest.vCenter.z + (_tDest.vScale.z * 0.5f);
	}
	else
	{
		m_Point.z = _tSrc.vCenter.z;
	}

	// MAX == Z : FRONT


	// 구의 반지름과 구와 m_Point의 거리 비교
	// 구의 반지름이 더 크면 충돌
	DxVector3 vDist = _tSrc.vCenter - m_Point;
	if (_tSrc.fRadius > vDist.Length())
	{
		if (vDist.x == vDist.Max()
			&& vDist.y == vDist.z)
		{
			_tDest.eColAABB = CAP_RIGHT;
		}
		else if (vDist.z == vDist.Max()
			&& vDist.x == vDist.y)
		{
			_tDest.eColAABB = CAP_FRONT;
		}
		else if (vDist.x == vDist.Min()
			&& vDist.y == vDist.z)
		{
			_tDest.eColAABB = CAP_LEFT;
		}
		else if (vDist.z == vDist.Min()
			&& vDist.x == vDist.y)
		{
			_tDest.eColAABB = CAP_BACK;
		}
		//else
		//{
		//	_tDest.eColAABB = CAP_UP;
		//}

		return true;
	}
	
	return false;
}

CCollider::CCollider()
{
	m_eComponentType = CT_COLLIDER;

	SetColor(0.0f, 1.0f, 0.0f, 1.0f);
	SetColColor(1.0f, 0.0f, 0.0f, 1.0f);

#ifdef _DEBUG
	m_pMesh = NULL;
	m_pShader = NULL;
	m_pWireFrame = NULL;
	m_pInputLayout = NULL;
#endif // _DEBUG

}

CCollider::CCollider(const CCollider & _Collider) :
	CComponent(_Collider)
{
	m_eColType = _Collider.m_eColType;
	m_eColCheck = _Collider.m_eColCheck;
	m_vPrevPos = _Collider.m_vPrevPos;

#ifdef _DEBUG
	m_pMesh = _Collider.m_pMesh;
	m_pShader = _Collider.m_pShader;
	m_pWireFrame = _Collider.m_pWireFrame;
	m_pInputLayout = _Collider.m_pInputLayout;

	m_vColor = _Collider.m_vColor;
	m_vColColor = _Collider.m_vColColor;

	if (NULL != m_pMesh)
	{
		m_pMesh->AddRef();
	}

	if (NULL != m_pShader)
	{
		m_pShader->AddRef();
	}

	if (NULL != m_pWireFrame)
	{
		m_pWireFrame->AddRef();
	}
#endif // _DEBUG

}

CCollider::~CCollider()
{
#ifdef _DEBUG
	SAFE_RELEASE(m_pWireFrame);
	SAFE_RELEASE(m_pMesh);
	SAFE_RELEASE(m_pShader);
#endif // _DEBUG

}
