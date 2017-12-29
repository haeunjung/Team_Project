#pragma once
#include "05.Scene/SceneScript.h"

class CToolScene :
	public CSceneScript
{
public:
	bool Init() override;
	void Update(float _fTime) override;
public:
	CToolScene();
	~CToolScene();
};

