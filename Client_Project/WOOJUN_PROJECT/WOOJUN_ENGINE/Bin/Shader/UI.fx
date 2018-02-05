#include "Share.fx"

VS_TEX_OUTPUT UIVS(VS_TEX_INPUT input)
{
    VS_TEX_OUTPUT output = (VS_TEX_OUTPUT) 0;

    float3 vPos = input.vPos;
    float3 vPivot = g_vMeshMin + (g_vMeshSize * g_vPivot);

    vPos -= vPivot;

    output.vPos = mul(float4(vPos, 1.0f), g_matWVP);
    output.vUV = input.vUV;

    return output;
}

PS_OUTPUT UIPS(VS_TEX_OUTPUT input)
{
    PS_OUTPUT output = (PS_OUTPUT) 0;

    output.vTarget0 = g_DiffuseTexture.Sample(g_DiffuseSampler, input.vUV) * g_vMaterialDiffuse;

    return output;
}