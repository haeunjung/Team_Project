#pragma once
#include "07.Component/Script.h"
#include "07.Component/Material.h"

WOOJUN_USING

class CGearCount : public CScript
{
private:
	CMaterial * m_pMaterial;
	int m_iCount;
	string m_strKey;
public:
	void PlusCount();
	void MinusCount();
	int GetCount();
public:
	bool Init() override;
	void Update(float _fTime) override;
	CGearCount* Clone() override;
private:
	void SetTexture();
public:
	CGearCount();
	CGearCount(const CGearCount& _GearCount);
	~CGearCount();
};

