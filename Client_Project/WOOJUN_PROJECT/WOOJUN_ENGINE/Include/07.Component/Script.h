#pragma once
#include "Component.h"

WOOJUN_BEGIN

class CCollider;
class DLL CScript :	public CComponent
{
public:
	virtual bool Init() = 0;
	virtual void Input(float _fTime) override;
	virtual void Update(float _fTime) override;
	virtual void LateUpdate(float _fTime) override;
	virtual void Collision(float _fTime) override;
	virtual void Render(float _fTime) override;
	virtual CScript* Clone() override;
public:
	virtual void OnCollisionEnter(CCollider* _pSrc, CCollider* _pDest, float _fTime) override;
	virtual void OnCollisionStay(CCollider* _pSrc, CCollider* _pDest, float _fTime) override;
	virtual void OnCollisionLeave(CCollider* _pSrc, CCollider* _pDest, float _fTime) override;
protected:
	CScript();
	CScript(const CScript& _Script);
	virtual ~CScript() = 0;
};

WOOJUN_END