#pragma once
#include "06.GameObject/GameObject.h"
#include "07.Component/Script.h"
#include "07.Component/Collider.h"

WOOJUN_USING

class CMinion : public CScript
{
private:
	CGameObject*	m_pPlayer;
	float		m_fTime;
public:
	void SetPlayer(CGameObject* _pPlayer);
private:
	void BulletFire();
public:
	bool Init() override;	
	void Update(float _fTime) override;	
	CMinion* Clone() override;
public:
	void OnCollisionEnter(CCollider* _pSrc, CCollider* _pDest, float _fTime) override;
public:
	CMinion();
	CMinion(const CMinion& _Minion);
	~CMinion();
};

