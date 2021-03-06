#pragma once
#include "Collider.h"

WOOJUN_BEGIN

class DLL CColliderSphere :
	public CCollider
{
private:
	friend class CGameObject;
private:
	SPHEREINFO	m_tSphereInfo;
	bool		m_bAniCollider;
	int			m_iBoneIndex;
public:
	SPHEREINFO GetSphereInfo() const;
	float GetSphereTop() const;
	float GetSphereBottom() const;
	float GetSphereLeft() const;
	float GetSphereRight() const;
	void SetBoneIndex(int _BoneIndex);
public:
	void SetSphereInfo(const SPHEREINFO& _tSphereInfo);
	void SetSphereInfo(const DxVector3& _vCenter, float _fRadius);
	void SetSphereInfo(float _fX, float _fY, float _fZ, float _fRadius);
	void SetSpherePos(const DxVector3 _vPos);
public:
	virtual bool Init();
	virtual void Input(float _fTime);
	virtual void Update(float _fTime);
	virtual void LateUpdate(float _fTime);
	virtual void Collision(float _fTime);
	virtual void Render(float _fTime);
	virtual CColliderSphere* Clone();
	virtual bool Collision(CCollider* _pCollider);
private:
	CColliderSphere();
	CColliderSphere(const CColliderSphere& _ColliderSphere);
	~CColliderSphere();
};

WOOJUN_END