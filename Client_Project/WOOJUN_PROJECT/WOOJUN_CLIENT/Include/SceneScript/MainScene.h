#pragma once
#include "05.Scene/SceneScript.h"
#include "06.GameObject/GameObject.h"

WOOJUN_USING

class CMainScene : public CSceneScript
{
private:
	CGameObject* m_pPlayerObject;
	float m_fRespawnTime;
	float m_fRespawnLimitTime;

	bool	m_bCheck;
private:
	void CreateProtoType();
	void CreateCheckBox();
	void CreateRadioButton();
	void CreateMouse();

	void CreateInventory();
public:
	virtual bool Init();
	virtual void Update(float _fTime);
public:
	void CheckButton(CGameObject* _pObj, float _fTime);
	void RadioButton(CGameObject* _pObj, float _fTime);
public:
	CMainScene();
	~CMainScene();
};

