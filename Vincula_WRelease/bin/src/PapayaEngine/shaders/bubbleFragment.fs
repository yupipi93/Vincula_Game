#version 330 core
out vec4 FragColor;

in vec2 texCoords;

// texture sampler
uniform sampler2D bubbleTexture;

void main()
{
	FragColor = texture(bubbleTexture, texCoords);
}