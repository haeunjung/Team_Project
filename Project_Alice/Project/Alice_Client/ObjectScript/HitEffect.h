#pragma once
#include "07.Component/Script.h"

WOOJUN_USING

class CHitEffect : public CScript
{
public:
	bool Init() override;
	void Update(float _fTime) override;
	CHitEffect* Clone() override;
public:
	void SetPos(const DxVector3& _vPos);
public:
	CHitEffect();
	CHitEffect(const CHitEffect& _HitEffect);
	~CHitEffect();
};

