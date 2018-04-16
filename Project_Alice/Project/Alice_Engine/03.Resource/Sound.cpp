#include "Sound.h"
#include "../01.Core/PathMgr.h"

WOOJUN_USING

bool CSound::LoadSound(FMOD::System * _System, const string & _strKey, const string& _strPath /*= SOUNDPATH*/)
{
	assert(_System);
	if (nullptr == _System)
	{
		return false;
	}

//#ifdef _UNICODE
//	string Path = CW2A(_Path.c_str(), CP_UTF8);
//	_System->createSound(Path.c_str(), FMOD_DEFAULT, nullptr, &m_pSound);
//#else
//	_System->createSound(_Path.c_str(), FMOD_DEFAULT, nullptr, &m_pSound);
//#endif
	
	string strFullPath = GET_SINGLE(CPathMgr)->FindPathToMultiByte(_strPath) + _strKey;
	_System->createSound(strFullPath.c_str(), FMOD_DEFAULT, nullptr, &m_pSound);

	if (nullptr == _System)
	{
		assert(_System);
		return false;
	}

	return true;
}

CSound::CSound()
{
}


CSound::~CSound()
{
}
