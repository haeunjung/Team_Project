#include "Transform.h"
#include "06.GameObject/GameObject.h"

WOOJUN_USING

DxVector3 CTransform::GetLocalScale() const
{
	return m_vLocalScale;
}

DxVector3 CTransform::GetLocalRot() const
{
	return m_vLocalRot;
}

DxVector3 CTransform::GetLocalPos() const
{
	return m_vLocalPos;
}

DxVector3 CTransform::GetPivot() const
{
	return m_vPivot;
}

DxVector3 CTransform::GetWorldScale() const
{
	return m_vWorldScale;
}

DxVector3 CTransform::GetWorldRot() const
{
	return m_vWorldRot;
}

DxVector3 CTransform::GetWorldPos() const
{
	return m_vWorldPos;
}

DxVector3 CTransform::GetWorldHierarchyRot() const
{
	if (NULL == m_pParentTransform)
	{
		return m_vWorldRot;
	}

	return m_vWorldRot + m_pParentTransform->GetWorldHierarchyRot();
}

DxVector3 CTransform::GetWorldAxis(AXIS _Axis) const
{
	return m_vWorldAxis[_Axis];
}

CTransform * CTransform::GetParentTransform() const
{
	if (NULL != m_pParentTransform)
	{
		m_pParentTransform->AddRef();
	}

	return m_pParentTransform;
}

void CTransform::SetWorldAxis(AXIS _eAxis, const DxVector3 & _vAxis)
{
	m_vWorldAxis[_eAxis] = _vAxis;
}

void CTransform::SetParentTransform(CTransform * _pParentTransform)
{
	m_pParentTransform = _pParentTransform;
}

void CTransform::ActiveUpdate()
{
	m_bUpdate = true;

	m_pGameObject->ChildTransformActiveUpdate();
}

void CTransform::ComputeWorldAxis()
{
	for (int i = 0; i < AXIS_MAX; ++i)
	{
		m_vWorldAxis[i] = WORLDAXIS[i].TransformNormal(GetParentRotMatrix()).Normalize();
	}

	list<CGameObject*>* pChildList = m_pGameObject->GetChildList();

	list<CGameObject*>::iterator iter;
	list<CGameObject*>::iterator iterEnd = pChildList->end();

	for (iter = pChildList->begin(); iter != iterEnd; ++iter)
	{
		CTransform* pTransform = (*iter)->GetTransform();

		pTransform->ComputeWorldAxis();

		SAFE_RELEASE(pTransform);
	}
}

void CTransform::SetLocalScale(const Vector3 & v)
{
	m_vLocalScale = v;
	*m_matLocalScale = XMMatrixScaling(m_vLocalScale.x, m_vLocalScale.y, m_vLocalScale.z);

	ActiveUpdate();
}

void CTransform::SetLocalScale(const DxVector3 & v)
{
	m_vLocalScale = v;
	*m_matLocalScale = XMMatrixScaling(m_vLocalScale.x, m_vLocalScale.y, m_vLocalScale.z);

	ActiveUpdate();
}

void CTransform::SetLocalScale(const XMVECTOR & v)
{
	m_vLocalScale = v;
	*m_matLocalScale = XMMatrixScaling(m_vLocalScale.x, m_vLocalScale.y, m_vLocalScale.z);

	ActiveUpdate();
}

void CTransform::SetLocalScale(float x, float y, float z)
{
	m_vLocalScale = Vector3(x, y, z);
	*m_matLocalScale = XMMatrixScaling(m_vLocalScale.x, m_vLocalScale.y, m_vLocalScale.z);

	ActiveUpdate();
}

void CTransform::SetLocalScale(float f[3])
{
	m_vLocalScale = f;
	*m_matLocalScale = XMMatrixScaling(m_vLocalScale.x, m_vLocalScale.y, m_vLocalScale.z);

	ActiveUpdate();
}

void CTransform::SetLocalScaleX(float x)
{
	m_vLocalScale.x = x;
	*m_matLocalScale = XMMatrixScaling(m_vLocalScale.x, m_vLocalScale.y, m_vLocalScale.z);

	ActiveUpdate();
}

void CTransform::SetLocalScaleY(float y)
{
	m_vLocalScale.y = y;
	*m_matLocalScale = XMMatrixScaling(m_vLocalScale.x, m_vLocalScale.y, m_vLocalScale.z);

	ActiveUpdate();
}

void CTransform::SetLocalScaleZ(float z)
{
	m_vLocalScale.z = z;
	*m_matLocalScale = XMMatrixScaling(m_vLocalScale.x, m_vLocalScale.y, m_vLocalScale.z);

	ActiveUpdate();
}

