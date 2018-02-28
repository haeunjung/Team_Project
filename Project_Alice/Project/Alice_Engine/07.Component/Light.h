#pragma once
#include "Component.h"

WOOJUN_BEGIN

class DLL CLight :
	public CComponent
{
protected:
	LIGHTINFO	m_tInfo;
	LIGHTCBUFFER	m_tCBuffer;
public:
	inline LIGHTINFO GetLightInfo() const {
		return m_tInfo;
	}
public:
	void SetLightInfo(const LIGHTINFO& _tInfo);
	void SetLightInfo(const DxVector4& _vDiffuse, const DxVector4& _vAmbient, const DxVector4& _vSpecular);
public:
	virtual bool Init() = 0;
	virtual void Input(float _fTime) = 0;
	virtual void Update(float _fTime) = 0;
	virtual void LateUpdate(float _fTime) = 0;
	virtual void Collision(float _fTime) = 0;
	virtual void Render(float _fTime) = 0;
	virtual CLight* Clone() = 0;
public:
	virtual void OnCollisionEnter(CCollider* _pSrc, CCollider* _pDest, float _fTime);
	virtual void OnCollisionStay(CCollider* _pSrc, CCollider* _pDest, float _fTime);
	virtual void OnCollisionLeave(CCollider* _pSrc, CCollider* _pDest, float _fTime);
public:
	virtual void SetLight();
protected:
	CLight();
	CLight(const CLight& _Light);
	virtual ~CLight() = 0;
};

WOOJUN_END