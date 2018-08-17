#pragma once
#include "05.Scene/Layer.h"
#include "07.Component/Transform.h"
#include "07.Component/Renderer2D.h"
#include "07.Component/Material.h"
#include "../ObjectScript/BatteryCount.h"
#include "../ObjectScript/GearCount.h"

WOOJUN_USING

class CPlayer;
class CTimeBar;
class CUIMgr
{
private:
	bool m_bInit;

	float m_fColor;
	bool m_bColor;

	CBatteryCount * m_pBatteryCount;
	CMaterial * m_pBattery2DMaterial;
	bool m_bGetBattery;

	CGearCount* m_pGearCount;
	CMaterial * m_pGear2DMaterial;
	bool m_bGetGear;	

	CTimeBar * m_pTimeBar;

	CTransform* m_pSpringTransform;

	vector<CRenderer2D*> m_vecHpRenderer2D;
	int m_PlayerHp;
public:
	void GetBattery();
	void UseBattery();
	void GetGear();
	int GetGearCount();
	void SetHpOff(int _Hp);
	void SetHpOn(int _Hp);
	void SetPlayer(CPlayer* _pPlayer);
public:
	bool Init(CScene* _pScene);	
	void Update(float _fTime);
private:
	void CreateBattery2D(CLayer* _pLayer);
	void CreateBatteryCount(CLayer* _pLayer);
	void CreateGear2D(CLayer* _pLayer);
	void CreateGearCount(CLayer* _pLayer);
	void CreateHpIcon(CLayer* _pLayer);
	void CreateTimeBar(CLayer* _pLayer);
	void CreateSpring(CLayer* _pLayer);

	DECLARE_SINGLE(CUIMgr)
};

