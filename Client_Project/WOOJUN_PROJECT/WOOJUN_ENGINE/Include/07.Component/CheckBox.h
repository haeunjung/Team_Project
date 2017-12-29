#pragma once
#include "UIButton.h"

WOOJUN_BEGIN

class DLL CCheckBox :
	public CUIButton
{
private:
	friend class CGameObject;
	friend class CRadioButton;
private:
	bool	m_bCheck;
public:
	inline void CheckBoxTrue() {
		m_bCheck = true;
	}
public:
	bool Init() override;
	void Input(float _fTime) override;
	void Update(float _fTime) override;
	void LateUpdate(float _fTime) override;
	void Collision(float _fTime) override;
	void Render(float _fTime) override;
	CCheckBox* Clone() override;
public:
	void OnCollisionEnter(CCollider* _pSrc, CCollider* _pDest, float _fTime) override;
	void OnCollisionStay(CCollider* _pSrc, CCollider* _pDest, float _fTime) override;
	void OnCollisionLeave(CCollider* _pSrc, CCollider* _pDest, float _fTime) override;
private:
	CCheckBox();
	CCheckBox(const CCheckBox& _CheckBox);
	~CCheckBox();
};

WOOJUN_END