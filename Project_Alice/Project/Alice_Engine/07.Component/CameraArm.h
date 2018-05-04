#pragma once
#include "Component.h"
#include "ColliderSphere.h"

WOOJUN_BEGIN

class DLL CCameraArm :
	public CComponent
{
private:
	CColliderSphere * m_pColSphere;
	bool m_bLCol;
	bool m_bRCol;
private:
	friend class CGameObject;
private:
	float		m_fMinDist;
	float		m_fMaxDist;
	float		m_fZoomSpeed;	
private:
	void Zoom(float _fTime);
	void RotationDrag(float _fTime);
	void ColPosCheck(const DxVector3 _vPos);
public:
	void SetZoomDistance(float _fMin, float _fMax);
	void SetZoomSpeed(float _fSpeed);
	bool RotateY(float _fAngle, float _fTime);
	bool RotateY(MATRIX _matRot);
public:
	bool Init() override;
	void Input(float _fTime) override;
	void Update(float _fTime) override;
	void LateUpdate(float _fTime) override;
	void Collision(float _fTime) override;
	void Render(float _fTime) override;
	CCameraArm* Clone() override;
public:
	void OnCollisionStay(CCollider* _pSrc, CCollider* _pDest, float _fTime) override;
	void OnCollisionLeave(CCollider* _pSrc, CCollider* _pDest, float _fTime) override;
private:
	CCameraArm();
	CCameraArm(const CCameraArm& _CameraArm);
	~CCameraArm();
};

WOOJUN_END