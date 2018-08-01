#pragma once
#include "07.Component/Script.h"
#include "07.Component/SoundPlayer.h"

WOOJUN_USING

class CGear : public CScript
{
private:
	CSoundPlayer * m_pGetGearSound;
public:
	void SetGearPos(const DxVector3& _vPos);
public:
	bool Init() override;
	void Update(float _fTime) override;
	CGear* Clone() override;
public:
	void OnCollisionEnter(CCollider * _pSrc, CCollider * _pDest, float _fTime);
public:
	CGear();
	CGear(const CGear& _Gear);
	~CGear();
};