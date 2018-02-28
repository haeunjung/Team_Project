#pragma once
#include "Component.h"

WOOJUN_BEGIN

class DLL CFrustum :
	public CComponent
{
private:
	friend class CGameObject;
private:
	DxVector3		m_vPos[8];			// 절두체로 쓰일 육면체 정점의 위치 8개
	XMVECTOR		m_Plane[FP_END];	// 평면 6개
public:
	bool FrustumInPoint(const DxVector3& _vPos);
	bool FrustumInSphere(const SPHEREINFO& _tSphere);
	bool FrustumInSphere(const DxVector3& _vCenter, float _fRad);
public:
	bool Init() override;
	void Input(float _fTime) override;
	void Update(float _fTime) override;
	void LateUpdate(float _fTime) override;
	void Collision(float _fTime) override;
	void Render(float _fTime) override;
	CFrustum* Clone() override;
private:
	CFrustum();
	CFrustum(const CFrustum& _Frustum);
	~CFrustum();
};

WOOJUN_END