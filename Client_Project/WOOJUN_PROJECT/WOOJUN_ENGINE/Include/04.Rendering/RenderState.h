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
	// 3���� State ��� ID3D11DeviceChild�� ��ӹ޾� �����
	// �׷��Ƿ� ID3D11DeviceChild�� ������ �����ϴ�
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