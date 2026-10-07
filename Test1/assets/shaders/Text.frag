#version 330 core

in vec4 Color;
in vec2 TexCoord;
flat in uint TexIndex;

out vec4 FragColor;

uniform sampler2D u_Textures[MAX_TEXTURE_SLOTS];

float median(float r, float g, float b)
{
    return max(min(r, g), min(max(r, g), b));
}

void main()
{
    vec4 color = SampleTexture(int(TexIndex), TexCoord);

    if(median(color.r, color.g, color.b) >= 0.5f)
    {
        FragColor = Color;
    }
    else
    {
        discard;
    }
}