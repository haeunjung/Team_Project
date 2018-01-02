#pragma once
#include "UI.h"

WOOJUN_BEGIN

class DLL CUIBack :
	public CUI
{
protected:
	friend class CGameObject;
public:
	virtual bool Init() override;
	virtual void Input(float _fTime) override;
	virtual void Update(float _fTime) override;
	virtual void LateUpdate(float _fTime) override;
	virtual void Collision(float _fTime) override;
	virtual void Render(float _fTime) override;
	virtual CUIBack* Clone() override;
public:
	virtual void OnCollisionEnter(CCollider* _pSrc, CCollider* _pDest, float _fTime);
	virtual void OnCollisionStay(CCollider* _pSrc, CCollider* _pDest, float _fTime);
	virtual void OnCollisionLeave(CCollider* _pSrc, CCollider* _pDest, float _fTime);
protected:
	CUIBack();
	CUIBack(const CUIBack& _UIBack);
	virtual ~CUIBack();
};

WOOJUN_END