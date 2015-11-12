struct VertexOut
{
    float4 PosH  : SV_POSITION;
    float4 Color : COLOR;
};

float4 main(VertexOut input) : SV_TARGET
{
    return input.Color;
}