#pragma once
#include "Engine_Header.h"

WOOJUN_BEGIN

typedef struct _tagSimpleVertex
{
	XMFLOAT3 Pos;
}SimpleVertex;

class DLL CDevice
{
private:
	// 1. ID3D11Device �������̽��� ��� ���� ���˰� �ڿ� �Ҵ翡 ���δ�.
	ID3D11Device*				m_pDevice;				// ����̽� ������
	// 2. ID3D11DeviceContext �������̽��� ���� ����� �����ϰ�,
	//    �ڿ��� �׷��� ���������ο� ����,
	//    GPU�� ������ ������ ��ɵ��� �����ϴ� �� ���δ�.
	ID3D11DeviceContext*		m_pDeviceContext;		// ����̽� ���ؽ�Ʈ ������
	IDXGISwapChain*				m_pSwapChain;			// ����ü�� ������
	ID3D11RenderTargetView*		m_pRenderTargetView;	// ����Ÿ�ٺ� ������
	ID3D11Texture2D*			m_pDepthBuffer;			// ���� ����
	ID3D11DepthStencilView*		m_pDepthView;			// ���� ��
	float						m_fClearColor[4];		// ���� �迭

	ID3D11VertexShader*			m_pVS;
	ID3D11PixelShader*			m_pPS;
	ID3D11InputLayout*			m_pVertexLayout;
	D3D11_INPUT_ELEMENT_DESC	layout[2];
public:
	inline ID3D11Device* GetDevice() const {
		return m_pDevice;
	}
	inline ID3D11DeviceContext* GetDeviceContext() const {
		return m_pDeviceContext;
	}
	inline IDXGISwapChain* GetSwapChain() const {
		return m_pSwapChain;
	}
public:
	void SetClearColor(float _fColor[4]);
public:
	bool Init(HWND _hWnd, unsigned int _iWidth, unsigned int _iHeight, bool _IsWindowMode);
	void Clear();	
	void Present();

	DECLARE_SINGLE(CDevice)
};

WOOJUN_END
