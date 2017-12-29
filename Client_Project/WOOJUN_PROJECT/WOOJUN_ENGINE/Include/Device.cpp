#include "Device.h"

WOOJUN_USING

DEFINITION_SINGLE(CDevice)

void CDevice::SetClearColor(float _fColor[4])
{
	memcpy_s(m_fClearColor, sizeof(m_fClearColor), _fColor, sizeof(_fColor));
}

bool CDevice::Init(HWND _hWnd, unsigned int _iWidth, unsigned int _iHeight, bool _IsWindowMode)
{
	unsigned int iCreateFlag = 0;

#ifdef _DEBUG
	iCreateFlag |= D3D11_CREATE_DEVICE_DEBUG;
#endif // _DEBUG
	D3D_FEATURE_LEVEL eFeatureLevel = (D3D_FEATURE_LEVEL)0;

	DXGI_SWAP_CHAIN_DESC tSwapChain = {};
	tSwapChain.BufferDesc.Width = _iWidth;
	tSwapChain.BufferDesc.Height = _iHeight;
	tSwapChain.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	tSwapChain.BufferDesc.RefreshRate.Numerator = 60;
	tSwapChain.BufferDesc.RefreshRate.Denominator = 1;
	tSwapChain.BufferCount = 1;
	tSwapChain.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	tSwapChain.OutputWindow = _hWnd;
	tSwapChain.SampleDesc.Count = 1;
	tSwapChain.SampleDesc.Quality = 0;
	tSwapChain.Windowed = _IsWindowMode;

	// Device 및 DeviceContext 생성
	if (FAILED(D3D11CreateDeviceAndSwapChain(0, D3D_DRIVER_TYPE_HARDWARE, 0, iCreateFlag, 0, 0, D3D11_SDK_VERSION,
		&tSwapChain, &m_pSwapChain, &m_pDevice, &eFeatureLevel, &m_pDeviceContext)))
	{		
		return false;
	}

	if (eFeatureLevel != D3D_FEATURE_LEVEL_11_0)
	{
		return false;
	}

	ID3D11Texture2D* pBackBuffer = nullptr;

	m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer);

	m_pDevice->CreateRenderTargetView(pBackBuffer, 0, &m_pRenderTargetView);

	SAFE_RELEASE(pBackBuffer);

	// 깊이ㆍ스텐실 버퍼 생성
	D3D11_TEXTURE2D_DESC tDesc = {};
	tDesc.Width = _iWidth;
	tDesc.Height = _iHeight;
	tDesc.MipLevels = 1;
	tDesc.ArraySize = 1;
	tDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	tDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	tDesc.SampleDesc.Count = 1;
	tDesc.SampleDesc.Quality = 0;
	tDesc.Usage = D3D11_USAGE_DEFAULT;
	tDesc.CPUAccessFlags = 0;
	tDesc.MiscFlags = 0;

	if (FAILED(m_pDevice->CreateTexture2D(&tDesc, 0, &m_pDepthBuffer)))
	{
		return false;
	};

	if (FAILED(m_pDevice->CreateDepthStencilView(m_pDepthBuffer, 0, &m_pDepthView)))
	{
		return false;
	}

	// 뷰를 렌더타겟에 묶기
	m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthView);
	
	// 뷰포트 설정
	D3D11_VIEWPORT tViewPort = { };
	tViewPort.Width = (float)_iWidth;
	tViewPort.Height = (float)_iHeight;
	tViewPort.MaxDepth = 1.0f;

	m_pDeviceContext->RSSetViewports(1, &tViewPort);

	return true;
}

void CDevice::Clear()
{
	// 후면 버퍼를 지운다
	m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, m_fClearColor);

	// 깊이 버퍼를 1.0f로, 스텐실 버퍼를 0으로 지운다
	m_pDeviceContext->ClearDepthStencilView(m_pDepthView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void CDevice::Present()
{
	// 후면 버퍼를 화면에 제시한다
	m_pSwapChain->Present(0, 0);
}

CDevice::CDevice()
	: m_pDevice(nullptr),
	m_pDeviceContext(nullptr),
	m_pSwapChain(nullptr),
	m_pRenderTargetView(nullptr),
	m_pDepthBuffer(nullptr),
	m_pDepthView(nullptr)
{
	memset(m_fClearColor, 0, sizeof(m_fClearColor) * 4);
	m_fClearColor[2] = 1.0f;
}


CDevice::~CDevice()
{
	SAFE_RELEASE(m_pDepthView);
	SAFE_RELEASE(m_pDepthBuffer);

	SAFE_RELEASE(m_pRenderTargetView);
	SAFE_RELEASE(m_pSwapChain);

	if (nullptr != m_pDeviceContext)
	{
		m_pDeviceContext->ClearState();
	}

	SAFE_RELEASE(m_pDeviceContext);
	SAFE_RELEASE(m_pDevice);
}