#pragma once
#include "07.Component/Script.h"
#include "07.Component/ColliderSphere.h"

WOOJUN_USING

class CGear : public CScript
{
private:
	CColliderSphere * m_pGearCol;
public:
	void SetGearPos(const DxVector3& _vPos);
public:
	bool Init() override;
	void Update(float _fTime) override;
	CGear* Clone() override;
public:
	CGear();
	CGear(const CGear& _Gear);
	~CGear();
};