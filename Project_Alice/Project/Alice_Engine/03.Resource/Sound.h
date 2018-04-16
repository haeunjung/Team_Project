#pragma once
#include "../BaseObj.h"

WOOJUN_BEGIN

class DLL CSound
	: public CBaseObj
{
private:
	FMOD::Sound* m_pSound;
public:
	FMOD::Sound* GetSound() {
		return m_pSound;
	}
public:
	bool LoadSound(FMOD::System*  _System, const string& _strKey, const string& _strPath = SOUNDPATH);
public:
	CSound();
	~CSound();
};

WOOJUN_END