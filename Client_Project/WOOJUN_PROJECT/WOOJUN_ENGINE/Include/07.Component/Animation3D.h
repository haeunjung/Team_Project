#pragma once
#include "Component.h"

WOOJUN_BEGIN

class DLL CAnimation3D :
	public CComponent
{
private:
	friend class CGameObject;
public:
	bool Init() override ;
	void Input(float _fTime) override;
	void Update(float _fTime) override;
	void LateUpdate(float _fTime) override;
	void Collision(float _fTime) override;
	void Render(float _fTime) override;
	CAnimation3D* Clone() override;
private:
	CAnimation3D();
	CAnimation3D(const CAnimation3D& _Animation3D);
	~CAnimation3D();
};

WOOJUN_END