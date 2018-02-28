#pragma once
#include "Light.h"

WOOJUN_BEGIN

class DLL CPointLight :
	public CLight
{
private:
	friend class CGameObject;
private:
	CPointLight();
	CPointLight(const CPointLight& _LightDir);
	~CPointLight();
public:
	virtual bool Init();
	virtual void Input(float _fTime);
	virtual void Update(float _fTime);
	virtual void LateUpdate(float _fTime);
	virtual void Collision(float _fTime);
	virtual void Render(float _fTime);
	virtual CPointLight* Clone();
public:
	virtual void OnCollisionEnter(CCollider* _pSrc, CCollider* _pDest, float _fTime);
	virtual void OnCollisionStay(CCollider* _pSrc, CCollider* _pDest, float _fTime);
	virtual void OnCollisionLeave(CCollider* _pSrc, CCollider* _pDest, float _fTime);
public:
	virtual void SetLight();
};

WOOJUN_END