#pragma once
#include "06.GameObject/GameObject.h"	
#include "07.Component/Script.h"
#include "07.Component/Animation3D.h"

WOOJUN_USING

class CMinion : public CScript
{
private:
	CAnimation3D*	m_pAniController;
	CGameObject*	m_pPlayer;
	float		m_fTime;
public:
	void SetPlayer(CGameObject* _pPlayer);
public:
	bool Init() override;	
	void Update(float _fTime) override;	
	CMinion* Clone() override;
public:
	void OnCollisionEnter(CCollider* _pSrc, CCollider* _pDest, float _fTime) override;
private:
	bool BackAttackCheck(const DxVector3& _SrcForward, const DxVector3& _DestForward);
public:
	CMinion();
	CMinion(const CMinion& _Minion);
	~CMinion();
};

