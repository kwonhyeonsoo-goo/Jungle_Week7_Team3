#ifndef LIGHTING_COMMON_HLSLI
#define LIGHTING_COMMON_HLSLI

#include "CommonTypes.hlsli"

// LocalLights StructuredBuffer - t6 slot
StructuredBuffer<FLocalLightInfo> g_LightBuffer : register(t6);

float3 GetAmbientLightColor()
{
    return Ambient.Color * Ambient.Intensity;
}

// 특정 인덱스의 Directional Light 방향 반환
float3 GetDirectionalLightDirection(int Index)
{
    if (Index < NumDirectionalLights)
    {
        float3 Dir = Directional[Index].Direction;
        return (length(Dir) > 0.0001f) ? normalize(Dir) : float3(0, -1, 0);
    }
    return float3(0, -1, 0);
}

// 특정 인덱스의 Directional Light 색상 반환
float3 GetDirectionalLightColor(int Index)
{
    if (Index < NumDirectionalLights)
    {
        return Directional[Index].Color * Directional[Index].Intensity;
    }
    return float3(0, 0, 0);
}

float ComputeLambertTerm(float3 Normal, float3 L)
{
    // Normal과 L은 호출부에서 정규화되어 넘어온다고 가정 (성능 최적화)
    return saturate(dot(Normal, -L));
}

float4 ComputeGouraudLighting(float4 BaseColor, float4 GouraudL)
{
    return float4(BaseColor.rgb * GouraudL.rgb, BaseColor.a);
}

float3 ComputeGouraudLightingColor(float3 Normal, float3 WorldPosition)
{
    float3 N = normalize(Normal);
    float3 TotalLight = GetAmbientLightColor();

    for (int i = 0; i < NumDirectionalLights; ++i)
    {
        float3 L = normalize(Directional[i].Direction);
        float Diffuse = saturate(dot(N, -L));
        TotalLight += Diffuse * Directional[i].Color * Directional[i].Intensity;
    }

    for (int j = 0; j < NumLocalLights; ++j)
    {
        FLocalLightInfo LocalLight = g_LightBuffer[j];
        float3 LightVector = LocalLight.Position - WorldPosition;
        float Distance = length(LightVector);
        
        if (Distance < LocalLight.AttenuationRadius && LocalLight.AttenuationRadius > 0.001f)
        {
            float3 L = LightVector / Distance;
            float Diffuse = saturate(dot(N, L));
            float Attenuation = saturate(1.0f - (Distance / LocalLight.AttenuationRadius));
            Attenuation *= Attenuation;
            
            if (dot(LocalLight.Direction, LocalLight.Direction) > 0.0001f)
            {
                float3 SpotDir = normalize(LocalLight.Direction);
                float CosAngle = dot(-L, SpotDir);
                float CosInner = cos(radians(LocalLight.InnerConeAngle));
                float CosOuter = cos(radians(LocalLight.OuterConeAngle));
                Attenuation *= smoothstep(CosOuter, CosInner, CosAngle);
            }
            
            TotalLight += Diffuse * LocalLight.Color * LocalLight.Intensity * Attenuation;
        }
    }

    return saturate(TotalLight);
}

float3 ReconstructWorldPositionFromSceneDepth(float2 UV)
{
    float Depth = SceneDepth.Sample(PointClampSampler, UV).r;
    float4 Clip = float4(UV * 2.0f - 1.0f, Depth, 1.0f);
    Clip.y *= -1.0f;
    float4 World = mul(Clip, InvViewProj);
    return World.xyz / max(World.w, 0.0001f);
}

float4 ComputeLambertLighting(float4 BaseColor, float3 Normal)
{
    float3 N = normalize(Normal);
    float3 TotalLight = GetAmbientLightColor();

    for (int i = 0; i < NumDirectionalLights; ++i)
    {
        float3 L = normalize(Directional[i].Direction);
        TotalLight += saturate(dot(N, -L)) * Directional[i].Color * Directional[i].Intensity;
    }

    return float4(BaseColor.rgb * saturate(TotalLight), BaseColor.a);
}

