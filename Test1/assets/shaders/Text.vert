#version 330 core

layout(location = 0) in vec2 a_Pos;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in uint a_TexIndex;

uniform mat4 u_View;

out vec4 Color;
out vec2 TexCoord;
flat out uint TexIndex;

void main() {
    gl_Position = u_View * vec4(a_Pos, 0.f, 1.f);

    Color = a_Color;
    TexCoord = a_TexCoord;
    TexIndex = a_TexIndex;
}
