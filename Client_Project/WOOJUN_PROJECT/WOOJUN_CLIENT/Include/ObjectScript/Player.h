#pragma once
#include "07.Component/Script.h"
#include "07.Component/Animation3D.h"

WOOJUN_USING

class CPlayer : public CScript
{
private:
	CGameObject*	m_pChild;	
	float	m_fSpeed;
	int		m_iHp;
	bool	m_bUp;
	CAnimation3D*	m_pAniController;
public:
	void AniCallback(float _fTime);
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