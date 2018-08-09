#pragma once
#include "Minion.h"

WOOJUN_USING

class CPlant : public CMinion
{
public:
	bool Init() override;	
	void Update(float _fTime) override;	
	CPlant* Clone() override;
public:
	void OnCollisionEnter(CCollider* _pSrc, CCollider* _pDest, float _fTime) override;
	void OnCollisionStay(CCollider* _pSrc, CCollider* _pDest, float _fTime) override;
private:
	void PlantIdle(float _fTime);
	void PlantWalk(float _fTime);
	void PlantTrace(float _fTime);
	void PlantSpotTrace(float _fTime);
	void PlantAttack();
	void PlantDeath();
public:
	CPlant();
	CPlant(const CPlant& _Plant);
	~CPlant();
};

