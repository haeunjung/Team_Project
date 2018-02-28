#pragma once
#include "07.Component\Script.h"

WOOJUN_USING

class CMouse :
	public CScript
{
public:
	bool Init() override;
	void Input(float _fTime) override;
	void Update(float _fTime) override;
	void LateUpdate(float _fTime) override;
	void Collision(float _fTime) override;
	CMouse* Clone() override;
public:
	virtual void OnCollisionEnter(CCollider* _pSrc, CCollider* _pDest, float _fTime) override;
	virtual void OnCollisionStay(CCollider* _pSrc, CCollider* _pDest, float _fTime) override;
	virtual void OnCollisionLeave(CCollider* _pSrc, CCollider* _pDest, float _fTime) override;
public:
	CMouse();
	CMouse(const CMouse& _Mouse);
	~CMouse();
};

