// Color Vertex Input Structure
struct VS_COLOR_INPUT
{
    float3 vPos : POSITION;
    float4 vColor : COLOR;
};

// Color Vertex Output Structure
struct VS_COLOR_OUTPUT
{
    float4 vPos : SV_POSITION;
	// SV는 System Value의 약자로 
	// System Value가 되면 건드리면 안된다.
    float4 vColor : COLOR;
};

// Tex Vertex Input Structure
struct VS_TEX_INPUT
{
    float3 vPos : POSITION;
    float3 vUV : TEXCOORD;
};

struct VS_TEX_OUTPUT
{
    float4 vPos : SV_POSITION;
    float2 vUV : TEXCOORD;
};

// Texture Normal Vertex
struct VS_TEXTURENORMAL_INPUT
{
    float3 vPos : POSITION;
    float3 vNormal : NORMAL;
    float2 vUV : TEXCOORD;
};

struct VS_TEXTURENORMAL_ANIM_INPUT
{
    float3 vPos : POSITION;
    float3 vNormal : NORMAL;
    float2 vUV : TEXCOORD;
    float4 vWeights : BLENDWEIGHTS;
    float4 vIndices : BLENDINDICES;
};

struct VS_TEXTURENORMAL_OUTPUT
{
    float4 vPos : SV_POSITION;
    float3 vNormal : NORMAL;
    float2 vUV : TEXCOORD;
    float4 vProjPos : POSITION;
};

// Texture Bump Vertex
struct VS_BUMP_INPUT
{
    float3 vPos : POSITION;
    float3 vNormal : NORMAL;
    float2 vUV : TEXCOORD;
    float3 vTangent : TANGENT;
    float3 vBinormal : BINORMAL;
};

struct VS_ANI_BUMP_INPUT
{
    float3 vPos : POSITION;
    float3 vNormal : NORMAL;
    float2 vUV : TEXCOORD;
    float3 vTangent : TANGENT;
    float3 vBinormal : BINORMAL;
    float4 vWeights : BLENDWEIGHTS;
    float4 vIndices : BLENDINDICES;
};

struct VS_BUMP_OUTPUT
{
    float4 vPos : SV_POSITION;
    float3 vNormal : NORMAL;
    float2 vUV : TEXCOORD;
    float4 vProjPos : POSITION;
    float3 vTangent : TANGENT;
    float3 vBinormal : BINORMAL;
};

// Single Target Output Structure
struct PS_OUTPUT
{
    float4 vTarget0 : SV_Target;
    float4 vTarget1 : SV_Target1;
    float4 vTarget2 : SV_Target2;
    float4 vTarget3 : SV_Target3;
};

// 상수버퍼
// 쉐이더 외부에서 값을 넘겨줄때 사용
cbuffer Transform : register(b0)
{
    matrix g_matWorld;
    matrix g_matView;
    matrix g_matProj;
    matrix g_matWV;
    matrix g_matWVP;
    matrix g_matVP;
    matrix g_matInvProj;
    matrix g_matInvView;
    matrix g_matInvVP;

    float3 g_vPivot;
    float g_Empty1;
    float3 g_vMeshSize;
    float g_Empty2;
    float3 g_vMeshMin;
    float g_Empty3;
    float3 g_vMeshMax;
    float g_Empty4;
}

cbuffer Material : register(b1)
{
    float4 g_vMaterialDiffuse;
    float4 g_vMaterialAmbient;
    float4 g_vMaterialSpecular;
    float4 g_vMaterialEmissive;

    int g_iBump;
    int g_iSpecular;
    float g_fSpecularPower;
    int g_iMaterialEmpty;
}

cbuffer Light : register(b2)
{
    float4 g_vLightDiffuse;
    float4 g_vLightAmbient;
    float4 g_vLightSpecular;
    int g_iLightType;
    float3 g_vLightDir;
    float3 g_vLightPos;
    float g_fLightRange;
    float3 g_vAttenuation;
    float g_fSpot;
}

// Diffuse Texture
Texture2D g_DiffuseTexture : register(t0);
SamplerState g_DiffuseSampler : register(s0);

// Normal Texture
Texture2D g_NormalTexture : register(t1);
SamplerState g_NormalSampler : register(s1);

// Specular Texture
Texture2D g_SpecularTexture : register(t2);
SamplerState g_SpecularSampler : register(s2);

