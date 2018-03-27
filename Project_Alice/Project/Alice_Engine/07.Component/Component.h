#pragma once
#include "../BaseObj.h"

WOOJUN_BEGIN

class CScene;
class CLayer;
class CGameObject;
class CTransform;
class CCollider;
class DLL CComponent : public CBaseObj
{
protected:
	friend class CGameObject;
protected:
	CScene*			m_pScene;
	CLayer*			m_pLayer;
	CGameObject*	m_pGameObject;
	CTransform*		m_pTransform;
	COMPONENT_TYPE	m_eComponentType;
public:
	CGameObject* GetGameObject() const;
	CTransform* GetTransform() const;
	inline COMPONENT_TYPE& GetComponentType() {
		return m_eComponentType;
	}
	inline void SetScene(CScene* _pScene) {
		m_pScene = _pScene;
	}
	inline void SetLayer(CLayer* _pLayer) {
		m_pLayer = _pLayer;
	}
	inline void SetGameObject(CGameObject* _pGameObject) {
		m_pGameObject = _pGameObject;
	}
private:
	inline void SetTransform(CTransform* _pTransfrom) {
		m_pTransform = _pTransfrom;
	}
public:
	virtual bool Init() = 0;
	virtual void Input(float _fTime) = 0;
	virtual void Update(float _fTime) = 0;
	virtual void LateUpdate(float _fTime) = 0;
	virtual void Collision(float _fTime) = 0;
	virtual void Render(float _fTime) = 0;
	virtual CComponent* Clone() = 0;
public:
	virtual void OnCollisionEnter(CCollider* _pSrc, CCollider* _pDest, float _fTime);
	virtual void OnCollisionStay(CCollider* _pSrc, CCollider* _pDest, float _fTime);
	virtual void OnCollisionLeave(CCollider* _pSrc, CCollider* _pDest, float _fTime);
protected:
	CComponent();
	CComponent(const CComponent& _Component);
	virtual ~CComponent() = 0;
};

WOOJUN_END