#pragma once
#include "07.Component/Script.h"
#include "07.Component/Animation3D.h"
#include "07.Component/ColliderSphere.h"
#include "07.Component/UIBar.h"

WOOJUN_USING

// 타이머 기능
// 특정상태마다 카메라
// 엎드리기
// 기어오르기

enum PLAYER_STATE
{
	PS_DEFAULT,
	PS_ATTACK,
	PS_JUMP,
	PS_MOVINGJUMP
};

class CPlayer : public CScript
{
private:
	PLAYER_STATE	m_ePlayerState;
	bool	m_bAttack;
	bool	m_bJump;

	float	m_fSpeed;
	int		m_iHp;
	int		m_iHpMax;
	CAnimation3D*		m_pAniController;
	CColliderSphere*	m_pAttCol;
	CColliderSphere*	m_pHitCol;
	CUIBar*				m_pHpBar;

	//////////
	bool m_bChange;
public:
	void AniCallback(float _fTime);
public:
	bool Init() override;
	void Input(float _fTime) override;
	void Update(float _fTime) override;	
public:
	void OnCollisionStay(CCollider* _pSrc, CCollider* _pDest, float _fTime) override;
private:
	void CreateHpBar();
	void Attack();
	void Jump();
	void MovingJump(float _fTime);
public:
	CPlayer();
	~CPlayer();
};