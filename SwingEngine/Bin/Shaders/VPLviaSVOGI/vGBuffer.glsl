#version 430 core

in vec4 vPosition;
in vec3 vNormal;
in vec2 vTCoord;

out vec4 vNormalWorld;
out vec4 vPositionWorld;
out vec2 fTCoord;

uniform mat4 World;
uniform mat4 View;
uniform mat4 Proj;

void main()
{
    vPositionWorld = World * vPosition;
    vNormalWorld = World * vec4(vNormal, 0.0);
    fTCoord = vTCoord;
    gl_Position = Proj * View * vPositionWorld;
}
