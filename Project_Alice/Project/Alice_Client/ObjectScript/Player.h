#pragma once
#include "07.Component/Script.h"
#include "07.Component/Animation3D.h"
#include "07.Component/ColliderSphere.h"
#include "07.Component/UIBar.h"
#include "07.Component/CameraArm.h"
#include "07.Component/ColliderAABB.h"
#include "07.Component/SoundPlayer.h"

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
	PS_CLIMB,
	PS_CLIMBIDLE,
	PS_FALL,
	PS_HEAT,
	PS_CLIMBTOUP,
	PS_DEATH
};

class CPlayer : public CScript
{
private:
	PLAYER_STATE	m_ePlayerState;
	bool	m_bAttack;
	bool	m_bJump;
	bool	m_bHeat;
	bool	m_bClimbToTop;
	bool	m_bDeath;

	float	m_fHeatTime;
	float	m_fSpeed;
	int		m_iHp;
	
	bool	m_bFrontCol;
	bool	m_bBackCol;
	bool	m_bLeftCol;
	bool	m_bRightCol;
	bool	m_bFall;

	CTransform*			m_pChildTransform;
	CAnimation3D*		m_pAniController;
	CColliderSphere*	m_pAttCol;
	CColliderSphere*	m_pHitCol;
	CColliderSphere*	m_pFootCol;
	CUIBar*				m_pHpBar;
	CCameraArm*			m_pCameraArm;
	DxVector3			m_vPrePos;
	DxVector3			m_vColAxis[AXIS_MAX];

	CCollider*			m_pOtherCol;
	bool	m_bClimb;

	CSoundPlayer*		m_pHitSound;
	CSoundPlayer*		m_pAttackSound;
	CSoundPlayer*		m_pWarningSound;
	//////////
	bool m_bChange;
	
	// Jump
	float m_fJumpTime;
	float m_fJumpPower;
	float m_fGravity;

	float m_fy;
	float m_fz;

	// Cheat Key
private:
	static bool m_bCheat;
public:
	static void CheatKey();
private:
	// Sound Callback
	void FirstStepSound(float _fTime);
	void SecondStepSound(float _fTime);
public:
	bool PlayerStateIsDeath();
	const PLAYER_STATE& GetPlayerState() const;
	void SetPlayerDeath();
	void PlayAttackSound();
public:
	bool Init() override;
	void Input(float _fTime) override;
	void Update(float _fTime) override;	
public:
	void OnCollisionEnter(CCollider* _pSrc, CCollider* _pDest, float _fTime) override;
	void OnCollisionStay(CCollider* _pSrc, CCollider* _pDest, float _fTime) override;
	void OnCollisionLeave(CCollider* _pSrc, CCollider* _pDest, float _fTime) override;
private:
	void PlayerMove(float _fTime);
	void MoveForward(float _fTime);
	void MoveBack(float _fTime);
	void MoveUp(float _fTime);
	void MoveDown(float _fTime);
	void PlayerAttack();
	void PlayerJump(float _fTime);
	void PlayerMovingJump(float _fTime);
	void PlayerClimbIdle();
	void PlayerFall(float _fTime);
	void PlayerHit(float _fTime);
	void PlayerClimbToTop();
	void PlayerDeath();
	void CreatePlayerLight();
	void PlayerHitEffect();
public:
	CPlayer();
	~CPlayer();
};