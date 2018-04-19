#pragma once
#include "07.Component/Script.h"

WOOJUN_USING

class CBattery : public CScript
{
private:
	float m_fSpeed;
	bool m_IsCreate;
public:
	void SetBatteryPos(const DxVector3& _vPos);
public:
	bool Init() override;
	void Update(float _fTime) override;
	CBattery* Clone() override;
public:
	void OnCollisionEnter(CCollider* _pSrc, CCollider* _pDest, float _fTime) override;
public:
	CBattery();
	CBattery(const CBattery& _PlayerBullet);
	~CBattery();
};