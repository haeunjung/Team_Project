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
    float4 vPos : SV_Position;
    float2 vUV : TEXCOORD;
    float4 vProjPos : POSITION;
    uint iPrimID : SV_PrimitiveID;
};

Texture2DArray g_EffectTexArray : register(t7);
texture2D g_GBufferDepth : register(t13);
SamplerState g_GBufferDepthSmp : register(s11);

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
        output.vProjPos = mul(vPos[i], g_matVP);
        output.vUV = vUV[i];
        output.iPrimID = iPrimID;
        output.vPos = output.vProjPos;

        TriStream.Append(output);
    }
}

PS_OUTPUT EffectPS(GS_OUTPUT input)
{
    PS_OUTPUT output = (PS_OUTPUT) 0;

    if (2 == g_iAniType)
    {
        float3 vUV = float3(input.vUV, g_iAniFrameX);
        output.vTarget0 = g_EffectTexArray.Sample(g_DiffuseSampler, vUV);
    }
    else
    {
        output.vTarget0 = g_DiffuseTexture.Sample(g_DiffuseSampler, input.vUV);
    }        

    if (output.vTarget0.a == 0.0f)
        clip(-1);

    // 깊이값을 얻어온다.
	// GBufferDepth에서 깊이를 얻어오기 위한 투영좌표를 이용한 UV 좌표를 구해준다.
    float2 vDepthUV;
    vDepthUV.x = input.vProjPos.x / input.vProjPos.w * 0.5f + 0.5f;
    vDepthUV.y = input.vProjPos.y / input.vProjPos.w * -0.5f + 0.5f;
    float fDepth = g_GBufferDepth.Sample(g_GBufferDepthSmp, vDepthUV).w;
    float fSrcDepth = input.vProjPos.w;

    float fAlpha = (fDepth - fSrcDepth) / 0.5f;

    if (fAlpha > 1.f)
        fAlpha = 1.f;
    else if (fAlpha < 0.f)
        fAlpha = 0.f;

    if (fDepth == 0.f)
        fAlpha = 1.f;

    output.vTarget0.a *= fAlpha;

    return output;
}
