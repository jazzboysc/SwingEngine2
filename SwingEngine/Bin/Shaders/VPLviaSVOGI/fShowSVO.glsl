#version 430 core

in vec4 boxColor;

out vec4 Output;

uniform bool IsLeafPass;

void main()
{
    if( IsLeafPass )
    {
        if( boxColor.g > 0.25 )
        {
            discard;
        }
    }
    else
    {
        if( boxColor.g < 0.5 )
        {
            discard;
        }
    }
    Output = boxColor;
}