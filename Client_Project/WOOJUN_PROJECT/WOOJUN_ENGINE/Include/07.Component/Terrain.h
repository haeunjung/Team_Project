#pragma once
#include "Component.h"

WOOJUN_BEGIN

class DLL CTerrain :
	public CComponent
{
protected:
	friend class CGameObject;
private:
	UINT	m_iVtxNumX;
	UINT	m_iVtxNumZ;
	UINT	m_iVtxSizeX;
	UINT	m_iVtxSizeZ;

	vector<DxVector3>	m_vecPos;
public:
	bool CreateTerrain(const string& _strKey, UINT _iVtxNumX, UINT _iVtxNumZ, UINT _iVtxSizeX, UINT _iVtxSizeZ);
	void SetBaseTexture(const string& _strKey, TCHAR* _pFileName, const string& _strPathKey = TEXTUREPATH);
	void SetNormalTexture(const string& _strKey, TCHAR* _pFileName, const string& _strPathKey = TEXTUREPATH);
	void SetSpecularTexture(const string& _strKey, TCHAR* _pFileName, const string& _strPathKey = TEXTUREPATH);
public:
	bool Init() override;
	void Input(float _fTime) override;
	void Update(float _fTime) override;
	void LateUpdate(float _fTime) override;
	void Collision(float _fTime) override;
	void Render(float _fTime) override;
	CTerrain* Clone() override;
public:
	void OnCollisionEnter(CCollider* _pSrc, CCollider* _pDest, float _fTime);
	void OnCollisionStay(CCollider* _pSrc, CCollider* _pDest, float _fTime);
	void OnCollisionLeave(CCollider* _pSrc, CCollider* _pDest, float _fTime);
protected:
	CTerrain();
	CTerrain(const CTerrain& _Terrain);
	~CTerrain();
};

WOOJUN_END