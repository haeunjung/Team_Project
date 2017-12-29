#include "Collider.h"
#include "Transform.h"
#include "../Device.h"
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
