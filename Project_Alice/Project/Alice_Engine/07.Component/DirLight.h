#pragma once
#include "Light.h"

WOOJUN_BEGIN

class DLL CDirLight :
	public CLight
{
private:
	friend class CGameObject;
private:
	CDirLight();
	CDirLight(const CDirLight& _DirLight);
	~CDirLight();
public:
	virtual bool Init();
	virtual void Input(float _fTime);
	virtual void Update(float _fTime);
	virtual void LateUpdate(float _fTime);
	virtual void Collision(float _fTime);
	virtual void Render(float _fTime);
	virtual CDirLight* Clone();
public:
	virtual void OnCollisionEnter(CCollider* _pSrc, CCollider* _pDest, float _fTime);
	virtual void OnCollisionStay(CCollider* _pSrc, CCollider* _pDest, float _fTime);
	virtual void OnCollisionLeave(CCollider* _pSrc, CCollider* _pDest, float _fTime);
public:
	virtual void SetLight();
};

WOOJUN_END