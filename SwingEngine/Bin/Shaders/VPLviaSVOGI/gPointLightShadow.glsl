#version 430 core

#include "VPLviaSVOGI/sSceneLight.glsl"

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in float teViewPosZ[];
in float teDepth[];

out float gViewPosZ;
out float gDepth;

void main()
{
    uint curLightIndex = shadowMapUniformBuffer.info.CurLightIndex;

    for( int i = 0; i < gl_in.length(); ++i )
    {
        gViewPosZ = teViewPosZ[i];
        gDepth = teDepth[i];
        gl_Position = gl_in[i].gl_Position;

        gl_Layer = int(curLightIndex);
        EmitVertex();
    }

    EndPrimitive();
}