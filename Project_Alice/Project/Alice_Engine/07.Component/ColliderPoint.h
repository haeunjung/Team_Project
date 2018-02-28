#pragma once
#include "Collider.h"

WOOJUN_BEGIN

class DLL CColliderPoint :
	public CCollider
{
private:
	friend class CGameObject;
private:
	DxVector3	m_vPos;
public:
	DxVector3 GetPoint() const;
public:
	virtual bool Init();
	virtual void Input(float _fTime);
	virtual void Update(float _fTime);
	virtual void LateUpdate(float _fTime);
	virtual void Collision(float _fTime);
	virtual void Render(float _fTime);
	virtual CColliderPoint* Clone();
	virtual bool Collision(CCollider* _pCollider);
private:
	CColliderPoint();
	CColliderPoint(const CColliderPoint& _ColliderPoint);
	~CColliderPoint();
};

WOOJUN_END