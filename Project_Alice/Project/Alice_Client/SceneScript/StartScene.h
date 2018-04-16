#pragma once
#include "05.Scene/SceneScript.h"
#include "06.GameObject/GameObject.h"
#include "07.Component/SoundPlayer.h"

WOOJUN_USING

class CStartScene :
	public CSceneScript
{
private:
	CSoundPlayer* m_pSoundPlayer;
public:
	virtual bool Init();
	virtual void Update(float _fTime);
private:
	void CreateStartButton();
	void CreateEndButton();

public:
	void ExitButton(CGameObject* _pObj, float _fTime);
	void StartButton(CGameObject* _pObj, float _fTime);
public:
	CStartScene();
	~CStartScene();
};

