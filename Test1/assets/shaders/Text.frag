#version 330 core

out vec4 Color;
out vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D u_TextAtlases[MAX_TEXTURE_SLOTS];