#pragma once
#include "Component.h"

WOOJUN_BEGIN

class DLL CUI :
	public CComponent
{
protected:
	friend class CGameObject;
protected:
	UI_TYPE	m_eUIType;
public:
	UI_TYPE GetUIType() const;
public:
	virtual bool Init() = 0;
	virtual void Input(float _fTime) = 0;
	virtual void Update(float _fTime) = 0;
	virtual void LateUpdate(float _fTime) = 0;
	virtual void Collision(float _fTime) = 0;
	virtual void Render(float _fTime) = 0;
	virtual CUI* Clone() = 0;
public:
	virtual void OnCollisionEnter(CCollider* _pSrc, CCollider* _pDest, float _fTime);
	virtual void OnCollisionStay(CCollider* _pSrc, CCollider* _pDest, float _fTime);
	virtual void OnCollisionLeave(CCollider* _pSrc, CCollider* _pDest, float _fTime);
protected:
	CUI();
	CUI(const CUI& _UI);
	virtual ~CUI() = 0;
};

WOOJUN_END