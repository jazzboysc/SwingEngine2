#version 430 core

#include "VPLviaSVOGI/sVirtualPointLight.glsl"

// imageLoad function in vertex shader is broken since NVIDIA 350.12 driver.
//layout (binding = 0, rgba32f) uniform image1D VPLSamplePattern;

uniform int VPLCount;
uniform int RSMFluxMipmapMaxLevel;

uniform sampler2DArray RSMPosition;
uniform sampler2DArray RSMNormal;
uniform sampler2DArray RSMFlux;
uniform sampler1D      VPLSamplePattern;

void main()
{
	// Fetch sample.
    // imageLoad function in vertex shader is broken since NVIDIA 350.12 driver.
    //vec4 samplePosition = imageLoad(VPLSamplePattern, int(gl_VertexID));
    vec3 samplePosition = texture(VPLSamplePattern, float(gl_VertexID) / float(VPLCount)).rgb;

	VPL tempVPL;

	// Randomly choose a RSM to sample.
    ivec3 rsmSize = textureSize(RSMPosition, 0);
    int textureIndex = int(samplePosition.z * rsmSize.z);

	// Perform RSM importance sampling.

	int prevLevelDim = 1;
	int curMipmapLevel = RSMFluxMipmapMaxLevel;
	for( ; curMipmapLevel >= 0; curMipmapLevel-- )
	{
		ivec2 prevLevelStartXY = ivec2(samplePosition.xy*float(prevLevelDim));
		ivec2 curLevelStartXY = prevLevelStartXY * 2;
		float curLevelWarpExtension = 1.0 / float(prevLevelDim);
        float curLevelPixelExtension = curLevelWarpExtension*0.5;
        vec2 curLevelWarpStartXY = vec2(curLevelStartXY)*curLevelPixelExtension;

		// Transform sample point from texture space to warp space.
		samplePosition.xy = (samplePosition.xy - curLevelWarpStartXY) / curLevelWarpExtension;

		// Get importance values for the current warp space.
		vec3 flux00 = texelFetch(RSMFlux, ivec3(curLevelStartXY.x, curLevelStartXY.y, textureIndex), curMipmapLevel).rgb;
		vec3 flux10 = texelFetch(RSMFlux, ivec3(curLevelStartXY.x + 1, curLevelStartXY.y, textureIndex), curMipmapLevel).rgb;
		vec3 flux01 = texelFetch(RSMFlux, ivec3(curLevelStartXY.x, curLevelStartXY.y + 1, textureIndex), curMipmapLevel).rgb;
		vec3 flux11 = texelFetch(RSMFlux, ivec3(curLevelStartXY.x + 1, curLevelStartXY.y + 1, textureIndex), curMipmapLevel).rgb;
		float importance00 = ComputeImportance(flux00);
		float importance10 = ComputeImportance(flux10);
		float importance01 = ComputeImportance(flux01);
		float importance11 = ComputeImportance(flux11);

		// Normalize importance values.
		float invSum = 1.0 / (importance00 + importance10 + importance01 + importance11);
        importance00 *= invSum;
        importance10 *= invSum;
        importance01 *= invSum;
        importance11 *= invSum;

		// Vertical and horizontal warps performed in warp space.
		float upperWeight = importance01 + importance11;
		float lowerWeight = importance00 + importance10;
		if( samplePosition.y >= lowerWeight )
		{
			samplePosition.y = (samplePosition.y - lowerWeight)*0.5 / upperWeight + 0.5;

			float leftWeight = importance01 / (importance01 + importance11);
			float rightWeight = 1.0 - leftWeight;

			if( samplePosition.x >= leftWeight )
			{
				samplePosition.x = (samplePosition.x - leftWeight)*0.5 / rightWeight + 0.5;
			}
			else
			{
				samplePosition.x = samplePosition.x*0.5 / leftWeight;
			}
		}
		else
		{
			samplePosition.y = samplePosition.y*0.5 / lowerWeight;

			float leftWeight = importance00 / (importance00 + importance10);
			float rightWeight = 1.0 - leftWeight;

			if( samplePosition.x >= leftWeight )
			{
				samplePosition.x = (samplePosition.x - leftWeight)*0.5 / rightWeight + 0.5;
			}
			else
			{
				samplePosition.x = samplePosition.x*0.5 / leftWeight;
			}
		}

		// Transform the sample back to texture space.
		samplePosition.xy = samplePosition.xy*curLevelWarpExtension + curLevelWarpStartXY;

		prevLevelDim = prevLevelDim * 2;
	}

	// Fetch VPL data.
	tempVPL.WorldPosition = texture(RSMPosition, vec3(samplePosition.x, samplePosition.y, textureIndex));
	tempVPL.WorldNormal = texture(RSMNormal, vec3(samplePosition.x, samplePosition.y, textureIndex));
	tempVPL.Flux = texture(RSMFlux, vec3(samplePosition.x, samplePosition.y, textureIndex));

	// Calculate luminance for this VPL.
	tempVPL.Flux.a = 0.2126*tempVPL.Flux.b + 0.7152*tempVPL.Flux.b + 0.0722*tempVPL.Flux.b;

    vec3 normal = tempVPL.WorldNormal.xyz*2.0 - 1.0;
    tempVPL.WorldPosition.xyz = tempVPL.WorldPosition.xyz + normal*0.000001;  // Avoid self-intersection.

    vec3 E = tempVPL.WorldPosition.xyz;
    vec3 D = -normal;
    tempVPL.View = GetVPLViewTransform(E, D);

    VPLBuffer.vpls[gl_VertexID] = tempVPL;
}