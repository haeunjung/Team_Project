#include "Share.fx"

SamplerState g_LightBlendSampler : register(s11);

Texture2D g_GBufferAlbedo : register(t11);
Texture2D g_LightDiffuse : register(t12);
Texture2D g_LightSpecular : register(t13);

static const float2 vPos[4] =
{
    float2(-1.f, 1.f),
	float2(1.f, 1.f),
	float2(-1.f, -1.f),
	float2(1.f, -1.f)
};

static const float2 vUV[4] =
{
    float2(0.f, 0.f),
	float2(1.f, 0.f),
	float2(0.f, 1.f),
	float2(1.f, 1.f)
};

VS_TEX_OUTPUT LightBlendVS(uint iVertexID : SV_VertexID)
{
    VS_TEX_OUTPUT output = (VS_TEX_OUTPUT) 0;

    output.vPos = float4(vPos[iVertexID], 0.f, 1.f);
    output.vUV = vUV[iVertexID];

    return output;
}

PS_OUTPUT LightBlendPS(VS_TEX_OUTPUT input)
{
    PS_OUTPUT output = (PS_OUTPUT) 0;

	// Albedo와 Diffuse, Specular  합성을 해야 한다.
    float4 vAlbedo = g_GBufferAlbedo.Sample(g_LightBlendSampler, input.vUV);

    if (vAlbedo.a == 0.f)
        clip(-1);

    float4 vDif = g_LightDiffuse.Sample(g_LightBlendSampler, input.vUV);
    float4 vSpc = g_LightSpecular.Sample(g_LightBlendSampler, input.vUV);
    
    output.vTarget0 = vAlbedo * vDif + vSpc;
    output.vTarget0.a = 1.f;

    return output;
}

PS_OUTPUT LightBlendOutputPS(VS_TEX_OUTPUT input)
{
    PS_OUTPUT output = (PS_OUTPUT) 0;

    float4 vAlbedo = g_GBufferAlbedo.Sample(g_LightBlendSampler, input.vUV);

    if (vAlbedo.a == 0.f)
        clip(-1);

    output.vTarget0 = vAlbedo;

    return output;
}