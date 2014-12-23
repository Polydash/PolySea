#version 150 core

in vec3 in_Vertex;

uniform mat4 mvp;

void main()
{
    gl_Position = mvp * vec4(in_Vertex, 1.0);
}
