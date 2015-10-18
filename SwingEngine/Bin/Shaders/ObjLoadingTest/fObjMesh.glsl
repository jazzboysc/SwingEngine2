#version 430 core

in vec4 vPositionWorld;
in vec4 vNormalWorld;
in vec2 fTCoord;

uniform sampler2D diffuseMap;
uniform int ShowMode;

void main()
{
    vec3 normal = normalize(vNormalWorld).xyz;
	//vec3 normal = cross(dFdx(vPositionWorld.xyz), dFdy(vPositionWorld.xyz));
	//normal = normalize(normal);
	normal = normal*0.5 + 0.5;
    vec3 diffuseColor = texture(diffuseMap, fTCoord).rgb;

	if( ShowMode == 0 )
	{
		gl_FragData[0] = vec4(normal, 1.0);
	}
	else if( ShowMode == 1 )
	{
		gl_FragData[0] = vec4(diffuseColor, 1.0);
		
	}
	else if( ShowMode == 2 )
	{
		gl_FragData[0] = vec4(fTCoord, 0.0, 1.0);
	}
}
