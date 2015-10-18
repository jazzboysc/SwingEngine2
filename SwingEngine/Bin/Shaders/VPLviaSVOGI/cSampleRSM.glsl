#version 430 core

#include "VPLviaSVOGI/sVirtualPointLight.glsl"

layout(local_size_x = 1) in;

layout (binding = 0, rgba32f) uniform image1D VPLSamplePattern;

uniform sampler2DArray RSMPosition;
uniform sampler2DArray RSMNormal;
uniform sampler2DArray RSMFlux;

void main()
{
    vec4 sampleValue = imageLoad(VPLSamplePattern, int(gl_GlobalInvocationID.x));

    ivec3 rsmSize = textureSize(RSMPosition, 0);
	ivec3 p = ivec3(sampleValue.xyz * vec3(rsmSize));

    VPL tempVPL;
    tempVPL.WorldPosition = texelFetch(RSMPosition, p, 0);
	tempVPL.WorldNormal   = texelFetch(RSMNormal, p, 0);
	tempVPL.Flux          = texelFetch(RSMFlux, p, 0);

    vec3 normal = tempVPL.WorldNormal.xyz*2.0 - 1.0;
    tempVPL.WorldPosition.xyz = tempVPL.WorldPosition.xyz + normal*0.000001;  // Avoid self-intersection.

    vec3 E = tempVPL.WorldPosition.xyz;
    vec3 D = -normal;
    tempVPL.View = GetVPLViewTransform(E, D);

    VPLBuffer.vpls[gl_GlobalInvocationID.x] = tempVPL;
}