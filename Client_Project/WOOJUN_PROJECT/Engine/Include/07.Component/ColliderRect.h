#pragma once
#include "Collider.h"

WOOJUN_BEGIN

class DLL CColliderRect :
	public CCollider
{
private:
	friend class CGameObject;
private:
	bool	m_bFirst;
	RECTINFO	m_tRectInfo;
public:
	RECTINFO GetRectInfo() const;
public:
	void SetRectInfo(float _fLeft, float _fTop, float _fRight, float _fBottom, bool _bCenter);
	void SetRectSize(float _fSizeX, float _fSizeY);
public:
	virtual bool Init();
	virtual void Input(float _fTime);
	virtual void Update(float _fTime);
	virtual void LateUpdate(float _fTime);
	virtual void Collision(float _fTime);
	virtual void Render(float _fTime);
	virtual CColliderRect* Clone();
	virtual bool Collision(CCollider* _pCollider);
private:
	CColliderRect();
	CColliderRect(const CColliderRect& _ColliderRect);
	~CColliderRect();
};

WOOJUN_END