void CTransform::SetLocalRot(const Vector3 & v)
{
	m_vLocalRot = v;

	*m_matLocalRotX = XMMatrixRotationX(m_vLocalRot.x);
	*m_matLocalRotY = XMMatrixRotationY(m_vLocalRot.y);
	*m_matLocalRotZ = XMMatrixRotationZ(m_vLocalRot.z);

	*m_matLocalRot = (*m_matLocalRotX) * (*m_matLocalRotY) * (*m_matLocalRotZ);

	for (int i = 0; i < AXIS_MAX; ++i)
	{
		m_vLocalAxis[i] = WORLDAXIS[i].TransformNormal(*m_matLocalRot).Normalize();
	}

	ActiveUpdate();
}

void CTransform::SetLocalRot(const DxVector3 & v)
{
	m_vLocalRot = v;

	*m_matLocalRotX = XMMatrixRotationX(m_vLocalRot.x);
	*m_matLocalRotY = XMMatrixRotationY(m_vLocalRot.y);
	*m_matLocalRotZ = XMMatrixRotationZ(m_vLocalRot.z);

	*m_matLocalRot = (*m_matLocalRotX) * (*m_matLocalRotY) * (*m_matLocalRotZ);

	for (int i = 0; i < AXIS_MAX; ++i)
	{
		m_vLocalAxis[i] = WORLDAXIS[i].TransformNormal(*m_matLocalRot).Normalize();
	}

	ActiveUpdate();
}

void CTransform::SetLocalRot(const XMVECTOR & v)
{
	m_vLocalRot = v;

	*m_matLocalRotX = XMMatrixRotationX(m_vLocalRot.x);
	*m_matLocalRotY = XMMatrixRotationY(m_vLocalRot.y);
	*m_matLocalRotZ = XMMatrixRotationZ(m_vLocalRot.z);

	*m_matLocalRot = (*m_matLocalRotX) * (*m_matLocalRotY) * (*m_matLocalRotZ);

	for (int i = 0; i < AXIS_MAX; ++i)
	{
		m_vLocalAxis[i] = WORLDAXIS[i].TransformNormal(*m_matLocalRot).Normalize();
	}

	ActiveUpdate();
}

void CTransform::SetLocalRot(float x, float y, float z)
{
	m_vLocalRot = Vector3(x, y, z);

	*m_matLocalRotX = XMMatrixRotationX(m_vLocalRot.x);
	*m_matLocalRotY = XMMatrixRotationY(m_vLocalRot.y);
	*m_matLocalRotZ = XMMatrixRotationZ(m_vLocalRot.z);

	*m_matLocalRot = (*m_matLocalRotX) * (*m_matLocalRotY) * (*m_matLocalRotZ);

	for (int i = 0; i < AXIS_MAX; ++i)
	{
		m_vLocalAxis[i] = WORLDAXIS[i].TransformNormal(*m_matLocalRot).Normalize();
	}

	ActiveUpdate();
}

void CTransform::SetLocalRot(float f[3])
{
	m_vLocalRot = f;

	*m_matLocalRotX = XMMatrixRotationX(m_vLocalRot.x);
	*m_matLocalRotY = XMMatrixRotationY(m_vLocalRot.y);
	*m_matLocalRotZ = XMMatrixRotationZ(m_vLocalRot.z);

	*m_matLocalRot = (*m_matLocalRotX) * (*m_matLocalRotY) * (*m_matLocalRotZ);

	for (int i = 0; i < AXIS_MAX; ++i)
	{
		m_vLocalAxis[i] = WORLDAXIS[i].TransformNormal(*m_matLocalRot).Normalize();
	}

	ActiveUpdate();
}

void CTransform::SetLocalRotX(float x)
{
	m_vLocalRot.x = x;
	*m_matLocalRotX = XMMatrixRotationX(m_vLocalRot.x);

	*m_matLocalRot = (*m_matLocalRotX) * (*m_matLocalRotY) * (*m_matLocalRotZ);

	for (int i = 0; i < AXIS_MAX; ++i)
	{
		m_vLocalAxis[i] = WORLDAXIS[i].TransformNormal(*m_matLocalRot).Normalize();
	}

	ActiveUpdate();
}

void CTransform::SetLocalRotY(float y)
{
	m_vLocalRot.y = y;
	*m_matLocalRotY = XMMatrixRotationY(m_vLocalRot.y);

	*m_matLocalRot = (*m_matLocalRotX) * (*m_matLocalRotY) * (*m_matLocalRotZ);

	for (int i = 0; i < AXIS_MAX; ++i)
	{
		m_vLocalAxis[i] = WORLDAXIS[i].TransformNormal(*m_matLocalRot).Normalize();
	}

	ActiveUpdate();
}

