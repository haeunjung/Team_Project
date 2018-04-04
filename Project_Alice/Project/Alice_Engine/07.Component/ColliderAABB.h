#pragma once
#include "Collider.h"

WOOJUN_BEGIN

class DLL CColliderAABB :
	public CCollider
{
private:
	friend class CGameObject;
private:
	AABBINFO	m_tAABBInfo;
public:
	bool Init() override;
	void Input(float _fTime) override;
	void Update(float _fTime) override;
	void LateUpdate(float _fTime) override;
	void Collision(float _fTime) override;
	void Render(float _fTime) override;
	CColliderAABB* Clone() override;
	bool Collision(CCollider* _pCollider) override;
private:
	CColliderAABB();
	CColliderAABB(const CColliderAABB& _ColliderAABB);
	~CColliderAABB();
};

WOOJUN_END