// Bone Texture
Texture2D g_BoneTexture : register(t3);
SamplerState g_BoneSampler : register(s3);

struct _tagMaterial
{
    float4 vDiffuse;
    float4 vAmbient;
    float4 vSpecular;
};

float3 ComputeNormal(float3 vTangent, float3 vBinormal, float3 vVtxNormal, float2 vUV)
{
    float4 vNormalMap = g_NormalTexture.Sample(g_NormalSampler, vUV);
    float3 vNormal = vNormalMap.xyz * 2.0f - 1.0f;

    // 기저벡터를 이용하여 탄젠트공간의 법선을 뷰공간으로 만들어준다.
    float3x3 mat = float3x3(vTangent, vBinormal, vVtxNormal);

    // Tangent, Binormal, Normal은 버텍스 셰이더에서 View 공간으로 변환된 축정도블이다.
    // 이 축정보를 이용해서 행렬을 아래처럼 구성한다.
    // Tx     Bx     Nx
    // Ty     By     Ny
    // Tz     Bz     Nz
    // 이렇게 구성하면 뷰공간에 있는 어느 벡터를 탄젠트 공간으로 변환하는 행렬이 만들어진다.
    // NormalMap에 저장된 법선 정보는 탄젠트공간에 존재하는 법선들이다.
    // 이 법선정보를 뷰공간으로 바꾸려면 위 행렬의 역행렬이 필요하다.
    // 직교행렬의 역행렬은 전치행렬과 같기때문에
    // Tx     Ty     Tz
    // Bx     By     Bz
    // Nx     Ny     Nz
    // 위와 같은 행렬이라고 볼 수 있다.
    // NormalMap에서 뽑아온 벡터를 위 행렬에 곱하게 되면 탄젠트공간에 있던 법선 정보가 뷰공간으로 변환된다.

    vNormal = mul(vNormal, mat);
    vNormal = normalize(vNormal);

    return vNormal;
}

_tagMaterial ComputeLight(float3 vNormal, float3 vViewPos, float2 vUV)
{
    _tagMaterial tMtrl = (_tagMaterial) 0;

	// 조명 타입에 따라 조명의 방향을 구하고 뷰공간으로 변환한다.
    float3 vLightDir = (float3) 0;
    float3 vLightPos = (float3) 0;
    float fAtt = (float) 1.0f;
    float fDist = (float) 0;
    float fSpot = (float) 1.0f;
	// 방향성 조명일 경우
    if (g_iLightType == 0)
    {
        vLightDir = mul(float4(g_vLightDir, 0.f), g_matView).xyz;
        vLightDir = normalize(vLightDir);
    }

	// 점 조명일 경우
    if (g_iLightType == 1)
    {
        // 조명의 위치를 뷰공간으로 바꾼다.
        vLightPos = mul(float4(g_vLightPos, 1.0f), g_matView).xyz;
        vLightDir = vLightPos - vViewPos;

        fDist = length(vLightDir);

        vLightDir = normalize(vLightDir);

        if (fDist > g_fLightRange)
            return tMtrl;
        else
        {
            fAtt = 1.0f / dot(g_vAttenuation.xyz, float3(1.0f, fDist, fDist * fDist));
        }
    }

	// Spot
    if (g_iLightType == 2)
    {
        vLightPos = mul(float4(g_vLightPos, 1.0f), g_matView).xyz;
        vLightDir = vLightPos - vViewPos;
        fDist = length(vLightDir);
        
        if (fDist > g_fLightRange)
        {
            vLightDir = float4(0.0f, 0.0f, 0.0f, 0.0f).xyz;
        }

        vLightDir /= fDist;
        float3 vLight = -mul(float4(g_vLightDir, 0.0f), g_matView).xyz;

        fSpot = pow(max(dot(vLightDir, vLight), 0.0f), g_fSpot);
        fAtt = fSpot / dot(g_vAttenuation, float3(1.0f, fDist, fDist * fDist));
    }

	// Diffuse를 구한다.
    tMtrl.vDiffuse = g_vLightDiffuse * g_vMaterialDiffuse * max(0, dot(vNormal, vLightDir)) * fAtt;
    tMtrl.vAmbient = g_vLightAmbient * g_vMaterialAmbient * fSpot;

	// 정반사광을 구한다.
	// 반사벡터를 구해준다.
    float3 vReflect = 2.f * vNormal * dot(vNormal, vLightDir) - vLightDir;
    vReflect = normalize(vReflect);

	// 정점에서 카메라를 향하는 벡터를 만든다.
    float3 vView = -normalize(vViewPos);

    float4 vSpecular = g_vMaterialSpecular;

    if (1 == g_iSpecular)
    {
        vSpecular.rgb = g_SpecularTexture.Sample(g_SpecularSampler, vUV).rgb;
    }

    tMtrl.vSpecular = g_vLightSpecular * vSpecular * pow(max(0, dot(vReflect, vView)), g_fSpecularPower) * fAtt;

    return tMtrl;
}

