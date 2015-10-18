#version 430 core

#include "VPLviaSVOGI/sSceneLight.glsl"

in vec4 gPositionWorld;
in vec4 gNormalWorld;
in vec2 fTCoord;

out vec4[3] Output;

uniform vec3 MaterialColor;
uniform bool UseDiffuseMap;
uniform sampler2D DiffuseMap;

void main()
{
    // Fetch light info.
    uint curLightIndex = rsmUniformBuffer.info.CurLightIndex;
    SESceneLight curLight = sceneLightUniformBuffer.lights[curLightIndex];

    vec3 normal = normalize(gNormalWorld.xyz);
    normal.xyz = (normal.xyz + 1.0) * 0.5;

    vec3 diffuse;
    if( UseDiffuseMap )
    {
        diffuse = pow(texture(DiffuseMap, fTCoord).rgb, vec3(2.2));
    }
    else
    {
        diffuse = MaterialColor;
    }

    Output[0] = gPositionWorld;
    Output[1] = vec4(normal, 0.0);
    Output[2] = vec4(diffuse.rgb*curLight.Intensity.rgb, 1.0);
}