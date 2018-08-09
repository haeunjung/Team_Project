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

class CPlayer;
class CMinion : public CScript
{
protected:
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

	bool	m_bTest;
public:
	inline void SetIsTest(bool _bTest) {
		m_bTest = _bTest;
	}
public:
	void SetRespawnPos(DxVector3 _vPos);
	void SetPlayer(CPlayer* _pPlayer);
	void SetMonsterWorldPos(const DxVector3 _Pos);
	float DistCheckFromPlayer();
	void TracePlayer();
	MONSTER_STATE GetState();
	bool RespawnUpdate(float _fTime);
	void MinionAttackSound();
public:
	virtual bool Init() override;	
	virtual void Update(float _fTime) override;	
	virtual CMinion* Clone() override;
public:
	virtual void OnCollisionEnter(CCollider* _pSrc, CCollider* _pDest, float _fTime) override;
	virtual void OnCollisionStay(CCollider* _pSrc, CCollider* _pDest, float _fTime) override;
protected:
	bool BackAttackCheck(const DxVector3& _SrcForward, const DxVector3& _DestForward);
	void SetRespawnPos();
public:
	CMinion();
	CMinion(const CMinion& _Minion);
	~CMinion();
};

