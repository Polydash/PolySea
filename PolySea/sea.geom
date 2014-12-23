#version 150 core

layout (triangles) in;
layout (triangle_strip, max_vertices=3) out;

in vec3 worldPos[3];
out vec3 normal;

void main()
{
	normal = cross(worldPos[1] - worldPos[0], worldPos[2] - worldPos[0]);
	normal = normalize(normal);

	for(int i=0; i<gl_in.length(); ++i)
	{
		gl_Position = gl_in[i].gl_Position;
		EmitVertex();
	}

	EndPrimitive();
}