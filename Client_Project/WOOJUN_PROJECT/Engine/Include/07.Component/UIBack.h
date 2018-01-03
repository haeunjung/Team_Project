#pragma once
#include "UI.h"

WOOJUN_BEGIN

class DLL CUIBack :
	public CUI
{
protected:
	friend class CGameObject;
public:
	bool Init() override;
	void Input(float _fTime) override;
	void Update(float _fTime) override;
	void LateUpdate(float _fTime) override;
	void Collision(float _fTime) override;
	void Render(float _fTime) override;
	CUIBack* Clone() override;
public:
	void OnCollisionEnter(CCollider* _pSrc, CCollider* _pDest, float _fTime);
	void OnCollisionStay(CCollider* _pSrc, CCollider* _pDest, float _fTime);
	void OnCollisionLeave(CCollider* _pSrc, CCollider* _pDest, float _fTime);
protected:
	CUIBack();
	CUIBack(const CUIBack& _UIBack);
	~CUIBack();
};

WOOJUN_END