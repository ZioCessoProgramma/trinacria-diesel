#version 330 core

layout(location = 0) in vec2 a_Pos;

out vec2 FragPos;

void main()
{
    gl_Position = vec4(a_Pos, 0.f, 1.f);

    FragPos = a_Pos;
}
