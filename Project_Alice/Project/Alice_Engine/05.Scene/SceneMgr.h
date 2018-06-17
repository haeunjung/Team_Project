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

	bool		m_bChange;
public:
	inline CScene* GetCurScene() {
		return m_pCurScene;
	}
	inline bool GetIsChange() {
		return m_bChange;
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

// Tool ¿ë
private:
	unordered_map<string, CScene*>	m_mapScene;
public:
	bool InsertScene(CScene* _pScene);
	CScene* FindScene(const string& _strKey);
	bool ChangeScene(const string& _strKey);

	DECLARE_SINGLE(CSceneMgr)
};

WOOJUN_END