#pragma once
#include "../BaseObj.h"

WOOJUN_BEGIN

class CScene;
class CGameObject;
class DLL CLayer : public CBaseObj
{
private:
	friend class CScene;
private:	
	CScene*		m_pScene;

	list<CGameObject*>		m_listGameObject;
	list<CGameObject*>::iterator	m_iter;
	list<CGameObject*>::iterator	m_iterEnd;

	SORT_FLAG	m_eSortFlag;
	int			m_iZOrder;
	bool		m_bZOrder;
public:
	inline void SetZOrder(int iZOrder) {
		m_iZOrder = iZOrder;
		m_bZOrder = true;
	}
	inline int GetZOrder() const {
		return m_iZOrder;
	}
public:
	CScene* GetScene() const;
	void SetScene(CScene* _pScene);
public:
	void DisableZOrder();
	void SetSortFlag(SORT_FLAG _eFlag);
	bool GetZOrderEnable() const;
public:
	void AddObject(CGameObject* _pGameObject);
public:
	static bool ObjectZSort(CGameObject* _p1, CGameObject* _p2);
	static bool ObjectZSortDescending(CGameObject* _p1, CGameObject* _p2);
public:
	bool Init();
	void Input(float _fTime);
	void Update(float _fTime);
	void LateUpdate(float _fTime);
	void Collision(float _fTime);
	void Render(float _fTime);
private:
	CLayer();
	~CLayer();
};

WOOJUN_END

