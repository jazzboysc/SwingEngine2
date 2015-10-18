#version 430 core

#include "VPLviaSVOGI/sTempResult.glsl"

in vec2 pTCoord;
out vec4 Output;

uniform bool UseToneMapper;
uniform sampler2D tempSampler;

void main()
{
    int level = 0;
    ivec2 levelRes = textureSize(tempSampler, level);
    ivec2 pos = ivec2(pTCoord.xy * vec2(levelRes));

    vec3 result = texelFetch(tempSampler, pos, level).rgb;
    if( UseToneMapper )
    {
        const float gamma = 2.2;
        const float exposure = 2.0;
        result = HDRToneMapping(result, gamma, exposure);
    }
    Output = vec4(result, 1.0);
}