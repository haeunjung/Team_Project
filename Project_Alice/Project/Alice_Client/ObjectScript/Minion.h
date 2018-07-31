#pragma once
#include "06.GameObject/GameObject.h"	
#include "07.Component/Script.h"
#include "07.Component/Animation3D.h"
#include "07.Component/ColliderSphere.h"
#include "07.Component/SoundPlayer.h"

WOOJUN_USING

enum MONSTER_STATE
{
	MS_DEFAULT,
	MS_WALK,
	MS_TRACE,
	MS_ATTACK,
	MS_SPOTTRACE,
	MS_DEATH
};

enum RESPAWN_POS
{
	POS_ONE,
	POS_TWO,
	POS_THREE,
	POS_FOUR,
	POS_FIVE,
	POS_TEST,
};

class CPlayer;
class CMinion : public CScript
{
private:
	//RESPAWN_POS		m_eRespawnPos;
	DxVector3		m_vRespawnPos;
	MONSTER_STATE	m_eMonsterState;
	CAnimation3D*	m_pAniController;
	CTransform*		m_pPlayerTransform;
	CPlayer*		m_pPlayerScript;

	float	m_fSpeed;
	float	m_fTime;
	float	m_fDist;
	float	m_fRespawnTime;

	CColliderSphere*	m_pViewCol;
	CColliderSphere*	m_pAttCol;

	CSoundPlayer*		m_pHitSound;
	CSoundPlayer*		m_pAttackSound;

	CGameObject* m_pParticleObj;
	// Test¿ë
private:
	bool	m_bTest;
public:
	inline void SetIsTest(bool _bTest) {
		m_bTest = _bTest;
	}
public:
	void SetRespawnPos(RESPAWN_POS _eRespawnPos);
	void SetRespawnPos(DxVector3 _vPos);
	void SetPlayer(CPlayer* _pPlayer);
	void SetMonsterWorldPos(const DxVector3 _Pos);
	float DistCheckFromPlayer();
	void TracePlayer();
	MONSTER_STATE GetState();
	bool RespawnUpdate(float _fTime);
	void MinionAttackSound();
public:
	bool Init() override;	
	void Update(float _fTime) override;	
	CMinion* Clone() override;
public:
	void OnCollisionEnter(CCollider* _pSrc, CCollider* _pDest, float _fTime) override;
	void OnCollisionStay(CCollider* _pSrc, CCollider* _pDest, float _fTime) override;
private:
	bool BackAttackCheck(const DxVector3& _SrcForward, const DxVector3& _DestForward);
	void MonsterIdle(float _fTime);
	void MonsterWalk(float _fTime);
	void MonsterTrace(float _fTime);
	void MonsterSpotTrace(float _fTime);
	void MonsterAttack();
	void MonsterDeath();
	void SetRespawnPos();
public:
	CMinion();
	CMinion(const CMinion& _Minion);
	~CMinion();
};

