#version 430 core

layout(vertices = 3) out;

in vec4 vNormalWorld[];
out vec4 tcNormalWorld[];

uniform float TessLevel;

void main(void)
{
    gl_TessLevelOuter[0] = TessLevel;
    gl_TessLevelOuter[1] = TessLevel;
    gl_TessLevelOuter[2] = TessLevel;
    gl_TessLevelInner[0] = TessLevel;

    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
    tcNormalWorld[gl_InvocationID] = vNormalWorld[gl_InvocationID];
}
