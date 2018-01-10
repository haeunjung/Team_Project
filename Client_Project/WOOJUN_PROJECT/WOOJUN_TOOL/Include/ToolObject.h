#pragma once
#include "07.Component/Script.h"

WOOJUN_USING

class CToolObject :
	public CScript
{
private:
	bool m_bLButton;
public:
	bool Init() override;	
	void Input(float _fTIme) override;
	void Update(float _fTime) override;
	CToolObject* Clone() override;
public:
	void OnCollisionStay(CCollider* _pSrc, CCollider* _pDest, float _fTime) override;
public:
	CToolObject();
	CToolObject(const CToolObject& _Object);
	~CToolObject();
};