void CTransform::SetLocalRotZ(float z)
{
	m_vLocalRot.z = z;
	*m_matLocalRotZ = XMMatrixRotationZ(m_vLocalRot.z);

	*m_matLocalRot = (*m_matLocalRotX) * (*m_matLocalRotY) * (*m_matLocalRotZ);

	for (int i = 0; i < AXIS_MAX; ++i)
	{
		m_vLocalAxis[i] = WORLDAXIS[i].TransformNormal(*m_matLocalRot).Normalize();
	}

	ActiveUpdate();
}

void CTransform::SetLocalPos(const Vector3 & v)
{
	m_vLocalPos = v;
	*m_matLocalPos = XMMatrixTranslation(m_vLocalPos.x, m_vLocalPos.y, m_vLocalPos.z);

	ActiveUpdate();
}

void CTransform::SetLocalPos(const DxVector3 & v)
{
	m_vLocalPos = v;
	*m_matLocalPos = XMMatrixTranslation(m_vLocalPos.x, m_vLocalPos.y, m_vLocalPos.z);

	ActiveUpdate();
}

void CTransform::SetLocalPos(const XMVECTOR & v)
{
	m_vLocalPos = v;
	*m_matLocalPos = XMMatrixTranslation(m_vLocalPos.x, m_vLocalPos.y, m_vLocalPos.z);

	ActiveUpdate();
}

void CTransform::SetLocalPos(float x, float y, float z)
{
	m_vLocalPos = Vector3(x, y, z);
	*m_matLocalPos = XMMatrixTranslation(m_vLocalPos.x, m_vLocalPos.y, m_vLocalPos.z);

	ActiveUpdate();
}

void CTransform::SetLocalPos(float f[3])
{
	m_vLocalPos = f;
	*m_matLocalPos = XMMatrixTranslation(m_vLocalPos.x, m_vLocalPos.y, m_vLocalPos.z);

	ActiveUpdate();
}

void CTransform::SetLocalPosX(float x)
{
	m_vLocalPos.x = x;
	*m_matLocalPos = XMMatrixTranslation(m_vLocalPos.x, m_vLocalPos.y, m_vLocalPos.z);

	ActiveUpdate();
}

void CTransform::SetLocalPosY(float y)
{
	m_vLocalPos.y = y;
	*m_matLocalPos = XMMatrixTranslation(m_vLocalPos.x, m_vLocalPos.y, m_vLocalPos.z);

	ActiveUpdate();
}

void CTransform::SetLocalPosZ(float z)
{
	m_vLocalPos.z = z;
	*m_matLocalPos = XMMatrixTranslation(m_vLocalPos.x, m_vLocalPos.y, m_vLocalPos.z);

	ActiveUpdate();
}

void CTransform::SetPivot(const DxVector3 & v)
{
	m_vPivot = v;
}

void CTransform::SetPivot(float x, float y, float z)
{
	m_vPivot = DxVector3(x, y, z);
}

void CTransform::SetWorldScale(const Vector3 & v)
{
	m_vWorldScale = v;
	*m_matWorldScale = XMMatrixScaling(m_vWorldScale.x, m_vWorldScale.y, m_vWorldScale.z);

	ActiveUpdate();
}

void CTransform::SetWorldScale(const DxVector3 & v)
{
	m_vWorldScale = v;
	*m_matWorldScale = XMMatrixScaling(m_vWorldScale.x, m_vWorldScale.y, m_vWorldScale.z);

	ActiveUpdate();
}

void CTransform::SetWorldScale(const XMVECTOR & v)
{
	m_vWorldScale = v;
	*m_matWorldScale = XMMatrixScaling(m_vWorldScale.x, m_vWorldScale.y, m_vWorldScale.z);

	ActiveUpdate();
}

void CTransform::SetWorldScale(float x, float y, float z)
{
	m_vWorldScale = Vector3(x, y, z);
	*m_matWorldScale = XMMatrixScaling(m_vWorldScale.x, m_vWorldScale.y, m_vWorldScale.z);

	ActiveUpdate();
}

void CTransform::SetWorldScale(float f[3])
{
	m_vWorldScale = f;
	*m_matWorldScale = XMMatrixScaling(m_vWorldScale.x, m_vWorldScale.y, m_vWorldScale.z);

	ActiveUpdate();
}

void CTransform::SetWorldScaleX(float x)
{
	m_vWorldScale.x = x;
	*m_matWorldScale = XMMatrixScaling(m_vWorldScale.x, m_vWorldScale.y, m_vWorldScale.z);

	ActiveUpdate();
}

