#pragma once
#include "Minion.h"

WOOJUN_USING

class CMaw : public CMinion
{
public:
	bool Init() override;	
	void Update(float _fTime) override;	
	CMaw* Clone() override;
public:
	void OnCollisionEnter(CCollider* _pSrc, CCollider* _pDest, float _fTime) override;
	void OnCollisionStay(CCollider* _pSrc, CCollider* _pDest, float _fTime) override;
private:
	void MawIdle(float _fTime);
	void MawWalk(float _fTime);
	void MawTrace(float _fTime);
	void MawSpotTrace(float _fTime);
	void MawAttack();
	void MawDeath();
public:
	CMaw();
	CMaw(const CMaw& _Maw);
	~CMaw();
};

// Maw 내용 변경