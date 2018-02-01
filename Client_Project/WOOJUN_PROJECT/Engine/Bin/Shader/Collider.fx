#include "Share.fx"

// 나만의 상수버퍼 - 그래봐야 컬러값
cbuffer ColliderColor : register(b10)
{
    float4 g_vColor;
}

VS_COLOR_OUTPUT ColliderVS(VS_COLOR_INPUT input)
{
    VS_COLOR_OUTPUT output = (VS_COLOR_OUTPUT) 0;

    output.vPos = mul(float4(input.vPos, 1.0f), g_matWVP);
    output.vColor = g_vColor;

    return output;
}

PS_OUTPUT ColliderPS(VS_COLOR_OUTPUT input)
{
    PS_OUTPUT output = (PS_OUTPUT) 0;
        
    output.vTarget0 = input.vColor;

    return output;
}