#pragma once
#include "Light.h"

WOOJUN_BEGIN

class CGameObject;
class CTransform;
class DLL CPlayerLight :
	public CLight
{
private:
	friend class CGameObject;
private:
	CTransform* m_pAttachTransform;
	DxVector3 m_vDir;
	bool m_bLight;
public:
	void SetAttachObject(CTransform* _pAttachTransform);
public:
	virtual bool Init();
	virtual void Input(float _fTime);
	virtual void Update(float _fTime);
	virtual void LateUpdate(float _fTime);
	virtual void Collision(float _fTime);
	virtual void Render(float _fTime);
	virtual CPlayerLight* Clone();
public:
	virtual void SetLight();
private:
	CPlayerLight();
	CPlayerLight(const CPlayerLight& _PlayerLight);
	~CPlayerLight();
};

WOOJUN_END