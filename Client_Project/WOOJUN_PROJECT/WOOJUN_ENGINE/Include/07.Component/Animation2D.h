#pragma once
#include "Component.h"

WOOJUN_BEGIN

class DLL CAnimation2D :
	public CComponent
{
private:
	friend class CGameObject;
private:
	vector<pANIMATIONCLIP2D>	m_vecAnimationClip;
	ANIMATION2DCBUFFER			m_tCBuffer;
	int							m_iCurrentAnimation;
	int							m_iDefaultAnimation;
public:
	void AddAnimation2DClip(const string& _strName, ANIMATION2D_TYPE _eType, ANIMATION2D_OPTION _eOption,
		int _iFrameMaxX, int _iFrameMaxY, float _fLimitTime,
		int _iLoopCount = 0, float _fLoopTime = 0.0f,
		const string& _strTexKey = "", int _iTexRegister = 0, TCHAR* _pFileName = NULL,
		const string& _strPathKey = TEXTUREPATH);
	void AddAnimation2DClip(const string& _strName, ANIMATION2D_TYPE _eType, ANIMATION2D_OPTION _eOption,
		int _iFrameMaxX, int _iFrameMaxY, float _fLimitTime,
		int _iLoopCount = 0, float _fLoopTime = 0.0f,
		const string& _strTexKey = "", int _iTexRegister = 0, const vector<wstring>* _vecFileName = NULL,
		const string& _strPathKey = TEXTUREPATH);
	void ChangeAnimation(int _iAnimation);
	void SetDefaultAnimation(int _iAnimation);
	void SetShader();
	void Start(bool _b2D);
public:
	virtual bool Init();
	virtual void Input(float _fTime);
	virtual void Update(float _fTime);
	virtual void LateUpdate(float _fTime);
	virtual void Collision(float _fTime);
	virtual void Render(float _fTime);
	virtual CAnimation2D* Clone();
private:
	CAnimation2D();
	CAnimation2D(const CAnimation2D& _Animation2D);
	~CAnimation2D();
};

WOOJUN_END