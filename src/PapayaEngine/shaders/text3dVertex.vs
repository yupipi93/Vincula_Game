#version 330 core
layout (location = 0) in vec4 vertex; // world position
out vec2 texCoords;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

void main()
{
    gl_Position = projection * view * model * vec4(vertex.xy, 0.0, 1.0);
    texCoords = vertex.zw;
}