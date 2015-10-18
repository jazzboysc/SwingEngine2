#version 430 core

#include "VPLviaSVOGI/sSceneLight.glsl"

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec4 vPositionWorld[];
in vec4 vNormalWorld[];
in vec2 gTCoord[];

out vec4 gPositionWorld;
out vec4 gNormalWorld;
out vec2 fTCoord;

void main()
{
    uint curLightIndex = rsmUniformBuffer.info.CurLightIndex;
    uint curRSMBaseIndex = rsmUniformBuffer.info.CurRSMBaseIndex;

    for( int i = 0; i < gl_in.length(); ++i )
    {
        gPositionWorld = vPositionWorld[i];
        gNormalWorld = vNormalWorld[i];
        fTCoord = gTCoord[i];
        gl_Position = gl_in[i].gl_Position;

        gl_Layer = int(curRSMBaseIndex + curLightIndex);
        EmitVertex();
    }

    EndPrimitive();
}