float4 ComputeBlinnPhongLighting(float4 BaseColor, float3 Normal, float4 MaterialParam, float3 WorldPosition, float3 ViewDirection)
{
    float3 N = normalize(Normal);
    float3 TotalDiffuse = GetAmbientLightColor();
    float3 TotalSpecular = 0;

    float Shininess = max(MaterialParam.x, 1.0f);
    float SpecularStrength = max(MaterialParam.y, 0.0f);

    for (int i = 0; i < NumDirectionalLights; ++i)
    {
        float3 L = normalize(-Directional[i].Direction);
        float3 H = normalize(ViewDirection + L);

        float Diffuse = saturate(dot(N, L));
        float Specular = pow(saturate(dot(N, H)), Shininess) * SpecularStrength;

        float3 LightColor = Directional[i].Color * Directional[i].Intensity;
        TotalDiffuse += Diffuse * LightColor;
        TotalSpecular += Specular * LightColor;
    }

    return float4(BaseColor.rgb * saturate(TotalDiffuse) + TotalSpecular, BaseColor.a);
}

float3 LocalLightBlinnPhong(FLocalLightInfo LocalLight, float3 N, float3 WorldPosition, float3 V, float Shininess, float SpecularStrength)
{
    float3 LightVector = LocalLight.Position - WorldPosition;
    float Distance = length(LightVector);

    if (Distance >= LocalLight.AttenuationRadius || LocalLight.AttenuationRadius <= 0.001f)
        return 0;

    float3 L = LightVector / Distance;
    float3 H = normalize(V + L);

    float Diffuse = saturate(dot(N, L));
    float Specular = pow(saturate(dot(N, H)), Shininess) * SpecularStrength;

    float Attenuation = saturate(1.0f - (Distance / LocalLight.AttenuationRadius));
    Attenuation *= Attenuation;

    if (dot(LocalLight.Direction, LocalLight.Direction) > 0.0001f)
    {
        float3 SpotDir = normalize(LocalLight.Direction);
        Attenuation *= smoothstep(cos(radians(LocalLight.OuterConeAngle)), cos(radians(LocalLight.InnerConeAngle)), dot(-L, SpotDir));
    }

    float3 LightColor = LocalLight.Color * LocalLight.Intensity;
    return (Diffuse * LightColor + Specular * LightColor) * Attenuation;
}

float3 LocalLightLambert(FLocalLightInfo LocalLight, float3 N, float3 WorldPosition)
{
    float3 LightVector = LocalLight.Position - WorldPosition;
    float Distance = length(LightVector);

    if (Distance >= LocalLight.AttenuationRadius || LocalLight.AttenuationRadius <= 0.001f)
        return 0;

    float3 L = LightVector / Distance;
    float Diffuse = saturate(dot(N, L));
    float Attenuation = saturate(1.0f - (Distance / LocalLight.AttenuationRadius));
    Attenuation *= Attenuation;

    if (dot(LocalLight.Direction, LocalLight.Direction) > 0.0001f)
    {
        float3 SpotDir = normalize(LocalLight.Direction);
        Attenuation *= smoothstep(cos(radians(LocalLight.OuterConeAngle)), cos(radians(LocalLight.InnerConeAngle)), dot(-L, SpotDir));
    }

    return Diffuse * LocalLight.Color * LocalLight.Intensity * Attenuation;
}

// ============================================================
// Tile-Based Light Culling for Gouraud Vertex Shading
// ============================================================
#if defined(USE_LIGHT_CULLING)

// 타일당 최대 1024개 조명 → 32개 uint 비트마스크 버킷
#define TILE_LC_BUCKET_COUNT 32

// t7: LightCullingPass가 채운 타일별 조명 비트마스크
StructuredBuffer<uint> g_PerTileLightMask : register(t7);

