#pragma once
#include "../Engine_Header.h"

WOOJUN_BEGIN

class CScene;
class DLL CSceneMgr
{
private:
	CScene*		m_pCurScene;
	CScene*		m_pNextScene;
	CScene*		m_pPrevScene;	
public:
	inline CScene* GetCurScene() {
		return m_pCurScene;
	}
public:
	CScene* CreateScene(const string& _strTag);
	int ChangeScene();
	void ReplaceScene(CScene* _pScene);
	void PushScene(CScene* _pScene);
	void PrevScene();
public:
	bool Init();
	int Input(float _fTime);
	int Update(float _fTime);
	int LateUpdate(float _fTime);
	void Collision(float _fTime);
	void Render(float _fTime);

	DECLARE_SINGLE(CSceneMgr)
};

WOOJUN_END