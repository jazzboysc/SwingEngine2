#version 430 core

in vec2 pTCoord;

out vec4 Output;

uniform sampler2D SplittedBufferSampler;
uniform vec4 TileInfo;  // x:tile width y:tile height z:m w:n

void main()
{
	vec2 splittedBufferWH = textureSize(SplittedBufferSampler, 0).xy;
	ivec2 xy = ivec2(gl_FragCoord.xy);
    ivec2 mn = ivec2(TileInfo.zw);
    ivec2 wh = ivec2(TileInfo.xy);
	ivec2 ij = xy % mn;
	ivec2 ab = xy / mn;
	ivec2 uv = ij*wh + ab;

    Output = texelFetch(SplittedBufferSampler, uv, 0);
}
