#pragma once
#include "../BaseObj.h"
#include "../03.Resource/FbxLoader.h"

WOOJUN_BEGIN

typedef struct DLL _tagAnimationCallback
{
	int		iAnimationProgress;
	float	fAnimationProgress;
	function<void(float)>	func;
}ANIMATIONCALLBACK, *pANIMATIONCALLBACK;

typedef struct DLL _tagAnimation3DClip
{
	ANIMATION_OPTION	eOption;
	string		strName;
	float		fStartTime;
	float		fEndTime;
	float		fTimeLength;
	int			iStartFrame;
	int			iEndFrame;
	int			iFrameLength;
	vector<pANIMATIONCALLBACK>	vecCallback;

	_tagAnimation3DClip() :
		eOption(AO_LOOP),
		strName(""),
		fStartTime(0.0f),
		fEndTime(0.0f),
		fTimeLength(0.0f),
		iStartFrame(0),
		iEndFrame(0),
		iFrameLength(0)
	{
	}
}ANIMATION3DCLIP, *pANIMATION3DCLIP;

class DLL CAnimation3DClip :
	public CBaseObj
{
private:
	friend class CAnimation3D;
private:
	ANIMATION3DCLIP		m_tInfo;
	int		m_iAnimationLimitFrame;
public:
	ANIMATION3DCLIP GetInfo() const;
public:
	void AddCallback(int _iFrame, void(*_pFunc)(float));
	template <typename T>
	void AddCallback(int _iFrame, T* _pObj, void(T::*_pFunc)(float))
	{
		pANIMATIONCALLBACK	pCallback = new ANIMATIONCALLBACK();

		pCallback->iAnimationProgress = _iFrame;
		pCallback->fAnimationProgress = (_iFrame - m_tInfo.iStartFrame) / (float)m_tInfo.iEndFrame;
		pCallback->func = bind(_pFunc, _pObj, placeholders::_1);

		m_tInfo.vecCallback.push_back(pCallback);
	}

	void AddCallback(float _fProgress, void(*_pFunc)(float));
	template <typename T>
	void AddCallback(float _fProgress, T* _pObj, void(T::*_pFunc)(float))
	{
		pANIMATIONCALLBACK	pCallback = new ANIMATIONCALLBACK();

		pCallback->iAnimationProgress = (_fProgress * m_tInfo.fTimeLength + m_tInfo.fStartTime) * m_iAnimationLimitFrame;
		pCallback->fAnimationProgress = _fProgress;
		pCallback->func = bind(_pFunc, _pObj, placeholders::_1);

		m_tInfo.vecCallback.push_back(pCallback);
	}
public:
	void SetClipInfo(const string& _strName, ANIMATION_OPTION _eOption,
		int _iAnimationLimitFrame, int _iStartFrame, int _iEndFrame,
		float _fStartTime, float _fEndTime);
	void SetClipInfo(ANIMATION_OPTION _eOption, pFBXANIMATIONCLIP _pClip);
public:
	void Save(FILE* _pFile);
	void Load(FILE* _pFile);
public:
	bool Init();
	int Update(float _fTime);
	int LateUpdate(float _fTime);
	CAnimation3DClip* Clone();
private:
	CAnimation3DClip();
	CAnimation3DClip(const CAnimation3DClip& _Clip);
	~CAnimation3DClip();
};

WOOJUN_END