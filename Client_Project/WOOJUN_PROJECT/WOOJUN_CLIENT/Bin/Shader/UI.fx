#include "Share.fx"

// Tex Vertex Input Structure
struct VS_TEX_INPUT
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
};

struct VS_TEX_OUTPUT
{
    float4 vPos : SV_POSITION;
    float2 vUV : TEXCOORD;
};


VS_TEX_OUTPUT UIVS(VS_TEX_INPUT input)
{
    VS_TEX_OUTPUT output = (VS_TEX_OUTPUT) 0;

    output.vPos = mul(float4(input.vPos, 1.f), g_matWVP);
    output.vUV = input.vUV;

    return output;
}

PS_SINGLE_OUTPUT UIPS(VS_TEX_OUTPUT input)
{
    PS_SINGLE_OUTPUT output = (PS_SINGLE_OUTPUT) 0;

    output.vTarget0 = g_DiffuseTexture.Sample(g_DiffuseSampler, input.vUV) * g_vMaterialDiffuse;

    return output;
}