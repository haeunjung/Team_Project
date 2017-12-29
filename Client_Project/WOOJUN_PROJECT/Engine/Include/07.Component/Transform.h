#pragma once
#include "Component.h"

WOOJUN_BEGIN

class DLL CTransform : public CComponent
{
private:
	friend class CGameObject;
private:
	// 얘 벡터값 초기화 어디서하냐 대체??

	DxVector3		m_vLocalScale;
	DxVector3		m_vLocalRot;
	DxVector3		m_vLocalPos;
	DxVector3		m_vLocalAxis[AXIS_MAX];

	DxVector3		m_vWorldScale;
	DxVector3		m_vWorldRot;
	DxVector3		m_vWorldPos;
	DxVector3		m_vWorldAxis[AXIS_MAX];

	bool			m_bUpdate;
	CTransform*		m_pParentTransform;
public:
	DxVector3 GetLocalScale() const;
	DxVector3 GetLocalRot() const;
	DxVector3 GetLocalPos() const;

	DxVector3 GetWorldScale() const;
	DxVector3 GetWorldRot() const;
	DxVector3 GetWorldPos() const;
	DxVector3 GetWorldHierarchyRot() const;		// 부모계층구조의 Rot을 모두 가져온다.
	DxVector3 GetWorldAxis(AXIS _Axis) const;

	CTransform* GetParentTransform() const;
public:
	void SetParentTransform(CTransform* _pParentTransform);
	void ActiveUpdate();
	void ComputeWorldAxis();
public:
	// ============= Local Scale ==============
	void SetLocalScale(const Vector3& v);
	void SetLocalScale(const DxVector3& v);
	void SetLocalScale(const XMVECTOR& v);
	void SetLocalScale(float x, float y, float z);
	void SetLocalScale(float f[3]);
	void SetLocalScaleX(float x);
	void SetLocalScaleY(float y);
	void SetLocalScaleZ(float z);

	// ============= Local Rot ==============
	void SetLocalRot(const Vector3& v);
	void SetLocalRot(const DxVector3& v);
	void SetLocalRot(const XMVECTOR& v);
	void SetLocalRot(float x, float y, float z);
	void SetLocalRot(float f[3]);
	void SetLocalRotX(float x);
	void SetLocalRotY(float y);
	void SetLocalRotZ(float z);

	// ============= Local Pos ==============
	void SetLocalPos(const Vector3& v);
	void SetLocalPos(const DxVector3& v);
	void SetLocalPos(const XMVECTOR& v);
	void SetLocalPos(float x, float y, float z);
	void SetLocalPos(float f[3]);
	void SetLocalPosX(float x);
	void SetLocalPosY(float y);
	void SetLocalPosZ(float z);

	// ============= World Scale ==============
	void SetWorldScale(const Vector3& v);
	void SetWorldScale(const DxVector3& v);
	void SetWorldScale(const XMVECTOR& v);
	void SetWorldScale(float x, float y, float z);
	void SetWorldScale(float f[3]);
	void SetWorldScaleX(float x);
	void SetWorldScaleY(float y);
	void SetWorldScaleZ(float z);

	// ============= World Rot ==============
	void SetWorldRot(const Vector3& v);
	void SetWorldRot(const DxVector3& v);
	void SetWorldRot(const XMVECTOR& v);
	void SetWorldRot(float x, float y, float z);
	void SetWorldRot(float f[3]);
	void SetWorldRotX(float x);
	void SetWorldRotY(float y);
	void SetWorldRotZ(float z);

	// ============= World Pos ==============
	void SetWorldPos(const Vector3& v);
	void SetWorldPos(const DxVector3& v);
	void SetWorldPos(const XMVECTOR& v);
	void SetWorldPos(float x, float y, float z);
	void SetWorldPos(float f[3]);
	void SetWorldPosX(float x);
	void SetWorldPosY(float y);
	void SetWorldPosZ(float z);
private:
	pMATRIX		m_matLocalScale;
	pMATRIX		m_matLocalRotX;
	pMATRIX		m_matLocalRotY;
	pMATRIX		m_matLocalRotZ;
	pMATRIX		m_matLocalRot;
	pMATRIX		m_matLocalPos;
	pMATRIX		m_matLocal;

	pMATRIX		m_matWorldScale;
	pMATRIX		m_matWorldRotX;
	pMATRIX		m_matWorldRotY;
	pMATRIX		m_matWorldRotZ;
	pMATRIX		m_matWorldRot;
	pMATRIX		m_matWorldPos;
	pMATRIX		m_matParent;
	pMATRIX		m_matWorld;
public:
	MATRIX GetLocalMatrix()	const;
	MATRIX GetWorldRotMatrix() const;
	MATRIX GetWorldPosMatrix() const;
	MATRIX GetWorldMatrix()	const;
	MATRIX GetParentMatrix() const;
	MATRIX GetParentRotMatrix() const;
public:
	// Move
	void Forward(float _fSpeed, float _fTime);
	void Forward(float _fSpeed);
	void Up(float _fSpeed, float _fTime);
	void Up(float _fSpeed);
	void Right(float _fSpeed, float _fTime);
	void Right(float _fSpeed);
	void Move(const DxVector3& _vDir, float _fSpeed);
	void Move(const DxVector3& _vDir, float _fSpeed, float _fTime);
	void Move(const DxVector3& _vMove);

	// Rotate
	void Rotate(const DxVector3& vRot, bool bLocal = false);
	void Rotate(float x, float y, float z, bool bLocal = false);
	void RotateX(float x, bool bLocal = false);
	void RotateY(float y, bool bLocal = false);
	void RotateZ(float z, bool bLocal = false);
	void Rotate(const DxVector3& vRot, float fTime, bool bLocal = false);
	void Rotate(float x, float y, float z, float fTime, bool bLocal = false);
	void RotateX(float x, float fTime, bool bLocal = false);
	void RotateY(float y, float fTime, bool bLocal = false);
	void RotateZ(float z, float fTime, bool bLocal = false);

	// LookAt
	// 대상을 바라보는 방향을 구한다.
	void LookAt(CTransform* _pTransform, AXIS _Axis = AXIS_Z);
	void LookAt(CGameObject* _pGameObject, AXIS _Axis = AXIS_Z);

	void CopyTransform(CTransform* _pTransform);
	void ComputeWorldMatrix();
public:
	virtual bool Init() override;
	virtual void Input(float _fTime) override;
	virtual void Update(float _fTime) override;
	virtual void LateUpdate(float _fTime) override;
	virtual void Collision(float _fTime) override;
	virtual void Render(float _fTime) override;
	virtual CTransform* Clone() override;
private:
	CTransform();
	CTransform(const CTransform& _Transform);
	virtual ~CTransform();
//////////////////////////////////////////////////
public:
	inline void ParentMatrixIdentity() {
		*m_matParent = XMMatrixIdentity();
	}
};

WOOJUN_END