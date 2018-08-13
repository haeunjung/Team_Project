#pragma once
#include "Minion.h"

WOOJUN_USING

class CPlantRange : public CMinion
{
public:
	bool Init() override;	
	void Update(float _fTime) override;	
	CPlantRange* Clone() override;
public:
	void OnCollisionEnter(CCollider* _pSrc, CCollider* _pDest, float _fTime) override;
	void OnCollisionStay(CCollider* _pSrc, CCollider* _pDest, float _fTime) override;
private:
	void PlantRangeIdle(float _fTime);
	void PlantRangeSpotTrace(float _fTime);
	void PlantRangeAttack();
	void PlantRangeDeath();
	void CreateEnergyBall();
public:
	CPlantRange();
	CPlantRange(const CPlantRange& _PlantRange);
	~CPlantRange();
};

