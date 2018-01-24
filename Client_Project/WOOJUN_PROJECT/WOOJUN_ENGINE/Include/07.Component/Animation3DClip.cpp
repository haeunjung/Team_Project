#include "Animation3DClip.h"

WOOJUN_USING

CAnimation3DClip::CAnimation3DClip()
{
}

CAnimation3DClip::CAnimation3DClip(const CAnimation3DClip & _Clip)
{
	*this = _Clip;
}

CAnimation3DClip::~CAnimation3DClip()
{
}

void CAnimation3DClip::SetClipInfo(const string & _strName, ANIMATION_OPTION _eOption, int _iAnimationLimitFrame, int _iStartFrame, int _iEndFrame, float _fStartTime, float _fEndTime)
{
	m_tInfo.eOption = _eOption;
	m_tInfo.strName = _strName;
	m_tInfo.iStartFrame = _iStartFrame;
	m_tInfo.iEndFrame = _iEndFrame;
	m_tInfo.iFrameLength = _iEndFrame - _iStartFrame;

	m_tInfo.fStartTime = _fStartTime;
	m_tInfo.fEndTime = _fEndTime;
	m_tInfo.fTimeLength = _fEndTime - _fStartTime;
}

void CAnimation3DClip::SetClipInfo(ANIMATION_OPTION _eOption, pFBXANIMATIONCLIP _pClip)
{
	m_tInfo.eOption = _eOption;
	m_tInfo.strName = _pClip->strName;
	m_tInfo.iStartFrame = _pClip->tStart.GetFrameCount(_pClip->eTimeMode);
	m_tInfo.iEndFrame = _pClip->tEnd.GetFrameCount(_pClip->eTimeMode);
	m_tInfo.iFrameLength = m_tInfo.iEndFrame - m_tInfo.iStartFrame;

	m_tInfo.fStartTime = _pClip->tStart.GetSecondDouble();
	m_tInfo.fEndTime = _pClip->tEnd.GetSecondDouble();
	m_tInfo.fTimeLength = m_tInfo.fEndTime - m_tInfo.fStartTime;
}

void CAnimation3DClip::Save(FILE * _pFile)
{
	fwrite(&m_tInfo.eOption, 4, 1, _pFile);

	int iLength = m_tInfo.strName.length();
	fwrite(&iLength, 4, 1, _pFile);
	fwrite(m_tInfo.strName.c_str(), 1, iLength, _pFile);

	fwrite(&m_tInfo.fStartTime, 4, 1, _pFile);
	fwrite(&m_tInfo.fEndTime, 4, 1, _pFile);
	fwrite(&m_tInfo.fTimeLength, 4, 1, _pFile);
	fwrite(&m_tInfo.iStartFrame, 4, 1, _pFile);
	fwrite(&m_tInfo.iEndFrame, 4, 1, _pFile);
	fwrite(&m_tInfo.iFrameLength, 4, 1, _pFile);	
}

void CAnimation3DClip::Load(FILE * _pFile)
{
	fread(&m_tInfo.eOption, 4, 1, _pFile);

	int iLength = 0;
	fread(&iLength, 4, 1, _pFile);
	char strName[256] = {};
	fread(strName, 1, iLength, _pFile);
	m_tInfo.strName = strName;

	fread(&m_tInfo.fStartTime, 4, 1, _pFile);
	fread(&m_tInfo.fEndTime, 4, 1, _pFile);
	fread(&m_tInfo.fTimeLength, 4, 1, _pFile);
	fread(&m_tInfo.iStartFrame, 4, 1, _pFile);
	fread(&m_tInfo.iEndFrame, 4, 1, _pFile);
	fread(&m_tInfo.iFrameLength, 4, 1, _pFile);
}

bool CAnimation3DClip::Init()
{
	return true;
}

int CAnimation3DClip::Update(float _fTime)
{
	return 0;
}

int CAnimation3DClip::LateUpdate(float _fTime)
{
	return 0;
}

CAnimation3DClip * CAnimation3DClip::Clone()
{
	return new CAnimation3DClip(*this);
}
