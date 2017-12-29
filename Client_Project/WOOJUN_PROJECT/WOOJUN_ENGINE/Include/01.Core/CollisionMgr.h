#pragma once
#include "../Engine_Core.h"

WOOJUN_BEGIN

class CGameObject;
class DLL CCollisionMgr
{
private:
	vector<CGameObject*>	m_vecCollision;	
public:
	void AddObject(CGameObject* _pGameObject);
	void ClearObject() {
		m_vecCollision.clear();
	}
public:
	bool Init();
	void Collision(float _fTime);
private:
	void CollisionObject(CGameObject* _pSrc, CGameObject* _pDest, float _fTime);
	static bool ZSort(CGameObject* _pSrc, CGameObject* _pDest);

	DECLARE_SINGLE(CCollisionMgr)
};

WOOJUN_END;