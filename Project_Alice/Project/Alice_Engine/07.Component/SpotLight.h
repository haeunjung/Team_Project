#pragma once
#include "Light.h"

WOOJUN_BEGIN

class CColliderRay;
class CColliderSphere;
class DLL CSpotLight :
	public CLight
{
private:
	CColliderRay* m_pColRay;
	CColliderSphere* m_pColSphere;
	
	DxVector3 m_ColPos;
	float m_ColDist;
	DxVector3 m_Dir;
private:
	friend class CGameObject;
private:
	CSpotLight();
	CSpotLight(const CSpotLight& _SpotLight);
	~CSpotLight();
public:
	virtual bool Init();
	virtual void Input(float _fTime);
	virtual void Update(float _fTime);
	virtual void LateUpdate(float _fTime);
	virtual void Collision(float _fTime);
	virtual void Render(float _fTime);
	virtual CSpotLight* Clone();
public:
	virtual void OnCollisionEnter(CCollider* _pSrc, CCollider* _pDest, float _fTime);
	virtual void OnCollisionStay(CCollider* _pSrc, CCollider* _pDest, float _fTime);
	virtual void OnCollisionLeave(CCollider* _pSrc, CCollider* _pDest, float _fTime);
public:
	virtual void SetLight();
};

WOOJUN_END