struct _tagSkinning
{
    float3 vPos;
    float3 vNormal;
    float3 vTangent;
    float3 vBinormal;
};

matrix GetBoneMatrix(int idx)
{
    matrix matBone =
    {
        g_BoneTexture.Load(int3(idx * 4, 0, 0)),
        g_BoneTexture.Load(int3(idx * 4 + 1, 0, 0)),
        g_BoneTexture.Load(int3(idx * 4 + 2, 0, 0)),
        g_BoneTexture.Load(int3(idx * 4 + 3, 0, 0))
    };

    return matBone;
}

_tagSkinning Skinning(float3 vPos, float3 vNormal, float3 vTangent, float3 vBinormal, float4 vWeights, float4 vIndices)
{
    _tagSkinning tSkinning = (_tagSkinning) 0;

    float fWeights[4];
    fWeights[0] = vWeights.x;
    fWeights[1] = vWeights.y;
    fWeights[2] = vWeights.z;
    fWeights[3] = 1.0f - vWeights.x - vWeights.y - vWeights.z;

    for (int i = 0; i < 4; ++i)
    {
        matrix matBone = GetBoneMatrix((int) vIndices[i]);

        tSkinning.vPos += fWeights[i] * mul(float4(vPos, 1.0f), matBone).xyz;
        tSkinning.vNormal += fWeights[i] * mul(float4(vNormal, 0.0f), matBone).xyz;
        tSkinning.vTangent += fWeights[i] * mul(float4(vTangent, 0.0f), matBone).xyz;
        tSkinning.vBinormal += fWeights[i] * mul(float4(vBinormal, 0.0f), matBone).xyz;
    }

    tSkinning.vNormal = normalize(tSkinning.vNormal);
    tSkinning.vTangent = normalize(tSkinning.vTangent);
    tSkinning.vBinormal = normalize(tSkinning.vBinormal);

    return tSkinning;
}

_tagSkinning Skinning(float3 vPos, float3 vNormal, float4 vWeights, float4 vIndices)
{
    _tagSkinning tSkinning = (_tagSkinning) 0;

    float fWeights[4];
    fWeights[0] = vWeights.x;
    fWeights[1] = vWeights.y;
    fWeights[2] = vWeights.z;
    fWeights[3] = 1.0f - vWeights.x - vWeights.y - vWeights.z;

    for (int i = 0; i < 4; ++i)
    {
        matrix matBone = GetBoneMatrix((int) vIndices[i]);

        tSkinning.vPos += fWeights[i] * mul(float4(vPos, 1.0f), matBone).xyz;
        tSkinning.vNormal += fWeights[i] * mul(float4(vNormal, 0.0f), matBone).xyz;
    }

    tSkinning.vNormal = normalize(tSkinning.vNormal);

    return tSkinning;
}

int ColorToPixel(float4 vColor)
{
    int r, g, b, a;
    r = vColor.r * 255;
    g = vColor.g * 255;
    b = vColor.b * 255;
    a = vColor.a * 255;
	
    int iPixel = r;
    iPixel <<= 8;
    iPixel |= g;
    iPixel <<= 8;
    iPixel |= b;
    iPixel <<= 8;
    iPixel |= a;

    return iPixel;
}

float4 PixelToColor(float fPixel)
{
    float4 vColor;
    int iPixel = (int) fPixel;

    vColor.a = iPixel & 0x000000ff;
    iPixel >>= 8;
    vColor.b = iPixel & 0x000000ff;
    iPixel >>= 8;
    vColor.g = iPixel & 0x000000ff;
    iPixel >>= 8;
    vColor.r = iPixel & 0x000000ff;

    return vColor;
}