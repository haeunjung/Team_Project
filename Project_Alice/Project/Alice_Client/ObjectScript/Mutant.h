#pragma once
#include "Minion.h"

WOOJUN_USING

class CMutant : public CMinion
{
public:
	bool Init() override;	
	void Update(float _fTime) override;	
	CMutant* Clone() override;
public:
	void OnCollisionEnter(CCollider* _pSrc, CCollider* _pDest, float _fTime) override;
	void OnCollisionStay(CCollider* _pSrc, CCollider* _pDest, float _fTime) override;
private:
	void MutantIdle(float _fTime);
	void MutantWalk(float _fTime);
	void MutantTrace(float _fTime);
	void MutantSpotTrace(float _fTime);
	void MutantAttack();
	void MutantDeath();
public:
	CMutant();
	CMutant(const CMutant& _Plant);
	~CMutant();
};

