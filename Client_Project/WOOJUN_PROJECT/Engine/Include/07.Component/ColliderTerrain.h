#pragma once
#include "Collider.h"

WOOJUN_BEGIN

class DLL CColliderTerrain :
	public CCollider
{
private:
	friend class CGameObject;
private:
	TERRAININFO		m_tTerrainInfo;
public:
	TERRAININFO GetTerrainInfo() const;
	void SetTerrainInfo(const vector<DxVector3>& _vecPos, UINT _iNumW, UINT _iNumH);
public:
	bool Init() override;
	void Input(float _fTime) override;
	void Update(float _fTime) override;
	void LateUpdate(float _fTime) override;
	void Collision(float _fTime) override;
	void Render(float _fTime) override;
	CColliderTerrain* Clone() override;
	bool Collision(CCollider* _pCollider) override;
private:
	CColliderTerrain();
	CColliderTerrain(const CColliderTerrain& _ColliderTerrain);
	~CColliderTerrain();
};

WOOJUN_END