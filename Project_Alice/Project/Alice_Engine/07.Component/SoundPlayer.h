#pragma once
#include "Component.h"

WOOJUN_BEGIN

class DLL CSoundPlayer
	: public CComponent
{
public:
	friend class CResMgr;
private:
	static FMOD::System* m_pFmodSystem;
	static void SystemInit(FMOD::System* _pFmodSystem);
private:
	bool m_IsPlaying;
public:
	FMOD::Channel* m_pChannel;
	bool MyPlaySound(const string& _strKey, FMOD_MODE _Loop = FMOD_LOOP_OFF,
		unsigned int _LoopCount = -1, const string& _strPath = SOUNDPATH);
public:
	bool Init() override;
	void Input(float _fTime) override;
	void Update(float _fTime) override;
	void LateUpdate(float _fTime) override;
	void Collision(float _fTime) override;
	void Render(float _fTime) override;	
	CSoundPlayer* Clone() override;
public:
	CSoundPlayer();
	CSoundPlayer(const CSoundPlayer& _SoundPlayer);
	~CSoundPlayer();
};

WOOJUN_END