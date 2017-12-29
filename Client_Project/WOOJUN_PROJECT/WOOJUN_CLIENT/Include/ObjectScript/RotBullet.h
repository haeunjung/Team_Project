#pragma once
#include "07.Component/Script.h"
#include "06.GameObject/GameObject.h"

WOOJUN_USING

class CRotBullet : public CScript
{
public:
	static bool m_IsFire;
private:
	float	m_fSpeed;
	float	m_fDist;
	CGameObject*	m_pChild;
public:
	bool Init() override;	
	void Update(float _fTime) override;	
	CRotBullet* Clone() override;
public:
	CRotBullet();
	CRotBullet(const CRotBullet& _RotBullet);
	~CRotBullet();
};

