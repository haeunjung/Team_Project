#pragma once
#include "07.Component/Script.h"
#include "06.GameObject/GameObject.h"

WOOJUN_USING

class CMonsterBullet : public CScript
{
private:
	CGameObject*	m_pPlayer;

	float	m_fSpeed;
	float	m_fDist;
	float	m_fTime;	
public:
	void SetPlayer(CGameObject* _pPlayer);
public:
	bool Init() override;	
	void Update(float _fTime) override;	
	CMonsterBullet* Clone() override;
public:
	void OnCollisionEnter(CCollider* _pSrc, CCollider* _pDest, float _fTime) override;
public:
	CMonsterBullet();
	CMonsterBullet(const CMonsterBullet& _Bullet);
	~CMonsterBullet();
};

