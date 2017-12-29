#pragma once
#include "../Engine_Header.h"

WOOJUN_BEGIN

class CScene;
class DLL CSceneScript
{
protected:
	friend class CScene;
protected:
	CScene*		m_pScene;
public:
	virtual bool Init() = 0;
	virtual void Input(float _fTime);
	virtual void Update(float _fTime);
	virtual void LateUpdate(float _fTime);
	virtual void Collision(float _fTime);
	virtual void Render(float _fTime);
public:
	CSceneScript();
	virtual ~CSceneScript() = 0;
};

WOOJUN_END