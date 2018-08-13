#pragma once
#include "07.Component/Script.h"

WOOJUN_USING

class CMonsterBullet : public CScript
{
private:
	CTransform*	m_pPlayerTransform;

	float	m_fSpeed;
	float	m_fDist;
	float	m_fTime;	
public:
	void SetPlayerTransform(CTransform* _pPlayerTransform);
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

