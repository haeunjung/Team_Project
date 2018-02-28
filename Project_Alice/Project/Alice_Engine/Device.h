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
	// 1. ID3D11Device 인터페이스는 기능 지원 점검과 자원 할당에 쓰인다.
	ID3D11Device*				m_pDevice;				// 디바이스 포인터
	// 2. ID3D11DeviceContext 인터페이스는 렌더 대상을 설정하고,
	//    자원을 그래픽 파이프라인에 묶고,
	//    GPU가 수행할 렌더링 명령들을 지시하는 데 쓰인다.
	ID3D11DeviceContext*		m_pDeviceContext;		// 디바이스 컨텍스트 포인터
	IDXGISwapChain*				m_pSwapChain;			// 스왑체인 포인터
	ID3D11RenderTargetView*		m_pRenderTargetView;	// 렌더타겟뷰 포인터
	ID3D11Texture2D*			m_pDepthBuffer;			// 깊이 버퍼
	ID3D11DepthStencilView*		m_pDepthView;			// 깊이 뷰
	float						m_fClearColor[4];		// 배경색 배열

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
