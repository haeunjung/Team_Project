#pragma once
#include "05.Scene/Layer.h"
#include "07.Component/Transform.h"
#include "07.Component/Renderer2D.h"
#include "07.Component/Material.h"

WOOJUN_USING

class CPlayer;
class CBatteryCount;
class CTimeBar;
class CUIMgr
{
private:
	CMaterial * m_pBattery2DMaterial;
	bool m_bGetBattery;
	float m_fColor;
	bool m_bColor;

	CBatteryCount * m_pBatteryCount;

	CTimeBar * m_pTimeBar;

	CTransform* m_pSpringTransform;

	vector<CRenderer2D*> m_vecHpRenderer2D;
	int m_PlayerHp;
public:
	void GetBattery();
	void UseBattery();
	void SetHp(int _Hp);
	void SetPlayer(CPlayer* _pPlayer);
public:
	bool Init(CScene* _pScene);	
	void Update(float _fTime);
private:
	void CreateBattery2D(CLayer* _pLayer);
	void CreateBatteryCount(CLayer* _pLayer);
	void CreateHpIcon(CLayer* _pLayer);
	void CreateTimeBar(CLayer* _pLayer);
	void CreateSpring(CLayer* _pLayer);

	DECLARE_SINGLE(CUIMgr)
};

