#pragma once
#include "07.Component/Script.h"
#include "07.Component/Collider.h"

WOOJUN_USING

class CPlayerBullet : public CScript
{
private:
	float	m_fSpeed;
	float	m_fDist;	
public:
	bool Init() override;	
	void Update(float _fTime) override;	
	CPlayerBullet* Clone() override;
public:
	void OnCollisionEnter(CCollider* _pSrc, CCollider* _pDest, float _fTime) override;
public:
	CPlayerBullet();
	CPlayerBullet(const CPlayerBullet& _PlayerBullet);
	~CPlayerBullet();
};