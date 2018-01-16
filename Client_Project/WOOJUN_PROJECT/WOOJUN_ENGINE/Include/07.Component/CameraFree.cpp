#include "CameraFree.h"
#include "Camera.h"
#include "Transform.h"
#include "../01.Core/Input.h"
#include "../06.GameObject/GameObject.h"

WOOJUN_USING

CCameraFree::CCameraFree()
{
	SetTag("CameraFree");
	SetTypeName("CCameraFree");
	SetTypeID<CCameraFree>();
	m_eComponentType = CT_CAMERAFREE;
}

CCameraFree::CCameraFree(const CCameraFree & _CameraFree) :
	CComponent(_CameraFree)
{
}

CCameraFree::~CCameraFree()
{
}

void CCameraFree::RotationDrag(float _fTime)
{
	CCamera*	pCamera = m_pGameObject->FindComponentFromTypeID<CCamera>();

	// ���콺 RButton Drag���� üũ
	if (true == KEYPUSH("RButton"))
	{
		POINT tMouseMove = MOUSEMOVE;

		// X�̵�
		if (0 != tMouseMove.x)
		{
			DxVector3	vAxis[AXIS_MAX];
			DxVector3	vPos = m_pTransform->GetWorldPos();

			for (int i = 0; i < AXIS_MAX; ++i)
			{
				vAxis[i] = pCamera->GetAxis((AXIS)i);
			}

			// ���콺�� �̵��� ���� �̿��ؼ� ������ ���Ѵ�
			float fAngle = tMouseMove.x * _fTime;

			// ���콺�� X�� ���������ϱ� Y�� ȸ��
			MATRIX	matRot = XMMatrixRotationY(fAngle);
	
			// ���� �ٽ� ���Ѵ�
			for (int i = 0; i < AXIS_MAX; ++i)
			{
				vAxis[i] = vAxis[i].TransformNormal(matRot).Normalize();
				//vAxis[i] = vAxis[i].Normalize();				

				pCamera->SetAxis(vAxis);
			}

			// ����� ȸ�� ������ AttachObject�� ��ġ�� ����
			// �̵��� �����ִ� 4�࿡ ���� ����
			//matRot._41 = vPos.x;
			//matRot._42 = vPos.y;
			//matRot._43 = vPos.z;
		}
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
			//matRot._41 = vPos.x;
			//matRot._42 = vPos.y;
			//matRot._43 = vPos.z;		
		}		
	}

	SAFE_RELEASE(pCamera);
}

bool CCameraFree::Init()
{
	return true;
}

void CCameraFree::Input(float _fTime)
{
	if (true == KEYPRESS("CameraForward"))
	{
		m_pTransform->Forward(10.0f, _fTime);
	}
	if (true == KEYPRESS("CameraBack"))
	{
		m_pTransform->Forward(-10.0f, _fTime);
	}
	if (true == KEYPRESS("CameraLeft"))
	{
		m_pTransform->Right(-10.0f, _fTime);
	}
	if (true == KEYPRESS("CameraRight"))
	{
		m_pTransform->Right(10.0f, _fTime);
	}
}

void CCameraFree::Update(float _fTime)
{
	RotationDrag(_fTime);
}

void CCameraFree::LateUpdate(float _fTime)
{
}

void CCameraFree::Collision(float _fTime)
{
}

void CCameraFree::Render(float _fTime)
{
}

CCameraFree * CCameraFree::Clone()
{
	return new CCameraFree(*this);
}
