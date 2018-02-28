#pragma once
#include "../BaseObj.h"

WOOJUN_BEGIN

class DLL CRenderState :
	public CBaseObj
{
protected:
	friend class CRenderMgr;
protected:
	// ID3D11RasterizerState
	// ID3D11BlendState
	// ID3D11DepthStencilState
	// 3가지 State 모드 ID3D11DeviceChild를 상속받아 만든다
	// 그러므로 ID3D11DeviceChild로 관리가 가능하다
	ID3D11DeviceChild*	m_pState;
	ID3D11DeviceChild*	m_pDefaultState;
	string				m_strKey;
	RENDER_STATE_TYPE	m_eStateType;
public:
	inline RENDER_STATE_TYPE GetType() const {
		return m_eStateType;
	}
public:
	virtual void SetState() = 0;
	virtual void ResetState() = 0;
protected:
	CRenderState();
	virtual ~CRenderState();
};

WOOJUN_END