void CTransform::SetWorldScaleY(float y)
{
	m_vWorldScale.y = y;
	*m_matWorldScale = XMMatrixScaling(m_vWorldScale.x, m_vWorldScale.y, m_vWorldScale.z);

	ActiveUpdate();
}

void CTransform::SetWorldScaleZ(float z)
{
	m_vWorldScale.z = z;
	*m_matWorldScale = XMMatrixScaling(m_vWorldScale.x, m_vWorldScale.y, m_vWorldScale.z);

	ActiveUpdate();
}

void CTransform::SetWorldRot(const Vector3 & v)
{
	m_vWorldRot = v;

	*m_matWorldRotX = XMMatrixRotationX(m_vWorldRot.x);
	*m_matWorldRotY = XMMatrixRotationY(m_vWorldRot.y);
	*m_matWorldRotZ = XMMatrixRotationZ(m_vWorldRot.z);

	*m_matWorldRot = *m_matWorldRotX * *m_matWorldRotY * *m_matWorldRotZ;

	ComputeWorldAxis();

	ActiveUpdate();
}

void CTransform::SetWorldRot(const DxVector3 & v)
{
	m_vWorldRot = v;

	*m_matWorldRotX = XMMatrixRotationX(m_vWorldRot.x);
	*m_matWorldRotY = XMMatrixRotationY(m_vWorldRot.y);
	*m_matWorldRotZ = XMMatrixRotationZ(m_vWorldRot.z);

	*m_matWorldRot = *m_matWorldRotX * *m_matWorldRotY * *m_matWorldRotZ;

	ComputeWorldAxis();

	ActiveUpdate();
}

void CTransform::SetWorldRot(const XMVECTOR & v)
{
	m_vWorldRot = v;

	*m_matWorldRotX = XMMatrixRotationX(m_vWorldRot.x);
	*m_matWorldRotY = XMMatrixRotationY(m_vWorldRot.y);
	*m_matWorldRotZ = XMMatrixRotationZ(m_vWorldRot.z);

	*m_matWorldRot = *m_matWorldRotX * *m_matWorldRotY * *m_matWorldRotZ;

	ComputeWorldAxis();

	ActiveUpdate();
}

void CTransform::SetWorldRot(float x, float y, float z)
{
	m_vWorldRot = Vector3(x, y, z);

	*m_matWorldRotX = XMMatrixRotationX(m_vWorldRot.x);
	*m_matWorldRotY = XMMatrixRotationY(m_vWorldRot.y);
	*m_matWorldRotZ = XMMatrixRotationZ(m_vWorldRot.z);

	*m_matWorldRot = *m_matWorldRotX * *m_matWorldRotY * *m_matWorldRotZ;

	ComputeWorldAxis();

	ActiveUpdate();
}

void CTransform::SetWorldRot(float f[3])
{
	m_vWorldRot = f;

	*m_matWorldRotX = XMMatrixRotationX(m_vWorldRot.x);
	*m_matWorldRotY = XMMatrixRotationY(m_vWorldRot.y);
	*m_matWorldRotZ = XMMatrixRotationZ(m_vWorldRot.z);

	*m_matWorldRot = *m_matWorldRotX * *m_matWorldRotY * *m_matWorldRotZ;

	ComputeWorldAxis();

	ActiveUpdate();
}

void CTransform::SetWorldRotX(float x)
{
	m_vWorldRot.x = x;
	*m_matWorldRotX = XMMatrixRotationX(m_vWorldRot.x);

	*m_matWorldRot = *m_matWorldRotX * *m_matWorldRotY * *m_matWorldRotZ;

	ComputeWorldAxis();

	ActiveUpdate();
}

void CTransform::SetWorldRotY(float y)
{
	m_vWorldRot.y = y;
	*m_matWorldRotY = XMMatrixRotationY(m_vWorldRot.y);

	*m_matWorldRot = *m_matWorldRotX * *m_matWorldRotY * *m_matWorldRotZ;

	ComputeWorldAxis();

	ActiveUpdate();
}

void CTransform::SetWorldRotZ(float z)
{
	m_vWorldRot.z = z;
	*m_matWorldRotZ = XMMatrixRotationZ(m_vWorldRot.z);

	*m_matWorldRot = *m_matWorldRotX * *m_matWorldRotY * *m_matWorldRotZ;

	ComputeWorldAxis();

	ActiveUpdate();
}

void CTransform::SetWorldPos(const Vector3 & v)
{
	m_vWorldPos = v;
	*m_matWorldPos = XMMatrixTranslation(m_vWorldPos.x, m_vWorldPos.y, m_vWorldPos.z);

	ActiveUpdate();
}

