#pragma once
#include "05.Scene/Layer.h"
#include "07.Component/Transform.h"

WOOJUN_USING

class CBatteryCount;
class CTimeBar;
class CUIMgr
{
private:
	CBatteryCount * m_pBatteryCount;
	CTimeBar * m_pTimeBar;

	CTransform* m_pSpringTransform;
public:
	void GetBattery();
	void UseBattery();
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

