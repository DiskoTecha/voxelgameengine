#version 430 core

out vec4 FragColor;

in vec3 ourColor;
in vec2 texCoord;

uniform sampler2D imageTex;

void main()
{
	FragColor = texture(imageTex, texCoord);
}