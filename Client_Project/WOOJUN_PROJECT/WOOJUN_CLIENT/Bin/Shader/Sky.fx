
cbuffer Transform : register(b0)
{
    matrix g_matWorld;
    matrix g_matView;
    matrix g_matProj;
    matrix g_matWV;
    matrix g_matWVP;
}

struct VS_INPUT
{
    float3 vPos : POSITION;
};

struct VS_OUTPUT
{
    float4 vPos : SV_POSITION;
    float3 vLocalPos : POSITION;
};

struct PS_OUTPUT
{
    float4 vTarget : SV_Target;
};

// Diffuse Texture
TextureCube g_DiffuseTex : register(t0);
SamplerState g_DiffuseSmp : register(s0);

VS_OUTPUT SkyVS(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;

    output.vPos = mul(float4(input.vPos, 1.f), g_matWVP).xyww;
    output.vLocalPos = input.vPos;

    return output;
}

PS_OUTPUT SkyPS(VS_OUTPUT input)
{
    PS_OUTPUT output = (PS_OUTPUT) 0;

    output.vTarget = g_DiffuseTex.Sample(g_DiffuseSmp, input.vLocalPos);

    return output;
}
