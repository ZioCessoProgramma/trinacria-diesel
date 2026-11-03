#version 330 core

uniform sampler2D u_ColorAttachment;

in vec2 FragPos;

out vec4 FragColor;

void main()
{
    FragColor = texture(u_ColorAttachment, FragPos * 0.5f + 0.5f);
}
