#pragma once
#include "07.Component/Script.h"
#include "07.Component/Animation3D.h"
#include "07.Component/ColliderSphere.h"
#include "07.Component/UIBar.h"
#include "07.Component/CameraArm.h"
#include "07.Component/ColliderAABB.h"

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
	PS_MOVINGJUMP,
	PS_CLIMB
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

	bool	m_bFrontCol;
	bool	m_bBackCol;
	bool	m_bLeftCol;
	bool	m_bRightCol;

	CAnimation3D*		m_pAniController;
	CColliderSphere*	m_pAttCol;
	CColliderSphere*	m_pHitCol;
	CUIBar*				m_pHpBar;
	CCameraArm*			m_pCameraArm;
	DxVector3			m_vPrePos;
	DxVector3			m_vColAxis[AXIS_MAX];

	AABBINFO			m_pOtherCol;
	bool	m_bClimb;

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
	void OnCollisionLeave(CCollider* _pSrc, CCollider* _pDest, float _fTime) override;
private:
	void PlayerMove(float _fTime);
	void MoveForward(float _fTime);
	void MoveBack(float _fTime);
	void MoveUp(float _fTime);
	void MoveDown(float _fTime);
	void CreateHpBar();
	void Attack();
	void Jump();
	void MovingJump(float _fTime);
	void Climb();
public:
	CPlayer();
	~CPlayer();
};