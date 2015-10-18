#version 430 core

#include "VPLviaSVOGI/sVirtualPointLight.glsl"
#include "VPLviaSVOGI/sSparseVoxelOctree.glsl"

in vec2 pTCoord;

out vec4 Output;

uniform int VPLCount;
uniform int PatternSize;
uniform float BounceSingularity;
uniform vec3 SceneBBCenter;
uniform vec3 SceneBBExtension;
uniform int dim;

uniform sampler2D GBufferPositionSampler;
uniform sampler2D GBufferNormalSampler;

void main()
{
	vec4 PositionWorld = texelFetch(GBufferPositionSampler, ivec2(gl_FragCoord.xy), 0);
    if( PositionWorld.w == 0.0 )
    {
        discard;
    }

	vec3 NormalWorld = texelFetch(GBufferNormalSampler, ivec2(gl_FragCoord.xy), 0).xyz;
    NormalWorld = NormalWorld*2.0 - 1.0;

	ivec2 tileWH = textureSize(GBufferPositionSampler, 0).xy / PatternSize;
	ivec2 ij = ivec2(gl_FragCoord.xy) / tileWH;
	int patternIndex = ij.x + ij.y*PatternSize;
    int sampleVPLCount = VPLCount / (PatternSize*PatternSize);
    int vplBufferIndex = sampleVPLCount * patternIndex;

    // Initialize SVO root node.
    SVONode root;
    root.info = SVO_NODE_FLAG_MASK;
    root.nodeBox.Min = Ivec3ToUint(ivec3(0, 0, 0));
    root.nodeBox.Max = Ivec3ToUint(ivec3(SVO_MAX_LEVEL_DIM,
        SVO_MAX_LEVEL_DIM, SVO_MAX_LEVEL_DIM));

    vec3 indirectColor = vec3(0.0, 0.0, 0.0);
    for( int i = 0; i < sampleVPLCount; ++i )
    {
        // Fetch VPL.
        VPL vpl = VPLBuffer.vpls[vplBufferIndex + i];

		// Ignore this VPL if its luminance is too small.
		float luminance = 0.2126*vpl.Flux.b + 0.7152*vpl.Flux.b + 0.0722*vpl.Flux.b;

        vpl.WorldNormal = vpl.WorldNormal*2.0 - 1.0;

        vec3 incidentDir = PositionWorld.xyz - vpl.WorldPosition.xyz;
        float len = length(incidentDir);
        incidentDir = incidentDir / len;

        // Compute G term.
        float cos0 = max(0.0, dot(NormalWorld, -incidentDir));
        float cos1 = max(0.0, dot(incidentDir, vpl.WorldNormal.xyz));
        float geometricTerm = cos0 * cos1 / max(len*len, BounceSingularity);

        if( luminance > VPL_LUMINANCE_EPSILON && geometricTerm > 0.0 )
		{
			// VPL visibility test.
			uint vTerm = SVOIntersectionTest(PositionWorld.xyz, vpl.WorldPosition.xyz, root);

			// Accumulate VPL's contribution.
			indirectColor += vpl.Flux.rgb * geometricTerm * (1 - vTerm);
		}
    }

    indirectColor = indirectColor * 2.0 * PI * VPL_INTENSITY / sampleVPLCount;
    Output = vec4(indirectColor, 1.0);
}
