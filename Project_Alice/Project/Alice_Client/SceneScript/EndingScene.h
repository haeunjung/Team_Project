#pragma once
#include "05.Scene/SceneScript.h"
#include "07.Component/SoundPlayer.h"

WOOJUN_USING

class CEndingScene :
	public CSceneScript
{
private:
	CSoundPlayer * m_pSoundPlayer;
	float m_fTime;
public:
	bool Init() override;
	void Update(float _fTime) override;
public:
	CEndingScene();
	~CEndingScene();
};

