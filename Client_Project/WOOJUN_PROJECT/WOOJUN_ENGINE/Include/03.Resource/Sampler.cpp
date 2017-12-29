#include "Sampler.h"
#include "../Device.h"

WOOJUN_USING

bool CSampler::CreateSampler(const string& _strKey,
	D3D11_FILTER _eFilter /*= D3D11_FILTER_MIN_MAG_MIP_LINEAR*/,
	D3D11_TEXTURE_ADDRESS_MODE _eAddrU /*= D3D11_TEXTURE_ADDRESS_WRAP*/,
	D3D11_TEXTURE_ADDRESS_MODE _eAddrV /*= D3D11_TEXTURE_ADDRESS_WRAP*/,
	D3D11_TEXTURE_ADDRESS_MODE _eAddrW /*= D3D11_TEXTURE_ADDRESS_WRAP*/)
{
	D3D11_SAMPLER_DESC	tDesc = {};

	tDesc.Filter = _eFilter;
	tDesc.AddressU = _eAddrU;
	tDesc.AddressV = _eAddrV;
	tDesc.AddressW = _eAddrW;

	if (FAILED(DEVICE->CreateSamplerState(&tDesc, &m_pSampler)))
	{
		return false;
	}

	return true;
}

void CSampler::SetSampler(int _iRegister, int _iShaderType)
{
	if (_iShaderType & CUT_VERTEX)
	{
		CONTEXT->VSSetSamplers(_iRegister, 1, &m_pSampler);
	}
	if (_iShaderType & CUT_PIXEL)
	{
		CONTEXT->PSSetSamplers(_iRegister, 1, &m_pSampler);
	}
}

CSampler::CSampler() :
	m_pSampler(NULL)
{
}

CSampler::~CSampler()
{
	SAFE_RELEASE(m_pSampler);
}
