#pragma once
#include "Component.h"

WOOJUN_BEGIN

class CGameObject;
class CTransform;
class DLL CCamera : public CComponent
{
public:
	friend class CGameObject;
private:
	pMATRIX			m_matView;
	pMATRIX			m_matProj;
	CGameObject*	m_pAttachObject;
	CTransform*		m_pAttachTransform;
	DxVector3		m_vPrevPos;
	DxVector3		m_vDist;
	DxVector3		m_vAttachPos;
	DxVector3		m_vMove;

	bool			m_bAction;
	float			m_fActionTime;
	DxVector3		m_vRandPos;

	DxVector3		m_vAxis[AXIS_MAX];
public:
	XMMATRIX GetViewMatrix() const;
	XMMATRIX GetProjMatrix() const;
	DxVector3 GetAxis(AXIS _Axis) const;
public:
	void SetAxis(DxVector3 _vAxis[AXIS_MAX]);
	void SetAxis(const DxVector3& _vAxis, AXIS _Axis);
public:
	void Attach(CGameObject* _pGameObject, const DxVector3& _vDist);
	void Detach();
	CGameObject* GetAttachObject();
	void ActionCameraOn();
public:
	void CreateProjection(float _fAngle, float _fWidth, float _fHeight, float _fNear = 0.1f, float _fFar = 1000.0f);
	void CreateOrthoProjection(float _fWidth, float _fHeight, float _fNear = 0.1f, float _fFar = 1000.0f);
private:
	void ComputeViewMatrix();
	void ActionCamera(float _fTime);
public:
	bool Init() override;
	void Input(float _fTime) override;
	void Update(float _fTime) override;
	void LateUpdate(float _fTime) override;
	void Collision(float _fTime) override;
	void Render(float _fTime) override;
	CCamera* Clone() override;
private:
	CCamera();
	CCamera(const CCamera& _Camera);
	~CCamera();
};

WOOJUN_END