void CTransform::SetWorldPos(const DxVector3 & v)
{
	m_vWorldPos = v;
	*m_matWorldPos = XMMatrixTranslation(m_vWorldPos.x, m_vWorldPos.y, m_vWorldPos.z);

	ActiveUpdate();
}

void CTransform::SetWorldPos(const XMVECTOR & v)
{
	m_vWorldPos = v;
	*m_matWorldPos = XMMatrixTranslation(m_vWorldPos.x, m_vWorldPos.y, m_vWorldPos.z);

	ActiveUpdate();
}

void CTransform::SetWorldPos(float x, float y, float z)
{
	m_vWorldPos = Vector3(x, y, z);
	*m_matWorldPos = XMMatrixTranslation(m_vWorldPos.x, m_vWorldPos.y, m_vWorldPos.z);

	ActiveUpdate();
}

void CTransform::SetWorldPos(float f[3])
{
	m_vWorldPos = f;
	*m_matWorldPos = XMMatrixTranslation(m_vWorldPos.x, m_vWorldPos.y, m_vWorldPos.z);

	ActiveUpdate();
}

void CTransform::SetWorldPosX(float x)
{
	m_vWorldPos.x = x;
	*m_matWorldPos = XMMatrixTranslation(m_vWorldPos.x, m_vWorldPos.y, m_vWorldPos.z);

	ActiveUpdate();
}

void CTransform::SetWorldPosY(float y)
{
	m_vWorldPos.y = y;
	*m_matWorldPos = XMMatrixTranslation(m_vWorldPos.x, m_vWorldPos.y, m_vWorldPos.z);

	ActiveUpdate();
}

void CTransform::SetWorldPosZ(float z)
{
	m_vWorldPos.z = z;
	*m_matWorldPos = XMMatrixTranslation(m_vWorldPos.x, m_vWorldPos.y, m_vWorldPos.z);

	ActiveUpdate();
}

MATRIX CTransform::GetLocalMatrix() const
{
	return *m_matLocal;
}

MATRIX CTransform::GetWorldRotMatrix() const
{
	return *m_matWorldRot;
}

MATRIX CTransform::GetWorldPosMatrix() const
{
	return *m_matWorldPos;
}

MATRIX CTransform::GetWorldMatrix() const
{
	return *m_matWorld;
}

MATRIX CTransform::GetParentMatrix() const
{
	if (NULL != m_pParentTransform)
	{
		*m_matParent = m_pParentTransform->GetWorldMatrix();
	}

	return *m_matParent;
}

MATRIX CTransform::GetParentRotMatrix() const
{
	MATRIX	matParent;

	if (NULL != m_pParentTransform)
	{
		matParent = m_pParentTransform->GetParentRotMatrix();
	}

	return *m_matWorldRot * matParent;
}

void CTransform::SetWorldRotMatrix(MATRIX & _Mat)
{
	*m_matWorldRot = _Mat;

	ComputeWorldAxis();

	ActiveUpdate();
}

void CTransform::Forward(float _fSpeed, float _fTime)
{
	m_vWorldPos += m_vWorldAxis[AXIS_Z] * _fSpeed * _fTime;

	*m_matWorldPos = XMMatrixTranslation(m_vWorldPos.x, m_vWorldPos.y, m_vWorldPos.z);

	ActiveUpdate();
}

void CTransform::Forward(float _fSpeed)
{
	m_vWorldPos += m_vWorldAxis[AXIS_Z] * _fSpeed;

	*m_matWorldPos = XMMatrixTranslation(m_vWorldPos.x, m_vWorldPos.y, m_vWorldPos.z);

	ActiveUpdate();
}

void CTransform::Up(float _fSpeed, float _fTime)
{
	m_vWorldPos += m_vWorldAxis[AXIS_Y] * _fSpeed * _fTime;

	*m_matWorldPos = XMMatrixTranslation(m_vWorldPos.x, m_vWorldPos.y, m_vWorldPos.z);

	ActiveUpdate();
}

void CTransform::Up(float _fSpeed)
{
	m_vWorldPos += m_vWorldAxis[AXIS_Y] * _fSpeed;

	*m_matWorldPos = XMMatrixTranslation(m_vWorldPos.x, m_vWorldPos.y, m_vWorldPos.z);

	ActiveUpdate();
}

void CTransform::Right(float _fSpeed, float _fTime)
{
	m_vWorldPos += m_vWorldAxis[AXIS_X] * _fSpeed * _fTime;

	*m_matWorldPos = XMMatrixTranslation(m_vWorldPos.x, m_vWorldPos.y, m_vWorldPos.z);

	ActiveUpdate();
}

