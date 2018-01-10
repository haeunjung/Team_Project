#pragma once
#include "Component.h"

WOOJUN_BEGIN

class DLL CCameraFree :
	public CComponent
{
private:
	friend class CGameObject;
private:	
	void RotationDrag(float _fTime);
public:
	bool Init() override;
	void Input(float _fTime) override;
	void Update(float _fTime) override;
	void LateUpdate(float _fTime) override;
	void Collision(float _fTime) override;
	void Render(float _fTime) override;
	CCameraFree* Clone() override;
private:
	CCameraFree();
	CCameraFree(const CCameraFree& _CameraFree);
	~CCameraFree();
};

WOOJUN_END