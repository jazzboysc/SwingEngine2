#version 430 core

in vec2 pTCoord;

out vec4[2] Output;

uniform sampler2D GBufferPositionSampler;
uniform sampler2D GBufferNormalSampler;

uniform vec4 TileInfo;  // x:tile width y:tile height z:m w:n

void main()
{
	ivec2 coords = ivec2(gl_FragCoord.xy);
	ivec2 tileWH = ivec2(TileInfo.xy);
	ivec2 mn = ivec2(TileInfo.zw);
	ivec2 ij = coords / tileWH;
	ivec2 ab = coords % tileWH;
	ivec2 uv = ab*mn + ij;

    vec4 position = texelFetch(GBufferPositionSampler, uv, 0);
    vec4 normal = texelFetch(GBufferNormalSampler, uv, 0);

	Output[0] = position;
	Output[1] = normal;
}
