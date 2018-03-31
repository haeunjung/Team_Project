#pragma once
#include "06.GameObject/GameObject.h"	
#include "07.Component/Script.h"
#include "07.Component/Animation3D.h"
#include "07.Component/ColliderSphere.h"

WOOJUN_USING

enum MONSTER_STATE
{
	MS_DEFAULT,
	MS_WALK,
	MS_TRACE,
	MS_ATTACK,
	MS_DEATH
};

class CMinion : public CScript
{
private:
	MONSTER_STATE	m_eMonsterState;
	CAnimation3D*	m_pAniController;
	CGameObject*	m_pPlayer;
	CTransform*		m_pPlayerTransform;

	float	m_fSpeed;
	float	m_fTime;
	float	m_fDist;

	CColliderSphere*	m_pViewCol;
	CColliderSphere*	m_pAttCol;

	// Test¿ë
private:
	bool	m_bTest;
public:
	inline void SetIsTest(bool _bTest) {
		m_bTest = _bTest;
	}
public:
	void SetPlayer(CGameObject* _pPlayer);
	void SetMonsterWorldPos(const DxVector3 _Pos);
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
	void MonsterAttack();
	void MonsterDeath();
public:
	CMinion();
	CMinion(const CMinion& _Minion);
	~CMinion();
};