void CTransform::Right(float _fSpeed)
{
	m_vWorldPos += m_vWorldAxis[AXIS_X] * _fSpeed;

	*m_matWorldPos = XMMatrixTranslation(m_vWorldPos.x, m_vWorldPos.y, m_vWorldPos.z);

	ActiveUpdate();
}

void CTransform::Move(const DxVector3 & _vDir, float _fSpeed)
{
	m_vWorldPos += _vDir * _fSpeed;

	*m_matWorldPos = XMMatrixTranslation(m_vWorldPos.x, m_vWorldPos.y,
		m_vWorldPos.z);

	ActiveUpdate();
}

void CTransform::Move(const DxVector3 & _vDir, float _fSpeed, float _fTime)
{
	m_vWorldPos += _vDir * _fSpeed * _fTime;

	*m_matWorldPos = XMMatrixTranslation(m_vWorldPos.x, m_vWorldPos.y,
		m_vWorldPos.z);

	ActiveUpdate();
}
void CTransform::Move(const DxVector3 & _vMove)
{
	m_vWorldPos += _vMove;

	*m_matWorldPos = XMMatrixTranslation(m_vWorldPos.x, m_vWorldPos.y, m_vWorldPos.z);

	ActiveUpdate();
}
void CTransform::Rotate(const DxVector3 & vRot, bool bLocal)
{
	if (bLocal)
	{
		SetLocalRot(m_vLocalRot + vRot);
	}

	else
	{
		SetWorldRot(m_vWorldRot + vRot);
	}
}

void CTransform::Rotate(float x, float y, float z, bool bLocal)
{
	if (bLocal)
	{
		SetLocalRot(m_vLocalRot + DxVector3(x, y, z));
	}

	else
	{
		SetWorldRot(m_vWorldRot + DxVector3(x, y, z));
	}
}

void CTransform::RotateX(float x, bool bLocal)
{
	if (bLocal)
	{
		SetLocalRotX(m_vLocalRot.x + x);
	}

	else
	{
		SetWorldRotX(m_vWorldRot.x + x);
	}
}

void CTransform::RotateY(float y, bool bLocal)
{
	if (bLocal)
	{
		SetLocalRotY(m_vLocalRot.y + y);
	}

	else
	{
		SetWorldRotY(m_vWorldRot.y + y);
	}
}

void CTransform::RotateZ(float z, bool bLocal)
{
	if (bLocal)
	{
		SetLocalRotZ(m_vLocalRot.z + z);
	}

	else
	{
		SetWorldRotZ(m_vWorldRot.z + z);
	}
}

void CTransform::Rotate(const DxVector3 & vRot, float fTime, bool bLocal)
{
	if (bLocal)
	{
		SetLocalRot(m_vLocalRot + vRot * fTime);
	}

	else
	{
		SetWorldRot(m_vWorldRot + vRot * fTime);
	}
}

void CTransform::Rotate(float x, float y, float z, float fTime, bool bLocal)
{
	if (bLocal)
	{
		SetLocalRot(m_vLocalRot + DxVector3(x, y, z) * fTime);
	}

	else
	{
		SetWorldRot(m_vWorldRot + DxVector3(x, y, z) * fTime);
	}
}

void CTransform::RotateX(float x, float fTime, bool bLocal)
{
	if (bLocal)
	{
		SetLocalRotX(m_vLocalRot.x + x * fTime);
	}

	else
	{
		SetWorldRotX(m_vWorldRot.x + x * fTime);
	}
}

void CTransform::RotateY(float y, float fTime, bool bLocal)
{
	if (bLocal)
	{
		SetLocalRotY(m_vLocalRot.y + y * fTime);
	}

	else
	{
		SetWorldRotY(m_vWorldRot.y + y * fTime);
	}
}

void CTransform::RotateZ(float z, float fTime, bool bLocal)
{
	if (bLocal)
	{
		SetLocalRotZ(m_vLocalRot.z + z * fTime);
	}

	else
	{
		SetWorldRotZ(m_vWorldRot.z + z * fTime);
	}
}

void CTransform::LookAt(CTransform * _pTransform, AXIS _Axis)
{	
	DxVector3 vView = _pTransform->GetWorldPos() - m_vWorldPos;
	vView = vView.Normalize();

	DxVector3 vAxis = WORLDAXIS[_Axis];

	DxVector3 vRotAxis = vAxis.Cross(vView);
	float fAngle = vAxis.GetAngle(vView);

	// 외적값이 0이면 XMMatrixRotationAxis에서 터진다.
	if (DxVector3(0.0f, 0.0f, 0.0f) != vRotAxis)
	{
		// XMMatrixRotationAxis
		// 지정 축, 지정 각도로 회전행렬을 만들어준다.
		*m_matWorldRot = XMMatrixRotationAxis(vRotAxis.Convert(), fAngle);

		ComputeWorldAxis();

		ActiveUpdate();
	}
}

