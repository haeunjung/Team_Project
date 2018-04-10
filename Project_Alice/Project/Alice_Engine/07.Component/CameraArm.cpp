#include "CameraArm.h"
#include "Camera.h"
#include "../01.Core/Input.h"
#include "../07.Component/Transform.h"
#include "../06.GameObject/GameObject.h"

WOOJUN_USING

CCameraArm::CCameraArm()
{
	SetTag("CameraArm");
	SetTypeName("CCameraArm");
	SetTypeID<CCameraArm>();
	m_eComponentType = CT_CAMERAARM;
}

CCameraArm::CCameraArm(const CCameraArm & _CameraArm) :
	CComponent(_CameraArm)
{
}


CCameraArm::~CCameraArm()
{
}

void CCameraArm::Zoom(float _fTime)
{
	// 카메라에 붙어있는 오브젝트와 현재 카메라의 거리를 구한다.
	CCamera*	pCamera = m_pGameObject->FindComponentFromTypeID<CCamera>();

	// Zoom거리	=	방향	*	속도	*	시간
	float fSpeed = WHEELDIR * m_fZoomSpeed * _fTime;
	// 카메라의 현재 위치
	DxVector3	vPos = m_pTransform->GetWorldPos();
	// 카메라의 현재 Z축
	DxVector3	vAxisZ = pCamera->GetAxis(AXIS_Z);

	// 계산
	vPos += vAxisZ * fSpeed;
	// 세팅
	m_pTransform->SetWorldPos(vPos);	

	CGameObject*	pAttachObject = pCamera->GetAttachObject();

	SAFE_RELEASE(pCamera);

	if (NULL != pAttachObject)
	{
		CTransform*	pTransform = pAttachObject->GetTransform();
		DxVector3 vAttachPos = pTransform->GetWorldPos();
		float fDist = vAttachPos.Distance(vPos);

		if (fDist < m_fMinDist)
		{
			fDist -= m_fMinDist;
			vPos += vAxisZ * fDist;
			m_pTransform->SetWorldPos(vPos);
		}
		else if (fDist > m_fMaxDist)
		{
			fDist -= m_fMaxDist;
			vPos += vAxisZ * fDist;
			m_pTransform->SetWorldPos(vPos);
		}

		SAFE_RELEASE(pTransform);
		SAFE_RELEASE(pAttachObject);
	}
}

void CCameraArm::RotationDrag(float _fTime)
{
	CCamera*	pCamera = m_pGameObject->FindComponentFromTypeID<CCamera>();
	CGameObject*	pAttachObject = pCamera->GetAttachObject();

	if (NULL != pAttachObject)
	{
		CTransform* pTransform = pAttachObject->GetTransform();
		DxVector3 vAttachPos = pTransform->GetWorldPos();

		// 마우스 RButton Drag상태 체크
		if (true == KEYPUSH("RButton"))
		{
			POINT tMouseMove = MOUSEMOVE;
			
			//// X이동
			//if (0 != tMouseMove.x)
			//{
			//	DxVector3	vAxis[AXIS_MAX];
			//	DxVector3	vPos = m_pTransform->GetWorldPos();

			//	for (int i = 0; i < AXIS_MAX; ++i)
			//	{
			//		vAxis[i] = pCamera->GetAxis((AXIS)i);
			//	}

			//	// 마우스가 이동한 양을 이용해서 각도를 구한다
			//	float fAngle = tMouseMove.x * _fTime;

			//	// 마우스가 X로 움직였으니까 Y축 회전
			//	MATRIX	matRot = XMMatrixRotationY(fAngle);

			//	// 축을 다시 구한다
			//	for (int i = 0; i < AXIS_MAX; ++i)
			//	{
			//		vAxis[i] = vAxis[i].TransformNormal(matRot).Normalize();
			//		//vAxis[i] = vAxis[i].Normalize();

			//		pCamera->SetAxis(vAxis);
			//	}

			//	// 행렬의 회전 중점을 AttachObject의 위치로 설정
			//	// 이동에 관련있는 4행에 값을 세팅
			//	matRot._41 = vAttachPos.x;
			//	matRot._42 = vAttachPos.y;
			//	matRot._43 = vAttachPos.z;

			//	// AttachObject로 부터 상대적인 위치를 구한다
			//	DxVector3	vDist = vPos - vAttachPos;
			//	vPos = vDist.TransformCoord(matRot);
			//	m_pTransform->SetWorldPos(vPos);
			//}
			// Y이동
			if (tMouseMove.y != 0)
			{
				DxVector3	vAxis[AXIS_MAX];
				DxVector3	vPos = m_pTransform->GetWorldPos();

				for (int i = 0; i < AXIS_MAX; ++i)
				{
					vAxis[i] = pCamera->GetAxis((AXIS)i);
				}

				// 마우스가 이동한 양을 이용해서 각도를 구한다
				float fAngle = tMouseMove.y * _fTime;

				// 마우스가 Y로 움직였으니까 카메라의 X축 회전
				MATRIX	matRot = XMMatrixRotationAxis(vAxis[AXIS_X].Convert(), fAngle);

				// 축을 다시 구한다
				for (int i = 0; i < AXIS_MAX; ++i)
				{
					vAxis[i] = vAxis[i].TransformNormal(matRot).Normalize();
					//vAxis[i] = vAxis[i].Normalize();

					pCamera->SetAxis(vAxis);
				}

				// 행렬의 회전 중점을 AttachObject의 위치로 설정
				matRot._41 = vAttachPos.x;
				matRot._42 = vAttachPos.y;
				matRot._43 = vAttachPos.z;

				// AttachObject로 부터 상대적인 위치를 구한다
				DxVector3	vDist = vPos - vAttachPos;
				vPos = vDist.TransformCoord(matRot);
				m_pTransform->SetWorldPos(vPos);
			}
		}

		SAFE_RELEASE(pTransform);
		SAFE_RELEASE(pAttachObject);
	}

	SAFE_RELEASE(pCamera);
}

