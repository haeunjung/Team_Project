#pragma once
#include "Light.h"

WOOJUN_BEGIN

class CGameObject;
class CSpotLight;
class DLL CSpotParent :
	public CLight
{
private:
	CGameObject* m_pChild;
	CSpotLight* m_pChildSpotLight;
public:
	void InitChildSpotLight();

private:
	friend class CGameObject;
private:
	CSpotParent();
	CSpotParent(const CSpotParent& _SpotParent);
	~CSpotParent();

public:
	virtual bool Init();
	virtual void Input(float _fTime);
	virtual void Update(float _fTime);
	virtual void LateUpdate(float _fTime);
	virtual void Collision(float _fTime);
	virtual void Render(float _fTime);
	virtual CSpotParent* Clone();
public:
	virtual void OnCollisionEnter(CCollider* _pSrc, CCollider* _pDest, float _fTime);
	virtual void OnCollisionStay(CCollider* _pSrc, CCollider* _pDest, float _fTime);
	virtual void OnCollisionLeave(CCollider* _pSrc, CCollider* _pDest, float _fTime);
public:
	virtual void SetLight();
};

WOOJUN_END
