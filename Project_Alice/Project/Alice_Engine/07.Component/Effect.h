#pragma once
#include "Component.h"

WOOJUN_BEGIN

class DLL CEffect :
	public CComponent
{
private:
	friend class CGameObject;
private:
	EFFECTCBUFFER	m_tEffectCBuffer;
public:
	void SetEffectCBuffer();
public:
	virtual bool Init();
	virtual void Input(float _fTime);
	virtual void Update(float _fTime);
	virtual void LateUpdate(float _fTime);
	virtual void Collision(float _fTime);
	virtual void Render(float _fTime);
	virtual CEffect* Clone();
private:
	CEffect();
	CEffect(const CEffect& _Effect);
	~CEffect();
};

WOOJUN_END