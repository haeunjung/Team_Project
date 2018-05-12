#pragma once
#include "07.Component/Script.h"

WOOJUN_USING

class CPlayerHitEffect : public CScript
{
public:
	bool Init() override;
	void Update(float _fTime) override;
	CPlayerHitEffect* Clone() override;
public:
	void SetPos(const DxVector3& _vPos);
public:
	CPlayerHitEffect();
	CPlayerHitEffect(const CPlayerHitEffect& _PlayerHitEffect);
	~CPlayerHitEffect();
};

