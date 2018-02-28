#pragma once
#include "UIButton.h"

WOOJUN_BEGIN

class DLL CRadioButton :
	public CUIButton
{
private:
	friend class CGameObject;
private:
	bool	m_bClick;
public:
	void RadioButtonOff();
	bool GetClick();
public:
	bool Init() override;
	void Input(float _fTime) override;
	void Update(float _fTime) override;
	void LateUpdate(float _fTime) override;
	void Collision(float _fTime) override;
	void Render(float _fTime) override;
	CRadioButton* Clone() override;
public:
	void OnCollisionEnter(CCollider* _pSrc, CCollider* _pDest, float _fTime) override;
	void OnCollisionStay(CCollider* _pSrc, CCollider* _pDest, float _fTime) override;
	void OnCollisionLeave(CCollider* _pSrc, CCollider* _pDest, float _fTime) override;
private:
	CRadioButton();
	CRadioButton(const CRadioButton& _RadioButton);
	~CRadioButton();
};

WOOJUN_END