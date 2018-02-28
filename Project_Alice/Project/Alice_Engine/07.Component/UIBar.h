#pragma once
#include "UI.h"

WOOJUN_BEGIN

class DLL CUIBar :
	public CUI
{
protected:
	friend class CGameObject;
private:
	float		m_fMin;
	float		m_fMax;
	float		m_fCurValue;
	float		m_fValue;
	float		m_fMoveValue;
	float		m_fSize;
	BAR_DIR		m_eBarDir;
public:
	void SetBarDir(BAR_DIR _eDir);
	void SetMinMax(float _fMin, float _fMax);
	void SetCurValue(float _fValue);
	void AddValue(float _fValue);
private:
	DxVector3 ComputeScale(DxVector3 _vScale);
public:
	bool Init() override;
	void Input(float _fTime) override;
	void Update(float _fTime) override;
	void LateUpdate(float _fTime) override;
	void Collision(float _fTime) override;
	void Render(float _fTime) override;
	CUIBar* Clone() override;
public:
	void OnCollisionEnter(CCollider* _pSrc, CCollider* _pDest, float _fTime);
	void OnCollisionStay(CCollider* _pSrc, CCollider* _pDest, float _fTime);
	void OnCollisionLeave(CCollider* _pSrc, CCollider* _pDest, float _fTime);
protected:
	CUIBar();
	CUIBar(const CUIBar& _Bar);
	~CUIBar();
};

WOOJUN_END