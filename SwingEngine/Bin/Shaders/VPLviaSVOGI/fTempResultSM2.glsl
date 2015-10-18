#version 430 core

#include "VPLviaSVOGI/sTempResult.glsl"

in vec2 pTCoord;
out vec4 Output;

uniform sampler2D tempSampler;
uniform sampler2D tempSampler2;
uniform sampler2D GBufferAlbedoSampler;

void main()
{
	vec3 indirectColor = texture(tempSampler, pTCoord).rgb;
	vec3 directColor = texture(tempSampler2, pTCoord).rgb;
	vec3 albedo = texture(GBufferAlbedoSampler, pTCoord).rgb;

    const float gamma = 2.2;
    const float exposure = 2.0;
	vec3 result = indirectColor*albedo + directColor;
    vec3 mapped = HDRToneMapping(result, gamma, exposure);
    Output = vec4(mapped, 1.0);
}