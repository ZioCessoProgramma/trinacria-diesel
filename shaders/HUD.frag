#version 330 core

in vec4 Color;
out vec4 FragColor;
flat in uint TextureIndex;
in vec2 TexCoords;
flat in uint FillTextureIndex;
flat in float Progress;
in vec4 FillColor;

uniform sampler2D u_Textures[MAX_TEXTURE_SLOTS];

void main()
{
    if(int(TextureIndex) == 0)
    {
        FragColor = Color;
    }
    else
    {
        vec4 fillTexColor = SampleTexture(int(FillTextureIndex), TexCoords);

        if(fillTexColor.rgb == vec3(1.f, 0.f, 1.f))
        {
            if (TexCoords.x <= Progress && Progress != -1.f)
            {
                FragColor = FillColor;
            }
            else
            {
                FragColor = SampleTexture(int(TextureIndex), TexCoords);
            }
        }
        else
        {
            FragColor = SampleTexture(int(TextureIndex), TexCoords);
        }

    }
}
