#pragma once
#include "07.Component/Script.h"

WOOJUN_USING

class CPortal :	public CScript
{
public:
	bool Init() override;
	void Update(float _fTime) override;
public:
	void OnCollisionEnter(CCollider* _pSrc, CCollider* _pDest, float _fTime) override;
private:
	void StageChange();
public:
	CPortal();
	~CPortal();
};

