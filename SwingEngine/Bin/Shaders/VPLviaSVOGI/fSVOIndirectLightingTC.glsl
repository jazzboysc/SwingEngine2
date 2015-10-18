#version 430 core

#include "VPLviaSVOGI/sVirtualPointLight.glsl"
#include "VPLviaSVOGI/sSparseVoxelOctree.glsl"

in vec2 pTCoord;

layout(binding = 0, rgba32f) uniform image2D IndirectLightingBuffer;

uniform int VPLCount;
uniform int PatternSize;
uniform float BounceSingularity;
uniform sampler2D GBufferPositionSampler;
uniform sampler2D GBufferNormalSampler;
uniform sampler2D GBufferAlbedoSampler;
uniform vec3 SceneBBCenter;
uniform vec3 SceneBBExtension;
uniform int dim;
uniform int FrameCounter;

void main()
{
    ivec2 fragmentCoords = ivec2(int(gl_FragCoord.x), int(gl_FragCoord.y));

    vec4 PositionWorld = texture(GBufferPositionSampler, pTCoord);
    if( PositionWorld.w == 0.0 )
    {
        imageStore(IndirectLightingBuffer, fragmentCoords, vec4(0.0, 0.0, 0.0, 1.0));
        return;
    }

    vec3 NormalWorld = texture(GBufferNormalSampler, pTCoord).rgb;
    NormalWorld = NormalWorld*2.0 - 1.0;

    vec4 MaterialColor = texture(GBufferAlbedoSampler, pTCoord);

    int patternIndex = fragmentCoords.x % PatternSize + (fragmentCoords.y % PatternSize) * PatternSize;
    int patternLen = PatternSize*PatternSize;
    int sampleVPLCount = VPLCount / patternLen;
    int vplBufferIndex = sampleVPLCount * patternIndex;

    // Initialize SVO root node.
    SVONode root;
    root.info = SVO_NODE_FLAG_MASK;
    root.nodeBox.Min = Ivec3ToUint(ivec3(0, 0, 0));
    root.nodeBox.Max = Ivec3ToUint(ivec3(SVO_MAX_LEVEL_DIM,
        SVO_MAX_LEVEL_DIM, SVO_MAX_LEVEL_DIM));

    //int vplBufferFrameOffset = (FrameCounter - 1) % 4;
    vec3 indirectColor = vec3(0.0, 0.0, 0.0);
    for( int i = 0; i < sampleVPLCount; ++i )
    {
        // Fetch VPL.
        VPL vpl = VPLBuffer.vpls[vplBufferIndex + i];
        //VPL vpl = VPLBuffer.vpls[vplBufferIndex + 4 * i + vplBufferFrameOffset];

        // VPL visibility test.
        uint vTerm = SVOIntersectionTest(PositionWorld.xyz, vpl.WorldPosition.xyz, root);

        vpl.WorldNormal = vpl.WorldNormal*2.0 - 1.0;

        vec3 incidentDir = PositionWorld.xyz - vpl.WorldPosition.xyz;
        float len = length(incidentDir);
        incidentDir = incidentDir / len;

        // Compute G term.
        float cos0 = max(0.0, dot(NormalWorld, -incidentDir));
        float cos1 = max(0.0, dot(incidentDir, vpl.WorldNormal.xyz));
        float geometricTerm = cos0 * cos1 / max(len*len, BounceSingularity);

        // Accumulate VPL's contribution.
        indirectColor += vpl.Flux.rgb * geometricTerm * (1 - vTerm);
    }

    indirectColor = (MaterialColor.rgb * indirectColor * 2.0 * PI * VPL_INTENSITY) / sampleVPLCount;
    //vec3 cacheData = imageLoad(IndirectLightingBuffer, fragmentCoords).xyz;
    //indirectColor = 0.4*indirectColor + 0.6*cacheData;
    imageStore(IndirectLightingBuffer, fragmentCoords, vec4(indirectColor, 1.0));
}
