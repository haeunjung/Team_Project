#pragma once
#include "05.Scene/SceneScript.h"
#include "06.GameObject/GameObject.h"
#include "07.Component/UIBar.h"

WOOJUN_USING

class CMainScene : public CSceneScript
{
private:
	CGameObject* m_pPlayerObject;
	float m_fRespawnTime;
	float m_fRespawnLimitTime;

	bool	m_bCheck;
	CUIBar*	m_pHpBar;
private:
	void CreateProtoType();
	void CreateObject();
	void LoadObject(const string& _strKey, const DxVector3& _vPos, const DxVector3& _vScale, const DxVector3& _vRot);
	void CreateCheckBox();
	void CreateRadioButton();	
	void CreateTerrain();
	void CreateInventory();
	void CreateHpBar();
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

