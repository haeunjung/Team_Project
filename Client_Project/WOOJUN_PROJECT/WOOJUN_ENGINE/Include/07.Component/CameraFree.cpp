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

	// 마우스 RButton Drag상태 체크
	if (true == KEYPUSH("RButton"))
	{
		POINT tMouseMove = MOUSEMOVE;

		// X이동
		if (0 != tMouseMove.x)
		{
			DxVector3	vAxis[AXIS_MAX];
			DxVector3	vPos = m_pTransform->GetWorldPos();

			for (int i = 0; i < AXIS_MAX; ++i)
			{
				vAxis[i] = pCamera->GetAxis((AXIS)i);
			}

			// 마우스가 이동한 양을 이용해서 각도를 구한다
			float fAngle = tMouseMove.x * _fTime;

			// 마우스가 X로 움직였으니까 Y축 회전
			MATRIX	matRot = XMMatrixRotationY(fAngle);
	
			// 축을 다시 구한다
			for (int i = 0; i < AXIS_MAX; ++i)
			{
				vAxis[i] = vAxis[i].TransformNormal(matRot).Normalize();
				//vAxis[i] = vAxis[i].Normalize();				

				pCamera->SetAxis(vAxis);
			}

			// 행렬의 회전 중점을 AttachObject의 위치로 설정
			// 이동에 관련있는 4행에 값을 세팅
			//matRot._41 = vPos.x;
			//matRot._42 = vPos.y;
			//matRot._43 = vPos.z;
		}
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
