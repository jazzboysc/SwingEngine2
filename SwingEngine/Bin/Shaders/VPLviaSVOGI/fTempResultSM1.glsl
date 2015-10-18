#version 430 core

#include "VPLviaSVOGI/sTempResult.glsl"
#include "VPLviaSVOGI/sVirtualPointLight.glsl"

in vec2 pTCoord;
out vec4 Output;

uniform int TextureArrayIndex;
uniform int CurMipmapLevel;
uniform bool IsShadowMap;
uniform bool UseToneMapper;
uniform bool ShowImportance;
uniform float MaxRadiance;

uniform sampler2DArray tempSamplerArray;

void main()
{
    if( IsShadowMap )
    {
        float depth = texture(tempSamplerArray, vec3(pTCoord, TextureArrayIndex)).r;
        Output = vec4(depth, depth, depth, 1.0);
    }
    else
    {
		ivec3 tSize = textureSize(tempSamplerArray, CurMipmapLevel);
		vec3 result = texelFetch(tempSamplerArray, ivec3(ivec2(pTCoord.xy*tSize.xy), TextureArrayIndex), CurMipmapLevel).rgb;

		if( ShowImportance )
		{
			float importance = ComputeImportance(result);
			float mappedimportance = pow(min(importance / 50.0, 1.0), 1.0 / 2.2);
			result.rgb = vec3(mappedimportance);
		}
		else
		{
			if( UseToneMapper )
			{
				float greyRadiance = max(0.001, 0.3 * result.r + 0.6 * result.g + 0.1 * result.b);
				float mappedRadiance = pow(min(greyRadiance / MaxRadiance, 1.0), 1.0 / 2.2);
				result.rgb = (mappedRadiance / greyRadiance) * result.rgb;
			}
		}

        Output = vec4(result, 1.0);
    }
}