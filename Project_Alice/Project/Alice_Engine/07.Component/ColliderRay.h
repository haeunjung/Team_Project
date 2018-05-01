#pragma once
#include "Collider.h"

WOOJUN_BEGIN

class DLL CColliderRay :
	public CCollider
{
private:
	friend class CGameObject;
private:
	DxVector3 vColPos;
	RAY m_tRay;
public:
	RAY GetRay() const;
	void SetRay(const RAY& _Ray);
	void SetRay(const DxVector3& _Pos, const DxVector3& _Dir);
public:
	bool Init() override;
	void Input(float _fTime) override;
	void Update(float _fTime) override;
	void LateUpdate(float _fTime) override;
	void Collision(float _fTime) override;
	CCollider* Clone() override;
	bool Collision(CCollider* pCollider) override;
public:
	CColliderRay();
	CColliderRay(const CColliderRay& _ColliderRay);
	~CColliderRay();
};

WOOJUN_END
