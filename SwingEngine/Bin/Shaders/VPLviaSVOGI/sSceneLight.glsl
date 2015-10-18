#define MAX_LIGHT_COUNT 256
#define RSM_POINT_LIGHT_FACE_COUNT 6
#define SHADOWMAP_COMPARISON_BIAS 0.005

struct SELightBufferHead
{
    uint PointLightCount;
    uint SpotLightCount;
    uint Reserved1;
    uint Reserved2;
};

struct ShadowMapInfo
{
    uint CurLightIndex;
    uint Reserved1;
    uint Reserved2;
    uint Reserved3;
};

struct RSMInfo
{
    uint CurLightIndex;
    uint CurRSMBaseIndex;
    uint Reserved1;
    uint Reserved2;
};

//----------------------------------------------------------------------------
// Scene Light. Currently support point and spot lights.
//----------------------------------------------------------------------------
struct SESceneLight
{
    vec4 WorldPositionAndType; // type 1: point light, type 2: spot light
    vec4 Intensity;

    vec4 Params1; // SpotDirection, CosCutoff
    vec4 Params2; // SpotExponent, ConstantAttenuation, QuadraticAttenuation, SpotInnerCosCutoff

    mat4 LightProjectorView;
    mat4 LightProjectorProj;
    vec4 LightProjectorNearFar;
};

layout(std140, binding = 1) uniform _sceneLightUniformBuffer
{
    SELightBufferHead info;
    SESceneLight lights[MAX_LIGHT_COUNT];
} sceneLightUniformBuffer;

layout(std140, binding = 2) uniform _shadowMapUniformBuffer
{
    ShadowMapInfo info;
} shadowMapUniformBuffer;

layout(std140, binding = 3) uniform _rsmUniformBuffer
{
    RSMInfo info;
} rsmUniformBuffer;

uniform bool ShowShadow;
uniform sampler2DArray ShadowMapSampler;

//----------------------------------------------------------------------------
float ComputeShadowMapPCF(int i, sampler2DArray shadowMapSampler, vec3 pos)
{
    float res = 0.0;

    // TODO:
    // Instead of using constant values, try to adjust them base on dxdz and dydz.
    int extY = 6;
    int extX = 6;
    int dY = 2;
    int dX = 2;

    ivec2 wh = textureSize(shadowMapSampler, 0).xy;
    ivec2 xy = ivec2(pos.xy * wh);
    int count = 0;
    for( int offsetY = -extY; offsetY <= extY; offsetY += dY )
    {
        for( int offsetX = -extX; offsetX <= extX; offsetX += dX )
        {
            ivec2 samplePos = xy + ivec2(offsetX, offsetY);
            float depth = texelFetch(shadowMapSampler, ivec3(samplePos, i), 0).r;

            if( (pos.z - depth) > SHADOWMAP_COMPARISON_BIAS )
            {
                res += 1.0;
            }

            count++;
        }
    }
    res /= count;

    return res;
}
//----------------------------------------------------------------------------
vec4 ComputePointLight(int i, vec4 PositionWorld, vec3 NormalWorld, vec4 Material)
{
    vec4 res;

    SESceneLight light = sceneLightUniformBuffer.lights[i];

    // Compare with shadow map bias.
    bool skipShadow = false;
    vec4 viewPos = light.LightProjectorView * PositionWorld;
    if( viewPos.z > -0.05 )
    {
        skipShadow = true;
    }
    vec3 viewDir = viewPos.xyz;
    float len = length(viewDir);
    viewDir = normalize(viewDir);
    vec3 halfDir = viewDir + vec3(0.0, 0.0, -1.0);
    float u = -halfDir.x / halfDir.z;
    float v = -halfDir.y / halfDir.z;
    float currDepth = (len - light.LightProjectorNearFar.x) /
        (light.LightProjectorNearFar.y - light.LightProjectorNearFar.x);

    vec2 texCoords = vec2(u, v);
    texCoords = texCoords*0.5 + 0.5;
    float depth = texture(ShadowMapSampler, vec3(texCoords, float(i))).r;

    // Shadow map test.
    if( (currDepth - depth) > 0.01 && !skipShadow && ShowShadow )
    {
        // In Shadow.
        res = vec4(0.0, 0.0, 0.0, 1.0);
    }
    else
    {
        // Direct lighting.
        vec3 lightDir = light.WorldPositionAndType.xyz - PositionWorld.xyz;
        float len = length(lightDir);
        lightDir = lightDir / len;
        float d = max(0.0, dot(lightDir, NormalWorld));
        vec3 color = Material.rgb * light.Intensity.xyz * d / (1.0 + 0.2*len*len);
        res = vec4(color, 1.0);
    }

    return res;
}
//----------------------------------------------------------------------------
vec4 ComputeSpotLight(int i, vec4 PositionWorld, vec3 NormalWorld, vec4 Material)
{
    vec4 res = vec4(0.0, 0.0, 0.0, 1.0);

    SESceneLight light = sceneLightUniformBuffer.lights[i];

    vec3 lightDir = light.WorldPositionAndType.xyz - PositionWorld.xyz;
    float len = length(lightDir);
    lightDir = lightDir / len;

    float attenuation = 0.0;
    float spotEffect = dot(light.Params1.xyz, -lightDir);
    float spotCosCutoff = light.Params1.w;
    float spotInnerCosCutoff = light.Params2.w;
    float spotExponent = light.Params2.x;
    float constantAttenuation = light.Params2.y;
    float quadraticAttenuation = light.Params2.z;

    if( spotEffect >= spotCosCutoff )
    {
        // Shadow map test.

        vec4 viewPos = light.LightProjectorView * PositionWorld;
        float currDepth = (viewPos.z - light.LightProjectorNearFar.x) /
            (light.LightProjectorNearFar.y - light.LightProjectorNearFar.x);

        vec4 projPos = light.LightProjectorProj * viewPos;
        projPos.xy /= projPos.w;
        projPos.xy = projPos.xy*0.5 + 0.5;
        projPos.z = currDepth;

        float pcfTerm = 0.0;
        if( ShowShadow )
        {
            pcfTerm = ComputeShadowMapPCF(i, ShadowMapSampler, projPos.xyz);
        }

        // Direct lighting.

        float falloff = clamp((spotEffect - spotInnerCosCutoff) /
            (spotInnerCosCutoff - spotCosCutoff), 0.0, 1.0);

        attenuation = falloff * pow(spotEffect, spotExponent) / 
            (constantAttenuation + quadraticAttenuation*len*len);
        float d = max(0.0, dot(lightDir, NormalWorld));
        res.rgb = Material.rgb * light.Intensity.xyz * attenuation * d * (1.0 - pcfTerm);
    }

    return res;
}
//----------------------------------------------------------------------------