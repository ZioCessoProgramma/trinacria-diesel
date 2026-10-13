#version 330 core

layout(location = 0) in vec3 a_Pos;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in uint a_TexIndex;

uniform mat4 u_View;
uniform mat4 u_Transform;

out vec4 Color;
out vec2 TexCoord;
flat out uint TexIndex;

void main() {
    if(a_Pos.z > 0)
        gl_Position = u_Transform * u_View * vec4(a_Pos.xy, 0.f, 1.f);
    else
        gl_Position = u_Transform * vec4(a_Pos.xy, 0.f, 1.f);

    Color = a_Color;
    TexCoord = a_TexCoord;
    TexIndex = a_TexIndex;
}
