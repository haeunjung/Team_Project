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
	// ī�޶� �پ��ִ� ������Ʈ�� ���� ī�޶��� �Ÿ��� ���Ѵ�.
	CCamera*	pCamera = m_pGameObject->FindComponentFromTypeID<CCamera>();

	// Zoom�Ÿ�	=	����	*	�ӵ�	*	�ð�
	float fSpeed = WHEELDIR * m_fZoomSpeed * _fTime;
	// ī�޶��� ���� ��ġ
	DxVector3	vPos = m_pTransform->GetWorldPos();
	// ī�޶��� ���� Z��
	DxVector3	vAxisZ = pCamera->GetAxis(AXIS_Z);

	// ���
	vPos += vAxisZ * fSpeed;
	// ����
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

		// ���콺 RButton Drag���� üũ
		if (true == KEYPUSH("RButton"))
		{
			POINT tMouseMove = MOUSEMOVE;
			
			//// X�̵�
			//if (0 != tMouseMove.x)
			//{
			//	DxVector3	vAxis[AXIS_MAX];
			//	DxVector3	vPos = m_pTransform->GetWorldPos();

			//	for (int i = 0; i < AXIS_MAX; ++i)
			//	{
			//		vAxis[i] = pCamera->GetAxis((AXIS)i);
			//	}

			//	// ���콺�� �̵��� ���� �̿��ؼ� ������ ���Ѵ�
			//	float fAngle = tMouseMove.x * _fTime;

			//	// ���콺�� X�� ���������ϱ� Y�� ȸ��
			//	MATRIX	matRot = XMMatrixRotationY(fAngle);

			//	// ���� �ٽ� ���Ѵ�
			//	for (int i = 0; i < AXIS_MAX; ++i)
			//	{
			//		vAxis[i] = vAxis[i].TransformNormal(matRot).Normalize();
			//		//vAxis[i] = vAxis[i].Normalize();

			//		pCamera->SetAxis(vAxis);
			//	}

			//	// ����� ȸ�� ������ AttachObject�� ��ġ�� ����
			//	// �̵��� �����ִ� 4�࿡ ���� ����
			//	matRot._41 = vAttachPos.x;
			//	matRot._42 = vAttachPos.y;
			//	matRot._43 = vAttachPos.z;

			//	// AttachObject�� ���� ������� ��ġ�� ���Ѵ�
			//	DxVector3	vDist = vPos - vAttachPos;
			//	vPos = vDist.TransformCoord(matRot);
			//	m_pTransform->SetWorldPos(vPos);
			//}
			// Y�̵�
			if (tMouseMove.y != 0)
			{
				DxVector3	vAxis[AXIS_MAX];
				DxVector3	vPos = m_pTransform->GetWorldPos();

				for (int i = 0; i < AXIS_MAX; ++i)
				{
					vAxis[i] = pCamera->GetAxis((AXIS)i);
				}

				// ���콺�� �̵��� ���� �̿��ؼ� ������ ���Ѵ�
				float fAngle = tMouseMove.y * _fTime;

				// ���콺�� Y�� ���������ϱ� ī�޶��� X�� ȸ��
				MATRIX	matRot = XMMatrixRotationAxis(vAxis[AXIS_X].Convert(), fAngle);

				// ���� �ٽ� ���Ѵ�
				for (int i = 0; i < AXIS_MAX; ++i)
				{
					vAxis[i] = vAxis[i].TransformNormal(matRot).Normalize();
					//vAxis[i] = vAxis[i].Normalize();

					pCamera->SetAxis(vAxis);
				}

				// ����� ȸ�� ������ AttachObject�� ��ġ�� ����
				matRot._41 = vAttachPos.x;
				matRot._42 = vAttachPos.y;
				matRot._43 = vAttachPos.z;

				// AttachObject�� ���� ������� ��ġ�� ���Ѵ�
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

		// ȸ�� ����
		float fAngle = _fAngle * _fTime;

		MATRIX	matRot = XMMatrixRotationY(fAngle);

		// ���� �ٽ� ���Ѵ�
		for (int i = 0; i < AXIS_MAX; ++i)
		{
			vAxis[i] = vAxis[i].TransformNormal(matRot).Normalize();


			pCamera->SetAxis(vAxis);
		}

		// ����� ȸ�� ������ AttachObject�� ��ġ�� ����
		// �̵��� �����ִ� 4�࿡ ���� ����
		matRot._41 = vAttachPos.x;
		matRot._42 = vAttachPos.y;
		matRot._43 = vAttachPos.z;

		// AttachObject�� ���� ������� ��ġ�� ���Ѵ�
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
