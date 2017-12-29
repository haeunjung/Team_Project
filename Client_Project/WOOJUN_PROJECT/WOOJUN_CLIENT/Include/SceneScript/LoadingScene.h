#pragma once
#include "05.Scene/SceneScript.h"

WOOJUN_USING

class CLoadingScene :
	public CSceneScript
{
public:
	bool Init() override;
	void Update(float _fTime) override;
public:
	CLoadingScene();
	~CLoadingScene();
};

