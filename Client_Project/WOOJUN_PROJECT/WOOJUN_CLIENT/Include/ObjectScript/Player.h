#pragma once
#include "07.Component/Script.h"

WOOJUN_USING

class CPlayer : public CScript
{
private:
	CGameObject*	m_pChild;	
	float	m_fSpeed;
	int		m_iHp;
	bool	m_bUp;
public:
	bool Init() override;
	void Input(float _fTime) override;
	void Update(float _fTime) override;	
public:
	void OnCollisionEnter(CCollider* _pSrc, CCollider* _pDest, float _fTime) override;
public:
	CPlayer();
	~CPlayer();
};