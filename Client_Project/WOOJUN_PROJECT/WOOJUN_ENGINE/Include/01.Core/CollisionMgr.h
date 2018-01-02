#pragma once
#include "../Engine_Core.h"

WOOJUN_BEGIN

class CGameObject;
class DLL CCollisionMgr
{
private:
	vector<CGameObject*>	m_vecCollision;	
	vector<CGameObject*>	m_vecUICollision;
public:
	void AddObject(CGameObject* _pGameObject);
public:
	bool Init();
	void Collision(float _fTime);
private:
	bool CollisionMouseUI(float _fTime);
	void CollisionMouseObject(float _fTime);
	void CollisionObject(CGameObject* _pSrc, CGameObject* _pDest, float _fTime);
	static bool ZSort(CGameObject* _pSrc, CGameObject* _pDest);

	DECLARE_SINGLE(CCollisionMgr)
};

WOOJUN_END;