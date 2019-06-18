#version 330 core
out vec4 color;

in vec4 spriteColor;
in vec2 TexCoords;  

uniform sampler2D sprite;

void main()
{
    color =  ( spriteColor);
}