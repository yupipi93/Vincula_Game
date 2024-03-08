#version 330 core
out vec4 FragColor;
  
uniform vec4 objectColor;
in vec2 TexCoord;

uniform sampler2D ourTexture;

void main()
{
    FragColor = texture(ourTexture, TexCoord) * objectColor;
}