// b5: LightCullingParams (b2의 StaticMeshMaterialBuffer와 충돌 방지를 위해 b5 사용)
cbuffer LightCullingParamsVS : register(b5)
{
    uint2  ScreenSize;
    uint2  TileSize;
    uint   Enable25DCulling;
    float  NearZ;
    float  FarZ;
    float  NumLightsLC;
};

// Gouraud Vertex Shader용 타일 기반 컬링 라이팅 계산
// ClipPos: VS에서 출력되는 SV_POSITION (perspective divide 이전의 클립 공간 좌표)
float3 ComputeGouraudLightingColorCulled(float3 Normal, float3 WorldPosition, float4 ClipPos)
{
    float3 N = normalize(Normal);
    float3 TotalLight = GetAmbientLightColor();

    // 방향광은 타일 컬링 없이 전체 기여
    for (int i = 0; i < NumDirectionalLights; ++i)
    {
        float3 L = normalize(Directional[i].Direction);
        float Diffuse = saturate(dot(N, -L));
        TotalLight += Diffuse * Directional[i].Color * Directional[i].Intensity;
    }

    // 로컬 라이트가 없거나 스크린 크기 미설정 시 조기 반환
    if (NumLocalLights <= 0 || ScreenSize.x == 0u || ScreenSize.y == 0u || ClipPos.w <= 0.0f)
        return saturate(TotalLight);

    // 클립 공간 → NDC → 스크린 픽셀 → 타일 좌표
    float2 NDC      = ClipPos.xy / ClipPos.w;
    float2 UV       = float2(NDC.x * 0.5f + 0.5f, -NDC.y * 0.5f + 0.5f);
    float2 PixelF   = UV * float2(ScreenSize);

    uint2 TileCoord;
    TileCoord.x = (uint)PixelF.x / TileSize.x;
    TileCoord.y = (uint)PixelF.y / TileSize.y;

    uint NumTilesX = (ScreenSize.x + TileSize.x - 1u) / TileSize.x;
    uint NumTilesY = (ScreenSize.y + TileSize.y - 1u) / TileSize.y;
    TileCoord.x    = min(TileCoord.x, NumTilesX - 1u);
    TileCoord.y    = min(TileCoord.y, NumTilesY - 1u);

    uint TileIndex  = TileCoord.y * NumTilesX + TileCoord.x;
    uint TileOffset = TileIndex * TILE_LC_BUCKET_COUNT;

    for (int j = 0; j < NumLocalLights; ++j)
    {
        uint BucketIdx = (uint)j / 32u;
        uint BitIdx    = (uint)j % 32u;

        if (BucketIdx >= TILE_LC_BUCKET_COUNT)
            break;

        // 타일 마스크에서 해당 조명 비트 확인 — 0이면 스킵
        if ((g_PerTileLightMask[TileOffset + BucketIdx] & (1u << BitIdx)) == 0u)
            continue;

        FLocalLightInfo LocalLight = g_LightBuffer[j];
        float3 LightVector = LocalLight.Position - WorldPosition;
        float  Distance    = length(LightVector);

        if (Distance >= LocalLight.AttenuationRadius || LocalLight.AttenuationRadius <= 0.001f)
            continue;

        float3 L           = LightVector / Distance;
        float  Diffuse     = saturate(dot(N, L));
        float  Attenuation = saturate(1.0f - Distance / LocalLight.AttenuationRadius);
        Attenuation *= Attenuation;

        if (dot(LocalLight.Direction, LocalLight.Direction) > 0.0001f)
        {
            float3 SpotDir  = normalize(LocalLight.Direction);
            float  CosAngle = dot(-L, SpotDir);
            float  CosInner = cos(radians(LocalLight.InnerConeAngle));
            float  CosOuter = cos(radians(LocalLight.OuterConeAngle));
            Attenuation *= smoothstep(CosOuter, CosInner, CosAngle);
        }

        TotalLight += Diffuse * LocalLight.Color * LocalLight.Intensity * Attenuation;
    }

    return saturate(TotalLight);
}
#endif // USE_LIGHT_CULLING

#endif