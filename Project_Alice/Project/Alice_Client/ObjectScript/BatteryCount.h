#pragma once
#include "07.Component/Script.h"
#include "07.Component/Material.h"
#include "07.Component/SoundPlayer.h"

WOOJUN_USING

class CBatteryCount : public CScript
{
private:
	CMaterial * m_pMaterial;
	int m_iCount;
	string m_strKey;

	CSoundPlayer* m_pUseSound;
public:
	void PlusCount();
	void MinusCount();
public:
	bool Init() override;
	void Input(float _fTime) override;
	void Update(float _fTime) override;
	CBatteryCount* Clone() override;
private:
	void SetTexture();
public:
	CBatteryCount();
	CBatteryCount(const CBatteryCount& _BatteryCount);
	~CBatteryCount();
};

