#version 430 core

in vec2 pTCoord;

out vec4 Output;

uniform sampler2D SrcBufferSampler;
uniform sampler2D GBufferPositionSampler;
uniform sampler2D GBufferNormalSampler;

uniform vec2 Threshold; // x:position y:normal
uniform vec4 TileInfo;  // x:tile width y:tile height z:m w:n

void main()
{
	int m = int(TileInfo.z);
	int n = int(TileInfo.w);
	int offsetX = m / 2;
	int offsetY = n / 2;
	ivec2 p = ivec2(gl_FragCoord.xy);

	vec3 pos = texelFetch(GBufferPositionSampler, p, 0).xyz;
	vec3 normal = texelFetch(GBufferNormalSampler, p, 0).xyz;
	normal = normal*2.0 - 1.0;

	int count = 0;
	vec3 accumulation = vec3(0.0, 0.0, 0.0);
	for( int y = 0; y < n; ++y )
	{
		for( int x = 0; x < m; ++x )
		{
			int tx = p.x + x - offsetX;
			int ty = p.y + y - offsetY;

			vec3 pos2 = texelFetch(GBufferPositionSampler, ivec2(tx, ty), 0).xyz;
			vec3 normal2 = texelFetch(GBufferNormalSampler, ivec2(tx, ty), 0).xyz;
			normal2 = normal2*2.0 - 1.0;

			vec3 dir = pos2 - pos;
			float a = dot(dir, dir);
			float b = dot(normal, normal2);

            if( a < Threshold.x && b > Threshold.y )
			{
				vec3 color = texelFetch(SrcBufferSampler, ivec2(tx, ty), 0).rgb;
				accumulation += color;
				count++;
			}
		}
	}

	accumulation /= count;
	Output = vec4(accumulation, 1.0);
}