void CTransform::LookAt(CGameObject * _pGameObject, AXIS _Axis)
{
	CTransform * pTransform = _pGameObject->GetTransform();

	DxVector3 vView = pTransform->GetWorldPos() - m_vWorldPos;
	vView = vView.Normalize();

	DxVector3	vAxis = WORLDAXIS[_Axis];

	DxVector3	vRotAxis = vAxis.Cross(vView);
	float	fAngle = vAxis.GetAngle(vView);
		
	SAFE_RELEASE(pTransform);

	// 외적값이 0이면 XMMatrixRotationAxis에서 터진다.
	if (DxVector3(0.0f, 0.0f, 0.0f) != vRotAxis)
	{
		// XMMatrixRotationAxis
		// 지정 축, 지정 각도로 회전행렬을 만들어준다.
		*m_matWorldRot = XMMatrixRotationAxis(vRotAxis.Convert(), fAngle);

		ComputeWorldAxis();

		ActiveUpdate();
	}	
}

void CTransform::CopyTransform(CTransform * _pTransform)
{
	m_vLocalScale = _pTransform->m_vLocalScale;
	m_vLocalRot = _pTransform->m_vLocalRot;
	m_vLocalPos = _pTransform->m_vLocalPos;

	m_vWorldScale = _pTransform->m_vWorldScale;
	SetWorldRot(_pTransform->GetWorldHierarchyRot());
	m_vWorldPos = _pTransform->m_vWorldPos;

	for (int i = 0; i < AXIS_MAX; ++i)
	{
		m_vLocalAxis[i] = _pTransform->m_vLocalAxis[i];
		//m_vWorldAxis[i] = pTransform->m_vWorldAxis[i];
	}

	*m_matLocalScale = *_pTransform->m_matLocalScale;
	*m_matLocalRotX = *_pTransform->m_matLocalRotX;
	*m_matLocalRotY = *_pTransform->m_matLocalRotY;
	*m_matLocalRotZ = *_pTransform->m_matLocalRotZ;
	*m_matLocalRot = *_pTransform->m_matLocalRot;
	*m_matLocalPos = *_pTransform->m_matLocalPos;
	*m_matLocal = *_pTransform->m_matLocal;

	*m_matWorldScale = *_pTransform->m_matWorldScale;
	//*m_matWorldRotX = *pTransform->m_matWorldRotX;
	//*m_matWorldRotY = *pTransform->m_matWorldRotY;
	//*m_matWorldRotZ = *pTransform->m_matWorldRotZ;
	//*m_matWorldRot = *pTransform->m_matWorldRot;
	*m_matWorldPos = *_pTransform->m_matWorldPos;
	//*m_matParent = *pTransform->m_matParent;
	*m_matWorld = *_pTransform->m_matWorld;

	// 최종 월드상에서의 위치를 구한다.
	m_vWorldPos = m_vWorldPos.TransformCoord(*_pTransform->m_matParent);
}

void CTransform::ComputeWorldMatrix()
{
	if (NULL != m_pParentTransform)
	{
		*m_matParent = m_pParentTransform->GetWorldMatrix();
	}

	*m_matLocal = (*m_matLocalScale) * (*m_matLocalRot) * (*m_matLocalPos);
	*m_matWorld = (*m_matWorldScale) * (*m_matWorldRot) * (*m_matWorldPos) * (*m_matParent);
}

bool CTransform::Init()
{
	m_matLocalScale = new MATRIX;
	m_matLocalRotX = new MATRIX;
	m_matLocalRotY = new MATRIX;
	m_matLocalRotZ = new MATRIX;
	m_matLocalRot = new MATRIX;
	m_matLocalPos = new MATRIX;
	m_matLocal = new MATRIX;

	m_matWorldScale = new MATRIX;
	m_matWorldRotX = new MATRIX;
	m_matWorldRotY = new MATRIX;
	m_matWorldRotZ = new MATRIX;
	m_matWorldRot = new MATRIX;
	m_matWorldPos = new MATRIX;
	m_matParent = new MATRIX;
	m_matWorld = new MATRIX;

	for (int i = 0; i < AXIS_MAX; ++i)
	{
		m_vLocalAxis[i] = WORLDAXIS[i];
		m_vWorldAxis[i] = WORLDAXIS[i];
	}

	m_vLocalScale = DxVector3(1.0f, 1.0f, 1.0f);
	m_vWorldScale = DxVector3(1.0f, 1.0f, 1.0f);
	m_vPivot = DxVector3(0.5f, 0.5f, 0.5f);

	return true;
}

