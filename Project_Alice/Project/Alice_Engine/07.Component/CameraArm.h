#pragma once
#include "Component.h"

WOOJUN_BEGIN

class DLL CCameraArm :
	public CComponent
{
private:
	friend class CGameObject;
private:
	float		m_fMinDist;
	float		m_fMaxDist;
	float		m_fZoomSpeed;	
private:
	void Zoom(float _fTime);
	void RotationDrag(float _fTime);
public:
	void SetZoomDistance(float _fMin, float _fMax);
	void SetZoomSpeed(float _fSpeed);
public:
	bool Init() override;
	void Input(float _fTime) override;
	void Update(float _fTime) override;
	void LateUpdate(float _fTime) override;
	void Collision(float _fTime) override;
	void Render(float _fTime) override;
	CCameraArm* Clone() override;
private:
	CCameraArm();
	CCameraArm(const CCameraArm& _CameraArm);
	~CCameraArm();
};

WOOJUN_END