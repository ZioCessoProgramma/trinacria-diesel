#version 330 core

layout(location = 0) in vec2 a_Pos;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in uint a_TextureIndex; // 0 = none
layout(location = 3) in vec2 a_TexCoords;
layout(location = 4) in uint a_FillTextureIndex; // 0 = none
layout(location = 5) in float a_Progress; // -1 = no progress bar
layout(location = 6) in vec4 a_FillColor;

out vec4 Color;
flat out uint TextureIndex;
out vec2 TexCoords;
flat out uint FillTextureIndex;
flat out float Progress;
out vec4 FillColor;

uniform mat4 u_Transform;

void main()
{
    Color = a_Color;
    TexCoords = a_TexCoords;
    TextureIndex = a_TextureIndex;
    FillTextureIndex = a_FillTextureIndex;
    Progress = a_Progress;
    FillColor = a_FillColor;

    gl_Position =  u_Transform * vec4(a_Pos, 0.f, 1.f);
}
