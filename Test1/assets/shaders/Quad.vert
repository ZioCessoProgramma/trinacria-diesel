#version 330 core

layout(location = 0) in vec2 a_Pos;
layout(location = 1) in int a_TextureIndex;
layout(location = 2) in vec2 a_TexCoords;
layout(location = 3) in vec4 a_Color;
layout(location = 4) in int a_MaterialIndex;

uniform mat4 u_View;

out vec2 TexCoords;
flat out int Index;
out vec4 Color;
out vec2 FragPos;
flat out int MaterialIndex;

void main()
{
	gl_Position = u_View * vec4(a_Pos, 0.f, 1.f);

	TexCoords = a_TexCoords;
	Index = a_TextureIndex;
	Color = a_Color;
	FragPos = a_Pos;
	MaterialIndex = a_MaterialIndex;
}