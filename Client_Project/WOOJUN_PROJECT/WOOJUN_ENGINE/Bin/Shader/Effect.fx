#include "Share.fx"

cbuffer Effect : register(b11)
{
    float3  g_vCenter;
    float   g_fEmpty1;
    float3  g_vCamAxisY;
    float   g_fEmpty2;
    float3  g_vCamPos;
    float   g_fEmpty3;
    float2  g_vSize;
    float2  g_fEmpty4;
}

cbuffer Animation2D : register(b12)
{
    int     g_iAniType;
    int     g_iAniFrameX;
    int     g_iAniFrameY;
    int     g_iAniFrameMaxX;
    int     g_iAniFrameMaxY;
    float3  vEmpty;
}

struct VS_INPUT_POS
{
    float3  vPos    : POSITION;
};

struct VS_OUTPUT_POS
{
    float3  vPos    : POSITION;
};

struct GS_OUTPUT
{
    float4  vPos    : SV_Position;
    float2  vUV     : TEXCOORD;
    uint    iPrimID : SV_PrimitiveID;
};

struct PS_OUTPUT
{
    float4  vTarget0 : SV_Target;
};

Texture2DArray g_EffectTexArray : register(t11);

float2 ComputeAtlasUV(float2 vCurUV)
{
    float2 vUV = (float2) 0;

    vUV.x = (g_iAniFrameX + vCurUV.x) / g_iAniFrameMaxX;
    vUV.y = (g_iAniFrameY + vCurUV.y) / g_iAniFrameMaxY;

    return vUV;
}

VS_OUTPUT_POS EffectVS(VS_INPUT_POS input)
{
    VS_OUTPUT_POS     output = (VS_OUTPUT_POS) 0;

    output.vPos = input.vPos;

    return output;
}

[maxvertexcount(4)]
void EffectGS(point VS_OUTPUT_POS input[1], 
    uint iPrimID : SV_PrimitiveID, 
    inout TriangleStream<GS_OUTPUT> TriStream)
{
    float3 vView = g_vCamPos - g_vCenter;
    vView = normalize(vView);

    float3 vUp = float3(0.0f, 1.0f, 0.0f);
    float3 vRight = cross(vUp, vView);
    vRight = normalize(vRight);

    vUp = cross(vView, vRight);
    vUp = normalize(vUp);

    float fHalfX = g_vSize.x * 0.5f;
    float fHalfY = g_vSize.y * 0.5f;

    float4 vPos[4];
    vPos[0] = float4(g_vCenter + fHalfX * vRight - fHalfY * vUp, 1.0f);
    vPos[1] = float4(g_vCenter + fHalfX * vRight + fHalfY * vUp, 1.0f);
    vPos[2] = float4(g_vCenter - fHalfX * vRight - fHalfY * vUp, 1.0f);
    vPos[3] = float4(g_vCenter - fHalfX * vRight + fHalfY * vUp, 1.0f);

    float2 vUV[4] =
    {
        float2(0.0f, 1.0f),
        float2(0.0f, 0.0f),
        float2(1.0f, 1.0f),
        float2(1.0f, 0.0f)
    };

    // Atlas
    if (1 == g_iAniType)
    {
        for (int i = 0; i < 4; ++i)
        {
            vUV[i] = ComputeAtlasUV(vUV[i]);
        }
    }

    GS_OUTPUT output;

    [unroll]
    for (int i = 0; i < 4; ++i)
    {
        output.vPos = mul(vPos[i], g_matVP);
        output.vUV = vUV[i];
        output.iPrimID = iPrimID;

        TriStream.Append(output);
    }
}

PS_SINGLE_OUTPUT EffectPS(GS_OUTPUT input)
{
    PS_SINGLE_OUTPUT output = (PS_SINGLE_OUTPUT) 0;

    if (2 == g_iAniType)
    {
        float3 vUV = float3(input.vUV, g_iAniFrameX);
        output.vTarget0 = g_EffectTexArray.Sample(g_DiffuseSampler, vUV);
    }
    else
    {
        output.vTarget0 = g_DiffuseTexture.Sample(g_DiffuseSampler, input.vUV);
    }        

    return output;
}
