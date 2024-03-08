#version 330 core

in vec2 aPos;

out vec2 TexCoord;

uniform mat4 model;
uniform mat4 projection;

void main()
{
    gl_Position = projection * model * vec4(aPos, 0.0, 1.0);
    TexCoord = vec2((aPos.x + 1.0)/2.0, 1 - (aPos.y + 1.0)/2.0);
}