void CTransform::Input(float _fTime)
{
}

void CTransform::Update(float _fTime)
{
	if (false == m_bUpdate)
	{
		return;
	}

	if (NULL != m_pParentTransform)
	{
		*m_matParent = m_pParentTransform->GetWorldMatrix();		
	}	

	*m_matLocal = (*m_matLocalScale) * (*m_matLocalRot) * (*m_matLocalPos);
	*m_matWorld = (*m_matWorldScale) * (*m_matWorldRot) * (*m_matWorldPos) * (*m_matParent);

	m_bUpdate = false;
}

void CTransform::LateUpdate(float _fTime)
{
	if (false == m_bUpdate)
	{
		return;
	}

	if (NULL != m_pParentTransform)
	{
		*m_matParent = m_pParentTransform->GetWorldMatrix();
	}

	*m_matLocal = (*m_matLocalScale) * (*m_matLocalRot) * (*m_matLocalPos);
	*m_matWorld = (*m_matWorldScale) * (*m_matWorldRot) * (*m_matWorldPos) * (*m_matParent);

	m_bUpdate = false;
}

void CTransform::Collision(float _fTime)
{
}

void CTransform::Render(float _fTime)
{
}

CTransform * CTransform::Clone()
{
	return new CTransform(*this);
}

CTransform::CTransform()
{
	SetTypeName("CTransform");
	SetTypeID<CTransform>();
	SetTag("Transform");
	m_eComponentType = CT_TRANSFROM;
	m_bUpdate = true;
	m_pParentTransform = NULL;
}

CTransform::CTransform(const CTransform & _Transform) : CComponent(_Transform)
{
	*this = _Transform;
	m_bUpdate = true;
	m_pParentTransform = NULL;

	m_matLocalScale = new MATRIX;
	m_matLocalRotX = new MATRIX;
	m_matLocalRotY = new MATRIX;
	m_matLocalRotZ = new MATRIX;
	m_matLocalRot = new MATRIX;
	m_matLocalPos = new MATRIX;
	m_matLocal = new MATRIX;

	m_matWorldScale = new MATRIX;
	m_matWorldRotX = new MATRIX;
	m_matWorldRotY = new MATRIX;
	m_matWorldRotZ = new MATRIX;
	m_matWorldRot = new MATRIX;
	m_matWorldPos = new MATRIX;
	m_matParent = new MATRIX;
	m_matWorld = new MATRIX;

	*m_matLocalScale = *_Transform.m_matLocalScale;
	*m_matLocalRotX = *_Transform.m_matLocalRotX;
	*m_matLocalRotY = *_Transform.m_matLocalRotY;
	*m_matLocalRotZ = *_Transform.m_matLocalRotZ;
	*m_matLocalRot = *_Transform.m_matLocalRot;
	*m_matLocalPos = *_Transform.m_matLocalPos;
	*m_matLocal = *_Transform.m_matLocal;
	*m_matWorldScale = *_Transform.m_matWorldScale;
	*m_matWorldRotX = *_Transform.m_matWorldRotX;
	*m_matWorldRotY = *_Transform.m_matWorldRotY;
	*m_matWorldRotZ = *_Transform.m_matWorldRotZ;
	*m_matWorldRot = *_Transform.m_matWorldRot;
	*m_matWorldPos = *_Transform.m_matWorldPos;
	//*m_matParent = *_Transform.m_matParent;
	*m_matWorld = *_Transform.m_matWorld;
}

CTransform::~CTransform()
{
	SAFE_DELETE(m_matLocalScale);
	SAFE_DELETE(m_matLocalRotX);
	SAFE_DELETE(m_matLocalRotY);
	SAFE_DELETE(m_matLocalRotZ);
	SAFE_DELETE(m_matLocalRot);
	SAFE_DELETE(m_matLocalPos);
	SAFE_DELETE(m_matLocal);

	SAFE_DELETE(m_matWorldScale);
	SAFE_DELETE(m_matWorldRotX);
	SAFE_DELETE(m_matWorldRotY);
	SAFE_DELETE(m_matWorldRotZ);
	SAFE_DELETE(m_matWorldRot);
	SAFE_DELETE(m_matWorldPos);
	SAFE_DELETE(m_matParent);
	SAFE_DELETE(m_matWorld);
}