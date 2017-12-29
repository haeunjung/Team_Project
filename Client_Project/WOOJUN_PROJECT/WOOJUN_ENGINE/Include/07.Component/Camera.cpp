#include "Camera.h"
#include "Transform.h"
#include "../Engine_Core.h"
#include "../06.GameObject/GameObject.h"
#include "CameraArm.h"

WOOJUN_USING

XMMATRIX CCamera::GetViewMatrix() const
{
	return m_matView->mat;
}

XMMATRIX CCamera::GetProjMatrix() const
{
	return m_matProj->mat;
}

DxVector3 CCamera::GetAxis(AXIS _Axis) const
{
	return m_vAxis[_Axis];
}

void CCamera::SetAxis(DxVector3 _vAxis[AXIS_MAX])
{
	// 전체 축 세팅
	for (int i = 0; i < AXIS_MAX; ++i)
	{
		m_vAxis[i] = _vAxis[i];
	}

	ComputeViewMatrix();
}

void CCamera::SetAxis(const DxVector3 & _vAxis, AXIS _Axis)
{
	// 지정 축 세팅
	m_vAxis[_Axis] = _vAxis;

	ComputeViewMatrix();
}

void CCamera::Attach(CGameObject * _pGameObject, const DxVector3& _vDist)
{
	// 기존에 있던 Attach Release
	SAFE_RELEASE(m_pAttachTransform);
	SAFE_RELEASE(m_pAttachObject);

	// _pGameObject 참조하니까 AddRef
	_pGameObject->AddRef();
	m_pAttachObject = _pGameObject;

	// GetTransform에서 AddRef함
	m_pAttachTransform = _pGameObject->GetTransform();
	
	m_vPrevPos = m_pAttachTransform->GetWorldPos();
	
	// 거리를 이용해 카메라의 위치를 구해준다.
	m_pTransform->SetWorldPos(m_vPrevPos + _vDist);

	// 대상을 바라보게 회전시킨다.
	m_pTransform->LookAt(m_pAttachTransform);

	for (int i = 0; i < AXIS_MAX; ++i)
	{
		m_vAxis[i] = m_pTransform->GetWorldAxis((AXIS)i);
	}
}

void CCamera::Detach()
{
	SAFE_RELEASE(m_pAttachTransform);
	SAFE_RELEASE(m_pAttachObject);
}

CGameObject * CCamera::GetAttachObject()
{
	if (NULL == m_pAttachObject)
	{
		return NULL;
	}

	m_pAttachObject->AddRef();

	return m_pAttachObject;
}

void CCamera::CreateProjection(float _fAngle, float _fWidth, float _fHeight, float _fNear /*= 0.3f*/, float _fFar /*= 1000.0f*/)
{
	*m_matProj = XMMatrixPerspectiveFovLH(_fAngle, _fWidth / _fHeight, _fNear, _fFar);
}

void CCamera::CreateOrthoProjection(float _fWidth, float _fHeight, float _fNear /*= 0.0f*/, float _fFar /*= 1000.0f*/)
{
	*m_matProj = XMMatrixOrthographicOffCenterLH(0.0f, _fWidth, _fHeight, 0.0f, _fNear, _fFar);
}

void CCamera::ComputeViewMatrix()
{
	// 뷰행렬 갱신
	*m_matView = m_matView->Identity();

	DxVector3	vPos = m_pTransform->GetWorldPos();
	DxVector3	vAxis[AXIS_MAX];

	// CameraArm 컴포넌트 확인
	bool	bCheckArm = m_pGameObject->CheckComponentFromTypeID<CCameraArm>();

	for (int i = 0; i < AXIS_MAX; ++i)
	{
		if (false == bCheckArm)
		{
			vAxis[i] = m_pTransform->GetWorldAxis((AXIS)i);
		}
		else
		{
			vAxis[i] = m_vAxis[i];
		}
		memcpy(&m_matView->m[i][0], &vAxis[i], sizeof(DxVector3));
	}

	*m_matView = m_matView->Transpose();

	for (int i = 0; i < AXIS_MAX; ++i)
	{
		m_matView->m[3][i] = -vPos.Dot(vAxis[i]);
	}
}

bool CCamera::Init()
{
	m_matView = new MATRIX();
	m_matProj = new MATRIX();

	// 디폴트 투영행렬 생성
	CreateProjection(PI / 3.0f, (float)_RESOLUTION.m_iWidth, (float)_RESOLUTION.m_iHeight);

	for (int i = 0; i < AXIS_MAX; ++i)
	{
		m_vAxis[i] = WORLDAXIS[i];
	}

	return true;
}

void CCamera::Input(float _fTime)
{
}

void CCamera::Update(float _fTime)
{
	// 붙어있는 오브젝트가 움직였을 경우 이동시켜준다.
	if (NULL != m_pAttachObject)
	{
		DxVector3	vAttachPos = m_pAttachTransform->GetWorldPos();
		DxVector3	vMove = vAttachPos - m_vPrevPos;

		m_pTransform->Move(vMove);

		m_vPrevPos = vAttachPos;
	}
}

void CCamera::LateUpdate(float _fTime)
{
	ComputeViewMatrix();
}

void CCamera::Collision(float _fTime)
{
}

void CCamera::Render(float _fTime)
{
}

CCamera * CCamera::Clone()
{
	return new CCamera(*this);
}

CCamera::CCamera() :
	m_matView(NULL),
	m_matProj(NULL),
	m_pAttachObject(NULL),
	m_pAttachTransform(NULL)
{
	SetTypeName("CCamera");
	SetTypeID<CCamera>();
	m_eComponentType = CT_CAMERA;
}

CCamera::CCamera(const CCamera & _Camera) :
	CComponent(_Camera)
{
	m_matView = new MATRIX();
	m_matProj = new MATRIX();

	*m_matView = *(_Camera.m_matView);
	*m_matProj = *(_Camera.m_matProj);	
}

CCamera::~CCamera()
{
	SAFE_RELEASE(m_pAttachTransform);
	SAFE_RELEASE(m_pAttachObject);
	SAFE_DELETE(m_matView);
	SAFE_DELETE(m_matProj);
}