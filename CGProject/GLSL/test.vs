#version 330 core
layout (location = 0) in vec4 vertex; // <vec2 position, vec2 texCoords>

out vec4 spriteColor;
out vec2 TexCoords;

uniform mat4 projection;
uniform mat4 view;
uniform vec4 color;
uniform vec2 offset;

void main()
{
	spriteColor = color;
	TexCoords = vertex.zw;
	float scale = 1.0f;
    gl_Position = projection * view * vec4((vertex.xy * scale) + offset,  0.0, 1.0);
}