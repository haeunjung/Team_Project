#pragma once
#include "05.Scene/SceneScript.h"

WOOJUN_USING

class CToolScene :
	public CSceneScript
{
private:
	void CreateTerrain();
public:
	bool Init() override;
	void Update(float _fTime) override;
public:
	CToolScene();
	~CToolScene();
};

