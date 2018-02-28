#pragma once
#include "UI.h"

WOOJUN_BEGIN

class DLL CUIButton :
	public CUI
{
protected:
	friend class CGameObject;
protected:
	BUTTON_STATE	m_eButtonState;
	DxVector4		m_vNormalCol;
	DxVector4		m_vMouseOnCol;
	DxVector4		m_vClickCol;

	function<void(CGameObject*, float)>		m_pCallback;
	bool			m_bCallback;
public:
	template <typename T>
	void SetButtonFunc(T* _pColRect, void(T::*_pFunc)(CGameObject*, float))
	{
		m_bCallback = true;
		m_pCallback = bind(_pFunc, _pColRect, placeholders::_1, placeholders::_2);
	}
public:
	void SetNormalColor(float _r, float _g, float _b, float _a);
	void SetMouseOnColor(float _r, float _g, float _b, float _a);
	void SetClickColor(float _r, float _g, float _b, float _a);
public:
	bool Init() override;
	void Input(float _fTime) override;
	void Update(float _fTime) override;
	void LateUpdate(float _fTime) override;
	void Collision(float _fTime) override;
	void Render(float _fTime) override;
	CUIButton* Clone() override;
public:
	void OnCollisionEnter(CCollider* _pSrc, CCollider* _pDest, float _fTime) override;
	void OnCollisionStay(CCollider* _pSrc, CCollider* _pDest, float _fTime) override;
	void OnCollisionLeave(CCollider* _pSrc, CCollider* _pDest, float _fTime) override;
protected:
	CUIButton();
	CUIButton(const CUIButton& _Button);
	~CUIButton();
};

WOOJUN_END