#pragma once
#include "Minion.h"

WOOJUN_USING

class CWarrok : public CMinion
{
public:
	bool Init() override;	
	void Update(float _fTime) override;	
	CWarrok* Clone() override;
public:
	void OnCollisionEnter(CCollider* _pSrc, CCollider* _pDest, float _fTime) override;
	void OnCollisionStay(CCollider* _pSrc, CCollider* _pDest, float _fTime) override;
private:
	void WarrokIdle(float _fTime);
	void WarrokWalk(float _fTime);
	void WarrokTrace(float _fTime);
	void WarrokSpotTrace(float _fTime);
	void WarrokAttack();
	void WarrokDeath();
public:
	CWarrok();
	CWarrok(const CWarrok& _Warrok);
	~CWarrok();
};