void CCameraArm::SetZoomDistance(float _fMin, float _fMax)
{
	m_fMinDist = _fMin;
	m_fMaxDist = _fMax;
}

void CCameraArm::SetZoomSpeed(float _fSpeed)
{
	m_fZoomSpeed = _fSpeed;
}

void CCameraArm::RotateY(float _fAngle, float _fTime)
{
	CCamera*	pCamera = m_pGameObject->FindComponentFromTypeID<CCamera>();
	CGameObject*	pAttachObject = pCamera->GetAttachObject();

	if (NULL != pAttachObject)
	{
		CTransform* pTransform = pAttachObject->GetTransform();
		DxVector3 vAttachPos = pTransform->GetWorldPos();

		DxVector3	vAxis[AXIS_MAX];
		DxVector3	vPos = m_pTransform->GetWorldPos();

		for (int i = 0; i < AXIS_MAX; ++i)
		{
			vAxis[i] = pCamera->GetAxis((AXIS)i);
		}

		// 회전 각도
		float fAngle = _fAngle * _fTime;

		MATRIX	matRot = XMMatrixRotationY(fAngle);

		// 축을 다시 구한다
		for (int i = 0; i < AXIS_MAX; ++i)
		{
			vAxis[i] = vAxis[i].TransformNormal(matRot).Normalize();


			pCamera->SetAxis(vAxis);
		}

		// 행렬의 회전 중점을 AttachObject의 위치로 설정
		// 이동에 관련있는 4행에 값을 세팅
		matRot._41 = vAttachPos.x;
		matRot._42 = vAttachPos.y;
		matRot._43 = vAttachPos.z;

		// AttachObject로 부터 상대적인 위치를 구한다
		DxVector3	vDist = vPos - vAttachPos;
		vPos = vDist.TransformCoord(matRot);
		m_pTransform->SetWorldPos(vPos);

		SAFE_RELEASE(pTransform);
		SAFE_RELEASE(pAttachObject);
	}

	SAFE_RELEASE(pCamera);
}

bool CCameraArm::Init()
{
	m_fMinDist = 1.0f;
	m_fMaxDist = 5.0f;
	m_fZoomSpeed = 100.0f;

	return true;
}

void CCameraArm::Input(float _fTime)
{
	
}

void CCameraArm::Update(float _fTime)
{
	Zoom(_fTime);
	RotationDrag(_fTime);
}

void CCameraArm::LateUpdate(float _fTime)
{
}

void CCameraArm::Collision(float _fTime)
{
}

void CCameraArm::Render(float _fTime)
{
}

CCameraArm * CCameraArm::Clone()
{
	return new CCameraArm(*this);
}
