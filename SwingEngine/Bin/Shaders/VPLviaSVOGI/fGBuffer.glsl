#version 430 core

in vec4 vNormalWorld;
in vec4 vPositionWorld;
in vec2 fTCoord;

out vec4[3] Output;

uniform vec3 MaterialColor;
uniform bool UseDiffuseMap;
uniform sampler2D DiffuseMapSampler;

void main()
{
    vec3 vNormal = normalize(vNormalWorld.xyz);
	vNormal.xyz = (vNormal.xyz + 1.0) * 0.5;

    vec3 albedo;
    if( UseDiffuseMap )
    {
        albedo = pow(texture(DiffuseMapSampler, fTCoord).rgb, vec3(2.2));
    }
    else
    {
        albedo = MaterialColor;
    }

    Output[0] = vPositionWorld;
    Output[1] = vec4(vNormal, 0.0);
    Output[2] = vec4(albedo.rgb, 1.0);
}
