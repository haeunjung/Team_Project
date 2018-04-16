#include "SoundPlayer.h"
#include "../03.Resource/Sound.h"
#include "../03.Resource/ResMgr.h"

WOOJUN_USING

FMOD::System* CSoundPlayer::m_pFmodSystem = nullptr;

CSoundPlayer::CSoundPlayer() : m_pChannel(nullptr)
{
}

CSoundPlayer::CSoundPlayer(const CSoundPlayer & _SoundPlayer) :
	CComponent(_SoundPlayer)
{

}

CSoundPlayer::~CSoundPlayer()
{
	m_pChannel->stop();
	m_pChannel = nullptr;
}

void CSoundPlayer::SystemInit(FMOD::System * _pFmodSystem)
{
	m_pFmodSystem = _pFmodSystem;
}

bool CSoundPlayer::MyPlaySound(const string& _strKey, FMOD_MODE _Loop /*= FMOD_LOOP_OFF*/,
	unsigned int _LoopCount /*= -1*/, const string& _strPath /*= SOUNDPATH*/)
{
	if (nullptr != m_pChannel)
	{
		m_pChannel->stop();
		m_pChannel = nullptr;		
	}

	if (nullptr == m_pFmodSystem)
	{
		assert(m_pFmodSystem);
		return false;
	}

	CSound* pSound = GET_SINGLE(CResMgr)->FindSound(_strKey);
	if (nullptr == pSound)
	{
		pSound = GET_SINGLE(CResMgr)->SoundLoad(_strKey, _strPath);
	}
		
	m_pFmodSystem->playSound(pSound->GetSound(), nullptr, false, &m_pChannel);
	SAFE_RELEASE(pSound);
	
	if (nullptr == m_pChannel)
	{
		assert(m_pChannel);
		return false;
	}
		
	m_pChannel->setMode(_Loop);

	if (FMOD_LOOP_OFF != _Loop)
	{
		m_pChannel->setLoopCount(_LoopCount);
	}

	m_pChannel->setUserData(this);

	return true;
}

bool CSoundPlayer::Init()
{
	return true;
}

void CSoundPlayer::Input(float _fTime)
{
}

void CSoundPlayer::Update(float _fTime)
{
	if (nullptr == m_pChannel)
	{
		return;
	}

	bool IsPlaying;
	m_pChannel->isPlaying(&IsPlaying);
	if (false == IsPlaying)
	{
		m_pChannel->stop();
		m_pChannel = nullptr;
	}	
}

void CSoundPlayer::LateUpdate(float _fTime)
{
}

void CSoundPlayer::Collision(float _fTime)
{
}

void CSoundPlayer::Render(float _fTime)
{
}

CSoundPlayer * CSoundPlayer::Clone()
{
	return new CSoundPlayer(*this);
}


